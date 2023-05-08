#ifndef STNC_H
#define STNC_H
#include "part_a.c"
#include "ipv4_tcp.c"
#include "ipv6_tcp.c"
#include "ipv4_udp.c"
#include "ipv6_udp.c"


//PART A FUNCTIONS
void part_a_start_server(int port);
void part_a_connect_server(char *ip, int port);


//PART B FUNCTIONS
void handle_client_ipv4_tcp(char *ip, int port);
void handle_server_ipv4_tcp(int port);
void handle_client_ipv4_udp(char *ip, int port);
void handle_server_ipv4_udp(int port);
void handle_client_ipv6_tcp(char *ip, int port);
void handle_server_ipv6_tcp(int port);
void handle_client_ipv6_udp(char *ip, int port);
void handle_server_ipv6_udp(int port);


#endif

