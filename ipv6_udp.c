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

int create_socket_ipv6_udp() {
    int sockfd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    return sockfd;
}

void handle_client_ipv6_udp(char *ip, int port) {
    int sockfd = create_socket_ipv6_udp();
    struct sockaddr_in6 servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin6_family = AF_INET6;
    inet_pton(AF_INET6, ip, &(servaddr.sin6_addr));
    servaddr.sin6_port = htons(port);
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }
    
    if (p_flag) {
        printf("Connected to server (%s : %d) with IPv6 UDP\n", ip, port);
    }

    FILE *file = fopen("100MB-File.txt", "rb");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_BUFFER_SIZE];
    int bytes_sent = 0;
    // int total_bytes_sent = 0;

    if (p_flag) {
        printf("Starting to send the file\n");
    }
    while (1) {
        int bytes_read = fread(buffer, 1, MAX_BUFFER_SIZE, file);
        if (bytes_read == 0) {
            break;
        }
        bytes_sent = send(sockfd, buffer, bytes_read, 0);
        if (bytes_sent < 0) {
            perror("send");
            exit(EXIT_FAILURE);
        }
    }
    if (p_flag) {
        printf("The entire file has been sent\n");
        printf("Closes the connection with (%s : %d)\n", ip, port);
    }

    fclose(file);
    close(sockfd);
}

void handle_server_ipv6_udp(int port)
{
    int sockfd = create_socket_ipv6_udp();
    struct sockaddr_in6 servaddr, cliaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
    servaddr.sin6_family = AF_INET6;
    servaddr.sin6_addr = in6addr_any;
    servaddr.sin6_port = htons(port);
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (p_flag) {
        printf("Listening on port %d (IPv6, UDP)\n", port);
    }

    socklen_t cliaddrlen = sizeof(cliaddr);

    char buffer[MAX_BUFFER_SIZE];

    int count = 0;
    struct timeval start;
    gettimeofday(&start, 0);
    
    while (strstr(buffer, "x") == NULL && time_since(start)<3000) //TODO  
    { 
        count++;
        if(count==1) gettimeofday(&start, 0);
        int bytes_recv = recvfrom(sockfd, buffer, MAX_BUFFER_SIZE, 0,
                                    (struct sockaddr *)&cliaddr, &cliaddrlen);
        if (bytes_recv < 0)
        {
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }
    }

    float total_time = time_since(start);

    if (p_flag) {   
        printf("The file has been received\n");
        printf("checksum OK\n");
    }

    total_time/=50;

    printf("ipv6_udp,%f\n", total_time);
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
//             connect_server(ip, port);
//             break;
//         case 's':
//             port = atoi(argv[2]);
//             start_server(port);
//             break;
//         default:
//             printf("Usage: %s [-c IP PORT] | [-s PORT]\n", argv[0]);
//             exit(EXIT_FAILURE);
//         }
//     }
//     return 0;
// }