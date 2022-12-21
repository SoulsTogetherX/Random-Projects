.data
#;	System Service Codes
	SYSTEM_EXIT				= 10
	SYSTEM_PRINT_STRING		= 4
	SYSTEM_READ_INTEGER		= 5
	SYSTEM_READ_STRING		= 8
	SYSTEM_PRINT_CHAR		= 11
	SYSTEM_READ_CHAR		= 12

	#; ASCII Character Codes
	NULL					= 0
	LF						= 10

	#; TicTacToe Piece Codes
	X_						= 0
	O_						= 1
	EMPTY					= 2
	TIE						= 3
	TickTacks:		.asciiz	  "XO "

	#; GameBoard
	BOARDSIZE				= 3
	gameBoardData:	.space	  9
	gameBoardSlice:	.asciiz	  " ===========\n"
	gameBoardVal:	.asciiz	  "| - | - | - |\n"

	#; User prompts
	turn:			.asciiz	  "Current turn: -\n"
	coords:			.asciiz	  "Enter X and Y coords: "
	gamingAddic:	.asciiz	  "Play again? (Y/n): "

	#; Errors
	unknownChar:	.asciiz	  "Error - Unrecognized character inputted.\n\n"
	badXCoord:		.asciiz	  "Error - X coordinate must between 1-3.\n\n"
	badYCoord:		.asciiz	  "Error - Y coordinate must between 1-3.\n\n"
	nonEmptyCoord:	.asciiz	  "Error - coordinate position (x, x) is nonempty.\n"

	#; End Game Messages
	xWins:		.asciiz	  "X won!\n"
	oWins:		.asciiz	  "O won!\n"
	tie:		.asciiz	  "It's a tie!\n"

#;  Main function
#;  No command-line arguments
.text
.globl main
.ent main
main:
	li $a0, '\n'
	li $v0, SYSTEM_PRINT_CHAR
	syscall

	#; Plays the game immediately
	li $a0, X_
	jal runGame_TicTacToe

	#; Checks who won and outputs the expected victory message
	beq $v0, X_, end1
	beq $v0, O_, end2
	beq $v0, TIE, end3

	end1:
		la $a0, xWins
	j finish
	end2:
		la $a0, oWins
	j finish
	end3:
		la $a0, tie
	j finish
	finish:
	li $v0, SYSTEM_PRINT_STRING
	syscall

badInput_:

	#; Checks if the player wishes to play again
	la $a0, gamingAddic
	li $v0, SYSTEM_PRINT_STRING
	syscall

	li $v0, SYSTEM_READ_CHAR
	syscall

	#; Checks player output
	beq $v0, 'N', fin
	beq $v0, 'n', fin
	beq $v0, 'Y', main
	beq $v0, 'y', main

	#; Only here is player entered anything other than 'Y', 'y', 'N', or 'n'.
	li $a0, '\n'
	li $v0, SYSTEM_PRINT_CHAR
	syscall

	la $a0, unknownChar
	li $v0, SYSTEM_PRINT_STRING
	syscall

	jr badInput_
fin:
	#; Ends Program
	li $v0, SYSTEM_EXIT
	syscall
.end main

#; Turns all tiles in the board empty
#; No arguments
.globl initializeGameBoard
.ent initializeGameBoard
initializeGameBoard:
	li $t0, 8
	li $t1, EMPTY
	initialize:
		sb $t1, gameBoardData($t0)
		sub $t0, $t0, 1
	bgez $t0, initialize

	jr $ra
.end initializeGameBoard

#;  Keeps track of the current player and fills the game board appropriately
#;	Argument 1: The first player's Piece
.globl runGame_TicTacToe
.ent runGame_TicTacToe
runGame_TicTacToe:
	#; push
	subu $sp, $sp, 12
	sw $s1, 8($sp)
	sw $s0, 4($sp)
	sw $ra, ($sp)

	#; store piece
	move $s0, $a0

	#; Clears the board completely
	jal initializeGameBoard

	#; Display the board to console
	jal display

	#; Loops plays each turn of the game until the game ends
	gameLoop:
		move $a0, $s0
		jal playerTurn

		move $s1, $v0
		jal display

		beq $s1, EMPTY, gameContinue
			move $v0, $s1

			#; pop
			lw $ra, ($sp)
			lw $s0, 4($sp)
			lw $s1, 8($sp)
			addu $sp, $sp, 12

			jr $ra
		gameContinue:

		#; Swaps the current player X->O or O->X
		add $s0, $s0, 1
		and $s0, $s0, 1
	j gameLoop
.end display

#;  Accepts player input and enacts on the gameboard appropriately
#;	Argument 1: Current player piece (0 -> X or 1 -> O)
.globl playerTurn
.ent playerTurn
playerTurn:
	#; push
	subu $sp, $sp, 4
	sw $ra, ($sp)

	#; Gets the current piece and changes a prompt, to be printed later, appropriately
	move $t2, $a0
	lb $t4, TickTacks($t2)
	la $t3, turn
	sb $t4, 14($t3)

	errorPrompt:
		#; Prints the prompt (displays which turn it currently is)
		la $a0, turn
		li $v0, SYSTEM_PRINT_STRING
		syscall

		#; Requests the player to input X and Y coordinates to place next piece
		la $a0, coords
		li $v0, SYSTEM_PRINT_STRING
		syscall

		li $v0, SYSTEM_READ_INTEGER
		syscall

		#; Checks if given X input is vaild
		blt $v0, 1, badX
		bgt $v0, 3, badX
			move $t0, $v0

			li $v0, SYSTEM_READ_INTEGER
			syscall

			#; Checks if given Y input is vaild
			blt $v0, 1, badY
			bgt $v0, 3, badY

				mul $v0, $v0, BOARDSIZE
				add $t0, $t0, $v0
				sub $t0, $t0, 4

				#; Checks if the position, at the coordinates the player gave, is empty in the board
				lb $t1, gameBoardData($t0)
				bne $t1, EMPTY, nonEmpty

					#; Inserts the player's piece in the board
					sb $t2, gameBoardData($t0)

					#; Checks if the game as ended
					jal checkGameState_full

					#; pop
					lw $ra, ($sp)
					addu $sp, $sp, 4
					jr $ra

		#; Error processing
		badX:
			la $a0, badXCoord
			li $v0, SYSTEM_PRINT_STRING
			syscall
		j errorPrompt

		badY:
			la $a0, badYCoord
			li $v0, SYSTEM_PRINT_STRING
			syscall
		j errorPrompt

		nonEmpty:
			la $a0, nonEmptyCoord
			li $v0, SYSTEM_PRINT_STRING
			syscall
		j errorPrompt
.end playerTurn

#;  Checks if a player board, or if a tie has been reached, on the gameboard
#;  No arguments
#;  Returns current board state
#;		3 -> Tie
#;		2 -> Still playing
#;		1 -> O wins
#;		0 -> X wins
.globl checkGameState_full
.ent checkGameState_full
checkGameState_full:
	li $t0, 0
	la $t1, gameBoardData


	#;	=======================
	#;		Checks all rows
	#;	=======================
	li $t2, 3	#; Loop Index
	rowCheck:
		#; Checks all tiles on the current row
		sub $t2, $t2, 1
		mul $t3, $t2, 3
		add $t3, $t3, $t1

		lb $t4, ($t3)
		lb $t6, 1($t3)
		lb $t7, 2($t3)
		#; Ignore if first is empty
		beq $t4, EMPTY, isEmptyR1
			add $t5, $t4, 1
			and $t5, $t5, 1

			#; Ends the game if all three pieces, in this row, are the same
			bne $t4, $t6, rowFail
			bne $t6, $t7, rowFail
				move $v0, $t4
				jr $ra
			rowFail:

			#; If another piece, on this row, can make a difference in the game, set $t0 to true
			beq $t5, $t6, tieCheckFailR
			beq $t5, $t7, tieCheckFailR
				li $t0, 1
			j tieCheckFailR
		isEmptyR1:

		#; If another piece, on this row, can make a difference in the game, set $t0 to true
		beq $t6, EMPTY, isEmptyR2
			add $t5, $t6, 1
			and $t5, $t5, 1
			beq $t5, $t7, tieCheckFailR
		isEmptyR2:
			li $t0, 1
		tieCheckFailR:
	bgtz $t2, rowCheck
	rowCheckOut:


	#;	==========================
	#;		Checks all columns
	#;	==========================
	li $t2, 3	#; Loop Index
	columnCheck:
		#; Checks all tiles on the current column
		sub $t2, $t2, 1
		add $t3, $t2, $t1

		lb $t4, ($t3)
		lb $t6, 3($t3)
		lb $t7, 6($t3)
		#; Ignore if first is empty
		beq $t4, EMPTY, isEmptyC1
			add $t5, $t4, 1
			and $t5, $t5, 1

			#; Ends the game if all three pieces, in this column, are the same
			bne $t4, $t6, columnFail
			bne $t6, $t7, columnFail
				move $v0, $t4
				jr $ra
			columnFail:

			#; If another piece, on this column, can make a difference in the game, set $t0 to true
			beq $t5, $t6, tieCheckFailC
			beq $t5, $t7, tieCheckFailC
				li $t0, 1
			j tieCheckFailC
		isEmptyC1:

		#; If another piece, on this column, can make a difference in the game, set $t0 to true
		beq $t6, EMPTY, isEmptyC2
			add $t5, $t6, 1
			and $t5, $t5, 1
			beq $t5, $t7, tieCheckFailC
		isEmptyC2:
			li $t0, 1
		tieCheckFailC:
	bgtz $t2, columnCheck
	columnCheckOut:


	#;	=================================
	#;		Checks all first diagonal
	#;	=================================
	lb $t4, ($t1)
	lb $t6, 4($t1)
	lb $t7, 8($t1)
	#; Ignore if first is empty
	beq $t4, EMPTY, isEmptyD1
		add $t5, $t4, 1
		and $t5, $t5, 1

		#; Ends the game if all three pieces, in this diagonal, are the same
		bne $t4, $t6, diagonalFail1
		bne $t6, $t7, diagonalFail1
			move $v0, $t4
			jr $ra
		diagonalFail1:

		#; If another piece, on this diagonal, can make a difference in the game, set $t0 to true
		beq $t5, $t6, tieCheckFailD1
		beq $t5, $t7, tieCheckFailD1
			li $t0, 1
		j tieCheckFailD1
	isEmptyD1:

	#; If another piece, on this diagonal, can make a difference in the game, set $t0 to true
	beq $t6, EMPTY, isEmptyD2
		add $t5, $t6, 1
		and $t5, $t5, 1
		beq $t5, $t7, tieCheckFailD1
	isEmptyD2:
		li $t0, 1
	tieCheckFailD1:


	#;	=================================
	#;		Checks all second diagonal
	#;	=================================
	lb $t4, 2($t1)
	#; lb $t6, 4($t1)
	lb $t7, 6($t1)
	#; Ignore if first is empty
	beq $t4, EMPTY, isEmptyD3
		add $t5, $t4, 1
		and $t5, $t5, 1

		#; Ends the game if all three pieces, in this diagonal, are the same
		bne $t4, $t6, diagonalFail2
		bne $t6, $t7, diagonalFail2
			move $v0, $t4
			jr $ra
		diagonalFail2:

		#; If another piece, on this diagonal, can make a difference in the game, set $t0 to true
		beq $t5, $t6, tieCheckFailD2
		beq $t5, $t7, tieCheckFailD2
			li $t0, 1
		j tieCheckFailD2
	isEmptyD3:

	#; If another piece, on this diagonal, can make a difference in the game, set $t0 to true
	beq $t6, EMPTY, isEmptyD4
		add $t5, $t6, 1
		and $t5, $t5, 1
		beq $t5, $t7, tieCheckFailD2
	isEmptyD4:
		li $t0, 1
	tieCheckFailD2:



	#; If no pieces placed on any diagonal, diagonal, or row can make a different, end game with TIE
	bnez $t0, noTie
		li $v0, TIE
		jr $ra
	noTie:

	#; Continue game
	li $v0, EMPTY
	jr $ra
.end checkGameState_full

#;  Displays the game board
#;  No arguments
.globl display
.ent display
display:
	#; Prints game board
	la $a0, gameBoardSlice
	li $v0, SYSTEM_PRINT_STRING
	syscall

	li $t0, 3 #; first loop index
	la $t1, gameBoardData
	sliceLoop:
		li $t2, 3 #; second loop index
		la $t3, gameBoardVal
		add $t3, $t3, 2

		inValueLoop:
			lb $t4, ($t1)
			lb $t4, TickTacks($t4)
			sb $t4, ($t3)

			add $t1, $t1, 1
			add $t3, $t3, 4
		sub $t2, $t2, 1
		bnez $t2, inValueLoop

		la $a0, gameBoardVal
		li $v0, SYSTEM_PRINT_STRING
		syscall

		la $a0, gameBoardSlice
		li $v0, SYSTEM_PRINT_STRING
		syscall

	sub $t0, $t0, 1
	bnez $t0, sliceLoop

	jr $ra
.end display