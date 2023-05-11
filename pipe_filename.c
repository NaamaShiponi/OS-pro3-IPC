#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <poll.h>
#include <string.h>

#define FILE_PIPE "file_pipe"

void handle_server_pipe_filename() {
    int fd, bytes_read;
    char buffer[1024];
    struct pollfd pfd;

    // create the named pipe
    mkfifo(FILE_PIPE, 0666);

    // open the named pipe for reading
    fd = open(FILE_PIPE, O_RDONLY);

    // poll for events
    pfd.fd = fd;  // fd: The file descriptor to be monitored
    pfd.events = POLLIN;  // The events to monitor for, POLLIN for data to read
        
    printf("Server open for received message\n");

    do {
        // wait for an event
        poll(&pfd, 1, -1);

    // read the data from the pipe
    } while (read(fd, buffer, sizeof(buffer)) > 0);

    printf("Server received message\n");
    
    // close the pipe and remove the file
    close(fd);
    unlink(FILE_PIPE);
}


void handle_client_pipe_filename() {
    int fd;
    char buffer[1024];

    // open the named pipe for writing
    fd = open(FILE_PIPE, O_WRONLY);

    // open the file to be sent
    FILE* fp = fopen("100MB-File.txt", "r");

    // read the contents of the file and write them to the pipe
    printf("The client reads from the 100MB-File.txt file and write them to the pipe\n");
    while (fgets(buffer, sizeof(buffer), fp)) {
        write(fd, buffer, sizeof(buffer));
    }
    printf("The client has finished sending the file through the pipe\n");

    // close the file and the pipe
    fclose(fp);
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
