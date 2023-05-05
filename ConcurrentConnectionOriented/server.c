#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

void handle_client(int sockfd) {
    // This function handles the incoming client connection and writes the data received to a file
    char buffer[BUFFER_SIZE];
    FILE *file = fopen("registrations.txt", "a");
    read(sockfd, buffer, BUFFER_SIZE);
    fprintf(file, "%s\n", buffer);
    fclose(file);
}

int main() {
    int sockfd, newsockfd;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_length = sizeof(client_address);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(SERVER_PORT);

    bind(sockfd, (struct sockaddr *)&server_address, sizeof(server_address));
    listen(sockfd, 5);

    while (1) {
        // Wait for a new client connection and handle it in a new process
        newsockfd = accept(sockfd, (struct sockaddr *)&client_address, &client_address_length);
        if (fork() == 0) {
            close(sockfd);
            handle_client(newsockfd);
            exit(0);
        }
        close(newsockfd);
    }

    close(sockfd);
    return 0;
}
