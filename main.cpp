//#include <string.h>
//#include <gst/gst.h>
//#include <signal.h>
//#include <unistd.h>
//#include <stdlib.h>
//#include <iostream>

//static GMainLoop *loop;
//static GstElement *pipeline, *appsrc, *src, *tee, *encoder, *muxer, *filesink, *videoconvert, *videosink, *queue_record, *queue_display, *caps_filter, *capsfilter_klv;
//static GstBus *bus;

///* KLV data from Day_Flight.mpg */
//static const guint8 rtp_KLV_frame_data[] = {
//    0x06, 0x0e, 0x2b, 0x34, 0x02, 0x0b, 0x01, 0x01,
//    0x0e, 0x01, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00,
//    0x81, 0x91, 0x02, 0x08, 0x00, 0x04, 0x6c, 0x8e,
//    0x20, 0x03, 0x83, 0x85, 0x41, 0x01, 0x01, 0x05,
//    0x02, 0x3d, 0x3b, 0x06, 0x02, 0x15, 0x80, 0x07,
//    0x02, 0x01, 0x52, 0x0b, 0x03, 0x45, 0x4f, 0x4e,
//    0x0c, 0x0e, 0x47, 0x65, 0x6f, 0x64, 0x65, 0x74,
//    0x69, 0x63, 0x20, 0x57, 0x47, 0x53, 0x38, 0x34,
//    0x0d, 0x04, 0x4d, 0xc4, 0xdc, 0xbb, 0x0e, 0x04,
//    0xb1, 0xa8, 0x6c, 0xfe, 0x0f, 0x02, 0x1f, 0x4a,
//    0x10, 0x02, 0x00, 0x85, 0x11, 0x02, 0x00, 0x4b,
//    0x12, 0x04, 0x20, 0xc8, 0xd2, 0x7d, 0x13, 0x04,
//    0xfc, 0xdd, 0x02, 0xd8, 0x14, 0x04, 0xfe, 0xb8,
//    0xcb, 0x61, 0x15, 0x04, 0x00, 0x8f, 0x3e, 0x61,
//    0x16, 0x04, 0x00, 0x00, 0x01, 0xc9, 0x17, 0x04,
//    0x4d, 0xdd, 0x8c, 0x2a, 0x18, 0x04, 0xb1, 0xbe,
//    0x9e, 0xf4, 0x19, 0x02, 0x0b, 0x85, 0x28, 0x04,
//    0x4d, 0xdd, 0x8c, 0x2a, 0x29, 0x04, 0xb1, 0xbe,
//    0x9e, 0xf4, 0x2a, 0x02, 0x0b, 0x85, 0x38, 0x01,
//    0x2e, 0x39, 0x04, 0x00, 0x8d, 0xd4, 0x29, 0x01,
//    0x02, 0x1c, 0x5f
//};

//static gboolean
//message_cb (GstBus * bus, GstMessage * message, gpointer user_data)
//{
//    switch (GST_MESSAGE_TYPE (message)) {
//    case GST_MESSAGE_ERROR:{
//        g_main_loop_quit (loop);

//        GError *err = NULL;
//        gchar *name, *debug = NULL;
//        name = gst_object_get_path_string (message->src);
//        gst_message_parse_error (message, &err, &debug);

//        std::string err_message = err->message;
//        if(err_message == "Output window was closed"){
//            std::cout << std::endl << err_message << std::endl;
//        }
//        else{
//            g_printerr ("\nERROR: from element %s: %s\n", name, err->message);
//            if (debug != NULL)
//                g_printerr ("Additional debug info:\n%s\n", debug);
//        }

//        g_error_free (err);
//        g_free (debug);
//        g_free (name);
//        break;
//    }
//    case GST_MESSAGE_WARNING:{
//        GError *err = NULL;
//        gchar *name, *debug = NULL;

//        name = gst_object_get_path_string (message->src);
//        gst_message_parse_warning (message, &err, &debug);

//        g_printerr ("ERROR: from element %s: %s\n", name, err->message);
//        if (debug != NULL)
//            g_printerr ("Additional debug info:\n%s\n", debug);

//        g_error_free (err);
//        g_free (debug);
//        g_free (name);
//        break;
//    }
//    case GST_MESSAGE_EOS: {
//        g_print ("Got EOS\n");
//        g_main_loop_quit(loop);
//        break;
//    }
//    default:
//        break;
//    }

//    return TRUE;
//}
//void need_data(GstElement* element, guint, gpointer userData)
//{
//    std::cout << "need_data" << std::endl;
//    // TODO push binary data (rtp_KLV_frame_data) in Big-Endian.
//    GstBuffer *buffer = gst_buffer_new();
//    GstMapInfo map;

//    gst_buffer_map(buffer, &map, GST_MAP_WRITE);
//    map.data = (guint8*)g_memdup(rtp_KLV_frame_data, sizeof (rtp_KLV_frame_data));
//    map.size = sizeof (rtp_KLV_frame_data);
//    gst_buffer_unmap (buffer, &map);

//    GST_BUFFER_TIMESTAMP(buffer) = appsrc->base_time;
//    GST_BUFFER_DURATION (buffer) = gst_util_uint64_scale_int (1, GST_SECOND, 1);
//    appsrc->base_time += GST_BUFFER_DURATION (buffer);
//}
//void sigintHandler(int unused)
//{
//    g_print("You ctrl-c-ed!");
//    gst_element_send_event(pipeline, gst_event_new_eos());
//}
//int main(int argc, char *argv[])
//{
//    signal(SIGINT, sigintHandler);
//    gst_init (&argc, &argv);

//    guint major, minor, micro, nano;
//    gst_version (&major, &minor, &micro, &nano);
//    std::string nano_str = nano==1 ? "(CVS)" : (nano==2 ? "(Prerelease)" : "");
//    std::cout << "This program is linked against GStreamer " << major << "." << minor<< "." << micro << " " << nano_str << std::endl;

//    pipeline = gst_pipeline_new(NULL);
//    appsrc = gst_element_factory_make ("appsrc", "source");
//    src = gst_element_factory_make("v4l2src", NULL);
//    tee = gst_element_factory_make("tee", "tee");
//    encoder = gst_element_factory_make("x264enc", NULL);
//    muxer = gst_element_factory_make("mpegtsmux", NULL);
//    filesink = gst_element_factory_make("filesink", NULL);
//    videoconvert = gst_element_factory_make("videoconvert", NULL);
//    videosink = gst_element_factory_make("autovideosink", NULL);
//    queue_display = gst_element_factory_make("queue", "queue_display");
//    queue_record = gst_element_factory_make("queue", "queue_record");
//    caps_filter  = gst_element_factory_make("capsfilter", NULL);
//    capsfilter_klv = gst_element_factory_make("capsfilter", NULL);

//    if (!pipeline || !src || !tee || !encoder || !muxer || !filesink || !videoconvert || !videosink || !queue_record || !queue_display) {
//        std::cerr << "Failed to create elements:"
//                  << (!pipeline? " pipeline " : "")
//                  << (!src? " v4l2src " : "")
//                  << (!tee? " tee " : "")
//                  << (!encoder? " x264enc " : "")
//                  << (!muxer? " mpegtsmux " : "")
//                  << (!filesink? " filesink " : "")
//                  << (!videoconvert? " videoconvert " : "")
//                  << (!videosink? " autovideosink " : "")
//                  << (!queue_display? " queue_display " : "")
//                  << (!queue_record? " queue_record " : "")
//                  << std::endl;
//        return -1;
//    }

//    // Create caps
//    /*GstCaps *video_cap = gst_caps_new_simple ("video/x-raw",
//                         "width", G_TYPE_INT, 640,
//                         "height", G_TYPE_INT, 480,
//                         "framerate", GST_TYPE_FRACTION, 30, 1,
//                         NULL);*/
//    GstCaps *klv_caps = gst_caps_new_simple("meta/x-klv", "parsed", G_TYPE_BOOLEAN, TRUE, "sparse", G_TYPE_BOOLEAN, TRUE, nullptr);
//    GstCaps *caps = gst_caps_new_full (
//                gst_structure_new ("video/x-raw",
//                                   "width", G_TYPE_INT, 640,
//                                   "height", G_TYPE_INT, 480,
//                                   "framerate", GST_TYPE_FRACTION, 30, 1,
//                                   NULL),
//                gst_structure_new ("meta/x-klv",
//                                   "parsed", G_TYPE_BOOLEAN, TRUE,
//                                   "sparse", G_TYPE_BOOLEAN, TRUE,
//                                   NULL),
//                NULL);

//    g_object_set(appsrc, "caps", klv_caps, nullptr);
//    g_object_set(appsrc, "format", GST_FORMAT_TIME, nullptr);

//    g_object_set (src, "device", "/dev/video0", NULL);
//    g_object_set (encoder, "noise-reduction", 10000, NULL);
//    //g_object_set (encoder, "tune", "Zero latency", NULL);
//    g_object_set (encoder, "threads", 4, NULL);
//    g_object_set(filesink, "location", "rec.mpg", NULL);
//    g_object_set(filesink, "async", 0, NULL);
//    g_object_set(caps_filter, "caps", caps, NULL);
//    //g_object_set(capsfilter_klv, "caps", klv_caps, NULL);
//    gst_caps_unref(caps);
//    //gst_caps_unref(klv_caps);

//    gst_bin_add_many(GST_BIN(pipeline), src, appsrc, caps_filter, tee, queue_record, encoder, muxer, filesink, queue_display, videoconvert, videosink, NULL);
//    if (!gst_element_link_many(src, tee, caps_filter, NULL)
//            || !gst_element_link_many(tee, queue_record, encoder, muxer, filesink, NULL)
//            || !gst_element_link_many(tee, queue_display, videoconvert, videosink, NULL)) {
//        g_error("Failed to link elements");
//        return -2;
//    }

//    gst_element_set_state(pipeline, GST_STATE_PLAYING);
//    if (gst_element_get_state (pipeline, NULL, NULL, -1) == GST_STATE_CHANGE_FAILURE) {
//        g_error("Failed to go into PLAYING state");
//    }

//    bus = gst_pipeline_get_bus(GST_PIPELINE (pipeline));
//    gst_bus_add_signal_watch(bus);
//    g_signal_connect(G_OBJECT(bus), "message", G_CALLBACK(message_cb), NULL);
//    //g_signal_connect(bus, "need-data", G_CALLBACK(need_data), NULL);
//    gst_object_unref(GST_OBJECT(bus));

//    g_signal_connect(appsrc, "need-data", G_CALLBACK(need_data), NULL);

//    loop = g_main_loop_new(NULL, FALSE);
//    g_print("Starting loop");
//    g_main_loop_run(loop);

//    // Free resources
//    gst_object_unref (bus);
//    gst_element_set_state (pipeline, GST_STATE_NULL);
//    gst_object_unref (pipeline);

//    return 0;
//}

//// Thanks!
