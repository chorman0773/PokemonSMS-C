#ifndef UTIL_SHADENBT_NBT_H_2019_11_30_19_16
#define UTIL_SHADENBT_NBT_H_2019_11_30_19_16

#ifdef __cplusplus
extern"C"{
#endif

#include <stdint.h>
#include <Stream.h>

typedef struct NBT_TAG NBT_Tag;

enum{
    TAG_End = 0,
    TAG_Byte = 1,
    TAG_Short = 2,
    TAG_Int = 3,
    TAG_Long = 4,
    TAG_Float = 5,
    TAG_Double = 6,
    TAG_ByteArray = 7,
    TAG_String = 8,
    TAG_List = 9,
    TAG_Compound = 10,
    TAG_IntArray = 11,
    TAG_LongArray = 12,
    TAG_FloatArray = 13,
    TAG_DoubleArray = 14,
    TAG_UUID = 15
}

typedef struct Compound Compound;
typedef struct List List;

typedef struct ByteArray{
    int32_t len;
    unsigned char* buff;
} ByteArray;

typedef struct IntArray{
    int32_t len;
    int32_t* array;
} IntArray;

typedef struct LongArray{
    int32_t len;
    int64_t* array;
} LongArray;

typedef struct FloatArray{
    int32_t len;
    float* array;
} FloatArray;

typedef struct DoubleArray{
    int32_t len;
    double* array;
} DoubleArray;

typedef uint8_t TAG_Type;



#ifdef __cplusplus
};
#endif

#endif
