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

	call    __libc_init_array       # Run global initialization functions

	li		a0,0		# a0 = argc = 0
	la		a1,argv		# a1 = argv = { NULL };
	la		a2,argv		# a2 = envp = { NULL };

	call    main

	# abort execution here
	ebreak			

	.size  _start, .-_start

argv:
	.word	0		# a 32-bit integer whose value is zero (NULL)
