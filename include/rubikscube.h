#ifndef RUBIKSCUBE
#define RUBIKSCUBE

#include "raylib.h"

typedef struct {
	Color back;
	Color down;
	Color front;
	Color left;
	Color right;
	Color up;
} Colors;

typedef struct {
	Colors colors;
} Block;

typedef struct {
	Block* blocks[3][3][3];
} Cube;

Block* createBlock(Vector3 position);
Cube* createCube();
void destroyCube(Cube* cube);

typedef enum {
	BACK,
	DOWN,
	FRONT,
	LEFT,
	RIGHT,
	UP,
	BACK_P,
	DOWN_P,
	FRONT_P,
	LEFT_P,
	RIGHT_P,
	UP_P
} Move;

typedef enum {
	CLOCKWISE,
	COUNTERCLOCKWISE,
	DOWNWARDS,
	LEFTWARDS,
	RIGHTWARDS,
	UPWARDS
} Rotation;

void executeMove(Cube* cube, Move move);

#endif
