// Registration client side
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE];

    // Create a new TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up server address structure
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr) <= 0) {
        perror("Invalid server IP address");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Send data to the server
    char data[] = "Hello, server!";
    if (write(sockfd, data, strlen(data)) < 0) {
        perror("Write failed");
        exit(EXIT_FAILURE);
    }

    // Receive response from the server
    memset(buffer, 0, BUFFER_SIZE);
    if (read(sockfd, buffer, BUFFER_SIZE) < 0) {
        perror("Read failed");
        exit(EXIT_FAILURE);
    }

    printf("Received response from server: %s\n", buffer);

    // Close the socket
    close(sockfd);

    return 0;
}
