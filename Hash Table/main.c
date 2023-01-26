#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "function.c"

int main(void){
    HashTable *ht = create_table(CAPACITY);
    generator_items(ht);
    number_collision(ht);
    return 0;
}
