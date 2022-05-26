	.align 2
	.globl __mulu32
	.type  __mulu32, @function
__mulu32:
	link    fp, #-0

	move.w   8(fp), d0     ; d0  = a0
	mulu.w  14(fp), d0     ; d0 *= b1
	move.w  10(fp), d1     ; d1  = a1
	mulu.w  12(fp), d1     ; d1 *= b0

	add.w   d1, d0         ; d0 += d1
	swap    d0
	clr.w   d0             ; d0  = (d0 >> 16) << 16

	move.w  10(fp), d1     ; d1  = a1
	mulu.w  14(fp), d1     ; d1 *= b1
	add.l   d1,d0          ; d0 += d1

	unlk    fp
	rts
