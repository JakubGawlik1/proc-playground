CC = gcc
CFLAGS = -Wall -Wextra -g

SRC = $(wildcard src/*.c)
BIN = $(SRC:.c=)

all: $(BIN)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f src/*

