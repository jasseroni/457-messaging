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

    int listen_fd, conn_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    char buffer[1024];

    // Step 1: Create a socket
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        perror("socket");
        exit(1);
    }

    // Step 2: Bind to a port
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);  // Use port 8080

    if (bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(1);
    }

    // Step 3: Listen for incoming connections
    if (listen(listen_fd, 10) < 0) {
        perror("listen");
        exit(1);
    }

    printf("Listening on port 8080...\n");

    // Step 4: Accept a client connection
    client_len = sizeof(client_addr);
    conn_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_len);
    if (conn_fd < 0) {
        perror("accept");
        exit(1);
    }

    printf("Connected to client!\n");

    // Step 5: Interact with the client
    while (1) {
        // Clear buffer
        memset(buffer, 0, sizeof(buffer));

        // Receive a message from the client
        int read_bytes = recv(conn_fd, buffer, sizeof(buffer), 0);
        if (read_bytes <= 0) {
            printf("Client disconnected or error.\n");
            break;
        }

        // Print the client's message to the screen
        printf("Client: %s\n", buffer);

        // Get a message to send to the client
        printf("You: ");
        fgets(buffer, sizeof(buffer), stdin);

        // Send the message to the client
        send(conn_fd, buffer, strlen(buffer), 0);
    }

    // Step 6: Close the sockets
    close(conn_fd);
    close(listen_fd);
}



void client_side(const char* ip_address, int port) {
    printf("Client side!\n");
    printf("Address: %s\nPort: %d\n", ip_address, port);

    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[1024];

    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("socket");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip_address, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        exit(1);
    }

    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        exit(1);
    }

    printf("Connected to server!\n");

    while (1) {
        printf("You: ");
        fgets(buffer, sizeof(buffer), stdin);
        send(client_fd, buffer, strlen(buffer), 0);

        memset(buffer, 0, sizeof(buffer));
        int read_bytes = recv(client_fd, buffer, sizeof(buffer), 0);
        if (read_bytes <= 0) {
            printf("Server disconnected or error.\n");
            break;
        }

        printf("Server: %s\n", buffer);
    }

    close(client_fd);
}


int main(int argc, char* argv[]) {
    int port = 8080;  // Default port for the server

    // No arguments given, run as a server on the default port
    if(argc == 1) {
        server_side(port);
        return 0;
    }

    // Help option
    if (strcmp(argv[1], "-h") == 0) {
        printf("This program is designed to act as a client and server for messaging.\n");
        printf("Please use the format ./chat to launch the server, and ./chat -p [port] -s [address] to launch the client.\n");
        printf("Thank you!\n");
        return 0;
    }

    // Check for too many arguments
    if(argc > 5) {
        printf("Too many arguments!\n");
        printf("Please use the format ./chat to launch the server, and ./chat -p [port] -s [address] to launch the client.\n");
        printf("Thank you!\n");
        return 1;
    }

    // Run as a client with server address and port number
    if((strcmp(argv[1], "-s") == 0 && strcmp(argv[3], "-p") == 0) || 
       (strcmp(argv[1], "-p") == 0 && strcmp(argv[3], "-s") == 0)) {

        const char* ip_address = (strcmp(argv[1], "-s") == 0) ? argv[2] : argv[4];
        port = (strcmp(argv[1], "-p") == 0) ? atoi(argv[2]) : atoi(argv[4]);
        client_side(ip_address, port);
        return 0;
    }

    // Incorrect arguments provided
    printf("Incorrect arguments.\n");
    printf("Please use the format ./chat to launch the server, and ./chat -p [port] -s [address] to launch the client.\n");
    printf("Thank you!\n");
    return 1;
}
