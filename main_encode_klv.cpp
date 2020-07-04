/*
 * Read video frame with FFmpeg and convert to OpenCV image
 *
 * Copyright (c) 2016 yohhoy
 */
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
#include "ByteManipulation.h"
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

    Klv kTimeStamp(0x02,8,ByteManipulation::ToBytes(timestamp,Endianness::Little));
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

    unsigned int dataLength = metaData.size();
    if(dataLength < 128){
        uint8_t length = dataLength;
        metaData.insert(metaData.begin(),length);
        printf("Short Data length = %d\r\n",length);
    }else{
        uint16_t length = (uint16_t)dataLength | 0x8000;
        std::vector<uint8_t> aLength = ByteManipulation::ToBytes(length,Endianness::Little);
        metaData.insert(metaData.begin(),aLength.begin(),aLength.end());
        printf("Long Data length = %d [%04X]\r\n",length,length);
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
            float valueFloat = static_cast<float>(valueInt)/(4294967295)*360;
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
            float valueFloat = static_cast<float>(valueInt)/(4294967295)*360;
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
#define DEB_ENCODE
int main(int argc, char* argv[])
{
#ifdef DEB_ENCODE
    std::vector<uint8_t> klvData = encodeMeta();
    int klvLength = klvData.size();
    uint8_t* data = klvData.data();
#else
    uint8_t klvData[] = {0X06,0X0E,0X2B,0X34,0X02,0X0B,0X01,0X01,0X0E,0X01,0X03,0X01,0X01,0X00,0X00,0X00,
                         0X81,0XF1,
                         0X02,0X08,0X00,0X04,0XCA,0X14,0X0D,0X43,0X23,0X0B,
                         0X03,0X15,0X45,0X53,0X52,0X49,0X5F,0X4D,0X65,0X74,0X61,0X64,0X61,0X74,0X61,0X5F,0X43,0X6F,0X6C,0X6C,0X65,0X63,0X74,
                         0X04,0X06,0X4E,0X39,0X37,0X38,0X32,0X36,
                         0X05,0X02,0XF8,0X6E,
                         0X06,0X02,0X11,0X9A,
                         0X07,0X02,0XED,0X0B,
                         0X0A,0X05,0X43,0X32,0X30,0X38,0X42,
                         0X0B,0X00,
                         0X0C,0X00,
                         0X0D,0X04,0X3A,0X84,0X1D,0XA4,
                         0X0E,0X04,0XB5,0X80,0XF4,0XA1,
                         0X0F,0X02,0X31,0XC7,0X10,0X02,0X01,0X8B,0X11,0X02,0X00,0XDE,0X12,0X04,0XCD,0X04,0X44,0X45,0X13,0X04,0XF1,0X26,0X66,0X66,0X14,0X04,0X00,0X00,0X00,0X00,0X15,0X04,0X00,0X20,0X37,0XBB,0X16,0X02,0X00,0X00,0X17,0X04,0X3A,0X85,0X62,0X87,0X18,0X04,0XB5,0X7C,0X46,0XAC,0X19,0X02,0X23,0X81,0X1A,0X02,0XFF,0XBA,0X1B,0X02,0XFF,0X55,0X1C,0X02,0X00,0X6F,0X1D,0X02,0XFF,0X80,0X1E,0X02,0X00,0X45,0X1F,0X02,0X00,0XA8,0X20,0X02,0XFF,0X94,0X21,0X02,0X00,0X7E,0X2F,0X01,0X00,0X30,0X2A,0X01,0X01,0X01,0X02,0X01,0X01,0X03,0X04,0X2F,0X2F,0X43,0X41,0X04,0X00,0X05,0X00,0X06,0X02,0X43,0X41,0X15,0X10,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X16,0X02,0X00,0X05,0X38,0X01,0X00,0X3B,0X08,0X46,0X69,0X72,0X65,0X62,0X69,0X72,0X64,0X41,0X01,0X01,0X48,0X08,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
                         0X01,0X02,0XDA,0X78};
    int klvLength = sizeof (klvData);
    uint8_t* data = klvData;
#endif
    for(int i=0; i< klvLength; i++){
        printf("0X%02X,",data[i]);
    }
    printf("\r\n");
    unsigned int startIndex = 18;
    while(startIndex < klvLength){
        uint8_t key = klvData[startIndex];
        uint8_t length = klvData[startIndex+1];
        if(length+startIndex+1>= klvLength)
            break;
        std::vector<uint8_t> value(data+startIndex+2,
                                   data+startIndex+2+length);
        startIndex += length+2;
        Klv klv(key,length,value);
        parseKlv(klv);
    }
    return 0;
}
