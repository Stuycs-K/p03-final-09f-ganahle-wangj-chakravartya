#ifndef NETWORKING_H
#define NETWORKING_H

// stuff from client.c that's important for game
#define MSG_WAIT "WAIT"
#define MSG_MATCH_REQUEST "MATCH?"
#define MSG_YOU_PICK "PICK"
#define MSG_YOU_GUESS "GUESS"
#define MSG_RESULT_WIN "WIN"
#define MSG_RESULT_LOSE "LOSE"
#define MSG_RESULT_TIE "TIE"
#define MSG_PLAY_AGAIN "AGAIN?"
#define MSG_OPPONENT_LEFT "OPPONENT_LEFT"
#define MIN_NUMBER 1
#define MAX_NUMBER 10

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
