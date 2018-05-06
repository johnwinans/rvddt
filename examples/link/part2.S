	.data				# put this into the data section
	.align	2			# align to 2^2
	.globl	_vector		# make _vector visible to other load modules

	.ascii		"STRT"

_vector:
	.skip		4*4, 0xaa

	.ascii		"END"
