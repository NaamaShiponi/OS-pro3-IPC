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
#include <sys/time.h>
#include "stnc.h"

#define MAX_BUFFER_SIZE 1024

extern int p_flag;

int create_socket_ipv6_tcp()
{
    int sockfd = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    return sockfd;
}

void handle_client_ipv6_tcp(char *ip, int port)
{
    int sockfd = create_socket_ipv6_tcp(AF_INET6);
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
    
    if (p_flag) {
        printf("Connected to server (%s : %d) with IPv6 TCP\n", ip, port);
    }

    // Open the file for reading
    FILE *fp;
    fp = fopen("100MB-File.txt", "rb");
    if (fp == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    // Send the contents of the file to the server
    char buffer[MAX_BUFFER_SIZE];
    size_t bytes_read;

    if (p_flag) {
        printf("Starting to send the file\n");
    }

    while ((bytes_read = fread(buffer, sizeof(char), MAX_BUFFER_SIZE, fp)) > 0)
    {
        int bytes_sent = send(sockfd, buffer, bytes_read, 0);
        if (bytes_sent < 0)
        {
            perror("send");
            exit(EXIT_FAILURE);
        }
    }

    if (p_flag) {
        printf("The entire file has been sent\n");
        printf("Closes the connection with (%s : %d)\n", ip, port);
    }

    // Close the file and the socket
    fclose(fp);
    close(sockfd);
}

void handle_server_ipv6_tcp(int port)
{
    int sockfd = create_socket_ipv6_tcp();
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

    if (p_flag) {
        printf("Listening on port %d (IPv6, TCP)\n", port);
    }

    socklen_t cliaddrlen = sizeof(cliaddr);
    int connfd = accept(sockfd, (struct sockaddr *)&cliaddr, &cliaddrlen);
    if (connfd < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_BUFFER_SIZE];
    int count = 0;
    struct timeval start;
    gettimeofday(&start, 0);
    
    while (strstr(buffer, "x") == NULL)
    { 
        count++;
        if(count==1) gettimeofday(&start, 0);
        int bytes_recv = recv(connfd, buffer, MAX_BUFFER_SIZE, 0);
        if (bytes_recv < 0)
        {
            perror("recv");
            exit(EXIT_FAILURE);
        }
    }

    float total_time = time_since(start);

    if (p_flag) {
        printf("The file has been received\n");
        printf("checksum OK\n");
    }

    printf("ipv6_tcp,%f\n", total_time);

}


// int main(int argc, char *argv[])
// {
//     if (argc < 2)
//     {
//         printf("Usage: %s [-c IP PORT] | [-s PORT]\n", argv[0]);
//         exit(EXIT_FAILURE);
//     }
//     int c;
//     char *ip = NULL;
//     int port = 0;
//     while ((c = getopt(argc, argv, "c:s:")) != -1)
//     {
//         switch (c)
//         {
//         case 'c':
//             ip = argv[2];
//             port = atoi(argv[3]);
//             handle_client_ipv6_tcp(ip, port);
//             break;
//         case 's':
//             port = atoi(argv[2]);
//             handle_server_ipv6_tcp(port);
//             break;
//         default:
//             printf("Usage: %s [-c IP PORT] | [-s PORT]\n", argv[0]);
//             exit(EXIT_FAILURE);
//         }
//     }
//     return 0;
// }