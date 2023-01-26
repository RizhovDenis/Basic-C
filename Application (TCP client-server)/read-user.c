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

// example: ./read-user --login=nowhere --password=first
// compile: clang read-user.c -ljson-c -o read-user

int main(int argc, char *argv[]){
    char login_flag[] = "--login", password_flag[] = "--password";
    char *last, *login, *password;

    if (strcmp(login_flag, strtok_r(argv[1], "=", &last))==0){
        login = last;
    }
    if (strcmp(password_flag, strtok_r(argv[2], "=", &last))==0){
        password = last;
    }

    HashTable *ht = create_table(CAPACITY);
    HashTable *ht_filename = create_table(CAPACITY);
    int hash_password = hash_table_insert(ht, password, login);
    int hash_filename = hash_table_insert(ht_filename, login, login);

    read_user(login, hash_filename, hash_password, STORAGE, READ_USER_STATUS);

    return 0;
}