#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <string.h>
#include "stnc.h"

#define SOCKET_PATH "/tmp/socket_path"
#define BUFFER_SIZE 1024
#define FILE_PATH "100MB-File.txt"

void handle_client_uds_stream()
{
    struct sockaddr_un addr;
    int fd, bytes_sent;
    char buffer[BUFFER_SIZE];
    FILE *fp;

    // create a socket
    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // set socket address parameters
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    // connect to the server
    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    // open file to send
    fp = fopen(FILE_PATH, "rb");
    if (fp == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    // send file to the server
    if (p_flag)
    {
        printf("Starting to send the file\n");
    }
    
    while (!feof(fp))
    {
        bytes_sent = fread(buffer, 1, BUFFER_SIZE, fp);
        if (bytes_sent <= 0)
        {
            break;
        }
        if (write(fd, buffer, bytes_sent) == -1)
        {
            perror("write");
            exit(EXIT_FAILURE);
        }
        memset(&buffer, 0, sizeof(buffer));
    }
    // send_stop();
    if(p_flag){
        printf("The entire file has been sent\n");
    }
    // close the file
    fclose(fp);

    // close the connection
    close(fd);
}

void handle_server_uds_stream()
{
    struct sockaddr_un addr;
    int fd, client_fd, bytes_received;
    char buffer[BUFFER_SIZE];

    // create a socket
    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // set socket address parameters
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    // bind the socket to the address
    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // listen for connections
    if (listen(fd, 1) == -1)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if(p_flag){
        printf("Listening on uds stream\n");
    }
    // accept a connection
    if ((client_fd = accept(fd, NULL, NULL)) == -1)
    {
        perror("accept");
    }

    struct timeval start;
    gettimeofday(&start, 0);

    // receive file from the client
    while (strstr(buffer, "x") == NULL && time_since(start)<3000)
    {
        bytes_received = read(client_fd, buffer, BUFFER_SIZE);
        if (bytes_received == -1)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }
    }
    float total_time = time_since(start);
    if (p_flag) {
        printf("The file has been received\n");
    }
    printf("uds_stream,%f\n", total_time);

    // close the client connection
    close(client_fd);
    close(fd);

    int status = remove(SOCKET_PATH);
    if (status != 0)
    {
        printf("Could not delete file. You will need to delete it manually before running uds stream again, run the command rm /tmp/socket_path\n");
    }
}


// int main(int argc, char **argv) {
//     if (argc == 2 && strcmp(argv[1], "-s") == 0) {
//         handle_server_uds_stream();
//     }
//     else if (argc == 2 && strcmp(argv[1], "-c") == 0) {
//         handle_client_uds_stream();
//     }
//     else {
//         printf("Usage:\n");
//         printf("\t%s -s : Run as server\n", argv[0]);
//         printf("\t%s -c : Run as client\n", argv[0]);
//     }

//     return 0;
// }
