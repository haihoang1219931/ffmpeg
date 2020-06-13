/*
 * Read video frame with FFmpeg and convert to OpenCV image
 *
 * Copyright (c) 2016 yohhoy
 */
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QObject>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <QAudioFormat>
#include <QAudioOutput>
#include <QBuffer>

// FFmpeg
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/pixdesc.h>
#include <libswscale/swscale.h>
}
// OpenCV
#include <opencv2/opencv.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>
using namespace std;
using namespace cv;
CascadeClassifier face_cascade;
void detectAndDisplay( Mat &frame ,std::vector<Rect>& faces){
    Mat frame_gray;
    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );
    //-- Detect faces
    face_cascade.detectMultiScale( frame_gray, faces );
}

class Klv{
public:
    Klv(uint8_t key, uint8_t length, std::vector<uint8_t> value){
        m_key = key;
        m_length = length;
        m_value = value;
    }
    ~Klv(){}

public:
    uint8_t m_key;
    uint8_t m_length;
    std::vector<uint8_t> m_value;
};
void parseKlv(Klv klv){
    switch (klv.m_key) {
    case 0x02:
        if(klv.m_length >=8){
            uint64_t timestamp =  0;
            const uint16_t dummy = 1;
            const bool is_little_endian = *(const uint8_t*)&dummy == 1;
            for(size_t i=0; i<8; i++)
            {
                size_t shift = i*8;

                if(is_little_endian)
                {
                    shift = 64-8-shift;
                }
                timestamp |= (uint64_t)klv.m_value[i] << shift;
            }
            time_t    sec = timestamp / 1000000;
            uint64_t      usec = timestamp % 1000000;
            struct tm tm;
            gmtime_r(&sec, &tm);
            char tstr[30];
            int len = strftime(tstr, sizeof(tstr), "%Y/%m/%d %H:%M:%S", &tm);
            sprintf(tstr + len, ".%03ld ", usec / 1000);
            printf("UNIX Time stamp L[%d]: [%ld] = %s\r\n",
                   klv.m_length,timestamp,
                   tstr);
        }else{
            printf("Wrong UNIX TimeStamp\r\n");
        }
        break;
    case 0x03:
        printf("Mission ID: ");
        for(int i=0; i< klv.m_value.size(); i++){
            printf("%c",klv.m_value[i]);
        }
        printf("\r\n");
        break;
    case 0x04:
        printf("Platform Tail Number: ");
        for(int i=0; i< klv.m_value.size(); i++){
            printf("%c",klv.m_value[i]);
        }
        printf("\r\n");
        break;
    case 0x05:
    {
        if(klv.m_value.size() >=2){
            uint16_t valueInt = klv.m_value[0]*256 + klv.m_value[1];
            float valueFloat = static_cast<float>(valueInt)/(65536-1)*360;
            printf("Platform Heading Angle: %f\r\n",valueFloat);
        }
    }
        break;
    case 0x06:
    {
        if(klv.m_value.size() >=2){
            int16_t valueInt = klv.m_value[0]*256 + klv.m_value[1];
            //            int valueInt = 0xFD*256 + 0x3D;
            float valueFloat = static_cast<float>(valueInt+32767)/(65534)*40-20;
            printf("Platform Pitch Angle: %f\r\n",valueFloat);
        }
    }
        break;
    case 0x07:
    {
        if(klv.m_value.size() >=2){
            int16_t valueInt = klv.m_value[0]*256 + klv.m_value[1];
            //            int valueInt = 0x08*256 + 0xB8;
            float valueFloat = static_cast<float>(valueInt+32767)/(65534)*100-50;
            printf("Platform Roll Angle: %f\r\n",valueFloat);
        }
    }
        break;
    case 0x0A:
        printf("Platform Designation: ");
        for(int i=0; i< klv.m_value.size(); i++){
            printf("%c",klv.m_value[i]);
        }
        printf("\r\n");
        break;
    case 0x0D:
    {
        if(klv.m_value.size() >=4){
            int valueInt = klv.m_value[0]*256*256*256 + klv.m_value[1]*256*256 +
                    klv.m_value[2]*256 + klv.m_value[3];
            float valueFloat = static_cast<float>(valueInt)/(4294967294)*180;
            printf("Sensor Latitude: %f\r\n",valueFloat);
        }
    }
        break;
    case 0x0E:
    {
        if(klv.m_value.size() >=4){
            int valueInt = klv.m_value[0]*256*256*256 + klv.m_value[1]*256*256 +
                    klv.m_value[2]*256 + klv.m_value[3];
            float valueFloat = static_cast<float>(valueInt)/(4294967294)*360;
            printf("Sensor Longitude: %f\r\n",valueFloat);
        }
    }
        break;
    case 0x0F:
    {
        if(klv.m_value.size() >=2){
            int valueInt = klv.m_value[0]*256 + klv.m_value[1];
            float valueFloat = static_cast<float>(valueInt)/(65535)*19900-900;
            printf("Sensor Altitude: %f\r\n",valueFloat);
        }
    }
        break;
    case 0x10:
    {
        if(klv.m_value.size() >=2){
            int valueInt = klv.m_value[0]*256 + klv.m_value[1];
            float valueFloat = static_cast<float>(valueInt)/(65535)*180;
            printf("Sensor HFOV: %f\r\n",valueFloat);
        }
    }
        break;
    case 0x11:
    {
        if(klv.m_value.size() >=2){
            int valueInt = klv.m_value[0]*256 + klv.m_value[1];
            float valueFloat = static_cast<float>(valueInt)/(65535)*180;
            printf("Sensor VFOV: %f\r\n",valueFloat);
        }
    }
        break;
    case 0x12:
    {
        if(klv.m_value.size() >=4){
            int valueInt = klv.m_value[0]*256*256*256 + klv.m_value[1]*256*256 +
                    klv.m_value[2]*256 + klv.m_value[3];
            float valueFloat = static_cast<float>(valueInt)/(4294967294)*360;
            if(valueFloat<0){
                valueFloat+=360;
            }
            printf("Sensor Relative Azimuth: %f\r\n",valueFloat);
        }
    }
        break;
    case 0x13:
    {
        if(klv.m_value.size() >=4){
            int valueInt = klv.m_value[0]*256*256*256 + klv.m_value[1]*256*256 +
                    klv.m_value[2]*256 + klv.m_value[3];
            float valueFloat = static_cast<float>(valueInt)/(4294967294)*360;
            printf("Sensor Relative Elevation: %f\r\n",valueFloat);
        }
    }
        break;
    case 0x14:
    {
        if(klv.m_value.size() >=4){
            int valueInt = klv.m_value[0]*256*256*256 + klv.m_value[1]*256*256 +
                    klv.m_value[2]*256 + klv.m_value[3];
            float valueFloat = static_cast<float>(valueInt)/(4294967295)*360;
            printf("Sensor Relative Roll: %f\r\n",valueFloat);
        }
    }
        break;
    case 0x15:
    {
        if(klv.m_value.size() >=4){
            int valueInt = klv.m_value[0]*256*256*256 + klv.m_value[1]*256*256 +
                    klv.m_value[2]*256 + klv.m_value[3];
            float valueFloat = static_cast<float>(valueInt)/(4294967295)*5000000;
            printf("SlantRanged: %f\r\n",valueFloat);
        }
    }
        break;
    case 0x17:
    {
        if(klv.m_value.size() >=4){
            int valueInt = klv.m_value[0]*256*256*256 + klv.m_value[1]*256*256 +
                    klv.m_value[2]*256 + klv.m_value[3];
            float valueFloat = static_cast<float>(valueInt)/(4294967294)*180;
            printf("Frame center Latitude: %f\r\n",valueFloat);
        }
    }
        break;
    case 0x18:
    {
        if(klv.m_value.size() >=4){
            int valueInt = klv.m_value[0]*256*256*256 + klv.m_value[1]*256*256 +
                    klv.m_value[2]*256 + klv.m_value[3];
            float valueFloat = static_cast<float>(valueInt)/(4294967294)*360;
            printf("Frame center Longitude: %f\r\n",valueFloat);
        }
    }
        break;
    case 0x19:
    {
        if(klv.m_value.size() >=2){
            int valueInt = klv.m_value[0]*256 + klv.m_value[1];
            float valueFloat = static_cast<float>(valueInt)/(65535)*19900-900;
            printf("Frame center Elevation: %f\r\n",valueFloat);
        }
    }
        break;
    case 0x1A:
    {
        if(klv.m_value.size() >=2){
            int16_t valueInt = klv.m_value[0]*256 + klv.m_value[1];
            float valueFloat = static_cast<float>(valueInt)/(65534)*0.15f;
            printf("Offset corner Latitude Point1: %f\r\n",valueFloat);
        }
    }
        break;
    case 0x1B:
    {
        if(klv.m_value.size() >=2){
            int16_t valueInt = klv.m_value[0]*256 + klv.m_value[1];
            float valueFloat = static_cast<float>(valueInt)/(65534)*0.15f;
            printf("Offset corner Longitude Point1: %f\r\n",valueFloat);
        }
    }
        break;
    case 0x1C:
    {
        if(klv.m_value.size() >=2){
            int16_t valueInt = klv.m_value[0]*256 + klv.m_value[1];
            float valueFloat = static_cast<float>(valueInt)/(65534)*0.15f;
            printf("Offset corner Latitude Point2: %f\r\n",valueFloat);
        }
    }
        break;
    case 0x1D:
    {
        if(klv.m_value.size() >=2){
            int16_t valueInt = klv.m_value[0]*256 + klv.m_value[1];
            float valueFloat = static_cast<float>(valueInt)/(65534)*0.15f;
            printf("Offset corner Longitude Point2: %f\r\n",valueFloat);
        }
    }
        break;
    case 0x1E:
    {
        if(klv.m_value.size() >=2){
            int16_t valueInt = klv.m_value[0]*256 + klv.m_value[1];
            float valueFloat = static_cast<float>(valueInt)/(65534)*0.15f;
            printf("Offset corner Latitude Point3: %f\r\n",valueFloat);
        }
    }
        break;
    case 0x1F:
    {
        if(klv.m_value.size() >=2){
            int16_t valueInt = klv.m_value[0]*256 + klv.m_value[1];
            float valueFloat = static_cast<float>(valueInt)/(65534)*0.15f;
            printf("Offset corner Longitude Point3: %f\r\n",valueFloat);
        }
    }
        break;
    case 0x20:
    {
        if(klv.m_value.size() >=2){
            int16_t valueInt = klv.m_value[0]*256 + klv.m_value[1];
            float valueFloat = static_cast<float>(valueInt)/(65534)*0.15f;
            printf("Offset corner Latitude Point4: %f\r\n",valueFloat);
        }
    }
        break;
    case 0x21:
    {
        if(klv.m_value.size() >=2){
            int16_t valueInt = klv.m_value[0]*256 + klv.m_value[1];
            float valueFloat = static_cast<float>(valueInt)/(65534)*0.15f;
            printf("Offset corner Longitude Point4: %f\r\n",valueFloat);
        }
    }
        break;
    default:
        break;
    }
}
int main(int argc, char* argv[])
{
    //    QGuiApplication app(argc, argv);
    //    // initialize parameters
    //        qreal sampleRate = 40000;   // sample rate
    //        qreal duration = 10.000;     // duration in seconds
    //        qreal frequency = 1000;     // frequency
    //        const quint32 n = static_cast<quint32>(duration * sampleRate);   // number of data samples
    //    QAudioFormat audioFormat;
    //    audioFormat.setSampleRate(static_cast<int>(sampleRate));
    //    audioFormat.setChannelCount(1);
    //    audioFormat.setSampleSize(32);   // set the sample size in bits. We set it to 32 bis, because we set SampleType to float (one float has 4 bytes ==> 32 bits)
    //    audioFormat.setCodec("audio/pcm");
    //    audioFormat.setByteOrder(QAudioFormat::LittleEndian);
    //    audioFormat.setSampleType(QAudioFormat::Float);   // use Float, to have a better resolution than SignedInt or UnSignedInt

    //    // create a QAudioDeviceInfo object, to make sure that our audioFormat is supported by the device
    //    QAudioDeviceInfo deviceInfo(QAudioDeviceInfo::defaultOutputDevice());
    //    if(!deviceInfo.isFormatSupported(audioFormat))
    //    {
    //        std::cout << "Raw audio format not supported by backend, cannot play audio.";
    //        return -1;
    //    }
    //    QByteArray *byteBuffer = new QByteArray();  // create a new instance of QByteArray class (in the heap, dynamically arranged in memory), and set its pointer to byteBuffer
    //        byteBuffer->resize(sizeof(float) * n);  // resize byteBuffer to the total number of bytes that will be needed to accommodate all the n data samples that are of type float

    //        for (quint32 i = 0; i < n; i++)
    //        {
    //            qreal sinVal = (qreal)sin(2.0 * M_PI * frequency * i / sampleRate);  // create sine wave data samples, one at a time

    //            // break down one float into four bytes
    //            float sample = (float)sinVal;  // save one data sample in a local variable, so I can break it down into four bytes
    //            char *ptr = (char*)(&sample);  // assign a char* pointer to the address of this data sample
    //            char byte00 = *ptr;         // 1st byte
    //            char byte01 = *(ptr + 1);   // 2nd byte
    //            char byte02 = *(ptr + 2);   // 3rd byte
    //            char byte03 = *(ptr + 3);   // 4th byte

    //            // put byte data into QByteArray, one byte at a time
    //            (*byteBuffer)[4 * i] = byte00;       // put 1st byte into QByteArray
    //            (*byteBuffer)[4 * i + 1] = byte01;   // put 2nd byte into QByteArray
    //            (*byteBuffer)[4 * i + 2] = byte02;   // put 3rd byte into QByteArray
    //            (*byteBuffer)[4 * i + 3] = byte03;   // put 4th byte into QByteArray
    //        }
    //    // Make a QBuffer with our QByteArray
    //    QBuffer* input = new QBuffer(byteBuffer);
    //    input->open(QIODevice::ReadOnly);   // set the QIODevice to read-only

    //    // Create an audio output with our QAudioFormat
    //    QAudioOutput* audio = new QAudioOutput(audioFormat, nullptr);


//    //    // start the audio (i.e., play sound from the QAudioOutput object that we just created)
//    //    audio->start(input);
//    //    return app.exec();
////    String face_cascade_name = "haarcascade_frontalcatface.xml";
////    //-- 1. Load the cascades
////    if( !face_cascade.load( face_cascade_name ) )
////    {
////        cout << "--(!)Error loading face cascade\n";
////        return -1;
////    };
//    if (argc < 2) {
//        std::cout << "Usage: ff2cv <infile>" << std::endl;
//        return 1;
//    }

    const char* infile = argv[1];

    // initialize FFmpeg library

    avformat_network_init();
    av_register_all();
    av_log_set_level(AV_LOG_QUIET);
    int ret;

    // open input file context
    AVFormatContext* inctx = nullptr;
    ret = avformat_open_input(&inctx, infile, nullptr, nullptr);
    if (ret < 0) {
        std::cerr << "fail to avformat_open_input(\"" << infile << "\"): ret=" << ret;
        return 2;
    }
    // retrive input stream information
    ret = avformat_find_stream_info(inctx, nullptr);
    if (ret < 0) {
        std::cerr << "fail to avformat_find_stream_info: ret=" << ret;
        return 2;
    }

    // find primary video stream
    int meta_stream_index = -1;
    int audio_stream_index = -1;
    int video_stream_index = -1;
    for (int i = 0; i < inctx->nb_streams; i++) {
        if (inctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_DATA)
        {
            meta_stream_index = i;
        }else if (inctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            video_stream_index = i;
        }else if (inctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            audio_stream_index = i;
        }
    }
    AVCodec* vcodec = nullptr;
    ret = av_find_best_stream(inctx, AVMEDIA_TYPE_VIDEO, -1, -1, &vcodec, 0);
    if (ret < 0) {
        std::cerr << "fail to av_find_best_stream: ret=" << ret;
        return 2;
    }

    AVStream* vstrm = inctx->streams[video_stream_index];
    // open video decoder context
    ret = avcodec_open2(vstrm->codec, vcodec, nullptr);
    if (ret < 0) {
        std::cerr << "fail to avcodec_open2 for video: ret=" << ret;
        return 2;
    }

    AVCodec* acodec = nullptr;
    ret = av_find_best_stream(inctx, AVMEDIA_TYPE_AUDIO, -1, -1, &acodec, 0);
    if(acodec!= nullptr){
        AVStream* astrm = inctx->streams[audio_stream_index];
        ret = avcodec_open2(astrm->codec, acodec, nullptr);
        if (ret < 0) {
            std::cerr << "fail to avcodec_open2 for audio: ret=" << ret;
            return 2;
        }

    }

    // print input video stream informataion
    std::cout
            << "infile: " << infile << "\n"
            << "format: " << inctx->iformat->name << "\n"
            << "duration: " << inctx->duration << "\n"
            << "framerate: " << inctx->fps_probe_size << "\n"
            << "vcodec: " << vcodec->name << "\n"
            << "vcodec id: " << vcodec->id << "\n"
            << "vcodec type: " << vcodec->type << "\n"
            << "acodec: " << (acodec != nullptr?acodec->name:" no audio ") << "\n"
            << "size:   " << vstrm->codec->width << 'x' << vstrm->codec->height << "\n"
            << "fps:    " << av_q2d(vstrm->codec->framerate) << " [fps]\n"
            << "length: " << av_rescale_q(vstrm->duration, vstrm->time_base, {1,1000}) / 1000. << " [sec]\n"
            << "pixfmt: " << av_get_pix_fmt_name(vstrm->codec->pix_fmt) << "\n"
            << "frame:  " << vstrm->nb_frames << "\n"
            << std::flush;

    // initialize sample scaler
    int dst_width = vstrm->codec->width;
    int dst_height = vstrm->codec->height;
//    dst_width = 896;
//    dst_height = 896;
    printf("dst [%dx%d]\r\n",dst_width,dst_height);
    AVPixelFormat dst_pix_fmt = AV_PIX_FMT_YUV420P;
    SwsContext* swsctx = sws_getCachedContext(
                nullptr, vstrm->codec->width, vstrm->codec->height, vstrm->codec->pix_fmt,
                dst_width, dst_height, dst_pix_fmt, SWS_BICUBIC, nullptr, nullptr, nullptr);
    if (!swsctx) {
        std::cerr << "fail to sws_getCachedContext";
    }
    std::cout << "output: " << dst_width << 'x' << dst_height << ',' << av_get_pix_fmt_name(dst_pix_fmt) << std::endl;

    // allocate frame buffer for output
    AVFrame* frame = av_frame_alloc();
    std::vector<uint8_t> framebuf(avpicture_get_size(dst_pix_fmt, dst_width, dst_height));
    avpicture_fill(reinterpret_cast<AVPicture*>(frame), framebuf.data(), dst_pix_fmt, dst_width, dst_height);

    // decoding loop
    AVFrame* decframe = av_frame_alloc();
    unsigned nb_frames = 0;
    AVPacket pkt;
    AVOutputFormat *fmt = new AVOutputFormat();
    AVFormatContext *oc ;
    while (av_read_frame(inctx, &pkt) >= 0){
        if(pkt.stream_index == video_stream_index){
            int got_pic = 0;
            avcodec_decode_video2(vstrm->codec, decframe, &got_pic, &pkt);
            if (got_pic){
                sws_scale(swsctx, decframe->data, decframe->linesize, 0, decframe->height, frame->data, frame->linesize);
//                printf("V++++++++++++++++++++\r\n");
                // convert frame to OpenCV matrix
                cv::Mat imageY(dst_height, dst_width, CV_8UC1, decframe->data[0]);
                cv::Mat imageU(dst_height/4, dst_width/1, CV_8UC1, decframe->data[1]);
                cv::Mat imageV(dst_height/4, dst_width/1, CV_8UC1, decframe->data[2]);
                cv::Mat image(dst_height*3/2, dst_width, CV_8UC1);
                imageY.copyTo(image(cv::Rect(0,0,dst_width, dst_height)));
                imageU.copyTo(image(cv::Rect(0,dst_height, dst_width/1,dst_height/4)));
                imageV.copyTo(image(cv::Rect(0,dst_height*5/4, dst_width/1,dst_height/4)));
                cv::Mat imageRGB;
                cv::cvtColor(image,imageRGB,cv::COLOR_YUV2RGB_YV12);
//                cv::Mat imageResize;
//                cv::resize(imageRGB,imageResize,cv::Size(imageRGB.cols/2,imageRGB.rows/2));
//                std::vector<Rect> faces;
//                detectAndDisplay( imageResize,faces );
//                for ( size_t i = 0; i < faces.size(); i++ )
//                {
//                    Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
//                    ellipse( imageResize, center, Size( faces[i].width/2, faces[i].height/2 ), 0, 0, 360, Scalar( 255, 0, 255 ), 4 );
//                }
//                cv::imshow("imageResize", imageResize);
                cv::imshow("imageRGB", imageRGB);
                cv::waitKey(30);
                ++nb_frames;
            }
        }else if(pkt.stream_index == audio_stream_index){
//            printf("A===========\r\n");
        }else if(pkt.stream_index == meta_stream_index){
            printf("M\r\n");
            for(int i=0; i< pkt.buf->size; i++){
                printf("0X%02X,",pkt.buf->data[i]);
            }
            printf("\r\n");
            unsigned char *data=pkt.buf->data;
            unsigned int startIndex = 18;
            while(startIndex < pkt.buf->size){
                uint8_t key = pkt.buf->data[startIndex];
                uint8_t length = pkt.buf->data[startIndex+1];
                if(length+startIndex+1>= pkt.buf->size)
                    break;
                std::vector<uint8_t> value(pkt.buf->data+startIndex+2,
                                           pkt.buf->data+startIndex+2+length);
                startIndex += length+2;
                Klv klv(key,length,value);
                parseKlv(klv);
            }
        }
        av_free_packet(&pkt);
    }
    av_frame_free(&decframe);
    av_frame_free(&frame);
    avcodec_close(vstrm->codec);
    avformat_close_input(&inctx);
    return 0;
}
