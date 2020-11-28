#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#define closesocket(s) close(s)
#endif

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

#ifdef _WIN32
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        fprintf(stderr, "WSAStartup() failed\n");
        exit(EXIT_FAILURE);
    }
#endif

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;        /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM;    /* TCP */

    if ((rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo() failed\n");
        exit(EXIT_FAILURE);
    }

    // boucle sur les résultats, crée la socket et se connect
    for (ptr = servinfo; ptr != NULL; ptr = ptr->ai_next)
    {
        if ((sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol)) == -1)
        {
            fprintf(stderr, "socket() failed\n");
            continue;
        }

        if (connect(sock, ptr->ai_addr, ptr->ai_addrlen) == -1)
        {
            fprintf(stderr, "connect() failed\n");
            closesocket(sock);
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
        fprintf(stderr, "send() failed\n");
        exit(EXIT_FAILURE);
    }

    for (;;)
    {
        rv = recv(sock, buffer, 1024, 0);

        if (rv == -1)
        {
            fprintf(stderr, "send() failed\n");
            exit(EXIT_FAILURE);
        }

        if (rv == 0) break;

        buffer[rv] = 0;

        printf("%s", buffer);
    }

    closesocket(sock);

#if _WIN32
    WSACleanup();
#endif

    return 0;
}

