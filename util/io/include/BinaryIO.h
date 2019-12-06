#ifndef UTIL_IO_BINARY_IO_H_2019_11_30_17_14
#define UTIL_IO_BINARY_IO_H_2019_11_30_17_14

#ifdef __cplusplus
extern"C"{
#else
#include <stdbool.h>
#endif

#include <Stream.h>
#include <Time.h>
#include <stdint.h>

typedef struct BinaryIO BinaryIO;

typedef uint8_t endianess;

typedef uint16_t version;

typedef struct{
    uint64_t most;
    uint64_t least;
}UUID;

enum{
    BINARY_LITTLE_ENDIAN = 0,
    BINARY_BIG_ENDIAN = 1
};

typedef void(error_handler_t)(void*,const char*);

IO_API BinaryIO* wrapInputStream(InputStream* stream,endianess end);
IO_API BinaryIO* wrapOutputStream(OutputStream* stream,endianess end);
IO_API InputStream* get_wrapped_input(BinaryIO* io);
IO_API OutputStream* get_wrapped_output(BinaryIO* out);
IO_API error_handler_t* setErrorHandler(BinaryIO* io,error_handler_t* err_handler,void* data,void** savedData);

IO_API void BinaryIO_free(BinaryIO*);

IO_API endianess setEndianess(BinaryIO* io,endianess end);

IO_API void write_i8(BinaryIO* out,int8_t val);
IO_API void write_i16(BinaryIO* out,int16_t val);
IO_API void write_i32(BinaryIO* out,int32_t val);
IO_API void write_i64(BinaryIO* out,int64_t val);
IO_API void write_u8(BinaryIO* out,uint8_t val);
IO_API void write_u16(BinaryIO* out,uint16_t val);
IO_API void write_u32(BinaryIO* out,uint32_t val);
IO_API void write_u64(BinaryIO* out,uint64_t val);
IO_API void write_f32(BinaryIO* out,float val);
IO_API void write_f64(BinaryIO* out,double val);
IO_API void write_bool(BinaryIO* out,bool val);
IO_API void write_version(BinaryIO* out,version val);
IO_API void write_uuid(BinaryIO* out,UUID id);
IO_API void write_string(BinaryIO* out,const char* ch);
IO_API void write_duration(BinaryIO* out,Duration dur);
IO_API void write_instant(BinaryIO* out,Instant dur);
IO_API void write_bytes(BinaryIO* out,const void* bytes,size_t len);

IO_API int8_t read_i8(BinaryIO* in);
IO_API int16_t read_i16(BinaryIO* in);
IO_API int32_t read_i32(BinaryIO* in);
IO_API int64_t read_i64(BinaryIO* in);
IO_API uint8_t read_u8(BinaryIO* in);
IO_API uint16_t read_u16(BinaryIO* in);
IO_API uint32_t read_u32(BinaryIO* in);
IO_API uint64_t read_u64(BinaryIO* in);
IO_API float read_f32(BinaryIO* out);
IO_API double read_f64(BinaryIO* out);
IO_API bool read_bool(BinaryIO* out);
IO_API version read_version(BinaryIO* in);
IO_API UUID read_uuid(BinaryIO* in);
/**
* Returns pointer to a null-terminated multibyte string encoded in utf-8, allocated using malloc and read from in. 
* If an error occurs, NULL is returned (the buffer will be freed automatically in this case). 
* Otherwise, the program is responsible for freeing the buffer. 
*/
IO_API char* read_string(BinaryIO* in);
IO_API Duration read_duration(BinaryIO* in);
IO_API Instant read_instant(BinaryIO* in);
IO_API size_t read_fully(BinaryIO* io,void* out,size_t len);


IO_API void BinaryIO_raise_error(BinaryIO* io,const char* msg);
IO_API const char* BinaryIO_last_error(BinaryIO* io);
#ifdef __cplusplus
};
#endif

#endif
