// Registration UDP concurrency with multiplexing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/select.h>

#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_length = sizeof(client_address);
    fd_set read_fds, temp_fds;
    int max_fd, activity, i;
    char buffer[BUFFER_SIZE];

    // Create a new UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(SERVER_PORT);

    // Bind socket to the server address
    bind(sockfd, (struct sockaddr *)&server_address, sizeof(server_address));
    printf("Server started and waiting for data...\n");

    // Initialize the file descriptor sets
    FD_ZERO(&read_fds);
    FD_SET(sockfd, &read_fds);
    max_fd = sockfd;

    while (1) {
        temp_fds = read_fds;
        
        // Use select() to monitor the sockets for activity
        activity = select(max_fd + 1, &temp_fds, NULL, NULL, NULL);
        if (activity < 0) {
            perror("select");
            exit(EXIT_FAILURE);
        }

        for (i = 0; i <= max_fd; i++) {
            if (FD_ISSET(i, &temp_fds)) {
                if (i == sockfd) {
                    // New client connection, receive data and handle the request
                    memset(buffer, 0, BUFFER_SIZE);
                    recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_address, &client_address_length);
                    printf("Received data from client: %s\n", buffer);

                    // Write data to file
                    FILE *file = fopen("registrations.txt", "a");
                    fprintf(file, "%s\n", buffer);
                    fclose(file);
                    printf("Data written to file.\n");

                    // Send acknowledgement to client
                    char ack[] = "Data received successfully";
                    sendto(sockfd, ack, strlen(ack), 0, (struct sockaddr *)&client_address, client_address_length);
                    printf("Sent acknowledgement to client.\n");
                }
            }
        }
    }

    close(sockfd);
    return 0;
}
