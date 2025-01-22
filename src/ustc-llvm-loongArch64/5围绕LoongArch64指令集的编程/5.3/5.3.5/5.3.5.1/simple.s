	# 伪指令部分
	.text					# 标记代码段
	.globl	main			# 标记main全局可见
	.type	main, @function	# 标记main是一个函数

main:	# 标记程序入口，即main函数
	# main函数进入
	addi.d	$sp, $sp, -16	# 分配栈空间，-16为16字节
	st.d	$ra, $sp, 8		# 保存返回地址
	st.d	$fp, $sp, 0		# 保存fp寄存器
	addi.d	$fp, $sp, 16	# 设置栈帧指针
	# main函数退出
	addi.w	$a0, $zero, 0	# 设置返回值为 0
	ld.d	$ra, $sp, 8		# 恢复返回地址
	ld.d	$fp, $sp, 0		# 恢复fp寄存器
	addi.d	$sp, $sp, 16	# 释放栈空间
	jr		$ra				# 返回调用main函数的父函数