CC := gcc 
CFLAGS := -Wall -Wextra -Werror -pedantic -std=c99 -g -O2
LDFLAGS := -lm
RM := rm -f

SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)
HDRS := $(wildcard *.h)

.PHONY: all clean

all: client server

server: server.o
	$(CC) -o $@ $^ $(LDFLAGS)

client: client.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(TARGET) $(OBJS)
