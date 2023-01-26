#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> // socket function
#include <sys/types.h> // AF_INET, SOCK_STREAM
#include <netinet/in.h> // sockaddr_in
#include <arpa/inet.h> // for htons
#include <unistd.h>
#include <json-c/json.h>
#include <sys/stat.h> // for creating folders
#include <dirent.h> // for counting files in folder
#include <pthread.h>
#include <errno.h>

#include "/home/denis/Documents/Software/C Training Projects/mini_app/Hash Table/struct.h" // use hash for password 
#include "/home/denis/Documents/Software/C Training Projects/mini_app/Hash Table/function.c" 
#include "header_TCP.h"
#include "TCP_functions.c"
#include "header_app.h"
#include "functions.c"

// compile: clang client.c -ljson-c -o client
// connect: ./client --host=127.0.0.1 --port=9090 --login=advantage --password=hello
// connect: ./client --host=127.0.0.1 --port=9090 --login=nowhere --password=first



int main(int argc, char *argv[]){
    char login_flag[] = "--login", password_flag[] = "--password", host_flag[] = "--host", port_flag[] = "--port";
    char *last, *login, *password, *host;
    int port;

    if (strcmp(host_flag, strtok_r(argv[1], "=", &last))==0){
        host = last;
    }
    if (strcmp(port_flag, strtok_r(argv[2], "=", &last))==0){
        port = atoi(last);
    }
    if (strcmp(login_flag, strtok_r(argv[3], "=", &last))==0){
        login = last;
    }
    if (strcmp(password_flag, strtok_r(argv[4], "=", &last))==0){
        password = last;
    }

    HashTable *ht = create_table(CAPACITY);
    HashTable *ht_filename = create_table(CAPACITY);
    int hash_password = hash_table_insert(ht, password, login);
    int hash_filename = hash_table_insert(ht_filename, login, login);

    read_user(login, hash_filename, hash_password, STORAGE, READ_USER_STATUS);
    printf("Connected to server succesfully as %s\n", login);
    strcpy(chat_name, login);


    struct sockaddr_in server_addr;

    sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(host);
    server_addr.sin_port = htons(port);

    Connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    
    send(sockfd, chat_name, 32, 0);

    pthread_t send_msg_thread;
    if(pthread_create(&send_msg_thread, NULL, (void *) send_msg_handler, NULL) != 0){
		printf("ERROR: pthread\n");
    return EXIT_FAILURE;
	}

	pthread_t recv_msg_thread;
    if(pthread_create(&recv_msg_thread, NULL, (void *) recv_msg_handler, NULL) != 0){
		printf("ERROR: pthread\n");
		return EXIT_FAILURE;
	}

    while (1){
		if(flag){
			printf("Exit succesfuly\n");
			break;
        }
    }

    close(sockfd);

    return EXIT_SUCCESS;
}