#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct node {
    char *value;
    struct node* next;
} Queue;

Queue *back; // for take last element
Queue *forward; // for take first element

void queue_push(Queue *head, char *data){
    Queue *pointer = (Queue*)malloc(sizeof(Queue));
    if (pointer == NULL){
        printf("\n queue is empty\n");
        return;
    }
    pointer -> value = data;
    if (forward == NULL){
        forward = pointer;
        back = pointer;
        forward -> next = NULL;
        back -> next = NULL;
    }
    else{
        back -> next = pointer;
        back = pointer;
        back -> next = NULL;
    }
}

void queue_pop(Queue *head){
    if (forward == NULL){
        printf("queue is empty\n");
        return;
    }
    head = forward;
    forward = forward -> next;
    free(head);
}

int queue_len(Queue *head){
    int count = 0;
    Queue *current = head;
    while (current != NULL){
        ++count;
        current = current -> next;
    }
    return count;
}

void deleteLine(FILE *input, FILE *temp, const int line){ // for read_from_database
    char buffer[7];
    int count = 1;
    while ((fgets(buffer, 7, input)) != NULL){
        if (line != count)
            fputs(buffer, temp);
        count++;
    }
}    

char* read_from_database(void){
    char *data = malloc(sizeof(char)*7);
    char path[] = "../Database/database.txt";
    FILE *temp=fopen("../Database/delete.tmp","w");
    FILE *input=fopen(path,"r"); // open file
    if (input == NULL || temp == NULL){
        printf("File can't be opened");
        return 0;
    }
    fgets(data, 7, input);
    deleteLine(input, temp, 1); // input, temp file, number string which we want to delete
    remove(path);
    rename("../Database/delete.tmp", path);
    fclose(temp);
    fclose(input);
    return data;
}


int main(void){
    Queue *head = NULL;
    for (int i=0;i<1000000;i++)
        if (rand()%3==0){ // possible 0,1,2 
            queue_pop(head);
            printf("Last element was deleted\n");
        }
        else{
            queue_push(head, read_from_database());
            printf("added element : %s\n", back->value);
        }
    printf("queue length - %d\n", queue_len(forward));
    return 0;
}