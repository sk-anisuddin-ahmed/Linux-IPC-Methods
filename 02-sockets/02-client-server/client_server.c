#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/client_server.sock"
#define BUFFER_SIZE 256

// Server code
int server() {
    int server_fd, client_fd;
    struct sockaddr_un addr;
    char buffer[BUFFER_SIZE];

    // Create socket
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        return EXIT_FAILURE;
    }

    // Set up address
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    // Bind
    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1) {
        perror("bind");
        close(server_fd);
        return EXIT_FAILURE;
    }

    // Listen
    if (listen(server_fd, 5) == -1) {
        perror("listen");
        close(server_fd);
        return EXIT_FAILURE;
    }

    printf("Server listening on %s\n", SOCKET_PATH);

    // Accept connection
    client_fd = accept(server_fd, NULL, NULL);
    if (client_fd == -1) {
        perror("accept");
        close(server_fd);
        return EXIT_FAILURE;
    }

    // Read and echo
    ssize_t num_read;
    while ((num_read = read(client_fd, buffer, BUFFER_SIZE)) > 0) {
        write(client_fd, buffer, num_read);
    }

    // Clean up
    close(client_fd);
    close(server_fd);
    unlink(SOCKET_PATH);

    return 0;
}

// Client code
int client() {
    int sockfd;
    struct sockaddr_un addr;
    char buffer[BUFFER_SIZE] = "Hello from client";

    // Create socket
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return EXIT_FAILURE;
    }

    // Set up address
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    // Connect
    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1) {
        perror("connect");
        close(sockfd);
        return EXIT_FAILURE;
    }

    // Send message
    write(sockfd, buffer, strlen(buffer));

    // Read response
    ssize_t num_read = read(sockfd, buffer, BUFFER_SIZE);
    if (num_read > 0) {
        buffer[num_read] = '\0';
        printf("Received: %s\n", buffer);
    }

    // Clean up
    close(sockfd);

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <server|client>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "server") == 0) {
        return server();
    } else if (strcmp(argv[1], "client") == 0) {
        return client();
    } else {
        fprintf(stderr, "Invalid argument: %s\n", argv[1]);
        return EXIT_FAILURE;
    }
}