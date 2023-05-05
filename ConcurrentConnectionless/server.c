#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

// Define server port and buffer size
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

/* This function writes the contents of the buffer to a file called "registrations.txt". */
void handle_client(int sockfd, struct sockaddr_in client_address, char *buffer) {
    socklen_t client_address_length = sizeof(client_address);
    FILE *file = fopen("registrations.txt", "a");
    fprintf(file, "%s\n", buffer);
    fclose(file);
}

int main() {
    int sockfd;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_length = sizeof(client_address);
    char buffer[BUFFER_SIZE];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(SERVER_PORT);

    bind(sockfd, (struct sockaddr *)&server_address, sizeof(server_address));

    /* This loop listens for incoming datagrams and creates a child process to handle each one. */
    while (1) {
        recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_address, &client_address_length);
        if (fork() == 0) {
            handle_client(sockfd, client_address, buffer);
            exit(0);
        }
    }

    close(sockfd);
    return 0;
}
