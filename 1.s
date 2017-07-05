.text
main:
    la   $t0,  number    #读取两个整数并放入寄存器$t1,$t2
	lw   $t1,  0($t0)
	lw   $t2,  4($t0)
	li   $v0,  4	#打印msg1
	la   $a0,  msg1
	syscall
	li   $v0,  1        #打印转换前$t1中的值
	move $a0,  $t1
	syscall
	li   $v0,  4        #打印msg2
	la   $a0,  msg2
	syscall
	li   $v0,  1	#打印转换前$t2中的值
	move $a0,  $t2
	syscall
	move $t3,  $t1      #关键部分，在寄存器间move数据
	move $t1,  $t2
	move $t2,  $t3
	li   $v0,  4        #打印msg3
	la   $a0,  msg3
	syscall

	li   $v0,  1        #打印转换后$t1中的值
	move $a0,  $t1
	syscall
	li   $v0,  4	#打印换行符  /n
	la   $a0,  msg4
	syscall
	li   $v0,  1	#打印转换后$t2中的值
	move $a0,  $t2
	syscall
    li $a0 0
    li $v0 17
    syscall

	.data
number:
	.word 12,34
msg1:
	.asciiz  "the first number is:\n"
msg2:
	.asciiz  "\nthe second number is:\n"
msg3:
	.asciiz  "\nnow they are swapped as:\n"
msg4:
	.ascii   "\n"
