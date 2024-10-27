#ifndef SERVER_FUNCTIONS_H
#define SERVER_FUNCTIONS_H

#include <sys/socket.h>
#include <netinet/in.h>


#define SA struct sockaddr //SA de Socket Address
#define SA_IN struct sockaddr_in //SA_IN de Socket Adress IN
#define LISTENQ 50 //Cola de 50

int srv_init(int port);
int srv_accept_client(int server_fd);

#endif

