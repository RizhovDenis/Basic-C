#define CAPACITY 100000 // Size of the Hash Table
 
typedef struct Element{
    char *key;
    char *data;
}Element;

typedef struct LinkedList{ // for collision
    Element *item;
    int number_collision_by_key;
    struct LinkedList *next;
    struct LinkedList *prev;
}LinkedList;

typedef struct HashTable{
    int size;
    int capacity;
    int global_number_collision;
    Element **items;
    LinkedList **collision_array;
}HashTable;

HashTable *create_table(int capacity);
void delete_table(HashTable *table);
int hash_table_insert(HashTable *table, char *key, char* data);
void hash_table_delete(HashTable *table, char *key); // for deleting element
void print_element(HashTable *table, char *key);
int number_collision(HashTable *table);
int collision_by_key(HashTable *table, char *key);
void generator_items(HashTable *table); // for generation items
