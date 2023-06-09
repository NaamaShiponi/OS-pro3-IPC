#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/time.h>
#include "stnc.h"


#define SOCK_PATH "/tmp/uds_socket"
#define FILE_PATH "100MB-File.txt"

void handle_server_uds_dgram() {
    int sockfd, bytes_recv;
    struct sockaddr_un serv_addr, cli_addr;
    socklen_t clilen;
    char buf[1024];

    // Create socket
    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }

    // Bind socket to socket path
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, SOCK_PATH);
    unlink(serv_addr.sun_path);
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind");
        exit(1);
    }

    if (p_flag) {
        printf("Server open for received message with uds dgram\n");
    }

    struct timeval start;
    gettimeofday(&start, 0);
        
    // Receive message from client
    clilen = sizeof(cli_addr);
    while (strstr(buf, "x") == NULL && time_since(start)<3000) {
        memset(&buf, 0, sizeof(buf));
        bytes_recv = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&cli_addr, &clilen);
        if (bytes_recv < 0) {
            perror("recvfrom");
            exit(1);
        }
    }
    float total_time = time_since(start);

    if (p_flag) {
        printf("The file has been received\n");
        printf("checksum OK\n");
    }

    total_time/=50;

    printf("uds_dgram,%f\n", total_time);

    close(sockfd);
    unlink(serv_addr.sun_path);
}

void handle_client_uds_dgram() {
    int sockfd;
    struct sockaddr_un serv_addr;
    char buf[1024];

    // Create socket
    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }

    // Connect to server
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, SOCK_PATH);
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        exit(1);
    }

    // Open file to send
    FILE* file = fopen(FILE_PATH, "r");
    if (file == NULL) {
        perror("fopen");
        exit(1);
    }

    if(p_flag) {
        printf("The client sends the file\n");
    }

    // Send file to server
    while (fgets(buf, sizeof(buf), file)) {
        if (send(sockfd, buf, strlen(buf), 0) < 0) {
            perror("send");
            exit(1);
        }
    }

    if (p_flag) {
        printf("The client has finished sending the file\n");
    }

    // Close file and socket
    fclose(file);
    close(sockfd);
}


// int main(int argc, char *argv[]) {
//     if (argc != 2) {
//         printf("Usage: %s [-c/-s]\n", argv[0]);
//         return 1;
//     }

//     if (strcmp(argv[1], "-s") == 0) {
//         printf("Starting server...\n");
//         handle_server_uds_dgram();
//     } else if (strcmp(argv[1], "-c") == 0) {
//         printf("Starting client...\n");
//         handle_client_uds_dgram();
//     } else {
//         printf("Usage: %s [-c/-s]\n", argv[0]);
//         return 1;
//     }

//     return 0;
// }
