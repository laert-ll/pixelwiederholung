CC = gcc
CFLAGS = -Wall -std=c17

all: main

main: main.o img_proc.o bmp_io.o
	$(CC) $(CFLAGS) -o main main.o img_proc.o bmp_io.o

main.o: main.c img_proc.h bmp_io.h
	$(CC) $(CFLAGS) -c main.c

img_proc.o: img_proc.c img_proc.h
	$(CC) $(CFLAGS) -c img_proc.c

bmp_io.o: bmp_io.c bmp_io.h
	$(CC) $(CFLAGS) -c bmp_io.c

clean:
	rm -f *.o main
