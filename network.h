/* All of the networking functions */

#ifndef NETWORK_H
#define NETWORK_H

#define MAX_CONNECTIONS 2
#define MAXMSGLEN 1024

// This is the main socket variable we'll use to accept() and connect()

int main_sockfd;

int peer_listen(const char *port);
int peer_connect(const char *host, const char *port);
int peer_send(const char *message);
int peer_recv(void);
void peer_close(void);

#endif
