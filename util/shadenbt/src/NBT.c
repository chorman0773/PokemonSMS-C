#include <NBT.h>
#include <Map.h>
#include <List.h>
#include <stdio.h>
#include <ShadeConstants.h>
#include <string.h>

struct CompoundTag{
    TreeMap* map;
};

struct ListTag{
    TAG_Type listTagType;
    ArrayList* list;
};

struct NBT_Tag{
    TAG_Type type;
    uint16_t refCount;
    union{
        char t_end;
        int8_t t_byte;
        int16_t t_short;
        int32_t t_int;
        int64_t t_long;
        float t_float;
        double t_double;
        ByteArray t_barr;
        char* t_str;
        ListTag t_list;
        CompoundTag t_compound;
        IntArray t_iarr;
        LongArray t_larr;
        FloatArray t_farr;
        DoubleArray t_darr;
        UUID t_uuid;
    };
};

static const version tag_minVersions[TAG_Count] =
{
    v1_0,
    v1_0,
    v1_0,
    v1_0,
    v1_0,
    v1_0,
    v1_0,
    v1_0,
    v1_0,
    v1_0,
    v1_1,
    v1_1,
    v1_3,
    v1_3,
    v1_3
}

static void nbt_copy(void* v,const void* vv){
    *((NBT_Tag**)v) = NBT_DupTag(*((NBT_Tag*const*)vv));
}

static void nbt_dtor(void* v){
    NBT_FreeTag(*((NBT_Tag**)v)));
}
static void nbt_dealloc(void* v,void* data){
    NBT_FreeTag((NBT_Tag*)v);
}

static bool strless(const void* s1,const void* s2){
    return strcmp((const char*) s1,(const char*) s2)<0;
}

NBT_Tag* NBT_CreateTag(TAG_Type type){
    NBT_Tag* tag = (NBT_Tag*) malloc(sizeof(NBT_Tag));
    tag->type = type;
    tag->refCount = 0;
    switch(type){
    case TAG_Byte:
        tag->t_byte = 0;
    break;
    case TAG_Short:
        tag->t_short = 0;
    break;
    case TAG_Int:
        tag->t_int = 0;
    break;
    case TAG_Long:
        tag->t_long = 0;
    break;
    case TAG_Float:
        tag->t_float = 0;
    break;
    case TAG_Double:
        tag->t_double = 0;
    break;
    case TAG_ByteArray:
        tag->t_barr.len = 0;
        tag->t_barr.arr = NULL;
    break;
    case TAG_String:
        tag->t_str = malloc(1);
        tag->t_str[0] = '\0';
    break;
    case TAG_List:
        tag->t_list.listTagType = 0;
        tag->t_list.list = ArrayList_new(nbt_dtor,nbt_copy,NULL,sizeof(NBT_Tag*));
    break;
    case TAG_Compound:
        tag->t_compound.map = map_new(NULL,strless,nbt_dealloc,free_dtor);
    break;
    case TAG_IntArray:
        tag->t_iarr.len = 0;
        tag->t_iarr.arr = NULL;
    break;
    case TAG_LongArray:
        tag->t_larr.len = 0;
        tag->t_larr.arr = NULL;
    break;
    case TAG_FloatArray:
        tag->t_farr.len = 0;
        tag->t_farr.arr = NULL;
    break;
    case TAG_DoubleArray:
        tag->t_darr.len = 0;
        tag->t_darr.arr = NULL;
    break;
    case TAG_UUID:
        tag->t_uuid.most = 0;
        tag->t_uuid.least = 0;
    break;
    }
    return tag;
}
NBT_Tag* NBT_DupTag(NBT_Tag* tag){
    tag->refCount++;
    return tag;
}
void NBT_FreeTag(NBT_Tag* tag){
    if(!tag->refCount){
        switch(tag->type){
        case TAG_ByteArray:
            if(tag->t_barr.arr)
                free(tag->t_barr.arr);
        break;
        case TAG_String:
            free(tag->t_str);
        break;
        case TAG_List:
            ArrayList_free(tag->t_list.list);
        break;
        case TAG_Compound:
            Map_free(tag->t_compound.map);
        break;
        case TAG_IntArray:
            if(tag->t_iarr.arr)
                free(tag->t_iarr.arr);
        break;
        case TAG_LongArray:
            if(tag->t_larr.arr)
                free(tag->t_larr.arr);
        break;
        case TAG_FloatArray:
            if(tag->t_farr.arr)
                free(tag->t_barr.arr);
        break;
        case TAG_DoubleArray:
            if(tag->t_darr.arr)
                free(tag->t_barr.arr);
        break;
        }
        free(tag);
    }else
        tag->refCount--;
}

static char* nbt_strdup(const char* str){
    size_t len = strlen(str)+1;
    char* ret = (char*) malloc(len);
    memcpy(ret,str,len);
    return ret;
}

static void shade_handler(void* jmpBuff,const char* name){
    longjmp(*(jmp_buff*)jmpBuff,-1);
}

NBT_Tag* NBT_Read(NBT_Tag* tag,BinaryIO* io,version shade_v){
    void* err_data_saved;
    error_handler_t* err_handler_saved;
    jmp_buff buf;
    err_handler_saved = setErrorHandler(io,shade_handler,&buf,&err_data_saved);
    if(setjmp(buf)){
        NBT_FreeTag(tag);
        setErrorHandler(io,err_handler_saved,err_data_saved,NULL);
        err_handler_saved(err_data_saved,BinaryIO_last_error(io));
        return NULL;
    }
    
    if(shade_v<tag_minVersions[tag->tagType]){
       BinaryIO_raise_error(io,"Invalid Tag on stream for targeted shade version");
       __builtin_unreachable();//Since the error handler is currently doing a longjmp
    }
    switch(tag->type){
    case TAG_Byte:
        tag->t_byte = read_i8(io);
    break;
    case TAG_Short:
        tag->t_short = read_i16(io);
    break;
    case TAG_Int:
        tag->t_int = read_i32(io);
    break;
    case TAG_Long:
        tag->t_long = read_i64(io);
    break;
    case TAG_Float:
        tag->t_float = read_f32(io);
    break;
    case TAG_Double:
        tag->t_double = read_f64(io);
    break; 
    case TAG_ByteArray:
        if(tag->t_barr.arr)
            free(tag->t_barr.arr);
        tag->t_barr.len = read_i32(io);
        if(tag->t_barr.len<0){
            BinaryIO_raise_error(io,"Negative length on array tag");
            __builtin_unreachable();//Since the error handler is currently doing a longjmp
        }
        tag->t_barr.arr = (int8_t*)malloc(tag->t_barr.len);
        if(!tag->t_barr.arr){
            BinaryIO_raise_error(io,"Unable to allocate space for array tag");
            __builtin_unreachable();//Since the error handler is currently doing a longjmp
        }
        for(int32_t i = 0;i<tag->t_barr.len;i++)
            tag->t_barr.arr[i] = read_i8(io);
    break;
    case TAG_List:
        {
            TAG_Type type = read_u8(io);
            if(type>=TAG_Count){
                BinaryIO_raise_error(io,"Unknown Tag Type in List Tag");
                __builtin_unreachable();//Since the error handler is currently doing a longjmp
            }else if(shade_v<tag_minVersions[type]){
               BinaryIO_raise_error(io,"Invalid Tag on stream for targeted shade version");
               __builtin_unreachable();//Since the error handler is currently doing a longjmp
            }
            ArrayList_clear(tag->t_list.list);
            int32_t len = read_i32(io);
            if(len<0){
                BinaryIO_raise_error(io,"Negative length on array tag");
                __builtin_unreachable();//Since the error handler is currently doing a longjmp
            }
            for(int32_t i = 0;i<len;i++){
                NBT_Tag* tag = NBT_CreateTag(type);
                tag = NBT_Read(tag,io,shade_v);
                ArrayList_add(&tag);
            }
        }
    break;
    case TAG_Compound:
        {
            TAG_Type type;
            char* tagN;
            while((type=read_u8(io))!=0){
                tagN = read_string(io);
                NBT_Tag* tag = NBT_CreateTag(type);
                NBT_ReadTag(tag,io,shade_v);
                map_put(tag->t_compound.map,tagN,tag);
            }
        }
    break;
    case TAG_IntArray:
        if(tag->t_iarr.arr)
            free(tag->t_iarr.arr);
        tag->t_iarr.len = read_i32(io);
        if(tag->t_iarr.len<0){
            BinaryIO_raise_error(io,"Negative length on array tag");
            __builtin_unreachable();//Since the error handler is currently doing a longjmp
        }
        tag->t_iarr.arr = (int32_t*)calloc(tag->t_iarr.len,sizeof(int32_t));
        if(!tag->t_iarr.arr){
            BinaryIO_raise_error(io,"Unable to allocate space for array tag");
            __builtin_unreachable();//Since the error handler is currently doing a longjmp
        }
        for(int32_t i = 0;i<tag->t_iarr.len;i++)
            tag->t_iarr.arr[i] = read_i32(io);
    break;
    case TAG_LongArray:
        if(tag->t_larr.arr)
            free(tag->t_larr.arr);
        tag->t_larr.len = read_i32(io);
        if(tag->t_larr.len<0){
            BinaryIO_raise_error(io,"Negative length on array tag");
            __builtin_unreachable();//Since the error handler is currently doing a longjmp
        }
        tag->t_larr.arr = (int64_t*)calloc(tag->t_larr.len,sizeof(int64_t));
        if(!tag->t_larr.arr){
            BinaryIO_raise_error(io,"Unable to allocate space for array tag");
            __builtin_unreachable();//Since the error handler is currently doing a longjmp
        }
        for(int32_t i = 0;i<tag->t_larr.len;i++)
            tag->t_larr.arr[i] = read_i64(io);
    break;
    case TAG_FloatArray:
        if(tag->t_farr.arr)
            free(tag->t_farr.arr);
        tag->t_farr.len = read_i32(io);
        if(tag->t_farr.len<0){
            BinaryIO_raise_error(io,"Negative length on array tag");
            __builtin_unreachable();//Since the error handler is currently doing a longjmp
        }
        tag->t_farr.arr = (float*)calloc(tag->t_farr.len,sizeof(float));
        if(!tag->t_farr.arr){
            BinaryIO_raise_error(io,"Unable to allocate space for array tag");
            __builtin_unreachable();//Since the error handler is currently doing a longjmp
        }
        for(int32_t i = 0;i<tag->t_farr.len;i++)
            tag->t_farr.arr[i] = read_f32(io);
    break;
    case TAG_DoubleArray:
        if(tag->t_darr.arr)
            free(tag->t_darr.arr);
        tag->t_darr.len = read_i32(io);
        if(tag->t_darr.len<0){
            BinaryIO_raise_error(io,"Negative length on array tag");
            __builtin_unreachable();//Since the error handler is currently doing a longjmp
        }
        tag->t_darr.arr = (double*)calloc(tag->t_darr.len,sizeof(double));
        if(!tag->t_darr.arr){
            BinaryIO_raise_error(io,"Unable to allocate space for array tag");
            __builtin_unreachable();//Since the error handler is currently doing a longjmp
        }
        for(int32_t i = 0;i<tag->t_darr.len;i++)
            tag->t_darr.arr[i] = read_f64(io);
    break;
    case TAG_UUID:
        tag->t_uuid = read_uuid(io);
    break;
    }
    return tag;
}
void NBT_Write(NBT_Tag* tag,BinaryIO* io,version shade_v){

}

//Direct Accessors
int8_t* NBT_AsByte(NBT_Tag* tag){
    return &tag->t_byte;
}
int16_t* NBT_AsShort(NBT_Tag* tag){
    return &tag->t_short;
}
int32_t* NBT_AsInt(NBT_Tag* tag){
    return &tag->t_int;
}
int64_t* NBT_AsLong(NBT_Tag* tag){
    return &tag->t_long;
}
float* NBT_AsFloat(NBT_Tag* tag){
    return &tag->t_float;
}
double* NBT_AsDouble(NBT_Tag* tag){
    return &tag->t_double;
}
const char* NBT_GetString(NBT_Tag* tag){
    return tag->t_string;
}
void NBT_SetString(NBT_Tag* tag,const char* str){
    free(tag->t_string);
    tag->t_string = nbt_strdup(str);
}
UUID* NBT_AsUUID(NBT_Tag* tag){
    return &tag->t_uuid;
}

//Primitive-generic accessors
bool NBT_GetBoolean(NBT_Tag* tag);
int8_t NBT_GetByte(NBT_Tag* tag);
int16_t NBT_GetShort(NBT_Tag* tag);
int32_t NBT_GetInt(NBT_Tag* tag);
int64_t NBT_GetLong(NBT_Tag* tag);
float NBT_GetFloat(NBT_Tag* tag);
double NBT_GetDouble(NBT_Tag* tag);

//Special Accessors
ListTag* NBT_AsList(NBT_Tag* tag){
    return &tag->t_list;
}
CompoundTag* NBT_AsCompound(NBT_Tag* tag){
    return &tag->t_compound;
}

//Array Accessors
ByteArray* NBT_AsByteArray(NBT_Tag* tag){
    return &tag->t_barr;
}
IntArray* NBT_AsIntArray(NBT_Tag* tag){
    return &tag->t_iarr;
}
LongArray* NBT_AsLongArray(NBT_Tag* tag){
    return &tag->t_larr;
}
FloatArray* NBT_AsFloatArray(NBT_Tag* tag){
    return &tag->t_farr;
}
DoubleArray* NBT_AsDoubleArray(NBT_Tag* tag){
    return &tag->t_darr;
}

bool NBT_IsTagType(NBT_Tag* tag,TAG_Type type){
    if(type==TAG_AnyNumeric)
        return tag->type>=TAG_Byte&&tag->type<=TAG_Float;
    else
        return tag->type==type;
}

void Compound_PutTag(CompoundTag* tag,const char* name,NBT_Tag* tag);
NBT_Tag* Compound_GetTag(CompoundTag* tag,const char* name);
bool Compound_HasTag(CompoundTag* tag,const char* name);
bool Compound_HasTagType(CompoundTag* tag,const char* name,Tag_Type);
bool Compound_GetBoolean(CompoundTag* tag,const char* name);
int8_t Compound_GetByte(CompoundTag* tag,const char* name);
int16_t Compound_GetShort(CompoundTag* tag,const char* name);
int32_t Compound_GetInt(CompoundTag* tag,const char* name);
int64_t Compound_GetLong(CompoundTag* tag,const char* name);
float Compound_GetFloat(CompoundTag* tag,const char* name);
double Compound_GetDouble(CompoundTag* tag,const char* name);
UUID Compound_GetUUID(CompoundTag* tag,const char* name);

int32_t List_GetLength(ListTag* tag);
Tag_Type List_GetListTagType(ListTag* tag);
bool List_Add(ListTag* tag,NBT_Tag* toAdd);
NBT_Tag* List_Get(ListTag* tag,int32_t idx);
