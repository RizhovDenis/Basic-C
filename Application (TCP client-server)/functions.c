static int count_lenth(int number){
    int size = 0;
    while (number != 0){
        number /= 10;
        ++size;
    }
    return size;
}

static char *int_to_string(int number){
    int size = count_lenth(number);
    char string[size];
    char *result = malloc(strlen(string)+1);

    sprintf(string, "%d", number);
    strcpy(result, string);
    
    return result;
}

static char *concat(char *label, char *string_name){
    char *result = malloc(strlen(string_name) + strlen(label) + 1);
    strcpy(result, string_name);
    strcat(result, label);
    return result;
}

static char *user_path(char *storage, char *folder_name){
    char *path = malloc(strlen(storage) + strlen("/") + 1);
    strcpy(path, storage);
    strcat(path, "/");
    char *result = malloc(strlen(path) + strlen(folder_name) + 1);
    strcpy(result, path);
    strcat(result, folder_name);
    return result;
}

static void create_folder(char *name){
    mkdir(name, S_IRWXU);
}

static void dump(char *path){             
    DIR *theFolder = opendir(path);
    struct dirent *next_file;
    char filepath[256];

    while ((next_file = readdir(theFolder)) != NULL){         // build the path for each file in the folder
        sprintf(filepath, "%s/%s", path, next_file->d_name);
        remove(filepath);
    }
    closedir(theFolder);
}

static const char *encode_json(char *login, int hash_password, char *name, char *lastname, char *age){    
    int flag = JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY;

	json_format = json_object_new_object();
	json_object_object_add(json_format, "login", json_object_new_string(login));
	json_object_object_add(json_format, "password", json_object_new_int(hash_password));
    json_object_object_add(json_format, "name", json_object_new_string(name));
    json_object_object_add(json_format, "lastname", json_object_new_string(lastname));
    json_object_object_add(json_format, "age", json_object_new_string(age));
 
    const char *encoded_json = json_object_to_json_string_ext(json_format, flag);

    return encoded_json;
}

static const char *message_to_json(char *from, char *message){
    int flag = JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY;

	json_format = json_object_new_object();
	json_object_object_add(json_format, "from", json_object_new_string(from));
	json_object_object_add(json_format, "message", json_object_new_string(message));
 
    const char *encoded_json = json_object_to_json_string_ext(json_format, flag);

    return encoded_json;
}

static int json_parser(char *path, int hash_password, char *status){
	char buffer[BUFFER_SIZE+1];
	FILE *input = fopen(path,"r");
	fread(buffer, BUFFER_SIZE+1, 1, input);
	fclose(input);
	char *password = int_to_string(hash_password);
    json_format = json_tokener_parse(buffer);

    json_object_object_get_ex(json_format, "password", &hash_password_system);
    if (strcmp(password, json_object_get_string(hash_password_system))!=0){
        printf("Incorect login or password\n");
        return 1;
    }

    json_object_object_get_ex(json_format, "name", &name);
    json_object_object_get_ex(json_format, "lastname", &lastname);
    json_object_object_get_ex(json_format, "age", &age);
    
    if (strcmp(status, "action_with_message") == 0){
        return 0;
    }

    printf("name=%s, lastname=%s, age=%s \n", json_object_get_string(name), json_object_get_string(lastname), json_object_get_string(age));
    
    return 0;
}

static const char *parse_messages(char *path, char *parse_status){
    char buffer[BUFFER_SIZE+1];
	FILE *input = fopen(path,"r");
	fread(buffer, BUFFER_SIZE+1, 1, input);
	fclose(input);

    json_format = json_tokener_parse(buffer);
    if (strcmp(parse_status, "read_mailbox")==0){
        json_object_object_get_ex(json_format, "from", &name);
        return json_object_get_string(name);
    }
    if (strcmp(parse_status, "read_mail")==0){
        json_object_object_get_ex(json_format, "message", &message);
        return json_object_get_string(message);
    }
    return 0;
}

static void write_message(const char *encoded_text, char *path){
    FILE *out=fopen(path, "w");
	fputs(encoded_text, out);
    fclose(out);
}

static int count_messages(char *path){
    int file_count = 0;
    DIR * dirp;
    struct dirent * entry;
    dirp = opendir(path); /* There should be error handling after this */
    while ((entry = readdir(dirp)) != NULL) {
        if (entry->d_type == DT_REG) { /* If the entry is a regular file */
            file_count++;
        }
    }
    closedir(dirp);
    return file_count;
}

void create_user(char *login, int hash_password, char *name, char *lastname, char *age, int hash_filename, char *storage){
    char *string_filename = int_to_string(hash_filename);
    char *folder_path = user_path(storage, string_filename);
    create_folder(folder_path);
    const char *encoded_json = encode_json(login, hash_password, name, lastname, age);
    char *filename = concat(LABEL, string_filename);
    char *final_path = user_path(folder_path, filename);
    write_message(encoded_json, final_path);
    printf("User %s was created\n", name);
}

int read_user(char *login, int hash_filename, int hash_password, char *storage, char *status){ // if 0 pass elif 1 break
    char *string_filename = int_to_string(hash_filename);
    int checker = 0;
    char *folder_path = user_path(storage, string_filename);
    char *final_path = user_path(folder_path, string_filename);
    char *filename = concat(".json", final_path);
    FILE *file;
    file = fopen(filename, "r");
    if (!file){
        printf("Incorect login or password\n");
        checker = 1;
        return checker;
    }
    else{
        checker = json_parser(filename, hash_password, status);
    }
    fclose(file);
    return checker;
}

static char *create_messages_path(int hash_receiver, char *storage){
    char *dest_string = int_to_string(hash_receiver);
    char *dest_path = user_path(storage, dest_string);
    char *messages_path = user_path(dest_path, MESSEGES_PATH);
    return messages_path;
}

void send_message(char *from, int hash_filename, int hash_password, int hash_dest, char *destination, char *message, char *storage, char *status){
    int checker = read_user(from, hash_filename, hash_password, storage, status);
    if (checker == 1){
        return;
    }
    char *messages_path = create_messages_path(hash_dest, storage);
    create_folder(messages_path);
    int number_messages = count_messages(messages_path);
    if (number_messages >= MESSAGES_LIMIT){
        dump(messages_path);
        number_messages = 0;
    }
    char *string_number_message = int_to_string(number_messages + 1);

    const char *encoded_message = message_to_json(from, message);
    char *filename = concat(LABEL, string_number_message);
    char *final_path = user_path(messages_path, filename);
    write_message(encoded_message, final_path);
    printf("Messages was sent succesful to %s\n", destination);
}

void read_mailbox(char *login, int hash_filename, int hash_password, char *storage, char *status, char *parse_status){
    int checker = read_user(login, hash_filename, hash_password, storage, status);
    if (checker == 1){
        return;
    }
    char *messages_path = create_messages_path(hash_filename, storage);
    int number_messages = count_messages(messages_path);
    printf("Incoming messages: %d\n", number_messages);
        for (int i = 0; i < number_messages; i++){
        char *string_number_message = int_to_string(i + 1);
        char *filename = concat(LABEL, string_number_message);
        char *final_path = user_path(messages_path, filename);
        const char *from = parse_messages(final_path, parse_status);
        printf("id=%d from=%s\n", i+1, from);
    }
}

void read_mail(char *login, int hash_filename, int hash_password, char *message_id, char *storage, char *status, char *parse_status){
    int checker = read_user(login, hash_filename, hash_password, storage, status);
    if (checker == 1){
        return;
    }
    char *messages_path = create_messages_path(hash_filename, storage);
    char *filename = concat(LABEL, message_id);
    char *final_path = user_path(messages_path, filename);
    const char *message = parse_messages(final_path, parse_status);
    printf("%s\n", message);
}