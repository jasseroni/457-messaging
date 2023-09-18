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

void server_side(){
  printf("Server side!\n");
}

void client_side(const char* ip_address, int port){
  printf("Client side!\n");
  printf("Address: %s\nPort: %d\n", ip_address, port);
  
  if(check_port(port) == 1){
    printf("Invalid port\n");
    exit(1);
  }
}

int isValidIpAddress(char *ipAddress){
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipAddress, &(sa.sin_addr)); //1 for valid, 0 for invalid
    //printf("%d\n", result);
    return result;
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
    perror("Too many arguments!!\nPlease use the format ./chat to launch the server, and ./chat -p [port] -s [address] to launch the client.\nThank you!\n");
    exit(1);
  }

  if(strcmp(argv[1], "-s") == 0){
    char* ip_address = argv[2];
    int port = atoi(argv[4]);
    //SANITY CHECK
    if(isValidIpAddress(ip_address) == 0 || check_port(port) == 1){
      if(isValidIpAddress(ip_address) == 0){
        printf("Invalid IP address\n");
      }
      if(check_port(port) == 1){
        printf("Invalid port\n");
      }
      exit(1);
    }

    client_side(ip_address, port);

  } else if(strcmp(argv[1], "-p") == 0){
    char* ip_address = argv[4];
    int port = atoi(argv[2]);
    //SANITY CHECK
    if(isValidIpAddress(ip_address) == 0 || check_port(port) == 1){
      if(isValidIpAddress(ip_address) == 0){
        printf("Invalid IP address\n");
      }
      if(check_port(port) == 1){
        printf("Invalid port\n");
      }
      exit(1);
    }

    client_side(ip_address, port);

  } else{
    printf("Incorrect arguments.\nPlease use the format ./chat to launch the server, and ./chat -p [port] -s [address] to launch the client.\nThank you!\n");
  }

  return 0;
}
