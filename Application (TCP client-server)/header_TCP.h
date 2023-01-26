#ifndef HEADER_TCP_H
#define HEADER_TCP_H
#define BUFFER_SIZE_MESSAGE 2048
#define MAX_CLIENTS 100
#define LENGHT_NAME 64

// for server
static _Atomic unsigned int cli_count = 0;
static int uid = 10;

typedef struct{
     struct sockaddr_in clientAddr;
     int sockfd;
     int uid;
     char name[LENGHT_NAME];
 }client_t;

client_t *clients[MAX_CLIENTS];

pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

// for client functions
int flag = 0;
int sockfd = 0;
char chat_name[LENGHT_NAME];

int Socket(int domain, int type, int protocol);
void Bind(int sockfd, const struct sockaddr * addr, socklen_t addrlen);
void Listen(int sockfd, int backlog);
int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
void Connect(int sockfd, const struct sockaddr * addr, socklen_t addrlen);


void print_client_addr(struct sockaddr_in addr);
void queue_add(client_t *cl);
void *handle_client(void *arg);
void send_msg_handler();
void recv_msg_handler();


#endif