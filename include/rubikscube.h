#ifndef RUBIKSCUBE
#define RUBIKSCUBE

#include "raylib.h"

#define ELEMENTAT(cube, i, j, k) cube->blocks[i * cube->side * cube->side + j * cube->side + k]

typedef struct {
	bool rotating;
	int layer;
	float angle;
	float direction; // -1 or 1
	Vector3 axis;
	bool finished;
	int side; // -1 => from under, 0 => straight on, 1 => from above
} RotationAnimation;

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
	int row;
	int column;
	bool enabled;
	float scale;
	int direction;
} Selection;

typedef struct {
	int side;
	Block** blocks;
	Selection* selection;
} Cube;

Block* createBlock(Vector3 position, int side);
Cube* createCube(int side);
void destroyCube(Cube* cube);

void UpdateSelection(Cube* cube);

typedef enum {
	FRONT,
	FRONT_P,
	UP,
	UP_P,
	LEFT,
	LEFT_P,
	BACK,
	BACK_P,
	DOWN,
	DOWN_P,
	RIGHT,
	RIGHT_P
} Move;

typedef enum {
	CLOCKWISE,
	COUNTERCLOCKWISE,
	DOWNWARDS,
	LEFTWARDS,
	RIGHTWARDS,
	UPWARDS
} Rotation;

void executeMove(Cube* cube, Move move, int layer);

void rotateCube(Cube* cube, Rotation rotation, RotationAnimation* rotationAnimation);

void shuffle(Cube* cube);

#endif
