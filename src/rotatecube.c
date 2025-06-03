#include "rotatecube.h"
#include "rubikscube.h"
#include <stdbool.h>

void startRotation(RotationAnimation* rotationAnimation, Move move) {
	if (rotationAnimation->rotating == false && rotationAnimation->finished == false) {
		rotationAnimation->rotating = true;
		rotationAnimation->angle = 0.0f;
		rotationAnimation->direction = move.direction;
		rotationAnimation->axis = move.axis;
		rotationAnimation->layer = move.layer;
		updateRotation(rotationAnimation);
	}
}

void updateRotation(RotationAnimation *rotationAnimation) {
	if (rotationAnimation->rotating) {
		rotationAnimation->angle += rotationAnimation->direction * 5.0f;
		if (rotationAnimation->angle >= 90.0f || rotationAnimation->angle <= -90.0f) {
			rotationAnimation->rotating = false;
			rotationAnimation->finished = true;
		}
	} else {
		rotationAnimation->delay--;
		if (rotationAnimation->delay < 0) {
			rotationAnimation->delay = 0;
		}
	}
}

void decodeMove(Cube *cube, RotationAnimation *rotationAnimation) {
	Move move;
	move.layer = rotationAnimation->layer;
	move.direction = rotationAnimation->direction;
	move.axis = rotationAnimation->axis;
	executeMove(cube, move);
}
