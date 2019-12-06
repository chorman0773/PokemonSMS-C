#ifndef UTIL_SHADENBT_SHADENBT_H_2019_12_02_17_09
#define UTIL_SHADENBT_SHADENBT_H_2019_12_02_17_09
#include <NBT.h>
#include <ShadeConstants.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif

typedef struct ShadeFile ShadeFile;

ShadeFile* Shade_Open(BinaryIO* io);
ShadeFile* Shade_OpenEncrypted(BinaryIO* io,const char* passwd);

ShadeFile* Shade_NewFile(version shade_v,uint8_t flags);
CompoundTag* Shade_GetCompound(ShadeFile*);
void Shade_SetVersion(ShadeFile* file,version shade_v);

void Shade_Write(ShadeFile* file,BinaryIO* io);
void Shade_WriteEncrypted(ShadeFile* file,BinaryIO* io);

void Shade_Free(ShadeFile* file);

#ifdef __cplusplus
};
#endif
#endif
