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

void ClassifiedCommunication(char *side,char *ip, int port,char *type,char *param){
    if ((strcmp(type, "ipv4") == 0) && (strcmp(param, "tcp") == 0)) {
        if(strcmp(side, "c")== 0){
            printf("handle_client_ipv4_tcp %s %s\n",type,param);
            //handle_client_ipv4_tcp();
        }else{
            printf("handle_server_ipv4_tcp %s %s\n",type,param);
            //handle_server_ipv4_tcp();
        }
    } else if ((strcmp(type, "ipv4")== 0) && (strcmp(param, "udp") == 0)) {
        if(strcmp(side, "c")== 0){
            printf("handle_client_ipv4_tcp %s %s\n",type,param);
            //handle_client_ipv4_udp();
        }else{
            printf("handle_server_ipv4_udp %s %s\n",type,param);
            //handle_server_ipv4_udp();
        }
        
    } else if ((strcmp(type, "ipv6")== 0) && (strcmp(param, "tcp") == 0)) {
        if(strcmp(side, "c")== 0){
            printf("handle_client_ipv6_tcp %s %s\n",type,param);
            //handle_client_ipv6_tcp();
        }else{
            printf("handle_server_ipv6_tcp %s %s\n",type,param);
            //handle_server_ipv6_tcp();
        }
        
    } else if ((strcmp(type, "ipv6")== 0) && (strcmp(param, "udp") == 0)) {
        
        if(strcmp(side, "c")== 0){
            printf("handle_client_ipv6_udp %s %s\n",type,param);
            //handle_client_ipv6_udp();
        }else{
            printf("handle_server_ipv6_udp %s %s\n",type,param);
            //handle_server_ipv6_udp();
        }
    } else if ((strcmp(type, "uds")== 0) && (strcmp(param, "dgram") == 0)) {
        
        if(strcmp(side, "c")== 0){
            printf("handle_uclient_ds_dgram %s %s\n",type,param);
            //handle_uclient_ds_dgram();
        }else{
            printf("handle__serveruds_dgram %s %s\n",type,param);
            //handle__serveruds_dgram();
        }
    } else if ((strcmp(type, "uds")== 0) && (strcmp(param, "stream") == 0)) {
        
        if(strcmp(side, "c")== 0){
            printf("handle_udclient_s_stream %s %s\n",type,param);
            //handle_udclient_s_stream();
        }else{
            printf("handle_u_serverds_stream %s %s\n",type,param);
            //handle_u_serverds_stream();
        }
    } else if ((strcmp(type, "mmap")== 0) && (strcmp(param, "filename") == 0)) {
        
        if(strcmp(side, "c")== 0){
            printf("handle_mmap_client_filename %s %s\n",type,param);
            //handle_mmap_client_filename();
        }else{
            printf("handle_mmap_server_filename %s %s\n",type,param);
            //handle_mmap_server_filename();
        }
    } else if ((strcmp(type, "pipe")== 0) && (strcmp(param, "filename") == 0)) {
        
        if(strcmp(side, "c")== 0){
            printf("handle_pipe_client_filename %s %s\n",type,param);
            //handle_pipe_client_filename();
        }else{
            printf("handle_pipe_server_filename %s %s\n",type,param);
            //handle_pipe_server_filename();
        }
    } else {
        printf("Unknown option: %s %s\n", type, param);
    }

}


int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s [-c IP PORT] | [-s PORT]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int c;
    char *ip = NULL;
    int port = 0;
    char *type = NULL;
    char *param = NULL;
    while ((c = getopt(argc, argv, "c:s:")) != -1)
    {
        switch (c)
        {
        case 'c':
            ip = argv[2];
            port = atoi(argv[3]);
            if(argc >= 6){
                type = argv[4];
                param = argv[5];
                ClassifiedCommunication("c",ip, port,type,param);
            }else{
                part_a_connect_server(ip, port);
            }
            break;
        case 's':
            port = atoi(argv[2]);
            if(argc >= 5){
                type = argv[3];
                param = argv[4];
                ClassifiedCommunication("s","",port,type,param);

            }else{
                part_a_start_server(port);
            }
            break;
        default:
            printf("Usage: %s [-c IP PORT] | [-s PORT] | [-c IP PORT TYPE PARAM] | [-s PORT TYPE PARAM]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}