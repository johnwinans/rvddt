
PROG=app

all:: $(PROG).bin

$(PROG):: part1.o part2.o
	$(LINK.c) $(LDFLAGS) -o $@ $^ $(LDLIBS)
	$(SIZE) -x -A $@

clean::
	rm -f $(PROG) *.o *.lst *.bin *.srec


TOP=..
include $(TOP)/Make.rules
