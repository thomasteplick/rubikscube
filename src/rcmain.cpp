//============================================================================
// Name        : rcmain.cpp
// Author      : Tom Teplick
// Version     :
// Copyright   : 
// Description : Solve 3x3 Rubik's Cube using IDA and IDA* Richard Korf algorithm, display cube
//============================================================================

/*
 * Use cases:
 * Create pattern databases for  1@8 corners and 2@6 edges
 * Scramble cube starting position and display 6 faces for each twist
 * Perform IDA, iterative deepening of DFS
 * Perform IDA* using pattern databases
 * Perform K trials for 1 to N twists, save solution times and number of twists in table
 * Map Cube state to faces (facelet colors) and display unfolded cube (the faces)
 * Map Cube state via faces to 3D scatterplot in matplotlib
 * Find the face colors for the cube in any position and display the faces
 * Functions for the 18 Rubik's cube rotations:  CW90, CCW90, CW90CW90 degrees
 * Map cube number to position and colors; ie., this cubie has these colors and is an edge or corner
 */
#include <iostream>
#include <sstream>
#include <fstream>
#include "rcmain.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <Windows.h>
#include <wincon.h>

// constructor
Cube::Cube(int nt, int mt)
{
	ntrials = nt;
	maxTwists = mt;
}

// Up face rotate CW 90 deg, z axis, plane 2
void Cube::twistU()
{
	uint8_t buf[DIM][DIM];
	int plane = 2;

	// swap rows and columns, reverse rows
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			buf[i][DIM-1-j] = cube[plane][j][i];
		}
	}
	// copy back to cube from buf
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			cube[plane][j][i] = buf[j][i];
		}
	}
}

// Up face rotate CCW 90 deg, z axis, plane 2
void Cube::twistU_()
{
	uint8_t buf[DIM][DIM];
	int plane = 2;

	// swap rows and columns, reverse columns
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			buf[DIM-1-i][j] = cube[plane][j][i];
		}
	}
	// copy back to cube from buf
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			cube[plane][j][i] = buf[j][i];
		}
	}
}

// Up face rotate 180 deg, z axis, plane 2
void Cube::twistU2()
{
	uint8_t buf[DIM][DIM];
	int plane = 2;

	// reverse rows and columns
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			buf[DIM-1-j][DIM-1-i] = cube[plane][j][i];
		}
	}
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			cube[plane][j][i] = buf[j][i];
		}
	}
}

// Down face rotate CW 90 deg, -z axis, plane 0
void Cube::twistD()
{
	uint8_t buf[DIM][DIM];
	int plane = 0;

	// swap rows and columns, reverse rows
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			buf[i][DIM-1-j] = cube[plane][j][i];
		}
	}
	// copy back to cube from buf
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			cube[plane][j][i] = buf[j][i];
		}
	}
}

// Down face rotate CCW 90 deg, -z axis, plane 0
void Cube::twistD_()
{
	uint8_t buf[DIM][DIM];
	int plane = 0;

	// swap rows and columns, reverse columns
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			buf[DIM-1-i][j] = cube[plane][j][i];
		}
	}
	// copy back to cube from buf
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			cube[plane][j][i] = buf[j][i];
		}
	}
}

// Down face rotate 180 deg, -z axis, plane 0
void Cube::twistD2()
{
	uint8_t buf[DIM][DIM];
	int plane = 0;

	// reverse rows and columns
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			buf[DIM-1-j][DIM-1-i] = cube[plane][j][i];
		}
	}
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			cube[plane][j][i] = buf[j][i];
		}
	}
}

// Left face rotate CW 90 deg, x axis, plane 0
void Cube::twistL()
{
	uint8_t buf[DIM][DIM];
	int plane = 0;

	// swap rows and columns, reverse rows
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			buf[i][DIM-1-j] = cube[j][i][plane];
		}
	}
	// copy back to cube from buf
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			cube[j][i][plane] = buf[j][i];
		}
	}
}

// Left face rotate CCW 90 deg, x axis, plane 0
void Cube::twistL_()
{
	uint8_t buf[DIM][DIM];
	int plane = 0;

	// swap rows and columns, reverse columns
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			buf[DIM-1-i][j] = cube[j][i][plane];
		}
	}
	// copy back to cube from buf
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			cube[j][i][plane] = buf[j][i];
		}
	}
}

// Left face rotate 180 deg, x axis plane 0
void Cube::twistL2()
{
	uint8_t buf[DIM][DIM];
	int plane = 0;

	// reverse rows and columns
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			buf[DIM-1-j][DIM-1-i] = cube[j][i][plane];
		}
	}
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			cube[j][i][plane] = buf[j][i];
		}
	}
}

// Right face rotate CW 90 deg, x axis, plane 2
void Cube::twistR()
{
	uint8_t buf[DIM][DIM];
	int plane = 2;

	// swap rows and columns, reverse rows
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			buf[i][DIM-1-j] = cube[j][i][plane];
		}
	}
	// copy back to cube from buf
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			cube[j][i][plane] = buf[j][i];
		}
	}

}

// Right face rotate CCW 90 deg, x axis plane 2
void Cube::twistR_()
{
	uint8_t buf[DIM][DIM];
	int plane = 2;

	// swap rows and columns, reverse columns
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			buf[DIM-1-i][j] = cube[j][i][plane];
		}
	}
	// copy back to cube from buf
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			cube[j][i][plane] = buf[j][i];
		}
	}
}

// Right face rotate 180 deg, x axis plane 2
void Cube::twistR2()
{
	uint8_t buf[DIM][DIM];
	int plane = 2;

	// reverse rows and columns
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			buf[DIM-1-j][DIM-1-i] = cube[j][i][plane];
		}
	}
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			cube[j][i][plane] = buf[j][i];
		}
	}
}

// Front face rotate CW 90 deg, y axis, plane 2
void Cube::twistF()
{
	uint8_t buf[DIM][DIM];
	int plane = 2;

	// swap rows and columns, reverse rows
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			buf[i][DIM-1-j] = cube[j][plane][i];
		}
	}
	// copy back to cube from buf
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			cube[j][plane][i] = buf[j][i];
		}
	}
}

// Front face rotate CCW 90 deg, y axis, plane 2
void Cube::twistF_()
{
	uint8_t buf[DIM][DIM];
	int plane = 2;

	// swap rows and columns, reverse columns
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			buf[DIM-1-i][j] = cube[j][plane][i];
		}
	}
	// copy back to cube from buf
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			cube[j][plane][i] = buf[j][i];
		}
	}
}

// Front face rotate 180 deg, y axis, plane 2
void Cube::twistF2()
{
	uint8_t buf[DIM][DIM];
	int plane = 2;

	// reverse rows and columns
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			buf[DIM-1-j][DIM-1-i] = cube[j][plane][i];
		}
	}
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			cube[j][plane][i] = buf[j][i];
		}
	}
}

// Back face rotate CW 90 deg, y axis, plane 0
void Cube::twistB()
{
	uint8_t buf[DIM][DIM];
	int plane = 0;

	// swap rows and columns, reverse rows
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			buf[i][DIM-1-j] = cube[j][plane][i];
		}
	}
	// copy back to cube from buf
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			cube[j][plane][i] = buf[j][i];
		}
	}
}

// Back face rotate CCW 90 deg, y axis, plane 0
void Cube::twistB_()
{
	uint8_t buf[DIM][DIM];
	int plane = 0;

	// swap rows and columns, reverse columns
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			buf[DIM-1-i][j] = cube[j][plane][i];
		}
	}
	// copy back to cube from buf
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			cube[j][plane][i] = buf[j][i];
		}
	}
}

// Back face rotate 180 deg, y axis, plane 0
void Cube::twistB2()
{
	uint8_t buf[DIM][DIM];
	int plane = 0;

	// reverse rows and columns
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			buf[DIM-1-j][DIM-1-i] = cube[j][plane][i];
		}
	}
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			cube[j][plane][i] = buf[j][i];
		}
	}
}

bool Cube::isSolved()
{
	// are the cubies in the initialized order state 0-26
	uint8_t n = 0;
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			for (int k = 0; k < DIM; ++k) {
				if (cube[i][j][k] != n) {
					return false;
				}
				++n;
			}
		}
	}
	return true;
}

// scramble solved cube with nmoves
void Cube::scrambleCube(int nmoves)
{
	uint8_t n = 0;
	// initialize the cube order for the solved cube
	for (int i = 0; i < DIM; ++i) {
		for (int j = 0; j < DIM; ++j) {
			for (int k = 0; k < DIM; ++k) {
				cube[i][j][k] = uint8_t(n);
				++n;
			}
		}
	}

	// clear the move queue
	while (!mvQueue.empty()) {
		mvQueue.pop();
	}
	// loop over the number of requested twists
	for (int i = 0; i < nmoves; ++i) {
		// random selection of move
		rotate mv = revRotate[std::rand() % MOVES];
		//rotate mv = rotate::B2;
		doMove(mv);
		// push to move queue for later display
		mvQueue.push(mv);
	}
}

// Create pattern databases, 1@8corners, 2@6edges
void Cube::createPatternDB()
{
/*
	Algorithm 2 Generate Lookup Table
	foundStates <- 0
	lookupTable <- array[numStates]{−1, −1, . . . , −1}
	function GenerateLookupTable
		bound <- 0
		while not foundStates == numStates do
			boundedDFS (solvedCubeState, 0, bound)
			bound <- bound + 1
		end while
		return moveStack
	end function

	function boundedDFS(cube, depth, bound)
		if depth > bound then
		return
		end if
		index <- calculateIndex (cube)
		if lookupTable[index] == -1 then ⊲ Set the entry when not set yet
			lookupTable [index] <- depth
		end if
		for move={U , U’, U2, ..., D, D’, D2} do ⊲ Loop trough all possible moves
			ApplyMove (cube,move)
			BoundedDFS(cube, bound, depth + 1)
			ApplyMove (cube, Inverse (move))
		end for
	end function

 */
}

// Display the cube faces using Windows color attributes
void Cube::displayCubeFaces()
{

	//COLORS LIST foreground 1-15
	//0: Black
	//1: Blue
	//2: Green
	//3: Cyan
	//4: Red
	//5: Purple
	//6: Brown
	//7: Default white
	//8: Gray/Grey
	//9: Bright blue
	//10: Bright green
	//11: Bright cyan
	//12: Bright red
	//13: Pink/Magenta
	//14: Yellow
	//15: Bright white
	//Numbers after 15 include background colors
	//16: background blue  0x10
	//32: background green 0x20
	//48: background cyan  0x30
	//64: background red   0x40
	//80: background purple 0x50
	//96: background brown 0x60
	//108: background white 0x70
	//124: background gray 0x80

	// map cube color to windows color attribute
	// Up=white, Down=yellow, Left=green, Right=blue,
	// Front=red, Back=orange

	// colors not in wincon.h
	enum Color : int {
		FOREGROUND_BLACK=0,
		FOREGROUND_CYAN=3,
		FOREGROUND_PURPLE=5,
		FOREGROUND_BROWN=6,
		FOREGROUND_WHITE=7,
		FOREGROUND_GRAY=8,
		FOREGROUND_ORANGE=12,
		FOREGROUND_YELLOW=14,
	};

	std::vector<int> face2FGcolor = {
		FOREGROUND_WHITE,
		FOREGROUND_YELLOW,
		FOREGROUND_RED,
		FOREGROUND_ORANGE,
		FOREGROUND_GREEN,
		FOREGROUND_BLUE,
	};

	std::vector<int> face2BGcolor = {
		FOREGROUND_WHITE << 4,
		FOREGROUND_YELLOW << 4,
		FOREGROUND_RED << 4,
		FOREGROUND_ORANGE << 4,
		FOREGROUND_GREEN << 4,
		FOREGROUND_BLUE << 4,
	};

	 HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	 CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	 // Save the current text colors.
	 GetConsoleScreenBufferInfo(hConsole, &csbiInfo);
	WORD wOldColorAttrs = csbiInfo.wAttributes;

	// print "L B D F R U" above faces (left,back,down,front,right,up)
	std::cout << "       L                B                D                F                R                U" << "\n";
	// display the faces L B D F R U as 3x3 squares
	// show the scrambled and solved cube
	// clear the move queue
	for (int row = 0; row < DIM; ++row) {
		for (face f : {face::L, face::B, face::D, face::F, face::R, face::U}) {
			// facelets
			for (int col = 0; col < DIM; ++col) {
				SetConsoleTextAttribute(hConsole,
						face2FGcolor[facelets[static_cast<int>(f)][row][col]] | face2BGcolor[facelets[static_cast<int>(f)][row][col]]);
				std::cout << "    ";
				// Restore the original text colors.
				SetConsoleTextAttribute(hConsole, wOldColorAttrs);
				// add vertical separation between facelets
				std::cout << " ";
			}
			// Restore the original text colors.
			SetConsoleTextAttribute(hConsole, wOldColorAttrs);
			std::cout << "  ";
		}
		std::cout << "\n";
		for (face f : {face::L, face::B, face::D, face::F, face::R, face::U}) {
			// facelets
			for (int col = 0; col < DIM; ++col) {
				SetConsoleTextAttribute(hConsole,
						face2FGcolor[facelets[static_cast<int>(f)][row][col]] | face2BGcolor[facelets[static_cast<int>(f)][row][col]]);
				std::cout << "    ";
				// Restore the original text colors.
				SetConsoleTextAttribute(hConsole, wOldColorAttrs);
				// add vertical separation between facelets
				std::cout << " ";
			}
			// Restore the original text colors.
			SetConsoleTextAttribute(hConsole, wOldColorAttrs);
			std::cout << "  ";
		}
		std::cout << "\n";
		// add horizontal separation between facelets
		for (int i = 0; i < NFACES; i++) {
			// facelets
			for (int col = 0; col < DIM; ++col) {
				std::cout << "    ";
				std::cout << " ";
			}
			std::cout << "  ";
		}
		std::cout << "\n";
	}
	// Restore the original text colors.
	SetConsoleTextAttribute(hConsole, wOldColorAttrs);
}


void Cube::tabulateTestResults()
{
	// show trial move count and time

	// convert rotations to string
	std::vector<std::string> rot2char{
		"U", "U'", "U2", "D", "D'", "D2", "L", "L'", "L2", "R", "R'", "R2", "F", "F'", "F2", "B", "B'", "B2"};

	// buffer to hold the moves in reverse order
	std::vector<rotate> buf;
	// clear the move queue
	while (!mvQueue.empty()) {
		mvQueue.pop();
	}
	// Copy the rotStack solution rotations to buffer in reverse order
	while (!rotStack.empty()) {
		buf.push_back(rotStack.top());
		rotStack.pop();
	}
	std::cout << "cube solution moves: ";
	// recover the forward IDA solution moves by
	// reversing the reverse order in buffer
	for (auto revit = buf.rbegin(); revit != buf.rend(); ++revit) {
		mvQueue.push(*revit);
	}
}

// Create six 3x3 cube faces from the cube state
void Cube::createCubeFaces(bool init)
{

	uint8_t prev[DIM];
	uint8_t buf[DIM][DIM];
	const std::vector<std::string> rotate2str{"U", "U'", "U2", "D", "D'", "D2",
		"L", "L'", "L2", "R", "R'", "R2", "F", "F'", "F2", "B", "B'", "B2"};

	// initialize the facelets of a face to the same color (the solved state)
	if (init) {
		for (int i = 0; i < DIM; ++i) {
			for (int j = 0; j < DIM; ++j) {
				// U and D faces, white and yellow
				facelets[static_cast<uint8_t>(face::U)][i][j] = static_cast<uint8_t>(color::WHITE);
				facelets[static_cast<uint8_t>(face::D)][i][j] = static_cast<uint8_t>(color::YELLOW);
				// L and R faces, green and blue
				facelets[static_cast<uint8_t>(face::L)][i][j] = static_cast<uint8_t>(color::GREEN);
				facelets[static_cast<uint8_t>(face::R)][i][j] = static_cast<uint8_t>(color::BLUE);
				// F and B faces, red and orange
				facelets[static_cast<uint8_t>(face::F)][i][j] = static_cast<uint8_t>(color::RED);
				facelets[static_cast<uint8_t>(face::B)][i][j] = static_cast<uint8_t>(color::ORANGE);
			}
		}
		// print the scramble moves
		std::cout << "cube scramble moves: ";
	}

	// Determine which face to rotate by popping the mvQueue
	while (!mvQueue.empty()) {
		rotate rot = mvQueue.front();
		std::cout << rotate2str[static_cast<uint8_t>(rot)] << " ";
		switch (rot) {
		/**********************************************************************/
		case rotate::U:
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					buf[i][DIM-1-j] = facelets[static_cast<uint8_t>(face::U)][j][i];
				}
			}
			// copy back to facelets from buf
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					facelets[static_cast<uint8_t>(face::U)][j][i] = buf[j][i];
				}
			}
			// U, rotate R,B,L,F row 0 to the left
			// previous = F
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[static_cast<uint8_t>(face::F)][0][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::F)][0][i] = facelets[static_cast<uint8_t>(face::R)][0][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::R)][0][i] = facelets[static_cast<uint8_t>(face::B)][0][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::B)][0][i] = facelets[static_cast<uint8_t>(face::L)][0][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::L)][0][i] = prev[i];
			}
			break;
		/**********************************************************************/
		case rotate::U_:
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					buf[DIM-1-i][j] = facelets[static_cast<uint8_t>(face::U)][j][i];
				}
			}
			// copy back to facelets from buf
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					facelets[static_cast<uint8_t>(face::U)][j][i] = buf[j][i];
				}
			}
			// U_, rotate L,B,R,F, row 0 to the right
			// previous = F
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[static_cast<uint8_t>(face::F)][0][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::F)][0][i] = facelets[static_cast<uint8_t>(face::L)][0][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::L)][0][i] = facelets[static_cast<uint8_t>(face::B)][0][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::B)][0][i] = facelets[static_cast<uint8_t>(face::R)][0][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::R)][0][i] = prev[i];
			}
			break;
		/**********************************************************************/
		case rotate::U2:
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					buf[DIM-1-j][DIM-1-i] = facelets[static_cast<uint8_t>(face::U)][j][i];
				}
			}
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					facelets[static_cast<uint8_t>(face::U)][j][i] = buf[j][i];
				}
			}
			// U2, swap L-R row 0
			// previous = L
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[static_cast<uint8_t>(face::L)][0][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::L)][0][i] = facelets[static_cast<uint8_t>(face::R)][0][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::R)][0][i] = prev[i];
			}
			// U2, swap B-F row 0
			// previous = B
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[static_cast<uint8_t>(face::B)][0][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::B)][0][i] = facelets[static_cast<uint8_t>(face::F)][0][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::F)][0][i] = prev[i];
			}
			break;
		/**********************************************************************/
		case rotate::D:
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					buf[i][DIM-1-j] = facelets[static_cast<uint8_t>(face::D)][j][i];
				}
			}
			// copy back to facelets from buf
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					facelets[static_cast<uint8_t>(face::D)][j][i] = buf[j][i];
				}
			}
			// D, rotate L,B,R,F row 2 to the right
			// previous = F
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[static_cast<uint8_t>(face::F)][DIM-1][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::F)][DIM-1][i] = facelets[static_cast<uint8_t>(face::L)][DIM-1][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::L)][DIM-1][i] = facelets[static_cast<uint8_t>(face::B)][DIM-1][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(static_cast<uint8_t>(face::B))][DIM-1][i] = facelets[static_cast<uint8_t>(face::R)][DIM-1][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::R)][DIM-1][i] = prev[i];
			}
			break;
		/**********************************************************************/
		case rotate::D_:
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					buf[DIM-1-i][j] = facelets[static_cast<uint8_t>(face::D)][j][i];
				}
			}
			// copy back to facelets from buf
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					facelets[static_cast<uint8_t>(face::D)][j][i] = buf[j][i];
				}
			}
			// D_, rotate R,B,L,F row 2 to the left
			// previous = F
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[static_cast<uint8_t>(face::F)][DIM-1][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::F)][DIM-1][i] = facelets[static_cast<uint8_t>(face::R)][DIM-1][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::R)][DIM-1][i] = facelets[static_cast<uint8_t>(face::B)][DIM-1][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::B)][DIM-1][i] = facelets[static_cast<uint8_t>(face::L)][DIM-1][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::L)][DIM-1][i] = prev[i];
			}
			break;
		/**********************************************************************/
		case rotate::D2:
			// D2, swap L-R row 2
			// previous = L
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[static_cast<uint8_t>(face::L)][DIM-1][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::L)][DIM-1][i] = facelets[static_cast<uint8_t>(face::R)][DIM-1][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::R)][DIM-1][i] = prev[i];
			}
			// D2, swap B-F row 2
			// previous = B
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[static_cast<uint8_t>(face::B)][DIM-1][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::B)][DIM-1][i] = facelets[static_cast<uint8_t>(face::F)][DIM-1][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::F)][DIM-1][i] = prev[i];
			}
			break;
		/**********************************************************************/
		case rotate::L:
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					buf[i][DIM-1-j] = facelets[static_cast<uint8_t>(face::L)][j][i];
				}
			}
			// copy back to facelets from buf
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					facelets[static_cast<uint8_t>(face::L)][j][i] = buf[j][i];
				}
			}
			// L, rotate B,D,F,U col 0 down
			// previous = U
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[static_cast<uint8_t>(face::U)][i][DIM-1];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::U)][i][DIM-1] = facelets[static_cast<uint8_t>(face::B)][i][DIM-1];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::B)][i][DIM-1] = facelets[static_cast<uint8_t>(face::D)][DIM-1-i][0];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::D)][i][0] = facelets[static_cast<uint8_t>(face::F)][i][0];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::F)][i][0] = prev[DIM-1-i];
			}
			break;
		/**********************************************************************/
		case rotate::L_:
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					buf[DIM-1-i][j] = facelets[static_cast<uint8_t>(face::L)][j][i];
				}
			}
			// copy back to facelets from buf
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					facelets[static_cast<uint8_t>(face::L)][j][i] = buf[j][i];
				}
			}
			// L_, rotate F,D,B,U col 0 up
			// previous = U
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[static_cast<uint8_t>(face::U)][i][DIM-1];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::U)][i][DIM-1] = facelets[static_cast<uint8_t>(face::F)][DIM-1-i][0];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::F)][i][0] = facelets[static_cast<uint8_t>(face::D)][i][0];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::D)][i][0] = facelets[static_cast<uint8_t>(face::B)][DIM-1-i][DIM-1];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::B)][i][DIM-1] = prev[i];
			}
			break;
		/**********************************************************************/
		case rotate::L2:
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					buf[DIM-1-j][DIM-1-i] = facelets[static_cast<uint8_t>(face::L)][j][i];
				}
			}
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					facelets[static_cast<uint8_t>(face::L)][j][i] = buf[j][i];
				}
			}
			// L2, swap U-D col 0
			// previous = U
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[static_cast<uint8_t>(face::U)][i][DIM-1];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::U)][i][DIM-1] = facelets[static_cast<uint8_t>(face::D)][DIM-1-i][0];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::D)][DIM-1-i][0] = prev[i];
			}
			// L2, swap B-F col 0
			// previous = B
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[static_cast<uint8_t>(face::B)][i][DIM-1];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::B)][i][DIM-1] = facelets[static_cast<uint8_t>(face::F)][DIM-1-i][0];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::F)][i][0] = prev[DIM-1-i];
			}
			break;
		/**********************************************************************/
		case rotate::R:
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					buf[i][DIM-1-j] = facelets[static_cast<uint8_t>(face::R)][j][i];
				}
			}
			// copy back to facelets from buf
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					facelets[static_cast<uint8_t>(face::R)][j][i] = buf[j][i];
				}
			}
			// R, rotate F,D,B,U col 2 up
			// previous = U
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[static_cast<uint8_t>(face::U)][i][0];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::U)][i][0] = facelets[static_cast<uint8_t>(face::F)][DIM-1-i][DIM-1];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::F)][i][DIM-1] = facelets[static_cast<uint8_t>(face::D)][i][DIM-1];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::D)][i][DIM-1] = facelets[static_cast<uint8_t>(face::B)][DIM-1-i][0];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::B)][i][0] = prev[i];
			}
			break;
		/**********************************************************************/
		case rotate::R_:
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					buf[DIM-1-i][j] = facelets[static_cast<uint8_t>(face::R)][j][i];
				}
			}
			// copy back to facelets from buf
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					facelets[static_cast<uint8_t>(face::R)][j][i] = buf[j][i];
				}
			}
			// R_, rotate B,D,F,U col 2 down
			// previous = U
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[static_cast<uint8_t>(face::U)][i][0];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::U)][i][0] = facelets[static_cast<uint8_t>(face::B)][i][0];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::B)][i][0] = facelets[static_cast<uint8_t>(face::D)][DIM-1-i][DIM-1];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::D)][i][DIM-1] = facelets[static_cast<uint8_t>(face::F)][i][DIM-1];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::F)][i][DIM-1] = prev[DIM-1-i];
			}
			break;
		/**********************************************************************/
		case rotate::R2:
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					buf[DIM-1-j][DIM-1-i] = facelets[static_cast<uint8_t>(face::R)][j][i];
				}
			}
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					facelets[static_cast<uint8_t>(face::R)][j][i] = buf[j][i];
				}
			}
			// R2, swap U-D col 2
			// previous = U
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[static_cast<uint8_t>(face::U)][i][0];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::U)][i][0] = facelets[static_cast<uint8_t>(face::D)][DIM-1-i][DIM-1];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::D)][i][DIM-1] = prev[DIM-1-i];
			}
			// R2, swap B-F col 2
			// previous = B
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[static_cast<uint8_t>(face::B)][i][0];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::B)][i][0] = facelets[static_cast<uint8_t>(face::F)][DIM-1-i][DIM-1];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::F)][i][DIM-1] = prev[DIM-1-i];
			}
			break;
		/**********************************************************************/
		case rotate::F:
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					buf[i][DIM-1-j] = facelets[static_cast<uint8_t>(face::F)][j][i];
				}
			}
			// copy back to facelets from buf
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					facelets[static_cast<uint8_t>(face::F)][j][i] = buf[j][i];
				}
			}
			// F, rotate L col 2 up, D row 2 left, R col 0 down,  U row 2 right
			// previous = U
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[static_cast<uint8_t>(face::U)][0][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::U)][0][i] = facelets[static_cast<uint8_t>(face::L)][i][DIM-1];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::L)][i][DIM-1] = facelets[static_cast<uint8_t>(face::D)][0][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::D)][0][i] = facelets[static_cast<uint8_t>(face::R)][DIM-1-i][0];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::R)][i][0] = prev[DIM-1-i];
			}
			break;
		/**********************************************************************/
		case rotate::F_:
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					buf[DIM-1-i][j] = facelets[static_cast<uint8_t>(face::F)][j][i];
				}
			}
			// copy back to facelets from buf
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					facelets[static_cast<uint8_t>(face::F)][j][i] = buf[j][i];
				}
			}
			// F_, rotate R col 0 up, D row 2 right, L col 2 down, U row 2 left
			// previous = U
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[static_cast<uint8_t>(face::U)][0][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::U)][0][i] = facelets[static_cast<uint8_t>(face::R)][DIM-1-i][0];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::R)][i][0] = facelets[static_cast<uint8_t>(face::D)][0][DIM-1-i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::D)][0][i] = facelets[static_cast<uint8_t>(face::L)][i][DIM-1];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::L)][i][DIM-1] = prev[i];
			}
			break;
		/**********************************************************************/
		case rotate::F2:
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					buf[DIM-1-j][DIM-1-i] = facelets[static_cast<uint8_t>(face::F)][j][i];
				}
			}
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					facelets[static_cast<uint8_t>(face::F)][j][i] = buf[j][i];
				}
			}
			// F2, swap Lcol2-Rcol0
			// previous = L
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[static_cast<uint8_t>(face::L)][i][DIM-1];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::L)][i][DIM-1] = facelets[static_cast<uint8_t>(face::R)][DIM-1-i][0];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::R)][DIM-1-i][0] = prev[i];
			}
			// F2, swap Urow2-Drow2
			// previous = U
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[static_cast<uint8_t>(face::U)][0][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::U)][0][i] = facelets[static_cast<uint8_t>(face::D)][0][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::D)][0][i] = prev[i];
			}
			break;
		/**********************************************************************/
		case rotate::B:
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					buf[i][DIM-1-j] = facelets[static_cast<uint8_t>(face::B)][j][i];
				}
			}
			// copy back to facelets from buf
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					facelets[static_cast<uint8_t>(face::B)][j][i] = buf[j][i];
				}
			}
			// B, rotate R col 2 up, D row 0 left, L col 0 down, U row 0 right
			// previous = U
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[static_cast<uint8_t>(face::U)][DIM-1][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::U)][DIM-1][i] = facelets[static_cast<uint8_t>(face::R)][DIM-1-i][DIM-1];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::R)][i][DIM-1] = facelets[static_cast<uint8_t>(face::D)][DIM-1][DIM-1-i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::D)][DIM-1][i] = facelets[static_cast<uint8_t>(face::L)][i][0];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::L)][i][0] = prev[i];
			}
			break;
		/**********************************************************************/
		case rotate::B_:
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					buf[DIM-1-i][j] = facelets[static_cast<uint8_t>(face::B)][j][i];
				}
			}
			// copy back to facelets from buf
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					facelets[static_cast<uint8_t>(face::B)][j][i] = buf[j][i];
				}
			}
			// B_, rotate L col 0 up, D row 0 right, R col 2 down, U row 0 left
			// previous =
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[static_cast<uint8_t>(face::U)][DIM-1][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::U)][DIM-1][i] = facelets[static_cast<uint8_t>(face::L)][i][0];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::L)][i][0] = facelets[static_cast<uint8_t>(face::D)][DIM-1][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::D)][DIM-1][i] = facelets[static_cast<uint8_t>(face::R)][DIM-1-i][DIM-1];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::R)][DIM-1-i][DIM-1] = prev[i];
			}
			break;
		/**********************************************************************/
		case rotate::B2:
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					buf[DIM-1-j][DIM-1-i] = facelets[static_cast<uint8_t>(face::B)][j][i];
				}
			}
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					facelets[static_cast<uint8_t>(face::B)][j][i] = buf[j][i];
				}
			}
			// B2, swap Lcol0-Rcol2
			// previous = L
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[static_cast<uint8_t>(face::L)][i][0];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::L)][i][0] = facelets[static_cast<uint8_t>(face::R)][DIM-1-i][DIM-1];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::R)][DIM-1-i][DIM-1] = prev[i];
			}
			// B2, swap Urow0-Drow0
			// previous = U
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[static_cast<uint8_t>(face::U)][DIM-1][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::U)][DIM-1][i] = facelets[static_cast<uint8_t>(face::D)][DIM-1][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[static_cast<uint8_t>(face::D)][DIM-1][i] = prev[i];
			}
			break;
		/**********************************************************************/
		default:
			std::cout << "createCubeFaces invalid rotation " << static_cast<int>(rot) << std::endl;
		}
		mvQueue.pop();
	}
	std::cout << "\n";
}

// run IDA*, IDDFS with pruning, Richard Korf algorithm
void Cube::performIDAstar()
{
    /*
	function Iterative Deepening A*(cube)
		moveStack <- []
		bound <- GetHeuristic (cube)
		while IDA_Iteration(cube, 0, bound) == False do ⊲ As long as no solution is found
			bound = nextBound
			nextBound = Max_Value // Let this be accessible for IDA_Iteration
		end while
		return moveStack
	end function

	function IDA_Iteration(cube, depth, bound)
		estimatedMoves  <- GetHeuristic(cube) + depth
		if estimatedMoves > bound then
		  return False;
		else
		  if estimatedMoves < nextBound then
		    nextBound <- estimatedMoves
		  end if
		end if
		if IsSolved(cube) then
		  return True;
		end if
		for move={U , U’, U2, ..., D, D’, D2} do // Loop through all possible moves
			ApplyMove (cube,move)
			moveStack.push(move)
			if IDA_Iteration(cube,depth + 1, bound) == True then
			    return True;
			end if
			moveStack.pop()
			ApplyMove (cube, Inverse(move))
		end for
		return False
	end function
    */

}

inline void Cube::doMove(rotate rot)
{
	(this->*cubefcn[int(rot)])();
}

// embed cube faces into the 3D scatterplot cube
// Up/Down face maps to z-axis planes 0, 47
void Cube::embedUD(uint8_t cube3D[DIM3D][DIM3D][DIM3D])
{
	// Duplicate for Up and Down face

    // Left/Right to X-axis, Front/Back to Y-axis, Up/Down to -Z-axis
    // Add 1 to the enum::color value, 1-6 instead of 0-5
    // Insert the facelet color in the 16x16 area of the plane of the 3D cube
    // facelet borders:  (0, 15), (16, 31), (32, 47)

	// width and height of facelets
	int spc = 16;
	// loop over row facelets, 0,1,2
	for (int rowfacelt = 0; rowfacelt < DIM; ++rowfacelt) {
        // set start,stop row for this facelet: (0, 16, 32)
		int rowfacestart = rowfacelt*spc;
		int rowfacestop = rowfacestart + spc;
	    // loop over column facelets, 0,1,2
		for (int colfacelt = 0; colfacelt < DIM; ++colfacelt) {
	        // set start,stop column for this facelet: (0, 16, 32)
			int colfacestart = colfacelt*spc;
			int colfacestop = colfacestart + spc;
		    // Add 1 to the enum::color value, 1-6 instead of 0-5
			uint8_t clr1 = facelets[static_cast<uint8_t>(face::D)][rowfacelt][colfacelt] + 1;
			uint8_t clr2 = facelets[static_cast<uint8_t>(face::U)][rowfacelt][colfacelt] + 1;
		    // loop delta row, duplicate 16x this color over rows
			for (int row = rowfacestart; row < rowfacestop; ++row) {
			    // loop delta col, duplicate 16x this color over cols
				for (int col = colfacestart; col < colfacestop; ++col) {
					// set color from facelet[U|D][row][col] to cube3D[0|47][row][col]
					cube3D[row][col][0] = clr1;
					cube3D[row][col][DIM3D-1] = clr2;
				}
			}
		}
	}

	// set black borders around facelets rows/cols at: 0, 15, 16, 31, 32, 47
	// for row in (0,15,16,31,32,47)
	for (int row : {0, 15, 16, 31, 32, 47}) {
	  // loop over cols of cube3D
		for (int col = 0; col < DIM3D; ++col) {
			// set cube3D[0|47][row][col] to black
			cube3D[row][col][0] = black;
			cube3D[row][col][DIM3D-1] = black;
		}
	}
	// for col in (0,15,16,31,32,47)
	for (int col : {0, 15, 16, 31, 32, 47}) {
		// loop over rows of cube3D
		for (int row = 0; row < DIM3D; ++row) {
		    // set cube3D[0|47][row][col] to black
			cube3D[row][col][0] = black;
			cube3D[row][col][DIM3D-1] = black;
		}
	}
}

// Left/Right face maps to x-axis planes 0, 47
void Cube::embedLR(uint8_t cube3D[DIM3D][DIM3D][DIM3D])
{
	// Duplicate for Left and Right face

    // Left/Right to X-axis, Front/Back to Y-axis, Up/Down to -Z-axis
    // Add 1 to the enum::color value, 1-6 instead of 0-5
    // Insert the facelet color in the 16x16 area of the plane of the 3D cube
    // facelet borders:  (0, 15), (16, 31), (32, 47)

	// width and height of facelets
	int spc = 16;
	// loop over row facelets, 0,1,2
	for (int rowfacelt = 0; rowfacelt < DIM; ++rowfacelt) {
        // set start,stop row for this facelet: (0, 16, 32)
		int rowfacestart = rowfacelt*spc;
		int rowfacestop = rowfacestart + spc;
	    // loop over column facelets, 0,1,2
		for (int colfacelt = 0; colfacelt < DIM; ++colfacelt) {
	        // set start,stop column for this facelet: (0, 16, 32)
			int colfacestart = colfacelt*spc;
			int colfacestop = colfacestart + spc;
		    // Add 1 to the enum::color value, 1-6 instead of 0-5
			uint8_t clr1 = facelets[static_cast<uint8_t>(face::L)][rowfacelt][colfacelt] + 1;
			uint8_t clr2 = facelets[static_cast<uint8_t>(face::R)][rowfacelt][colfacelt] + 1;
		    // loop delta row, duplicate 16x this color over rows
			for (int row = rowfacestart; row < rowfacestop; ++row) {
			    // loop delta col, duplicate 16x this color over cols
				for (int col = colfacestart; col < colfacestop; ++col) {
					// set color from facelet[U|D][row][col] to cube3D[0|47][row][col]
					cube3D[0][row][col] = clr1;
					cube3D[DIM3D-1][row][col] = clr2;
				}
			}
		}
	}

	// set black borders around facelets rows/cols at: 0, 15, 16, 31, 32, 47
	// for row in (0,15,16,31,32,47)
	for (int row : {0, 15, 16, 31, 32, 47}) {
	  // loop over cols of cube3D
		for (int col = 0; col < DIM3D; ++col) {
			// set cube3D[0|47][row][col] to black
			cube3D[0][row][col] = black;
			cube3D[DIM3D-1][row][col] = black;
		}
	}
	// for col in (0,15,16,31,32,47)
	for (int col : {0, 15, 16, 31, 32, 47}) {
		// loop over rows of cube3D
		for (int row = 0; row < DIM3D; ++row) {
		    // set cube3D[0|47][row][col] to black
			cube3D[0][row][col] = black;
			cube3D[DIM3D-1][row][col] = black;
		}
	}
}

// Front/Back face maps to y-axis planes 0, 47
void Cube::embedFB(uint8_t cube3D[DIM3D][DIM3D][DIM3D])
{
	// Duplicate for Front and Back face

    // Left/Right to X-axis, Front/Back to Y-axis, Up/Down to -Z-axis
    // Add 1 to the enum::color value, 1-6 instead of 0-5
    // Insert the facelet color in the 16x16 area of the plane of the 3D cube
    // facelet borders:  (0, 15), (16, 31), (32, 47)

	// width and height of facelets
	int spc = 16;
	// loop over row facelets, 0,1,2
	for (int rowfacelt = 0; rowfacelt < DIM; ++rowfacelt) {
        // set start,stop row for this facelet: (0, 16, 32)
		int rowfacestart = rowfacelt*spc;
		int rowfacestop = rowfacestart + spc;
	    // loop over column facelets, 0,1,2
		for (int colfacelt = 0; colfacelt < DIM; ++colfacelt) {
	        // set start,stop column for this facelet: (0, 16, 32)
			int colfacestart = colfacelt*spc;
			int colfacestop = colfacestart + spc;
		    // Add 1 to the enum::color value, 1-6 instead of 0-5
			uint8_t clr1 = facelets[static_cast<uint8_t>(face::B)][rowfacelt][colfacelt] + 1;
			uint8_t clr2 = facelets[static_cast<uint8_t>(face::F)][rowfacelt][colfacelt] + 1;
		    // loop delta row, duplicate 16x this color over rows
			for (int row = rowfacestart; row < rowfacestop; ++row) {
			    // loop delta col, duplicate 16x this color over cols
				for (int col = colfacestart; col < colfacestop; ++col) {
					// set color from facelet[U|D][row][col] to cube3D[0|47][row][col]
					cube3D[row][DIM3D-1][col] = clr1;
					cube3D[row][0][col] = clr2;
				}
			}
		}
	}

	// set black borders around facelets rows/cols at: 0, 15, 16, 31, 32, 47
	// for row in (0,15,16,31,32,47)
	for (int row : {0, 15, 16, 31, 32, 47}) {
	  // loop over cols of cube3D
		for (int col = 0; col < DIM3D; ++col) {
			// set cube3D[0|47][row][col] to black
			cube3D[row][0][col] = black;
			cube3D[row][DIM3D-1][col] = black;
		}
	}
	// for col in (0,15,16,31,32,47)
	for (int col : {0, 15, 16, 31, 32, 47}) {
		// loop over rows of cube3D
		for (int row = 0; row < DIM3D; ++row) {
		    // set cube3D[0|47][row][col] to black
			cube3D[row][0][col] = black;
			cube3D[row][DIM3D-1][col] = black;
		}
	}
}

// Convert facelets[][][] to a 3D cube for a matplotlib.pyplot scatterplot and save .txt
void Cube::create3Dcube(const std::string &file)
{
	// Create a realistic-looking Rubik's cube for matplotlib
	// Map each Rubik Cube face to a 48x48 plane in a 3D cube
	// The planes are the 0 and 47 edges of the 3D cube, thus creating a surface
	// Expand each facelet to occupy 16x16 elements of the 3D cube in the corresponding plane
	// Draw black borders separating the facelets: 4 horizontal and 4 vertical

	// create uint8_t cube3D[DIM3D][DIM3D][DIM3D] and initialize elements to 0
	uint8_t cube3D[DIM3D][DIM3D][DIM3D];
	for (int i = 0; i < DIM3D; ++i) {
		for (int j = 0; j < DIM3D; ++j) {
			for (int k = 0; k < DIM3D; ++k) {
				cube3D[i][j][k] = 0;
			}
		}
	}

	embedUD(cube3D);
	embedLR(cube3D);
	embedFB(cube3D);

	// Save the cube to rubikcube_scrambled.txt or rubikcube_solved.txt
	std::fstream fcube3D;
	// Save the cube to a file
	fcube3D.open(file.c_str(), std::fstream::out);
	if (!fcube3D.is_open()) {
		std::cout << "cannot open file " + file << std::endl;
		throw std::runtime_error("cannot open file " + file);
	}

	for (const auto &dim1 : cube3D) {
		for (const auto &dim2 : dim1) {
			for (int dim3 : dim2) {
				fcube3D << dim3 << " ";
			}
			fcube3D << std::endl;
		}
	}
	fcube3D.close();

}

// recurse the cube state tree with DFS
bool Cube::boundDFS(int depth, int bound)
{
	if (depth > bound) {
	    return false;
	}
	if (isSolved()) {
	    return true;
	}
	// Loop through all possible moves
	for (rotate rot : {rotate::U, rotate::U_, rotate::U2, rotate::D, rotate::D_, rotate::D2,
		rotate::L, rotate::L_, rotate::L2, rotate::R, rotate::R_, rotate::R2,
		rotate::F, rotate::F_, rotate::F2, rotate::B, rotate::B_, rotate::B2}) {
		// do a face twist of the cube state
		doMove(rot);
		// keep track of our moves
		rotStack.push(rot);
		if (boundDFS(depth + 1, bound)) {
			  return true;
		}
		// remove the last move since it didn't work
	    rotStack.pop();
	    // undo the move, do the reverse twist to the cube state
	    doMove(revRotate[static_cast<int>(rot)]);
	}
	return false;
}

// run IDDFS, Iterative Deepening Depth First Search
void Cube::performIDA()
{
	int depth = 0;
	int bound = 1;
	std::cout << "search bound = " << bound << std::endl;
	while (!boundDFS(depth, bound)) {
		++bound;
		std::cout << "search bound = " << bound << std::endl;
	}
}

void handleIDA(int trials, int moves)
{
	// create a cube with the trials and moves
	Cube cube(trials, moves);

    // show start, end, and elapsed times
    time_t rawtime1;
    time_t rawtime2;
    struct tm *timeinfo;

	// loop over the trials
	for (int tri = 0; tri < trials; ++tri) {
		// start time
	    time(&rawtime1);
	    timeinfo = localtime (&rawtime1);
	    std::cout << std::string("Start local time and date: ") << std::string(asctime(timeinfo)) << std::endl;

		// Scramble cube starting position and save moves
		cube.scrambleCube(moves);

		// Create the cube faces using saved moves
		// and display them, 6 faces 3x3 in one row
		cube.createCubeFaces(true);
		cube.displayCubeFaces();

		// Save the scrambled cube for 3D scatterplot in matplotlib.pyplot
		cube.create3Dcube(rubik_scrambled);

		// Perform IDA()
		cube.performIDA();

		// tabulate the results:  solution time and 1/4 turn metric (QTM)
		cube.tabulateTestResults();

		// Create and Display the faces of the solution
		cube.createCubeFaces(false);
		cube.displayCubeFaces();

		// Save the solution cube for 3D scatterplot in matplotlib.pyplot
		cube.create3Dcube(rubik_solved);

		// end time, elapsed time
	    time(&rawtime2);
	    timeinfo = localtime (&rawtime2);
	    std::cout << std::string("Finish local time and date: ") << std::string(asctime(timeinfo)) << std::endl;
	    double seconds = std::difftime(rawtime2,rawtime1);
	    std::cout << "Elapsed time: " << seconds << " seconds\n";
	}
}

void handleIDAstar(int trials, int moves)
{
	// create a cube
	// create pattern databases if necessary
	// read in the three pattern databases:  corners1-8, edges1-6, edges 7-12
	// create a cube with the moves and trials
	// loop over the trials
	//   Scramble cube starting position
	//   Create the cube faces and display them, 6 faces 3x3 in one row
	//   Perform IDA* (IDDFS with pruning)
	//   tabulate the results:  solution time and 1/4 turn metric (QTM)
	//   Display the faces of the solution
}

int main(int argc, char *argv[]) {
	// seed the random number generator so it changes over time
	std::srand(time(NULL));

	// use command line arguments
	if (argc == 2) {
		if (std::strcmp(argv[1], "1") == 0) {
			handleIDA(1,1);
		} else if (std::strcmp(argv[1], "2") == 0) {
			handleIDAstar(1,1);
		} else {
			std::cout << "you entered invalid option " << argv[1] << std::endl;
		}
	} else {
		const int min_moves = 1;
		const int max_moves = 10;
		const int min_trials = 1;
		const int max_trials = 10;
		int moves = 1;
		int trials = 1;
		std::string prune;
		std::ostringstream result;
		// Enter the number of moves
		std::cout << "Enter the number of Rubik's Cube moves (1-10): ";
		std::cin >> moves;
		// Enter the number of trials
		std::cout << "Enter the number of trials using the given number of moves (1-10): ";
		std::cin >> trials;
		// Enter IDA or IDA*
		std::cout << "Use pruning tables (y/n): ";
		std::cin >> prune;
		if ((moves < min_moves) || (moves > max_moves)) {
			result << "moves not in [" << min_moves << "," << max_moves << "], ";
		}
		if ((trials < min_trials) || (trials > max_trials)) {
			result << "trials not in [" << min_trials << "," << max_trials << "], ";
		}
		if ((prune != "y") && (prune != "n")) {
			result << "prune is not 'y' or 'n'" << "\n";
		}
		if (result.str().size() > 0) {
			std::cout << result.str() << std::endl;
			return 1;
		}
		if (prune == "y") {
			handleIDAstar(trials, moves);
		} else {
			handleIDA(trials, moves);
		}

	}
	return 0;
}
