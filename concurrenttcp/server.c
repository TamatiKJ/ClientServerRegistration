// Registration TCP with child processes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

void handle_client(int sockfd) {
    char buffer[BUFFER_SIZE];
    FILE *file = fopen("registrations.txt", "a");

    // Read client request from socket
    read(sockfd, buffer, BUFFER_SIZE);
    printf("Received data from client: %s\n", buffer);

    // Write data to file
    fprintf(file, "%s\n", buffer);
    fclose(file);
    printf("Data written to file.\n");
}

int main() {
    int sockfd, newsockfd;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_length = sizeof(client_address);

    // Create a new TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(SERVER_PORT);

    // Bind socket to the server address
    bind(sockfd, (struct sockaddr *)&server_address, sizeof(server_address));
    printf("Server started and listening for connections...\n");

    // Listen for incoming connections
    listen(sockfd, 5);

    while (1) {
        // Accept client connections and handle their requests
        newsockfd = accept(sockfd, (struct sockaddr *)&client_address, &client_address_length);
        printf("Accepted a new client connection.\n");

        if (fork() == 0) {
            // Child process
            close(sockfd);
            handle_client(newsockfd);
            printf("Client connection handled and child process exiting.\n");
            exit(0);
        }

        // Parent process
        close(newsockfd);
    }

    close(sockfd);
    return 0;
}
