/* All of the networking functions */

#ifndef NETWORK_H
#define NETWORK_H

#define MAX_CONNECTIONS 2

// This is the main socket variable we'll use to accept() and connect()

int main_sockfd;

int peer_listen(const char *port);
int peer_connect(const char *host, const char *port);
void peer_close(void);

#endif
