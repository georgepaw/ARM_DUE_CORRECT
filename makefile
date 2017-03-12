LDFLAGS =-L/nfs/home/george/gsl/lib -lgsl -lgslcblas -lm
CC = gcc
CFLAGS = -I/nfs/home/george/gsl/include -Wall -std=c99

.PHONY: default all clean

default: textflip secded_test sample corrector
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

sample: sample.o

corrector: corrector.o secded_for_text.o secded.o instruction_secded.o fault_injector.o filter.o
	$(CC) -o $@ $^ $(LDFLAGS)


clean:
	-rm -f *.o
	-rm -f $(TARGET)