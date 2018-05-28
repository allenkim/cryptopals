SOURCES := $(wildcard *.c)
OBJECTS := $(SOURCES:.c=.o)
HEADERS := $(wildcard *.h)
HD_OBJS := $(HEADERS:.h=.o)
TARGETS := 1 2 3 4 5 6 7 8

CFLAGS=-Wall -O2 -std=c99
CC=gcc
LDADD=-lssl -lcrypto

all: $(TARGETS)
.PHONY : all

$(OBJECTS) : %.o : %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@ $(LDADD)

$(TARGETS) : % : %.o $(HD_OBJS)
	$(CC) -o $@ $^ $(LDADD)

clean:
	rm $(TARGETS) $(OBJECTS)
