static LinkedList *insert_item(LinkedList *list, Element *item){
    LinkedList *new_node = (LinkedList*)(malloc(sizeof(LinkedList)));
    new_node->item = item;
    new_node->number_collision_by_key = list->number_collision_by_key++;
    if (list == NULL){  // add a node at the front 
        new_node->next = list; // make next node as head and prev as NULL
        new_node->prev = NULL;
        list = new_node; // move the head to point to the new node
        return list;
    }
    LinkedList *last = list;
    new_node->next = NULL; // the node gonna be last node
    while (last->next != NULL){
        last = last->next;
    }
    last->next = new_node;
    new_node->prev = last;    
    return list;
}

static void collision(HashTable *table, int hash_key, Element *item){
    LinkedList *head = table->collision_array[hash_key];
    if (head == NULL){
        LinkedList *list = (LinkedList*)(malloc(sizeof(LinkedList)));
        head = list;
        head->number_collision_by_key = 1;
        head->item = item;
        table->collision_array[hash_key] = head;
        return;
    }
    table->collision_array[hash_key] = insert_item(head, item);
    return;
}

static LinkedList **create_collision_array(capacity){
    LinkedList **array = (LinkedList**)malloc(capacity*sizeof(LinkedList*)); 
    for (int i = 0; i < capacity; i++){
        array[i] = NULL;
    }
    return array;
}

static int hash_function(int capacity, char *str){
    int result = 0;
    char *temp = str;
    while (*temp){
        result+=(int)(*temp);
        temp++;
    }
    return result % capacity;
}

static char *hash_table_search(HashTable *table, char *key){
    int hash_key = hash_function(table->capacity, key);
    Element *item = table->items[hash_key];    
    LinkedList *head = table->collision_array[hash_key];
    while (item != NULL){
        if (strcmp(item->key, key) == 0){
            return item->data;
        }
        if (head == NULL){
            return 0;
        }
        item = head->item;
        head = head->next;
    }
    return 0;
}

static Element *create_element(char *new_key, char *new_data){
    Element *item = (Element*)(malloc(sizeof(Element)));
    item->key = malloc(sizeof(char)*strlen(new_key)+1);
    item->data = malloc(sizeof(char)*strlen(new_data)+1);
    strcpy(item->key, new_key);
    strcpy(item->data, new_data);
    return item;
}

static void delete_LinkedList(LinkedList *list){
    LinkedList *temp;
    while(list){
        temp = list;
        list = list->next;
        free(temp->item->data);
        free(temp->item->key);
        free(temp->item);
        free(temp);
    }
}

static void delete_collision_array(HashTable *table){
    LinkedList **array = table->collision_array;
    for (int i = 0; i < table->capacity; i++){
        delete_LinkedList(array[i]);
    }
    free(array);
}

static void delete_item(Element *item){
    free(item->key);
    free(item->data);
    free(item);
}

HashTable *create_table(int capacity){
    HashTable *table = (HashTable*)(malloc(sizeof(HashTable)));
    table->capacity = capacity;
    table->size = 0;
    table->global_number_collision = 0;
    table->items = malloc(sizeof(Element*)*table->capacity);
    for (int i = 0; i < table->capacity; i++){
        table->items[i] = NULL;
    }
    table->collision_array = create_collision_array(table->capacity);
    return table;
}

void delete_table(HashTable *table){
    for(int i = 0; i < table->capacity; i++){
        Element *item = table->items[i];
        if (item != NULL){
            delete_item(item);
        }
        delete_collision_array(table);
        free(table->items);
        free(table);
    }
}

int hash_table_insert(HashTable *table, char *key, char* data){
    Element *item = create_element(key, data); // create element
    int hash_key = hash_function(table->capacity, key);
    Element *current_item = table->items[hash_key];
    if (table->capacity == table->size){ // HashTable full
        printf("This element wasn't added. HashTable is full\n");
        delete_item(item);
        return -1;
    }
    if (current_item == NULL){ // Key doesn't exist
        table->items[hash_key] = item;
        table->size++;
        return hash_key;
    }
    table->global_number_collision++;

    collision(table, hash_key, item); // Key exist
    return hash_key;
}

void hash_table_delete(HashTable *table, char *key){
    int hash_key = hash_function(table->capacity, key);
    Element *item = table->items[hash_key];
    LinkedList *head = table->collision_array[hash_key];
    if (item == NULL){
        printf("This key doesn't exist\n");
        return;
    }
    if (head == NULL && strcmp(item->key, key) == 0){ // No collision chain
        table->items[hash_key] = NULL;
        delete_item(item);
        table->size--;
        return;
    }
    if (head != NULL){ // collision chain        
        if (strcmp(item->key, key) == 0){ // delete the item, and set the head of the list as the new item
            delete_item(item);
            LinkedList *node = head;
            head = head->next;
            node->next = NULL;
            table->items[hash_key] = create_element(node->item->key, node->item->data);
            delete_LinkedList(node);
            table->collision_array[hash_key] = head;
            table->size--;
            table->global_number_collision--;
            table->collision_array[hash_key]->number_collision_by_key--;
            return;
        }
        LinkedList *current = head;
        LinkedList *previous = NULL;
        table->size--;
        table->global_number_collision--;
        table->collision_array[hash_key]->number_collision_by_key--; 
        while(current){
            if (strcmp(current->item->key, key) == 0){ 
                if(previous == NULL){ // first element in list
                    delete_LinkedList(head); 
                    table->collision_array[hash_key] = NULL;
                    return;
                } 
                previous->next = current -> next;
                current->next = NULL;
                delete_LinkedList(current);
                table->collision_array[hash_key] = head;
                return;
            }
            current = current->next;
            previous = current;
        }
    }

}

void print_element(HashTable *table, char *key){
    char *value = hash_table_search(table, key);
    if (value == NULL){
        printf("%s doesn't exist\n", key);
        return;
    }
    printf("key - %s, value - %s\n", key, value);
}

int number_collision(HashTable *table){
    return printf("Number collision - %d\n", table->global_number_collision);
}

int collision_by_key(HashTable *table, char *key){
    int hash_key = hash_function(table->capacity, key);
    return printf("%s - key, collision - %d\n", key, table->collision_array[hash_key]->number_collision_by_key);
}

void generator_items(HashTable *table){
    char *letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
    size_t len_letters = strlen(letters);
    int index;
    int count_letter;
    for (int i = 0; i < table->capacity; i++){
        count_letter = rand()%20 + 1;
        char *key_gen = malloc(sizeof(char)*count_letter);
        char *data_gen = malloc(sizeof(char)*count_letter);
        for(index = 0; index < count_letter; ++index){
            key_gen[index] = letters[rand()%len_letters];
            data_gen[index] = letters[rand()%len_letters];
        }
        key_gen[index] = '\0';
        data_gen[index] = '\0';
        int hash_key = hash_table_insert(table, key_gen, data_gen);
    }
}
