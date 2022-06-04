; This file was generated from himul.c by m68k-gcc and manually tweaked.
;
; It's a bit convoluted and needs to be rewritten at some point by hand.

	.text
	.align	2
	.globl	__himulu32
	.type	__himulu32, @function
__himulu32:
	lea (-40,sp),sp
	move.l d2,-(sp)
	move.l 48(sp),d0
	clr.w d0
	swap d0
	move.w d0,38(sp)
	move.w 50(sp),36(sp)
	move.l 52(sp),d0
	clr.w d0
	swap d0
	move.w d0,34(sp)
	move.w 54(sp),32(sp)

	move.w 38(sp),d0
	move.w 34(sp),d1
	mulu.w d1, d0

	move.l d0,28(sp)
	move.w 36(sp),d0
	move.w 34(sp),d1

	mulu.w d1, d0

	move.l d0,24(sp)

	move.w 38(sp),d0
	move.w 32(sp),d1
	mulu.w d1, d0

	move.l d0,20(sp)
	move.w 36(sp),d0
	move.w 32(sp),d1
	mulu.w d1, d0

	move.l d0,16(sp)
	move.l 16(sp),d0
	clr.w d0
	swap d0
	move.l 20(sp),d1
	add.l d0,d1
	move.l d1,12(sp)
	move.l 12(sp),d0
	move.l d0,d1
	clr.w d1
	swap d1
	move.l 24(sp),d0
	clr.w d0
	swap d0
	move.l d1,d2
	add.l d0,d2
	move.l d2,40(sp)
	move.l 12(sp),d1
	move.l 24(sp),d0
	move.w d1,a0
	add.w d0,a0
	move.w a0,10(sp)
	clr.l d1
	move.w 10(sp),d1
	move.l 12(sp),d0
	and.l #65535,d0
	cmp.l d1,d0
	bhi .L2
	clr.l d1
	move.w 10(sp),d1
	move.l 24(sp),d0
	and.l #65535,d0
	cmp.l d1,d0
	bls .L3
.L2:
	addq.l #1,40(sp)
.L3:
	move.l 40(sp),d0
	add.l 28(sp),d0
	move.l d0,6(sp)
	move.l 6(sp),d0
	move.l (sp)+,d2
	lea (40,sp),sp
	rts
