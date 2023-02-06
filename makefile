# Makefile for client and server

CC = gcc
OBJCS = client2.c
OBJCSS = server2.c

CFLAGS =  -g -Wall
# setup for system
nLIBS =

all: client2 server2

client2: $(OBJCS)
	$(CC) $(CFLAGS) -o $@ $(OBJCS) $(LIBS)

server2: $(OBJCSS)
	$(CC) $(CFLAGS) -o $@ $(OBJCSS) $(LIBS)






clean:
	rm client2 server2
