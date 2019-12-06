#ifndef IO_INCLUDE_STREAM_H_2019_11_30_12_26
#define IO_INCLUDE_STREAM_H_2019_11_30_12_26


#ifdef __cplusplus
extern"C"{
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>

#ifdef _WIN32
#define IO_EXPORT __declspec(dllexport)
#define IO_IMPORT __declspec(dllimport)
#else
#define IO_EXPORT __attribute__((visibility("default")))
#define IO_IMPORT __attribute__((visibility("default")))
#endif

#ifdef IO_BUILD
#define IO_API IO_EXPORT
#else
#define IO_API IO_IMPORT
#endif

typedef size_t write_size;

typedef uint32_t stream_type;
typedef int8_t stream_status;


enum{
    IO_type_input = 0, IO_type_output = 1
};

enum{
    IO_status_OK = 0, IO_status_EOF = 1, IO_status_ERROR = -1
};

/**
* Structure containing callsites for Stream objects.
*/
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


typedef struct InputStream InputStream;
typedef struct OutputStream OutputStream;

/**
* Returns an IO Stream Call set with only a nullary constructor.
* Writes are discarded to no effect, reads return EOF
*/
IO_API const StreamCalls* IO_null();
/**
* Returns an IO Stream Call set with only a nullary constructor.
* Writes are discarded to no effect, reads result in 0 bytes.
*/
IO_API const StreamCalls* IO_zeros();

/**
* Returns an IO Stream Call set with a constructor that solely takes a FILE* arguement. 
* The constructor takes ownership of its argument. It may not be operated on, except through the Stream. It will be closed when destroyed.
* The constructor returns a null pointer if and only if the argument itself is NULL (this allows direct use of IE. fopen, from within the constructor). If the FILE* is already owned by another InputStream or OutputStream, the behavior is undefined. 
* The FILE* must be suitable for the mode. In particular, it shall be opened in read mode for InputStream and write/append mode for OutputStream. It is C implementation-depedant if it is necessary to open the file in binary mode. If the mode is not suitable for the stream being constructed, the behavior is undefined. 
*/
IO_API const StreamCalls* IO_file();

/**
* Returns an IO Stream Call set with a const void* buff,size_t size constructor (for input-streams), and a void** ptr,size_t* size_out,size_t init_size constructor (for output-streams)
* Input Streams read through the buffer passed in the constructor. 
* Output Streams write to the buffer pointed to by *ptr, until the init_size bytes are written, in which case the buffer is reallocated by a call to realloc with an unspecified size, and the new buffer is stored in *ptr. 
*  The number of bytes written is stored in *size_out (but this does not necessarily equal the size of the allocated storage).
* ptr may point to a null pointer (but may not itself be null) If so, the buffer is initially created by a call to malloc with an unspecified size when the first write occurs.
* The buffer at *ptr MUST NOT be freed in the lifetime of a constructed output Stream, but must be freed by a call to free after it is no longer used. The buffer at *ptr may be read or written to, up to size_out, except concurrently to a write operation.
* Expects:
*   For InputStream Constructor: buff shall point to a valid object, and at least size bytes shall be write-reachable through that object. Write-Reachability is described in OutputStream_write(OutputStream* stream,const void* data,size_t sz). For reachability purposes for calls to the constructed stream's read method, objects that start at buff are not reachable through the output buffer. 
*   For OutputStream Constructor: Neither ptr nor size_out shall be a Null pointer. Unless *ptr is null, *ptr shall have been returned from a call to malloc, realloc, or calloc, where the allocation size shall be at least size_out, and MUST NOT have been freed or subsequently passed to realloc. size_out shall not be EOF unless *ptr is null (but may be 0). For reachability purposes for calls to the constructed stream's write method, objects which start at *ptr are never reachable through the input buffer (despite the value of *ptr). 
*/
IO_API const StreamCalls* IO_buffer();

/**
* Constructs a new input stream. This method allocates an InputStream in an unspecified way, 
* binding it to the passed calls, then invokes calls->construct(IO_type_input,args), where args is a variable of type va_arg,
*  initialized as though by the call va_start(args,calls) (and subsequently deinitialized by a call to va_end after the call to construct).
* The return value of construct is bound to the InputStream. The resulting stream is then returned. 
* If the return value of construct is NULL, then NULL is returned (the allocated stream is deallocated)
* Expects: 
*  The StreamCalls type must have a valid constructor for input-streams that takes parameters of types convertible from each argument in order\
*   Where an argument is converted to a parameter as described in va_arg. 
*/
IO_API InputStream* InputStream_new(const StreamCalls* calls,...);

/**
* Constructs a new output stream. This method allocates an OutputStream in an unspecified way, 
* binding it to the passed calls, then invokes calls->construct(IO_type_input,args), where args is a variable of type va_arg,
*  initialized as though by the call va_start(args,calls) (and subsequently deinitialized by a call to va_end after the call to construct).
* The return value of construct is bound to the InputStream. The resulting stream is then returned. 
* If the return value of construct is NULL, then NULL is returned (the allocated stream is deallocated)
* Expects: 
*  The StreamCalls type must have a valid constructor for input-streams that takes parameters of types convertible from each argument in order\
*   Where an argument is converted to a parameter as described in va_arg. 
*/
IO_API OutputStream* OutputStream_new(const StreamCalls* calls,...);


/**
* Reads up to size bytes from stream into the buffer at out. Returns the number of bytes read, or EOF. 
* If EOF is returned, then the buffer at out was not affected by this call.
* If size is zero, whether or not the stream is read is unspecified. If EOF is reached, either 0 or EOF may be returned. 
* If data is not available when the call is made, the call may block until sufficient data is available, or return the number of read bytes.
* If no bytes were read, 0 is returned. 
* If a stream error occurs, either an unspecified number of bytes less than size or EOF is returned, and the stream reports an error. errno may be set. 
* Expects: 
* size MUST NOT be EOF. 
* out must point to a valid object, or data obtained from a call to malloc, realloc, or calloc, and at least size bytes are read-reachable through that object:
*  If out was returned from a call to malloc, realloc, or calloc, with an allocation size of n, then exactly n bytes are read-reachable through out.
*  If out points to an object, which is not the element of an array nor the first member of a structure, and is of a non-union type T, then sizeof(T) bytes are read-reachable through out
*  If out points to the nth element of an array of length l, then sizeof(T)*(l-n) bytes are read-reachable through out
*  If out points to a union, the number of bytes read-reachable through it is calculated as though out pointed to its active member.
*  If out points to the first member of a structure, then the number of bytes read-reachable through it is calculated as though it pointed to the structure object, unless the structure has any const or volatile members.
*  
* The object pointed to by out shall not be const or volatile, or have any const or volatile subobjects. 
* 
* If out points to an object which was created by part of a program written in C++, then the type of the object pointed to by out MUST Satisfy the *BytesReadable* concept, defined by LCLib-C++. The constraints on size are detailed in the method lightningcreations::lclib::InputStream::read(void*,size_t) defined by LCLib-C++. 
*/
IO_API size_t InputStream_read(InputStream* stream,void* out,size_t size);

/**
* Reads a single byte from the stream, and returns it as an int32_t (in [0,255]). If EOF was reached, returns -1. 
* If an error occurs, -1 is returned, and the stream reports an error (errno may be set). 
* If enough bytes are not available (IE. one), the call may block until data is returned, or return EOF and report an error.
*/
IO_API int32_t InputStream_readByte(InputStream* stream);

/**
* Reads up to size bytes from stream into the buffer at out. Returns the number of bytes written. 
* If size is zero, whether or not the stream is written to is unspecified.
* The write may block, or buffer the write for an unspecified period of time, until flush is called. 
* If a stream error occurs, either an unspecified number of bytes less than size or EOF is returned, and the stream reports an error. errno may be set. 
* Expects: 
* size MUST NOT be EOF. 
* out must point to a valid object, or data obtained from a call to malloc, realloc, or calloc, and at least size bytes are write-reachable through that object:
*  If buff was returned from a call to malloc, realloc, or calloc, with an allocation size of n, then exactly n bytes are write-reachable through buff. 
*  If buff points to an object, which is not the element of an array nor the first member of a structure, and is of a non-union type T, then sizeof(T) bytes are write-reachable through buff
*  If buff points to the nth element of an array of length l, then sizeof(T)*(l-n) bytes are write-reachable through buff
*  If buff points to a union, the number of bytes write-reachable through it is calculated as though buff pointed to its active member.
*  If buff points to the first member of a structure, then the number of bytes write-reachable through it is calculated as though it pointed to the structure object, unless the structure has any volatile members.
*  
* The object pointed to by buff shall not be volatile, or have any volatile subobjects. 
* 
* If buff points to an object which was created by part of a program written in C++, then the type of the object pointed to by buff MUST Satisfy the *BytesWritable* concept, defined by LCLib-C++. The constraints on size are detailed in the method lightningcreations::lclib::OutputStream::write(void*,size_t) defined by LCLib-C++. 
*/
IO_API size_t OutputStream_write(OutputStream* stream,const void* buff,size_t size);

/**
* Writes a single byte to stream. 
* If an error occurs, the stream reports an error (errno may be set). It is unspecified if the byte is written in this case. 
*/
IO_API void OutputStream_writeByte(OutputStream* stream,uint8_t byte);

/**
* Checks if the stream has an error.
*/
IO_API stream_status InputStream_checkError(const InputStream* stream);

/**
* Checks if the stream has an error.
*/
IO_API stream_status OutputStream_checkError(const OutputStream* stream);

/**
* Clears the any error on the output Stream.
* After this call, before any failing call to InputStream_read, InputStream_checkError will not return IO_status_ERROR.
* It is implementation-specific if IO_status_EOF will also be cleared
*/
IO_API void InputStream_clearError(InputStream* stream);
/**
* Clears the any error on the output Stream.
* After this call, before any failing call to OutputStream_write, OutputStream_checkError will not return IO_status_ERROR.
* It is implementation-specific if IO_status_EOF will also be cleared.
*/
IO_API void OutputStream_clearError(OutputStream* stream);

/**
* Flushes the buffer associated with stream, if any. After this call, any writes made to stream prior to this call will be guaranteed to be reflected on the underlying stream. 
*/
IO_API void OutputStream_flush(OutputStream* stream);

/**
* Destroys the stream, releasing the resources it uses.
*/
IO_API void InputStream_destroy(InputStream* stream);
/**
* Destroys the stream, releasing the resources it uses.
* On streams that use a buffer, this call flushes that buffer. 
*/
IO_API void OutputStream_destroy(OutputStream* stream);

/**
* Opens a File InputStream as though by InputStream_new(IO_file(),file).
* file shall be suitable for reading, as defined by IO_file(). Ownership of file is taken by this call as stated in IO_file().
*/
IO_API InputStream* InputStream_from(FILE* file);
/**
* Opens a File InputStream as though by InputStream_new(IO_file(),fopen(name,"rb")).
*/
IO_API InputStream* InputStream_openFilename(const char* name);

/**
* Opens a File OutputStream as though by OutputStream_new(IO_file(),file).
* file shall be suitable for writing, as defined by IO_file(). Ownership of file is taken by this call as stated in IO_file().
*/
IO_API OutputStream* OutputStream_from(FILE* file);

/**
* Opens a File OutputStream as though by OutputStream_new(IO_file(),fopen(name,"wb")).
*/
IO_API OutputStream* OutputStream_openFilename(const char* name);
/**
* Opens a File OutputStream as though by OutputStream_new(IO_file(),fopen(name,"ab")).
*/
IO_API OutputStream* OutputStream_appendFilename(const char* name);

#ifdef __cplusplus
};
#endif

#endif
