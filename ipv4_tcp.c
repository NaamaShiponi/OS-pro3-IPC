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
// #include "tcp_time.c"
#include "stnc.h"

#define MAX_BUFFER_SIZE 1024

int create_socket_ipv4_tcp()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    return sockfd;
}

void handle_client_ipv4_tcp(char *ip, int port)
{
    int sockfd = create_socket_ipv4_tcp();
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(ip);
    servaddr.sin_port = htons(port);
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("connect");
        exit(EXIT_FAILURE);
    }
    printf("Connected to server at %s IPv4 on port %d using TCP\n", ip, port);
    
    // Open the file and read its contents
    FILE* file = fopen("100MB-File.txt", "rb");
    if (!file) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    char buffer[MAX_BUFFER_SIZE];
    size_t bytes_read = fread(buffer, sizeof(char), MAX_BUFFER_SIZE, file);

    // Send the file in chunks of MAX_BUFFER_SIZE bytes
    printf("Starting to send the file\n");
    send_start();
    while (bytes_read > 0) {
        int bytes_sent = send(sockfd, buffer, bytes_read, 0);
        if (bytes_sent < 0)
        {
            perror("send");
            exit(EXIT_FAILURE);
        }
        bytes_read = fread(buffer, sizeof(char), MAX_BUFFER_SIZE, file);
    }
    send_stop();
    printf("The entire file has been sent\n");
    printf("Closes the connection with the server at %s on port %d using TCP\n", ip, port);

    // Close the file and socket
    fclose(file);
    close(sockfd);
}

void handle_server_ipv4_tcp(int port)
{
    int sockfd = create_socket_ipv4_tcp();
    struct sockaddr_in servaddr, cliaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
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
    printf("Listening on port %d (IPv4, TCP)\n", port);
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
        FD_SET(newsockfd, &set);
        FD_SET(connfd, &set);
        int max_fd = newsockfd > connfd ? newsockfd : connfd;
        select(max_fd + 1, &set, NULL, NULL, NULL);

        if (FD_ISSET(connfd, &set))
        {
            int bytes_recv = recv(connfd, buffer, MAX_BUFFER_SIZE, 0);
            if (bytes_recv < 0)
            {
                perror("recv");
                exit(EXIT_FAILURE);
            }
        }
        if (FD_ISSET(newsockfd, &set))
        {
            resvFun();         
        }
    }
}
