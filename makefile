LDFLAGS=-lm
CC=gcc
CXX=g++
CXXFLAGS=-std=c++11 -Wall -Ivixl/src
CFLAGS=-Wall -std=c99

.PHONY: default all clean

default: corrector sample
all: default

UNAME := $(shell uname -s)
ifeq ($(UNAME),Darwin)
	CFLAGS += -D OSX
	CXXFLAGS += -D OSX
endif

secded.o: secded.cc secded.hh

instruction_secded.o: instruction_secded.cc instruction_secded.hh

secded_for_text.o: secded_for_text.cc secded_for_text.hh

fault_injector.o: fault_injector.cc fault_injector.hh

filter.o: filter.cc filter.hh

elf_extractor.o: elf_extractor.cc elf_extractor.hh

asm_function.o: asm_function.cc asm_function.hh

vixl-decoder.o:	vixl/src/vixl/a64/decoder-a64.cc vixl/src/vixl/a64/decoder-a64.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

vixl-disasm.o: vixl/src/vixl/a64/disasm-a64.cc vixl/src/vixl/a64/disasm-a64.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

vixl-instructions.o: vixl/src/vixl/a64/instructions-a64.cc vixl/src/vixl/a64/instructions-a64.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

vixl-compiler-intrinsics.o:	vixl/src/vixl/compiler-intrinsics.cc vixl/src/vixl/compiler-intrinsics.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

vixl-utils.o:	vixl/src/vixl/utils.cc vixl/src/vixl/utils.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

corrector.o: corrector.cc

corrector: corrector.o secded.o instruction_secded.o secded_for_text.o fault_injector.o filter.o elf_extractor.o \
	asm_function.o \
	vixl-disasm.o vixl-decoder.o vixl-instructions.o vixl-compiler-intrinsics.o vixl-utils.o
	$(CXX) -o $@ $^ $(LDFLAGS)


sample: sample.c


clean:
	-rm -f *.o
	-rm -f $(TARGET)