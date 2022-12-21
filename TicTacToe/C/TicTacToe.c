#include <stdio.h>		/*	printf			*/
#include <stdlib.h>		/*	rand, malloc	*/
#include <time.h>		/*	time			*/

// use: gcc -Wall -pedantic -o TicTac TicTacToe.c

/* Type Definitions */
typedef enum {false, true} bool;
typedef enum {X_ = -1, empty = 0, O_ = 1} TickTack;
typedef struct {
	int horizontal;
	int vertical;
	int diagonal;
	int antidiag;
} checkCount;

/* Constant global variables	*/
	/* TickTacks */
	char TickTacks[3] = {'x', ' ', 'o'};
	TickTack *gameBoard;

// Function proptypes declarations
void menu(TickTack *piece, bool *AI, bool *rand);
void initializeGameBoard();
int runGame_TicTacToe(TickTack piece, bool AI, bool rand);
int playerTurn(TickTack piece);
int AITurn(TickTack piece, bool rand);
double minimax(int startCoords, TickTack piece, bool isMaximizing, bool rand, double alpha, double beta);
int checkGameState_full();
void displayBoard();

// Standard Function proptypes declarations
void toLower(char* str);
int strcmp(const char* str1, const char* str2);
int strti(const char* str, int *error);
void flush();
double doubleRand(double min, double max);

/* Macros */
#define TIE 3
#define POS_INF 1.0 / 0.0
#define NEG_INF -1.0 / 0.0
#define swap(x, y) do {typeof(x) temp = x; x = y; y = temp;} while(0);

/*
 * function_identifier: The program's main function.
 * parameters: 			N/A
 * return value:		The exit status of the program.
*/
int main(int argc, char *argv[]) {
	// Initializes variables
	int piece;
	bool AI, rand;
	srand(time(NULL));
	// Call the starting user GUI
	menu(&piece, &AI, &rand);

	// Plays the game and displays message appropriate to the game's ending status
	switch (runGame_TicTacToe(piece, AI, rand)) {
		case X_:	printf("X wins!\n"); return 0;
		case O_:	printf("O wins!\n"); return 0;
		case TIE:	printf("It's a Tie!\n"); return 0;
		default:	printf("Some unexpected error occured.\n"); return 1;
	}
}

/*
 * function_identifier: The starting GUI for the client
 * parameters: 			The address of a TickTack enum, and the address of an integer
 * return value:		N/A
*//*
 * function_identifier: The starting GUI for the client
 * parameters: 			The address of a TickTack enum, and the address of an integer
 * return value:		N/A
*/
void menu(TickTack *piece, bool *AI, bool *rand) {
	// Initializes variable
	char str[10];

	// Displays user prompt
	printf("Do you want to play with Ai or another user?\n1.) AI\n2.) User\n");

	// Second user prompt loop
	promptJump1:
		printf("Please enter your selection.\n");

		scanf("%s", str); 	// Gets a single character from the I/O buffer. Ignores newlines
		toLower(str);	// Converts string to lowercase

		// If the user did give a vaild character, set the piece to the appropriate value
		if (!(strcmp(str, "1") && strcmp(str, "ai"))) *AI = true;
		else if (!(strcmp(str, "2") && strcmp(str, "u") && strcmp(str, "user"))) *AI = false;
		else {
			// If the user did not give a vaild character, display error message and jump back
			printf("Invaild selection. Please try again.\n"); // 
			flush(); // Clears the I/O buffer
			goto promptJump1;
		}

	// Returns if no AI
	if (!*AI) {*piece = X_; *rand = false; return;}

	// Displays user prompt
	printf("Which piece will you play?\n1.) X\n2.) O\n");

	// First user prompt loop
	promptJump2:
		printf("Please enter your selection.\n");

		scanf("%s", str);	// Gets a single character from the I/O buffer. Ignores newlines
		toLower(str);	// Converts string to lowercase

		// If the user did give a vaild character, set the piece to the appropriate value
		if (!(strcmp(str, "1") && strcmp(str, "x"))) *piece = X_;
		else if (!(strcmp(str, "2") && strcmp(str, "o"))) *piece = O_;
		else {
			// If the user did not give a vaild character, display error message and jump back
			printf("Invaild selection. Please try again.\n"); // 
			flush(); // Clears the I/O buffer
			goto promptJump2;
		}

	// Displays user prompt
	printf("What difficulty do you wish to play at?\n1.) Easy\n2.) Hard\n");

	// Third user prompt loop
	promptJump3:
		printf("Please enter your selection.\n");

		scanf("%s", str);	// Gets a single character from the I/O buffer. Ignores newlines
		toLower(str);	// Converts string to lowercase

		// If the user did give a vaild character, set the piece to the appropriate value
		if (!(strcmp(str, "1") && strcmp(str, "e") && strcmp(str, "easy"))) *rand = true;
		else if (!(strcmp(str, "2") && strcmp(str, "h") && strcmp(str, "hard"))) *rand = false;
		else {
			// If the user did not give a vaild character, display error message and jump back
			printf("Invaild selection. Please try again.\n"); // 
			flush(); // Clears the I/O buffer
			goto promptJump3;
		}
}

/*
 * function_identifier: The starting GUI for the client
 * parameters: 			The address of a TickTack enum, and the address of an integer
 * return value:		The ending status of the game
*/
int runGame_TicTacToe(TickTack ticktack, bool AI, bool rand) {
	// Initializes variables and game board
	int result = empty, piece = X_;
	initializeGameBoard();

	// Displays the empty game board
	displayBoard();
	while(1) {
		// Plays each turn
			// A user turn
		if (piece == ticktack || !AI) {printf("\nPlayer %d's turn!\n", ((piece != ticktack)+1)); result = playerTurn((TickTack)piece);}
			// An AI turn
		else {printf("\nComputer's turn!\n"); result = AITurn((TickTack)piece, rand);}
		// Displays the game board after a turn
		displayBoard();
		// Ends the game when a conclution has occured
		switch(result) {
			case empty: break;
			case X_:
			case O_:
			case TIE:
				free(gameBoard);
				return result;
		}
		// Inverts the piece
		piece = -piece;
	}
	// If this instruction ever executes, an error occured
	return -2;
}

/*
 * function_identifier: Initializes the game board's values.
 * parameters: 			N/A
 * return value:		N/A
*/
void initializeGameBoard() {
	// Allocates memory for the game board
	gameBoard = (TickTack*)malloc(9*sizeof(TickTack));
	// Initializes allocated memory
	for (int i=0; i<9; i++) {gameBoard[i]=empty;}
}

/*
 * function_identifier: Plays a turn according to user input
 * parameters: 			The player's current piece
 * return value:		The status of the board after the player finished their turn
*/
int playerTurn(TickTack piece) {
	// Initializes variables and game board
	int X, Y, coords;

	// The prompt loop
	promptJump4:
		printf("Enter X and Y coords: ");
		scanf("%d%d", &X, &Y); // Scans the I/O buffer for integers
		if (X > 3 || X < 1) {
			// If an invalid X coordinate is given, display error and jump back to loop
			printf("Error - X coordinate must between 1-3.\n\n");
			flush(); // Clears the I/O buffer
			goto promptJump4;
		}
		if (Y > 3 || Y < 1) {
			// If an invalid Y coordinate is given, display error and jump back to loop
			printf("Error - Y coordinate must between 1-3.\n\n");
			flush(); // Clears the I/O buffer
			goto promptJump4;
		}
		if (gameBoard[(coords = X + Y*3 - 4 )] != empty) {
			// If an invalid coordinate is given, display error and jump back to loop
			printf("Error - coordinate position (%d, %d) is nonempty.\n\n", X, Y);
			flush(); // Clears the I/O buffer
			goto promptJump4;
		}

	gameBoard[coords] = piece; // Sets the coordinate
	return checkGameState_full(); // Return the gameBoard's status
}

/*
 * function_identifier: Plays a turn according to minMax algorithm
 * parameters: 			The AI's current piece
 * return value:		The status of the board after AI finished their turn
*/
int AITurn(TickTack piece, bool rand) {
	// Initializes variables and game board
	int bestMove = -2;
	double bestScore = -2, score;
	// If the given piece is empty, then just return TIE result
	if (piece == empty) {return TIE;}

	// Simulates placing a piece on all given board positions
	for (int coords=8; coords>=0; --coords) {
		if (gameBoard[coords] == empty) {
			score = minimax(coords, piece, false, rand, NEG_INF, POS_INF);
			if (score > bestScore) {
				bestScore = score;
				bestMove = coords;
			}
		}
	}

	// If no bestMove was found possible, error
	if (bestMove == -2) printf("ERROR\n");
	// Plays out the given best move
	gameBoard[bestMove] = piece;
	// Returns the current status of the game board
	return checkGameState_full();
}

/*
 * function_identifier: The minMax algorithm of a TickTacToe board
 * parameters: 			The AI's current piece
 * return value:		The vaild of the current board the algorithm is examining
*/
double minimax(int startCoords, TickTack piece, bool isMaximizing, bool rand, double alpha, double beta) {
	// Plays the given piece
	gameBoard[startCoords] = (isMaximizing) ? (TickTack)(-piece) : piece;
	// Checks the board's current state
	int result = checkGameState_full();
	// Ends this simulation branch if an end result is found
	if (result != empty) {
		gameBoard[startCoords] = empty;
		if (result==TIE) {return 0;}
		if (result==piece) {return 1;}
		return -1;
	}

	// Gets randomised value for lower difficulty
	double random = rand ? doubleRand(0, 0.99) : 0;

	// Initializes variables
	int coords;
	double bestScore, score;
	//	Acting player (max)
	if (isMaximizing) {
		bestScore = -2;
		// Simulates placing a piece on all given board positions for the acting player
		for (coords=9; coords>=0; --coords) {
			if (gameBoard[coords] == empty) {
				score = minimax(coords, piece, false, rand, alpha, beta) + random;
				// Alpha-Beta Pruning
				if (score > bestScore) {bestScore = score;}
				if (score > alpha) {alpha = score;}
				if (beta <= alpha) {break;}
			}
		}
	}
	//	Enemy Player (min)
	else {
		bestScore = 2;
		// Simulates placing a piece on all given board positions for the enemy player
		for (coords=9; coords>=0; --coords) {
			if (gameBoard[coords] == empty) {
				score = minimax(coords, piece, true, rand, alpha, beta) - random;
				// Alpha-Beta Pruning
				if (score < bestScore) {bestScore = score;}
				if (score < beta) {beta = score;}
				if (beta <= alpha) {break;}
			}
		}
	}

	// Ends this branch's simulation
	gameBoard[startCoords] = empty;
	return bestScore;
}

/*
 * function_identifier: Checks the state of the current game board
 * parameters: 			N/A
 * return value:		The current game board's state
*/
int checkGameState_full() {
	// Initializes variables
	int X, Y, temp, check, isTie = 1;

	// Row Check
	for (Y = 0; Y< 3; Y++) {
		temp = Y*3;
		// Checks all rows for three pieces of the same
		if (gameBoard[temp] != empty) {
			check = -gameBoard[temp];
			// If three pieces of the same
				// If so, return that end result
			if (gameBoard[temp] == gameBoard[temp + 1] && gameBoard[temp + 1] == gameBoard[temp + 2]) {return gameBoard[temp];}
			// Checks if there is a row where a piece could still win in
			if (gameBoard[temp + 1] != check && gameBoard[temp + 2] != check) {isTie = 0;}
			continue;
		}
		// Checks if there is a row where a piece could still win in
		if (gameBoard[temp + 1] == gameBoard[temp + 2]) {isTie = 0;}
	}
	// Column Check
	for (X = 0; X < 3; X++) {
		temp = X;
		// Checks all columns for three pieces of the same
		if (gameBoard[temp] != empty) {
			check = -gameBoard[temp];
			// If three pieces of the same
				// If so, return that end result
			if (gameBoard[temp] == gameBoard[temp + 3] && gameBoard[temp + 3] == gameBoard[temp + 6]) {return gameBoard[temp];}
			// Checks if there is a column where a piece could still win in
			if (gameBoard[temp + 3] != check && gameBoard[temp + 6] != check) {isTie = 0;}
			continue;
		}
		// Checks if there is a column where a piece could still win in
		if (gameBoard[temp + 3] == gameBoard[temp + 6]) {isTie = 0;}
	}
	// Diagonal Checks
	for (X = -1; X < 2; X += 2) {
		// Checks all diagonals for three pieces of the same
		if (gameBoard[4] != empty) {
			check = -gameBoard[4];
			// If three pieces of the same
				// If so, return that end result
			if (gameBoard[4] == gameBoard[1 + X] && gameBoard[4] == gameBoard[7 - X]) {return gameBoard[4];}
			// Checks if there is a diagonal where a piece could still win in
			if (gameBoard[1 + X] != check && gameBoard[7 - X] != check) {isTie = 0;}
			continue;
		}
		// Checks if there is a diagonal where a piece could still win in
		if (gameBoard[1 + X] == gameBoard[7 - X]) {isTie = 0;}
	}

	// If there are no rows, columns, or diagonal where a piece could win, return TIE result
	if (isTie) return TIE;
	// Else, return empty result
	return empty;
}

/*
 * function_identifier: Displays the current gameBoard
 * parameters: 			N/A
 * return value:		N/A
*/
void displayBoard() {
	// Prints game board
	printf(" ===========\n");

	for (int Y = 0; Y < 3; Y++) {
		printf("| ");
		for (int X = 0; X < 3; X++) {
			printf("%c | ", TickTacks[1 + gameBoard[ X + Y * 3 ]]);
		}
		printf("\n ===========\n");
	}
}

/*
 * function_identifier: Converts a string into lowercase
 * parameters: 			A string (or a const char array)
 * return value:		N/A
*/
void toLower(char* str) {
	// Initializes variables
	int length = sizeof(str);
	// Cycles through the given strings and converts all applicable characters to lower
	for (int i = 0; i < length; i++) {
		if (90 >= str[i] && str[i] >= 65) str[i] = str[i] + 32;
	}
}

/*
 * function_identifier: Compairs two strings
 * parameters: 			Two strings (or two const char arrays)
 * return value:		returns 0 if (str1 == str2), 1 if (str1 > str2), or -1 if (str1 < str2)
*/
int strcmp(const char* str1, const char* str2) {
	// Initializes variables
	int val=0, i=-1;
	// Cycles through the given strings
	do {
		i++;
		// Compairs character number i in both strings
		val = (str1[i] || str2[i]) * (((str1[i] > str2[i]) || !str2[i]) - ((str2[i] > str1[i]) || !str1[i]));
	} while (!val && str1[i] && str2[i]);	// Stops cycling if one of the strings end or an inequality was detected
	return val;		// Return comparison results
}

/*
 * function_identifier: Converts a string into a signed or unisigned integer
 * parameters: 			Two strings (or two const char arrays)
 * return value:		returns 0 if (str1 == str2), 1 if (str1 > str2), or -1 if (str1 < str2)
*/
int strti(const char* str, int *error) {
	// Initializes variables
	int i, sign=1, signedMax = 0x7FFFFFFF;
	unsigned int val=0, unsignedMax = 0xFFFFFFFF;

	for (i=0; (str[i] == ' '); i++ );		// Ignores all leading spaces
	if (!str[i]) {*error = 2; return 0;}	// Returns with error if string has no non-space chars
	if (str[i] == '-') {sign=-1; i++;}		// Sets "sign" to negative if '-' character is encountered before any numerical chars

	for (;str[i];i++) { // Ends if encounters a NULL character
		if (((str[i] > '9') || (str[i] < '0'))) {*error = i; return val;}		// returns calculated result and error if non-numerical char encountered
		// returns signedMax and error if integer overflow will occur
		if (sign < 0) {if ( val > ((signedMax-(str[i]-'0'))/10)) {*error = 3; return signedMax;}}
		// returns unsignedMax and error if unsigned integer overflow will occur
		else if (val > ((unsignedMax-(str[i]-'0'))/10)) {*error = 3; return unsignedMax;}
		
		// value = (value*10) + next_number_in_string_sequence
		val *= 10;
		val += (str[i]-'0');
	}

	// Returns with no error and converted value
	*error = 0;
	return val*sign;
}

/*
 * function_identifier: Clears the I/O terminal buffer
 * parameters: 			N/A
 * return value:		N/A
*/
void flush (){
	int c;
	// Takes all characters out of I/O buffer until end of line found
	while ((c = getchar()) != '\n' && c != EOF);
}

/*
 * function_identifier: Clears the I/O terminal buffer
 * parameters: 			N/A
 * return value:		N/A
*/
double doubleRand(double min, double max) {
	// Swaps parameters if min is greater than max
	if (min > max) swap(min, max);
	// Return random double between range
    return (min + (rand() / (RAND_MAX / (max - min))));
}