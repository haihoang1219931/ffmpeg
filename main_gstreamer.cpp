#include <gst/gst.h>
#include <gst/app/gstappsrc.h>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ByteManipulation.h"

GST_DEBUG_CATEGORY (appsrc_pipeline_debug);
#define GST_CAT_DEFAULT appsrc_pipeline_debug



typedef struct _App App;

struct _App
{
  GstElement *pipeline;
  GstElement *appsrc;

  GMainLoop *loop;
  guint sourceid;

  GTimer *timer;
  int count = 0;
  float m_frameRate = 30;
};
class Klv{
public:
    Klv(uint8_t key, uint8_t length, std::vector<uint8_t> value){
        m_key = key;
        m_length = length;
        m_value = value;
        encode();
    }
    Klv(uint8_t key, uint8_t length, uint8_t* value){
        m_key = key;
        m_length = length;
        m_value = std::vector<uint8_t>(value,value+length);
        encode();
    }
    ~Klv(){}
    void encode(){
        m_encoded.clear();
        m_encoded.push_back(m_key);
        m_encoded.push_back(m_length);
        m_encoded.insert(m_encoded.end(),m_value.begin(),m_value.end());
        printf("[KE]");
        for(int i=0; i< m_encoded.size(); i++){
            printf("0X%02X,",m_encoded[i]);
        }
        printf("\r\n");
    }
public:
    uint8_t m_key;
    uint8_t m_length;
    std::vector<uint8_t> m_value;
    std::vector<uint8_t> m_encoded;
};
unsigned short bcc_16 (
        unsigned char * buff, // Pointer to the first byte in the 16-byte UAS Datalink LS key.
        unsigned short len ) //Length from 16-byte US key up to 1-byte checksum length.
{
    unsigned short bcc = 0, i; // Initialize Checksum and counter variables.
    for ( i = 0 ; i < len; i++)
        bcc += buff[i] << (8 * ((i + 1) % 2));
    return bcc;
} // end of bcc_16 ()
std::vector<uint8_t> encodeMeta(){
    printf("Encode data\r\n");
    uint8_t keyST0601[] = {0X06,0X0E,0X2B,0X34,0X02,0X0B,0X01,0X01,0X0E,0X01,0X03,0X01,0X01,0X00,0X00,0X00};
    int totalLength = 0X81F1;
    uint16_t checkSum = 0;
    int localHour,gmtHour;
    time_t t = time(0);
    struct tm * lcl = localtime(&t);
    localHour = lcl->tm_hour;
    struct tm * gmt = gmtime(&t);
    gmtHour = gmt->tm_hour;
    uint64_t timestamp = (t + (localHour-gmtHour)*3600)*1000000;
    std::string missionID = "ESRI_Metadata_Collect";
    std::string platformTailNumber = "N97826";
    float m_platformHeadingAngle = 157.601288f;
    float m_platformPitchAngle = 3.390606f;
    float m_platformRollAngle = -6.491287f;
    std::string m_platformDesignation = "C208B";
    float m_sensorLatitude = 41.095737f;
    float m_sensorLongitude = -104.870224f;
    float m_sensorAltitude = 2933.031250f;
    float m_sensorHFOV = 3.065233f;
    float m_sensorVFOV = 1.722133f;
    float m_sensorRelativeAzimuth = 254.250000f;
    float m_sensorRelativeElevation = -20.382812f;
    float m_sensorRelativeRoll = 0.000000f;
    float m_slantRanged = 2291.890625f;
    float m_frameCenterLatitude = 41.106804f;
    float m_frameCenterLongitude = -104.851006f;
    float m_frameCenterElevation = 1867.203613f;
    float m_offsetCornerLatitudePoint1 = 0.000867f;
    float m_offsetCornerLongitudePoint1 = 0.000268f;
    float m_offsetCornerLatitudePoint2 = -0.000034f;
    float m_offsetCornerLongitudePoint2 = 0.001179f;
    float m_offsetCornerLatitudePoint3 = -0.000817f;
    float m_offsetCornerLongitudePoint3 = -0.000249f;
    float m_offsetCornerLatitudePoint4 = 0.000034f;
    float m_offsetCornerLongitudePoint4 = -0.001110f;

    std::vector<uint8_t> metaData;

    Klv kTimeStamp(0x02,8,ByteManipulation::ToBytes(timestamp,Endianness::Big));
    metaData.insert(metaData.end(),kTimeStamp.m_encoded.begin(),kTimeStamp.m_encoded.end());

    Klv kMissionID(0x03,static_cast<uint8_t>(missionID.length()),(uint8_t*)missionID.c_str());
    metaData.insert(metaData.end(),kMissionID.m_encoded.begin(),kMissionID.m_encoded.end());

    Klv kPlatformTailNumber(0x04,static_cast<uint8_t>(platformTailNumber.length()),(uint8_t*)platformTailNumber.c_str());
    metaData.insert(metaData.end(),kPlatformTailNumber.m_encoded.begin(),kPlatformTailNumber.m_encoded.end());

    unsigned short platformHeading = static_cast<unsigned short>(m_platformHeadingAngle*(65536-1)/360.0f);
    Klv kPlatformHeadAngle(0x05,2,ByteManipulation::ToBytes(
                               platformHeading,Endianness::Little));
    metaData.insert(metaData.end(),kPlatformHeadAngle.m_encoded.begin(),kPlatformHeadAngle.m_encoded.end());

    unsigned short platformPitch = static_cast<unsigned short>((m_platformPitchAngle+20)*(65534)/40.0f-32767);
    Klv kPlatformPitchAngle(0x06,2,ByteManipulation::ToBytes(
                                platformPitch,Endianness::Little));
    metaData.insert(metaData.end(),kPlatformPitchAngle.m_encoded.begin(),kPlatformPitchAngle.m_encoded.end());

    unsigned short platformRoll = static_cast<unsigned short>((m_platformRollAngle+50)*(65534)/100.0f-32767);
    Klv kPlatformRollAngle(0x07,2,ByteManipulation::ToBytes(
                               platformRoll,Endianness::Little));
    metaData.insert(metaData.end(),kPlatformRollAngle.m_encoded.begin(),kPlatformRollAngle.m_encoded.end());

    Klv kDesignation(0x0A,static_cast<uint8_t>(m_platformDesignation.length()),(uint8_t*)m_platformDesignation.c_str());
    metaData.insert(metaData.end(),kDesignation.m_encoded.begin(),kDesignation.m_encoded.end());

    int sensorLatitude = static_cast<int>((m_sensorLatitude)*(4294967294)/180.0f);
    Klv kSensorLatitude(0x0D,4,ByteManipulation::ToBytes(
                            sensorLatitude,Endianness::Little));
    metaData.insert(metaData.end(),kSensorLatitude.m_encoded.begin(),kSensorLatitude.m_encoded.end());

    int sensorLongitude = static_cast<int>((m_sensorLongitude)*(4294967294)/360.0f);
    Klv kSensorLongitude(0x0E,4,ByteManipulation::ToBytes(
                             sensorLongitude,Endianness::Little));
    metaData.insert(metaData.end(),kSensorLongitude.m_encoded.begin(),kSensorLongitude.m_encoded.end());

    unsigned short sensorAltitude = static_cast<unsigned short>((m_sensorAltitude+900)*(65535)/19900.0f);
    Klv kSensorAltitude(0x0F,2,ByteManipulation::ToBytes(
                            sensorAltitude,Endianness::Little));
    metaData.insert(metaData.end(),kSensorAltitude.m_encoded.begin(),kSensorAltitude.m_encoded.end());

    unsigned short sensorHFOV = static_cast<unsigned short>((m_sensorHFOV)*(65535)/180.0f);
    Klv kSensorHFOV(0x10,2,ByteManipulation::ToBytes(
                        sensorHFOV,Endianness::Little));
    metaData.insert(metaData.end(),kSensorHFOV.m_encoded.begin(),kSensorHFOV.m_encoded.end());

    unsigned short sensorVFOV = static_cast<unsigned short>((m_sensorVFOV)*(65535)/180.0f);
    Klv kSensorVFOV(0x11,2,ByteManipulation::ToBytes(
                        sensorVFOV,Endianness::Little));
    metaData.insert(metaData.end(),kSensorVFOV.m_encoded.begin(),kSensorVFOV.m_encoded.end());

    unsigned int sensorAzimuth = static_cast<unsigned int>((m_sensorRelativeAzimuth)*(4294967295)/360.0f);
    Klv kSensorAzimuth(0x12,4,ByteManipulation::ToBytes(
                           sensorAzimuth,Endianness::Little));
    metaData.insert(metaData.end(),kSensorAzimuth.m_encoded.begin(),kSensorAzimuth.m_encoded.end());

    int sensorElevation = static_cast<int>((m_sensorRelativeElevation)*(4294967295)/360.0f);
    Klv kSensorElevation(0x13,4,ByteManipulation::ToBytes(
                             sensorElevation,Endianness::Little));
    metaData.insert(metaData.end(),kSensorElevation.m_encoded.begin(),kSensorElevation.m_encoded.end());

    unsigned int sensorRoll = static_cast<unsigned int>((m_sensorRelativeRoll)*(4294967295)/360.0f);
    Klv kSensorRoll(0x14,4,ByteManipulation::ToBytes(
                        sensorRoll,Endianness::Little));
    metaData.insert(metaData.end(),kSensorRoll.m_encoded.begin(),kSensorRoll.m_encoded.end());

    unsigned int slantRanged = static_cast<unsigned int>((m_slantRanged)*(4294967295)/5000000.0f);
    Klv kSlantRanged(0x15,4,ByteManipulation::ToBytes(
                         slantRanged,Endianness::Little));
    metaData.insert(metaData.end(),kSlantRanged.m_encoded.begin(),kSlantRanged.m_encoded.end());

    int frameCenterLatitude = static_cast<int>((m_frameCenterLatitude)*(4294967294)/180.0f);
    Klv kFrameCenterLatitude(0x17,4,ByteManipulation::ToBytes(
                                 frameCenterLatitude,Endianness::Little));
    metaData.insert(metaData.end(),kFrameCenterLatitude.m_encoded.begin(),kFrameCenterLatitude.m_encoded.end());

    int frameCenterLongitude = static_cast<int>((m_frameCenterLongitude)*(4294967294)/360.0f);
    Klv kFrameCenterLongitude(0x18,4,ByteManipulation::ToBytes(
                                  frameCenterLongitude,Endianness::Little));
    metaData.insert(metaData.end(),kFrameCenterLongitude.m_encoded.begin(),kFrameCenterLongitude.m_encoded.end());

    unsigned short frameCenterElevation = static_cast<unsigned short>((m_frameCenterElevation+900)/19900.0f*(65535.0f));
    Klv kFrameCenterElevation(0x19,2,ByteManipulation::ToBytes(
                                  frameCenterElevation,Endianness::Little));
    metaData.insert(metaData.end(),kFrameCenterElevation.m_encoded.begin(),kFrameCenterElevation.m_encoded.end());

    unsigned short offsetLatitudePoint1 = static_cast<unsigned short>((m_offsetCornerLatitudePoint1)*(65534.0f)/0.15f);
    Klv kOffsetLatitudePoint1(0x1A,2,ByteManipulation::ToBytes(
                                  offsetLatitudePoint1,Endianness::Little));
    metaData.insert(metaData.end(),kOffsetLatitudePoint1.m_encoded.begin(),kOffsetLatitudePoint1.m_encoded.end());

    unsigned short offsetLongitudePoint1 = static_cast<unsigned short>((m_offsetCornerLongitudePoint1)*(65534.0f)/0.15f);
    Klv kOffsetLongitudePoint1(0x1B,2,ByteManipulation::ToBytes(
                                   offsetLongitudePoint1,Endianness::Little));
    metaData.insert(metaData.end(),kOffsetLongitudePoint1.m_encoded.begin(),kOffsetLongitudePoint1.m_encoded.end());

    unsigned short offsetLatitudePoint2 = static_cast<unsigned short>((m_offsetCornerLatitudePoint2)*(65534.0f)/0.15f);
    Klv kOffsetLatitudePoint2(0x1C,2,ByteManipulation::ToBytes(
                                  offsetLatitudePoint2,Endianness::Little));
    metaData.insert(metaData.end(),kOffsetLatitudePoint2.m_encoded.begin(),kOffsetLatitudePoint2.m_encoded.end());

    unsigned short offsetLongitudePoint2 = static_cast<unsigned short>((m_offsetCornerLongitudePoint2)*(65534.0f)/0.15f);
    Klv kOffsetLongitudePoint2(0x1D,2,ByteManipulation::ToBytes(
                                   offsetLongitudePoint2,Endianness::Little));
    metaData.insert(metaData.end(),kOffsetLongitudePoint2.m_encoded.begin(),kOffsetLongitudePoint2.m_encoded.end());

    unsigned short offsetLatitudePoint3 = static_cast<unsigned short>((m_offsetCornerLatitudePoint3)*(65534.0f)/0.15f);
    Klv kOffsetLatitudePoint3(0x1E,2,ByteManipulation::ToBytes(
                                  offsetLatitudePoint3,Endianness::Little));
    metaData.insert(metaData.end(),kOffsetLatitudePoint3.m_encoded.begin(),kOffsetLatitudePoint3.m_encoded.end());

    unsigned short offsetLongitudePoint3 = static_cast<unsigned short>((m_offsetCornerLongitudePoint3)*(65534.0f)/0.15f);
    Klv kOffsetLongitudePoint3(0x1F,2,ByteManipulation::ToBytes(
                                   offsetLongitudePoint3,Endianness::Little));
    metaData.insert(metaData.end(),kOffsetLongitudePoint3.m_encoded.begin(),kOffsetLongitudePoint3.m_encoded.end());

    unsigned short offsetLatitudePoint4 = static_cast<unsigned short>((m_offsetCornerLatitudePoint4)*(65534.0f)/0.15f);
    Klv kOffsetLatitudePoint4(0x20,2,ByteManipulation::ToBytes(
                                  offsetLatitudePoint4,Endianness::Little));
    metaData.insert(metaData.end(),kOffsetLatitudePoint4.m_encoded.begin(),kOffsetLatitudePoint4.m_encoded.end());

    unsigned short offsetLongitudePoint4 = static_cast<unsigned short>((m_offsetCornerLongitudePoint4)*(65534.0f)/0.15f);
    Klv kOffsetLongitudePoint4(0x21,2,ByteManipulation::ToBytes(
                                   offsetLongitudePoint4,Endianness::Little));
    metaData.insert(metaData.end(),kOffsetLongitudePoint4.m_encoded.begin(),kOffsetLongitudePoint4.m_encoded.end());

    unsigned int dataLength = metaData.size()+4;
    if(dataLength < 128){
        uint8_t length = dataLength;
        metaData.insert(metaData.begin(),length);
        printf("Short Data length = %d\r\n",dataLength);
    }else{
        uint16_t length = (uint16_t)dataLength | 0x8100;
        std::vector<uint8_t> aLength = ByteManipulation::ToBytes(length,Endianness::Little);
        metaData.insert(metaData.begin(),aLength.begin(),aLength.end());
        printf("Long Data length = %d [%04X]\r\n",dataLength,length);
    }
    std::vector<uint8_t> kKeyST0601(keyST0601,keyST0601+sizeof(keyST0601));
    metaData.insert(metaData.begin(),kKeyST0601.begin(),kKeyST0601.end());

    metaData.push_back(0X01);
    metaData.push_back(0X02);
    checkSum = bcc_16(metaData.data(),metaData.size());
    std::vector<uint8_t> kChecksum = ByteManipulation::ToBytes(checkSum,Endianness::Little);
    metaData.insert(metaData.end(),kChecksum.begin(),kChecksum.end());

    printf("Encode done:\r\n");
    return metaData;
}
App s_app;

static gboolean
read_data (App * app)
{
    printf("Push klv[%d]\r\n",app->count);
//    uint8_t data[] = {
//        0X06,0X0E,0X2B,0X34,0X02,0X0B,0X01,0X01,0X0E,0X01,0X03,0X01,0X01,0X00,0X00,0X00,
//        0X81,0XA0,
//        0X02,0X08,0X00,0X04,0XCA,0X14,0X28,0X28,0XCD,0X02,
//        0X03,0X15,0X45,0X53,0X52,0X49,0X5F,0X4D,0X65,0X74,0X61,0X64,0X61,0X74,0X61,0X5F,0X43,0X6F,0X6C,0X6C,0X65,0X63,0X74,
//        0X04,0X06,0X4E,0X39,0X37,0X38,0X32,0X36,
//        0X05,0X02,0X6C,0X97,
//        0X06,0X02,0X13,0XDA,
//        0X07,0X02,0XDD,0X0B,
//        0X0A,0X05,0X43,0X32,0X30,0X38,0X42,
////        0X0B,0X00,
////        0X0C,0X00,
//        0X0D,0X04,0X3A,0X72,0X10,0X0A,
//        0X0E,0X04,0XB5,0X6D,0X14,0X7C,
//        0X0F,0X02,0X31,0X4E,
//        0X10,0X02,0X04,0X5D,
//        0X11,0X02,0X02,0X74,
//        0X12,0X04,0XB6,0XE0,0XB6,0X0C,
//        0X13,0X04,0XF6,0X3B,0XBB,0XBC,
//        0X14,0X04,0X00,0X00,0X00,0X00,
//        0X15,0X04,0X00,0X1E,0X02,0XFE,
////        0X16,0X02,0X00,0X00,
//        0X17,0X04,0X3A,0X76,0X50,0X62,
//        0X18,0X04,0XB5,0X70,0X74,0XAF,
//        0X19,0X02,0X23,0X9F,
//        0X1A,0X02,0X01,0X7F,
//        0X1B,0X02,0X00,0X5F,
//        0X1C,0X02,0X00,0X01,
//        0X1D,0X02,0X02,0X04,
//        0X1E,0X02,0XFE,0X97,
//        0X1F,0X02,0XFF,0XA7,
//        0X20,0X02,0X00,0X00,
//        0X21,0X02,0XFE,0X1A,
////        0X2F,0X01,0X00,
////        0X30,0X2A,
////            0X01,0X01,0X01,0X02,0X01,0X01,0X03,0X04,0X2F,0X2F,0X43,0X41,0X04,0X00,0X05,0X00,
////            0X06,0X02,0X43,0X41,0X15,0X10,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
////            0X00,0X00,0X00,0X00,0X00,0X00,0X16,0X02,0X00,0X05,
////        0X38,0X01,0X00,
////        0X3B,0X08,0X46,0X69,0X72,0X65,0X62,0X69,0X72,0X64,
////        0X41,0X01,0X01,
////        0X48,0X08,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
//        0X01,0X02,0XBB,0X33,
////        0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X22,0X32,0X01,0X00,0X00,0X00,0X00
//    };

//    uint8_t data[] = {
//        0X06,0X0E,0X2B,0X34,0X02,0X0B,0X01,0X01,0X0E,0X01,0X03,0X01,0X01,0X00,0X00,0X00,0X81,0XA0,0X02,0X08,0XC0,0X81,0X15,0X52,0XB1,0XA9,0X05,0X00,0X03,0X15,0X45,0X53,0X52,0X49,0X5F,0X4D,0X65,0X74,0X61,0X64,0X61,0X74,0X61,0X5F,0X43,0X6F,0X6C,0X6C,0X65,0X63,0X74,0X04,0X06,0X4E,0X39,0X37,0X38,0X32,0X36,0X05,0X02,0X70,0X12,0X06,0X02,0X15,0XB3,0X07,0X02,0XEF,0X62,0X0A,0X05,0X43,0X32,0X30,0X38,0X42,0X0D,0X04,0X3A,0X72,0X80,0X40,0X0E,0X04,0XB5,0X6C,0XF5,0X80,0X0F,0X02,0X31,0X4F,0X10,0X02,0X04,0X5C,0X11,0X02,0X02,0X72,0X12,0X04,0XB4,0XCC,0XCD,0X00,0X13,0X04,0XF1,0X81,0X6C,0X10,0X14,0X04,0X00,0X00,0X00,0X00,0X15,0X04,0X00,0X1E,0X0A,0X4F,0X17,0X04,0X3A,0X76,0X87,0XC0,0X18,0X04,0XB5,0X70,0X75,0X00,0X19,0X02,0X23,0X99,0X1A,0X02,0X01,0X7A,0X1B,0X02,0X00,0X75,0X1C,0X02,0XFF,0XF2,0X1D,0X02,0X02,0X03,0X1E,0X02,0XFE,0X9C,0X1F,0X02,0XFF,0X94,0X20,0X02,0X00,0X0E,0X21,0X02,0XFE,0X1C,0X01,0X02,0X15,0X97,
//    };
//    int klvLength = sizeof (data);
    std::vector<uint8_t> klvData = encodeMeta();
    int klvLength = klvData.size();
    uint8_t* data = klvData.data();
    GstBuffer *buffer = gst_buffer_new_allocate(nullptr, klvLength, nullptr);
    GstMapInfo map;
    GstClock *clock;
    GstClockTime abs_time, base_time;

    gst_buffer_map (buffer, &map, GST_MAP_WRITE);
    memcpy(map.data, data, klvLength);
    gst_buffer_unmap (buffer, &map);

    GST_OBJECT_LOCK (app->appsrc);
    clock = GST_ELEMENT_CLOCK (app->appsrc);
    base_time = GST_ELEMENT (app->appsrc)->base_time;
    gst_object_ref (clock);
    GST_OBJECT_UNLOCK (app->appsrc);
    abs_time = gst_clock_get_time (clock);
    gst_object_unref (clock);

    GST_BUFFER_PTS (buffer) = abs_time - base_time;
    GST_BUFFER_DURATION (buffer) = GST_SECOND / app->m_frameRate;

    gst_app_src_push_buffer(GST_APP_SRC(app->appsrc), buffer);
    int ms = 1000 / app->m_frameRate * 5;
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
//    GstClockTime gstDuration = GST_SECOND / app->m_frameRate;
//    GST_BUFFER_PTS (buffer) = (app->count + 1) * gstDuration;
//    GST_BUFFER_DURATION (buffer) = GST_SECOND / 30;
//    gst_app_src_push_buffer(GST_APP_SRC(app->appsrc), buffer);
    app->count++;
    return true;
}

/* This signal callback is called when appsrc needs data, we add an idle handler
 * to the mainloop to start pushing data into the appsrc */
static void
start_feed (GstElement * pipeline, guint size, App * app)
{
  if (app->sourceid == 0) {
    GST_DEBUG ("start feeding");
    app->sourceid = g_idle_add ((GSourceFunc) read_data, app);

  }
}
static gboolean seek_data (GstElement* object,
                                         guint64 arg0,
                           App * app){
    printf("%s\r\n",__func__);
}
/* This callback is called when appsrc has enough data and we can stop sending.
 * We remove the idle handler from the mainloop */
static void
stop_feed (GstElement * pipeline, App * app)
{
  if (app->sourceid != 0) {
    GST_DEBUG ("stop feeding");
    g_source_remove (app->sourceid);
    app->sourceid = 0;
  }
  printf("Enough feed[%d]\r\n",app->count);
}

static gboolean
bus_message (GstBus * bus, GstMessage * message, App * app)
{
  GST_DEBUG ("got message %s",
      gst_message_type_get_name (GST_MESSAGE_TYPE (message)));

  switch (GST_MESSAGE_TYPE (message)) {
    case GST_MESSAGE_ERROR: {
        GError *err = NULL;
        gchar *dbg_info = NULL;

        gst_message_parse_error (message, &err, &dbg_info);
        g_printerr ("ERROR from element %s: %s\n",
            GST_OBJECT_NAME (message->src), err->message);
        g_printerr ("Debugging info: %s\n", (dbg_info) ? dbg_info : "none");
        g_error_free (err);
        g_free (dbg_info);
        g_main_loop_quit (app->loop);
        break;
    }
    case GST_MESSAGE_EOS:
      g_main_loop_quit (app->loop);
      break;
    default:
      break;
  }
  return TRUE;
}

int
main (int argc, char *argv[])
{
  App *app = &s_app;
  GError *error = NULL;
  GstBus *bus;
  GstCaps *caps;

  gst_init (&argc, &argv);

  GST_DEBUG_CATEGORY_INIT (appsrc_pipeline_debug, "appsrc-pipeline", 0,
      "appsrc pipeline example");

  /* create a mainloop to get messages and to handle the idle handler that will
   * feed data to appsrc. */
  app->loop = g_main_loop_new (NULL, TRUE);
  app->timer = g_timer_new();

  app->pipeline = gst_parse_launch("filesrc location=/home/hainh/Desktop/Video/Gen4-IR.mp4 ! qtdemux ! h264parse ! queue ! tee name=t t. ! queue ! avdec_h264 ! xvimagesink "
                                   "t. ! queue ! mpegtsmux name=mux ! filesink location=remux-encoded-klv.ts "
                                   "appsrc name=mysource ! mux."
                                   , NULL);
  g_assert (app->pipeline);

  bus = gst_pipeline_get_bus (GST_PIPELINE (app->pipeline));
  g_assert(bus);

  /* add watch for messages */
  gst_bus_add_watch (bus, (GstBusFunc) bus_message, app);

  /* get the appsrc */
    app->appsrc = gst_bin_get_by_name (GST_BIN(app->pipeline), "mysource");
    if(app->appsrc!= nullptr)
    {
        g_signal_connect (app->appsrc, "seek-data", G_CALLBACK (seek_data), app);
        g_signal_connect (app->appsrc, "need-data", G_CALLBACK (start_feed), app);
        g_signal_connect (app->appsrc, "enough-data", G_CALLBACK (stop_feed), app);

      /* set the caps on the source */
      caps = gst_caps_new_simple ("meta/x-klv",
         "parsed", G_TYPE_BOOLEAN, TRUE,
        nullptr);
       gst_app_src_set_caps(GST_APP_SRC(app->appsrc), caps);
       g_object_set(GST_APP_SRC(app->appsrc), "format", GST_FORMAT_TIME, nullptr);

    }

  /* go to playing and wait in a mainloop. */
  gst_element_set_state (app->pipeline, GST_STATE_PLAYING);

  /* this mainloop is stopped when we receive an error or EOS */
  g_main_loop_run (app->loop);

  GST_DEBUG ("stopping");

  gst_element_set_state (app->pipeline, GST_STATE_NULL);

  gst_object_unref (bus);
  g_main_loop_unref (app->loop);

  return 0;
}
