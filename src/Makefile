SOURCES := $(wildcard *.c)
OBJECTS := $(SOURCES:.c=.o)
HEADERS := $(wildcard *.h)
HD_OBJS := $(HEADERS:.h=.o)
TARGETS := main

CFLAGS=-Wall -O2 -std=c99
CC=gcc
LDADD=

all: $(TARGETS)
.PHONY : all

$(OBJECTS) : %.o : %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@ $(LDADD)

$(TARGETS) : % : %.o $(HD_OBJS)
	$(CC) -o $@ $^ $(LDADD)

clean:
	rm $(TARGETS) $(OBJECTS)
