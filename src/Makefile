
CFLAGS+=-DDEBUGGING_HACKS

CFLAGS+=-Wall -Werror
CXXFLAGS=$(CFLAGS) -std=c++11

CPPFLAGS+=-I.

CSRC=

CCSRC=\
	main.cc\
	memory.cc\
	devices.cc\
	saferead.cc\
	rv32.cc\
	dev/memcard.cc

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
