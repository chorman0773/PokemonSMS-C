#include <Stream.h>
#include <stdio.h>

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

static size_t IO_file_write(const void* data,size_t sz,void* obj){
    return fwrite(data,1,sz,(FILE*)obj);
}

static void IO_file_writeByte(uint8_t byte,void* obj){
    IO_file_write(&byte,1,obj);
}

static size_t IO_file_read(void* data,size_t sz,void* obj){
    return fread(data,1,sz,(FILE*)obj);
}

static int32_t IO_file_readByte(void* obj){
    uint8_t _out;
    if(IO_file_read(&_out,1,obj))
        return (_out)&0xff;
    else
        return -1;
}

static void* IO_file_construct(stream_type type,va_list list){
    return va_arg(list,FILE*);
}

static void IO_file_destroy(void* file){
    fclose((FILE*)file);
}

static stream_status IO_file_checkError(const void* file){
    if(feof((FILE*)file))
        return IO_status_EOF;
    else if(ferror((FILE*)file))
        return IO_status_ERROR;
    else
        return IO_status_OK;
}

static void IO_file_clearError(void* file){
    clearerr((FILE*)file);
}

static void IO_file_flush(void* file){
    fflush((FILE*)file);
}

static const StreamCalls IO_file_calls = {
    IO_file_write,
    IO_file_writeByte,
    IO_file_read,
    IO_file_readByte,
    IO_file_construct,
    IO_file_destroy,
    IO_file_checkError,
    IO_file_clearError,
    IO_file_flush
};

const StreamCalls* IO_file(){
    return &IO_file_calls;
}



