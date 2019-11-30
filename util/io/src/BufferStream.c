#include <Stream.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


/**
typedef struct StreamCalls{
    size_t(*write)(const void*,size_t,void*);
    void(*writeByte)(uint8_t,void*);
    size_t(*read)(void*,size_t,void*);
    int32_t(*readByte)(void*);
    void*(*construct)(stream_type,va_list);
    void(*destroy)(void*);
    stream_status(*checkError)(const void*);
    void(*clearError)(void*);
    void(*flush)(void*);
} StreamCalls; 
*/


struct DynamicBuffer{
    size_t capacity;
    size_t position;
    size_t* sizeOut;
    void** bufferOut;
};

struct ReadBuffer{
    size_t size;
    const unsigned char* buff;
    size_t position;
};

static void* IO_buffer_construct(stream_type type,va_list args){
    if(type==IO_type_input){
        struct ReadBuffer* buff = malloc(sizeof(struct ReadBuffer));
        buff->buff = (const unsigned char*) va_arg(args,const void*);
        buff->size = va_arg(args,size_t);
        buff->position = 0;
        return buff;
    }else{
        struct DynamicBuffer* buff = malloc(sizeof(struct DynamicBuffer));
        buff->bufferOut = va_arg(args,void**);
        buff->sizeOut = va_arg(args,size_t*);
        if(*(buff->bufferOut))
            buff->capacity = va_arg(args,size_t);
        buff->position = 0;
        return buff;
    }
}

static void IO_buffer_destroy(void* obj){
    free(obj);
}

static stream_status IO_buffer_checkError(const void* obj){
    return IO_status_OK;
}

static void IO_buffer_clearError(void* obj){}
static void IO_buffer_flush(void* obj){}


static size_t IO_buffer_write(const void* data,size_t sz,void* obj){
    if(sz==0)
        return 0;
    struct DynamicBuffer* buff = (struct DynamicBuffer*)obj;
    if(!*(buff->bufferOut)){
        buff->capacity = 1<<((int)ceil(log2(sz)));
        *(buff->bufferOut) = malloc(buff->capacity);
    }else if(buff->position+sz>buff->capacity){
        buff->capacity = 1<<((int)ceil(log2(buff->position+sz)));
        *(buff->bufferOut) = realloc(*(buff->bufferOut), buff->capacity);
    }
    char* out = ((char*)*(buff->bufferOut))+buff->position;
    memcpy(out,data,sz);
    buff->position += sz;
    *(buff->sizeOut) = buff->position;
    return sz;
}

static void IO_buffer_writeByte(uint8_t t,void* obj){
    IO_buffer_write(&t,1,obj);
}

static size_t IO_buffer_read(void* data,size_t sz,void* obj){
    if(sz==0)
        return 0;
    struct ReadBuffer* buff = (struct ReadBuffer*)obj;
    size_t moved = buff->position+sz<=buff->size?sz:buff->size-buff->position;
    if(moved==0)
        return 0;
    memcpy(data,&buff->buff[buff->position],moved);
    buff->position+=sz;
    return moved;
}

static int32_t IO_buffer_readByte(void* obj){
    struct ReadBuffer* buff = (struct ReadBuffer*)obj;
    if(buff->position!=buff->size)
        return buff->buff[buff->position++]&0xFF;
    else
        return -1;
}

static const StreamCalls IO_buffer_calls = {
    IO_buffer_write,
    IO_buffer_writeByte,
    IO_buffer_read,
    IO_buffer_readByte,
    IO_buffer_construct,
    IO_buffer_destroy,
    IO_buffer_checkError,
    IO_buffer_clearError,
    IO_buffer_flush
};

const StreamCalls* IO_buffer(){
    return &IO_buffer_calls;
}




