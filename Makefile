CC = gcc
CFLAGS = -g -Wall 

SRCS = stnc.c 
OBJS = $(SRCS:.c=.o)

all: stnc

stnc: $(OBJS)
	$(CC) $(CFLAGS) -o stnc $(OBJS)

stnc.o: stnc.c
	$(CC) $(CFLAGS) -c stnc.c

clean:
	rm -f stnc $(OBJS)
