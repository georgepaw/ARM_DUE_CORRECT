LDFLAGS = -lm
CC = gcc
CFLAGS = -Wall -std=c99

.PHONY: default all clean

default: textflip secded_test
all: default

UNAME := $(shell uname -s)
ifeq ($(UNAME),Darwin)
	CFLAGS += -D OSX
endif

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)

textflip: textflip.o my_mem.o secded.o

secded_test: secded_test.o my_mem.o secded.o

clean:
	-rm -f *.o
	-rm -f $(TARGET)