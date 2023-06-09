CC = gcc
CFLAGS = -g -Wall 

# MAIN
STNC_C = stnc.c
STNC_H= $(STNC_C:.c=.h) 
STNC_O= $(STNC_C:.c=.o) 

# PART A
PART_A_C= part_a.c
PART_A_O= $(PART_A_C:.c=.o) 


# PART B
#-tcp time
TCP_SOCK_C= tcp_sock.c
TCP_SOCK_O= $(TCP_SOCK_C:.c=.o) 

#-ipv6 tcp
IPV6_TCP_C= ipv6_tcp.c
IPV6_TCP_O= $(IPV6_TCP_C:.c=.o) 

#-ipv6 udp
IPV6_UDP_C= ipv6_udp.c
IPV6_UDP_O= $(IPV6_UDP_C:.c=.o) 

#-ipv4 udp
IPV4_UDP_C=ipv4_udp.c
IPV4_UDP_O= $(IPV4_UDP_C:.c=.o)

#-ipv4 tcp
IPV4_TCP_C=ipv4_tcp.c
IPV4_TCP_O= $(IPV4_TCP_C:.c=.o)

#-uds dgram
UDS_DGRAM_C=uds_dgram.c
UDS_DGRAM_O= $(UDS_DGRAM_C:.c=.o)

#-uds stream
UDS_STREAM_C=uds_stream.c
UDS_STREAM_O= $(UDS_STREAM_C:.c=.o)

#-mmap filename
MMAP_FILENAME_C=mmap_filename.c
MMAP_FILENAME_O= $(MMAP_FILENAME_C:.c=.o)

#-mmap filename
PIPE_FILENAME_C=pipe_filename.c
PIPE_FILENAME_O= $(PIPE_FILENAME_C:.c=.o)

#ALL FILE
OBJS = $(STNC_O) $(TCP_SOCK_O) $(IPV6_TCP_O) $(PART_A_O) $(IPV4_UDP_O) $(IPV6_UDP_O) $(IPV4_TCP_O) $(UDS_DGRAM_O) $(UDS_STREAM_O) $(MMAP_FILENAME_O) $(PIPE_FILENAME_O)
ALLFILES = $(OBJS) $(STNC_C:.c=) 

all: stnc

stnc: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $< -lrt

$(STNC_O): $(STNC_C) $(STNC_H)
	$(CC) $(CFLAGS) -c $< -o $@

$(PART_A_O): $(PART_A_C) $(STNC_H)
	$(CC) $(CFLAGS) -c $< -o $@

$(TCP_SOCK_O): $(TCP_SOCK_C) $(STNC_H)
	$(CC) $(CFLAGS) -c $< -o $@

$(IPV6_TCP_O): $(IPV6_TCP_C) $(STNC_H)
	$(CC) $(CFLAGS) -c $< -o $@

$(IPV6_UDP_O): $(IPV6_UDP_C) $(STNC_H)
	$(CC) $(CFLAGS) -c $< -o $@

$(IPV4_UDP_O): $(IPV4_UDP_C) $(STNC_H)
	$(CC) $(CFLAGS) -c $< -o $@

$(IPV4_TCP_O): $(IPV4_TCP_C) $(STNC_H)
	$(CC) $(CFLAGS) -c $< -o $@

$(UDS_DGRAM_O): $(UDS_DGRAM_C) $(STNC_H)
	$(CC) $(CFLAGS) -c $< -o $@

$(UDS_STREAM_O): $(UDS_STREAM_C) $(STNC_H)
	$(CC) $(CFLAGS) -c $< -o $@

$(MMAP_FILENAME_O): $(MMAP_FILENAME_C) $(STNC_H)
	$(CC) $(CFLAGS) -c $< -o $@ 

$(PIPE_FILENAME_O): $(PIPE_FILENAME_C) $(STNC_H)
	$(CC) $(CFLAGS) -c $< -o $@ 


clean:
	rm -f $(ALLFILES)

