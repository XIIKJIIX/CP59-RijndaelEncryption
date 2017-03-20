OBJS = utility.o base64.o
CFLAGS = -c -Iinclude
CC = gcc

all: lib.a

lib.a: $(OBJS)
	ar cr $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

utility.o: utility.c
base64.o: base64.c



clean:
	rm -f $(OBJS) lib.a
