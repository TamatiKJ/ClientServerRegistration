#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

// Define server port and buffer size
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

int main() {
    // Initialize variables
    int sockfd;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE];

    // Create a new socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&server_address, 0, sizeof(server_address));

    // Set server address and port
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); // localhost
    server_address.sin_port = htons(SERVER_PORT);

    // Connect to the server
    connect(sockfd, (struct sockaddr *)&server_address, sizeof(server_address));

    // Prompt user to enter registration number and name
    printf("Enter registration number and name: ");
    fgets(buffer, BUFFER_SIZE, stdin); // read input from user
    buffer[strcspn(buffer, "\n")] = 0; // remove newline character

    // Send data to the server
    write(sockfd, buffer, strlen(buffer));

    // Close the socket
    close(sockfd);
    return 0;
}
