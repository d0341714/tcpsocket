#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8888
#define BUFSIZE 1024

int main(void) {
    int serv_sock, clnt_sock;
    char message[BUFSIZE];
    int str_len;

    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);

    // Create socket
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1) {
        perror("socket() error");
        exit(1);
    }

    // Set server address
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORT);

    // Bind socket to address
    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("bind() error");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(serv_sock, 5) == -1) {
        perror("listen() error");
        exit(1);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        // Accept incoming connection
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        if (clnt_sock == -1) {
            perror("accept() error");
            exit(1);
        }

        printf("Client connected: %s\n", inet_ntoa(clnt_addr.sin_addr));

        while ((str_len = read(clnt_sock, message, BUFSIZE)) != 0) {
            message[str_len] = '\0';
            printf("Received message from client: %s\n", message);

            // Echo message back to client
            if (write(clnt_sock, message, str_len) == -1) {
                perror("write() error");
                exit(1);
            }
        }

        close(clnt_sock);
        printf("Client disconnected.\n");
    }

    close(serv_sock);

    return 0;
}
