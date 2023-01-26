#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <json-c/json.h>
#include <sys/stat.h>
#include <dirent.h> // for counting files in folder

#include "/home/denis/Documents/Software/C Training Projects/mini_app/Hash Table/struct.h" // use hash for password 
#include "/home/denis/Documents/Software/C Training Projects/mini_app/Hash Table/function.c" 

#include "header_app.h"
#include "functions.c"

//example: ./send-message --from=nowhere --password=first --destination=advantage --message='Hey there!'
// compile: clang send-message.c -ljson-c -o send-message

int main(int argc, char *argv[]){
    char from_flag[] = "--from", password_flag[] = "--password", dest_flag[] = "--destination", message_flag[] = "--message";
    char *last, *from, *password, *destination, *message;

    if (strcmp(from_flag, strtok_r(argv[1], "=", &last))==0){
        from = last;
    }
    if (strcmp(password_flag, strtok_r(argv[2], "=", &last))==0){
        password = last;
    }
    if (strcmp(dest_flag, strtok_r(argv[3], "=", &last))==0){
        destination = last;
    }
    if (strcmp(message_flag, strtok_r(argv[4], "=", &last))==0){
        message = last;
    }

    HashTable *ht = create_table(CAPACITY);
    int hash_from = hash_table_insert(ht, from, from);
    int hash_password = hash_table_insert(ht, password, password);
    int hash_dest = hash_table_insert(ht, destination, destination);
    send_message(from, hash_from, hash_password, hash_dest, destination, message, STORAGE, ACTION_STATUS);
    return 0;
}