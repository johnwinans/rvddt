	.text				# put this into the text section
	.align	2			# align to 2^2
	.globl	_start
_start:

	sw		t1, val, a0
	sw		t2, val, a1
	sw		t3, val, x29

	li		a0,0x12345678
	la		a1, val

	auipc	t0, %hi(val)
	addi	t0, t0, %lo(val)
	
done:
	ebreak
	
val:
	.word		0
