#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BACKLOG 1
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

    if (listen(sock, BACKLOG) == -1)
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

