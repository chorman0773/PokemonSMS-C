#include <Stream.h>
#include <stdlib.h>
#include <stdarg.h>

struct InputStream{
    const StreamCalls* calls;
    void* underlying;
};

struct OutputStream{
    const StreamCalls* calls;
    void* underlying;
};

IO_API InputStream* InputStream_new(const StreamCalls* calls,...){
    va_list args;
    va_start(args,calls);
    InputStream* stream = malloc(sizeof(InputStream));
    stream->calls = calls;
    stream->underlying = calls->construct(IO_type_input,args);
    if(!stream->underlying){
        free(stream);
        stream = NULL;
    }
    va_end(args);
    return stream;
}


IO_API OutputStream* OutputStream_new(const StreamCalls* calls,...){
    va_list args;
    va_start(args,calls);
    OutputStream* stream = malloc(sizeof(OutputStream));
    stream->calls = calls;
    stream->underlying = calls->construct(IO_type_output,args);
    if(!stream->underlying){
        free(stream);
        stream = NULL;
    }
    va_end(args);
    return stream;
}

IO_API void InputStream_destroy(InputStream* stream){
    stream->calls->destroy(stream->underlying);
    free(stream);
}
IO_API void OutputStream_destroy(OutputStream* stream){
    stream->calls->destroy(stream->underlying);
    free(stream);
}


IO_API size_t InputStream_read(InputStream* stream,void* out,size_t size){
    return stream->calls->read(out,size,stream->underlying);
}


IO_API int32_t InputStream_readByte(InputStream* stream){
    return stream->calls->readByte(stream->underlying);
}


IO_API size_t OutputStream_write(OutputStream* stream,const void* buff,size_t size){
    return stream->calls->write(buff,size,stream->underlying);
}


IO_API void OutputStream_writeByte(OutputStream* stream,uint8_t byte){
    stream->calls->writeByte(byte,stream->underlying);
}


IO_API stream_status InputStream_checkError(const InputStream* stream){
    return stream->calls->checkError(stream->underlying);
}


IO_API stream_status OutputStream_checkError(const OutputStream* stream){
    return stream->calls->checkError(stream->underlying);
}

IO_API void InputStream_clearError(InputStream* stream){
    stream->calls->clearError(stream->underlying);
}
IO_API void OutputStream_clearError(OutputStream* stream){
    stream->calls->clearError(stream->underlying);
}


IO_API void OutputStream_flush(OutputStream* stream){
    stream->calls->flush(stream->underlying);
}



IO_API InputStream* InputStream_from(FILE* file){
    return InputStream_new(IO_file(),file);
}

IO_API InputStream* InputStream_openFilename(const char* name){
    return InputStream_new(IO_file(),fopen(name,"rb"));
}

IO_API OutputStream* OutputStream_from(FILE* file){
    return OutputStream_new(IO_file(),file);
}
IO_API OutputStream* OutputStream_openFilename(const char* name){
    return OutputStream_new(IO_file(),fopen(name,"wb"));
}
IO_API OutputStream* OutputStream_appendFilename(const char* name){
    return OutputStream_new(IO_file(),fopen(name,"ab"));
}

