.global enable_interrupt      
.global delay

.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm


.text

enable_interrupt:
	ei
	jr $ra

delay:		#$t4 as an input int value, $t3 as an i.
	PUSH $ra
	PUSH $s0
	PUSH $s1
	move	$s0, $a0	#Setting ms value. Change it to adjust delay duration
	li	$s1, 4000
	jal	void
	nop
	slt	$t1, $zero, $s0	#if 0 < ms
	bne	$t1, $zero, while_delay   # Go to while
	nop
	POP $s1
	POP $s0
	POP $ra
	jr	$ra
	nop
	
while_delay:
	PUSH $ra
	PUSH $s2
	addi	$s0, $s0, -1        #ms = ms - 1 
	li	$s2, 0		# i = 0
	jal	void
	nop
	slt	$t1, $s2, $s1	#if i < $s1(400 in this case)
	bne	$t1, $zero, for_delay
	nop
	POP $s2
	POP $ra
	jr	$ra		#Go to the next while loop
	nop
	
for_delay: 
	addi	$s2, $s2, 1
	jr	$ra
	nop

void:	jr	$ra
	nop
        

