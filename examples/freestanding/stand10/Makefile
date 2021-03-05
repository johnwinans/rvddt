TOP=../..
include $(TOP)/Make.rules

LDLIBS=-lc -lgcc

CFLAGS+=-O

PROGS=prog prog.bin prog.lst 

all:: $(PROGS)

prog: crt0.o main.o myfunc.o stub_stdlib.o
	$(LINK.c) -o $@ $^ $(LDLIBS)
	$(SIZE) -A -x $@

clean::
	rm -f $(PROGS) *.o *.s *.lst *.bin *.srec

.PHONY: run
run: prog.bin
	../../../src/rvddt -l0x4000 -f prog.bin
