/*
C’est un exercice classique pour les débutants en algorithmique :
      il s’agit de deviner un nombre mystérieux tiré au sort.
      Le joueur peut réaliser des propositions et on lui indiquera
      si celles-ci sont inférieures, supérieures ou égales au nombre
      mystérieux… et dans ce dernier cas le joueur a gagné.

Nous réalisons une adaptation de ce jeu traditionnel par
         l’intermédiaire d’un serveur UDP.
Celui-ci doit se lancer par la commande suivante :

./updmystint 3210
Cela signifie que le serveur est attaché sur le port 3210
         (et sur toutes les interfaces). Au démarrage,
         le serveur tire au sort un nombre entre 1 et 1000
         qui sera le même pour tous les joueurs.
         La fonction rand (man page) peut vous y aider.

Pour jouer, nous envoyons un nombre sous forme de texte
         (encodé en ASCII) en UDP vers la socket du serveur à
         l’aide d’un client UDP (netcat peut faire l’affaire).
         Le serveur nous répond par un paquet contenant un seul
         caractère ASCII :

+ si le nombre mystérieux est plus grand que la proposition
- si le nombre mystérieux est plus petit que la proposition
= si nous avons envoyé pile-poil la valeur du nombre mystérieux

Implémentez le serveur UDP demandé. Utilisez netcat pour envoyer
des paquets vers le serveur :

nc -u localhost 3210
./updmystint 3210

*/
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

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("please provide all arguments\n");
    }

    char *interface = "0.0.0.0";
    uint16_t port = atoi(argv[1]); // port number

    // create the socket
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    // bind the socket
    struct sockaddr_in sa;
    sa.sin_family = AF_INET; // for an IPv4 address
    sa.sin_port = htons(port);
    inet_pton(AF_INET, interface, &(sa.sin_addr));
    bind(sock, (struct sockaddr *)&sa, sizeof(sa));

    srand(time(NULL));
    int number_to_guess = rand() % 1000 + 1;
    printf("number generated is %d \n", number_to_guess);

    while (1)
    {
        struct sockaddr_in sender;
        socklen_t sender_len = sizeof(sender);
        char buffer[250];
        // receive data on the socket
        ssize_t received = recvfrom(sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&sender, &sender_len);
        buffer[received] = '\0';
        int guessed_number = atoi(buffer);

        char *senddata;
        char data[255];
        if (guessed_number > number_to_guess)
        {
            printf("number guessed is bigger then number to guess  \n");
            senddata = "+ \n";
        }
        if (guessed_number < number_to_guess)
        {
            printf("Number guessed is smaller the number to guess \n");
            senddata = "- \n";
        }

        if (guessed_number == number_to_guess)
        {
            printf("you guessed it correctly it's %d \n", number_to_guess);
            senddata = "= \n";
            number_to_guess = rand() % 1000 + 1;
            printf("number generated is %d \n", number_to_guess);
        }

        int data_len = snprintf(data, sizeof(data), "nombre est %s\n", senddata);
        sendto(sock, data, data_len, 0, (struct sockaddr *)&sender, sender_len);
    }

    close(sock);
    return 0;
}