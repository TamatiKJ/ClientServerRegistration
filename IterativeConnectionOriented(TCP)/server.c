#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

// Define server port and buffer size
#define SERVER_PORT 8080 
#define BUFFER_SIZE 1024 

int main() {
    int sockfd, newsockfd; // The socket file descriptors for the server and client sockets
    struct sockaddr_in server_address, client_address; // The server and client address structures
    socklen_t client_address_length = sizeof(client_address); // The length of the client address structure
    char buffer[BUFFER_SIZE]; // The message buffer

    // Create a TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Initialize the server address structure with zeros
    memset(&server_address, 0, sizeof(server_address));

    // Set the server address family, IP, and port number
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY; // The server can bind to any available IP address
    server_address.sin_port = htons(SERVER_PORT);

    // Bind the server socket to the specified IP address and port number
    bind(sockfd, (struct sockaddr *)&server_address, sizeof(server_address));

    // Listen for incoming connections on the server socket
    listen(sockfd, 5);

    // Enter an infinite loop to accept and handle client connections
    while (1) {
        // Accept a new client connection and create a new socket file descriptor for the client
        newsockfd = accept(sockfd, (struct sockaddr *)&client_address, &client_address_length);

        // Open the registrations.txt file for writing and append the message received from the client
        FILE *file = fopen("registrations.txt", "a");
        read(newsockfd, buffer, BUFFER_SIZE);
        fprintf(file, "%s\n", buffer);
        fclose(file);

        // Close the client socket and continue listening for new connections
        close(newsockfd);
    }

    // Close the server socket and exit the program
    close(sockfd);
    return 0;
}
