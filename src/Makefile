OBJS = utility.o decode.o encode.o
CFLAGS = -c -Iinclude
CC = gcc

all: lib.a

lib.a: $(OBJS)
	ar cr $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

utility.o: utility.c
decode.o: decode.c
encode.o: encode.c


clean:
	rm -f $(OBJS) lib.a
