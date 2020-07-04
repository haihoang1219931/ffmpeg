#ifndef BYTEMAINPULATION_H
#define BYTEMAINPULATION_H
#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

union b_uint64{
    unsigned long value;
    unsigned char data[8];
};
union b_int64{
    long value;
    unsigned char data[8];
};
union b_int
{
    int value;
    unsigned char data[4];
};
union b_uint
{
    unsigned int value;
    unsigned char data[4];
};

union b_float
{
    float value;
    unsigned char data[4];
};

union b_short
{
    short value;
    unsigned char data[2];
};
union b_ushort
{
    unsigned short value;
    unsigned char data[2];
};

enum class Endianness
{
    Little,
    Big
};

class ByteManipulation {
public:
    static void ToBytes(unsigned long input, Endianness Endian, unsigned char *Array, int startIndex);
    static void ToBytes(long input, Endianness Endian, unsigned char *Array, int startIndex);
    static void ToBytes(unsigned int input, Endianness Endian, unsigned char *Array, int startIndex);
    static void ToBytes(int input, Endianness Endian, unsigned char *Array, int startIndex);
    static void ToBytes(unsigned short input, Endianness Endian, unsigned char *Array, int startIndex);
    static void ToBytes(short input, Endianness Endian, unsigned char *Array, int startIndex);
    static void ToBytes(float input, Endianness Endian, unsigned char *Array, int startIndex);
    //
    static vector<unsigned char> ToBytes(unsigned long input, Endianness Endian);
    static vector<unsigned char> ToBytes(long input, Endianness Endian);
    static vector<unsigned char> ToBytes(unsigned int input, Endianness Endian);
    static vector<unsigned char> ToBytes(int input, Endianness Endian);
    static vector<unsigned char> ToBytes(unsigned short input, Endianness Endian);
    static vector<unsigned char> ToBytes(short input, Endianness Endian);
    static vector<unsigned char> ToBytes(float input, Endianness Endian);
    //
    static unsigned int ToUInt64(unsigned char *bytes, int startIndex, Endianness endianess);
    static int ToInt64(unsigned char *bytes , int startIndex, Endianness endianness);
    static unsigned int ToUInt32(unsigned char *bytes, int startIndex, Endianness endianess);
    static int ToInt32(unsigned char *bytes , int startIndex, Endianness endianness);
    static unsigned short ToUInt16(unsigned char *bytes, int startIndex, Endianness endianess);
    static short ToInt16(unsigned char *bytes, int startIndex, Endianness endianess);
    static float ToFloat(unsigned char *input, int startIndex, Endianness inputEndianess);


};
#endif // !BYTEMAINPULATION_H

