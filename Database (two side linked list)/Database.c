#include <stdlib.h> // for malloc
#include <stdio.h> 
#include <string.h>
#include <time.h>

typedef struct Node{ // create struct List with value and Index
    char *value;
    struct Node *next;
    struct Node *prev;
} List_item;

void print(List_item *node) {
    int elem = 0;
    while (node != NULL) {
        printf("Id - %d, value - %s\n", elem, node -> value);
        node = node -> next;
        ++elem;
  }
}

char *generator_item(){
    char letters[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
    char *word = malloc(sizeof(char)*7);
    int index;
    for(index = 0; index < 7; ++index) 
    {
        word[index] = letters[rand()%26];
    }
    word[index] = '\0';
    return word;
}
void database_insert(List_item **head, char *new_value){
    List_item *new_node = (List_item*)malloc(sizeof(List_item)); // allocate memory for new node
    new_node -> value = new_value; // put new_value in value
    if (*head == NULL){  // add a node at the front 
        new_node -> next = *head; // make next node as head and prev as NULL
        new_node -> prev = NULL;
        (*head) = new_node; // move the head to point to the new node
    }
    else {           // add node back
        List_item *last = *head;
        new_node -> next = NULL; // the node gonna be last node
        while (last -> next != NULL){
            last = last -> next;
        }
        last -> next = new_node;
        new_node -> prev = last;
    }
}
void database_delete(List_item **head){
    List_item *current = *head;
    List_item *sequent;
    while (current != NULL){      
        sequent = current -> next;
        free(current);
        current = sequent;
    }
    (*head) = NULL;
    printf("Data base was deleted\n");
}

int length(List_item **head){
    int count = 0;
    List_item *current = *head;
    while (current != NULL){
        ++count;
        current = current -> next;
    }
    return count;
}

void write_to_file(List_item *node){
    FILE *out=fopen("database.txt","w"); // open file
    while (node != NULL){ // writing data to file
        strncat(node->value, "\n", 2);
	    fputs(node->value, out);
        node = node -> next;
    }
    fclose(out); // close file
    printf("File was written successful\n");
}

int main(void){
    List_item *head = NULL;
    clock_t time = clock();
    for (int cnt = 0; cnt < 1000000; cnt++){
        database_insert(&head, generator_item());  
    }
    time = clock() - time;
    print(head);
    printf("List filling speed - %f second\n", (double)time/CLOCKS_PER_SEC);
    printf("length of list - %i\n", length(&head));
    write_to_file(head);
    return 0;
}

