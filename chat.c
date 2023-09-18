/*******************************************
* Group Name  : XXXXXX

* Member1 Name: XXXXXX
* Member1 SIS ID: XXXXXX
* Member1 Login ID: XXXXXX

* Member2 Name: XXXXXX
* Member2 SIS ID: XXXXXX
* Member2 Login ID: XXXXXX
********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void server_side(){
  printf("Server side!\n");
}

void client_side(const char* ip_address, int port){
  printf("Client side!\n");
  printf("Address: %s\nPort: %d\n", ip_address, port);
  // Add actual client logic here
}

int check_port(int port){
  // Check if port is in range
  if(port < 1 || port > 65535){
    return 1;
  }

  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if(socket_fd < 0) {
    perror("socket");
    return 1;
  }

  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(port);

  if (bind(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
      // Port is in use
      close(socket_fd);
      return 1;
  }
  // Port is not in use
  close(socket_fd);
  return 0;
}

int main(int argc, char* argv[]){
  if(argc == 1){
    server_side();
    return 0;
  }

  if (strcmp(argv[1], "-h") == 0){
    printf("This program is designed to act as a client and server for messaging.\nPlease use the format ./chat to launch the server, and ./chat -p [port] -s [address] to launch the client.\nThank you!\n");
    exit(0);
  }

  if(argc > 5){
    printf("Too many arguments!!\nPlease use the format ./chat to launch the server, and ./chat -p [port] -s [address] to launch the client.\nThank you!\n");
    exit(0);
  }

  //struct sockaddr_in addr;
  //addr.sin_family = AF_INET;

  if(strcmp(argv[1], "-s") == 0){
    const char* ip_address = argv[2];
    int port = atoi(argv[4]);
    client_side(ip_address, port);

  } else if(strcmp(argv[1], "-p") == 0){
    const char* ip_address = argv[4];
    int port = atoi(argv[2]);
    client_side(ip_address, port);

  } else{
    printf("Incorrect arguments.\nPlease use the format ./chat to launch the server, and ./chat -p [port] -s [address] to launch the client.\nThank you!\n");
  }

  return 0;
}
