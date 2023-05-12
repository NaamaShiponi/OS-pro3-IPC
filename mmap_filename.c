#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include "stnc.h"

#define SHARED_MEM_NAME "/my_shared_memory"
#define SHARED_MEM_SIZE (1024*1024*1024) // 1 GB

extern int p_flag;

/**
 * To compile the code you need to write: gcc mmap_filename.c -o mmap_filename -lrt
 * Server: ./mmap_filename -s
 * Client: ./mmap_filename -c
*/

void handle_server_mmap_filename() {
    int fd = shm_open(SHARED_MEM_NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(fd, SHARED_MEM_SIZE) == -1) {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    void *addr = mmap(NULL, SHARED_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    if (p_flag) {
        printf("Server open for received message with mmap filename\n");
    }
    

    char *str = (char*)addr;

    while (*str == '\0') {}

    int count=0;
    struct timeval start;
    gettimeofday(&start, 0);
    while (strstr(str, "x") == NULL ) {
        count++;
    }
    float total_time = time_since(start);

    if (p_flag) {
        printf("The file has been received\n");
    }
    printf("mmap,%f\n", total_time);
    memset(addr, 0, SHARED_MEM_SIZE);
}

void handle_client_mmap_filename() {
    int fd = shm_open(SHARED_MEM_NAME, O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    void *addr = mmap(NULL, SHARED_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    if (p_flag) {
        printf("The shared memory object was created successfully\n");
    }

    char *str = (char*)addr;

    // Open the file
    FILE *fp = fopen("100MB-File.txt", "rb");
    if (fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    // Get the file size
    fseek(fp, 0L, SEEK_END);
    long int file_size = ftell(fp);
    rewind(fp);

    // Copy the file contents to the shared memory
    if (file_size > SHARED_MEM_SIZE) {
        fprintf(stderr, "File size is larger than shared memory size\n");
        exit(EXIT_FAILURE);
    }
    fread(str, sizeof(char), file_size, fp);

    if (p_flag) {
        printf("The entire file has been sent\n");
    }

    // Close the file
    fclose(fp);
}

// int main(int argc, char *argv[]) {
//     if (argc < 2) {
//         printf("Usage: mmap_filename -c for client or -s for server\n");
//         return 0;
//     }

//     if (strcmp(argv[1], "-c") == 0) {
//         handle_client_mmap_filename();
//     } else if (strcmp(argv[1], "-s") == 0) {
//         handle_server_mmap_filename();
//     } else {
//         printf("Usage: mmap_filename -c for client or -s for server\n");
//     }

//     return 0;
// }
