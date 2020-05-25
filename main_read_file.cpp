///*
// * Read video frame with FFmpeg and convert to OpenCV image
// *
// * Copyright (c) 2016 yohhoy
// */
//#include <iostream>
//#include <vector>
//#include <stdio.h>
//// FFmpeg
//extern "C" {
//#include <libavformat/avformat.h>
//#include <libavcodec/avcodec.h>
//#include <libavutil/avutil.h>
//#include <libavutil/pixdesc.h>
//#include <libswscale/swscale.h>
//}
//// OpenCV
//#include <opencv2/core.hpp>
//#include <opencv2/highgui.hpp>


//int main(int argc, char* argv[])
//{
//    if (argc < 2) {
//        std::cout << "Usage: ff2cv <infile>" << std::endl;
//        return 1;
//    }
//    const char* infile = argv[1];

//    // initialize FFmpeg library
//    av_register_all();
//    //  av_log_set_level(AV_LOG_DEBUG);
//    int ret;

//    // open input file context
//    AVFormatContext* inctx = nullptr;
//    ret = avformat_open_input(&inctx, infile, nullptr, nullptr);
//    if (ret < 0) {
//        std::cerr << "fail to avforamt_open_input(\"" << infile << "\"): ret=" << ret;
//        return 2;
//    }
//    // retrive input stream information
//    ret = avformat_find_stream_info(inctx, nullptr);
//    if (ret < 0) {
//        std::cerr << "fail to avformat_find_stream_info: ret=" << ret;
//        return 2;
//    }

//    // find primary video stream
//    AVCodec* vcodec = nullptr;
//    ret = av_find_best_stream(inctx, AVMEDIA_TYPE_VIDEO, -1, -1, &vcodec, 0);
//    if (ret < 0) {
//        std::cerr << "fail to av_find_best_stream: ret=" << ret;
//        return 2;
//    }
//    const int vstrm_idx = ret;
//    AVStream* vstrm = inctx->streams[vstrm_idx];

//    // open video decoder context
//    ret = avcodec_open2(vstrm->codec, vcodec, nullptr);
//    if (ret < 0) {
//        std::cerr << "fail to avcodec_open2: ret=" << ret;
//        return 2;
//    }

//    // print input video stream informataion
//    std::cout
//            << "infile: " << infile << "\n"
//            << "format: " << inctx->iformat->name << "\n"
//            << "duration: " << inctx->duration << "\n"
//            << "framerate: " << inctx->fps_probe_size << "\n"
//            << "vcodec: " << vcodec->name << "\n"
//            << "size:   " << vstrm->codec->width << 'x' << vstrm->codec->height << "\n"
//            << "fps:    " << av_q2d(vstrm->codec->framerate) << " [fps]\n"
//            << "length: " << av_rescale_q(vstrm->duration, vstrm->time_base, {1,1000}) / 1000. << " [sec]\n"
//            << "pixfmt: " << av_get_pix_fmt_name(vstrm->codec->pix_fmt) << "\n"
//            << "frame:  " << vstrm->nb_frames << "\n"
//            << std::flush;

//    // initialize sample scaler
//    const int dst_width = vstrm->codec->width;
//    const int dst_height = vstrm->codec->height;
//    const AVPixelFormat dst_pix_fmt = AV_PIX_FMT_YUV420P;
//    SwsContext* swsctx = sws_getCachedContext(
//                nullptr, vstrm->codec->width, vstrm->codec->height, vstrm->codec->pix_fmt,
//                dst_width, dst_height, dst_pix_fmt, SWS_BICUBIC, nullptr, nullptr, nullptr);
//    if (!swsctx) {
//        std::cerr << "fail to sws_getCachedContext";
//        //        return 2;
//    }
//    std::cout << "output: " << dst_width << 'x' << dst_height << ',' << av_get_pix_fmt_name(dst_pix_fmt) << std::endl;

//    // allocate frame buffer for output
//    AVFrame* frame = av_frame_alloc();
//    std::vector<uint8_t> framebuf(avpicture_get_size(dst_pix_fmt, dst_width, dst_height));
//    avpicture_fill(reinterpret_cast<AVPicture*>(frame), framebuf.data(), dst_pix_fmt, dst_width, dst_height);

//    // decoding loop
//    AVFrame* decframe = av_frame_alloc();
//    unsigned nb_frames = 0;
//    bool end_of_stream = false;
//    int got_pic = 0;
//    AVPacket pkt;
//    do {
//        if (!end_of_stream) {
//            // read packet from input file
//            ret = av_read_frame(inctx, &pkt);
//            if (ret < 0 && ret != AVERROR_EOF) {
//                std::cerr << "fail to av_read_frame: ret=" << ret;
//                return 2;
//            }
//            if (ret == 0 && pkt.stream_index != vstrm_idx)
//                goto next_packet;
//            end_of_stream = (ret == AVERROR_EOF);
//        }
//        if (end_of_stream) {
//            // null packet for bumping process
//            av_init_packet(&pkt);
//            pkt.data = nullptr;
//            pkt.size = 0;
//        }
//        // decode video frame
//        avcodec_decode_video2(vstrm->codec, decframe, &got_pic, &pkt);
//        if (!got_pic)
//            goto next_packet;
//        // convert frame to OpenCV matrix
////        sws_scale(swsctx, decframe->data, decframe->linesize, 0, decframe->height, frame->data, frame->linesize);
////        {
//////            printf("decframe [%dx%d]\r\n",decframe->width,decframe->height);
////            cv::Mat image(dst_height, dst_width, CV_8UC1, decframe->data[0]);
////            cv::imshow("press ESC to exit", image);
////            if (cv::waitKey(1) == 0x1b)
////                break;
////        }
//        //        std::cout << nb_frames << '\r' << std::flush;  // dump progress
//        ++nb_frames;
//next_packet:
////        printf("pkt.flags=%d pkt.stream_index=%d\r\n",pkt.flags,pkt.stream_index);
//        if(pkt.stream_index == 2){
//            printf("Metadata\r\n");
//            for(int i=0; i<pkt.buf->size; i++)
//            {
//                printf("0X%02X,",pkt.buf->data[i]);
//            }
//            printf("\r\n");
////            printf("Metadata\r\n");
////            for(int i=0; i<pkt.buf->size; i++)
////            {
////                printf("%c",pkt.buf->data[i]);
////            }
////            printf("\r\n");
//            unsigned char *data=pkt.buf->data;
//            printf("Mission ID: ");
//            int parsedIndex = 30;
//            int missionIDLength=0;
//            for(int i=parsedIndex; i< parsedIndex+127; i++){
//                if(isprint(data[i])){
//                    missionIDLength++;
//                    printf("%c",data[i]);
//                }else{
//                    break;
//                }
//            }
//            printf("[%d]\r\n",missionIDLength);
//            printf("Platform Tail Number: ");
//            parsedIndex += (missionIDLength+2);
//            int platformTailNumberLength=0;
//            for(int i=parsedIndex; i< parsedIndex+127; i++){
//                if(isprint(data[i])){
//                    platformTailNumberLength++;
//                    printf("%c",data[i]);
//                }else{
//                    break;
//                }
//            }
//            printf("[%d]\r\n",platformTailNumberLength);
//            parsedIndex += (platformTailNumberLength+1);
//            float headingAngle = (float)(data[parsedIndex] + data[parsedIndex+1]*16);
//            printf("headingAngle=%f\r\n",headingAngle/(65536-1)*360);
//            float pitchAngle = (float)(data[parsedIndex+4] + data[parsedIndex+5]*16);
//            printf("pitchAngle=%f\r\n",-((pitchAngle+32768-1)/(65536)*40-20));
//            float rollAngle = (float)(data[parsedIndex+8] + data[parsedIndex+9]*16);
//            printf("rollAngle=%f\r\n",-((rollAngle+32768-1)/(65536)*100-50));
//        }
//        av_free_packet(&pkt);
//    } while (!end_of_stream || got_pic);
//    std::cout << nb_frames << " frames decoded" << std::endl;

//    av_frame_free(&decframe);
//    av_frame_free(&frame);
//    avcodec_close(vstrm->codec);
//    avformat_close_input(&inctx);
//    return 0;
//}
