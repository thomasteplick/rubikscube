/*
 * rcmain.h
 *
 *  Created on: Aug 19, 2026
 *  Author: Tom Teplick
 */

#ifndef RCMAIN_H_
#define RCMAIN_H_

#include <stack>
#include <vector>
#include <queue>

/*
 * Home start position of the cube, Face=Color
 * Up=White, Down=Yellow, Left=Green, Right=Blue, Front=Red, Back=Orange
 */
// dimension of the Rubik's cube, number of cubies in each dimension (x,y,z) Cartesian coordinates
const int DIM = 3;
// number of moves for the Rubik's cube, rotating the faces CW or CCW
const int MOVES = 18;

// number of faces in the cube
const int NFACES = 6;

enum class color :char {WHITE, YELLOW, RED, ORANGE, GREEN, BLUE};
// corner has 3 facelets, edge has 2 facelets, center has 1 facelet
enum class position :char {CORNER, EDGE, CENTER};
// Faces: Up, Down, Left, Right, Front, Back
enum class face : char {L, B, D, F, R, U};

// Rotations of the faces, or twists (90 deg, 180 deg): CW, CCW, CWCW
enum class rotate {U, U_, U2, D, D_, D2, L, L_, L2, R, R_, R2, F, F_, F2, B, B_, B2};

// a Cubie can be a corner, edge, or center position, cube = 3x3x3 cubies
struct Cubie {
	// 0-26, using x,y,z coordinates for home position cube
	char num;
	position pos;
	color clr[DIM];
};


// 3x3x3 cubies, 8-bit uint element = [0, 26], use for IDA and IDA*
class Cube {
private:
	uint8_t cube[DIM][DIM][DIM];
	color facelets[NFACES][DIM][DIM];
	int ntrials;
	int maxTwists;
	// moves done while performing DFS
	std::stack<rotate> rotStack;
	// scramble moves and save for display
	std::queue<rotate> mvQueue;

	bool isSolved();
	bool boundDFS(int depth, int bound);
	void doMove(rotate move);

	// cube face moves, twists CW 90deg, CCW 90deg, 180deg
	// Up face
	void twistU();
	void twistU_();
	void twistU2();

	// Down face
	void twistD();
	void twistD_();
	void twistD2();

	// Left face
	void twistL();
	void twistL_();
	void twistL2();

	// Right face
	void twistR();
	void twistR_();
	void twistR2();

	// Front face
	void twistF();
	void twistF_();
	void twistF2();

	// Back face
	void twistB();
	void twistB_();
	void twistB2();

	// alias for a Cube member void function with no argument
	using CubeFcn = void(Cube::*)();
	std::vector<CubeFcn> cubefcn = {
		&Cube::twistU,
		&Cube::twistU_,
		&Cube::twistU2,
		&Cube::twistD,
		&Cube::twistD_,
		&Cube::twistD2,
		&Cube::twistL,
		&Cube::twistL_,
		&Cube::twistL2,
		&Cube::twistR,
		&Cube::twistR_,
		&Cube::twistR2,
		&Cube::twistF,
		&Cube::twistF_,
		&Cube::twistF2,
		&Cube::twistB,
		&Cube::twistB_,
		&Cube::twistB2,
	};

	// reverse rotate a face, the order must be the reverse rotation of enum rotate
	std::vector<rotate> revRotate{rotate::U_, rotate::U, rotate::U2, rotate::D_, rotate::D, rotate::D2,
								  rotate::L_, rotate::L, rotate::L2, rotate::R_,rotate::R, rotate::R2,
								  rotate::F_, rotate::F, rotate::F2, rotate::B_, rotate::B, rotate::B2};

public:
	Cube(int ntrials=1, int maxTwists=1);
	~Cube(){}
	Cube(const Cube &) = delete;
	Cube & operator=(const Cube &) = delete;
	Cube(const Cube &&) = delete;
	Cube & operator=(const Cube &&) = delete;

	void createPatternDB();
	void displayCubeFaces();
	void scrambleCube(int twists);
	void createCubeFaces(bool init);
	void performIDA();
	void performIDAstar();
	void tabulateTestResults();

};

void handleIDA(int trials, int maxTwists);

void handleIDAstar(int trials, int maxTwists);

#endif /* RCMAIN_H_ */
