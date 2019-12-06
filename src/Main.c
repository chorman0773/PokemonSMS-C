#include <Stream.h>
#include <BinaryIO.h>
#include <NBT.h>
#include <ShadeNBT.h>
#include <ShadeConstants.h>
#include <stdio.h>


int main(){
    InputStream* in = InputStream_openFilename("example.ssv");
    BinaryIO* io = wrapInputStream(in,BINARY_BIG_ENDIAN);
    ShadeFile* file = Shade_Open(io);
    if(!file){
        fprintf(stderr,"Failed to Open Shade File\n");
        return -1;
    }
    BinaryIO_free(io);
    InputStream_destroy(in);
    OutputStream* out = OutputStream_openFilename("example1.ssv");
    if(!out){
        fprintf(stderr,"Could not open example.ssv\n");
        Shade_Free(file);
        return -1;
    }
    io = wrapOutputStream(out,BINARY_BIG_ENDIAN);
    Shade_Write(file,io);
    BinaryIO_free(io);
    OutputStream_destroy(out);
    Shade_Free(file);
}
