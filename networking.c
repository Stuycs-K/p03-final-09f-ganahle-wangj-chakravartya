#include <netdb.h>
#include "networking.h"

/*Create and bind a socket.
* Place the socket in a listening state.
* returns the socket descriptor
*/
int server_setup() {

  //setup structs for getaddrinfo
  struct addrinfo hints, *results;
  memset(&hints, 0, sizeof(hints));

  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  err(getaddrinfo(NULL, PORT, &hints, &results), "where am i");

  //create the socket
  int clientd;//store the socket descriptor here
  clientd = socket(results -> ai_family, results -> ai_socktype, results -> ai_protocol);
  err(clientd, "socks no smell good");

  //this code should get around the address in use error
  int yes = 1;
  setsockopt(clientd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

  //bind the socket to address and port
  err(bind(clientd, results -> ai_addr, results -> ai_addrlen), "bind bad");

  //set socket to listen state
  err(listen(clientd, 10), "cant hear");

  //free the structs used by getaddrinfo
  freeaddrinfo(results);

  return clientd;

}

/*Accept a connection from a client
 *return the socket descriptor for the new socket connected to the client
 *blocks until connection is made.
 */
int server_tcp_handshake(int listen_socket) {

  int client_socket;

  //accept() the client connection
  client_socket = accept(listen_socket, NULL, NULL);
  err(client_socket, "accept (not)");

  return client_socket;

}

/*Connect to the server
 *return the to_server socket descriptor
 *blocks until connection is made.*/
int client_tcp_handshake(char * server_address) {

  //getaddrinfo
  struct addrinfo hints, *results;
  memset(&hints, 0, sizeof(hints));

  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  err(getaddrinfo(server_address, PORT, &hints, &results), "where am i");

  int serverd;//store the socket descriptor here
  //create the socket
  serverd = socket(results -> ai_family, results -> ai_socktype, results -> ai_protocol);
  err(serverd, "socks bad 2");

  //connect() to the server
  err(connect(serverd, results -> ai_addr, results -> ai_addrlen), "connect 5");

  freeaddrinfo(results);

  return serverd;

}

void err(int i, char*message) {

  if(i < 0){

      printf("Error: %s - %s\n", message, strerror(errno));
      exit(1);

  }
}
