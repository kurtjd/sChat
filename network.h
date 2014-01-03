/* All of the networking functions */

#ifndef NETWORK_H
#define NETWORK_H

#define MAX_CONNECTIONS 2
#define MAXMSGLEN 1024

int peer_listen(int sockfd, const char *port);
int peer_connect(int sockfd, const char *host, const char *port);
int peer_send(int sockfd, const char *message);
int peer_recv(int sockfd);
void peer_close(int sockfd);

#endif
