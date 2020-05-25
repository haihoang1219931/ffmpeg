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
//    av_register_all();
//    AVFormatContext* pFormatCtx = avformat_alloc_context();
//    avformat_open_input(&pFormatCtx, argv[1], NULL, NULL);
//    avformat_find_stream_info(pFormatCtx,NULL);
//    int64_t duration = pFormatCtx->duration;
//    AV_LOG_INFO;
//    std::cout
//        << "infile: " << argv[1] << "\n"
//        << "format: " << pFormatCtx->iformat->name << "\n"
//        << "duration: " << pFormatCtx->duration << "\n"
//        << "framerate: " << pFormatCtx->fps_probe_size << "\n"
//        << "start_time: " << pFormatCtx->start_time << "\n"
//        << std::endl;
//    // etc
//    avformat_close_input(&pFormatCtx);
//    avformat_free_context(pFormatCtx);
//    return 0;
//}
