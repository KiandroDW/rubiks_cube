#ifndef RUBIKSCUBE
#define RUBIKSCUBE

#include "queue.h"
#include "rotatecube.h"
#include "rubikscubeparts.h"


void executeMove(Cube* cube, Move move);

void rotateCube(Cube* cube, Rotation rotation, RotationAnimation* rotationAnimation);

void shuffle(Cube* cube, MoveQueue* queue);

void rightMove(Cube* cube, MoveQueue* queue, bool* clockwise);
void leftMove(Cube* cube, MoveQueue* queue, bool* clockwise);
void upMove(Cube* cube, MoveQueue* queue, bool* clockwise);
void downMove(Cube* cube, MoveQueue* queue, bool* clockwise);
void frontMove(Cube* cube, MoveQueue* queue, bool* clockwise);
void backMove(Cube* cube, MoveQueue* queue, bool* clockwise);
void rotateCubeX(Cube* cube, RotationAnimation* anim, bool* clockwise);
void rotateCubeY(Cube* cube, RotationAnimation* anim, bool* clockwise);
void rotateCubeZ(Cube* cube, RotationAnimation* anim, bool* clockwise);
void moveCursor(Cube* cube, int amountX, int amountY);

#endif
