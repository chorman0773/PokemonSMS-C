#include <ShadeNBT.h>
#include <string.h>
#include <setjmp.h>
#include <stdlib.h>


struct ShadeFile{
    NBT_Tag* root;
    uint8_t flags;
    version shade_v;
};

static void shade_handler(void* jmpBuff,const char* name){
    longjmp(*(jmp_buf*)jmpBuff,-1);
}


ShadeFile* Shade_Open(BinaryIO* io){
    void* err_data_saved;
    error_handler_t* err_handler_saved;
    jmp_buf buf;
    err_handler_saved = setErrorHandler(io,shade_handler,&buf,&err_data_saved);
    ShadeFile* file = (ShadeFile*)malloc(sizeof(ShadeFile));
    file->root = NBT_CreateTag(TAG_Compound);
    if(setjmp(buf)){
        Shade_Free(file);
        setErrorHandler(io,err_handler_saved,err_data_saved,NULL);
        err_handler_saved(err_data_saved,BinaryIO_last_error(io));
        return NULL;
    }
    unsigned char magic[4];
    read_fully(io,magic,4);
    if(memcmp(magic,"\xADNBT",4)){
        if(!memcmp(magic,"\xECNBT",4))
            BinaryIO_raise_error(io,"Attempt to open CryptoShade file through normal Shade_Open. Did you mean Shade_OpenEncrypted?");
        else
            BinaryIO_raise_error(io,"File is not a valid ShadeNBT file, Valid magic not read");
        __builtin_unreachable();
    }
    file->shade_v = read_version(io);
    if(file->shade_v>v1_3){
        BinaryIO_raise_error(io,"Shade Version on Stream is not understood by this implementation");
        __builtin_unreachable();
    }
    file->flags = file->shade_v>=v1_2 ? read_u8(io) : 0;
    if(file->flags&0x80)
        setEndianess(io,BINARY_LITTLE_ENDIAN);
    else
        setEndianess(io,BINARY_BIG_ENDIAN);
    if(read_u8(io)!=TAG_Compound||read_u16(io)!=0){
        BinaryIO_raise_error(io,"The top level tag in a ShadeNBT File MUST be a nameless compound tag");
        __builtin_unreachable();
    }
        
    NBT_Read(file->root,io,file->shade_v);
    return file;
}

ShadeFile* Shade_OpenEncrypted(BinaryIO* io,const char* passwd){
    void* err_data_saved;
    error_handler_t* err_handler_saved;
    jmp_buf buf;
    err_handler_saved = setErrorHandler(io,shade_handler,&buf,&err_data_saved);
    ShadeFile* file = (ShadeFile*)malloc(sizeof(ShadeFile));
    file->root = NBT_CreateTag(TAG_Compound);
    if(setjmp(buf)){
        Shade_Free(file);
        setErrorHandler(io,err_handler_saved,err_data_saved,NULL);
        err_handler_saved(err_data_saved,BinaryIO_last_error(io));
        return NULL;
    }
    unsigned char magic[4];
    read_fully(io,magic,4);
    if(memcmp(magic,"\xECNBT",4)){
        if(!memcmp(magic,"\xADNBT",4))
            BinaryIO_raise_error(io,"Attempt to open regular ShadeNBT file through normal Shade_OpenEncrypted. Did you mean Shade_Open?");
        else
            BinaryIO_raise_error(io,"File is not a valid CryptoShade file, Valid magic not read");
        __builtin_unreachable();
    }
    file->shade_v = read_version(io);
    if(file->shade_v>v1_3){
        BinaryIO_raise_error(io,"Shade Version on Stream is not understood by this implementation");
        __builtin_unreachable();
    }
    file->flags = file->shade_v>=v1_2 ? read_u8(io) : 0;
    BinaryIO_raise_error(io,"CryptoShade is not yet implemented");
    __builtin_unreachable();
}

ShadeFile* Shade_NewFile(version shade_v,uint8_t flags){
    ShadeFile* f = (ShadeFile*) malloc(sizeof(ShadeFile));
    f->root = NBT_CreateTag(TAG_Compound);
    f->shade_v = shade_v;
    f->flags = flags&(shade_v<v1_2?0x00:(shade_v<v1_3?0x80:0xA0));
    return f;//Control reaches end of function not returning void. That would do it.
}

CompoundTag* Shade_GetCompound(ShadeFile* io){
    return NBT_AsCompound(io->root);
}
void Shade_SetVersion(ShadeFile* f,version shade_v){
    f->shade_v = shade_v;
    f->flags &= (shade_v<v1_2?0x00:(shade_v<v1_3?0x80:0xA0));
}

void Shade_Write(ShadeFile* file,BinaryIO* io){
    write_bytes(io,"\xADNBT",4);
    write_version(io,file->shade_v);
    if(file->shade_v>=v1_2)
        write_u8(io,file->flags&0xA0);
    if(file->flags&0x80)
        setEndianess(io,BINARY_LITTLE_ENDIAN);
    else
        setEndianess(io,BINARY_BIG_ENDIAN);
    write_u8(io,TAG_Compound);
    write_string(io,"");
    NBT_Write(file->root,io,file->shade_v);
}
void Shade_WriteEncrypted(ShadeFile* file,BinaryIO* io){

}

void Shade_Free(ShadeFile* file){
    NBT_FreeTag(file->root);
}

