#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080       // Default port value
#define BUFFER 1024

int main(int argc, char *argv[]) {
    struct sockaddr_in address;
    int port = PORT;
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);    // Create socket
    int new_socket;
    int address_len = sizeof(address);
    int option = 1;
    char buffer[BUFFER] = {0};
    // Checks for -p argument and changes port value
    if (argc == 3 && strcmp(argv[1], "-p") == 0) {
        port = atoi(argv[2]);
    }
    // Create socket error
    if (server_fd == 0) {
        perror("Socket failed.");
        exit(EXIT_FAILURE);
    }
    // Set socket options error
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option))) {
        perror("Socket Options failed.");
        exit(EXIT_FAILURE);
    }
    // Address structure
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    // Binds socket to specified port and IP
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    // Listen for any connections
    if (listen(server_fd, 1) < 0) {
        perror("Listen failed.");
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d\n", port);

    while (1) {
        // Accept new connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&address_len)) < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }
         printf("Connection accepted.\n");
        
        // Echo messages back to client
        ssize_t read_bytes;
        while((read_bytes = read(new_socket, buffer, BUFFER)) > 0) {
            send(new_socket, buffer, read_bytes, 0);
        }

        // Close connection and clear buffer for next connection
        close(new_socket);
        memset(buffer, 0, BUFFER);
    }
    // Close server
    close(server_fd);
    return 0;
}