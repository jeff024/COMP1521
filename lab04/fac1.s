### COMP1521 19t2 ... week 04 lab
### Compute factorials -- no functions (except main)

## Global data
	.data
msg1:	.asciiz "n  = "
msg2:	.asciiz "n! = "
eol:	.asciiz "\n"


## main() function
	.text
	.globl	main
main:
	# Set up stack frame.
	sw	$fp, -4($sp)	# push $fp onto stack
	la	$fp, -4($sp)	# set up $fp for this function
	sw	$ra, -4($fp)	# save return address
	sw	$s0, -8($fp)	# save $s0 to use as ... int n
	sw	$s1, -12($fp)	# save $s1 to use as ... int i
	sw	$s2, -16($fp)	# save $s2 to use as ... int fac
	addi	$sp, $sp, -20	# move $sp to top of stack

	# code for main()
	li	$s0, 0		# n = 0

	la	$a0, msg1
	li	$v0, 4
	syscall			# printf("n  = ")

	li	$v0, 5
	syscall			# scanf("%d", into $v0)

	### TODO: your code for the body of main() goes here
	move $s0, $v0
	li $s1, 1
	li $s2, 1

while:
	bgt $s1, $s0, end_while
	mult $s2, $s1
	mflo $s2
	addi $s1, $s1, 1
	j while

end_while:
	
	### end of lab04 coding exercise

	la	$a0, msg2
	li	$v0, 4
	syscall			# printf("n! = ")

	move	$a0, $s2	# assume $s2 holds n!
	li	$v0, 1
	syscall			# printf("%d", fac)

	la	$a0, eol
	li	$v0, 4
	syscall			# printf("\n")

	# clean up stack frame
	lw	$s2, -16($fp)	# restore $s2 value
	lw	$s1, -12($fp)	# restore $s1 value
	lw	$s0, -8($fp)	# restore $s0 value
	lw	$ra, -4($fp)	# restore $ra for return
	la	$sp, 4($fp)	# restore $sp (remove stack frame)
	lw	$fp, ($fp)	# restore $fp (remove stack frame)

	li	$v0, 0
	jr	$ra		# return 0


