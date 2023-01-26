#ifndef STRUCT_H
#define STRUCT_H

#define LA_SIZE 15
#define SB_SIZE 4095
#define N 3
#define MAX_BIT_BUFFER 16
#define BIT_IO_BUFFER 4096

typedef struct{
    int offset, length;
    char next;
}token;

typedef struct{
    FILE *file;
    int bytepos; // actual byte's position in the buffer
    int bitpos;  // last bit's position in the byte
    unsigned char *buffer; // bits buffer
}bitFILE;

typedef struct{
    int length, offset;
    int parent;
    int left, right;
}node;

typedef struct{
    int offset, length;
}searcher; // for search best sequentence

void encode(FILE *file, bitFILE *out);
bitFILE *bitIO_open(const char *path);
int bitIO_close(bitFILE *bitF);


#endif