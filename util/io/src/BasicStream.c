#include <Stream.h>
#include <string.h>

/**
typedef struct StreamCalls{
    size_t(*write)(const void*,size_t,void*);
    void(*writeByte)(uint8_t,void*);
    size_t(*read)(void*,size_t,void*);
    int32_t(*readByte)(void*);
    void*(*construct)(stream_type,va_list);
    void(*destroy)(void*);
    stream_bool(*checkError)(const void*);
    void(*clearError)(void*);
    void(*flush)(void*);
} StreamCalls; 
*/

static size_t IO_null_write(const void* data,size_t sz,void* obj){
    return sz;
}

static void IO_null_writeByte(uint8_t byte,void* obj){}

static size_t IO_null_read(void* out,size_t sz,void* obj){
    return EOF;
}


static size_t IO_zeros_read(void* out,size_t sz,void* obj){
    memset(out,0,sz);
    return sz;
}

static int32_t IO_null_readByte(void* obj){
    return -1;
}

static int32_t IO_zeros_readByte(void* obj){
    return 0;
}

static char IO_null_placeholder; 

static void* IO_null_construct(stream_type type,va_list args){
    return &IO_null_placeholder;
}

static void IO_null_destroy(void* v){}

static stream_status IO_null_checkError(const void* obj){
    return IO_status_OK;
}

static void IO_null_clearError(void* obj){}

static void IO_null_flush(void* obj){}

static const StreamCalls null_Calls = {
    IO_null_write,
    IO_null_writeByte,
    IO_null_read,
    IO_null_readByte,
    IO_null_construct,
    IO_null_destroy,
    IO_null_checkError,
    IO_null_clearError,
    IO_null_flush
};

static const StreamCalls zeros_Calls = {
    IO_null_write,
    IO_null_writeByte,
    IO_zeros_read,
    IO_zeros_readByte,
    IO_null_construct,
    IO_null_destroy,
    IO_null_checkError,
    IO_null_clearError,
    IO_null_flush
};

const StreamCalls* IO_null(){
    return &null_Calls;
}

const StreamCalls* IO_zeros(){
    return &zeros_Calls;
}

