#include <iostream>
#include <stdlib.h>
#include <ctime>

using namespace std;

// Constant Definitions
	// Tiles
#define BLANK ' '
#define TILE '#'

	// Tile States
#define DToD 0
#define LToD 1
#define DToL 2
#define LToL 3

	// Board constraints
#define MIN_BOARDSIZE 3
#define MAX_BOARDSIZE 50

/*
 * class_identifier: The class definition of Conway's game of Life
*/
class CGOF {
	private:
		// Class Variables
		char **board;
		int height = -1, width = -1;
		bool wrapAround = false;

	public:
		/*
		 * function_identifier: Returns the width of the board
		 * parameters: 			N/A
		 * return value:		The width of the board (int) [-1 if no board]
		*/
		int getWidth() {return width;}

		/*
		 * function_identifier: Returns the height of the board
		 * parameters: 			N/A
		 * return value:		The height of the board (int) [-1 if no board]
		*/
		int getHeight() {return height;}

		/*
		 * function_identifier: Returns if the boarders are soild or wrap around
		 * parameters: 			N/A
		 * return value:		The value of variable wrapAround (bool) [False if soild boarders. True if wrap around boarders]
		*/
		bool isWrap() {return wrapAround;}

		/*
		 * function_identifier: Returns the number of alive cells in the current board
		 * parameters: 			N/A
		 * return value:		the number of tiles alive in the current board
		*/
		int aliveNum() {
			int alive = 0;
			for (int r=0; r<height; r++)
				for (int c=0; c<width; c++)
					if (board[r][c] & 1)
						alive++;
			return alive;
		}

	protected:
		/*
		 * function_identifier: Processes user input
		 * parameters: 			The min value for integer input, the max value for integer input, the variable to store integer input in,
		 *						a list of characters to deny, and a list of characters to accept
		 * return value:		The error/return code assocated given given user input and function arguments
		*/
		int getInput(int min, int max, int &num, string searchN = "", string searchY = "") {
			string str;
			cin >> str;
			if (cin.fail())
				return -2; // Critial error
			try {
				num = stoi(str); // Attempts to convert string into integer

				// If successful, check if number is in range
				if (num < min || num > max)
					return -1; // Give error code (out of range) if not
				// Give return code (integer) if is in range
				return 3;
			} catch (...) {
				// Input was not an integer

				// Checks if more than one character was inputted
				if (str.length() == 1) {
					// If so, check if it is in the given deny or accept lists
					for (size_t i = 0; i < searchN.length(); i++)
						if (searchN[i] == str[0])
							return 1; // Give return code (deny)
					for (size_t i = 0; i < searchY.length(); i++)
						if (searchY[i] == str[0])
							return 2; // Give return code (accept)
				}

				// Give error code (unknown string) if cannot parse string any futher
				return 0;
			}
		}

		/*
		 * function_identifier: Creates and populates the board based on user input
		 * parameters: 			N/A
		 * return value:		N/A
		*/
		void setUpBoard() {
			// Starting variables and prompt message
			int num, exitCode;
			cout << "Enter Width and Height of Board:" << endl;

			// Loop for board's dimensions
			while(true) {
				// Attempts to get the board's width from user input
				while(true) {
					cout << "Width (" << MIN_BOARDSIZE << "-" << MAX_BOARDSIZE << "): ";
					if (getInput(MIN_BOARDSIZE, MAX_BOARDSIZE, width) == 3)
						break;
					// Invaild input
					cin.clear();
					cin.ignore((unsigned int)-1, '\n');
					cout << "\t Error - bad width.\n\n";
				}

				// Attempts to get the board's height from user input
				while(true) {
					cout << "Height (" << MIN_BOARDSIZE << "-" << MAX_BOARDSIZE << "): ";
					if (getInput(MIN_BOARDSIZE, MAX_BOARDSIZE, height) == 3)
						break;
					// Invaild input
					cin.clear();
					cin.ignore((unsigned int)-1, '\n');
					cout << "\t Error - bad height.\n\n";
				}

				// Confirms if inputted dimensions are correct
				cout << "\nGiven Dimensions are ( " << width << ", " << height << " )\nConfirm(Y/n): ";
				while(true) {
					exitCode = getInput(-1, -1, num, "Yy", "Nn");
					if (exitCode == 1)
						goto break1; // breaks out of loop if dimensions are correct
					else if (exitCode == 2)
						break; // loops over if inputted dimensions are incorrect
					// Invaild input
					cin.clear();
					cin.ignore((unsigned int)-1, '\n');
					cout << "\t Error - unrecognized character.\n\n";
				}
			};
		break1:

			// Allocates memory to the board
			board = new char*[height];
			for (int i=0; i<height; i++)
				board[i] = new char[width];

			// Asks what type of boarders the board should have 
			while(true) {
				cout << "\nThe board's Boarders should:\n";	
				cout << "0.) Be soild.\n1.) Wrap around\nSelect an option (0-1): ";
				if (getInput(0, 1, num) == 3)
					break; // breaks out of loop if vaild input
				// Invaild input
				cin.clear();
				cin.ignore((unsigned int)-1, '\n');
				cout << "\t Error - bad input.\n\n";
			}

			// Sets the boarder type to what was inputted
			wrapAround = (bool)num;

			// Populates the board
			populate();
		}

		/*
		 * function_identifier: Toggles random tiles
		 * parameters: 			Number of tiles to toggle
		 * return value:		N/A
		*/
		void setRandom(int n) {
			// ensures no infinite or overly long loop ocurrs
			int temp = width*height/1.1 - aliveNum();
			if (temp < 0)
				return;
			else if (n > temp)
				n = temp;

			// Randomly places creatures in the board
			for (int i=0; i<n;) {
				temp = rand() % (width*height);
				if (!(board[temp/width][temp % width] & 1)) {
					board[temp/width][temp % width] = LToD;
					i++;
				}
			}
			
		}
	public:
		/*
		 * function_identifier: Constructor of the class
		 * parameters: 			N/A
		 * return value:		N/A
		*/
		CGOF() {
			// Starting values
			srand((unsigned)time(NULL));
			int num, exitCode;

			// Creates and populates the board
			setUpBoard();

			// Prints the board for user
			print();
			while(true) {
				while(true) {
					cout << "\nPress (Q/q) to quit, or the number of steps to move forward (1-100):\n";
					// Gets and checks user input
					exitCode = getInput(1, 100, num, "", "Qq");
					if (exitCode <= 1) {
						cin.clear();
						cin.ignore((unsigned int)-1, '\n');
						switch (exitCode) {
							case -2:
								cout << "\t Error - bad input.\n\n";
							break;
							case -1:
								cout << "\t Error - invaild number.\n\n";
							break;
							default:
							;
						}
					} else
						break;
				}
				if (exitCode == 2)
					break; // Exit out of loop

				// Continues for some number of steps
				for(int i=0; i<num; i++) {
					step();
					print();
				}
			}
		}

		/*
		 * function_identifier: Deconstructor of the class
		 * parameters: 			N/A
		 * return value:		N/A
		*/
		~CGOF() {
			// deletes allocated memory
			for (int i=0; i<width; i++)
				delete [] board[i];
			delete [] board;
		}

		/*
		 * function_identifier: Kills all tiles on the board
		 * parameters: 			N/A
		 * return value:		N/A
		*/
		void reset() {
			for (int r=0; r<height; r++)
				for (int c=0; c<width; c++)
					board[r][c] = DToD;
		}

		/*
		 * function_identifier: Toggles one indicated tile on the board
		 * parameters: 			X and Y integer coordinates in the board (index starts at 1)
		 * return value:		N/A
		*/
		void toggle(int X, int Y) {
			if (0 < X && X <= width && 0 < Y && Y <= height)
				board[Y-1][X-1] ^= 1;
		}

		/*
		 * function_identifier: Populates the board based on user input
		 * parameters: 			N/A
		 * return value:		N/A
		*/
		void populate() {
			// Starting variables
			int num, exitCode;
			reset();
back1:
			// Prints the board
			print();

			cout << "\nSet up Board:\n";
			// Asks for a perfered method of setting up the board
			while(true) {
				cout << "\n0.) Manually\n1.) Randomly\n2.) Clear\nSelect an option (0-2): ";
				if (getInput(0, 2, num) == 3)
					break;
				// Invaild input
				cin.clear();
				cin.ignore((unsigned int)-1, '\n');
				cout << "\t Error - bad input.\n\n";
			}

			// Randomly
			switch(num) {
				// Clear
				case 2:
					reset();
					goto back1;
				// Randomly
				case 1:
					cout << "\n([B/b] to back)\nNumber of random tiles that should be placed:\n";

					// Gets user number of tiles that are to be placed randomly
					while(true) {
						cout << "Count (1-" << (int)(width*height/1.1) << "): ";
						exitCode = getInput(0, (int)(width*height/1.1), num, "", "Bb");
						if (2 <= exitCode && exitCode <= 3)
							break;
						// Invaild input
						cin.clear();
						cin.ignore((unsigned int)-1, '\n');
						cout << "\t Error - bad count.\n\n";
					}

					// Randomly populate the title
					if (exitCode == 3) {
						setRandom(num);
						print();
						cout << "\nStay with this? (Y/n): ";

						// Gets user confirmation
						while(true) {
							exitCode = getInput(-1, -1, num, "Yy", "Nn");
							if (exitCode == 1)
								break; // breaks out of loop if accepted
							else if (exitCode == 2)
								goto back1; // loops over if not accepted
							// Invaild input
							cin.clear();
							cin.ignore((unsigned int)-1, '\n');
							cout << "\t Error - unrecognized character.\n\n";
						}
					}
				break;

				// Manually
				case 0:
					// Starting variables
					int X, Y;

					// Gets a set of coordinates from user input
					while(true) {
						print();
						cout << "\n([Q/q] to quit).\n([B/b] to back)\nGive Coordinates to where to toggle:\n";
						// Gets X coordinate from user input
						while(true) {
							cout << "X (1-" << width << "): ";
							exitCode = getInput(0, width, X, "Qq", "Bb");
							if (1 <= exitCode && exitCode <= 3)
								break;
							// Invaild input
							cin.clear();
							cin.ignore((unsigned int)-1, '\n');
							cout << "\t Error - bad X coordinates.\n\n";
						}
						// Exit
						if (exitCode <= 2) break;

						// Gets Y coordinate from user input
						while(true) {
							cout << "Y (1-" << height << "): ";
							exitCode = getInput(0, height, Y, "Qq", "Bb");
							if (1 <= exitCode && exitCode <= 3)
								break;
							// Invaild input
							cin.clear();
							cin.ignore((unsigned int)-1, '\n');
							cout << "\t Error - bad Y coordinates.\n\n";
						}
						// Exit
						if (exitCode <= 2) break;

						// Toggles a board position and prints the board
						board[Y-1][X-1] ^= 1;
					}
				break;
			}
			if (exitCode == 2)
				// Backsteps to options again
				goto back1;
		}

		/*
		 * function_identifier: Prints the board to console
		 * parameters: 			N/A
		 * return value:		N/A
		*/
		void print() {
			cout << string(width*4 + 1, '-') << endl;
			for (int r=0; r<height; r++) {
				cout << "| ";
				for (int c=0; c<width; c++)
					// Checks if the current tile is alive
					if (board[r][c] & 1)
						cout << TILE << " | ";
					else
						cout << BLANK << " | ";
				cout << endl << string(width*4 + 1, '-') << endl;
			}
		}

		/*
		 * function_identifier: Updates the state of the board
		 *		For each Cell:
		 *		Living: 2-3 Living Neighbors -> Stay Alive, otherwise Change to Dead
		 *		Dead: Exactly 3 Living Neighbors -> Change to Alive 
		 *		Cell States:
		 *			0: Currently Dead, Stay Dead		(00b - 0)
		 *			1: Currently Living, Change to Dead	(01b - 1)
		 *			2: Currently Dead, Change to Living	(10b - 2)
		 *			3: Currently Living, Stay Living	(11b - 3)
		 * parameters: 			N/A
		 * return value:		N/A
		*/
		void step() {
			// Starting variables
			int neighbors;

			// Cycles though the whole board once
			for (int r=0; r<height; r++) {
				for (int c=0; c<width; c++) {
					// Resets the count
					neighbors = 0;

					// Counts all neighbors differently depending on wrapAround boolean variable
					if (wrapAround)
						for (int r2=height+r-1; r2<=height+r+1; r2++) {
							for (int c2=width+c-1; c2<=width+c+1; c2++) {
								if (board[r2 % height][c2 % width] & 1)
									neighbors++;
							}
						}
					else {
						for (int r2 = ((r-1 < 0) ? 0 : r-1); r2 < ((r+2 < height) ? r+2 : height); r2++) {
							for (int c2 = ((c-1 < 0) ? 0 : c-1); c2 < ((c+2 < width) ? c+2 : width); c2++) {
								if (board[r2][c2] & 1)
									neighbors++;
							}
						}
					}

					// Changes the current tile's status
					if (board[r][c] & 1) {
						// It redudantly counted itself, so we check for a number between 3-4 instead of 2-3
						if (3 <= neighbors && neighbors <= 4)
							board[r][c] = LToL;
					} else {
						if (neighbors == 3)
							board[r][c] = DToL;
					}
				}
			}
			// Cycles though the whole board once more and updates each title
			for (unsigned int r=0; r<height; r++)
				for (unsigned int c=0; c<width; c++)
					board[r][c] >>= 1;
		}
};


/*
 * function_identifier: The main function of the program
 * parameters: 			N/A
 * return value:		Exit code
*/
int main() {
	CGOF();
}