#include "networking.h"
#include "game.h"


// subserver logic
void subserver_logic(int client1_socket, int client2_socket) {

  struct game g;
  char buffer[BUFFER_SIZE];

  // make client 1 pick a number
  send(client1_socket, MSG_YOU_PICK "\n",strlen(MSG_YOU_PICK),0);

  int output = recv(client1_socket,buffer,sizeof(buffer),0);
  // if output is -1 it means the client left
  if (output <= 0) {
    printf("Client 1 closed. \n");
    return; // end
  }
  buffer[output] = '\0'; // null terminate
  int client1_number = atoi(buffer); // get client number
  printf("Client 1 chose: %d\n", client1_number);

  game_init(&g, client1_number);
  printf("Server chose: %d\n", g.setpoint);

  // tell 2nd client to guess + and send them the server's number
  char guess_msg[BUFFER_SIZE];
  snprintf(guess_msg,BUFFER_SIZE,MSG_YOU_GUESS ":%d\n",g.setpoint);
  send(client2_socket,guess_msg,strlen(guess_msg),0);

  // get guess from client 2
  output = recv(client2_socket, buffer, sizeof(buffer), 0);
  // if no client (-1)
  if (output <= 0) {

    printf("Client 2 closed. \n");
    return; // end

  }
  buffer[output] = '\0'; // i alr commented this above (null terminate)
  printf("Client 2 guessed: %s\n", buffer);

  // czech if guess is correct
  int correct = check_guess(&g, buffer);

  // send results to both clients
  char result_msg[BUFFER_SIZE];
  if (correct) {

    snprintf(result_msg, BUFFER_SIZE, MSG_RESULT_WIN ":Client 2 wins! Client 1 chose %d, server chose %d. \n",
             client1_number, g.setpoint);
    send(client2_socket, result_msg, strlen(result_msg),0); // send it

    snprintf(result_msg, BUFFER_SIZE, MSG_RESULT_LOSE ":You lose! You chose %d, server chose %d. \n",
             client1_number, g.setpoint);
    send(client1_socket,result_msg,strlen(result_msg),0); // send it

  }
  // if not correct
  else {
    snprintf(result_msg, BUFFER_SIZE, MSG_RESULT_LOSE ":Client 2 loses! Client 1 chose %d, server chose %d\n",
             client1_number, g.setpoint);
    send(client2_socket, result_msg, strlen(result_msg), 0);

    snprintf(result_msg, BUFFER_SIZE, MSG_RESULT_WIN ":You win! You chose %d, server chose %d\n",
             client1_number, g.setpoint);
    send(client1_socket,result_msg,strlen(result_msg),0);
  }

  // clean up
  close(client1_socket);
  close(client2_socket);

}



// main
int main(int argc, char *argv[]) {
  // set up
  int listen_socket = server_setup();

  while (1) {
    printf("Waiting for two players... \n");

    // wWait for first client
    int client1_socket = server_tcp_handshake(listen_socket);
    printf("Player 1 connected. \n");
    send(client1_socket, MSG_WAIT "\n",strlen(MSG_WAIT), 0);

    // wait for second client
    int client2_socket = server_tcp_handshake(listen_socket);
    printf("Player 2 connected. \n");

    // fork to handle the game
    if (fork() == 0) {
      close(listen_socket);
      subserver_logic(client1_socket,client2_socket);
      exit(0);
    }

    // clean
    // printf("Clean up. \n");
    close(client1_socket);
    close(client2_socket);

  }//while
}
