	.text
	.attribute	4, 16
	.attribute	5, "rv64i2p1_m2p0_a2p1_f2p2_d2p2_c2p0_zicsr2p0_zmmul1p0"
	.file	"pow.c"
	.globl	main                            # -- Begin function main
	.p2align	1
	.type	main,@function
main:                                   # @main
# %bb.0:
	addi	sp, sp, -32
	sd	ra, 24(sp)                      # 8-byte Folded Spill
	sd	s0, 16(sp)                      # 8-byte Folded Spill
	addi	s0, sp, 32
	li	a0, 0
	sd	a0, -32(s0)                     # 8-byte Folded Spill
	sw	a0, -20(s0)
	lui	a0, 262144
	fmv.w.x	fa0, a0
	lui	a0, 264192
	fmv.w.x	fa1, a0
	call	powf
	fsw	fa0, -24(s0)
	flw	fa5, -24(s0)
	fcvt.d.s	fa5, fa5
	fmv.x.d	a1, fa5
.Lpcrel_hi0:
	auipc	a0, %pcrel_hi(.L.str)
	addi	a0, a0, %pcrel_lo(.Lpcrel_hi0)
	call	printf
                                        # kill: def $x11 killed $x10
	ld	a0, -32(s0)                     # 8-byte Folded Reload
	ld	ra, 24(sp)                      # 8-byte Folded Reload
	ld	s0, 16(sp)                      # 8-byte Folded Reload
	addi	sp, sp, 32
	ret
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
                                        # -- End function
	.type	.L.str,@object                  # @.str
	.section	.rodata.str1.8,"aMS",@progbits,1
	.p2align	3, 0x0
.L.str:
	.asciz	"a=%f\n"
	.size	.L.str, 6

	.ident	"Terapines LTD ZCC 4.1.1 (0a1162463d6c814fbcd077e53f7a0b55c96c5dd2) based on LLVM 19.1.6 [ Non-commercial use ](124523)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym printf
