
PROG=uart

all:: $(PROG).bin $(PROG).lst

$(PROG).bin:: $(PROG)

clean::
	rm -f $(PROG) *.o *.lst *.bin *.srec

TOP=..
include $(TOP)/Make.rules
