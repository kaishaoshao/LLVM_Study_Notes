	# 伪指令部分
	.text					# 标记代码段 
	.globl	add				# 标记add全局可见
	.type	add, @function	# 标记add是一个函数
add:	# 标记add 函数起始位置，两个参数分别位于a0和a1中
	addi.d	$sp, $sp, -16	# 分配栈空间，-16为16字节
	st.d	$ra, $sp, 8		# 保存返回地址，即bl add指令的下一条指令地址
	st.d	$fp, $sp, 0		# 保存fp寄存器
	addi.d	$fp, $sp, 16	# 设置帧指针
	add.d	$a0, $a0, $a1	# 计算a0+a1，函数返回值存储到a0中
	ld.d	$ra, $sp, 8		# 恢复返回地址
	ld.d	$fp, $sp, 0		# 恢复fp寄存器
	addi.d	$sp, $sp, 16	# 释放栈空间
	jr		$ra				# 返回main函数

	.globl	main			# 标记main全局可见
	.type	main, @function	# 标记main是一个函数
main:	# 标记程序入口，即main函数
	addi.d	$sp, $sp, -16	# 分配栈空间，-16为16字节
	st.d	$ra, $sp, 8		# 保存返回地址
	st.d	$fp, $sp, 0		# 保存fp寄存器
	addi.d	$fp, $sp, 16	# 设置帧指针

	addi.w	$a0, $zero, 1	# 设置第一个参数
	addi.w	$a1, $zero, 2	# 设置第二个参数
	bl		add				# 调用add函数
	# add的返回值在a0中，不需要操作就可以传入output函数
	bl		output# 调用output函数输出结果

	addi.w	$a0, $zero, 0 	# 返回0
	ld.d	$ra, $sp, 8		# 恢复返回地址
	ld.d	$fp, $sp, 0		# 恢复fp寄存器
	addi.d	$sp, $sp, 16	# 释放栈空间（必需）
	jr		$ra				# 返回调用main函数的父函数