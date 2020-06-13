//#include <gst/gst.h>
//#include <gst/app/gstappsrc.h>
//#include <iostream>
//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>

//GST_DEBUG_CATEGORY (appsrc_pipeline_debug);
//#define GST_CAT_DEFAULT appsrc_pipeline_debug

//typedef struct _App App;

//struct _App
//{
//  GstElement *pipeline;
//  GstElement *appsrc;

//  GMainLoop *loop;
//  guint sourceid;

//  GTimer *timer;
//  int count = 0;
//  float m_frameRate = 30;
//};

//App s_app;

//static gboolean
//read_data (App * app)
//{
//    printf("Push klv[%d]\r\n",app->count);
////    return false;
//    uint8_t data[] = {
//                0X06,0X0E,0X2B,0X34,0X02,0X0B,0X01,0X01,0X0E,0X01,0X03,0X01,0X01,0X00,0X00,0X00,0X81,0XF1,0X02,0X08,0X00,0X04,0XCA,0X14,0X28,0X1F,0X99,0X6A,0X03,0X15,0X45,0X53,0X52,0X49,0X5F,0X4D,0X65,0X74,0X61,0X64,0X61,0X74,0X61,0X5F,0X43,0X6F,0X6C,0X6C,0X65,0X63,0X74,0X04,0X06,0X4E,0X39,0X37,0X38,0X32,0X36,0X05,0X02,0X6D,0XDE,0X06,0X02,0X11,0XDA,0X07,0X02,0XDD,0X0B,0X0A,0X05,0X43,0X32,0X30,0X38,0X42,0X0B,0X00,0X0C,0X00,0X0D,0X04,0X3A,0X72,0X2D,0X65,0X0E,0X04,0XB5,0X6D,0X0B,0XC0,0X0F,0X02,0X31,0X4D,0X10,0X02,0X04,0X5C,0X11,0X02,0X02,0X73,0X12,0X04,0XB6,0X84,0X44,0X45,0X13,0X04,0XF6,0X2C,0X16,0XC1,0X14,0X04,0X00,0X00,0X00,0X00,0X15,0X04,0X00,0X1E,0X06,0X38,0X16,0X02,0X00,0X00,0X17,0X04,0X3A,0X76,0X5E,0XEB,0X18,0X04,0XB5,0X70,0X74,0XAC,0X19,0X02,0X23,0X9E,0X1A,0X02,0X01,0X7E,0X1B,0X02,0X00,0X65,0X1C,0X02,0XFF,0XFE,0X1D,0X02,0X02,0X03,0X1E,0X02,0XFE,0X98,0X1F,0X02,0XFF,0XA1,0X20,0X02,0X00,0X03,0X21,0X02,0XFE,0X1A,0X2F,0X01,0X00,0X30,0X2A,0X01,0X01,0X01,0X02,0X01,0X01,0X03,0X04,0X2F,0X2F,0X43,0X41,0X04,0X00,0X05,0X00,0X06,0X02,0X43,0X41,0X15,0X10,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X16,0X02,0X00,0X05,0X38,0X01,0X00,0X3B,0X08,0X46,0X69,0X72,0X65,0X62,0X69,0X72,0X64,0X41,0X01,0X01,0X48,0X08,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X02,0XDD,0XBE,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X61,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
//            };
//    GstBuffer *buffer = gst_buffer_new_allocate(nullptr, sizeof (data), nullptr);
//    GstMapInfo map;
//    GstClock *clock;
//    GstClockTime abs_time, base_time;

//    gst_buffer_map (buffer, &map, GST_MAP_WRITE);
//    memcpy(map.data, data, sizeof (data));
//    gst_buffer_unmap (buffer, &map);

//    GST_OBJECT_LOCK (app->appsrc);
//    clock = GST_ELEMENT_CLOCK (app->appsrc);
//    base_time = GST_ELEMENT (app->appsrc)->base_time;
//    gst_object_ref (clock);
//    GST_OBJECT_UNLOCK (app->appsrc);
//    abs_time = gst_clock_get_time (clock);
//    gst_object_unref (clock);

//    GST_BUFFER_PTS (buffer) = abs_time - base_time;
//    GST_BUFFER_DURATION (buffer) = GST_SECOND / app->m_frameRate;

//    gst_app_src_push_buffer(GST_APP_SRC(app->appsrc), buffer);
//    int ms = 1000 / app->m_frameRate * 5;
//    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
//    nanosleep(&ts, NULL);
////    GstClockTime gstDuration = GST_SECOND / app->m_frameRate;
////    GST_BUFFER_PTS (buffer) = (app->count + 1) * gstDuration;
////    GST_BUFFER_DURATION (buffer) = GST_SECOND / 30;
////    gst_app_src_push_buffer(GST_APP_SRC(app->appsrc), buffer);
//    app->count++;
//    return true;
//}

///* This signal callback is called when appsrc needs data, we add an idle handler
// * to the mainloop to start pushing data into the appsrc */
//static void
//start_feed (GstElement * pipeline, guint size, App * app)
//{
//  if (app->sourceid == 0) {
//    GST_DEBUG ("start feeding");
//    app->sourceid = g_idle_add ((GSourceFunc) read_data, app);
//  }
//}
//static gboolean seek_data (GstElement* object,
//                                         guint64 arg0,
//                           App * app){
//    printf("%s\r\n",__func__);
//}
///* This callback is called when appsrc has enough data and we can stop sending.
// * We remove the idle handler from the mainloop */
//static void
//stop_feed (GstElement * pipeline, App * app)
//{
//  if (app->sourceid != 0) {
//    GST_DEBUG ("stop feeding");
//    g_source_remove (app->sourceid);
//    app->sourceid = 0;
//  }
//  printf("Enough feed[%d]\r\n",app->count);
//}

//static gboolean
//bus_message (GstBus * bus, GstMessage * message, App * app)
//{
//  GST_DEBUG ("got message %s",
//      gst_message_type_get_name (GST_MESSAGE_TYPE (message)));

//  switch (GST_MESSAGE_TYPE (message)) {
//    case GST_MESSAGE_ERROR: {
//        GError *err = NULL;
//        gchar *dbg_info = NULL;

//        gst_message_parse_error (message, &err, &dbg_info);
//        g_printerr ("ERROR from element %s: %s\n",
//            GST_OBJECT_NAME (message->src), err->message);
//        g_printerr ("Debugging info: %s\n", (dbg_info) ? dbg_info : "none");
//        g_error_free (err);
//        g_free (dbg_info);
//        g_main_loop_quit (app->loop);
//        break;
//    }
//    case GST_MESSAGE_EOS:
//      g_main_loop_quit (app->loop);
//      break;
//    default:
//      break;
//  }
//  return TRUE;
//}

//int
//main (int argc, char *argv[])
//{
//  App *app = &s_app;
//  GError *error = NULL;
//  GstBus *bus;
//  GstCaps *caps;

//  gst_init (&argc, &argv);

//  GST_DEBUG_CATEGORY_INIT (appsrc_pipeline_debug, "appsrc-pipeline", 0,
//      "appsrc pipeline example");

//  /* create a mainloop to get messages and to handle the idle handler that will
//   * feed data to appsrc. */
//  app->loop = g_main_loop_new (NULL, TRUE);
//  app->timer = g_timer_new();

//  app->pipeline = gst_parse_launch("videotestsrc ! video/x-raw,format=I420,width=640,height=480,framerate=30/1 ! x264enc ! queue ! mpegtsmux name=mux ! filesink location=test.ts appsrc stream-type=0 name=mysource ! queue ! mux.", NULL);
//  g_assert (app->pipeline);

//  bus = gst_pipeline_get_bus (GST_PIPELINE (app->pipeline));
//  g_assert(bus);

//  /* add watch for messages */
//  gst_bus_add_watch (bus, (GstBusFunc) bus_message, app);

//  /* get the appsrc */
//    app->appsrc = gst_bin_get_by_name (GST_BIN(app->pipeline), "mysource");
//    g_assert(app->appsrc);
//    g_assert(GST_IS_APP_SRC(app->appsrc));
//    g_signal_connect (app->appsrc, "seek-data", G_CALLBACK (seek_data), app);
//    g_signal_connect (app->appsrc, "need-data", G_CALLBACK (start_feed), app);
//    g_signal_connect (app->appsrc, "enough-data", G_CALLBACK (stop_feed), app);

//  /* set the caps on the source */
//  caps = gst_caps_new_simple ("meta/x-klv",
//     "parsed", G_TYPE_BOOLEAN, TRUE,
//    nullptr);
//   gst_app_src_set_caps(GST_APP_SRC(app->appsrc), caps);
//   g_object_set(GST_APP_SRC(app->appsrc), "format", GST_FORMAT_TIME, nullptr);


//  /* go to playing and wait in a mainloop. */
//  gst_element_set_state (app->pipeline, GST_STATE_PLAYING);

//  /* this mainloop is stopped when we receive an error or EOS */
//  g_main_loop_run (app->loop);

//  GST_DEBUG ("stopping");

//  gst_element_set_state (app->pipeline, GST_STATE_NULL);

//  gst_object_unref (bus);
//  g_main_loop_unref (app->loop);

//  return 0;
//}
