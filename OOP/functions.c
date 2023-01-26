static char *concat(char *label, char *name){
    char *result = malloc(strlen(name) + strlen(label) + 1);
    strcpy(result, name);
    strcat(result, label);
    return result;
}

static void write(const char *encoded_text, char *file_name){
    FILE *out=fopen(file_name, "w");
	fputs(encoded_text, out);
    fclose(out);
}

static account *create_account(char *first_name, char *last_name, int age, char *address){
    account *item = malloc(sizeof(account));
    item->first_name = first_name;
    item->last_name = last_name;
    item->age = age;
    item->address = address;
    return item;
}

list_of_account *init_list(int number_accounts){
    list_of_account *list = malloc(sizeof(list_of_account));
    list->number_accounts = number_accounts;
    list->items = malloc(sizeof(account)*number_accounts);
    for (int i = 0; i < number_accounts; i++){
        list->items[i] = NULL;
    }
    return list;
}

list_of_account *generator_accounts(list_of_account *list){
    char *list_name[10] = {"Denis", "Jhon", "Vadim", "Svetlana", "Kamil", "Boris", "Anton", "Gleb", "Egor", "Anna"};
    char *list_family[10] = {"Ivanov", "Petrov", "Sidorov", "Ryzhov", "Kustikov", "Vlasov", "Pushkin", "Egorov", "Antonov", "Buchkalin"};
    char *list_address[10] = {"Moscow", "London", "Kiev", "Berlin", "Frankfurt", "Paris", "Vladimir", "Krasnodar", "Karabanovo", "Pushkino"};
    for (int i = 0; i < list->number_accounts; i++){
        char *first_name = list_name[rand() % 10];
        char *last_name = list_family[rand() % 10];
        int age = rand() % 50 + 18;
        char *address = list_address[rand() % 10];
        account *item = create_account(first_name, last_name, age, address);
        list->items[i] = item;
    }
    return list;
}

cycle **init_cycle(json *json_n, yaml *yaml_n, xml *xml_n){
    cycle **formats = (cycle**)malloc(3*sizeof(cycle*));
    formats[0] = malloc(sizeof(json));
    formats[1] = malloc(sizeof(yaml));
    formats[2] = malloc(sizeof(xml));
    formats[0]->action.dump = json_n->action.dump;
    formats[0]->action.decode = json_n->action.decode;
    formats[0]->action.encode = json_n->action.encode;
    formats[0]->label = json_n->label;

    formats[1]->action.dump = yaml_n->action.dump;
    formats[1]->action.decode = yaml_n->action.decode;
    formats[1]->action.encode = yaml_n->action.encode;
    formats[1]->label = yaml_n->label;

    formats[2]->action.dump = xml_n->action.dump;
    formats[2]->action.decode = xml_n->action.decode;
    formats[2]->action.encode = xml_n->action.encode;
    formats[2]->label = xml_n->label;

    return formats;
};

 // Polymorphism
json *init_json(){
    json *format_type = malloc(sizeof(json));
    (*format_type).label = ".json";
    (*format_type).action.dump = write_to_json;
    (*format_type).action.decode = json_parser;
    (*format_type).action.encode = encode_json;
    return format_type;
}

xml *init_xml(){
    xml *format_type = malloc(sizeof(xml));
    (*format_type).label = ".xml";
    (*format_type).action.dump = write_to_xml;
    (*format_type).action.decode = xml_parser;
    (*format_type).action.encode = encode_xml;
    return format_type;
}

yaml *init_yaml(){
    yaml *format_type = malloc(sizeof(yaml));
    (*format_type).label = ".yaml";
    (*format_type).action.dump = write_to_yaml;
    (*format_type).action.decode = yaml_parser;
    (*format_type).action.encode = encode_yaml;
    return format_type;
}

void write_to_json(json *json, const char *encoded_json, char *name){
    char *file_name = concat(json->label, name);
    write(encoded_json, file_name);
}

void write_to_xml(xml *xml, const char *encoded_xml, char *name){
    char *file_name = concat(xml->label, name);
    write(encoded_xml, file_name);
}

void write_to_yaml(yaml *yaml, const char *encoded_yaml, char *name){
    char *file_name = concat(yaml->label, name);
    write(encoded_yaml, file_name);
}

// Decode

void json_parser(json *json, char *name){
	char buffer[BUFFER_SIZE+1];
    char *file_name = concat(json->label, name);
	FILE *input = fopen(file_name,"r");
	fread(buffer, BUFFER_SIZE+1, 1, input);
	fclose(input);
	
    json_format = json_tokener_parse(buffer);

    json_object_object_get_ex(json_format, "first_name", &first_name);
    json_object_object_get_ex(json_format, "last_name", &last_name);
    json_object_object_get_ex(json_format, "age", &age);
    json_object_object_get_ex(json_format, "address", &address);

    printf("First name: %s\n", json_object_get_string(first_name));
    printf("Last name: %s\n", json_object_get_string(last_name));
    printf("Age: %d\n", json_object_get_int(age));
    printf("Address: %s\n", json_object_get_string(address));
}

void xml_parser(xml *xml, char *name){
    char *file_name = concat(xml->label, name);
    char buffer[BUFFER_SIZE+1];
    char labels[4][15] = {"first_name", "last_name", "age", "address"};
    FILE *input;

    for (int i = 0; i < 4; i++){
        input = fopen(file_name,"r");
	    fread(buffer, BUFFER_SIZE+1, 1, input);
	    fclose(input);
        char first[15] = "<";
        char *start_label = strcat(first, labels[i]);
        char *tagStartBegin = strstr(buffer, start_label);
        char *tagStartEnd = strstr(tagStartBegin,">");
        char *value = tagStartEnd+1;
        // get the end tag
        char first_end[15] = "</";
        char second_end[15] = ">";
        char *end_label = strcat(strcat(first_end, labels[i]), second_end);
        char *tagEndBegin = strstr(tagStartEnd, end_label);
        *tagEndBegin = '\0';
        
        printf("%s: %s\n",labels[i], value);
    }
}

void yaml_parser(yaml *yaml, char *name){
    int i = 1;
    char *file_name = concat(yaml->label, name);
    FILE *file = fopen(file_name, "r");
    yaml_parser_initialize(&parser);
    yaml_parser_set_input_file(&parser, file);

    if (!yaml_parser_load(&parser, &document)) {
        goto done;
    }
    while(1) {
        node = yaml_document_get_node(&document, i);
        if(!node) {
          break;
        }    
        i++;      
        if (i > 6 && i % 2 == 1){
            printf("%s\n", node->data.scalar.value);
        } 
    }
    yaml_document_delete(&document);

    done:
    yaml_parser_delete(&parser);
    fclose(file);
}

// Encode

const char *encode_json(account *item){    
    int flag = JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY;

	json_format = json_object_new_object();
	json_object_object_add(json_format, "first_name", json_object_new_string(item->first_name));
	json_object_object_add(json_format, "last_name", json_object_new_string(item->last_name));
    json_object_object_add(json_format, "age", json_object_new_int(item->age));
    json_object_object_add(json_format, "address", json_object_new_string(item->address));
 
    const char *encoded_json = json_object_to_json_string_ext(json_format, flag);

    return encoded_json;
}

static char **concat_xml(account *item){
    char str_age[2];
    int position = 0;
    sprintf( str_age, "%d", item->age);
    char labels[4][15] = {"first_name", "last_name", "age", "address"}; 
    char *data[4] = {item->first_name, item->last_name, str_age, item->address};
    char *encoded_data[6] = {"<person>"};
    char **encoded_list = encoded_data;

    for (int i = 0; i < 4; i = position){
        char *element = labels[position];
        char first[15] = "\n    <";
        char *start_label = strcat(first, element);        
        char *tagStartBegin = strcat(start_label, ">");
        char *value = strcat(tagStartBegin, data[position]);
        char first_end[15] = "</";
        char second_end[15] = ">\0";
        char *end_label = strcat(first_end, labels[position]);
        char *string = strcat(strcat(value, end_label), second_end); 
        encoded_data[position+1] = malloc(sizeof(char)*strlen(string)+1);
        strcpy(encoded_data[position+1], string);
        position++;
        i = position;
    }
    return encoded_list;
}

const char *encode_xml(account *item){
    char **encoded_list = concat_xml(item);
    for (int i = 1; i < 5; i++){;
        printf("%s\n", encoded_list[i]);

    }
    return encoded_xml;
}


const char *encode_yaml(account *item){
    size_t written = 0;
    unsigned char buffer[BUFFER_SIZE+1];
    char str_age[2];
    sprintf( str_age, "%d", item->age);
    char labels[4][15] = {"first_name", "last_name", "age", "address"}; 
    char *data[4] = {item->first_name, item->last_name, str_age, item->address};
    yaml_emitter_initialize(&emitter);
    yaml_emitter_set_output_string(&emitter, buffer, BUFFER_SIZE, &written);

    yaml_stream_start_event_initialize(&event, YAML_UTF8_ENCODING);
    if (!yaml_emitter_emit(&emitter, &event)) goto error;
    yaml_document_start_event_initialize(&event, NULL, NULL, NULL, 0);
    if (!yaml_emitter_emit(&emitter, &event)) goto error;
    yaml_mapping_start_event_initialize(&event, NULL, (yaml_char_t *)YAML_MAP_TAG,
        1, YAML_ANY_MAPPING_STYLE);
    if (!yaml_emitter_emit(&emitter, &event)) goto error;
    yaml_scalar_event_initialize(&event, NULL, (yaml_char_t *)YAML_STR_TAG,
        (yaml_char_t *)"Person", strlen("Person"), 1, 0, YAML_PLAIN_SCALAR_STYLE);
    if (!yaml_emitter_emit(&emitter, &event)) goto error;
    yaml_sequence_start_event_initialize(&event, NULL, (yaml_char_t *)YAML_SEQ_TAG,
       1, YAML_ANY_SEQUENCE_STYLE);
    if (!yaml_emitter_emit(&emitter, &event)) goto error;
    yaml_mapping_start_event_initialize(&event, NULL, (yaml_char_t *)YAML_MAP_TAG,
    1, YAML_ANY_MAPPING_STYLE);
    if (!yaml_emitter_emit(&emitter, &event)) goto error;
    for (int i = 0; i < 4; i++){
        yaml_scalar_event_initialize(&event, NULL, (yaml_char_t *)YAML_STR_TAG,
            (yaml_char_t *)labels[i], strlen(labels[i]), 1, 0, YAML_PLAIN_SCALAR_STYLE);
        if (!yaml_emitter_emit(&emitter, &event)) goto error;
        yaml_scalar_event_initialize(&event, NULL, (yaml_char_t *)YAML_STR_TAG,
            (yaml_char_t *)data[i], strlen(data[i]), 1, 0, YAML_PLAIN_SCALAR_STYLE);
        if (!yaml_emitter_emit(&emitter, &event)) goto error;
    }
    yaml_mapping_end_event_initialize(&event);
    if (!yaml_emitter_emit(&emitter, &event)) goto error;
    yaml_sequence_end_event_initialize(&event);
    if (!yaml_emitter_emit(&emitter, &event)) goto error;
    yaml_mapping_end_event_initialize(&event);
    if (!yaml_emitter_emit(&emitter, &event)) goto error;
    yaml_document_end_event_initialize(&event, 0);
    if (!yaml_emitter_emit(&emitter, &event)) goto error;
    yaml_stream_end_event_initialize(&event);
    if (!yaml_emitter_emit(&emitter, &event)) goto error;
    unsigned char *encoded_yaml = emitter.output.string.buffer;
    yaml_emitter_delete(&emitter);

    return (const char*)encoded_yaml;

    error:
    fprintf(stderr, "Failed to emit event %d: %s\n", event.type, emitter.problem);
    yaml_emitter_delete(&emitter);
    return 0;
}