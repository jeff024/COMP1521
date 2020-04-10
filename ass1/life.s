# board1.s ... Game of Life on a 10x10 grid

	.data

N:	.word 10  # gives board dimensions

board:
	.byte 1, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 1, 1, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 1, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 1, 0, 1, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 1, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 1, 1, 1, 0, 0, 0
	.byte 0, 0, 0, 1, 0, 0, 1, 0, 0, 0
	.byte 0, 0, 1, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 1, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 1, 0, 0, 0, 0, 0, 0, 0

newBoard: .space 100
# COMP1521 19t2 ... Game of Life on a NxN grid
#
# Written by haodong lu, June 2019

## Requires (from `boardX.s'):
# - N (word): board dimensions
# - board (byte[][]): initial board state
# - newBoard (byte[][]): next board state

## Provides:
	.globl	main
	.globl	decideCell
	.globl	neighbours
	.globl	copyBackAndShow

	.data
msg1:	.asciiz "# Iterations: "
msg2:	.asciiz "=== After iteration "
msg3:	.asciiz " ===\n"

########################################################################
# .TEXT <main>
	.text
main:

# Frame:		$fp, $ra, $s0, $s1, $s2, $s3, $s4
# Uses:			$s0, $s1, $s2, $s3, $s4, $t0, $t1, $t2, $t3
# Clobbers:		$t0, $t1, $t2, $t3

# Locals:
#		- 'maxiters' in $s0 
#		- 'n' in $s1
#		- 'i' in $s2
#		- 'j' in $s3
#		- 'N' in $s4

# Structure:
#		main
#		-> [prologue]
#		-> main_n_init
#		-> main_n_cond
#			-> main_i_init
#			-> main_i_cond
#				-> main_j_init
#				-> main_j_cond
#					-> main_j_step
#				=> main_j_f
#				-> main_i_step
#			=> main_i_f
#			-> main_n_step
#		=> main_n_f
#		-> [epilogue]

# code: 

# prologue
	sw	$fp, -4($sp)	    # push $fp onto stack
	la	$fp, -4($sp)	    # set up $fp for this function
	sw	$ra, -4($fp)	    # save return address
	sw	$s0, -8($fp)	    # save $s0 to use as int maxiters;
	sw	$s1, -12($fp)	    # save $s1 to use as int n;
	sw	$s2, -16($fp)	    # save $s2 to use as int i;
	sw	$s3, -20($fp)	    # save $s3 to use as int j;
	sw	$s4, -24($fp)	    # save $s4 to use as N;
	addi	$sp, $sp, -28	# reset $sp to last pushed item

	lw $s4, N
	
	la $a0, msg1		    # printf("# Iterations: ")
	li $v0, 4
	syscall

	li $v0, 5			    # scanf("%d", into $v0)
	syscall 
	move $s0, $v0		    # maxiters = $v0 (scaned value)

# start of loop_n
main_n_init:
	li $s1, 1			    # $s1 = 1 (used as n)
main_n_cond:
	bgt $s1, $s0, main_n_f
	nop
# start of loop_i
main_i_init:
	li $s2, 0			    # $s2 = 0 (used as i)
main_i_cond:
	bge $s2, $s4, main_i_f
	nop
# start of loop_j
main_j_init:
	li $s3, 0			    # $t2 = 0 (used as j)
main_j_cond:
	bge $s3, $s4, main_j_f	# if (j >= N) jump to end_loop_2
	nop
# start of statment in loop_j
	move $a0, $s2		    # give the function two arguments i, j
	move $a1, $s3
	jal neighbours		    # call the funtion neighbours
	nop

	move $a1, $v0		    # $a1 = nn
	
	mul	$t0, $s2, $s4	    # % <- row * N
	add	$t0, $t0, $s3	    #    + col
	lb $a0, board($t0)		# $a0 = board
	jal decideCell 		    # call the function neighbours
	nop

	mul	$t0, $s2, $s4	    # % <- row * N
	add	$t0, $t0, $s3	    #    + col
	sb $v0, newBoard($t0)   # store the returned value in newBoard

# end of statement in loop_j
main_j_step:
	addi $s3, $s3, 1	    # j ++
	j main_j_cond			# continue on loop_j

# end of loop_j
main_j_f:

main_i_step:
	addi $s2, $s2, 1	    # i ++
	j main_i_cond			# continue on loop_i

# end of loop_i
main_i_f:
	la $a0, msg2		    # printf("=== After iteration ")
	li $v0, 4
	syscall

	la $a0, ($s1)			# printf("%d", n)
	li $v0, 1
	syscall

	la $a0, msg3		    # printf(" ===\n")
	li $v0, 4
	syscall

	jal copyBackAndShow	    # call the funtion copyBackAndShow()
	nop
	
main_n_step:
	addi $s1, $s1, 1	    # n++
	j main_n_cond			# continue on loop_n

# end of loop_n
main_n_f:

# epilogue	
	lw	$s4, -24($fp)		# restore $s4 value
	lw	$s3, -20($fp)		# restore $s3 value
	lw	$s2, -16($fp)		# restore $s2 value
	lw	$s1, -12($fp)		# restore $s1 value
	lw	$s0, -8($fp)		# restore $s0 value
	lw	$ra, -4($fp)		# restore $ra for return
	la	$sp, 4($fp)			# restore $sp (remove stack frame)
	lw	$fp, ($fp)			# restore $fp (remove stack frame)
	jr	$ra					# return

########################################################################
# .text <decideCell>
	.text
decideCell:

# prologue
	sw	$fp, -4($sp)		# push $fp onto stack
	la	$fp, -4($sp)		# set up $fp for this function
	sw	$ra, -4($fp)		# save return address
	sw	$s0, -8($fp)		# save $s0 to use as ... int old;
	sw	$s1, -12($fp)		# save $s1 to use as ... int nn;
	addi	$sp, $sp, -16	# reset $sp to last pushed item
# code
	move $s0, $a0			# load board[i][j] into $s0 (old)
	move $s1, $a1			# load nn into $s1

	bne $s0, 1, decide_else_if	# if old != 1, goes to decide_else_if
	nop

	bge $s1, 2, oldOne_else_if	# if nn >= 2, goes to oldOne_else_if
	nop
	j ret_0					# return ret = 0
oldOne_else_if:
	beq $s1, 2, ret_1		# if nn == 2, return ret = 1
	nop
	beq $s1, 3, ret_1		# if nn == 3, return ret = 1
	nop
oldOne_else:
	j ret_0					# else, return ret = 0

decide_else_if:
	bne $s1, 3, decide_else	# if nn != 3, goes to decide_else
	nop
	j ret_1					# return ret = 1

decide_else:
	j ret_0					# else, return ret = 0

ret_1:
	li $v0, 1
	j decide_epi

ret_0:
	li $v0, 0

decide_epi:
## epilogue
	lw	$s1, -12($fp)		# restore $s1 value
	lw	$s0, -8($fp)		# restore $s0 value
	lw	$ra, -4($fp)		# restore $ra for return
	la	$sp, 4($fp)			# restore $sp (remove stack frame)
	lw	$fp, ($fp)			# restore $fp (remove stack frame)
	jr	$ra					# return
###########################################################################
# .text <neighbours>
	.text
neighbours:

# Frame:		$fp, $ra, $s0, $s1, $s2, $s3, $s4
# Uses:			$s0, $s1, $s2, $s3, $s4, $t0, $t1, $t2, $t3
# Clobbers:		$a0, $a1, $t0, $t1, $t2, $t3

# Locals:
#		- 'i' in $s0 
#		- 'j' in $s1
#		- 'x' in $s2
#		- 'y' in $s3
#		- 'nn' in $s4

# Structure:
#		neighbours
#		-> [prologue]
#		-> neighbour_x_init
#		-> neighbour_x_cond
#			-> neighbour_y_init
#			-> neighbour_y_cond
#				->if_add
#				->add_nn
#				-> neighbour_y_step
#			=> neighbour_y_f
#			-> neighbour_x_step
#		=> neighbour_x_f
#		-> [epilogue]

# code: 

# prologue
	sw	$fp, -4($sp)		# push $fp onto stack
	la	$fp, -4($sp)		# set up $fp for this function
	sw	$ra, -4($fp)		# save return address
	sw	$s0, -8($fp)		# save $s0 to use as ... int i;
	sw	$s1, -12($fp)		# save $s1 to use as ... int j;
	sw	$s2, -16($fp)		# save $s2 to use as ... int x;
	sw	$s3, -20($fp)		# save $s3 to use as ... int y;
	sw	$s4, -24($fp)		# save $s4 to use as ... int nn;
	addi	$sp, $sp, -28	# reset $sp to last pushed item

	li $s4, 0				# int nn = 0
	move $s0, $a0			# load i and j 
	move $s1, $a1

# body
neighbour_x_init:
	li	$s2, -1				# x = -1
neighbour_x_cond:
	bgt	$s2, 1, neighbour_x_f
	nop						#[branch delay]

neighbour_y_init:
	li	$s3, -1				# y = 0
neighbour_y_cond:
	
	bgt	$s3, 1, neighbour_y_f
	nop						#[branch delay]

	add $t0, $s0, $s2		# t0 = i + x
	add $t1, $s1, $s3		# t1 = j + y
	lw $t3, N
	addi $t3, $t3, -1		# t3 = N - 1

	blt $t0, 0, neighbour_y_step
	nop
	bgt $t0, $t3, neighbour_y_step
	nop
	blt $t1, 0, neighbour_y_step
	nop
	bgt $t1, $t3, neighbour_y_step
	nop
	bne $s2, 0, if_add
	nop
	bne $s3, 0, if_add
	nop
	j neighbour_y_step

if_add:
	la $t5, board
	lw $t6, N
	mul	$t4, $t0, $t6		# % <- row * N
	add	$t4, $t4, $t1		#    + col
	lb $t6, board($t4)		# $t6 = board[i][j]
	li $t7, 1
	beq $t6, $t7, add_nn	# if board[i][j] == 1, goes to add_nn
	nop
	j neighbour_y_step		# continue on loop_y

add_nn:
	addi $s4, $s4, 1

neighbour_y_step:
	addi	$s3, $s3, 1		# y++
	j	neighbour_y_cond
	nop						#[branch delay]
neighbour_y_f:

neighbour_x_step:
	addi	$s2, $s2, 1		# x++
	j	neighbour_x_cond
	nop						#[branch delay]
neighbour_x_f:

# epilogue
neighbour__epi:
	move $v0, $s4
	lw	$s4, -24($fp)		# restore $s4 value
	lw	$s3, -20($fp)		# restore $s3 value
	lw	$s2, -16($fp)		# restore $s2 value
	lw	$s1, -12($fp)		# restore $s1 value
	lw	$s0, -8($fp)		# restore $s0 value
	lw	$ra, -4($fp)		# restore $ra for return
	la	$sp, 4($fp)			# restore $sp (remove stack frame)
	lw	$fp, ($fp)			# restore $fp (remove stack frame)
	jr	$ra					# return

###########################################################################
# .text <copyBackAndShow>
	.text
copyBackAndShow:
## prologue
	sw	$fp, -4($sp)		# push $fp onto stack
	la	$fp, -4($sp)		# set up $fp for this function
	sw	$ra, -4($fp)		# save return address
	sw	$s0, -8($fp)		# save $s0 to use as ... int i;
	sw	$s1, -12($fp)		# save $s1 to use as ... int j;
	sw	$s2, -16($fp)		# save $s2 to use as ... N;
	addi	$sp, $sp, -20	# reset $sp to last pushed item
## code:
	lw $s2, N

	copy_row_init:
	li	$s0, 0				# i = 0
copy_row_cond:
	bge	$s0, $s2, copy_row_f
	nop						#[branch delay]
copy_col_init:
	li	$s1, 0				# j = 0
copy_col_cond:
	bge	$s1, $s2, copy_col_f	
	nop						#[branch delay]

	mul	$t0, $s0, $s2		# % <- row * N
	add	$t0, $t0, $s1		#    + col

	lb $a0, newBoard($t0)
	sb $a0, board($t0)

	bne $a0, 0, put_hash
	nop
	li	$a0, '.'
	li	$v0, 11
	syscall					# putchar('.')
	j copy_col_step
put_hash:
	li	$a0, '#'
	li	$v0, 11
	syscall					# putchar('#')

copy_col_step:
	addi	$s1, $s1, 1		# col++
	j	copy_col_cond
	nop						#[branch delay]
copy_col_f:

	li	$a0, '\n'
	li	$v0, 11
	syscall			        # putchar('\n')

copy_row_step:
	addi	$s0, $s0, 1	    # row++
	j	copy_row_cond
	nop	                    #[branch delay]
copy_row_f:

copy_epi:

## epilogue
	lw	$s2, -16($fp)		# restore $s2 value
	lw	$s1, -12($fp)		# restore $s1 value
	lw	$s0, -8($fp)		# restore $s0 value
	lw	$ra, -4($fp)		# restore $ra for return
	la	$sp, 4($fp)			# restore $sp (remove stack frame)
	lw	$fp, ($fp)			# restore $fp (remove stack frame)
	jr	$ra					# return
