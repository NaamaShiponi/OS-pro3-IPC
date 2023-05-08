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
#-ipv6 tcp
IPV6_TCP_C= ipv6_tcp.c
IPV6_TCP_O= $(IPV6_TCP_C:.c=.o) 

#-ipv4 udp
IPV4_UDP_C=ipv4_udp.c
IPV4_UDP_O= $(IPV4_UDP_C:.c=.o)


#ALL FILE
OBJS = $(STNC_O) $(IPV6_TCP_O) $(PART_A_O) $(IPV4_UDP_O)
ALLFILES = $(OBJS) $(STNC_C:.c=) 

all: stnc

stnc: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $<

$(STNC_O): $(STNC_C) $(STNC_H)
	$(CC) $(CFLAGS) -c $< -o $@

$(PART_A_O): $(PART_A_C)
	$(CC) $(CFLAGS) -c $< -o $@

$(IPV6_TCP_O): $(IPV6_TCP_C)
	$(CC) $(CFLAGS) -c $< -o $@

$(IPV4_UDP_O): $(IPV4_UDP_C)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(ALLFILES)

