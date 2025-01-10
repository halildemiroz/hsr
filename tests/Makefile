# Makefile to compile all .c files into /bin folder

CC = gcc
CFLAGS = -Wall -Wextra -g
SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c, bin/%.o, $(SRCS))
TARGETS = $(patsubst %.c, bin/%, $(SRCS))

.PHONY: all clean

all: setup $(TARGETS)

# Rule to build each target
bin/%: bin/%.o
	$(CC) $(CFLAGS) -o $@ $^ -lxcb

# Rule to compile each source file to an object file
bin/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ -lxcb

# Create the /bin directory if it doesn't exist
setup:
	@mkdir -p bin

clean:
	@rm -rf bin
