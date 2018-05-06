	.text				# put this into the text section
	.align	2			# align to 2^2
	.globl	_start
_start:

	li		t0, 0xffff0000		# UART base address

	addi	t1, zero, 79
	addi	t2, zero, 'z'

loop:
	sb		t2, 1(t0)		 # write a 'z' character to UART tx register
	addi	t1, t1, -1
	bne		t1, zero, loop

	addi    t2, zero, '\n'
	sb		t2, 1(t0)		# be kind, rewind
	
done:
	ebreak
