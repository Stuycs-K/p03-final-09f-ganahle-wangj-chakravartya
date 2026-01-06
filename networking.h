#ifndef NETWORKING_H
#define NETWORKING_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <semaphore.h>

#define BUFFER_SIZE 256
#define PORT "9845"
#define BACKLOG 5

void err(int i, char s);

int client_tcp_handshake(charip);

#endif
