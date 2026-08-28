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
#include "rcmain.h"
#include <vector>
#include <cstdlib>

// constructor
Cube::Cube(int nt, int mt)
{
	ntrials = nt;
	maxTwists = mt;
}

// Up face rotate CW 90 deg
void Cube::twistU()
{

}

// Up face rotate CCW 90 deg
void Cube::twistU_()
{

}

// Up face rotate 180 deg
void Cube::twistU2()
{

}

// Down face rotate CW 90 deg
void Cube::twistD()
{

}

// Down face rotate CCW 90 deg
void Cube::twistD_()
{

}

// Down face rotate 180 deg
void Cube::twistD2()
{

}

// Left face rotate CW 90 deg
void Cube::twistL()
{

}

// Left face rotate CCW 90 deg
void Cube::twistL_()
{

}

// Left face rotate 180 deg
void Cube::twistL2()
{

}

// Right face rotate CW 90 deg
void Cube::twistR()
{

}

// Right face rotate CCW 90 deg
void Cube::twistR_()
{

}

// Right face rotate 180 deg
void Cube::twistR2()
{

}

// Front face rotate CW 90 deg
void Cube::twistF()
{

}

// Front face rotate CCW 90 deg
void Cube::twistF_()
{

}

// Front face rotate 180 deg
void Cube::twistF2()
{

}

// Back face rotate CW 90 deg
void Cube::twistB()
{

}

// Back face rotate CCW 90 deg
void Cube::twistB_()
{

}

// Back face rotate 180 deg
void Cube::twistB2()
{

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

// scramble solved cube with ntwists
void Cube::scrambleCube(int ntwists)
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
	for (int i = 0; i < ntwists; ++i) {
		// random selection of move
		rotate mv = static_cast<rotate>(std::rand() % MOVES);
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

// Display the cube faces
void Cube::displayCubeFaces()
{
	// print the cube move and "L B D F R U" face heading
	// display the faces L B D F R U as 3x3 squares
	// show the solved cube and after each move
	// clear the move queue


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
	std::cout << "\nmoves: ";
	// recover the forward IDA solution moves by
	// reversing the reverse order in buffer
	for (auto revit = buf.rbegin(); revit != buf.rend(); ++revit) {
		mvQueue.push(*revit);
		std::cout << rot2char[static_cast<int>(*revit)] << " ";
	}
	std::cout << std::endl;
}

// Create six 3x3 cube faces from the cube state
void Cube::createCubeFaces(bool init)
{

	color prev[DIM];
	color cur[DIM];
	color buf[DIM][DIM];

	// initialize the facelets to the solved state, each face the same color
	if (init) {
		for (int i = 0; i < DIM; ++i) {
			for (int j = 0; j < DIM; j++) {
				// U and D faces, yellow and white
				facelets[0][i][j] = color::YELLOW;
				facelets[DIM-1][i][j] = color::WHITE;
				// L and R faces, green and blue
				facelets[i][0][j] = color::GREEN;
				facelets[i][DIM-1][j] = color::BLUE;
				// F and B faces, orange and red
				facelets[i][j][0] = color::ORANGE;
				facelets[i][j][DIM-1] = color::RED;
			}
		}
	}

	// print "L B D F R U" above faces
	std::cout << "\nface: L    B    D    F    R    U" << std::endl;
	// Determine which face to rotate by popping the mvQueue
	while (!mvQueue.empty()) {
		rotate rot = mvQueue.front();
		switch (rot) {
		/**********************************************************************/
		case rotate::U:
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					buf[i][DIM-1-j] = facelets[face::U][j][i];
				}
			}
			// copy back to facelets from buf
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					facelets[face::U][j][i] = buf[j][i];
				}
			}
			// U, rotate R,B,L,F row 0 to the left
			// previous = F
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[face::F][0][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::F][0][i] = facelets[face::R][0][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::R][0][i] = facelets[face::B][0][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::B][0][i] = facelets[face::L][0][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::L][0][i] = prev[i];
			}
			break;
		/**********************************************************************/
		case rotate::U_:
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					buf[DIM-1-i][j] = facelets[face::U][j][i];
				}
			}
			// copy back to facelets from buf
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					facelets[face::U][j][i] = buf[j][i];
				}
			}
			// U_, rotate L,B,R,F, row 0 to the right
			// previous = F
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[face::F][0][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::F][0][i] = facelets[face::L][0][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::L][0][i] = facelets[face::B][0][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::B][0][i] = facelets[face::R][0][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::R][0][i] = prev[i];
			}
			break;
		/**********************************************************************/
		case rotate::U2:
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					buf[DIM-1-j][DIM-1-i] = facelets[face::U][j][i];
				}
			}
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					facelets[face::U][j][i] = buf[j][i];
				}
			}
			// U2, swap L-R row 0
			// previous = L
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[face::L][0][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::L][0][i] = facelets[face::R][0][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::R][0][i] = prev[i];
			}
			// U2, swap B-F row 0
			// previous = B
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[face::B][0][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::B][0][i] = facelets[face::F][0][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::F][0][i] = prev[i];
			}
			break;
		/**********************************************************************/
		case rotate::D:
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					buf[i][DIM-1-j] = facelets[face::D][j][i];
				}
			}
			// copy back to facelets from buf
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					facelets[face::D][j][i] = buf[j][i];
				}
			}
			// D, rotate L,B,R,F row 2 to the right
			// previous = F
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[face::F][DIM-1][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::F][DIM-1][i] = facelets[face::L][DIM-1][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::L][DIM-1][i] = facelets[face::B][DIM-1][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::B][DIM-1][i] = facelets[face::R][DIM-1][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::R][DIM-1][i] = prev[i];
			}
			break;
		/**********************************************************************/
		case rotate::D_:
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					buf[DIM-1-i][j] = facelets[face::D][j][i];
				}
			}
			// copy back to facelets from buf
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					facelets[face::D][j][i] = buf[j][i];
				}
			}
			// D_, rotate R,B,L,F row 2 to the left
			// previous = F
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[face::F][DIM-1][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::F][DIM-1][i] = facelets[face::R][DIM-1][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::R][DIM-1][i] = facelets[face::B][DIM-1][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::B][DIM-1][i] = facelets[face::L][DIM-1][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::L][DIM-1][i] = prev[i];
			}
			break;
		/**********************************************************************/
		case rotate::D2:
			// D2, swap L-R row 2
			// previous = L
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[face::L][DIM-1][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::L][DIM-1][i] = facelets[face::R][DIM-1][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::R][DIM-1][i] = prev[i];
			}
			// D2, swap B-F row 2
			// previous = B
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[face::B][DIM-1][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::B][DIM-1][i] = facelets[face::F][DIM-1][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::F][DIM-1][i] = prev[i];
			}
			break;
		/**********************************************************************/
		case rotate::L:
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					buf[i][DIM-1-j] = facelets[face::L][j][i];
				}
			}
			// copy back to facelets from buf
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					facelets[face::L][j][i] = buf[j][i];
				}
			}
			// L, rotate B,D,F,U col 0 down
			// previous = U
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[face::U][i][0];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::U][i][0] = facelets[face::B][i][0];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::B][i][0] = facelets[face::D][i][0];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::D][i][0] = facelets[face::F][i][0];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::F][i][0] = prev[i];
			}
			break;
		/**********************************************************************/
		case rotate::L_:
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					buf[DIM-1-i][j] = facelets[face::L][j][i];
				}
			}
			// copy back to facelets from buf
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					facelets[face::L][j][i] = buf[j][i];
				}
			}
			// L_, rotate F,D,B,U col 0 up
			// previous = U
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[face::U][i][0];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::U][i][0] = facelets[face::F][i][0];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::F][i][0] = facelets[face::D][i][0];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::D][i][0] = facelets[face::B][i][0];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::B][i][0] = prev[i];
			}
			break;
		/**********************************************************************/
		case rotate::L2:
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					buf[DIM-1-j][DIM-1-i] = facelets[face::L][j][i];
				}
			}
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					facelets[face::L][j][i] = buf[j][i];
				}
			}
			// L2, swap U-D col 0
			// previous = U
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[face::U][i][0];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::U][i][0] = facelets[face::D][i][0];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::D][i][0] = prev[i];
			}
			// L2, swap B-F col 0
			// previous = B
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[face::B][i][0];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::B][i][0] = facelets[face::F][i][0];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::F][i][0] = prev[i];
			}
			break;
		/**********************************************************************/
		case rotate::R:
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					buf[i][DIM-1-j] = facelets[face::R][j][i];
				}
			}
			// copy back to facelets from buf
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					facelets[face::R][j][i] = buf[j][i];
				}
			}
			// R, rotate F,D,B,U col 2 up
			// previous = U
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[face::U][i][DIM-1];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::U][i][DIM-1] = facelets[face::F][i][DIM-1];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::F][i][DIM-1] = facelets[face::D][i][DIM-1];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::D][i][DIM-1] = facelets[face::B][i][DIM-1];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::B][i][DIM-1] = prev[i];
			}
			break;
		/**********************************************************************/
		case rotate::R_:
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					buf[DIM-1-i][j] = facelets[face::R][j][i];
				}
			}
			// copy back to facelets from buf
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					facelets[face::R][j][i] = buf[j][i];
				}
			}
			// R_, rotate B,D,F,U col 2 down
			// previous = U
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[face::U][i][DIM-1];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::U][i][DIM-1] = facelets[face::B][i][DIM-1];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::B][i][DIM-1] = facelets[face::D][i][DIM-1];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::D][i][DIM-1] = facelets[face::F][i][DIM-1];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::F][i][DIM-1] = prev[i];
			}
			break;
		/**********************************************************************/
		case rotate::R2:
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					buf[DIM-1-j][DIM-1-i] = facelets[face::R][j][i];
				}
			}
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					facelets[face::R][j][i] = buf[j][i];
				}
			}
			// R2, swap U-D col 2
			// previous = U
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[face::U][i][DIM-1];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::U][i][DIM-1] = facelets[face::D][i][DIM-1];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::D][i][DIM-1] = prev[i];
			}
			// R2, swap B-F col 2
			// previous = B
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[face::B][i][0];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::B][i][0] = facelets[face::F][i][0];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::F][i][0] = prev[i];
			}
			break;
		/**********************************************************************/
		case rotate::F:
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					buf[i][DIM-1-j] = facelets[face::F][j][i];
				}
			}
			// copy back to facelets from buf
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					facelets[face::F][j][i] = buf[j][i];
				}
			}
			// F, rotate L col 2 up, D row 2 left, R col 0 down,  U row 2 right
			// previous = U
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[face::U][DIM-1][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::U][DIM-1][i] = facelets[face::L][DIM-1-i][DIM-1];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::L][i][DIM-1] = facelets[face::D][DIM-1][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::D][DIM-1][i] = facelets[face::R][DIM-1-i][0];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::R][i][0] = prev[i];
			}
			break;
		/**********************************************************************/
		case rotate::F_:
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					buf[DIM-1-i][j] = facelets[face::F][j][i];
				}
			}
			// copy back to facelets from buf
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					facelets[face::F][j][i] = buf[j][i];
				}
			}
			// F_, rotate R col 0 up, D row 2 right, L col 2 down, U row 2 left
			// previous = U
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[face::U][DIM-1][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::U][DIM-1][i] = facelets[face::R][i][0];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::R][i][0] = facelets[face::D][DIM-1][DIM-1-i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::D][DIM-1][i] = facelets[face::L][i][DIM-1];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::L][DIM-1-i][DIM-1] = prev[i];
			}
			break;
		/**********************************************************************/
		case rotate::F2:
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					buf[DIM-1-j][DIM-1-i] = facelets[face::F][j][i];
				}
			}
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					facelets[face::F][j][i] = buf[j][i];
				}
			}
			// F2, swap Lcol2-Rcol0
			// previous = L
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[face::L][i][DIM-1];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::L][i][DIM-1] = facelets[face::R][DIM-1-i][0];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::R][DIM-1-i][0] = prev[i];
			}
			// F2, swap Urow2-Drow2
			// previous = U
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[face::U][DIM-1][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::U][DIM-1][i] = facelets[face::D][DIM-1][DIM-1-i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::D][DIM-1][DIM-1-i] = prev[i];
			}
			break;
		/**********************************************************************/
		case rotate::B:
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					buf[i][DIM-1-j] = facelets[face::B][j][i];
				}
			}
			// copy back to facelets from buf
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					facelets[face::B][j][i] = buf[j][i];
				}
			}
			// B, rotate R col 2 up, D row 0 left, L col 0 down, U row 0 right
			// previous = U
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[face::U][0][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::U][0][i] = facelets[face::R][i][DIM-1];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::R][i][DIM-1] = facelets[face::D][0][DIM-1-i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::D][0][i] = facelets[face::L][i][DIM-1];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::L][DIM-1-i][0] = prev[i];
			}
			break;
		/**********************************************************************/
		case rotate::B_:
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					buf[DIM-1-i][j] = facelets[face::B][j][i];
				}
			}
			// copy back to facelets from buf
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					facelets[face::B][j][i] = buf[j][i];
				}
			}
			// B_, rotate L col 0 up, D row 0 right, R col 2 down, U row 0 left
			// previous =
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[face::U][DIM-1][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::U][DIM-1][i] = facelets[face::L][DIM-1-i][DIM-1];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::L][i][DIM-1] = facelets[face::D][DIM-1][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::D][DIM-1][i] = facelets[face::R][DIM-1-i][0];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::R][i][0] = prev[i];
			}
			break;
		/**********************************************************************/
		case rotate::B2:
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					buf[DIM-1-j][DIM-1-i] = facelets[face::B][j][i];
				}
			}
			for (int i = 0; i < DIM; ++i) {
				for (int j = 0; j < DIM; ++j) {
					facelets[face::B][j][i] = buf[j][i];
				}
			}
			// B2, swap Lcol0-Rcol2
			// previous = L
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[face::L][i][0];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::L][i][0] = facelets[face::R][DIM-1-i][DIM-1];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::R][DIM-1-i][DIM-1] = prev[i];
			}
			// B2, swap Urow0-Drow0
			// previous = U
			for (int i = 0; i < DIM; ++i) {
				prev[i] = facelets[face::U][0][i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::U][0][i] = facelets[face::D][0][DIM-1-i];
			}
			for (int i = 0; i < DIM; ++i) {
				facelets[face::D][0][DIM-1-i] = prev[i];
			}
			break;
		/**********************************************************************/
		default:
			std::cout << "createCubeFaces invalid rotation " << static_cast<int>(rot) << std::endl;
		}
		mvQueue.pop();
	}
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
		for move={U , U’, U2, ..., D, D’, D2} do // Loop trough all possible moves
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

void Cube::doMove(rotate rot)
{
	(this->*cubefcn[int(rot)])();
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

// run IDA, IDDFS, Iterative Deepening Depth First Search
void Cube::performIDA()
{
	int depth = 0;
	int bound = 1;
	std::cout << "IDDFS bound = " << bound << std::endl;
	while (!boundDFS(depth, bound)) {
		++bound;
		std::cout << "IDDFSbound = " << bound << std::endl;
	}
}

void handleIDA(int trials, int maxTwists)
{
	// create a cube with the trials and maxTwists
	Cube cube(trials, maxTwists);
	// loop over the trials
	for (int tri = 0; tri < trials; ++tri) {
		// loop over the twists
		for (int tws = 0; tws < maxTwists; ++tws) {
			// Scramble cube starting position and save moves
			cube.scrambleCube(tws);
			// Create the cube faces using saved moves
			// and display them, 6 faces 3x3 in one row
			cube.createCubeFaces(true);
			cube.displayCubeFaces();
			// Perform IDA()
			cube.performIDA();
			// tabulate the results:  solution time and 1/4 turn metric (QTM)
			cube.tabulateTestResults();
			// Create and Display the faces of the solution
			cube.createCubeFaces(false);
			cube.displayCubeFaces();
		}
	}
}

void handleIDAstar(int trials, int maxTwists)
{
	// create a cube
	// create pattern databases if necessary
	// read in the three pattern databases:  corners1-8, edges1-6, edges 7-12
	// create a cube with the maxTwists and trials
	// loop over the trials
	//   loop over the twists
	//     Scramble cube starting position
	//     Create the cube faces and display them, 6 faces 3x3 in one row
	//     Perform IDA* (IDDFS with pruning)
	//     tabulate the results:  solution time and 1/4 turn metric (QTM)
	//     Display the faces of the solution
}

int main(int argc, char *argv[]) {
	std::cout << "!!!Hello World!!!" << std::endl; // prints !!!Hello World!!!
	// Enter the max number of twists
	// Enter the number of trials for a given number of twists
	// Enter IDA or IDA*
	// call handleIDA() or handleIDAstar() and supply the twists and trials
	return 0;
}
