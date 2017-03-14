LDFLAGS =-lm
CC =gcc
CXX =g++
CXXFLAGS =-std=c++11 -Wall
CFLAGS =-Wall -std=c99

.PHONY: default all clean

default: corrector
all: default

UNAME := $(shell uname -s)
ifeq ($(UNAME),Darwin)
	CFLAGS += -D OSX
endif

secded.o: secded.cc secded.hh

instruction_secded.o: instruction_secded.cc instruction_secded.hh

secded_for_text.o: secded_for_text.cc secded_for_text.hh

fault_injector.o: fault_injector.cc fault_injector.hh

filter.o: filter.cc filter.hh

corrector.o: corrector.cc

corrector: corrector.o secded.o instruction_secded.o secded_for_text.o fault_injector.o filter.o
	$(CXX) -o $@ $^ $(LDFLAGS)


clean:
	-rm -f *.o
	-rm -f $(TARGET)