/* All of the networking functions */

#ifndef NETWORK_H
#define NETWORK_H

#define MAX_CONNECTIONS 2

int peer_listen(const char *port);
int peer_connect(const char *host, const char *port);

#endif
