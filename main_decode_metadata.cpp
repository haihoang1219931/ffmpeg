//#include <stdio.h>
//#include <string.h>
//#include <gst/gst.h>
//#include <gst/app/gstappsink.h>
//#include <iostream>
//#include <sstream>
//#define QUEUE_SIZE      100
//GstFlowReturn read_frame_buffer(GstAppSink *sink, gpointer user_data){
//    printf("Metadata here\r\n");
//    return GstFlowReturn::GST_FLOW_OK;
//}
//GstPadProbeReturn pad_data_mod(GstPad *pad, GstPadProbeInfo *info, gpointer user_data)
//{
//    if(info->size!= 18800){
//        printf("Metadata here\r\n");
//        printf("info->type=%d info->id=%ld info->size=%d info->offset=%ld\r\n",
//               info->type,
//               info->id,
//               info->size,
//               info->offset);
//    }
//    return GstPadProbeReturn::GST_PAD_PROBE_OK;
//}
//int main(int argc, char** argv){
//    gst_init(&argc,&argv);
//    GError *err = nullptr;
//    GMainLoop *loop  = nullptr;
//    GstPipeline *pipeline = nullptr;
//    GstElement *vsink = nullptr;
//    GstElement *mPipeline = nullptr;
//    loop = g_main_loop_new(nullptr, FALSE);
//    // launch pipeline
//    std::ostringstream ss;
//    // decode 02
//    //    ss << "udpsrc multicast-group="<<m_ip<<" port="<<m_port<<" ! application/x-rtp,media=video,clock-rate=90000,encoding-name=MP2T-ES ! ";
//    //    ss << "rtpjitterbuffer mode=synced ! rtpmp2tdepay ! video/mpegts ! tsdemux ! queue name=myqueue ! tee name=t t. ! h265parse ! video/x-h265,stream-format=byte-stream ! avdec_h265 max-threads=4 ! ";
//    //    ss << "appsink  name=mysink sync=false async=false ";
//    //    ss << "rtspsrc location="<<m_source<<" ! application/x-rtp,media=video,clock-rate=90000,encoding-name=H264 ! ";
//    //    ss << "rtpjitterbuffer ! rtph264depay ! tee name=t t. ! queue ! h264parse ! avdec_h264 ! ";
//    //    ss << "appsink name=mysink sync=false async=false ";
//    //    ss << "t. ! queue ! mpegtsmux ! filesink location="<<m_logFile<<".ts";
//    ss << "filesrc location=/home/hainh/Desktop/Video/stream_20180925.ts ! tsdemux name=demux ";
////    ss << "demux. ! video/x-h264 ! h264parse ! avdec_h264 ! appsink name=videosink ";
//    ss << "demux. ! meta/x-klv ! appsink name=metadatasink";
//    std::cout << ss.str().c_str() << std::endl;
//    mPipeline = gst_parse_launch(ss.str().c_str(), &err);

//    if (err != nullptr) {
//        g_print("gstreamer decoder failed to create pipeline\n");
//        g_error_free(err);
//        return FALSE;
//    } else {
//        g_print("gstreamer decoder create pipeline success\n");
//    }

//    pipeline = GST_PIPELINE(mPipeline);

//    if (!pipeline) {
//        printf("gstreamer failed to cast GstElement into GstPipeline\n");
//        return FALSE;
//    } else {
//        g_print("gstreamer decoder create Gstpipeline success\n");
//    }

//    GstElement *m_sink = gst_bin_get_by_name((GstBin *)mPipeline, "metadatasink");
//    GstAppSink *m_appsink = (GstAppSink *)m_sink;

//    if (!m_sink || !m_appsink) {
//#ifdef DEBUG
//        g_print("Fail to get element \n");
//#endif
//        return FALSE;
//    }

//    gst_app_sink_set_drop(m_appsink, true);
//    g_object_set(m_appsink, "emit-signals", TRUE, nullptr);
//    //    gst_pipeline_use_clock(pipeline, nullptr);
//    //    GstCaps *caps = gst_caps_from_string("video/x-raw, format=I420");
//    //    gst_app_sink_set_caps(m_appsink, caps);
//    //    gst_caps_unref(caps);
//    // add call back received video data
//    GstAppSinkCallbacks cbs;
//    memset(&cbs, 0, sizeof(GstAppSinkCallbacks));
//    cbs.new_sample = read_frame_buffer;
//    gst_app_sink_set_callbacks(m_appsink, &cbs, nullptr, nullptr);

////    GstElement* demux = gst_bin_get_by_name(GST_BIN(mPipeline), "demux");
////    GstPad *padKLV = gst_element_get_static_pad(demux,"sink");
////    gst_pad_add_probe (padKLV, GST_PAD_PROBE_TYPE_BUFFER,
////            (GstPadProbeCallback) pad_data_mod, nullptr, nullptr);
////    gst_object_unref (padKLV);
//    const GstStateChangeReturn result = gst_element_set_state(mPipeline, GST_STATE_PLAYING);

//    if (result != GST_STATE_CHANGE_SUCCESS) {
//        g_print("gstreamer failed to playing\n");
//    }

//    g_main_loop_run(loop);
//    gst_element_set_state(GST_ELEMENT(mPipeline), GST_STATE_NULL);
//    //    g_object_unref(m_sink);
//    //    g_object_unref(m_appsink);
//    g_object_unref(mPipeline);
//    //    g_main_loop_unref(loop);
//    //    g_object_unref(pipeline);
//    printf("gstreamer setup done\n");
//    return 0;
//}
