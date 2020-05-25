#include <stdio.h>
#include <vector>
#include <iostream>
#include "Bytes/ByteManipulation.h"
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
    default:
        break;
    }
}

int main(){
    std::vector<uint8_t> test_pkt_uas = {
        0X06,0X0E,0X2B,0X34,
        0X02,0X0B,0X01,0X01,
        0X0E,0X01,0X03,0X01,
        0X01,0X00,0X00,0X00,
        0X81,0XF1,
        0X02,0X08,
            0X00,0X04,0XCA,0X14,0X0D,0XC3,0X64,0X5D,
        0X03,0X15,
            0X45,0X53,0X52,0X49,0X5F,0X4D,0X65,0X74,
            0X61,0X64,0X61,0X74,0X61,0X5F,0X43,0X6F,
            0X6C,0X6C,0X65,0X63,0X74,
        0X04,0X06,
            0X4E,0X39,0X37,0X38,0X32,0X36,
        0X05,0X02,
            0XF1,0X06,
        0X06,0X02,
            0X15,0X66,
        0X07,0X02,
            0XF0,0XC9,
        0X0A,0X05,
            0X43,0X32,0X30,0X38,0X42,
        0X0B,0X00,
        0X0C,0X00,
        0X0D,0X04,
            0X3A,0X85,0X7F,0X6E,
        0X0E,0X04,
            0XB5,0X80,0XA6,0X83,
        0X0F,0X02,
            0X31,0XB4,
        0X10,0X02,
            0X03,0XE6,
        0X11,0X02,
        0X02,0X31,0X12,0X04,0XCC,0XA4,0XFA,0X50,0X13,0X04,0XEE,0XEE,0XEE,0XEF,0X14,0X04,0X00,0X00,0X00,0X00,0X15,0X04,0X00,0X1E,0X73,0X4C,0X16,0X02,0X00,0X00,0X17,0X04,0X3A,0X85,0X6C,0XC6,0X18,0X04,0XB5,0X7C,0X38,0X35,0X19,0X02,0X23,0X81,0X1A,0X02,0XFF,0X1E,0X1B,0X02,0XFE,0XA5,0X1C,0X02,0X00,0XDD,0X1D,0X02,0XFE,0X9F,0X1E,0X02,0X00,0XD8,0X1F,0X02,0X01,0X4B,0X20,0X02,0XFF,0X2E,0X21,0X02,0X01,0X51,0X2F,0X01,0X00,0X30,0X2A,0X01,0X01,0X01,0X02,0X01,0X01,0X03,0X04,0X2F,0X2F,0X43,0X41,0X04,0X00,0X05,0X00,0X06,0X02,0X43,0X41,0X15,0X10,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X16,0X02,0X00,0X05,0X38,0X01,0X00,0X3B,0X08,0X46,0X69,0X72,0X65,0X62,0X69,0X72,0X64,0X41,0X01,0X01,0X48,0X08,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X02,0XC8,0XC9,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XF7,0XDE,0XF5,0X7A,0X9C,0X76,0X3A,0XFD,0X1B,0X72,0XA3,0XE9,0X1A,
    };
    unsigned int startIndex = 18;

    while(startIndex < test_pkt_uas.size()){
        uint8_t key = test_pkt_uas[startIndex];
        uint8_t length = test_pkt_uas[startIndex+1];
        if(length+startIndex+1>= test_pkt_uas.size())
            break;
        std::vector<uint8_t> value(test_pkt_uas.begin()+startIndex+2,
                                   test_pkt_uas.begin()+startIndex+2+length);
        startIndex += length+2;
//        printf("Decoded K[%02X] L[%02X]\r\n",key,length);
        Klv klv(key,length,value);
        parseKlv(klv);
    }
    return 0;
}
