#include <NBT.h>
#include <Map.h>
#include <List.h>
#include <stdio.h>
#include <ShadeConstants.h>
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>

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
    v1_0,
    v1_1,
    v1_3,
    v1_3,
    v1_3
};

static void nbt_copy(void* v,const void* vv){
    *((NBT_Tag**)v) = NBT_DupTag(*((NBT_Tag*const*)vv));
}

static void nbt_dtor(void* v){
    NBT_FreeTag(*((NBT_Tag**)v));
}
static void nbt_dealloc(void* v,void* data){
    NBT_FreeTag((NBT_Tag*)data);
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
        tag->t_barr.array = NULL;
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
        tag->t_iarr.array = NULL;
    break;
    case TAG_LongArray:
        tag->t_larr.len = 0;
        tag->t_larr.array = NULL;
    break;
    case TAG_FloatArray:
        tag->t_farr.len = 0;
        tag->t_farr.array = NULL;
    break;
    case TAG_DoubleArray:
        tag->t_darr.len = 0;
        tag->t_darr.array = NULL;
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
            if(tag->t_barr.array)
                free(tag->t_barr.array);
        break;
        case TAG_String:
            free(tag->t_str);
        break;
        case TAG_List:
            ArrayList_free(tag->t_list.list);
        break;
        case TAG_Compound:
            map_free(tag->t_compound.map);
        break;
        case TAG_IntArray:
            if(tag->t_iarr.array)
                free(tag->t_iarr.array);
        break;
        case TAG_LongArray:
            if(tag->t_larr.array)
                free(tag->t_larr.array);
        break;
        case TAG_FloatArray:
            if(tag->t_farr.array)
                free(tag->t_barr.array);
        break;
        case TAG_DoubleArray:
            if(tag->t_darr.array)
                free(tag->t_barr.array);
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
    longjmp(*(jmp_buf*)jmpBuff,-1);
}

NBT_Tag* NBT_Read(NBT_Tag* tag,BinaryIO* io,version shade_v){
    void* err_data_saved;
    error_handler_t* err_handler_saved;
    jmp_buf buf;
    err_handler_saved = setErrorHandler(io,shade_handler,&buf,&err_data_saved);
    if(setjmp(buf)){
        fprintf(stderr,"Backtrace: Reading Tag with type 0x%02x\n",(unsigned)tag->type);
        NBT_FreeTag(tag);
        setErrorHandler(io,err_handler_saved,err_data_saved,NULL);
        err_handler_saved(err_data_saved,BinaryIO_last_error(io));
        return NULL;
    }
    
    if(shade_v<tag_minVersions[tag->type]){
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
        if(tag->t_barr.array)
            free(tag->t_barr.array);
        tag->t_barr.len = read_i32(io);
        if(tag->t_barr.len<0){
            BinaryIO_raise_error(io,"Negative length on array tag");
            __builtin_unreachable();//Since the error handler is currently doing a longjmp
        }
        tag->t_barr.array = (int8_t*)malloc(tag->t_barr.len);
        if(!tag->t_barr.array){
            BinaryIO_raise_error(io,"Unable to allocate space for array tag");
            __builtin_unreachable();//Since the error handler is currently doing a longjmp
        }
        read_fully(io,tag->t_barr.array,tag->t_barr.len);
    break;
    case TAG_String:
        tag->t_str = read_string(io);//malloc allocated, so its fine to just assign here
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
                ArrayList_add(tag->t_list.list,&tag);
            }
        }
    break;
    case TAG_Compound:
        {
            TAG_Type type;
            char* tagN;
            while((type=read_u8(io))!=0){
                tagN = read_string(io);
                printf("Reading tag with name %s and type 0x%02x\n",tagN,type);
                NBT_Tag* ntag = NBT_CreateTag(type);
                NBT_Read(ntag,io,shade_v);
                map_put(tag->t_compound.map,tagN,ntag);
            }
        }
    break;
    case TAG_IntArray:
        if(tag->t_iarr.array)
            free(tag->t_iarr.array);
        tag->t_iarr.len = read_i32(io);
        if(tag->t_iarr.len<0){
            BinaryIO_raise_error(io,"Negative length on array tag");
            __builtin_unreachable();//Since the error handler is currently doing a longjmp
        }
        tag->t_iarr.array = (int32_t*)calloc(tag->t_iarr.len,sizeof(int32_t));
        if(!tag->t_iarr.array){
            BinaryIO_raise_error(io,"Unable to allocate space for array tag");
            __builtin_unreachable();//Since the error handler is currently doing a longjmp
        }
        for(int32_t i = 0;i<tag->t_iarr.len;i++)
            tag->t_iarr.array[i] = read_i32(io);
    break;
    case TAG_LongArray:
        if(tag->t_larr.array)
            free(tag->t_larr.array);
        tag->t_larr.len = read_i32(io);
        if(tag->t_larr.len<0){
            BinaryIO_raise_error(io,"Negative length on array tag");
            __builtin_unreachable();//Since the error handler is currently doing a longjmp
        }
        tag->t_larr.array = (int64_t*)calloc(tag->t_larr.len,sizeof(int64_t));
        if(!tag->t_larr.array){
            BinaryIO_raise_error(io,"Unable to allocate space for array tag");
            __builtin_unreachable();//Since the error handler is currently doing a longjmp
        }
        for(int32_t i = 0;i<tag->t_larr.len;i++)
            tag->t_larr.array[i] = read_i64(io);
    break;
    case TAG_FloatArray:
        if(tag->t_farr.array)
            free(tag->t_farr.array);
        tag->t_farr.len = read_i32(io);
        if(tag->t_farr.len<0){
            BinaryIO_raise_error(io,"Negative length on array tag");
            __builtin_unreachable();//Since the error handler is currently doing a longjmp
        }
        tag->t_farr.array = (float*)calloc(tag->t_farr.len,sizeof(float));
        if(!tag->t_farr.array){
            BinaryIO_raise_error(io,"Unable to allocate space for array tag");
            __builtin_unreachable();//Since the error handler is currently doing a longjmp
        }
        for(int32_t i = 0;i<tag->t_farr.len;i++)
            tag->t_farr.array[i] = read_f32(io);
    break;
    case TAG_DoubleArray:
        if(tag->t_darr.array)
            free(tag->t_darr.array);
        tag->t_darr.len = read_i32(io);
        if(tag->t_darr.len<0){
            BinaryIO_raise_error(io,"Negative length on array tag");
            __builtin_unreachable();//Since the error handler is currently doing a longjmp
        }
        tag->t_darr.array = (double*)calloc(tag->t_darr.len,sizeof(double));
        if(!tag->t_darr.array){
            BinaryIO_raise_error(io,"Unable to allocate space for array tag");
            __builtin_unreachable();//Since the error handler is currently doing a longjmp
        }
        for(int32_t i = 0;i<tag->t_darr.len;i++)
            tag->t_darr.array[i] = read_f64(io);
    break;
    case TAG_UUID:
        tag->t_uuid = read_uuid(io);
    break;
    }
    setErrorHandler(io,err_handler_saved,err_data_saved,NULL);//Remember to uninstall the handler
    return tag;
}

static TAG_Type alt_tag_type(TAG_Type type){
    if(type>=TAG_LongArray&&type<=TAG_DoubleArray)
        return TAG_List;
    else if(type==TAG_UUID)
        return TAG_Compound;
    else
        return type;
}

void NBT_Write(NBT_Tag* tag,BinaryIO* io,version shade_v){
    switch(tag->type){
    case TAG_Byte:
        write_i8(io,tag->t_byte);
    break;
    case TAG_Short:
        write_i16(io,tag->t_short);
    break;
    case TAG_Int:
        write_i32(io,tag->t_int);
    break;
    case TAG_Long:
        write_i64(io,tag->t_long);
    break;
    case TAG_Float:
        write_f32(io,tag->t_float);
    break;
    case TAG_Double:
        write_f64(io,tag->t_double);
    break;
    case TAG_ByteArray:
        write_i32(io,tag->t_barr.len);
        write_bytes(io,tag->t_barr.array,tag->t_barr.len);
    break;
    case TAG_String:
        write_string(io,tag->t_str);
    break;
    case TAG_List:
	{
        TAG_Type type = tag->t_list.listTagType;
        if(shade_v<tag_minVersions[type])
            type = alt_tag_type(type);
        write_u8(io,type);
        write_i32(io,(int32_t)ArrayList_length(tag->t_list.list));
        size_t len = ArrayList_length(tag->t_list.list);
        for(size_t i = 0;i<len;i++)
            NBT_Write((NBT_Tag*)ArrayList_get(tag->t_list.list,i),io,shade_v);
    }
    break;
    case TAG_Compound:
        for(MapIterator* it = map_begin(tag->t_compound.map);it!=NULL;it=map_next(it)){
            const char* name = (const char*)map_deref_it(it);
            NBT_Tag* ntag = (NBT_Tag*) map_deref_it_value(it);
            TAG_Type type = ntag->type;
            if(shade_v<tag_minVersions[type])
                type = alt_tag_type(type);
            write_u8(io,type);
            write_string(io,name);
            NBT_Write(ntag,io,shade_v);
        }
        write_u8(io,TAG_End);//End Tag at the end of the Compound
    break;
    case TAG_IntArray:
        write_i32(io,tag->t_iarr.len);
        for(size_t i = 0;i<tag->t_iarr.len;i++)
            write_i32(io,tag->t_iarr.array[i]);
    break;
    case TAG_LongArray:
        if(shade_v<v1_1)
            write_u8(io,TAG_Long);//Encode LongArray as List of Longs in 1.0
        write_i32(io,tag->t_larr.len);
        for(size_t i = 0;i<tag->t_larr.len;i++)
            write_i64(io,tag->t_larr.array[i]);
    break;
    case TAG_FloatArray:
        if(shade_v<v1_3)
            write_u8(io,TAG_Float);//Encode LongArray as List of Longs in 1.2 or below
        write_i32(io,tag->t_farr.len);
        for(size_t i = 0;i<tag->t_farr.len;i++)
            write_f32(io,tag->t_farr.array[i]);
    break;
    case TAG_DoubleArray:
        if(shade_v<v1_3)
            write_u8(io,TAG_Double);
        write_i32(io,tag->t_darr.len);
        for(size_t i = 0;i<tag->t_darr.len;i++)
            write_f64(io,tag->t_darr.array[i]);
    break;
    case TAG_UUID:
        if(shade_v<v1_3){
            write_u8(io,TAG_Long);
            write_string(io,"m");
            write_u64(io,tag->t_uuid.most);
            write_u8(io,TAG_Long);
            write_string(io,"l");
            write_u64(io,tag->t_uuid.least);
        }else
            write_uuid(io,tag->t_uuid);
    break;
    }
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
    return tag->t_str;
}
void NBT_SetString(NBT_Tag* tag,const char* str){
    free(tag->t_str);
    tag->t_str = nbt_strdup(str);
}
UUID* NBT_AsUUID(NBT_Tag* tag){
    return &tag->t_uuid;
}

//Primitive-generic accessors
bool NBT_GetBoolean(NBT_Tag* tag){
    if(!NBT_IsTagType(tag,TAG_AnyNumeric))
        return false;
    else switch(tag->type){
    case TAG_Byte:
        return tag->t_byte;
    break;
    case TAG_Short:
        return tag->t_short;
    break;
    case TAG_Int:
        return tag->t_int;
    break;
    case TAG_Long:
        return tag->t_long;
    break;
    case TAG_Float:
        return tag->t_float;
    break;
    case TAG_Double:
        return tag->t_double;
    break;
    }
    __builtin_unreachable();
}
int8_t NBT_GetByte(NBT_Tag* tag){
    if(!NBT_IsTagType(tag,TAG_AnyNumeric))
        return 0;
    else switch(tag->type){
    case TAG_Byte:
        return tag->t_byte;
    break;
    case TAG_Short:
        return tag->t_short;
    break;
    case TAG_Int:
        return tag->t_int;
    break;
    case TAG_Long:
        return tag->t_long;
    break;
    case TAG_Float:
        return tag->t_float;
    break;
    case TAG_Double:
        return tag->t_double;
    break;
    }
    __builtin_unreachable();
}
int16_t NBT_GetShort(NBT_Tag* tag){
    if(!NBT_IsTagType(tag,TAG_AnyNumeric))
        return 0;
    else switch(tag->type){
    case TAG_Byte:
        return tag->t_byte;
    break;
    case TAG_Short:
        return tag->t_short;
    break;
    case TAG_Int:
        return tag->t_int;
    break;
    case TAG_Long:
        return tag->t_long;
    break;
    case TAG_Float:
        return tag->t_float;
    break;
    case TAG_Double:
        return tag->t_double;
    break;
    }
    __builtin_unreachable();
}
int32_t NBT_GetInt(NBT_Tag* tag){
    if(!NBT_IsTagType(tag,TAG_AnyNumeric))
        return 0;
    else switch(tag->type){
    case TAG_Byte:
        return tag->t_byte;
    break;
    case TAG_Short:
        return tag->t_short;
    break;
    case TAG_Int:
        return tag->t_int;
    break;
    case TAG_Long:
        return tag->t_long;
    break;
    case TAG_Float:
        return tag->t_float;
    break;
    case TAG_Double:
        return tag->t_double;
    break;
    }
    __builtin_unreachable();
}
int64_t NBT_GetLong(NBT_Tag* tag){
    if(!NBT_IsTagType(tag,TAG_AnyNumeric))
        return 0;
    else switch(tag->type){
    case TAG_Byte:
        return tag->t_byte;
    break;
    case TAG_Short:
        return tag->t_short;
    break;
    case TAG_Int:
        return tag->t_int;
    break;
    case TAG_Long:
        return tag->t_long;
    break;
    case TAG_Float:
        return tag->t_float;
    break;
    case TAG_Double:
        return tag->t_double;
    break;
    }
    __builtin_unreachable();
}
float NBT_GetFloat(NBT_Tag* tag){
    if(!NBT_IsTagType(tag,TAG_AnyNumeric))
        return 0.f;
    else switch(tag->type){
    case TAG_Byte:
        return tag->t_byte;
    break;
    case TAG_Short:
        return tag->t_short;
    break;
    case TAG_Int:
        return tag->t_int;
    break;
    case TAG_Long:
        return tag->t_long;
    break;
    case TAG_Float:
        return tag->t_float;
    break;
    case TAG_Double:
        return tag->t_double;
    break;
    }
    __builtin_unreachable();
}
double NBT_GetDouble(NBT_Tag* tag){
    if(!NBT_IsTagType(tag,TAG_AnyNumeric))
        return 0.0;
    else switch(tag->type){
    case TAG_Byte:
        return tag->t_byte;
    break;
    case TAG_Short:
        return tag->t_short;
    break;
    case TAG_Int:
        return tag->t_int;
    break;
    case TAG_Long:
        return tag->t_long;
    break;
    case TAG_Float:
        return tag->t_float;
    break;
    case TAG_Double:
        return tag->t_double;
    break;
    }
    __builtin_unreachable();
}

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

void Compound_PutTag(CompoundTag* tag,const char* name,NBT_Tag* nbt){
    map_put(tag->map,nbt_strdup(name),NBT_DupTag(nbt));
}
NBT_Tag* Compound_GetTag(CompoundTag* tag,const char* name){
    return map_get(tag->map,name);
}
bool Compound_HasTag(CompoundTag* tag,const char* name){
    return map_get(tag->map,name);
}
bool Compound_HasTagType(CompoundTag* tag,const char* name,TAG_Type type){
    NBT_Tag* n_tag = map_get(tag->map,name);
    return n_tag && NBT_IsTagType(n_tag,type);
}
bool Compound_GetBoolean(CompoundTag* tag,const char* name){
    return Compound_HasTagType(tag,name,TAG_AnyNumeric) && NBT_GetBoolean(Compound_GetTag(tag,name));
}
int8_t Compound_GetByte(CompoundTag* tag,const char* name){
    return !Compound_HasTagType(tag,name,TAG_AnyNumeric)? 0 : NBT_GetByte(Compound_GetTag(tag,name));
}
int16_t Compound_GetShort(CompoundTag* tag,const char* name){
    return !Compound_HasTagType(tag,name,TAG_AnyNumeric)? 0 : NBT_GetShort(Compound_GetTag(tag,name));
}
int32_t Compound_GetInt(CompoundTag* tag,const char* name){
    return !Compound_HasTagType(tag,name,TAG_AnyNumeric)? 0 : NBT_GetInt(Compound_GetTag(tag,name));
}
int64_t Compound_GetLong(CompoundTag* tag,const char* name){
    return !Compound_HasTagType(tag,name,TAG_AnyNumeric)? 0 : NBT_GetLong(Compound_GetTag(tag,name));
}
float Compound_GetFloat(CompoundTag* tag,const char* name){
    return !Compound_HasTagType(tag,name,TAG_AnyNumeric)? 0 : NBT_GetFloat(Compound_GetTag(tag,name));
}
double Compound_GetDouble(CompoundTag* tag,const char* name){
    return !Compound_HasTagType(tag,name,TAG_AnyNumeric)? 0 : NBT_GetDouble(Compound_GetTag(tag,name));
}
UUID Compound_GetUUID(CompoundTag* tag,const char* name){
    if(Compound_HasTagType(tag,name,TAG_UUID))
        return Compound_GetTag(tag,name)->t_uuid;
    else if(Compound_HasTagType(tag,name,TAG_Compound)){
        UUID val;
        CompoundTag* cmp = NBT_AsCompound(Compound_GetTag(tag,name));
        val.most = (uint64_t)Compound_GetLong(cmp,"m");
        val.least = (uint64_t)Compound_GetLong(cmp,"l");
        return val;
    }else{
        UUID val = {0};
        size_t nlen = strlen(name);
        char* leastN = malloc(nlen+sizeof("Least"));
        memcpy(leastN,name,nlen+1);
        strcat(leastN,"Least");
        if(Compound_HasTagType(tag,leastN,TAG_AnyNumeric))
            val.least = Compound_GetLong(tag,name);
        strcpy(leastN+nlen,"Most");
        if(Compound_HasTagType(tag,leastN,TAG_AnyNumeric))
            val.most = Compound_GetLong(tag,name);
        return val;
    }
}

void Compound_SetBoolean(CompoundTag* tag,const char* name,bool val){
    NBT_Tag* btag = NBT_CreateTag(TAG_Byte);
    btag->t_byte = val?1:0;
    Compound_PutTag(tag,name,btag);
    NBT_FreeTag(btag);
}
void Compound_SetByte(CompoundTag* tag,const char* name,int8_t val){
    NBT_Tag* btag = NBT_CreateTag(TAG_Byte);
    btag->t_byte = val;
    Compound_PutTag(tag,name,btag);
    NBT_FreeTag(btag);
}
void Compound_SetShort(CompoundTag* tag,const char* name,int16_t val){
    NBT_Tag* btag = NBT_CreateTag(TAG_Short);
    btag->t_short = val;
    Compound_PutTag(tag,name,btag);
    NBT_FreeTag(btag);
}
void Compound_SetInt(CompoundTag* tag,const char* name,int32_t val){
    NBT_Tag* btag = NBT_CreateTag(TAG_Int);
    btag->t_int = val;
    Compound_PutTag(tag,name,btag);
    NBT_FreeTag(btag);
}
void Compound_SetLong(CompoundTag* tag,const char* name,int64_t val){
    NBT_Tag* btag = NBT_CreateTag(TAG_Long);
    btag->t_long = val;
    Compound_PutTag(tag,name,btag);
    NBT_FreeTag(btag);
}
void Compound_SetFloat(CompoundTag* tag,const char* name,float val){
    NBT_Tag* btag = NBT_CreateTag(TAG_Float);
    btag->t_float = val;
    Compound_PutTag(tag,name,btag);
    NBT_FreeTag(btag);
}
void Compound_SetDouble(CompoundTag* tag,const char* name,double val){
    NBT_Tag* btag = NBT_CreateTag(TAG_Double);
    btag->t_double = val;
    Compound_PutTag(tag,name,btag);
    NBT_FreeTag(btag);
}
void Compound_SetUUID(CompoundTag* tag,const char* name,UUID val){
    NBT_Tag* btag = NBT_CreateTag(TAG_UUID);
    btag->t_uuid = val;
    Compound_PutTag(tag,name,btag);
    NBT_FreeTag(btag);
}

int32_t List_GetLength(ListTag* tag){
    return ArrayList_length(tag->list);
}
TAG_Type List_GetListTagType(ListTag* tag){
    return tag->listTagType;
}
bool List_Add(ListTag* tag,NBT_Tag* toAdd){
    if(!tag->listTagType){
        tag->listTagType = toAdd->type;
        ArrayList_add(tag->list,toAdd);
        return true;
    }else if(tag->listTagType==toAdd->type){
        ArrayList_add(tag->list,toAdd);
        return true;
    }else
        return false;
    
}
NBT_Tag* List_Get(ListTag* tag,int32_t idx){
    return (NBT_Tag*)ArrayList_get(tag->list,idx);
}

