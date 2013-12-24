#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#include "network.h"

void create_daemon(const int port){

    // define and configure local and peer sockets 

    int lsockfd, psockfd;
    socklen_t sockaddr_size = sizeof(struct sockaddr);

    struct sockaddr_in server;
    struct sockaddr_in peer;

    if((lsockfd = socket( AF_INET, SOCK_STREAM, 0)) < 0){
        printf("Error: Unable to create socket\n");
        exit(1);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

    if(bind(lsockfd, (struct sockaddr *)&server, sockaddr_size) < 0){
        printf("Error: Unable to bind to port %i\n", port);
        exit(1);
    }

    if(listen(lsockfd, MAX_CONNECTIONS ) < 0){
        printf("Error: Unable to listen on port %i\n", port);
        exit(1);
    }

    psockfd = accept(lsockfd, (struct sockaddr *)&peer, &sockaddr_size);
    dup2(psockfd, 1);
    // printf("Connected\n");
    close(psockfd);
}
