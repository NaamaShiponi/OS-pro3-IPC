#ifndef STNC_H
#define STNC_H
#include "part_a.c"
#include "tcp_time.c"

//PART A FUNCTIONS
void part_a_start_server(int port);
void part_a_connect_server(char *ip, int port);


//PART B FUNCTIONS
    //tcp_time
    void server_tcp_time();
    void client_tcp_time();
    void send_start();
    void send_end();


#endif

