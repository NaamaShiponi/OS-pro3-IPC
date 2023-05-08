#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

#define MAX_BUFFER_SIZE 1024

int create_socket() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    return sockfd;
}

void connect_server(char *ip, int port)
{
    int sockfd = create_socket();
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &(servaddr.sin_addr));
    servaddr.sin_port = htons(port);
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("connect");
        exit(EXIT_FAILURE);
    }
    printf("Connected to server at %s:%d (%s, %s)\n", ip, port, "ipv4", "udp");

    char buffer[MAX_BUFFER_SIZE];
    fd_set set;
    FD_ZERO(&set);
    while (1)
    {
        FD_SET(STDIN_FILENO, &set);
        FD_SET(sockfd, &set);
        int max_fd = sockfd > STDIN_FILENO ? sockfd : STDIN_FILENO;
        select(max_fd + 1, &set, NULL, NULL, NULL);
        if (FD_ISSET(sockfd, &set))
        {
            int bytes_recv = recv(sockfd, buffer, MAX_BUFFER_SIZE, 0);
            if (bytes_recv < 0)
            {
                perror("recv");
                exit(EXIT_FAILURE);
            }
            else if (bytes_recv == 0)
            {
                printf("Server disconnected\n");
                exit(EXIT_FAILURE);
            }
            buffer[bytes_recv] = '\0';
            printf("anonymous: %s", buffer);
        }
        if (FD_ISSET(STDIN_FILENO, &set))
        {
            if (fgets(buffer, MAX_BUFFER_SIZE, stdin) != NULL)
            {
                int bytes_sent = send(sockfd, buffer, strlen(buffer), 0);
                if (bytes_sent < 0)
                {
                    perror("send");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
}

void start_server(int port)
{
    int sockfd = create_socket();
    struct sockaddr_in servaddr, cliaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    printf("Listening on port %d (%s, %s)\n", port, "ipv4", "udp");

    socklen_t cliaddrlen = sizeof(cliaddr);

    char buffer[MAX_BUFFER_SIZE];
    fd_set set;
    FD_ZERO(&set);

    while (1)
    {
        FD_SET(STDIN_FILENO, &set);
        FD_SET(sockfd, &set);
        int max_fd = sockfd > STDIN_FILENO ? sockfd : STDIN_FILENO;
        select(max_fd + 1, &set, NULL, NULL, NULL);
        if (FD_ISSET(sockfd, &set))
        {
            int bytes_recv = recvfrom(sockfd, buffer, MAX_BUFFER_SIZE, 0,
                                      (struct sockaddr *)&cliaddr, &cliaddrlen);
            if (bytes_recv < 0)
            {
                perror("recvfrom");
                exit(EXIT_FAILURE);
            }
            buffer[bytes_recv] = '\0';
            printf("%s:%d: %s", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port), buffer);
        }
        if (FD_ISSET(STDIN_FILENO, &set))
        {
            if (fgets(buffer, MAX_BUFFER_SIZE, stdin) != NULL)
            {
                int bytes_sent = sendto(sockfd, buffer, strlen(buffer), 0,
                                        (struct sockaddr *)&cliaddr, sizeof(cliaddr));
                if (bytes_sent < 0)
                {
                    perror("sendto");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s [-c IP PORT] | [-s PORT]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int c;
    char *ip = NULL;
    int port = 0;
    while ((c = getopt(argc, argv, "c:s:")) != -1)
    {
        switch (c)
        {
        case 'c':
            ip = argv[2];
            port = atoi(argv[3]);
            connect_server(ip, port);
            break;
        case 's':
            port = atoi(argv[2]);
            start_server(port);
            break;
        default:
            printf("Usage: %s [-c IP PORT] | [-s PORT]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}