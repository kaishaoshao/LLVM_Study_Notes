	.text
	.globl	main
	.type		main, @function
main:
	addi.d		$sp, $sp, -16 
	st.d		$ra, $sp, 8
	st.d		$fp, $sp, 0
	addi.d		$fp, $sp, 16
	addi.w		$t0, $zero,8		# 0x8的值赋给t0
	movgr2fr.w	$ft0, $t0			# 将t0的内容搬运到浮点寄存器ft0，ft0 = 0x8
	ffint.s.w	$ft0, $ft0			# 将ft0中存放的定点数转换为单精度浮点，ft0 = 8.0
	lu12i.w		$t1, 0x40600		# t1= 0x40600000，即浮点数3.5的单精度表示
	movgr2fr.w	$ft1, $t1			# 将t1的内容搬运到浮点寄存器ft1，ft1 = 3.5
	fcmp.clt.s	$fcc0, $ft0, $ft1	# 如果ft0<ft1，则置条件标志fcc0为1
	bceqz		$fcc0, .L1			# 如果fcc0等于0，则跳转到.L1处
	addi.w		$t2, $zero, 1		# t2 = 1
	b			.L2					# 跳转到.L2处
.L1:
	or			$t2, $zero, $zero	# t2 = 0
.L2:
	or			$a0, $t2, $zero		# 将t2写入返回值a0
	ld.d		$ra, $sp, 8
	ld.d		$fp, $sp, 0
	addi.d		$sp, $sp, 16
	jr			$ra
