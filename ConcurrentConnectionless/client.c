#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Define server port and buffer size
#define SERVER_PORT 8080 
#define BUFFER_SIZE 1024 

int main() {
    int sockfd; // The socket file descriptor
    struct sockaddr_in server_address; // The server address structure
    char buffer[BUFFER_SIZE]; // The message buffer

    // Create a UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Initialize the server address structure with zeros
    memset(&server_address, 0, sizeof(server_address));

    // Set the server address family, IP, and port number
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); // The server IP address
    server_address.sin_port = htons(SERVER_PORT);

    // Prompt the user for input
    printf("Enter registration number and name: ");
    
    // Read the input from the user and store it in the buffer
    fgets(buffer, BUFFER_SIZE, stdin);
    
    // Remove the trailing newline character
    buffer[strcspn(buffer, "\n")] = 0;

    // Send the message to the server
    sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_address, sizeof(server_address));

    // Close the socket and exit the program
    close(sockfd);
    return 0;
}
