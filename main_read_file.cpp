/*
 * Read video frame with FFmpeg and convert to OpenCV image
 *
 * Copyright (c) 2016 yohhoy
 */
#include <iostream>
#include <vector>
#include <stdio.h>
// FFmpeg
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/pixdesc.h>
#include <libswscale/swscale.h>
}
// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

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
        printf("UNIX Time stamp\r\n");
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
            int valueInt = klv.m_value[0]*256 + klv.m_value[1];
            float valueFloat = static_cast<float>(valueInt)/(65536-1)*360;
            printf("Platform Heading Angle: %f\r\n",valueFloat);
        }
    }
        break;
    case 0x06:
    {
        if(klv.m_value.size() >=2){
            int valueInt = klv.m_value[0]*256 + klv.m_value[1];
            //            int valueInt = 0xFD*256 + 0x3D;
            float valueFloat = static_cast<float>(valueInt+32767)/(65534)*40-20;
            printf("Platform Pitch Angle: %f\r\n",valueFloat);
        }
    }
        break;
    case 0x07:
    {
        if(klv.m_value.size() >=2){
            int valueInt = klv.m_value[0]*256 + klv.m_value[1];
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
            int valueInt = klv.m_value[0]*256 + klv.m_value[1];
            float valueFloat = static_cast<float>(valueInt)/(65534)*0.15f;
            printf("Offset corner Latitude Point1: %f\r\n",valueFloat);
        }
    }
        break;
    case 0x1B:
    {
        if(klv.m_value.size() >=2){
            int valueInt = klv.m_value[0]*256 + klv.m_value[1];
            float valueFloat = static_cast<float>(valueInt)/(65534)*0.15f;
            printf("Offset corner Longitude Point1: %f\r\n",valueFloat);
        }
    }
        break;
    case 0x1C:
    {
        if(klv.m_value.size() >=2){
            int valueInt = klv.m_value[0]*256 + klv.m_value[1];
            float valueFloat = static_cast<float>(valueInt)/(65534)*0.15f;
            printf("Offset corner Latitude Point2: %f\r\n",valueFloat);
        }
    }
        break;
    case 0x1D:
    {
        if(klv.m_value.size() >=2){
            int valueInt = klv.m_value[0]*256 + klv.m_value[1];
            float valueFloat = static_cast<float>(valueInt)/(65534)*0.15f;
            printf("Offset corner Longitude Point2: %f\r\n",valueFloat);
        }
    }
        break;
    case 0x1E:
    {
        if(klv.m_value.size() >=2){
            int valueInt = klv.m_value[0]*256 + klv.m_value[1];
            float valueFloat = static_cast<float>(valueInt)/(65534)*0.15f;
            printf("Offset corner Latitude Point3: %f\r\n",valueFloat);
        }
    }
        break;
    case 0x1F:
    {
        if(klv.m_value.size() >=2){
            int valueInt = klv.m_value[0]*256 + klv.m_value[1];
            float valueFloat = static_cast<float>(valueInt)/(65534)*0.15f;
            printf("Offset corner Longitude Point3: %f\r\n",valueFloat);
        }
    }
        break;
    case 0x20:
    {
        if(klv.m_value.size() >=2){
            int valueInt = klv.m_value[0]*256 + klv.m_value[1];
            float valueFloat = static_cast<float>(valueInt)/(65534)*0.15f;
            printf("Offset corner Latitude Point4: %f\r\n",valueFloat);
        }
    }
        break;
    case 0x21:
    {
        if(klv.m_value.size() >=2){
            int valueInt = klv.m_value[0]*256 + klv.m_value[1];
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
    if (argc < 2) {
        std::cout << "Usage: ff2cv <infile>" << std::endl;
        return 1;
    }
    const char* infile = argv[1];

    // initialize FFmpeg library
    av_register_all();
    //  av_log_set_level(AV_LOG_DEBUG);
    int ret;

    // open input file context
    AVFormatContext* inctx = nullptr;
    ret = avformat_open_input(&inctx, infile, nullptr, nullptr);
    if (ret < 0) {
        std::cerr << "fail to avforamt_open_input(\"" << infile << "\"): ret=" << ret;
        return 2;
    }
    // retrive input stream information
    ret = avformat_find_stream_info(inctx, nullptr);
    if (ret < 0) {
        std::cerr << "fail to avformat_find_stream_info: ret=" << ret;
        return 2;
    }

    // find primary video stream
    AVCodec* vcodec = nullptr;
    ret = av_find_best_stream(inctx, AVMEDIA_TYPE_VIDEO, -1, -1, &vcodec, 0);
    if (ret < 0) {
        std::cerr << "fail to av_find_best_stream: ret=" << ret;
        return 2;
    }
    const int vstrm_idx = ret;
    AVStream* vstrm = inctx->streams[vstrm_idx];

    // open video decoder context
    ret = avcodec_open2(vstrm->codec, vcodec, nullptr);
    if (ret < 0) {
        std::cerr << "fail to avcodec_open2: ret=" << ret;
        return 2;
    }

    // print input video stream informataion
    std::cout
            << "infile: " << infile << "\n"
            << "format: " << inctx->iformat->name << "\n"
            << "duration: " << inctx->duration << "\n"
            << "framerate: " << inctx->fps_probe_size << "\n"
            << "vcodec: " << vcodec->name << "\n"
            << "size:   " << vstrm->codec->width << 'x' << vstrm->codec->height << "\n"
            << "fps:    " << av_q2d(vstrm->codec->framerate) << " [fps]\n"
            << "length: " << av_rescale_q(vstrm->duration, vstrm->time_base, {1,1000}) / 1000. << " [sec]\n"
            << "pixfmt: " << av_get_pix_fmt_name(vstrm->codec->pix_fmt) << "\n"
            << "frame:  " << vstrm->nb_frames << "\n"
            << std::flush;

    // initialize sample scaler
    const int dst_width = vstrm->codec->width;
    const int dst_height = vstrm->codec->height;
    const AVPixelFormat dst_pix_fmt = AV_PIX_FMT_YUV420P;
    SwsContext* swsctx = sws_getCachedContext(
                nullptr, vstrm->codec->width, vstrm->codec->height, vstrm->codec->pix_fmt,
                dst_width, dst_height, dst_pix_fmt, SWS_BICUBIC, nullptr, nullptr, nullptr);
    if (!swsctx) {
        std::cerr << "fail to sws_getCachedContext";
        //        return 2;
    }
    std::cout << "output: " << dst_width << 'x' << dst_height << ',' << av_get_pix_fmt_name(dst_pix_fmt) << std::endl;

    // allocate frame buffer for output
    AVFrame* frame = av_frame_alloc();
    std::vector<uint8_t> framebuf(avpicture_get_size(dst_pix_fmt, dst_width, dst_height));
    avpicture_fill(reinterpret_cast<AVPicture*>(frame), framebuf.data(), dst_pix_fmt, dst_width, dst_height);

    // decoding loop
    AVFrame* decframe = av_frame_alloc();
    unsigned nb_frames = 0;
    bool end_of_stream = false;
    int got_pic = 0;
    AVPacket pkt;
    bool gotoNextPacket = false;
    do {
        if (!end_of_stream) {
            // read packet from input file
            ret = av_read_frame(inctx, &pkt);
            if (ret < 0 && ret != AVERROR_EOF) {
                std::cerr << "fail to av_read_frame: ret=" << ret;
                return 2;
            }
            if (ret == 0 && pkt.stream_index != vstrm_idx){
                //                goto next_packet;
                gotoNextPacket = true;
            }else{
                gotoNextPacket = false;
            }
            if(!gotoNextPacket)
                end_of_stream = (ret == AVERROR_EOF);
        }
        if(!gotoNextPacket){
            if (end_of_stream) {
                // null packet for bumping process
                av_init_packet(&pkt);
                pkt.data = nullptr;
                pkt.size = 0;
            }
            // decode video frame
            avcodec_decode_video2(vstrm->codec, decframe, &got_pic, &pkt);
            if (got_pic){
                // convert frame to OpenCV matrix
//                sws_scale(swsctx, decframe->data, decframe->linesize, 0, decframe->height, frame->data, frame->linesize);
//                {
//                    //            printf("decframe [%dx%d]\r\n",decframe->width,decframe->height);
//                    cv::Mat image(dst_height, dst_width, CV_8UC1, decframe->data[0]);
//                    cv::imshow("press ESC to exit", image);
//                    if (cv::waitKey(1) == 0x1b)
//                        break;
//                }
                //        std::cout << nb_frames << '\r' << std::flush;  // dump progress
                ++nb_frames;
            }
        }else{
            if(pkt.stream_index == 2){
                printf("Metadata\r\n");
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
    } while (!end_of_stream || got_pic);
    std::cout << nb_frames << " frames decoded" << std::endl;
    av_frame_free(&decframe);
    av_frame_free(&frame);
    avcodec_close(vstrm->codec);
    avformat_close_input(&inctx);
    return 0;
}
