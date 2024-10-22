	# 伪指令部分
	.text            					# 标记代码段
    .section 	.bss, “aw”, @nobits		# 将后面的全局变量放到BSS段, 并设置属性
	.globl		a						# 标记a为全局符号
	.type 		a, @object				# 标记a为数据对象/变量
	.size 		a, 4					# 标记a的大小为4字节
.a:	# 标记全局变量a
	.space 		4						# 为a分配4字节的空间

	.text								# 标记代码段
	.globl 		main					# 标记main为全局符号
	.type 		main, @function			# 标记main是一个函数
main:	# 标记程序入口，即main函数
	addi.d		$sp, $sp, -16			# 分配栈空间，-16为16字节
	st.d		$ra, $sp, 8  			# 保存返回地址
	st.d		$fp, $sp, 0				# 保存fp寄存器
	addi.d		$fp, $sp, 16			# 设置帧指针
.main_label_entry:        				# 分支判断入口
	addi.w		$t4, $zero, 4			# t4 = 4
	addi.w		$t2, $zero, 0 			# t2 = 0
	la.local	$t0, a    				# 将a所处的内存地址加载入t0
	stptr.w		$t4, $t0, 0  			# 将t4的数据保存入t0指向的地址中
	blt			$t2, $t4, .main_then	# 将t2和t4比较，若t2<t4则跳转到.main_then
	b			.main_else       		# 否则跳转到.main_else
.main_else:                    
	addi.w		$a0, $zero, 0			# 设置返回值为 0
	b			.main_label_return		# 跳转到.main_label_return
.main_then:                    
	addi.w		$a0, $zero, 1			# 设置返回值为 1
	b			.main_label_return		# 跳转到.main_label_return
.main_label_return:        				# 标记分支出口
	ld.d		$ra, $sp, 8   			# 恢复返回地址
	ld.d		$fp, $sp, 0				# 恢复fp寄存器
	addi.d		$sp, $sp, 16			# 释放栈空间
	jr			$ra            			# 返回调用main函数的父函数