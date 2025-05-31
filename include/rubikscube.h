#ifndef RUBIKSCUBE
#define RUBIKSCUBE

#include "queue.h"
#include "raylib.h"
#include "rotatecube.h"
#include "rubikscubeparts.h"

#define ELEMENTAT(cube, i, j, k) cube->blocks[i * cube->side * cube->side + j * cube->side + k]

Block* createBlock(Vector3 position, int side);
Cube* createCube(int side);
void destroyCube(Cube* cube);

void UpdateSelection(Cube* cube);

void executeMove(Cube* cube, Move move);

void rotateCube(Cube* cube, Rotation rotation, RotationAnimation* rotationAnimation);

void shuffle(Cube* cube, MoveQueue* queue);

#endif
