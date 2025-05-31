#include "rotatecube.h"
#include "rubikscube.h"
#include <stdbool.h>

void StartRotation(RotationAnimation* rotationAnimation, Move move) {
	if (rotationAnimation->rotating == false && rotationAnimation->finished == false) {
		rotationAnimation->rotating = true;
		rotationAnimation->angle = 0.0f;
		rotationAnimation->direction = move.direction;
		rotationAnimation->axis = move.axis;
		rotationAnimation->layer = move.layer;
		UpdateRotation(rotationAnimation);
	}
}

void UpdateRotation(RotationAnimation *rotationAnimation) {
	if (rotationAnimation->rotating) {
		rotationAnimation->angle += rotationAnimation->direction * 5.0f;
		if (rotationAnimation->angle >= 90.0f || rotationAnimation->angle <= -90.0f) {
			rotationAnimation->rotating = false;
			rotationAnimation->finished = true;
		}
	}
}

void DecodeMove(Cube *cube, RotationAnimation *rotationAnimation) {
	Move move;
	move.layer = rotationAnimation->layer;
	move.direction = rotationAnimation->direction;
	move.axis = rotationAnimation->axis;
	executeMove(cube, move);
}
