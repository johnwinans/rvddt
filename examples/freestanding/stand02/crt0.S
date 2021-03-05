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
	la      a0,_edata		# a0 = start zeroing memory here
	la      a1,_end			# a1 = end zeroing here-1
	li      a2,0

clear_bss:
	bgeu	a0,a1,done_bss
	sb		a2,0(a0)
	addi	a0,a0,1
	beq		x0,x0,clear_bss
done_bss:

	# abort execution here
	ebreak			

	.size  _start, .-_start


	// how to put items into the data region
	.data
v1:
	.word	0x11,0x22,0x33,0x44							// this is same as: int32_t v1[] = {0x11,0x22,0x33,0x44};
v2:
	.word	0x11111111,0x22222222,0x33333333,0x44444444	// this is same as: int32_t v2[] = {0x11111111,0x22222222,0x33333333,0x44444444};


	// how to put items into the bss
	.lcomm b1,4											// this is the same as: int32_t b1;
	.lcomm b2,16										// this is the same as: int32_t b2[4];
