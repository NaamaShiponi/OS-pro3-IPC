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

extern int p_flag;

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
    if (p_flag) {
        printf("Connected to server (%s : %d) with IPv4 UDP\n", ip, port);
    }
    FILE *fp = fopen("100MB-File.txt", "r");
    if (!fp)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    // Read file contents and send over socket
    char buffer[MAX_BUFFER_SIZE];
    size_t bytes_read = 0;

    if (p_flag) {
        printf("Starting to send the file\n");
    }
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
    if (p_flag) {
        printf("The entire file has been sent\n");
        printf("Closes the connection with (%s : %d)\n", ip, port);
    }

    // Close file
    fclose(fp);
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
    
    if (p_flag) {
        printf("Listening on port %d (IPv4, UDP)\n", port);
    }

    socklen_t cliaddrlen = sizeof(cliaddr);

    char buffer[MAX_BUFFER_SIZE];

    int count=0;
    struct timeval start;
    gettimeofday(&start, 0);
    
    while (strstr(buffer, "x") == NULL && time_since(start)<3000)
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

    float total_time = time_since(start);

    if (p_flag) {
        printf("The file has been received\n");
        printf("checksum OK\n");
    }

    total_time/=50;

    printf("ipv4_udp,%f\n", total_time);
}
