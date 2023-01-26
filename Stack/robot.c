#include <stdlib.h>
#include <string.h>
#include <stdio.h>
//create 3 stacks
typedef struct node_food {
    char *value;
    struct node_food* next;
} section_food;

typedef struct node_appliances {
    char *value;
    struct node_appliances* next;
} section_appliances;

typedef struct node_gadgets {
    char *value;
    struct node_gadgets* next;
} section_gadgets;

section_food *top_food;
section_appliances *top_app;
section_gadgets *top_gadged;

void robot_put(char *data){
    section_food *pointer = (section_food*)malloc(sizeof(section_food));
    if (pointer == NULL){
        printf("\n section is empty\n");
        return;
    }
    pointer -> value = data;
    if (top_food == NULL){
        pointer -> next = NULL;
    }
    else{
        pointer -> next = top_food;
    }
    top_food = pointer;
}

void robot_pop(){
    if (top_food == NULL){
        printf("section is empty\n");
        return;
    }
    section_food *temp = top_food;
    top_food = top_food -> next;
    free(temp);
}

int robot_len(){
    int count = 0;
    section_food *current = top_food;
    while (current != NULL){
        ++count;
        current = current -> next;
    }
    return printf("lenght section food - %d\n", count);
}

int main(void){
    robot_put("Apples");
    robot_put("meal");
    robot_put("Orange");
    printf("forward element: %s\n", top_food->value);
    robot_pop();
    printf("back element: %s\n", top_food->value);
    robot_len();
}

// Возникла проблема. Как можно передать структуру в функцию? Хочу чтобы robot_put, robot_pop,
// robot_len получали на вход название структуры и выводили значения для данной структуры.
// Как это сделать, без копирования, уменьшив код?