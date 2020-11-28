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

