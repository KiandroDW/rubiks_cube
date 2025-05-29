#ifndef RUBIKSCUBE
#define RUBIKSCUBE

#include "raylib.h"

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
	Block* blocks[3][3][3];
} Cube;

Block* createBlock(Vector3 position);
Cube* createCube();
void destroyCube(Cube* cube);

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

void executeMove(Cube* cube, Move move);

void rotateCube(Cube* cube, Rotation rotation, RotationAnimation* rotationAnimation);

void shuffle(Cube* cube);

#endif
