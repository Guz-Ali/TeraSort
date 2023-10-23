CC=gcc
CFLAGS=-Wall
OFLAGS=-O3

all: mysort

fileio: mysort.c
	$(CC) $(CFLAGS) $(OFLAGS) -o mysort mysort.c

clean:
	rm mysort
