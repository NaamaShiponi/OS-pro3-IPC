CC = gcc
CFLAGS = -g -Wall 

SRCS = stnc.c stnc1.c
OBJS = $(SRCS:.c=.o)

all: stnc stnc1

stnc: stnc.o
	$(CC) $(CFLAGS) -o $@ $<

stnc.o: stnc.c
	$(CC) $(CFLAGS) -c $< -o $@

stnc1: stnc1.o
	$(CC) $(CFLAGS) -o $@ $<

stnc1.o: stnc1.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f stnc stnc1 $(OBJS)

