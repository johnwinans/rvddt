
CFLAGS+=-DDEBUGGING_HACKS

CFLAGS+=-Wall -Werror
CXXFLAGS=$(CFLAGS) -std=c++0x


CSRC=

CCSRC=\
	main.cc\
	memory.cc\
	rv32.cc

COBJ=$(CSRC:.c=.o)
CCOBJ=$(CCSRC:.cc=.o)
OBJS=$(COBJ) $(CCOBJ)

.PHONY: all clean world

all: rvddt

rvddt: $(OBJS)
	$(LINK.cc) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS) 

clean:
	rm -f *.o rvddt

world: clean all
