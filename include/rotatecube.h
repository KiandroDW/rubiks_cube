#ifndef ROTATECUBE
#define ROTATECUBE
#include "rubikscube.h"

void StartRotation(RotationAnimation* rotationAnimation, Move move, int side);

void UpdateRotation(RotationAnimation* rotationAnimation);

void DecodeMove(Cube* cube, RotationAnimation* rotationAnimation);

#endif
