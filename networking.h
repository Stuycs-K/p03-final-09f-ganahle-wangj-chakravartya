#ifndef NETWORKING_H
#define NETWORKING_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>

#define BUFFER_SIZE 256
#define PORT "9845"
#define BACKLOG 5

int server_setup();
int server_tcp_handshake(int listen_socket);
int client_tcp_handshake(char *server_address);
void err(int i, char *message);

#endif
