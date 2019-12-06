#ifndef UTIL_SHADENBT_NBT_H_2019_11_30_19_16
#define UTIL_SHADENBT_NBT_H_2019_11_30_19_16

#ifdef __cplusplus
extern"C"{
#else
#include <stdbool.h>
#endif

#include <stdint.h>
#include <Stream.h>
#include <BinaryIO.h>
#include <Map.h>
#include <List.h>



typedef uint8_t TAG_Type;

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
    TAG_UUID = 15,
    TAG_Count = 16,
    TAG_AnyNumeric = 99
};

typedef struct NBTList{
    uint8_t listTagType;
    ArrayList* list;
} NBTList;

typedef struct ByteArray{
    int32_t len;
    unsigned char* array;
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

typedef struct CompoundTag CompoundTag;
typedef struct ListTag ListTag;

typedef struct NBT_Tag NBT_Tag;

NBT_Tag* NBT_CreateTag(TAG_Type);
NBT_Tag* NBT_DupTag(NBT_Tag*);
void NBT_FreeTag(NBT_Tag*);
NBT_Tag* NBT_Read(NBT_Tag* tag,BinaryIO* io,version shade_v);
void NBT_Write(NBT_Tag* tag,BinaryIO* io,version shade_v);

//Direct Accessors
int8_t* NBT_AsByte(NBT_Tag* tag);
int16_t* NBT_AsShort(NBT_Tag* tag);
int32_t* NBT_AsInt(NBT_Tag* tag);
int64_t* NBT_AsLong(NBT_Tag* tag);
float* NBT_AsFloat(NBT_Tag* tag);
double* NBT_AsDouble(NBT_Tag* tag);
const char* NBT_GetString(NBT_Tag* tag);
void NBT_SetString(NBT_Tag* tag,const char* str);
UUID* NBT_AsUUID(NBT_Tag* tag);

//Primitive-generic accessors
bool NBT_GetBoolean(NBT_Tag* tag);
int8_t NBT_GetByte(NBT_Tag* tag);
int16_t NBT_GetShort(NBT_Tag* tag);
int32_t NBT_GetInt(NBT_Tag* tag);
int64_t NBT_GetLong(NBT_Tag* tag);
float NBT_GetFloat(NBT_Tag* tag);
double NBT_GetDouble(NBT_Tag* tag);

//Special Accessors
ListTag* NBT_AsList(NBT_Tag* tag);
CompoundTag* NBT_AsCompound(NBT_Tag* tag);

//Array Accessors
ByteArray* NBT_AsByteArray(NBT_Tag* tag);
IntArray* NBT_AsIntArray(NBT_Tag* tag);
LongArray* NBT_AsLongArray(NBT_Tag* tag);
FloatArray* NBT_AsFloatArray(NBT_Tag* tag);
DoubleArray* NBT_AsDoubleArray(NBT_Tag* tag);

bool NBT_IsTagType(NBT_Tag* tag,TAG_Type);

void Compound_PutTag(CompoundTag* tag,const char* name,NBT_Tag* nbt);
NBT_Tag* Compound_GetTag(CompoundTag* tag,const char* name);
bool Compound_HasTag(CompoundTag* tag,const char* name);
bool Compound_HasTagType(CompoundTag* tag,const char* name,TAG_Type);
bool Compound_GetBoolean(CompoundTag* tag,const char* name);
int8_t Compound_GetByte(CompoundTag* tag,const char* name);
int16_t Compound_GetShort(CompoundTag* tag,const char* name);
int32_t Compound_GetInt(CompoundTag* tag,const char* name);
int64_t Compound_GetLong(CompoundTag* tag,const char* name);
float Compound_GetFloat(CompoundTag* tag,const char* name);
double Compound_GetDouble(CompoundTag* tag,const char* name);
UUID Compound_GetUUID(CompoundTag* tag,const char* name);
void Compound_SetBoolean(CompoundTag* tag,const char* name,bool val);
void Compound_SetByte(CompoundTag* tag,const char* name,int8_t val);
void Compound_SetShort(CompoundTag* tag,const char* name,int16_t val);
void Compound_SetInt(CompoundTag* tag,const char* name,int32_t val);
void Compound_SetLong(CompoundTag* tag,const char* name,int64_t val);
void Compound_SetFloat(CompoundTag* tag,const char* name,float val);
void Compound_SetDouble(CompoundTag* tag,const char* name,double val);
void Compound_SetUUID(CompoundTag* tag,const char* name,UUID uuid);

int32_t List_GetLength(ListTag* tag);
TAG_Type List_GetListTagType(ListTag* tag);
bool List_Add(ListTag* tag,NBT_Tag* toAdd);
NBT_Tag* List_Get(ListTag* tag,int32_t idx);

#ifdef __cplusplus
};
#endif

#endif
