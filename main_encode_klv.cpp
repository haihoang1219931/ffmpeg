/*
 * Libavformat API example: Output a media file in any supported
 * libavformat format. The default codecs are used.
 *
 * Copyright (c) 2003 Fabrice Bellard
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.1415926535897931
#endif

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/pixdesc.h>
#include <libswscale/swscale.h>
}

/* 5 seconds stream duration */
#define STREAM_DURATION   5.0
#define STREAM_FRAME_RATE 25 /* 25 images/s */
#define STREAM_NB_FRAMES  ((int)(STREAM_DURATION * STREAM_FRAME_RATE))
#define STREAM_PIX_FMT PIX_FMT_YUV420P /* default pix_fmt */

static int sws_flags = SWS_BICUBIC;

/**************************************************************/
/* audio output */

float t, tincr, tincr2;
int16_t *samples;
uint8_t *audio_outbuf;
int audio_outbuf_size;
int audio_input_frame_size;


/**************************************************************/
/* video output */

AVFrame *picture, *tmp_picture;
uint8_t *video_outbuf;
int frame_count, video_outbuf_size;

/* add a video output stream */
static AVStream *add_video_stream(AVFormatContext *oc, int codec_id)
{
    AVCodecContext *c;
    AVStream *st;

    int i;

    if (oc->nb_streams >= 20)
        return NULL;

    st = (AVStream *)av_mallocz(sizeof(AVStream));
    if (!st)
        return NULL;

    st->codec= avcodec_alloc_context3(oc->video_codec);
    if (oc->iformat) {
        /* no default bitrate if decoding */
        st->codec->bit_rate = 0;
    }
    st->index = oc->nb_streams;
    st->id = codec_id;
    st->start_time = AV_NOPTS_VALUE;
    st->duration = AV_NOPTS_VALUE;
        /* we set the current DTS to 0 so that formats without any timestamps
           but durations get some timestamps, formats with some unknown
           timestamps have their first few packets buffered and the
           timestamps corrected before they are returned to the user */
    st->cur_dts = 0;
    st->first_dts = AV_NOPTS_VALUE;
    st->probe_packets = 2500;

    /* default pts setting is MPEG-like */
//    av_set_pts_info(st, 33, 1, 90000);
    st->last_IP_pts = AV_NOPTS_VALUE;
    for(i=0; i<MAX_REORDER_DELAY+1; i++)
        st->pts_buffer[i]= AV_NOPTS_VALUE;
//    st->reference_dts = AV_NOPTS_VALUE;

    st->sample_aspect_ratio = (AVRational){0,1};


    oc->streams =&st;
    oc->streams[oc->nb_streams++] = st;
    if (!st) {
        fprintf(stderr, "Could not alloc stream\n");
        exit(1);
    }

    c = st->codec;
    c->codec_id = AVCodecID::AV_CODEC_ID_MPEG1VIDEO;
    c->codec_type =  AVMediaType::AVMEDIA_TYPE_VIDEO;

    /* put sample parameters */
    c->bit_rate = 400000;
    /* resolution must be a multiple of two */
    c->width = 352;
    c->height = 288;
    /* time base: this is the fundamental unit of time (in seconds) in terms
       of which frame timestamps are represented. for fixed-fps content,
       timebase should be 1/framerate and timestamp increments should be
       identically 1. */
    c->time_base.den = STREAM_FRAME_RATE;
    c->time_base.num = 1;
    c->gop_size = 12; /* emit one intra frame every twelve frames at most */
    c->pix_fmt = STREAM_PIX_FMT;
//    if (c->codec_id == CODEC_ID_MPEG2VIDEO) {
//        /* just for testing, we also add B frames */
//        c->max_b_frames = 2;
//    }
//    if (c->codec_id == CODEC_ID_MPEG1VIDEO){
//        /* needed to avoid using macroblocks in which some coeffs overflow
//           this doesnt happen with normal video, it just happens here as the
//           motion of the chroma plane doesnt match the luma plane */
//        c->mb_decision=2;
//    }
//    // some formats want stream headers to be seperate
//    if(!strcmp(oc->oformat->name, "mp4") || !strcmp(oc->oformat->name, "mov") || !strcmp(oc->oformat->name, "3gp"))
//        c->flags |= CODEC_FLAG_GLOBAL_HEADER;

    return st;
}

static AVFrame *alloc_picture(AVPixelFormat pix_fmt, int width, int height)
{
    printf("%s %d:%d:%d\r\n",__func__,pix_fmt,width,height);
    AVFrame *picture;
    uint8_t *picture_buf;
    int size;


    picture = avcodec_alloc_frame();
    picture->format = pix_fmt;
    picture->width = width;
    picture->height = height;
    size = width * height;
    picture_buf = (uint8_t *)malloc((size * 3) / 2); /* size for YUV 420 */

    picture->data[0] = picture_buf;
    picture->data[1] = picture->data[0] + size;
    picture->data[2] = picture->data[1] + size / 4;
    picture->linesize[0] = width;
    picture->linesize[1] = width / 2;
    picture->linesize[2] = width / 2;
    for(int y=0;y<height;y++) {
        for(int x=0;x<width;x++) {
            picture->data[0][y * picture->linesize[0] + x] = x + y + 1 * 3;
        }
    }

    /* Cb and Cr */
    for(int y=0;y<height/2;y++) {
        for(int x=0;x<width/2;x++) {
            picture->data[1][y * picture->linesize[1] + x] = 128 + y + 1 * 2;
            picture->data[2][y * picture->linesize[2] + x] = 64 + x + 1 * 5;
        }
    }
    printf("picture->pix_fmt=%d\r\n",picture->format);
    printf("picture->width=%d\r\n",picture->width);
    printf("picture->height=%d\r\n",picture->height);
    return picture;
}

static void open_video(AVFormatContext *oc, AVStream *st)
{
    AVCodec *codec;
    AVCodecContext *c;

    c = st->codec;

    /* find the video encoder */
    codec = avcodec_find_encoder(c->codec_id);
    if (!codec) {
        fprintf(stderr, "codec not found\n");
        exit(1);
    }

    /* open the codec */
    if (avcodec_open2(c, codec,nullptr) < 0) {
        fprintf(stderr, "could not open codec\n");
        exit(1);
    }

    video_outbuf = NULL;
    if (!(oc->oformat->flags & AVFMT_RAWPICTURE)) {
        /* allocate output buffer */
        /* XXX: API change will be done */
        /* buffers passed into lav* can be allocated any way you prefer,
           as long as they're aligned enough for the architecture, and
           they're freed appropriately (such as using av_free for buffers
           allocated with av_malloc) */
        video_outbuf_size = 200000;
        video_outbuf = (uint8_t *)av_malloc(video_outbuf_size);
    }

    /* allocate the encoded raw picture */

    printf("c->pix_fmt=%d\r\n",c->pix_fmt);
    printf("c->width=%d\r\n",c->width);
    printf("c->height=%d\r\n",c->height);
    picture = alloc_picture(c->pix_fmt, c->width, c->height);
    if (!picture) {
        fprintf(stderr, "Could not allocate picture\n");
        exit(1);
    }

    /* if the output format is not YUV420P, then a temporary YUV420P
       picture is needed too. It is then converted to the required
       output format */
    tmp_picture = NULL;
    if (c->pix_fmt != PIX_FMT_YUV420P) {
        tmp_picture = alloc_picture(PIX_FMT_YUV420P, c->width, c->height);
        if (!tmp_picture) {
            fprintf(stderr, "Could not allocate temporary picture\n");
            exit(1);
        }
    }
}

/* prepare a dummy image */
static void fill_yuv_image(AVFrame *pict, int frame_index, int width, int height)
{
    printf("%s pict = %p\r\n",__func__,pict);
    int x, y, i;

    i = frame_index;

    /* Y */
    for(y=0;y<height;y++) {
        for(x=0;x<width;x++) {
            pict->data[0][y * pict->linesize[0] + x] = x + y + i * 3;
        }
    }

    /* Cb and Cr */
    for(y=0;y<height/2;y++) {
        for(x=0;x<width/2;x++) {
            pict->data[1][y * pict->linesize[1] + x] = 128 + y + i * 2;
            pict->data[2][y * pict->linesize[2] + x] = 64 + x + i * 5;
        }
    }
}

static void write_video_frame(AVFormatContext *oc, AVStream *st)
{
    int out_size, ret;
    AVCodecContext *c;
    static struct SwsContext *img_convert_ctx;

    c = st->codec;

    if (frame_count >= STREAM_NB_FRAMES) {
        /* no more frame to compress. The codec has a latency of a few
           frames if using B frames, so we get the last frames by
           passing the same picture again */
    } else {
        if (c->pix_fmt != PIX_FMT_YUV420P) {
            /* as we only generate a YUV420P picture, we must convert it
               to the codec pixel format if needed */
            if (img_convert_ctx == NULL) {
                img_convert_ctx = sws_getContext(c->width, c->height,
                                                 PIX_FMT_YUV420P,
                                                 c->width, c->height,
                                                 c->pix_fmt,
                                                 sws_flags, NULL, NULL, NULL);
                if (img_convert_ctx == NULL) {
                    fprintf(stderr, "Cannot initialize the conversion context\n");
                    exit(1);
                }
            }
            fill_yuv_image(tmp_picture, frame_count, c->width, c->height);
            sws_scale(img_convert_ctx, tmp_picture->data, tmp_picture->linesize,
                      0, c->height, picture->data, picture->linesize);
        } else {
            fill_yuv_image(picture, frame_count, c->width, c->height);
        }
    }


    if (oc->oformat->flags & AVFMT_RAWPICTURE) {
        printf("AVFMT_RAWPICTURE\r\n");
        /* raw video case. The API will change slightly in the near
           futur for that */
        AVPacket pkt;
        av_init_packet(&pkt);

        pkt.flags |= 1;
        pkt.stream_index= st->index;
        pkt.data= (uint8_t *)picture;
        pkt.size= sizeof(AVPicture);

        ret = av_write_frame(oc, &pkt);
    } else {
        printf("encode the image\r\n");
        /* encode the image */
        printf("c->pix_fmt=%d\r\n",c->pix_fmt);
        printf("c->width=%d\r\n",c->width);
        printf("c->height=%d\r\n",c->height);
        picture = alloc_picture(c->pix_fmt, c->width, c->height);
        printf("picture->pix_fmt=%d\r\n",picture->format);
        printf("picture->width=%d\r\n",picture->width);
        printf("picture->height=%d\r\n",picture->height);
        out_size = avcodec_encode_video(c, video_outbuf, video_outbuf_size, picture);
        /* if zero size, it means the image was buffered */
        if (out_size > 0) {
            AVPacket pkt;
            av_init_packet(&pkt);

            pkt.pts= av_rescale_q(c->coded_frame->pts, c->time_base, st->time_base);
            if(c->coded_frame->key_frame)
                pkt.flags |= 1;
            pkt.stream_index= 0;
            pkt.data= video_outbuf;
            pkt.size= out_size;

            /* write the compressed frame in the media file */
            ret = av_write_frame(oc, &pkt);
        } else {
            ret = 0;
        }
    }
    if (ret != 0) {
        fprintf(stderr, "Error while writing video frame\n");
        exit(1);
    }
    frame_count++;
}

static void close_video(AVFormatContext *oc, AVStream *st)
{
    avcodec_close(st->codec);
    av_free(picture->data[0]);
    av_free(picture);
    if (tmp_picture) {
        av_free(tmp_picture->data[0]);
        av_free(tmp_picture);
    }
    av_free(video_outbuf);
}

/**************************************************************/
/* media file output */

int main(int argc, char **argv)
{
    const char *filename;
    AVOutputFormat *fmt;
    AVFormatContext *oc;
    AVStream *audio_st, *video_st;
    double audio_pts, video_pts;
    int i;

    /* initialize libavcodec, and register all codecs and formats */
    av_register_all();

    /* auto detect the output format from the name. default is
       mpeg. */
    fmt = av_guess_format(NULL, "test.mpg", NULL);
    if(fmt == nullptr){
        printf("Found no format\r\n");
    }else{
        printf("Found format[%s]\r\n",fmt->name);
    }
    /* allocate the output media context */
    avformat_alloc_output_context2(&oc,fmt,"mpeg","test.mpg");
//    if (!oc) {
//        fprintf(stderr, "Memory error\n");
//        exit(1);
//    }
//    oc->oformat = fmt;
//    snprintf(oc->filename, sizeof(oc->filename), "%s", filename);

    /* add the audio and video streams using the default format codecs
       and initialize the codecs */
    video_st = NULL;
    audio_st = NULL;
    video_st = add_video_stream(oc, fmt->video_codec);
    if (video_st)
        open_video(oc, video_st);

    /* open the output file, if needed */
//    if (!(fmt->flags & AVFMT_NOFILE)) {
//        if (url_fopen(&oc->pb, filename, URL_WRONLY) < 0) {
//            fprintf(stderr, "Could not open '%s'\n", filename);
//            exit(1);
//        }
//    }

    /* write the stream header, if any */
//    av_write_header(oc);

    for(;;) {
        /* compute current audio and video time */
        if (video_st)
            video_pts = (double)video_st->pts.val * video_st->time_base.num / video_st->time_base.den;
        else
            video_pts = 0.0;

        if ((!audio_st || audio_pts >= STREAM_DURATION) &&
            (!video_st || video_pts >= STREAM_DURATION))
            break;

        /* write interleaved audio and video frames */
        if (!video_st || (video_st && audio_st && audio_pts < video_pts)) {
//            write_audio_frame(oc, audio_st);
        } else {
            write_video_frame(oc, video_st);
        }
    }

    /* close each codec */
    if (video_st)
        close_video(oc, video_st);

    /* write the trailer, if any */
    av_write_trailer(oc);

    /* free the streams */
    for(i = 0; i < oc->nb_streams; i++) {
        av_freep(&oc->streams[i]->codec);
        av_freep(&oc->streams[i]);
    }

    if (!(fmt->flags & AVFMT_NOFILE)) {
        /* close the output file */
//        url_fclose(&oc->pb);
    }

    /* free the stream */
    av_free(oc);

    return 0;
}
