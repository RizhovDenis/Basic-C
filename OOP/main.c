#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>
#include <yaml.h>


#include "header.h"
#include "functions.c"

// compile: clang main.c -ljson-c -lyaml -o main
// compile with debug: clang main.c -ljson-c -lyaml -g

int main(){
    char label[6];
    char name[10];
    json *json = init_json();
    xml *xml = init_xml();
    yaml *yaml = init_yaml();

    list_of_account *list = init_list(NUMBER_ACCOUNTS); // account generation
    generator_accounts(list);


    cycle **fmt = init_cycle(json, yaml, xml); 
    for (int i = 0; i < 3; i++){
        const char *encoded_data = fmt[i]->action.encode(list->items[2]);
        fmt[i]->action.dump(fmt[i], encoded_data, "data/test");
        fmt[i]->action.decode(fmt[i], "data/test");    
    }
    
    return 0;
}