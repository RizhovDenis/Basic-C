#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <json-c/json.h>
#include <sys/stat.h> // for creating folders
#include <dirent.h> // for counting files in folder

#include "/home/denis/Documents/Software/C Training Projects/mini_app/Hash Table/struct.h" // use hash for password 
#include "/home/denis/Documents/Software/C Training Projects/mini_app/Hash Table/function.c" 

#include "header_app.h"
#include "functions.c"

// example: ./create-user --login=advantage --password=hello --name=denis --lastname=ryzhov --age=25
// example: ./create-user --login=nowhere --password=first --name=sergey --lastname=volkov --age=34
//compile: clang create-user.c -ljson-c -o create-user

int main(int argc, char *argv[]){
    char login_flag[] = "--login", password_flag[] = "--password", name_flag[] = "--name", lastname_flag[] = "--lastname", age_flag[] = "--age";
    char *last, *login, *password, *name, *lastname, *age;
    
    if (strcmp(login_flag, strtok_r(argv[1], "=", &last))==0){
        login = last;
    }
    if (strcmp(password_flag, strtok_r(argv[2], "=", &last))==0){
        password = last;
    }
    if (strcmp(name_flag, strtok_r(argv[3], "=", &last))==0){
        name = last;
    }
    if (strcmp(lastname_flag, strtok_r(argv[4], "=", &last))==0){
        lastname = last;
    }
    if (strcmp(age_flag, strtok_r(argv[5], "=", &last))==0){
        age = last;
    }

    HashTable *ht = create_table(CAPACITY);
    HashTable *ht_filename = create_table(CAPACITY);
    int hash_password = hash_table_insert(ht, password, login);
    int hash_filename = hash_table_insert(ht_filename, login, login);
    
    create_user(login, hash_password, name, lastname, age, hash_filename, STORAGE);

    return 0;
}