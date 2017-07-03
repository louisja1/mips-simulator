    .data
    .word 1
_static_0:
    .asciiz "\n"
    .align 2
    .word 0
_static_2:
    .asciiz ""
    .align 2
_static_1:
    .word 0
    .data
_buffer:
    .word 0
    .text
_buffer_init:
    li $a0, 256
    li $v0, 9
    syscall
    sw $v0, _buffer
    jr $ra
    .text
main:
	jal _buffer_init
	add $fp, $zero, $sp
	li $8, 4
	add $9, $8, 1
	mul $10, $9, 4
	li $v0, 9
	move $a0, $10
	syscall
	sw $v0, -12($fp)
	li $8, 4
	lw $9, -12($fp)
	sw $8, 0($9)
	add $10, $9, 4
	sw $10, _static_1
	jal _func_main
	move $a0, $v0
	li $v0, 17
	syscall
_func_main:
	sw $fp, -256($sp)
	add $fp, $sp, -4
	add $sp, $fp, -260
	sw $ra, 0($sp)
	lw $8, _static_1
	sub $9, $8, 4
	lw $10, 0($9)
	add $11, $10, 1
	mul $12, $11, 4
	li $v0, 9
	move $a0, $12
	sw $10, -12($fp)
	syscall
	sw $v0, -24($fp)
	lw $8, -12($fp)
	lw $9, -24($fp)
	sw $8, 0($9)
	add $10, $9, 4
	li $11, 0
	sw $10, 0($fp)
	sw $11, -4($fp)
