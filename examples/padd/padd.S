	.text				# put this into the text section
	.align	2			# align to 2^2
	.globl	_start

_start:

	la		t0, val

	li		t1, 32				# t1 = 32 (num of elements in val array)
	li		t2, 0				# t2 = pointer used to fill inthe array
#	addi	t1, zero, 32		# t1 = 32 (num of elements in val array)
#	addi	t2, zero, 0			# t2 = 0
	
loop:
	sh		t2, 0(t0)			# 0(t0) = t2
	addi	t0, t0, 2			# point to next element in the val array
	addi	t2, t2, 1			# t2 = t2 + 1

	blt		t2, t1, loop		# if t2 < t1 then goto loop

done:
	ebreak
	
	.comm val, 64, 0x100		# align to a 256-byte boundary
