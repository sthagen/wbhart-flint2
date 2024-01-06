#
#   Copyright (C) 2023 Albin Ahlbäck
#
#   This file is part of FLINT.
#
#   FLINT is free software: you can redistribute it and/or modify it under
#   the terms of the GNU Lesser General Public License (LGPL) as published
#   by the Free Software Foundation; either version 2.1 of the License, or
#   (at your option) any later version.  See <https://www.gnu.org/licenses/>.
#

include(`config.m4')dnl
dnl
.text

.macro	m7 res=%rdi, res_offset=0, ap=%rsi, ap_offset=0, r0, r1, r2, r3, r4, r5, r6, scr1, scr2, zero
	mulx	(0+\ap_offset)*8(\ap), \scr1, \r0
	mulx	(1+\ap_offset)*8(\ap), \scr2, \r1
	mov	\scr1, \res_offset*8(\res)
	adcx	\scr2, \r0
	mulx	(2+\ap_offset)*8(\ap), \scr1, \r2
	mulx	(3+\ap_offset)*8(\ap), \scr2, \r3
	adcx	\scr1, \r1
	adcx	\scr2, \r2
	mulx	(4+\ap_offset)*8(\ap), \scr1, \r4
	mulx	(5+\ap_offset)*8(\ap), \scr2, \r5
	adcx	\scr1, \r3
	adcx	\scr2, \r4
	mulx	(6+\ap_offset)*8(\ap), \scr1, \r6
	adcx	\scr1, \r5
	adcx	\zero, \r6
.endm

.macro	am7 res=%rdi, res_offset=0, ap=%rsi, ap_offset=0, r0, r1, r2, r3, r4, r5, r6, r7, scr, zero
	mulx	(0+\ap_offset)*8(\ap), \scr, \r7
	adcx	\scr, \r0
	mov	\r0, \res_offset*8(\res)
	mulx	(1+\ap_offset)*8(\ap), \r0, \scr
	adcx	\r7, \r1
	adox	\r0, \r1
	mulx	(2+\ap_offset)*8(\ap), \r0, \r7
	adcx	\scr, \r2
	adox	\r0, \r2
	mulx	(3+\ap_offset)*8(\ap), \r0, \scr
	adcx	\r7, \r3
	adox	\r0, \r3
	mulx	(4+\ap_offset)*8(\ap), \r0, \r7
	adcx	\scr, \r4
	adox	\r0, \r4
	mulx	(5+\ap_offset)*8(\ap), \r0, \scr
	adcx	\r7, \r5
	adox	\r0, \r5
	mulx	(6+\ap_offset)*8(\ap), \r0, \r7
	adcx	\scr, \r6
	adox	\r0, \r6
	adcx	\zero, \r7
	adox	\zero, \r7
.endm

.global	FUNC(flint_mpn_mul_7_2)
.p2align	4, 0x90
TYPE(flint_mpn_mul_7_2)

FUNC(flint_mpn_mul_7_2):
	.cfi_startproc
	mov	%rdx, %rcx
	mov	0*8(%rcx), %rdx
	push	%rbx
	push	%rbp
	push	%r12
	push	%r13
	push	%r14

	xor	%r14d, %r14d

	m7	%rdi, 0, %rsi, 0, %r12, %r8, %r9, %r10, %r11, %rbx, %rbp, %rax, %r13, %r14

	mov	1*8(%rcx), %rdx
	am7	%rdi, 1, %rsi, 0, %r12, %r8, %r9, %r10, %r11, %rbx, %rbp, %rax, %r13, %r14

	mov	%r8, 2*8(%rdi)
	mov	%r9, 3*8(%rdi)
	mov	%r10, 4*8(%rdi)
	mov	%r11, 5*8(%rdi)
	mov	%rbx, 6*8(%rdi)
	mov	%rbp, 7*8(%rdi)
	mov	%rax, 8*8(%rdi)

	pop	%r14
	pop	%r13
	pop	%r12
	pop	%rbp
	pop	%rbx

	ret
.flint_mpn_mul_7_2_end:
SIZE(flint_mpn_mul_7_2, .flint_mpn_mul_7_2_end)
.cfi_endproc
