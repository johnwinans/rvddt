	.text				# put this into the text section
	.align	2			# align to 2^2
	.globl	_start

_start:

	la		t0, _vector			# this is undefined when this module is assembled!

	addi	t1, zero, 4			# t1 = 4
	addi	t2, zero, 0			# t2 = 0
	
loop:
	sw		t2, 0(t0)			# 0(t0) = t2
	addi	t0, t0, 4			# point to next element in the val array
	addi	t2, t2, 1			# t2 = t2 + 1

	blt		t2, t1, loop

done:
	ebreak
