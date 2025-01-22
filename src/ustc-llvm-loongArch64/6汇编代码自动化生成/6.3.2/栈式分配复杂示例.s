.text
	.globl	main
	.type	main, @function
main:
	addi.d		$sp, $sp, -32		# 更改栈指针
	st.d			$ra, $sp, 24		# 保存返回地址
	st.d			$fp, $sp, 16		# 保存帧指针
	addi.d		$fp, $sp, 32		# 当前函数栈帧底位置
	# a = 17
	addi.w		$t0, $zero, 17		# $t0 = 17
	st.w			$t0, $fp, -20 		# $fp-20 为a的栈帧地址
	# b = 34
	addi.w		$t0, $zero, 34		# $t0 = 34
	st.w			$t0, $fp, -24		# $fp-24 为b的栈帧地址
	# c = a + b
	ld.w			$t0, $fp, -20 		# $t0 = a
	ld.w			$t1, $fp, -24 		# $t1 = b
	add.w		$t2, $t0, $t1 		# $t2 = $t0 + $t1 = a + b
	st.w			$t2, $fp, -28 		# $fp-28 为c的栈帧地址
	# output(c)
	ld.w			$a0, $fp, -28 		# $a0 = c，设置函数参数
	bl			output			# 调用函数output
	ld.d			$ra, $sp, 24		# 恢复main返回地址
	ld.d			$fp, $sp, 16		# 恢复main父函数栈底
	addi.d		$sp, $sp, 32		# 恢复栈指针
	jr			$ra 				# 跳转到返回地址
