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
#include "stnc.h"


#define MAX_BUFFER_SIZE 1024

int create_socket_ipv4_udp() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    return sockfd;
}
void handle_client_ipv4_udp(char *ip, int port)
{
    int sockfd = create_socket_ipv4_udp();
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
    printf("Connected to server at %s IPv4 on port %d using UDP\n", ip, port);

    // Open file
    FILE *fp = fopen("100MB-File.txt", "r");
    if (!fp)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    // Read file contents and send over socket
    char buffer[MAX_BUFFER_SIZE];
    size_t bytes_read = 0;
    printf("Starting to send the file\n");
    // send_start();
    while ((bytes_read = fread(buffer, 1, MAX_BUFFER_SIZE, fp)) > 0)
    {
        int bytes_sent = send(sockfd, buffer, bytes_read, 0);
        if (bytes_sent < 0)
        {
            perror("send");
            exit(EXIT_FAILURE);
        }
    }
    // send_stop();
    printf("The entire file has been sent\n");
    printf("Closes the connection with the server at %s on port %d using UDP\n", ip, port);

    // Close file and socket
    fclose(fp);
    // close(sockfd);
}

void handle_server_ipv4_udp(int port)
{
    int sockfd = create_socket_ipv4_udp();
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
    printf("Listening on port %d (IPv4, UDP)\n", port);

    socklen_t cliaddrlen = sizeof(cliaddr);

    char buffer[MAX_BUFFER_SIZE];
    // fd_set set;
    // FD_ZERO(&set);
    int count=0;
    // while (1)
    // {
        // FD_SET(newsockfd, &set);
        // FD_SET(sockfd, &set);
        // int max_fd = newsockfd > sockfd ? newsockfd : sockfd;
        // select(max_fd + 1, &set, NULL, NULL, NULL);
        // if (FD_ISSET(sockfd, &set))
        // {
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
                perror("recv");
                exit(EXIT_FAILURE);
            }
           
        }
        // }
    //     if (FD_ISSET(newsockfd, &set))
    //     {
    //         resvFun();         
    //     }
    // }
    float total_time = time_since(start);
    printf("Timer stopped. Remaining seconds: %f\n", total_time);
}
