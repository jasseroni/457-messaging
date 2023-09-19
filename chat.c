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
  //Check if port is in range
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

  //Check is port is in use
  /*if (bind(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    close(socket_fd);
    return 1;
  }*/
  //If port is not in use
  close(socket_fd);
  return 0;
}

void server_side(){
    printf("Welcome to chat!\n");

    int listen_fd, conn_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    char buffer[1024];

    //Create a socket
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) { //if address is already in use
        perror("socket");
        exit(1);
    }

    //Bind to port
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);  // Change later for specification

    if (bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(1);
    }

    char str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &server_addr, str, INET_ADDRSTRLEN);

    printf("Waiting for a connection on %s port %d\n", str, server_addr.sin_port);

    //Listen on socket
    if (listen(listen_fd, 10) < 0) {
        perror("listen");
        exit(1);
    }

    printf("Listening on port 8080...\n");

    //Accept client connection
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
        printf("Client: %s", buffer);

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

int isValidIpAddress(char *address){
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, address, &(sa.sin_addr));
    return result;
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

        printf("Server: %s", buffer);
    }

    close(client_fd);
}


int main(int argc, char* argv[]) {
    int port = 8080;  // Default port for the server

    //SERVER
    if(argc == 1) {
        server_side();
        return 0;
    }

    //HELP
    if (strcmp(argv[1], "-h") == 0) {
        printf("This program is designed to act as a client and server for messaging.\n");
        printf("Please use the format ./chat to launch the server, and ./chat -p [port] -s [address] to launch the client.\n");
        printf("Thank you!\n");
        return 0;
    }

    //Too many arguments
    if(argc > 5) {
        printf("Too many arguments!\n");
        printf("Please use the format ./chat to launch the server, and ./chat -p [port] -s [address] to launch the client.\n");
        printf("Thank you!\n");
        return 1;
    }

    // Run as a client with server address and port number
    if((strcmp(argv[1], "-s") == 0 && strcmp(argv[3], "-p") == 0) || (strcmp(argv[1], "-p") == 0 && strcmp(argv[3], "-s") == 0)) {

        char* ip_address = (strcmp(argv[1], "-s") == 0) ? argv[2] : argv[4];
        port = (strcmp(argv[1], "-p") == 0) ? atoi(argv[2]) : atoi(argv[4]);
        //SANITY CHECK
        /*if(check_port(port) == 1 || isValidIpAddress(ip_address) == 0){
            if(check_port(port) == 1) printf("Invalid port\n");
            if(isValidIpAddress(ip_address) == 0) printf("Invalid IP address\n");
            exit(1);
        }*/
        client_side(ip_address, port);
        return 0;
    }

    // Incorrect arguments provided
    printf("Incorrect arguments.\n");
    printf("Please use the format ./chat to launch the server, and ./chat -p [port] -s [address] to launch the client.\n");
    printf("Thank you!\n");
    return 1;
}
