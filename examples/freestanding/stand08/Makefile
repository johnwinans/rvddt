TOP=../..
include $(TOP)/Make.rules

LDLIBS=-lc -lgcc

CFLAGS+=-O

PROGS=prog prog.bin prog.lst 

all:: $(PROGS)

prog: crt0.o main.o myfunc.o
	$(LINK.c) -o $@ $^ $(LDLIBS)
	$(SIZE) -A -x $@

clean::
	rm -f $(PROGS) *.o *.s *.lst *.bin *.srec

.PHONY: run
run: prog.bin
	../../../srtc/rvddt -l3000 -f prog.bin
