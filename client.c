#include "networking.h"
#include "game.h"

void send_safe(int socket, char *msg) {
  if (write(socket, msg, strlen(msg)) < 0) {
    perror("Write failed");
    close(socket);
    exit(1);
  }
}

// handle part of (game) state and interaciton with the user
void clientLogic(int server_socket) {

  // vars u need whole time
  char buff[BUFFER_SIZE];
  int game_active = 1;

  // loop
  while (game_active) {
    // read server message (from server)
    int bytes_read = read(server_socket,buff,(sizeof(buff)-1));
    // printf("bytes_read = %d\n", bytes_read);

    // check if the server disconnected
    if (bytes_read <= 0) {
      printf("Server disconnected. \n");
      close(server_socket);
      exit(0);
    }

    buff[bytes_read] = '\0';

    // parse message type by checking prefix
    if (strncmp(buff,MSG_WAIT,strlen(MSG_WAIT))==0) {
      // wait for opponent to pick num
      printf("Waiting for opponent to pick a number...\n");

    }
    else if (strncmp(buff,MSG_MATCH_REQUEST,strlen(MSG_MATCH_REQUEST))==0) {
      // match request. ask user if they want to play
      printf("\n--- Match Found! ---\n");
      printf("Do you want to play? (Click Y/N and enter): ");
      fflush(stdout);

      if (fgets(buff,BUFFER_SIZE,stdin) == NULL) {
        perror("Error fgets. ");
        close(server_socket);
        exit(0);
      }

      // send a response to server (to server)
      write(server_socket,buff,1); // just 1 char (y/n)

      if (buff[0]!='Y' && buff[0]!='y') {
        printf("You declined the match. Disconnecting...\n");
        game_active = 0;
      }

    }
    else if (strncmp(buff,MSG_YOU_PICK,strlen(MSG_YOU_PICK)) == 0) {
      // user needs to pick a number
      printf("\n--- Your Turn to Pick ---\n");
      printf("Pick a number from %d to %d and enter:",MIN_NUMBER,MAX_NUMBER);
      fflush(stdout);

      if (fgets(buff,BUFFER_SIZE,stdin) == NULL) {
        perror("Error fgets. ");
        close(server_socket);
        exit(0);
      }

      // validate input
      int num = atoi(buff); // turn string into int
      while ((num < MIN_NUMBER) || (num > MAX_NUMBER)) {
        printf("Invalid number! Pick a number from %d to %d: ", MIN_NUMBER, MAX_NUMBER);
        fflush(stdout);

        if (fgets(buff,BUFFER_SIZE,stdin) == NULL) {
          printf("Error fgets. ");
          close(server_socket);
          exit(0);
        }
        // try again
        num = atoi(buff);
      }

      // send number to server (to server)
      snprintf(buff,BUFFER_SIZE,"%d", num);
      write(server_socket, buff, strlen(buff));
      printf("You picked the number %d. \nWaiting for opponent to guess... \n",num);
    }
    else if (strncmp(buff,MSG_YOU_GUESS,strlen(MSG_YOU_GUESS))==0) {
      // user needs to guess
      // take the server number from message. format: "GUESS:X"
      char * colon = strchr(buff, ':');
      int server_num = 0;
      if (colon) {
        server_num = atoi(colon + 1);
      }

      // your turn to guess
      printf("\n--- Your Turn to Guess ---\n");
      printf("The server picked: %d\n", server_num);
      printf("Is your opponent's number HIGHER, LOWER, or EQUAL to the server's? \n");
      printf("Enter H (higher), L (lower), or E (equal) and enter: ");
      fflush(stdout);

      if (fgets(buff,BUFFER_SIZE,stdin) == NULL) {
        printf("Error fgets. ");
        close(server_socket);
        exit(0);
      }

      // make user input into game format
      char guess[BUFFER_SIZE];
      if (buff[0]=='H' || buff[0]=='h') {
        strcpy(guess,"HIGHER");
      } else if (buff[0]=='L' || buff[0]=='l') {
        strcpy(guess,"LOWER");
      } else if (buff[0]=='E' || buff[0]=='e') {
        strcpy(guess,"EQUAL");
      } else {
        // if whoever wrote this has size challenged fingers and put in the wrong input
        // this is the default that we put in the case that they mess up the input
        strcpy(guess,"EQUAL");
      }

      // send guess to server
      write(server_socket, guess, strlen(guess));
      printf("You guessed: %s \n", guess);

    }
    else if (  strncmp(buff,MSG_RESULT_WIN,strlen(MSG_RESULT_WIN))==  0 ||
               strncmp(buff,MSG_RESULT_LOSE,strlen(MSG_RESULT_LOSE))==0 ||
               strncmp(buff,MSG_RESULT_TIE,strlen(MSG_RESULT_TIE))==  0) {
      // game result
      char *  colon = strchr(buff,':');
      if (colon) {
        printf("\n--- Round Result: --- \n%s\n",colon+1); // show result
      }

    }
    else if (strncmp(buff,MSG_PLAY_AGAIN,strlen(MSG_PLAY_AGAIN)) == 0) {
      // ask user if they wanna play again
      printf("\nWant to play another round? (Press Y/N and enter): ");
      fflush(stdout); // clean

      if (fgets(buff,BUFFER_SIZE,stdin) == NULL) {
        perror("Error fgets. ");
        close(server_socket);
        exit(0); // exit
      }

      // send response to server
      write(server_socket,buff,1);

      if (buff[0]!='Y' && buff[0]!='y') {
        printf("Thanks for playing. :) \n");
        game_active = 0; // break loop
      }
    }
    else if (strncmp(buff,MSG_OPPONENT_LEFT,strlen(MSG_OPPONENT_LEFT)) == 0) {
      // the opponent disconnected (opp dipped)
      printf("\nYour opponent has left the game. \n");
      game_active = 0; // break loop
    }
    else {
      // print gneric server message
      printf("%s\n", buff);
    }
  }
  // end
}//clientLogic end



// main
int main(int argc, char * argv[]) {
  char * IP = "127.0.0.1";
  // printf("Stored IP: %s\n",IP);

  // arg to let client speicfy server ip
  if (argc > 1) {
    // printf("Param included. Setting IP to %s",argv[1]);
    IP = argv[1]; // set to da custom one if they speciified one
  }

  // welcoming client / say ip and port
  printf("--- Welcome to Higher or Lower (game) --- \n");
  printf("Connecting to server at %s:%s... \n",IP,PORT);

  // connect to the server
  // printf("Attempting client tpc handshake\n");
  int server_socket = client_tcp_handshake(IP); // handshake
  printf("Connected to server. \n");

  // start the game logic
  printf("Try client logic method\n");
  clientLogic(server_socket);

  // close socket + clean up
  printf("Cleaning up\n");
  close(server_socket);
  printf("Disconnected from server. \n");

  // end
  return 0;
}//main end
