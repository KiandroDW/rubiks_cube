#ifndef ROTATECUBE
#define ROTATECUBE

#include "rubikscubeparts.h"
typedef struct {
	bool rotating;
	int layer;
	float angle;
	float direction; // -1 or 1
	Vector3 axis;
	bool finished;
	int side; // -1 => from under, 0 => straight on, 1 => from above
} RotationAnimation;

void StartRotation(RotationAnimation* rotationAnimation, Move move);

void UpdateRotation(RotationAnimation* rotationAnimation);

void DecodeMove(Cube* cube, RotationAnimation* rotationAnimation);

#endif
