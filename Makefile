CC=gcc
CFLAGS= -g -Wall -Iinclude -c

all: rijndael

src/lib.a: src/Makefile
	cd src/ && make

rijndael.o: rijndael.c
	$(CC) $(CFLAGS) $^ -o $@

rijndael: rijndael.o src/lib.a
	$(CC) $^ -o $@

clean:
	rm -f rijndael
