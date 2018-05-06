# Count from zero to two using register t1

    .text               # put this into the text section
    .align  2           # align to 2^2
    .globl  _start
_start:

    addi    t0, zero, 3     # t0 = 3
    addi    t1, zero, 0     # t1 = 0

loop:
    addi    t1, t1, 1       # t1 = t1 + 1
    blt     t1, t0, loop    # if t1 < t0 then goto loop
    
done:
    ebreak
