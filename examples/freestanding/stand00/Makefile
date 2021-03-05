TOP=../..
include $(TOP)/Make.rules

LDLIBS=
CFLAGS+=-O

PROGS=prog prog.bin prog.lst 

all:: $(PROGS)

prog: crt0.o 
	$(LINK.c) -o $@ $^ $(LDLIBS)
	$(SIZE) -A -x $@

clean::
	rm -f $(PROGS) *.o *.s *.lst *.bin *.srec

run: prog.bin
	../../../srtc/rvddt -l40 -f prog.bin
