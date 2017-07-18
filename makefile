LDFLAGS = -lm -fopenmp
ARCH ?= native
CC = gcc
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Ivixl/src -MD -MP -O3 -fopenmp -march=$(ARCH)
CFLAGS = -Wall -std=c99

TARGET:= corrector sample
CORRECTOR_OBJS:= corrector.o secded.o instruction_secded.o secded_for_text.o fault_injector.o filter.o elf_extractor.o \
	asm_function.o trainer.o crc32c.o program.o\
	vixl-disasm.o vixl-decoder.o vixl-instructions.o vixl-compiler-intrinsics.o vixl-utils.o

.PHONY: default all clean

default: $(TARGET)
all: default


UNAME := $(shell uname -s)
ifeq ($(UNAME),Darwin)
	CFLAGS += -D OSX
	CXXFLAGS += -D OSX
endif

%.o: %.cc
	$(CXX) -c $(CXXFLAGS) $*.cc -o $*.o

vixl-decoder.o:	vixl/src/vixl/a64/decoder-a64.cc vixl/src/vixl/a64/decoder-a64.h vixl/src/vixl/a64/instruction-features-a64.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

vixl-disasm.o: vixl/src/vixl/a64/disasm-a64.cc vixl/src/vixl/a64/disasm-a64.h vixl/src/vixl/a64/instruction-features-a64.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

vixl-instructions.o: vixl/src/vixl/a64/instructions-a64.cc vixl/src/vixl/a64/instructions-a64.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

vixl-compiler-intrinsics.o:	vixl/src/vixl/compiler-intrinsics.cc vixl/src/vixl/compiler-intrinsics.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

vixl-utils.o:	vixl/src/vixl/utils.cc vixl/src/vixl/utils.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# corrector.o: corrector.cc

corrector: $(CORRECTOR_OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)


sample: sample.c


-include $(CORRECTOR_OBJS:.o=.d)

clean:
	-rm -f *.o *.d
	-rm -f $(TARGET)
