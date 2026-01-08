#include "networking.h"

void subserver_logic(int client_socket) {
  int is_it_picked=0; //boolean to make sure only picked when not picked
  char buffer[BUFFER_SIZE];
  if (!is_it_picked){
    send(client_socket, MSG_WAIT "\n", strlen(MSG_WAIT), 0); //tells client to wait at start, changed to incorporate message from client
    is_it_picked=1;
  }


  while (1) {

    int output = recv(client_socket, buffer, sizeof(buffer), 0);

    if (output <= 0) {

      printf("Client closed\n");
      break;

    }

    buffer[output] = '\0';
    printf("received: %s\n", buffer);

    output= send(client_socket, MSG_YOU_PICK "\n", strlen(MSG_YOU_PICK), 0); //changed this to incorporate message from client


    if (output <= 0) break;

  }

  close(client_socket);
}

int main(int argc, char *argv[]) {

  int listen_socket = server_setup();

  while (1) {

    int client_socket = server_tcp_handshake(listen_socket);

    if (fork() == 0) {

      close(listen_socket);
      subserver_logic(client_socket);
      exit(0);

    }

    close(client_socket);

  }
}
