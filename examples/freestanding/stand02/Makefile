TOP=../..
include $(TOP)/Make.rules

LDLIBS=
CFLAGS+=-O

LDFLAGS+=-Wl,--no-relax
LDFLAGS+=-Wl,-Tdata=0x1000

PROGS=prog prog.bin prog.lst 

all:: $(PROGS)

prog: crt0.o 
	$(LINK.c) -o $@ $^ $(LDLIBS)
	$(SIZE) -A -x $@

clean::
	rm -f $(PROGS) *.o *.s *.lst *.bin *.srec

.PHONY: run
run: prog.bin
	../../../src/rvddt -l0x2000 -f prog.bin
