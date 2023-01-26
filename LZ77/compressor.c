#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "struct.h"
#include "functions.c"

// compile : clang primary.c -o primary -lm    (lm for math)
// launch : ./compressor --file=test.txt --out=test.compressed

int main (int argc, char *argv[]){
    char input_flag[] = "--file", out_flag[] = "--out";
    char *last, *filename_input, *filename_output;

    if (strcmp(input_flag, strtok_r(argv[1], "=", &last))==0){
        filename_input = last;
    }
    if (strcmp(out_flag, strtok_r(argv[2], "=", &last))==0){
        filename_output = last;
    }


    FILE *file = NULL;
    bitFILE *bitF = NULL;

    
    if ((file = fopen(filename_input, "rb")) == NULL){
        perror("Opening input file failed");
        exit(EXIT_FAILURE);
    }
    if ((bitF = bitIO_open(filename_output)) == NULL){
        perror("Opening output file failed");
        exit(EXIT_FAILURE);
    }
    encode(file, bitF);

    fclose(file);
    bitIO_close(bitF);

    return EXIT_SUCCESS;
}