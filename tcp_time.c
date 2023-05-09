#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#define PORT 7521
#define MESSAGE_SIZE 1024
int sockfd_tcp_time,sockfd_s,newsockfd;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

long long time_since(time_t start_time) {
    struct timespec current_time;
    clock_gettime(CLOCK_REALTIME, &current_time);

    long long start_ms = ((long long)start_time * 1000LL);
    long long current_ms = ((long long)current_time.tv_sec * 1000LL) + (current_time.tv_nsec / 1000000LL);

    return current_ms - start_ms;
}
void send_start(){
    char buffer[MESSAGE_SIZE];

    strcpy(buffer, "start");
    int n = send(sockfd_tcp_time, buffer, strlen(buffer), 0);
    if (n < 0)
    {
        error("ERROR writing to socket");
    }else{
        printf("send start\n");
    }
}

void send_stop(){
    char buffer[MESSAGE_SIZE];

    strcpy(buffer, "stop");
    int n = send(sockfd_tcp_time, buffer, strlen(buffer), 0);
    if (n < 0)
    {
        error("ERROR writing to socket");
    }else{
        printf("send stop\n");
    }

    close(sockfd_tcp_time);
}



void client_tcp_time(){
    int portno;
    struct sockaddr_in serv_addr;
    srand(time(NULL));
    // Client mode.
        sockfd_tcp_time = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd_tcp_time < 0)
        {
            error("ERROR opening socket");
        }

        bzero((char *)&serv_addr, sizeof(serv_addr));
        portno = PORT;

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(portno);

        if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
        {
            error("ERROR invalid server address");
        }

        if (connect(sockfd_tcp_time, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            error("ERROR connecting to server");
        }else {
            printf("accept to TCP soket in server\n");
        }

}


void server_tcp_time()
{
    // time_t start_time;
    int portno;
    socklen_t clilen;
    char buffer[MESSAGE_SIZE];
    struct sockaddr_in serv_addr, cli_addr;
    // int n;
    srand(time(NULL));

    // Server mode.
    sockfd_s = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd_s < 0)
    {
        error("ERROR opening socket");
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = PORT;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    int yes = 1;
    if (setsockopt(sockfd_s, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        error("ERROR setting socket option");
    }
    if (bind(sockfd_s, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        error("ERROR on binding");
    }
    
    listen(sockfd_s, 5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd_s, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0)
    {
        error("ERROR on accept");
    }else{
        printf("client accept to TCP soket\n");
    }

    bzero(buffer, MESSAGE_SIZE);


    // n = recv(newsockfd, buffer, MESSAGE_SIZE - 1, 0);
    // if (n < 0)
    // {
    //     error("ERROR reading from socket");
    // }

    // printf("Server received message: %s\n", buffer);

    // if (strcmp(buffer, "start") == 0)
    // {
    //     start_time = time(NULL);
    //     printf("get start");
    //     bzero(buffer, MESSAGE_SIZE);
    // }
    

    // n = recv(newsockfd, buffer, MESSAGE_SIZE - 1, 0);
    // if (strcmp(buffer, "stop") == 0)
    // {
    //     printf("get stop");
    //     bzero(buffer, MESSAGE_SIZE);
    //     long long total_time = time_since(start_time);
    //     printf("Timer stopped. Remaining seconds: %lld\n", total_time);
    //     sprintf(buffer, "%lld", total_time);
    // }


    // close(newsockfd);
    // close(sockfd_s);
}

void resvFun(){
    time_t start_time;
    char buffer[MESSAGE_SIZE];
    int n;

    n = recv(newsockfd, buffer, MESSAGE_SIZE - 1, 0);
    if (n < 0)
    {
        error("ERROR reading from socket");
    }

    if (strcmp(buffer, "start") == 0)
    {
        start_time = time(NULL);
        printf("get start\n");
        bzero(buffer, MESSAGE_SIZE);
    }
    
    if (strcmp(buffer, "stop") == 0)
    {
        printf("get stop\n");
        bzero(buffer, MESSAGE_SIZE);
        long long total_time = time_since(start_time);
        printf("Timer stopped. Remaining seconds: %lld\n", total_time);
        sprintf(buffer, "%lld", total_time);
        close(newsockfd);
        exit(1);
    }

    
}







// int main(int argc, char *argv[])
// {
//     time_t start_time;
//     int sockfd, newsockfd, portno;
//     socklen_t clilen;
//     char buffer[MESSAGE_SIZE];
//     struct sockaddr_in serv_addr, cli_addr;
//     int n;
//     srand(time(NULL));

//     if (strcmp(argv[1], "-s") == 0){
//         server_tcp_time();
//     }else if (strcmp(argv[1], "-c") == 0)
//     {
//         client_tcp_time();
//     }
// }
