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

// example: ./read-mail --login=advantage --password=hello --message-id=3
// compile: clang read-mail.c -ljson-c -o read-mail

int main(int argc, char *argv[]){
    char login_flag[] = "--login", password_flag[] = "--password", message_id_flag[] = "--message-id";
    char *last, *login, *password, *message_id;

    if (strcmp(login_flag, strtok_r(argv[1], "=", &last))==0){
        login = last;
    }
    if (strcmp(password_flag, strtok_r(argv[2], "=", &last))==0){
        password = last;
    }
        if (strcmp(message_id_flag, strtok_r(argv[3], "=", &last))==0){
        message_id = last;
    }

    HashTable *ht = create_table(CAPACITY);
    HashTable *ht_filename = create_table(CAPACITY);
    int hash_password = hash_table_insert(ht, password, password);
    int hash_filename = hash_table_insert(ht_filename, login, login);
    
    read_mail(login, hash_filename, hash_password, message_id, STORAGE, ACTION_STATUS, READ_MESSAGE_STATUS);
    return 0;
}