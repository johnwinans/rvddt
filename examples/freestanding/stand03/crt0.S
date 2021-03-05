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
	la      a0, _edata		# a0 = start zeroing memory here
	la      a1, _end		# a1 = end zeroing here-1
	li      a2, 0

clear_bss:
	bgeu	a0,a1,finish_bss
	sb		a2,0(a0)
	addi	a0,a0,1
	beq		x0,x0,clear_bss

finish_bss:
	li		a0,0		# a0 = argc = 0
	li		a1,0		# a1 = argv = NULL
	li		a2,0		# a2 = envp = NULL

#if 1
	call    main
#else
callmain:
	auipc   x1,%pcrel_hi(main)
	jalr    x1,%pcrel_lo(callmain)(x1)
#endif

	# abort execution here
	ebreak			

	.size  _start, .-_start
