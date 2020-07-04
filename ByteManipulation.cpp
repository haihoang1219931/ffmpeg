#include "ByteManipulation.h"

using namespace std;

void ByteManipulation::ToBytes(unsigned long input, Endianness endianess, unsigned char *Array, int startIndex){
    b_uint64 tmp;
    tmp.value = input;
    if(endianess == Endianness::Big){
        for(int i=0; i< sizeof(input); i++){
            Array[startIndex+i] = tmp.data[i];
        }
    }else if(endianess == Endianness::Little){
        for(int i=0; i< sizeof(input); i++){
            Array[startIndex+i] = tmp.data[sizeof(input)-1-i];
        }
    }
}
void ByteManipulation::ToBytes(long input, Endianness endianess, unsigned char *Array, int startIndex){
    b_int64 tmp;
    tmp.value = input;
    if(endianess == Endianness::Big){
        for(int i=0; i< sizeof(input); i++){
            Array[startIndex+i] = tmp.data[i];
        }
    }else if(endianess == Endianness::Little){
        for(int i=0; i< sizeof(input); i++){
            Array[startIndex+i] = tmp.data[sizeof(input)-1-i];
        }
    }
}
void ByteManipulation::ToBytes(unsigned int input, Endianness endianess, unsigned char *Array, int startIndex){
    b_uint tmp;
    tmp.value = input;
    if(endianess == Endianness::Big){
        for(int i=0; i< sizeof(input); i++){
            Array[startIndex+i] = tmp.data[i];
        }
    }else if(endianess == Endianness::Little){
        for(int i=0; i< sizeof(input); i++){
            Array[startIndex+i] = tmp.data[sizeof(input)-1-i];
        }
    }

}
void ByteManipulation::ToBytes(int input, Endianness endianess, unsigned char *Array, int startIndex){
    b_int tmp;
    tmp.value = input;
    if(endianess == Endianness::Big){
        for(int i=0; i< sizeof(input); i++){
            Array[startIndex+i] = tmp.data[i];
        }
    }else if(endianess == Endianness::Little){
        for(int i=0; i< sizeof(input); i++){
            Array[startIndex+i] = tmp.data[sizeof(input)-1-i];
        }
    }
}
void ByteManipulation::ToBytes(unsigned short input, Endianness endianess, unsigned char *Array, int startIndex){
    b_ushort tmp;
    tmp.value = input;
    if(endianess == Endianness::Big){
        for(int i=0; i< sizeof(input); i++){
            Array[startIndex+i] = tmp.data[i];
        }
    }else if(endianess == Endianness::Little){
        for(int i=0; i< sizeof(input); i++){
            Array[startIndex+i] = tmp.data[sizeof(input)-1-i];
        }
    }
}
void ByteManipulation::ToBytes(short input, Endianness endianess, unsigned char *Array, int startIndex){
    b_short tmp;
    tmp.value = input;
    if(endianess == Endianness::Big){
        for(int i=0; i< sizeof(input); i++){
            Array[startIndex+i] = tmp.data[i];
        }
    }else if(endianess == Endianness::Little){
        for(int i=0; i< sizeof(input); i++){
            Array[startIndex+i] = tmp.data[sizeof(input)-1-i];
        }
    }
}
void ByteManipulation::ToBytes(float input, Endianness endianess, unsigned char *Array, int startIndex){
    b_float tmp;
    tmp.value = input;
    if(endianess == Endianness::Big){
        for(int i=0; i< sizeof(input); i++){
            Array[startIndex+i] = tmp.data[i];
        }
    }else if(endianess == Endianness::Little){
        for(int i=0; i< sizeof(input); i++){
            Array[startIndex+i] = tmp.data[sizeof(input)-1-i];
        }
    }
}
/**
*
*/
vector<unsigned char> ByteManipulation::ToBytes(unsigned long input, Endianness endianess){
    vector<unsigned char> result;
    b_uint64 tmp;
    tmp.value = input;
    if(endianess == Endianness::Big){
        for(int i=0; i< sizeof(input); i++){
            result.push_back(tmp.data[i]);
        }
    }else if(endianess == Endianness::Little){
        for(int i=0; i< sizeof(input); i++){
            result.push_back(tmp.data[sizeof(input)-1-i]);
        }
    }
    return result;
}
vector<unsigned char> ByteManipulation::ToBytes(long input, Endianness endianess){
    vector<unsigned char> result;
    b_int64 tmp;
    tmp.value = input;
    if(endianess == Endianness::Big){
        for(int i=0; i< sizeof(input); i++){
            result.push_back(tmp.data[i]);
        }
    }else if(endianess == Endianness::Little){
        for(int i=0; i< sizeof(input); i++){
            result.push_back(tmp.data[sizeof(input)-1-i]);
        }
    }
    return result;
}
vector<unsigned char> ByteManipulation::ToBytes(unsigned int input, Endianness endianess){
    vector<unsigned char> result;
    b_uint tmp;
    tmp.value = input;
    if(endianess == Endianness::Big){
        for(int i=0; i< sizeof(input); i++){
            result.push_back(tmp.data[i]);
        }
    }else if(endianess == Endianness::Little){
        for(int i=0; i< sizeof(input); i++){
            result.push_back(tmp.data[sizeof(input)-1-i]);
        }
    }
    return result;
}
vector<unsigned char> ByteManipulation::ToBytes(int input, Endianness endianess){
    vector<unsigned char> result;
    b_int tmp;
    tmp.value = input;
    if(endianess == Endianness::Big){
        for(int i=0; i< sizeof(input); i++){
            result.push_back(tmp.data[i]);
        }
    }else if(endianess == Endianness::Little){
        for(int i=0; i< sizeof(input); i++){
            result.push_back(tmp.data[sizeof(input)-1-i]);
        }
    }
    return result;
}
vector<unsigned char> ByteManipulation::ToBytes(unsigned short input, Endianness endianess){
    vector<unsigned char> result;
    b_ushort tmp;
    tmp.value = input;
    if(endianess == Endianness::Big){
        for(int i=0; i< sizeof(input); i++){
            result.push_back(tmp.data[i]);
        }
    }else if(endianess == Endianness::Little){
        for(int i=0; i< sizeof(input); i++){
            result.push_back(tmp.data[sizeof(input)-1-i]);
        }
    }
    return result;
}
vector<unsigned char> ByteManipulation::ToBytes(short input, Endianness endianess){
    vector<unsigned char> result;
    b_short tmp;
    tmp.value = input;
    if(endianess == Endianness::Big){
        for(int i=0; i< sizeof(input); i++){
            result.push_back(tmp.data[i]);
        }
    }else if(endianess == Endianness::Little){
        for(int i=0; i< sizeof(input); i++){
            result.push_back(tmp.data[sizeof(input)-1-i]);
        }
    }
    return result;
}
vector<unsigned char> ByteManipulation::ToBytes(float input, Endianness endianess){
    vector<unsigned char> result;
    b_float tmp;
    tmp.value = input;
    if(endianess == Endianness::Big){
        for(int i=0; i< sizeof(input); i++){
            result.push_back(tmp.data[i]);
        }
    }else if(endianess == Endianness::Little){
        for(int i=0; i< sizeof(input); i++){
            result.push_back(tmp.data[sizeof(input)-1-i]);
        }
    }
    return result;
}
/**
* Convert byte array to value
*/
unsigned int ByteManipulation::ToUInt64(unsigned char *bytes, int startIndex, Endianness endianess){
    b_uint64 tmp;
    if(endianess == Endianness::Little){
        for(int i=0; i< sizeof(tmp); i++){
            tmp.data[i] = bytes[startIndex+i];
        }
    }else if(endianess == Endianness::Big){
        for(int i=0; i< sizeof(tmp); i++){
            tmp.data[sizeof(tmp)-1-i] = bytes[startIndex+i];
        }
    }
    return tmp.value;
}
int ByteManipulation::ToInt64(unsigned char *bytes , int startIndex, Endianness endianess){
    b_int64 tmp;
    if(endianess == Endianness::Little){
        for(int i=0; i< sizeof(tmp); i++){
            tmp.data[i] = bytes[startIndex+i];
        }
    }else if(endianess == Endianness::Big){
        for(int i=0; i< sizeof(tmp); i++){
            tmp.data[sizeof(tmp)-1-i] = bytes[startIndex+i];
        }
    }
    return tmp.value;
}
unsigned int ByteManipulation::ToUInt32(unsigned char *bytes, int startIndex, Endianness endianess){
    b_uint tmp;
    if(endianess == Endianness::Little){
        for(int i=0; i< sizeof(tmp); i++){
            tmp.data[i] = bytes[startIndex+i];
        }
    }else if(endianess == Endianness::Big){
        for(int i=0; i< sizeof(tmp); i++){
            tmp.data[sizeof(tmp)-1-i] = bytes[startIndex+i];
        }
    }
    return tmp.value;
}
int ByteManipulation::ToInt32(unsigned char *bytes , int startIndex, Endianness endianess){
    b_int tmp;
    if(endianess == Endianness::Little){
        for(int i=0; i< sizeof(tmp); i++){
            tmp.data[i] = bytes[startIndex+i];
        }
    }else if(endianess == Endianness::Big){
        for(int i=0; i< sizeof(tmp); i++){
            tmp.data[sizeof(tmp)-1-i] = bytes[startIndex+i];
        }
    }
    return tmp.value;
}
unsigned short ByteManipulation::ToUInt16(unsigned char *bytes, int startIndex, Endianness endianess){

    b_ushort tmp;
    if(endianess == Endianness::Little){
        for(int i=0; i< sizeof(tmp); i++){
            tmp.data[i] = bytes[startIndex+i];
        }
    }else if(endianess == Endianness::Big){
        for(int i=0; i< sizeof(tmp); i++){
            tmp.data[sizeof(tmp)-1-i] = bytes[startIndex+i];
        }
    }
    //printf("fov=%d [%02x %02x]\r\n",tmp.value,tmp.data[0],tmp.data[1]);
    return tmp.value;
    /*
    unsigned short tmp = 0;
    if(endianess == Endianness::Big){
        tmp = (unsigned short)bytes[startIndex+0]*256+bytes[startIndex+1];
    }else if(endianess == Endianness::Little){
        tmp = (unsigned short)bytes[startIndex+1]*256+bytes[startIndex+0];
    }
    printf("fov=%04x\r\n",tmp);
    return tmp;
    */
}
short ByteManipulation::ToInt16(unsigned char *bytes, int startIndex, Endianness endianess){
    b_short tmp;
    if(endianess == Endianness::Little){
        for(int i=0; i< sizeof(tmp); i++){
            tmp.data[i] = bytes[startIndex+i];
        }
    }else if(endianess == Endianness::Big){
        for(int i=0; i< sizeof(tmp); i++){
            tmp.data[sizeof(tmp)-1-i] = bytes[startIndex+i];
        }
    }
    return tmp.value;
}
float ByteManipulation::ToFloat(unsigned char *bytes, int startIndex, Endianness endianess){
    b_float tmp;
    if(endianess == Endianness::Big){
        for(int i=0; i< sizeof(tmp); i++){
            tmp.data[i] = bytes[startIndex+i];
        }
    }else if(endianess == Endianness::Little){
        for(int i=0; i< sizeof(tmp); i++){
            tmp.data[sizeof(tmp)-1-i] = bytes[startIndex+i];
        }
    }
    return tmp.value;
}

