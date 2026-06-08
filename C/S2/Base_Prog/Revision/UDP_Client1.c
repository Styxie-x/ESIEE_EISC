/*
Il s’agit maintenant d’implémenter un client interrogeant le serveur précédent.
La particularité de ce client est qu’il va se débrouiller comme un grand pour trouver
le nombre mystérieux du serveur. Pour cela nous suivre la méthodologie suivante :

Nous ouvrons et attachons tout d’abord une socket UDP (sur n’importe quel port)
Nous envoyons des paquets UDP vers le serveur : ces paquets doivent contenir des nombres
        entiers au format texte (ASCII) ; le serveur nous répond à chaque paquet par +, - ou =
        Plutôt que de proposer n’importe quel nombre, nous allons procéder par dichotomie,
        i.e. d’abord proposer 500, si le nombre mystérieux nous proposons 250, si le nombre
        mystérieux est plus grand nous proposons 375… et ainsi de suite jusqu`à trouver
        le nombre mystérieux
Nous programme client se lance ainsi (pour interroger le serveur sur localhost et le port 3210) :

./udpmystcli localhost 3210
Le client doit nous afficher toutes les tentatives réalisées… jusqu’à parvenir à
        trouver le nombre mystérieux !

*/

#define _POSIX_C_SOURCE 200112L

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <netdb.h>

#define min(x, y) (((x) < (y) ? (x) : (y)))

int read_line(char *dst, int dst_len)
{
    static char buffer[2048];
    static int pos;
    while (1)
    {
        if (pos == sizeof(buffer))
        {
            pos = 0; // remove the head of the line if the line is too long for the buffer
        }
        int r = read(0, buffer + pos, sizeof(buffer) - pos); // read on stdin
        // check that a \n is present on the data read
        for (int i = pos; i < pos + r; i++)
        {
            if (buffer[i] == '\n')
            {
                memcpy(dst, buffer, min(dst_len, i + 1));
                int len = pos + r - i - 1;
                memmove(buffer, buffer + i + 1, len);
                pos = len;
                return i + 1;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("please provide all arguments\n");
    }

    char *hostname = argv[1];
    char *port = argv[2]; // port number

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;    // to use IPv4 or IPv6
    hints.ai_socktype = SOCK_DGRAM; // to use UDP
    struct addrinfo *result;
    struct addrinfo *current_result;
    struct addrinfo kept_result;

    int found = 0;
    if (0 == getaddrinfo(hostname, port, &hints, &result))
    {
        for (struct addrinfo *current_result = result; current_result != NULL; current_result = current_result->ai_next)
        {
            if (current_result->ai_family == AF_INET)
            {
                memcpy(&kept_result, current_result, sizeof(struct addrinfo));
                freeaddrinfo(result);
                result = NULL;
                found = 1;
                break;
            }
        }
    }

    // create the socket and bind it
    int sock = socket(kept_result.ai_family, kept_result.ai_socktype, kept_result.ai_protocol);
    struct sockaddr_in bind_addr;
    memset(&bind_addr, 0, sizeof(bind_addr));
    bind_addr.sin_family = AF_INET;

    while (1)
    {
        // send one packet by line read on stdin
        char buffer[255];
        ssize_t r = read_line(buffer, sizeof(buffer));
        // send this line to the remote socket
        ssize_t sent = sendto(sock, buffer, r, 0, kept_result.ai_addr, kept_result.ai_addrlen);

        // wait for the reply of the other side
        ssize_t received = recvfrom(sock, buffer, sizeof(buffer), 0, NULL, NULL);
        // print the reply
        write(1, buffer, received);
    }

    close(sock);
    return 0;
}