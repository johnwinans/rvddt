  .text
  .global _start
  .type   _start, @function

_start:

  # Initialize global pointer
.option push
.option norelax
setgp:	
	auipc gp,%pcrel_hi(__global_pointer$)
	addi  gp,gp, %pcrel_lo(setgp)
.option pop

	# Clear the bss segment

	// a0 = _edata
set_edata:
	auipc	a0,%pcrel_hi(_edata)
	addi	a0,a0,%pcrel_lo(set_edata)

	// a1 = _end
set_end:
	auipc	a1,%pcrel_hi(_end)
	addi	a1,a1,%pcrel_lo(set_end)

	// a2 = 0
	add		a2,x0,x0

clear_bss:
	bgeu	a0,a1,done_bss
	sb		a2,0(a0)
	addi	a0,a0,1
	beq		x0,x0,clear_bss
done_bss:

	ebreak


	.size  _start, .-_start
