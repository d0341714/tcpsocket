#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_ADDR "127.0.0.1"
#define PORT 8888
#define BUFSIZE 1024

int main(void) {
    int sock;
    char message[BUFSIZE];
    int str_len;

    struct sockaddr_in serv_addr;

    // Create socket
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket() error");
        exit(1);
    }

    // Set server address
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    serv_addr.sin_port = htons(PORT);

    // Connect to server
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("connect() error");
        exit(1);
    }

    while (1)
{
// Read message from user input
printf("Enter message to send to server (or type 'quit' to exit): ");
fgets(message, BUFSIZE, stdin);
message[strlen(message)-1] = '\0'; // Remove trailing newline
    // Send message to server
    if (write(sock, message, strlen(message)) == -1) {
        perror("write() error");
        exit(1);
    }

    // Check if user wants to quit
    if (strcmp(message, "quit") == 0) {
        printf("Quitting...\n");
        break;
    }

    // Receive echo message from server
    if ((str_len = read(sock, message, BUFSIZE)) == -1) {
        perror("read() error");
        exit(1);
    }

    message[str_len] = '\0';
    printf("Received echo message from server: %s\n", message);
}

close(sock);

return 0;
}