#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PIPE_NAME "my_pipe"

void handle_server_pipe_filename() {
    int fd;
    size_t bytes_read, total_bytes_read = 0;
    char file_buf[1024];
    mkfifo(PIPE_NAME, 0666);
    fd = open(PIPE_NAME, O_RDONLY);

    printf("Server open for received message\n");
    // Gets the content found in the file through the pipe
    while ((bytes_read = read(fd, file_buf, sizeof(file_buf))) > 0) {
    if (bytes_read == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }
    }

    printf("Server received message\n");    

    close(fd);
    fd = open(PIPE_NAME, O_WRONLY);

    // The server sends that it received
    printf("The server sends that it received\n"); 
    write(fd, "Message received", 17);
    close(fd);
    unlink(PIPE_NAME);
}


void handle_client_pipe_filename() {
    int fd;
    char buf[100];
    char file_buf[1024];
    size_t bytes_read;
    fd = open(PIPE_NAME, O_WRONLY);

    // Open and send contents of 100MB-File.txt
    FILE *fp = fopen("100MB-File.txt", "r");
    if (fp == NULL) {
        perror("Failed to open file");
        exit(1);
    }

    // The client reads from the file and sends it
    while ((bytes_read = fread(file_buf, 1, sizeof(file_buf), fp)) > 0) {
        write(fd, file_buf, bytes_read);
    }
    fclose(fp);

    close(fd);
    fd = open(PIPE_NAME, O_RDONLY);
    // The client received the confirmation that the server received
    read(fd, buf, sizeof(buf));
    printf("Client received message: %s\n", buf);
    close(fd);
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: pipe_filename -c for client or -s for server\n");
        return 0;
    }

    if (strcmp(argv[1], "-c") == 0) {
        handle_client_pipe_filename();
    } else if (strcmp(argv[1], "-s") == 0) {
        handle_server_pipe_filename();
    } else {
        printf("Usage: pipe_filename -c for client or -s for server\n");
    }

    return 0;
}
