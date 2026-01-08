#include "networking.h"
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netdb.h>
#include <unistd.h>



/*Create and bind a socket.
* Place the socket in a listening state.
* returns the socket descriptor
*/
int server_setup() {
  //setup structs for getaddrinfo
  struct addrinfo * hints, * results;//results is allocated in getaddrinfo

  memset(&hints, 0, sizeof(hints));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM; //TCP socket
  hints->ai_flags = AI_PASSIVE; //only needed on server
  int adressissue= getaddrinfo(NULL, PORT, hints, &results);  //Server sets node to NU
  err(adressissue, "error in getting address");

  //create the socket
  int sd = socket(results->ai_family,results->ai_socktype,results->ai_protocol);//telling them its listening, just for accepting new connections, its not going to the client
  err(sd, "socket error start");

  int clientd=sd;//store the socket descriptor
  //this code should get around the address in use error
  int yes = 1;
  int sockOpt =  setsockopt(clientd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
  err(sockOpt,"sockopt  erro");
    //what is this doing and why is there not an option for comment on? Stay tuned to find out next time!
  //bind the socket to address and port
  int binder= bind(clientd, results->ai_addr, results->ai_addrlen);
  err(binder, "error in the bind section");

  printf("bind complete\n");
  fflush(stdout);

  //set socket to listen state
  int l = listen(clientd, 10);
  err(l, "error in the listen section");

  //free the structs used by getaddrinfo
  free(hints);
  freeaddrinfo(results);

  return clientd;
}
/*Accept a connection from a client
 *return the socket descriptor for the new socket connected to the client
 *blocks until connection is made.
 */
int server_tcp_handshake(int listen_socket){
    int client_socket;

  //accept() the client connection
    socklen_t sock_size;
    struct sockaddr_storage client_address;

    sock_size = sizeof(client_address);
    client_socket = accept(listen_socket, (struct sockaddr *)&client_address, &sock_size);
    err(client_socket, "error in accepting client connection");
    return client_socket;
}


/*Connect to the server
 *return the to_server socket descriptor
 *blocks until connection is made.*/
int client_tcp_handshake(char * server_address) {
    //getaddrinfo
    struct addrinfo * hints, * results;//results is allocated in getaddrinfo
    memset(&hints, 0, sizeof(hints));
    hints->ai_family = AF_INET;
    hints->ai_socktype = SOCK_STREAM; //TCP socket
    getaddrinfo(server_address, PORT, hints, &results);


  int serverd;//store the socket descriptor here
  //create the socket
  serverd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
  err(serverd, "serverdbottom");

  //connect() to the server
  int c=connect(serverd, results->ai_addr, results->ai_addrlen);
  err(c, "error in clienttcpshake connection");

  free(hints);
  freeaddrinfo(results);

  return serverd;
}

void err(int i, char*message){
  if(i < 0){
      printf("Error: %s - %s\n",message, strerror(errno));
      exit(1);
  }
}
