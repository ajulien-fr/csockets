---
title: "Sockets en C"
slug: "c-sockets"
description: "Article sur les sockets en C"
tags: ["c"]
categories: ["coding", "networking"]
---

# Les sockets en C

## Qu'est ce qu'une socket ?

Une **socket** est un **endpoint** ; un point de terminaison permettant la communication entre deux processus.

Sur un réseau, une **socket**, c'est donc un **endpoint** pour envoyer ou recevoir des données permettant ainsi des interactions entre le **client** et le **serveur**.

Le serveur étant le poste informatique qui fournit le **service**, comme par exemple le service **http** qui se trouve par convention sur le port **80**, et le client étant le poste qui va consommer ou utiliser le service.

Pour pouvoir communiquer entre eux, une socket est créée sur le serveur, celle ci est configurée de façon à **écouter** sur un port, donc à attendre les connexions sur ce port.

Sur le client, la socket est créée et utilisée pour se **connecter** au port du service. La connexion se fait en configurant la socket avec l'**IP** du serveur ainsi que le **port** du service sur lequel se connecter.

Ensuite l'échange, le dialogue ou la communication entre le client et le serveur sur le réseau se fait par **paquet** au moyen d'une suite de **protocole** comme **TCP** et **UDP** / **IP**.

## Qu'est ce qu'un protocole ?

Dans l'informatique, un protocole est un ensemble de règles standard permettant de communiquer.

Internet repose principalement sur les protocoles **TCP/IP** et **UDP/IP**.

## Qu'est ce que le protocole IP

**IP** pour **Internet Protocol** est le protocole de **routage** se chargeant de l'acheminement des paquets.

**Le routage** c'est le moyen avec l'aide d'algorithme de **trouver pour la source la meilleure route pour accéder à la destination**.

Pour plus d'information sur le protocole **IP**, vous pouvez consulter la **RFC 791** : [https://www.rfc-editor.org/rfc/rfc791.html](https://www.rfc-editor.org/rfc/rfc791.html)

## Qu'est ce que le protocole TCP

**TCP** pour **Transmission Control Protocol** fournit tout en s'appuyant sur le protocole **IP** un service de remise des paquets de façon **contrôlé**, donc **fiable**.

**TCP** se charge entre autre de l'établissement et de la libération d'une connexion, du transfert de données, de la corrections d'erreurs et du contrôle de ses données par un système d'accusé de reception, un système d'acknowledgement / d'acquittement.

La **RFC 973** concernant le protocole **TCP** se trouve ici : [https://www.rfc-editor.org/rfc/rfc793.html](https://www.rfc-editor.org/rfc/rfc793.html)

## Qu'est ce que le protocole UDP

**UDP** qui signifi **User Datagram Protocol** et qui s'appui également sur le protocole **IP** permet la remise de paquet de façon **non contrôlé** et donc **non fiable**.

Par exemple, il ne garantit, ni la remise, ni l'ordre des paquets délivrés et il ne procède pas à de contrôle sur les données échangées.

Vous pouvez consulter la **RFC 768** ici : [https://www.rfc-editor.org/rfc/rfc768.html](https://www.rfc-editor.org/rfc/rfc768.html)

## IPv4 && IPv6

**IPv4** et **IPv6** sont deux versions différentes du protocole **IP**.

Sur la **version 4** du protocole, on utilise des adresses IP codée sur 32 bits.

Une **adresse IPv4** est **composée de quatre octets**.
Les octets sont séparés par des **points** et chacun d'eux a sa valeur comprise entre **0 et 255**. Exemple d'adresse IPv4 : **87.98.154.146**

Pour la **version 6** du protocole on utilise des adresses IP codée sur 128 bits. Exemple d'adresse IPv6 : **2001:41d0:1:c4d1::1**

## Les structures de données

La première structure à connaitre se nomme **struct addrinfo** : [https://www.man7.org/linux/man-pages/man3/getaddrinfo.3.html](https://www.man7.org/linux/man-pages/man3/getaddrinfo.3.html)

```c
struct addrinfo {
    int              ai_flags;
    int              ai_family; // AF_INET, AF_INET6 or AF_UNSPEC
    int              ai_socktype; // SOCK_STREAM or SOCK_DGRAM
    int              ai_protocol; // 0 (auto), or IPPROTO_TCP, IPPROTO_UDP
    socklen_t        ai_addrlen;
    struct sockaddr *ai_addr;
    char            *ai_canonname;
    struct addrinfo *ai_next;
};
```

La deuxième concerne uniquement l'**IPv4** et elle se nomme **struct sockaddr_in** : [https://linux.die.net/man/7/ip](https://linux.die.net/man/7/ip)

```c
struct sockaddr_in {
        short int          sin_family;  // Famille d'adresse, AF_INET
        unsigned short int sin_port;    // Numéro du port
        struct in_addr     sin_addr;    // Adresse internet IPv4
};

struct in_addr {
        uint32_t s_addr; // Adresse IPv4 en 32-bit unsigned int
};
```

La troisième, c'est l'inverse, elle concerne uniquement le protocole IPv6 : [https://linux.die.net/man/7/ipv6](https://linux.die.net/man/7/ipv6)

```c
struct sockaddr_in6 {
        u_int16_t       sin6_family;   // Famille d'adresse, AF_INET6
        u_int16_t       sin6_port;     // Numéro du port
        u_int32_t       sin6_flowinfo; // IPv6 flow information
        struct in6_addr sin6_addr;     // Adresse internet IPv6
        u_int32_t       sin6_scope_id; // Scope ID
};
    
struct in6_addr {
        unsigned char   s6_addr[16];   // Adresse IPv6
};
```

Ces deux structures : **sockaddr_in6** et **sockaddr_in** peuvent être converties par **cast** en **struct sockaddr**.
Une structure **struct sockaddr** peut être convertie en **struct sockaddr_in** ou **sockaddr_in6** :

```c
struct sockaddr {
    unsigned short    sa_family;    // Famille d'adresse
    // PADDING
};
```

La dernière structure est utilisée lorsque l'on ne connait pas en avance la version du protocole utilisé par une socket
Elle peut contenir les informations d'adresse IPv4 et d'adresse IPv6 : **struct sockaddr_storage**

```c
struct sockaddr_storage {
    sa_family_t  ss_family;
    // PADDING
}
```

## Création d'une socket

Voici la fonction permettant de créer une socket : **socket()** : ([https://www.man7.org/linux/man-pages/man2/socket.2.html](https://www.man7.org/linux/man-pages/man2/socket.2.html)) :

```c
int socket(int domain, int type, int protocol);
```

Pour de **l'IPv4** et le protocole **TCP**, on utilisera :

```c
int sock = socket(AF_INET, SOCK_STREAM, 0);
```

Et pour **l'IPv4** et le protocole **UDP**, ce sera :

```c
int sock = socket(AF_INET, SOCK_DGRAM, 0);
```

Pour de **l'IPv6**, il suffi de **changer AF_INET en AF_INET6**.

## Comment convertir un nom de domaine en IP

Pour convertir un nom de domaine en IP et pouvoir s'y connecter, il faut utiliser la fonction **getaddrinfo()** : [https://www.man7.org/linux/man-pages/man3/getaddrinfo.3.html](https://www.man7.org/linux/man-pages/man3/getaddrinfo.3.html) :

```c
int getaddrinfo(const char *nodename, const char *servname,
                const struct addrinfo *hints, struct addrinfo **res);
```

Pour convertir un nom de demaine en IP, elle s'utilise de cette façon :

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    struct addrinfo hints;
    struct addrinfo *result, *ptr;
    char ipstr[INET6_ADDRSTRLEN];

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <domain>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;        /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM;    /* TCP */

    if (getaddrinfo(argv[1], NULL, &hints, &result) != 0)
    {
        fprintf(stderr, "getaddrinfo() failed : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("Adresses IP pour : %s\n", argv[1]);

    for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
    {
        void *addr;
        char *ipver;

        if (ptr->ai_family == AF_INET)
        { // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)ptr->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        }

        else
        { // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)ptr->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        // Converti l'IP en string
        inet_ntop(ptr->ai_family, addr, ipstr, INET6_ADDRSTRLEN);

        // Affiche l'IP
        printf("\t %s: %s\n", ipver, ipstr);
    }

    freeaddrinfo(result);

    return 0;
}
```

Ce qui sur google.fr, nous donnes :

<img src="/screenshot-socket-getaddrinfo.png" class="d-block mx-auto">

## La connexion

Pour réaliser cette connexion, c'est la fonction : **connect()** : [https://man7.org/linux/man-pages/man2/connect.2.html](https://man7.org/linux/man-pages/man2/connect.2.html) :

```c
int connect(int sockfd, const struct sockaddr *addr,
            socklen_t addrlen);
```

Pour envoyer des données sur cette socket, ce sera la fonction : **send()** : [https://man7.org/linux/man-pages/man2/send.2.html](https://man7.org/linux/man-pages/man2/send.2.html) :

```c
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
```

Puis, pour recevoir des données de cette socket : **recv()** : [https://man7.org/linux/man-pages/man2/recv.2.html](https://man7.org/linux/man-pages/man2/recv.2.html) :

```c
ssize_t recv(int sockfd, void *buf, size_t len, int flags);
```

Pour démontrer ces trois fonctions, je vais écrire du code permettant de se connecter et de s'enregistrer sur un serveur **IRC**.

Voici la **RFC** du protocole **IRC** : [https://www.rfc-editor.org/rfc/rfc1459.html](https://www.rfc-editor.org/rfc/rfc1459.html)

Cette **RFC** nous dit entre autre qu'un **message IRC** est une ligne qui ne peut pas dépasser **512 charactères** et qui se termine par un **CR-LF**.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    struct addrinfo hints;
    int rv;
    struct addrinfo *servinfo, *ptr;
    int sock;
    char buffer[1024 + 1];

    if (argc != 4)
    {
        fprintf(stderr, "usage: %s <domain> <port> <nickname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;        /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM;    /* TCP */

    if ((rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo() failed : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // boucle sur les résultats, crée la socket et se connect
    for (ptr = servinfo; ptr != NULL; ptr = ptr->ai_next)
    {
        if ((sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol)) == -1)
        {
            fprintf(stderr, "socket() failed : %s\n", strerror(errno));
            continue;
        }

        if (connect(sock, ptr->ai_addr, ptr->ai_addrlen) == -1)
        {
            fprintf(stderr, "connect() failed : %s\n", strerror(errno));
            close(sock);
            continue;
        }

        break;
    }

    if (ptr == NULL)
    {
        fprintf(stderr, "info: unable to connect\n");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(servinfo);

    printf("info: connected !\n");

    sprintf(buffer, "NICK %s\r\nUSER %s 0 * :%s\r\n", argv[3], argv[3], argv[3]);

    if (send(sock, buffer, strlen(buffer), 0) == -1)
    {
        fprintf(stderr, "send() failed : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    for (;;)
    {
        rv = recv(sock, buffer, 1024, 0);

        if (rv == -1)
        {
            fprintf(stderr, "send() failed : %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        if (rv == 0) break;

        buffer[rv] = 0;

        printf("%s", buffer);
    }

    close(sock);

    return 0;
}
```

Ce code permet donc de se connecter sur le serveur IRC dont l'adresse est donnée en argument numéro un et le port en argument numéro deux.
Il permet également de s'enregistrer sur ce serveur IRC en tant qu'utilisateur dont le pseudonyme est donné en argument numéro trois.
Après être connecté et enregistré sur le seveur IRC, ce code affiche ce que ce serveur lui envoit.

```text
opc0de@lapt0p:~/src/sockets$ gcc socket-connect.c -o socket-connect -Wall
opc0de@lapt0p:~/src/sockets$ ./socket-connect irc.freenode.org 6667 opb0t
info: connected !
:adams.freenode.net NOTICE * :*** Looking up your hostname...
:adams.freenode.net NOTICE * :*** Checking Ident
:adams.freenode.net NOTICE * :*** Couldn't look up your hostname
:adams.freenode.net NOTICE * :*** No Ident response
:adams.freenode.net 001 opb0t :Welcome to the freenode Internet Relay Chat Network opb0t
~~~ bla bla bla ~~~
:adams.freenode.net 376 opb0t :End of /MOTD command.
:opb0t MODE opb0t :+i
PING :adams.freenode.net
:opb0t!~opb0t@2a01:e34:eea7:ea20:f355:3b98:1c4f:c99c QUIT :Ping timeout: 272 seconds
ERROR :Closing Link: 2a01:e34:eea7:ea20:f355:3b98:1c4f:c99c (Ping timeout: 272 seconds)
```

Je transforme donc le nom de domaine **"irc.freenode.org"** en IP. Je boucle sur ces IP, et à chaque tour, je crée la socket et tente de me connecter sur le port **"6667"**.
Une fois connecté, je m'enregistre sur le serveur IRC en tant qu'utilisateur **"opb0t"**.

Tout ce que je reçois de la socket est affiché sur la sortie standard de la console.

Pour fermer proprement la socket j'utilise la fonction **close()** : [https://man7.org/linux/man-pages/man2/close.2.html](https://man7.org/linux/man-pages/man2/close.2.html)

Dans l'output affiché dans la console, nous trouvons cette ligne intéressante : **PING :adams.freenode.net**.

Cette ligne correspond en fait à quelque chose de bien connu en réseau.

Lorsqu'un serveur veut être certain de la situation de la socket, de la connexion avec un client, il **PING** ce client et attend durant un certain temps de recevoir le **PONG** qui va avec.
Si le serveur ne reçoit pas ce **PONG** comme dans notre cas, le serveur considère la socket comme non valide et cette socket se retrouve fermée.

Pour rester connecter à ce serveur irc, le client aurait du envoyer **"PONG :adams.freenode.net"** en retour au ping.

## L'écoute

Pour recevoir et accepter des connexions sur un port donné, il faut utiliser les fonctions **bind()** : [https://man7.org/linux/man-pages/man2/bind.2.html](https://man7.org/linux/man-pages/man2/bind.2.html), **listen()** : [https://man7.org/linux/man-pages/man2/listen.2.html](https://man7.org/linux/man-pages/man2/listen.2.html) et **accept()** : [https://man7.org/linux/man-pages/man2/accept.2.html](https://man7.org/linux/man-pages/man2/accept.2.html) :

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MESSAGE "Hello World !\n"

int main(int argc, char *argv[])
{
    struct addrinfo hints;
    struct addrinfo *servinfo, *ptr;
    int rv;
    int sock, new_sock;
    int yes = 1;
    struct sockaddr_storage their_addr;
    socklen_t addr_size;

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;        /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM;    /* TCP */
    hints.ai_flags = AI_PASSIVE;        /* For wildcard IP address */

    if ((rv = getaddrinfo(NULL, argv[1], &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo() failed : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // boucle sur les résultats pour s'y "bind"
    for (ptr = servinfo; ptr != NULL; ptr = ptr->ai_next)
    {
        if ((sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol)) == -1)
        {
            fprintf(stderr, "socket() failed : %s\n", strerror(errno));
            continue;
        }

        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        {
            fprintf(stderr, "setsockopt() failed : %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        if (bind(sock, ptr->ai_addr, ptr->ai_addrlen) == -1)
        {
            fprintf(stderr, "bind() failed : %s\n", strerror(errno));
            close(sock);
            continue;
        }

        break;
    }

    if (ptr == NULL)
    {
        fprintf(stderr, "info: unable to bind\n");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(servinfo);

    if (listen(sock, SOMAXCONN) == -1)
    {
        fprintf(stderr, "listen() failed : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("info: listening !\n");

    addr_size = sizeof(struct sockaddr_storage);
    
    if ((new_sock = accept(sock, (struct sockaddr *)&their_addr, &addr_size)) == -1)
    {
        fprintf(stderr, "accept() failed : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (send(new_sock, MESSAGE, strlen(MESSAGE), 0) == -1)
    {
        fprintf(stderr, "send() failed : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // traitement : envoi et reception

    close(new_sock);
    close(sock);
    
    return 0;
}
```

Ce code compilé et exécuté nous donne :

<img src="/screenshot-socket-listen.png" class="d-block mx-auto">

Pour m'y connecter je vais utiliser l'outil **telnet** :

<img src="/screenshot-socket-listen-telnet.png" class="d-block mx-auto">

Je me connecte en [telnet](https://fr.wikipedia.org/wiki/Telnet) sur l'IPv4 **127.0.0.1** et le port **8080**.
Je recois bien le MESSAGE : **"Hello World !\n"** envoyé depuis le serveur.

Le client telnet se ferme ensuite car côté serveur, une fois le MESSAGE envoyé, on close les deux sockets et on quitte.

<img src="/screenshot-socket-listen-2.png" class="d-block mx-auto">

On imagine donc un quelconque traitement entre le client et le serveur sur new_sock à base de **send()** et de **recv()**, puis **d'analyse ou de "parsing"** sur les données échangées.

Dans cet exemple, sur le serveur, la fonction **accept()** est dîtes **"bloquante"**. Le programme s'arrête donc sur cette fonction jusqu'a ce qu'un client se connecte.

Le serveur accepte donc une seule connexion, dialogue avec le client depuis cette connexion (new_sock), puis lorsque ce dialogue prend fin sur "new_sock", le serveur s'arrête. Il n'est donc plus possible de s'y connecter.

Il est parfois nécessaire de continuer à attendre une nouvelle connexion voir d'accepter plusieurs clients simultanéments.

## Les sockets et le Multithreading

La programmation multithread est une technique utilisée pour permettre de boucler sur la fonction **accept()** et de traiter en même temps les connexions avec les clients.

Dès qu'un client se connecte au serveur, un **nouveau thread** est donc créé permettant les interactions entre le serveur et ce client.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define MESSAGE "Hello World !\n"

void * thread_start(void *arg);

int main(int argc, char *argv[])
{
    struct addrinfo hints;
    struct addrinfo *servinfo, *ptr;
    int rv;
    int sock, new_sock;
    int yes = 1;
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    pthread_t thread_id;

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;        /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM;    /* TCP */
    hints.ai_flags = AI_PASSIVE;        /* For wildcard IP address */

    if ((rv = getaddrinfo(NULL, argv[1], &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo() failed : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // boucle sur les résultats pour s'y "bind"
    for (ptr = servinfo; ptr != NULL; ptr = ptr->ai_next)
    {
        if ((sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol)) == -1)
        {
            fprintf(stderr, "socket() failed : %s\n", strerror(errno));
            continue;
        }

        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        {
            fprintf(stderr, "setsockopt() failed : %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        if (bind(sock, ptr->ai_addr, ptr->ai_addrlen) == -1)
        {
            fprintf(stderr, "bind() failed : %s\n", strerror(errno));
            close(sock);
            continue;
        }

        break;
    }

    if (ptr == NULL)
    {
        fprintf(stderr, "info: unable to bind\n");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(servinfo);

    if (listen(sock, SOMAXCONN) == -1)
    {
        fprintf(stderr, "listen() failed : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("info: listening !\n");

    for (;;)
    {
        addr_size = sizeof(struct sockaddr_storage);
    
        if ((new_sock = accept(sock, (struct sockaddr *)&their_addr, &addr_size)) == -1)
        {
            fprintf(stderr, "accept() failed : %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        if (pthread_create(&thread_id, NULL, thread_start, &new_sock) != 0)
        {
            fprintf(stderr, "pthread_create() failed : %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    close(sock);

    return 0;
}

void * thread_start(void *arg)
{
    int sock = *((int *)arg);

    if (send(sock, MESSAGE, strlen(MESSAGE), 0) == -1)
    {
        fprintf(stderr, "send() failed : %s\n", strerror(errno));
        close(sock);
        return NULL;
    }
    
    usleep(10000000); // traitement, send(), recv(), ...

    close(sock);

    return NULL;
}
```

Je compile et je run. J'utilise les touches CTRL+C pour fermer le serveur :

<img src="/screenshot-c-sockets-listen-pthread.png" class="d-block mx-auto">

Je m'y connecte trois fois avec **telnet** de cette façon :

<img src="/screenshot-c-sockets-pthread-telnet.png" class="d-block mx-auto">

Plusieurs clients (ici trois comme exemple) peuvent donc être connectés au serveur simultanéments. Aussi, le serveur reste en écoute sur le port 8080 jusqu'au **CTRL+C**.

## Les sockets et la fonction select()

Tout comme la fonction **accept()**, la fonction **recv()** est dîte **"bloquante"**.
Ce qui signifie qu'à l'appel de la fonction **recv()** dans le programme, si il n'y a pas de données à recevoir de la socket, ce programme se retrouve alors bloqué (jusqu'à recevoir des données).

Durant ce temps d'attente, le programme se retrouve donc bloqué.

Il est donc parfois nécessaire de savoir en avance s'il y a ou non des données à recevoir. Ceci ce fait grâce à la fonction **select()** : [https://www.man7.org/linux/man-pages/man2/select.2.html](https://www.man7.org/linux/man-pages/man2/select.2.html)

Nous l'utilisons alors de cette façon :

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    struct addrinfo hints;
    int rv;
    struct addrinfo *servinfo, *ptr;
    int sock;
    char buffer[1024 + 1];
    fd_set rfds;
    struct timeval tv;
    int retval;

    if (argc != 4)
    {
        fprintf(stderr, "usage: %s <domain> <port> <nickname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;        /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM;    /* TCP */

    if ((rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo() failed : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // boucle sur les résultats, crée la socket et se connect
    for (ptr = servinfo; ptr != NULL; ptr = ptr->ai_next)
    {
        if ((sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol)) == -1)
        {
            fprintf(stderr, "socket() failed : %s\n", strerror(errno));
            continue;
        }

        if (connect(sock, ptr->ai_addr, ptr->ai_addrlen) == -1)
        {
            fprintf(stderr, "connect() failed : %s\n", strerror(errno));
            close(sock);
            continue;
        }

        break;
    }

    if (ptr == NULL)
    {
        fprintf(stderr, "info: unable to connect\n");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(servinfo);

    printf("info: connected !\n");

    sprintf(buffer, "NICK %s\r\nUSER %s 0 * :%s\r\n", argv[3], argv[3], argv[3]);

    if (send(sock, buffer, strlen(buffer), 0) == -1)
    {
        fprintf(stderr, "send() failed : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    for (;;)
    {
        FD_ZERO(&rfds);
        FD_SET(sock, &rfds);

        tv.tv_sec = 8;
        tv.tv_usec = 0;

        retval = select(sock + 1, &rfds, NULL, NULL, &tv);

        if (retval < 0)
        {
            fprintf(stderr, "select() failed : %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        if (retval > 0)
        {
            rv = recv(sock, buffer, 1024, 0);

            if (rv == -1)
            {
                fprintf(stderr, "recv() failed : %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }

            buffer[rv] = 0;

            printf("%s", buffer);
        }

        else
        {
            printf("info: aucune donnée dans la socket\n");
        }
    }

    close(sock);

    return 0;
}
```

Compilation et exécution de ce code :

```text
opc0de@lapt0p:~/src/csockets$ gcc socket-select.c -o socket-select -Wall
opc0de@lapt0p:~/src/csockets$ ./socket-select 
usage: ./socket-select <domain> <port> <nickname>
opc0de@lapt0p:~/src/csockets$ ./socket-select irc.freenode.org 6667 opb0t
info: connected !
:adams.freenode.net NOTICE * :*** Looking up your hostname...
:adams.freenode.net NOTICE * :*** Checking Ident
:adams.freenode.net NOTICE * :*** Couldn't look up your hostname
:adams.freenode.net NOTICE * :*** No Ident response
:adams.freenode.net 001 opb0t :Welcome to the freenode Internet Relay Chat Network opb0t
~~~ bla bla bla ~~~
:adams.freenode.net 376 opb0t :End of /MOTD command.
:opb0t MODE opb0t :+i
info: aucune donnée dans la socket
info: aucune donnée dans la socket
^C
opc0de@lapt0p:~/src/csockets$ 
```

## Les sockets et la fonction poll()

La fonction **poll()** permet de faire différemment la même chose que la fonction **select()** :

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>

int main(int argc, char *argv[])
{
    struct addrinfo hints;
    int rv;
    struct addrinfo *servinfo, *ptr;
    int sock;
    char buffer[1024 + 1];
    struct pollfd pfds[1];

    if (argc != 4)
    {
        fprintf(stderr, "usage: %s <domain> <port> <nickname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;        /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM;    /* TCP */

    if ((rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo() failed : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // boucle sur les résultats, crée la socket et se connect
    for (ptr = servinfo; ptr != NULL; ptr = ptr->ai_next)
    {
        if ((sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol)) == -1)
        {
            fprintf(stderr, "socket() failed : %s\n", strerror(errno));
            continue;
        }

        if (connect(sock, ptr->ai_addr, ptr->ai_addrlen) == -1)
        {
            fprintf(stderr, "connect() failed : %s\n", strerror(errno));
            close(sock);
            continue;
        }

        break;
    }

    if (ptr == NULL)
    {
        fprintf(stderr, "info: unable to connect\n");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(servinfo);

    printf("info: connected !\n");

    sprintf(buffer, "NICK %s\r\nUSER %s 0 * :%s\r\n", argv[3], argv[3], argv[3]);

    if (send(sock, buffer, strlen(buffer), 0) == -1)
    {
        fprintf(stderr, "send() failed : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    pfds[0].fd = sock;
    pfds[0].events = POLLIN;

    for (;;)
    {
        rv = poll(pfds, 1, 8000);

        if (rv == -1)
        {
            fprintf(stderr, "poll() failed : %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        if (rv > 0)
        {
            rv = recv(sock, buffer, 1024, 0);

            if (rv == -1)
            {
                fprintf(stderr, "recv() failed : %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }

            buffer[rv] = 0;

            printf("%s", buffer);
        }

        else
        {
            printf("info: aucune données dans la socket\n");
        }
    }

    close(sock);

    return 0;
}
```

Nous obtenons bien la même chose :

```text
opc0de@lapt0p:~/src/csockets$ gcc socket-poll.c -o socket-poll -Wall
opc0de@lapt0p:~/src/csockets$ ./socket-poll irc.freenode.org 6667 opb0t
info: connected !
:adams.freenode.net NOTICE * :*** Looking up your hostname...
:adams.freenode.net NOTICE * :*** Checking Ident
:adams.freenode.net NOTICE * :*** Couldn't look up your hostname
~~~ bla bla bla ~~~
:adams.freenode.net 376 opb0t :End of /MOTD command.
:opb0t MODE opb0t :+i
info: aucune données dans la socket
^C
```

## Les sockets non bloquante

Comme nous l'avons vu les fonctions **recv()**, **accept()** sont des fonctions bloquante.
Ceci est le comportement par défaut de ces fonctions.

La fonction **fcntl()** : [https://man7.org/linux/man-pages/man2/fcntl.2.html](https://man7.org/linux/man-pages/man2/fcntl.2.html) permet de modifier ce comportement.
Avec cette fonction, il est possible de rendre ce genre de fonction **non-bloquante**.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    struct addrinfo hints;
    int rv;
    struct addrinfo *servinfo, *ptr;
    int sock;
    char buffer[1024 + 1];

    if (argc != 4)
    {
        fprintf(stderr, "usage: %s <domain> <port> <nickname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;        /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM;    /* TCP */

    if ((rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo() failed : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // boucle sur les résultats, crée la socket et se connect
    for (ptr = servinfo; ptr != NULL; ptr = ptr->ai_next)
    {
        if ((sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol)) == -1)
        {
            fprintf(stderr, "socket() failed : %s\n", strerror(errno));
            continue;
        }

        if (connect(sock, ptr->ai_addr, ptr->ai_addrlen) == -1)
        {
            fprintf(stderr, "connect() failed : %s\n", strerror(errno));
            close(sock);
            continue;
        }

        break;
    }

    if (ptr == NULL)
    {
        fprintf(stderr, "info: unable to connect\n");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(servinfo);

    printf("info: connected !\n");

    if (fcntl(sock, F_SETFL, O_NONBLOCK) == -1)
    {
        fprintf(stderr, "fcntl() failed : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    sprintf(buffer, "NICK %s\r\nUSER %s 0 * :%s\r\n", argv[3], argv[3], argv[3]);

    if (send(sock, buffer, strlen(buffer), 0) == -1)
    {
        fprintf(stderr, "send() failed : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    for (;;)
    {
        rv = recv(sock, buffer, 1024, 0);

        if (rv == -1)
        {
            if (errno == EAGAIN)
            {
                printf("info: aucune donnée dans la socket pour cette fois ci\n");
                sleep(4);
            }

            else
            {
                fprintf(stderr, "recv() failed : %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
        }

        else if (rv == 0) break;

        else
        {
            buffer[rv] = 0;
            printf("%s", buffer);
        }
    }

    close(sock);

    return 0;
}
```

Je compile et je run :

```text
opc0de@lapt0p:~/src/csockets$ gcc socket-nonblock.c -o socket-nonblock -Wall
opc0de@lapt0p:~/src/csockets$ ./socket-nonblock irc.freenode.org 6667 opb0t
info: connected !
info: aucune donnée dans la socket pour cette fois ci
:adams.freenode.net NOTICE * :*** Looking up your hostname...
:adams.freenode.net NOTICE * :*** Checking Ident
:adams.freenode.net NOTICE * :*** Couldn't look up your hostname
~~~ bla bla bla ~~~
info: aucune donnée dans la socket pour cette fois ci
info: aucune donnée dans la socket pour cette fois ci
:opb0t!~opb0t@2a01:e34:eea7:ea20:d10d:d543:8357:3a1d QUIT :Ping timeout: 272 seconds
ERROR :Closing Link: 2a01:e34:eea7:ea20:d10d:d543:8357:3a1d (Ping timeout: 272 seconds)
```

