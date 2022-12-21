def displayBoard():
	"""
	Uses ASCII text to display the current gameBoard
	"""
	# Checks if there is a board to display
	if len(gameBoard) != 0:
		print(' ===', '====' * (len(gameBoard) - 1), sep='') 		# Top border
		for row in gameBoard:
			print('| ', end='')
			for col in row:
				print(sprites[col], end=' | ')						# Displays the pieces
			print('')
			print(' ===', '====' * (len(gameBoard) - 1), sep='')	# Bottom border of each row
	else:
		print("No Board Created")

def intakeInteger():
	"""
	Only allows the input of integers
	Returns a "None" value otherwise
	"""
	intake = input()
	try:
		return int(intake)											# Tries to change input into an integer
	except ValueError:
		return None													# If failed, return None

def intakeOnly(vaildInputs):
	"""
	Only allows the input of values specified in the formal parameters
	Returns a "None" value otherwise
	"""
	intake = input()
	if intake in vaildInputs:										# Checks if input is in the list of verified inputs
		return intake

def intakecoords():
	"""
	Prompts the user for x and y coords of a position in the gameBoard
	"""
	print('\n')
	print(f'Where to place piece? (1-{len(gameBoard)},1-{len(gameBoard)})')
	while 1:
		print('X: ', end='')
		x = intakeInteger()	#	Gets an integer input
		if x != None and 1 <= x <= len(gameBoard):					#	Loop repeats if the inputted integer is out of the vaild range
			break
		print('Please enter a vaild number.')
	while 1:
		print('Y: ', end='')
		y = intakeInteger()	#	Gets an integer input
		if y != None and 1 <= y <= len(gameBoard):					#	Loop repeats if the inputted integer is out of the vaild range
			break
		print('Please enter a vaild number.')
	return (x,y)													#	Returns the intaken integers

def endCheck():
	"""
	Checks if any of the ending conditions for the game have been met
	"""
	Tie = True
	# Checks all possible horizonal line matches
	for y in range(len(gameBoard)):
		if gameBoard[y][0] != 2 and gameBoard[y].count(gameBoard[y][0]) == len(gameBoard): return gameBoard[y][0]	#	Checks if either X or O won horizonally
		elif Tie:
			for x in range(len(gameBoard)-1):
				if gameBoard[y][x] != 2 and ((gameBoard[y][x] + 1) % 2) in gameBoard[y][(x+1):]: break				#	Checks if either X or O can still win horizonally
			else: Tie = False
	
	# Checks all possible vertical line matches
	for x in range(len(gameBoard)):
		if gameBoard[0][x] != 2 and [row[x] for row in gameBoard].count(gameBoard[0][x]) == len(gameBoard): return gameBoard[0][x]	#	Checks if either X or O won vertically
		elif Tie:
			for y in range(len(gameBoard)-1):
				if gameBoard[y][x] != 2 and ((gameBoard[y][x] + 1) % 2) in [row[x] for row in gameBoard[(y+1):]]: break				#	Checks if either X or O can still win vertically
			else: Tie = False

	# Checks all possible diagonal line matches
	for i in [-1,0]:
		if gameBoard[i][0] != 2 and [gameBoard[z+(i*(2*z+1))][z] for z in range(len(gameBoard))].count(gameBoard[i][0]) == len(gameBoard): return gameBoard[i][0] 			#	Checks if either X or O won diagonally
		elif Tie:
			for z in range(len(gameBoard)-1):
				if gameBoard[z][z+(i*(2*z+1))] != 2 and ((gameBoard[z][z+(i*(2*z+1))] + 1) % 2) in [gameBoard[y][y+(i*(2*y+1))] for y in range(len(gameBoard)-z)]: break	#	Checks if either X or O can still win diagonally
			else: Tie = False

	if Tie:
		return 2	# If a tie occured, return 2

def minimax(coords, isMaximizing, piece, depth = 3, alpha = float('-inf'), beta = float('inf')):
	"""
	Uses the minimax algorithm, with alpha-beta pruning and a Transposition Table, to determine which future Boards are desirable
	"""
	from random import randint

	gameBoard[coords[1]][coords[0]] = (piece+isMaximizing)%2		#	Places a temporary piece on the board to visualize the current board situation
	result = endCheck()
	if result != None:												#	If current board repersents an end, return the board's end value
		gameBoard[coords[1]][coords[0]] = 2							#	Removes temporary piece, on the board, before function exit
		if result == 2: return 0
		elif result == piece: return 1
		elif result == ((piece+1)%2): return -1

	#	Acting player
	if isMaximizing:
		bestScore = -2	#	A placeholder bestScore that is less than the worst case
		for y in range(len(gameBoard)):
			for x in range(len(gameBoard)):
				if gameBoard[y][x] == 2:							#	Checks all positions that are vacant
					#	Finds the best path to an end for the Acting Player at this board position, while using slight randomness
					score = minimax((x,y), False, piece, depth-1, alpha, beta) + (randint(0, 15)/100)
					bestScore = max(score, bestScore)
					# Alpha Beta pruning
					alpha = max(score, alpha)
					if beta <= alpha: break
	#	Enemy Player
	else:
		bestScore = 2	#	A placeholder bestScore that is greater than the worst case
		for y in range(len(gameBoard)):
			for x in range(len(gameBoard)):
				if gameBoard[y][x] == 2:							#	Checks all positions that are vacant
					#	Finds the best path to an end for the Enemy Player at this board position, while using slight randomness
					score = minimax((x,y), True, piece, depth-1, alpha, beta) + (randint(-15, 0)/100)
					bestScore = min(score, bestScore)
					# Alpha Beta pruning
					beta = min(score, beta)
					if beta <= alpha: break

	gameBoard[coords[1]][coords[0]] = 2								#	Removes temporary piece, on the board, before function exit
	return bestScore

def AIBestMove(piece):
	"""
	Finds the best possible move, for the given piece, if it is the piece's current turn
	"""
	bestScore = -2		#	A placeholder bestScore that is less than the worst case
	bestMove = (-1,-1)	#	A placeholder invalid position
	
	for y in range(len(gameBoard)):
		for x in range(len(gameBoard)):
			if gameBoard[y][x] == 2:								#	Checks all positions that are vacant
				score = minimax((x,y), False, piece, 6)				#	Perform the minimax algorithm on these positions
				if bestScore < score:								#	Saves the best calculated moves from each minimax algorithm usage
					bestScore = score
					bestMove = (x+1,y+1)
	return bestMove


def placePiece(coords, piece):
	"""
	Places a single Tic Tac Toe piece on the board, if the given coordinate is available
	Return "True" if successful and "False" otherwise
	"""
	if all([1 <= coords[i] <= len(gameBoard) for i in range(len(coords))]) and gameBoard[coords[1]-1][coords[0]-1] == 2:
		gameBoard[coords[1]-1][coords[0]-1] = piece					#	Places a piece at position if position is on the board and vacant 
		return True
	else: return False												#	Else, return False

if __name__ == "__main__":
	import sys
	if len(sys.argv) == 1:											#	If there are only one given argument, default to a board length of three
		length = 3
	elif len(sys.argv) > 2:											#	If there are too many given arguments, quit out of the program
		print('Too many arguments.\nExpected format: python TicTacToe.py [integer between 3-5]')
		quit()
	else:
		try:
			length = int(sys.argv[1])
			if not (3 <= length <= 4 ):								#	If given argument is an integer outside of the vaild range, default to a board length of three
				print('Argument 1 out of valid range (3-4).\nDefaulting to 3...') 
				length = 3
		except ValueError:											#	If given argument is not an integer, quit out of the program
			print('Invalid argument 1.\nExpected format: python TicTacToe.py [integer between 3-4]') 
			quit()

	#	Loads up needed value references
	sprites = ( 'X', 'O', ' ' )
	endings = ( 'Player X has won.',
				'Player O has won.',
				'A tie has occured.'
			  )
	while 1:
		gameBoard = [[2 for _ in range(length)] for _ in range(length)]		#	Generates an enemy board
		currentTurn = False
		displayBoard()														#	Displays Board in ASCII
		while 1:
			#	Performs the current turn
			if not currentTurn:
				while not placePiece(AIBestMove(0), 0):
					print('\nThat position is unavailable. Try Again.')
			else:
				while not placePiece(AIBestMove(1), 1):
					print('\nThat position is unavailable. Try Again.')
			currentTurn = (not currentTurn)									#	Switches the current player
			displayBoard()													#	Displays Board in ASCII
			endType = endCheck()
			if endType != None:
				print(endings[endType])										#	Ends the game when applicable
				break
		print('Play again? (y/n)')
		intake = intakeOnly(['Y','y','N','n'])
		while intake not in ['Y','y']:										#	Restarts the game if either 'Y' or 'y' are entered
			if intake in ['N','n']: break									#	Exits the game if either 'N' or 'n' are entered
			print('Please enter a vaild input.\nPlay again? (y/n)')			#	Asks for input again if any other string is entered
			intake = intakeOnly(['Y','y','N','n'])
		else: continue
		break