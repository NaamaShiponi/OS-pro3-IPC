CC = gcc
CFLAGS = -g -Wall 

STNC_C = stnc.c
STNC_O= $(STNC_C:.c=.o) 

IPV6_TCP_C= ipv6_tcp.c
IPV6_TCP_O= $(IPV6_TCP_C:.c=.o) 

OBJS = $(STNC_O) $(IPV6_TCP_O)
ALLFILES = $(STNC_O) $(IPV6_TCP_O) $(IPV6_TCP_C:.c=) $(STNC_C:.c=) 

all: stnc

stnc: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $<

$(STNC_O): $(STNC_C)
	$(CC) $(CFLAGS) -c $< -o $@

$(IPV6_TCP_O): $(IPV6_TCP_C)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(ALLFILES)

