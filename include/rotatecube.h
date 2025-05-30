#ifndef ROTATECUBE
#define ROTATECUBE
#include "rubikscube.h"

void StartRotation(RotationAnimation* rotationAnimation, Cube* cube, Move move);

void UpdateRotation(RotationAnimation* rotationAnimation);

void DecodeMove(Cube* cube, RotationAnimation* rotationAnimation);

#endif
