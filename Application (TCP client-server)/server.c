#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> // socket function
#include <sys/types.h> // AF_INET, SOCK_STREAM
#include <netinet/in.h> // sockaddr_in
#include <arpa/inet.h> // for htons, htonl
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

#include "header_TCP.h"
#include "TCP_functions.c"


// example ./server start --host=127.0.0.1 --port=9090
// check Addresses: netstat -atnp


int main(int argc, char *argv[]){
    if (strcmp("start", argv[1])!=0){
        printf("action with server was not. Write action.");
        exit(EXIT_FAILURE);
    }

    char host_flag[] = "--host", port_flag[] = "--port";
    char *last, *host;
    int port;

    if (strcmp(host_flag, strtok_r(argv[2], "=", &last))==0){
        host = last;
    }
    if (strcmp(port_flag, strtok_r(argv[3], "=", &last))==0){
        port = atoi(last);
    }

    struct sockaddr_in serv_addr;
    struct sockaddr_in cli_addr;
    pthread_t tid;
    int option = 1;
    int fd = 0;

    int server = Socket(AF_INET, SOCK_STREAM, 0); // int domain, int type, int protocol
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(host); 
    serv_addr.sin_port = htons(port);

    Bind(server, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    Listen(server, 10);   
    
    
    while(1){
        socklen_t adrlen = sizeof(cli_addr);
        fd = Accept(server, (struct sockaddr *)&cli_addr, &adrlen); // file descriptor for communicate server with client
        
        // Check for MAX-CLIENTS
        if((cli_count + 1)==MAX_CLIENTS){
            printf("Maximum clients connected.\n");
            print_client_addr(cli_addr);
            printf(":%d\n", cli_addr.sin_port);
            close(fd);
            continue;
        }

        client_t * cli = (client_t *)malloc(sizeof(client_t));
        cli->clientAddr = cli_addr;
        cli->sockfd = fd;
        cli->uid = uid++;
        
        // Add client to queue
        queue_add(cli);
        pthread_create(&tid, NULL, &handle_client, (void*)cli);
        
        // Reduce CPU usage
        sleep(1);
    }

    return EXIT_FAILURE;
}