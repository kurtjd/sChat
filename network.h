/******************************************************************************
 * network.h: Handles all network connectivity in the program.                *
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
#ifndef NETWORK_H
#define NETWORK_H

#define MAX_CONNECTIONS 2

int peer_listen(int sockfd, const char *port);
int peer_connect(int sockfd, const char *host, const char *port);
int peer_send(int sockfd, const char *message);
int peer_recv(int sockfd);
int peer_start(int sockfd, const char *host, const char *port);
void peer_close(int sockfd);

#endif
