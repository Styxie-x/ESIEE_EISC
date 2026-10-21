/*
Un serveur proxy est un logiciel qui s’intercale entre un client et un serveur pour relayer des communications établies par le client vers le serveur. le proxy joue alors un double rôle : à la fois de serveur et de client. Nous allons mettre en œuvre un serveur proxy utilisant le protocole TCP.

Le programme que nous vous demandons de réaliser se lancera ainsi :

./tcproxy 4080 example.com 80
Cela signifie que le programme écoute sur toutes les interfaces et le port 4080 et a pour mission de relayer les communications vers le serveur example.com fonctionnant sur le port 80.

Le proxy doit donc réaliser les opérations suivantes :

D’abord ouvrir une socket TCP serveur attachée sur 0.0.0.0:4080
Ensuite d’écouter des connexions entrantes sur cette socket
On accepte chaque connexion entrante : on va alors créer une nouvelle socket TCP jouant maintenant le rôle de client et chargée de contacter le serveur distant (ici example.com sur le port 80). On pourra alors transférer les données bidirectionnellement :
Du client TCP vers le serveur TCP
Du serveur TCP vers le client TCP
Le transfert de données doit se réaliser en copiant des données d’un descripteur de fichier vers un autre.

Dans un premier temps, nous allons implémenter le serveur selon un mode itératif. Cela signifie que nous traitons qu’un seul client à la fois avec une seule thread. Après connexion vers le serveur final, on réceptionne les données du client et on les copie vers la socket connectée au serveur puis ensuite on réceptionne les données de la socket connectée au serveur et on les envoie au client et ainsi de suite…

Cette approche présente des limitations :

On ne peut communiquer qu’avec un seul client à la fois
On suppose que l’échange de données avec le serveur final est réalisé selon une séquence envoi, réception, envoi, réception… Si aprés l’envoi de données le serveur ne nous retourne rien, nous sommes bloqués. De même que si le client ne nous envoie rien à la connexion, nous ne pouvons rien envoyer au serveur et nous sommes également bloqués
Il faut donc utiliser des threads pour résoudre ce problème. Vous devrez donc mettre en place :

Une thread principale pour gérer l’acceptation des connexions sur le serveur. Vous n’avez pas besoin de la créer vous-même : il s’agit de la thread principale créée par défaut lorsque le programme est lancé
Pour chaque nouvelle connexion acceptée sur le serveur :
Une thread pour assurer la communication dans le sens client->proxy->serveur
Une thread pour assurer la communication dans le sens inverse serveur->proxy->client
Vous remarquerez que le code des deux threads de communication est le même (seul le sens de communication change). Vous pouvez donc créer une fonction qui prend en paramètre une structure contenant les descripteurs source et destination et lancer les deux threads appelant la même fonction mais en inversant les descripteurs.


./tcproxy 4080 example.com 80

*/
#define _POSIX_C_SOURCE 200112L
