#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Define server port and buffer size
#define SERVER_PORT 8080 
#define BUFFER_SIZE 1024 

int main() {
    int sockfd; // The socket file descriptor
    struct sockaddr_in server_address, client_address; // Structures for server and client addresses
    socklen_t client_address_length = sizeof(client_address); // The length of the client's address
    char buffer[BUFFER_SIZE]; // The message buffer

    // Create a UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    // Initialize the server address structure with zeros
    memset(&server_address, 0, sizeof(server_address));

    // Set the server address family, IP, and port number
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(SERVER_PORT);

    // Bind the socket to the server address
    bind(sockfd, (struct sockaddr *)&server_address, sizeof(server_address));

    // Loop to receive messages and save them in a file
    while (1) {
        FILE *file = fopen("registrations.txt", "a"); // Open the file for appending
        
        // Receive a message from a client and store it in the buffer
        recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_address, &client_address_length);
        
        // Write the message to the file
        fprintf(file, "%s\n", buffer);
        
        // Close the file
        fclose(file);
    }

    // Close the socket and exit the program
    close(sockfd);
    return 0;
}
