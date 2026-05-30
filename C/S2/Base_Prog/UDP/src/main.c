#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>

const char * EXIT_KEYWORD = "EXIT";

int main(int argc, char * argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: udplenserver <interface> <port>\n");
        return -1;
    }
    char *interface = argv[1];
    uint16_t port = atoi(argv[2]); // port number
    if (port == 0) {
        fprintf(stderr, "Invalid port number\n");
        return -1;
    }
    
    // create the socket
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == 0) {
        fprintf(stderr, "Cannot create a socket socket\n");
        return -1;
    }

    // bind the socket
    struct sockaddr_in sa;
    sa.sin_family = AF_INET; // for an IPv4 address
    sa.sin_port = htons(port);
    if (inet_pton(AF_INET, interface, &(sa.sin_addr)) != 1) {
        fprintf(stderr, "Invalid interface\n");
        close(sock);
        return -1;
    }
    if (0 != bind(sock, (struct sockaddr *)&sa, sizeof(sa))) {
        fprintf(stderr, "Cannot bind the socket\n");
        switch (errno) {
            case EACCES:
                fprintf(stderr, "Cannot use the port (not an admin)\n");
                break;
            case EADDRINUSE:
                fprintf(stderr, "Port already used\n");
        }
        close(sock);
        return -1;
    }

    while (true) {
        struct sockaddr_in sender;
        socklen_t sender_len = sizeof(sender);
        char buffer[2048];
        // receive data on the socket
        ssize_t received = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&sender, &sender_len);
        if (received > 0) {
            char ip_str[128];
            if (inet_ntop(AF_INET, &sender.sin_addr, ip_str, sizeof(ip_str)) != NULL) {
                fprintf(stderr, "Received %d bytes from %s\n", received, ip_str);
            }

            // check if the received data in the buffer starts with EXIT

            if (0 == strncmp(buffer, EXIT_KEYWORD, strlen(EXIT_KEYWORD))) {
                fprintf(stderr, "Receiving EXIT, we close the server\n");
                break;
            }
            // send back data to the socket to the sender
            char data[128];
            int data_len = snprintf(data, sizeof(data), "length=%d\n", received);
            if (data_len != sendto(sock, data, data_len, 0, (struct sockaddr *)&sender, sender_len)) {
                fprintf(stderr, "Replying to the received packet failed\n");
            }
        }
    }

    close(sock);
    return 0;
}