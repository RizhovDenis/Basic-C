static void str_overwrite_stdout() {
    printf("\r%s", "> ");
    fflush(stdout);
}

static void str_trim_lf (char* arr, int length) {
  int i;
  for (i = 0; i < length; i++) { // trim \n
    if (arr[i] == '\n') {
      arr[i] = '\0';
      break;
    }
  }
}

static void exit_from_chat() {
    flag = 1;
}

/* Remove clients to queue */
static void queue_remove(int uid){
	pthread_mutex_lock(&clients_mutex);

	for(int i = 0; i < MAX_CLIENTS; ++i){
		if(!clients[i]){
            break;
        }
		if(clients[i]->uid == uid){
			clients[i] = NULL;
			break;
		}
	}
	pthread_mutex_unlock(&clients_mutex);
}


/* Send message to all clients except sender */
static void send_message_tcp(char *message, int uid){
	pthread_mutex_lock(&clients_mutex);

	for(int i = 0; i < MAX_CLIENTS; ++i){
		if(!clients[i]){
            break;
        }
		if(clients[i]->uid != uid){
		    if(write(clients[i]->sockfd, message, strlen(message)) < 0){
				perror("ERROR: write to descriptor failed");
				break;
			}
		}
	}

	pthread_mutex_unlock(&clients_mutex);
}


int Socket(int domain, int type, int protocol){ // checker error
    int res = socket(domain, type, protocol);
    if (res == -1){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    return res;
}

void Bind(int sockfd, const struct sockaddr * addr, socklen_t addrlen){  // bind socket with address
    int res = bind(sockfd, addr, addrlen);
    if (res == -1){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
}

void Listen(int sockfd, int backlog){ // backlog = length queue of pending connections for sockfd
    int res = listen(sockfd, backlog);
    if (res == -1){
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
}

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen){
    int res = accept(sockfd, addr, addrlen);
    if (res == -1){
        perror("accept failed");
        exit(EXIT_FAILURE);
    }
    return res;
}

void Connect(int sockfd, const struct sockaddr * addr, socklen_t addrlen){
    int res = connect(sockfd, addr, addrlen);
    if (res == -1){
        perror("connect failed");
        exit(EXIT_FAILURE);
    }
}


void print_client_addr(struct sockaddr_in addr){
    printf("%d.%d.%d.%d",
        addr.sin_addr.s_addr & 0xff,
        (addr.sin_addr.s_addr & 0xff00) >> 8,
        (addr.sin_addr.s_addr & 0xff0000) >> 16,
        (addr.sin_addr.s_addr & 0xff000000) >> 24);
}


/* Add clients to queue */
void queue_add(client_t *cl){
	pthread_mutex_lock(&clients_mutex);

	for(int i = 0; i < MAX_CLIENTS; ++i){
		if(!clients[i]){
			clients[i] = cl;
			break;
		}
	}

	pthread_mutex_unlock(&clients_mutex);
}

/* Handle all communication with the client */
void *handle_client(void *arg){
	char buff_out[BUFFER_SIZE_MESSAGE];
	char name[32];
	int leave_flag = 0;

	cli_count++;
	client_t *cli = (client_t *)arg;

	// Name
	if(recv(cli->sockfd, name, 32, 0) == -1){
		printf("Incorrect chatname.\n");
		leave_flag = 1;
	} 
    else{
		strcpy(cli->name, name);
		sprintf(buff_out, "%s has joined\n", cli->name);
		printf("%s", buff_out);
		send_message_tcp(buff_out, cli->uid);
	}

	bzero(buff_out, BUFFER_SIZE_MESSAGE);

	while(1){
		if (leave_flag) {
			break;
		}
		int receive = recv(cli->sockfd, buff_out, BUFFER_SIZE_MESSAGE, 0);
		if (receive == -1){
			printf("ERROR: -1\n");
			leave_flag = 1;
		} 
        if (receive == 0 || strcmp(buff_out, "exit") == 0){
			sprintf(buff_out, "%s has left\n", cli->name);
			printf("%s", buff_out);
			send_message_tcp(buff_out, cli->uid);
			leave_flag = 1;
		} 
		if(strlen(buff_out) > 0){
			send_message_tcp(buff_out, cli->uid);
			str_trim_lf(buff_out, strlen(buff_out));
			printf("%s -> %s\n", buff_out, cli->name);
		}

		bzero(buff_out, BUFFER_SIZE_MESSAGE);
	}

    /* Delete client from queue and yield thread */
    close(cli->sockfd);
    queue_remove(cli->uid);
    free(cli);
    cli_count--;
    pthread_detach(pthread_self()); 

	return NULL;
}

void send_msg_handler() {
    char message[BUFFER_SIZE_MESSAGE];
	char buffer[BUFFER_SIZE_MESSAGE + 32];
    
    while(1) {
  	    str_overwrite_stdout();
        fgets(message, BUFFER_SIZE_MESSAGE, stdin);
        str_trim_lf(message, BUFFER_SIZE_MESSAGE);

        if (strcmp(message, "exit") == 0) {
			break;
        }
		sprintf(buffer, "%s: %s\n", chat_name, message);
		send(sockfd, buffer, strlen(buffer), 0);

		bzero(message, BUFFER_SIZE_MESSAGE);
        bzero(buffer, BUFFER_SIZE_MESSAGE + 32);
  }
  exit_from_chat();
}

void recv_msg_handler() {
	char message[BUFFER_SIZE_MESSAGE];
  	while (1) {
		int receive = recv(sockfd, message, BUFFER_SIZE_MESSAGE, 0);
    	if (receive == 0) {
			break;
    	} 
		else if (receive > 0) {
			printf("%s", message);
      		str_overwrite_stdout();
    }
		memset(message, 0, sizeof(message)); // fill message zeros
  }
}