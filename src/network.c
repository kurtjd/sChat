/******************************************************************************
 * network.c: Implementation of all network functionality.                    *
 * Copyright (C) 2014  Kurtis Dinelle & McConnell Wade                        *
 *                                                                            *
 * This file is a part of sChat.                                              *
 *                                                                            *
 * sChat is free software: you can redistribute it and/or modify              *
 * it under the terms of the GNU General Public License as published by       *
 * the Free Software Foundation, either version 3 of the License, or          *
 * (at your option) any later version.                                        *
 *                                                                            *
 * sChat is distributed in the hope that it will be useful,                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with sChat.  If not, see [http://www.gnu.org/licenses/].             *
 ******************************************************************************/
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

#include "message.h"
#include "network.h"

int peer_start(int sockfd, const char *host, const char *port)
{
	// Try to connect to the host provided 
	if ( (peer_connect(sockfd, host, port)) == 1){
		return 1;
	}

	// If we can't connect, act as the server
	if ( (peer_listen(sockfd, port)) == 1){
		return 1;
	}

	// Nothing is working. Today ain't your day.
	return 0;
}

int peer_listen(int sockfd, const char *port)
{

	int lsockfd;
	socklen_t sockaddr_size = sizeof(struct sockaddr);

	struct addrinfo server, *servinfo, *p;
	struct sockaddr_in peer;
	char peer_ip[INET_ADDRSTRLEN];

	// Configure the local socket and addrinfo

	memset(&server, 0, sizeof(server));
	server.ai_family = AF_UNSPEC;
	server.ai_socktype = SOCK_STREAM;
	server.ai_flags = AI_PASSIVE;

	if ((getaddrinfo(NULL, port, &server, &servinfo)) != 0){
		fprintf(stderr, "Error: Unable to prepare addrinfo\n");
		return 0;
	}

	// Loop through our addinfo structure and bind to the first possible result

	for(p = servinfo; p != NULL; p = p->ai_next){

		if ((lsockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol))  < 0 ){
			fprintf(stderr, "Error: Unable to create socket\n");
			return 0;
		}

		if (bind(lsockfd, p->ai_addr, p->ai_addrlen) < 0){
			fprintf(stderr, "Error: Bind\n");
			close(sockfd);
			continue;
		}

		break;
	}

	if (p == NULL){
		fprintf(stderr, "Error: Failed to bind to port %s\n", port);
	}

	freeaddrinfo(servinfo);

	if (listen(sockfd, MAX_CONNECTIONS) < 0){
		fprintf(stderr, "Error: Unable to listen on port %s\n", port);
		return 0;
	}

	// Wait for a peer to connect to us.

	while(1){
		if ( (sockfd = accept(sockfd, (struct sockaddr *)&peer, &sockaddr_size)) < 0){
			fprintf(stderr, "Error: could not accept connection from peer\n");
			return 0;
		}

		// Convert peer IP to a string

		inet_ntop(peer.sin_family, &peer.sin_addr, peer_ip, sizeof(peer_ip));

		printf("Connected to peer: %s\n", peer_ip);

		close(sockfd);
		break;
	}

	return 1;
}

int peer_connect(int sockfd, const char *host, const char *port)
{

	struct addrinfo client, *servinfo, *p;

	memset(&client, 0, sizeof(client));
	client.ai_family = AF_UNSPEC;
	client.ai_socktype = SOCK_STREAM;

	if ((getaddrinfo(host, port, &client, &servinfo)) != 0){
		fprintf(stderr, "Unable to assemble address info\n");
		return 0;
	}

	// Loop through our struct and connect to the first possible result

	while(1){

		for(p = servinfo; p != NULL; p = p->ai_next) {

			if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol))  < 0 ){
				fprintf(stderr, "Error unable to create socket\n");
				continue;
			}

			printf("Connecting to peer %s:%s\n",host,port);

			if (connect(sockfd, p->ai_addr, p->ai_addrlen) < 0){
				fprintf(stderr, "Error: Connect\n");
				close(sockfd);
				continue;
			}

			break;
		}

		if (p) break;
	}

	printf("Connected to peer %s:%s\n",host,port);
	freeaddrinfo(servinfo);

	return 1;
}

int peer_send(int sockfd, const char *message)
{
	if ((send(sockfd, message, strlen(message), 0)) < 0){
		fprintf(stderr, "Error: Unable to send message: %s",message);	
		return 0;
	}

	return 1;
}

int peer_recv(int sockfd)
{
	int r;
	char buf[MAX_MSG_LEN];
	if ((r = recv(sockfd, buf, MAX_MSG_LEN, 0)) < 0){
		fprintf(stderr, "Error: Unable to receive message\n");
		return 0;
	}

	return 1;
}

void peer_close(int sockfd)
{
	close(sockfd);
}
