#include "networking.h"

void subserver_logic(int client_socket) {

  char buffer[BUFFER_SIZE];

  while (1) {

    int output = recv(client_socket, buffer, sizeof(buffer), 0);

    if (output <= 0) {

      printf("Client closed\n");
      break;

    }

    buffer[output] = '\0';
    printf("received: %s\n", buffer);

    output = send(client_socket, buffer, strlen(buffer), 0);
    if (output <= 0) {

      break;

    }

  }

  // close(client_socket);

}

int main(int argc, char *argv[] ) {

  // int listen_socket = server_setup();

  // int client_socket = server_tcp_handshake(listen_socket);

  // close(listen_socket);

}
