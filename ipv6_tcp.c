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

int create_socket()
{
    int sockfd = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    return sockfd;
}

void connect_server(char *ip, int port)
{
    int sockfd = create_socket(AF_INET6);
    struct sockaddr_in6 servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin6_family = AF_INET6;
    inet_pton(AF_INET6, ip, &(servaddr.sin6_addr));
    servaddr.sin6_port = htons(port);
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("connect");
        exit(EXIT_FAILURE);
    }
    printf("Connected to server at %s IPv6 on port %d using TCP\n", ip, port);

    // Open the file for reading
    FILE *fp;
    fp = fopen("100MB-File.c", "rb");
    if (fp == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    // Send the contents of the file to the server
    char buffer[MAX_BUFFER_SIZE];
    size_t bytes_read;
    printf("Starting to send the file\n");
    while ((bytes_read = fread(buffer, sizeof(char), MAX_BUFFER_SIZE, fp)) > 0)
    {
        int bytes_sent = send(sockfd, buffer, bytes_read, 0);
        if (bytes_sent < 0)
        {
            perror("send");
            exit(EXIT_FAILURE);
        }
    }

    printf("The entire file has been sent\n");
    printf("Closes the connection with the server at %s on port %d using TCP\n", ip, port);

    // Close the file and the socket
    fclose(fp);
    close(sockfd);
}

void start_server(int port)
{
    int sockfd = create_socket();
    struct sockaddr_in6 servaddr, cliaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin6_family = AF_INET6;
    servaddr.sin6_addr = in6addr_any;
    servaddr.sin6_port = htons(port);
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    if (listen(sockfd, 1) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("Listening on port %d (IPv6, TCP)\n", port);
    socklen_t cliaddrlen = sizeof(cliaddr);
    int connfd = accept(sockfd, (struct sockaddr *)&cliaddr, &cliaddrlen);
    if (connfd < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    char buffer[MAX_BUFFER_SIZE];

    fd_set set;
    FD_ZERO(&set);

    while (1)
    {
        FD_SET(STDIN_FILENO, &set);
        FD_SET(connfd, &set);
        int max_fd = connfd > STDIN_FILENO ? connfd : STDIN_FILENO;
        select(max_fd + 1, &set, NULL, NULL, NULL);
        if (FD_ISSET(connfd, &set))
        {
            int bytes_recv = recv(connfd, buffer, MAX_BUFFER_SIZE, 0);
            if (bytes_recv < 0)
            {
                perror("recv");
                exit(EXIT_FAILURE);
            }
            else if (bytes_recv == 0)
            {
                printf("Client disconnected\n");
                exit(EXIT_SUCCESS);
            }
        }
        if (FD_ISSET(STDIN_FILENO, &set))
        {
            if (fgets(buffer, MAX_BUFFER_SIZE, stdin) != NULL)
            {
                int bytes_sent = send(connfd, buffer, strlen(buffer), 0);
                if (bytes_sent < 0)
                {
                    perror("send");
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