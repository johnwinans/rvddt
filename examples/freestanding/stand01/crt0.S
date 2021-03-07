  .text
  .global _start
  .type   _start, @function

_start:

  # Initialize global pointer
.option push
.option norelax
	la		gp,__global_pointer$
.option pop

    # Clear the bss segment
    la      a0,__bss_start      # a0 = start zeroing memory here
    la      a1,__BSS_END__      # a1 = end zeroing here-1

clear_bss:
    bgeu    a0,a1,done_bss      # while (!(a0 >= a1))
    sb      x0,0(a0)            #   *a0 = 0;
    addi    a0,a0,1             #   ++a0
    beq     x0,x0,clear_bss
done_bss:   

	# abort execution here
	ebreak

	.size  _start, .-_start
