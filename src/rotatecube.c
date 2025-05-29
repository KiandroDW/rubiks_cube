#include "rotatecube.h"
#include "rubikscube.h"
#include <stdbool.h>

void StartRotation(RotationAnimation* rotationAnimation, Move move, int side) {
	if (rotationAnimation->rotating == false && rotationAnimation->finished == false) {
		rotationAnimation->rotating = true;
		rotationAnimation->angle = 0.0f;
		if (move % 2 == 0) {
			rotationAnimation->direction = -1;
		} else {
			rotationAnimation->direction = 1;
		}
		switch (move / 2) {
			case 0: // F
				rotationAnimation->axis = (Vector3) {1, 0, 0};
				rotationAnimation->layer = side - 1;
				break;
			case 1: // U
				rotationAnimation->axis = (Vector3) {0, 1, 0};
				rotationAnimation->layer = side - 1;
				break;
			case 2: // L
				rotationAnimation->axis = (Vector3) {0, 0, 1};
				rotationAnimation->layer = side - 1;
				break;
			case 3: // B
				rotationAnimation->axis = (Vector3) {1, 0, 0};
				rotationAnimation->layer = 0;
				break;
			case 4: // D
				rotationAnimation->axis = (Vector3) {0, 1, 0};
				rotationAnimation->layer = 0;
				break;
			case 5: // R
				rotationAnimation->axis = (Vector3) {0, 0, 1};
				rotationAnimation->layer = 0;
				break;
		}
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
	int prime = 0;
	if (rotationAnimation->direction == 1) {
		prime = 1;
	}
	Move move;
	if (rotationAnimation->axis.x == 1) {
		if (rotationAnimation->layer == 0) {
			move = BACK;
		} else {
			move = FRONT;
		}
	} else if (rotationAnimation->axis.y == 1) {
		if (rotationAnimation->layer == 0) {
			move = DOWN;
		} else {
			move = UP;
		}
	} else {
		if (rotationAnimation->layer == 0) {
			move = RIGHT;
		} else {
			move = LEFT;
		}
	}
	executeMove(cube, move + prime);
}
