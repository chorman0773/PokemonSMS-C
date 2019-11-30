#include <BinaryIO.h>
#include <errno.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

struct BinaryIO{
    InputStream* in;
    OutputStream* out;
    void(*err_handler)(void* data);
    void* err_data;
    endianess end;
};

static void default_error_handler(void* v){
    errno = EILSEQ;
}

BinaryIO* wrapInputStream(InputStream* stream,endianess end){
    BinaryIO* io = (BinaryIO*) malloc(sizeof(BinaryIO));
    io->in = stream;
    io->out = NULL;
    io->err_handler = default_error_handler;
    io->err_data = NULL;
    io->end = end;
    return io;
}

BinaryIO* wrapOutputStream(OutputStream* stream,endianess end){
    BinaryIO* io = (BinaryIO*) malloc(sizeof(BinaryIO));
    io->in = NULL;
    io->out = stream;
    io->err_handler = default_error_handler;
    io->err_data = NULL;
    io->end = end;
    return io;
}

error_handler_t* setErrorHandler(BinaryIO* io,error_handler_t* err_handler,void* data){
    error_handler_t* prev_handler = io->err_handler;
    io->err_handler = err_handler;
    io->err_data = data;
    return prev_handler;
}

void BinaryIO_free(BinaryIO* io){
    free(io);
}

endianess setEndianess(BinaryIO* io,endianess end){
    endianess prev = io->end;
    io->end = end;
    return prev;
}

void write_i8(BinaryIO* out,int8_t val){
    OutputStream_writeByte(out->out,val);
}
void write_i16(BinaryIO* out,int16_t val){
    if(out->end==BINARY_LITTLE_ENDIAN){
        write_i8(out,(uint8_t)val);
        write_i8(out,(uint8_t)(val>>8));
    }else{
        write_i8(out,(uint8_t)val);
        write_i8(out,(uint8_t)(val>>8));
    }
}
void write_i32(BinaryIO* out,int32_t val){
    if(out->end==BINARY_LITTLE_ENDIAN){
        write_i8(out,(uint8_t)val);
        write_i8(out,(uint8_t)(val>>8));
        write_i8(out,(uint8_t)(val>>16));
        write_i8(out,(uint8_t)(val>>24));
    }else{
        write_i8(out,(uint8_t)(val>>24));
        write_i8(out,(uint8_t)(val>>16));
        write_i8(out,(uint8_t)(val>>8));
        write_i8(out,(uint8_t)val);
    }
}
void write_i64(BinaryIO* out,int64_t val){
    if(out->end==BINARY_LITTLE_ENDIAN){
        write_i8(out,(uint8_t)val);
        write_i8(out,(uint8_t)(val>>8));
        write_i8(out,(uint8_t)(val>>16));
        write_i8(out,(uint8_t)(val>>24));
        write_i8(out,(uint8_t)(val>>32));
        write_i8(out,(uint8_t)(val>>40));
        write_i8(out,(uint8_t)(val>>48));
        write_i8(out,(uint8_t)(val>>56));
    }else{
        write_i8(out,(uint8_t)(val>>56));
        write_i8(out,(uint8_t)(val>>48));
        write_i8(out,(uint8_t)(val>>40));
        write_i8(out,(uint8_t)(val>>32));
        write_i8(out,(uint8_t)(val>>24));
        write_i8(out,(uint8_t)(val>>16));
        write_i8(out,(uint8_t)(val>>8));
        write_i8(out,(uint8_t)val);
    }
}
void write_u8(BinaryIO* out,uint8_t val){
    OutputStream_writeByte(out->out,val);
}
void write_u16(BinaryIO* out,uint16_t val){
    if(out->end==BINARY_LITTLE_ENDIAN){
        write_i8(out,(uint8_t)val);
        write_i8(out,(uint8_t)(val>>8));
    }else{
        write_i8(out,(uint8_t)val);
        write_i8(out,(uint8_t)(val>>8));
    }
}
void write_u32(BinaryIO* out,uint32_t val){
    if(out->end==BINARY_LITTLE_ENDIAN){
        write_i8(out,(uint8_t)val);
        write_i8(out,(uint8_t)(val>>8));
        write_i8(out,(uint8_t)(val>>16));
        write_i8(out,(uint8_t)(val>>24));
    }else{
        write_i8(out,(uint8_t)(val>>24));
        write_i8(out,(uint8_t)(val>>16));
        write_i8(out,(uint8_t)(val>>8));
        write_i8(out,(uint8_t)val);
    }
}
void write_u64(BinaryIO* out,uint64_t val){
    if(out->end==BINARY_LITTLE_ENDIAN){
        write_i8(out,(uint8_t)val);
        write_i8(out,(uint8_t)(val>>8));
        write_i8(out,(uint8_t)(val>>16));
        write_i8(out,(uint8_t)(val>>24));
        write_i8(out,(uint8_t)(val>>32));
        write_i8(out,(uint8_t)(val>>40));
        write_i8(out,(uint8_t)(val>>48));
        write_i8(out,(uint8_t)(val>>56));
    }else{
        write_i8(out,(uint8_t)(val>>56));
        write_i8(out,(uint8_t)(val>>48));
        write_i8(out,(uint8_t)(val>>40));
        write_i8(out,(uint8_t)(val>>32));
        write_i8(out,(uint8_t)(val>>24));
        write_i8(out,(uint8_t)(val>>16));
        write_i8(out,(uint8_t)(val>>8));
        write_i8(out,(uint8_t)val);
    }
}
void write_f32(BinaryIO* out,float val){
    uint32_t u32;
    memcpy(&u32,&val,4);
    write_u32(out,u32);
}
void write_f64(BinaryIO* out,double val){
    uint64_t u64;
    memcpy(&u64,&val,8);
    write_u64(out,u64);
}
void write_bool(BinaryIO* out,bool val){
    write_u8(out,val?1:0);
}

void write_version(BinaryIO* out,version val){
    write_u8(out,(uint8_t)(val>>8));
    write_u8(out,(uint8_t)val);
}
void write_uuid(BinaryIO* out,UUID id){
    write_u64(out,id.most);
    write_u64(out,id.least);
}
void write_string(BinaryIO* out,const char* ch){
    size_t len = strlen(ch);
    if(len>65535)
        len = 65535;
    write_u16(out,(uint16_t)len);
    for(size_t i = 0;i<len;i++)
        write_u8(out,(uint8_t)ch[i]);
    
}
void write_duration(BinaryIO* out,Duration dur){
    write_u64(out,dur.seconds);
    write_u32(out,dur.nanos);
}
void write_instant(BinaryIO* out,Instant dur){
    write_u64(out,dur.seconds);
    write_u32(out,dur.nanos);
}

int8_t read_i8(BinaryIO* in){
    return (int8_t)read_u8(in);
}
int16_t read_i16(BinaryIO* in){
    return (int16_t)read_u16(in);
}
int32_t read_i32(BinaryIO* in){
    return (int32_t)read_u32(in);
}
int64_t read_i64(BinaryIO* in){
    return (int64_t)read_u64(in);
}
uint8_t read_u8(BinaryIO* in){
    int32_t val = InputStream_readByte(in->in);
    if(val<0)
        in->err_handler(in->err_data);
    return (uint8_t)val;
}
uint16_t read_u16(BinaryIO* in){
    if(in->end==BINARY_LITTLE_ENDIAN){
        uint8_t tmp = read_u8(in);
        return tmp | ((uint16_t)read_u8(in))<<8;
    }else{
        uint8_t tmp = read_u8(in);
        return ((uint16_t)tmp)<<8 | read_u8(in);
    }    
}
uint32_t read_u32(BinaryIO* in){
    if(in->end==BINARY_LITTLE_ENDIAN){
        uint16_t tmp = read_u16(in);
        return tmp | ((uint32_t)read_u16(in))<<16;
    }else{
        uint16_t tmp = read_u16(in);
        return ((uint32_t)tmp)<<8 | read_u8(in);
    }    
}
uint64_t read_u64(BinaryIO* in){
    if(in->end==BINARY_LITTLE_ENDIAN){
        uint32_t tmp = read_u32(in);
        return tmp | ((uint64_t)read_u16(in))<<32;
    }else{
        uint32_t tmp = read_u16(in);
        return ((uint64_t)tmp)<<32 | read_u8(in);
    } 
}  
float read_f32(BinaryIO* out){
    union{
        uint32_t a;
        float f;
    } u = {read_u32(out)};
    return u.f;
}
double read_f64(BinaryIO* out){
    union{
        uint64_t a;
        double f;
    } u = {read_u32(out)};
    return u.f;
}
bool read_bool(BinaryIO* out){
    uint8_t val = read_u8(out);
    if(val==1)
        return true;
    else if(val==0)
        return false;
    else{
        out->err_handler(out->err_data);
        return true;
    }
}
version read_version(BinaryIO* in){
    uint8_t tmp = read_u8(in);
    return ((uint16_t)tmp)<<8|read_u8(in);
}
UUID read_uuid(BinaryIO* in){
    UUID id;
    id.most = read_u64(in);
    id.least = read_u64(in);
    return id;
}
char* read_string(BinaryIO* in){
    size_t len = read_u16(in);
    char* buff = malloc(len+1);
    if(InputStream_read(in->in,buff,len)!=len){
        free(buff);
        in->err_handler(in->err_data);
        return NULL;
    }
    buff[len] = '\0';
    if(strlen(buff)!=len){
        free(buff);
        in->err_handler(in->err_data);
        return NULL;
    }
    return buff;
}

Duration read_duration(BinaryIO* in){
    Duration dur;
    dur.seconds = read_u64(in);
    dur.nanos = read_i32(in);
    if(dur.nanos>=1000000){
        dur.seconds = 0xFFFFFFFFFFFFFFFF;
        dur.nanos = 0xFFFFFFFF;
        in->err_handler(in->err_data);
    }
    return dur;
}

Instant read_instant(BinaryIO* in){
    Instant dur;
    dur.seconds = read_u64(in);
    dur.nanos = read_i32(in);
    if(dur.nanos>=1000000){
        dur.seconds = 0xFFFFFFFFFFFFFFFF;
        dur.nanos = 0xFFFFFFFF;
        in->err_handler(in->err_data);
    }
    return dur;
}
