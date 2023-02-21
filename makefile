# Makefile for client and server

CC = gcc
OBJCS = client2.c
OBJCSS = server1.c

CFLAGS =  -g -Wall
# setup for system
nLIBS =

all: client2 server1

client: $(OBJCS)
	$(CC) $(CFLAGS) -o $@ $(OBJCS) $(LIBS)

server0: $(OBJCSS)
	$(CC) $(CFLAGS) -o $@ $(OBJCSS) $(LIBS)


clean:
	rm client2 server1
