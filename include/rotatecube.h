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
	int delay;
} RotationAnimation;

void startRotation(RotationAnimation* rotationAnimation, Move move);

void updateRotation(RotationAnimation* rotationAnimation);

void decodeMove(Cube* cube, RotationAnimation* rotationAnimation);

#endif
