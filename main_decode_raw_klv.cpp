//#include <stdio.h>
//#include <ctype.h>
//#include "klv/Klv.h"
//int main(int argc, char *argv[])
//{
//    unsigned char data[] = {
//        0X06,0X0E,0X2B,0X34,
//        0X02,0X0B,0X01,0X01,
//        0X0E,0X01,0X03,0X01,
//        0X01,0X00,0X00,0X00,
//        0X81,0XF1,
//        0X02,0X08,0X00,0X04,0XCA,0X14,0X0D,0XDC,0X8E,0XD7,0X03,0X15,
//        0X45,0X53,0X52,0X49,0X5F,0X4D,0X65,0X74,0X61,0X64,
//        0X61,0X74,0X61,0X5F,0X43,0X6F,0X6C,0X6C,0X65,0X63,
//        0X74,
//        0X04,0X06,
//        0X4E,0X39,0X37,0X38,0X32,0X36,
//        0X05,0X02, 0XEE,0X85,
//        0X06,0X02, 0X16,0X5A,
//        0X07,0X02, 0XF3,0XB9,
//        0X0A,
//        0X05,
//        0X43,0X32,0X30,0X38,0X42,
//        0X0B,0X00,
//        0X0C,0X00,0X0D,0X04,0X3A,0X85,0XC3,0X2F,0X0E,0X04,0XB5,0X80,0X92,0X47,0X0F,0X02,0X31,0XB3,0X10,0X02,0X03,0XE6,0X11,0X02,0X02,0X31,0X12,0X04,0XCD,0XA7,0XD2,0X7E,0X13,0X04,0XED,0XF7,0X77,0X78,0X14,0X04,0X00,0X00,0X00,0X00,0X15,0X04,0X00,0X1E,0X32,0X92,0X16,0X02,0X00,0X00,0X17,0X04,0X3A,0X85,0X72,0X51,0X18,0X04,0XB5,0X7C,0X31,0X53,0X19,0X02,0X23,0X80,0X1A,0X02,0XFF,0X17,0X1B,0X02,0XFE,0XB6,0X1C,0X02,0X00,0XD2,0X1D,0X02,0XFE,0X9A,0X1E,0X02,0X00,0XDF,0X1F,0X02,0X01,0X3B,0X20,0X02,0XFF,0X3A,0X21,0X02,0X01,0X55,0X2F,0X01,0X00,0X30,0X2A,0X01,0X01,0X01,0X02,0X01,0X01,0X03,0X04,0X2F,0X2F,0X43,0X41,0X04,0X00,0X05,0X00,0X06,0X02,0X43,0X41,0X15,0X10,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X16,0X02,0X00,0X05,0X38,0X01,0X00,0X3B,0X08,0X46,0X69,0X72,0X65,0X62,0X69,0X72,0X64,0X41,0X01,0X01,0X48,0X08,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X02,0XA4,0X93,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X78,0XBC,0X28,0XC6,0XED,0XF2,0X59,0XEC,0X99,0XEB,0X7E,0X7B,0X33};
////    printf("%02X ",'E');
////    printf("%02X ",'R');
////    printf("%02X ",'S');
////    printf("%02X ",'I');
//    printf("Mission ID: ");
//    int parsedIndex = 30;
//    int missionIDLength=0;
//    for(int i=parsedIndex; i< parsedIndex+127; i++){
//        if(isprint(data[i])){
//            missionIDLength++;
//            printf("%c",data[i]);
//        }else{
//            break;
//        }
//    }
//    printf("[%d]\r\n",missionIDLength);
//    printf("Platform Tail Number: ");
//    parsedIndex += (missionIDLength+2);
//    int platformTailNumberLength=0;
//    for(int i=parsedIndex; i< parsedIndex+127; i++){
//        if(isprint(data[i])){
//            platformTailNumberLength++;
//            printf("%c",data[i]);
//        }else{
//            break;
//        }
//    }
//    printf("[%d]\r\n",platformTailNumberLength);
//    parsedIndex += (platformTailNumberLength+2);
//    float headingAngle = (float)(data[parsedIndex] + data[parsedIndex+1]*16);
//    printf("headingAngle=%f\r\n",headingAngle/(65536-1)*360);
//    float pitchAngle = (float)(data[parsedIndex+4] + data[parsedIndex+5]*16);
//    printf("pitchAngle=%f\r\n",(pitchAngle+16484-1)/(32768)*40-20);
//    float rollAngle = (float)(data[parsedIndex+8] + data[parsedIndex+9]*16);
//    printf("rollAngle=%f\r\n",(rollAngle+16484-1)/(32768)*100-50);
//    return 0;
//}
