#include "rubikscube.h"
#include <stdio.h>
#include <stdlib.h>

Block* createBlock(Vector3 position, int side) {
	/*
	 *	Righthanded system: x = forwards, y = upwards, z = leftwards
	 */
	Colors colors = {0};
	if (position.x == side - 1) {
		colors.front = GREEN;
	}
	if (position.x == 0) {
		colors.back = BLUE;
	}
	if (position.y == side - 1) {
		colors.up = WHITE;
	}
	if (position.y == 0) {
		colors.down = YELLOW;
	}
	if (position.z == side - 1) {
		colors.left = ORANGE;
	}
	if (position.z == 0) {
		colors.right = RED;
	}
	Block* block = malloc(sizeof(Block));
	block->colors = colors;
	return block;
}

Cube* createCube(int side) {
	Cube* cube = malloc(sizeof(Cube));
	cube->side = side;
	cube->blocks = malloc(side * side * side * sizeof(Block*));
	for (int i = 0; i < side; i++) {
		for (int j = 0; j < side; j++) {
			for (int k = 0; k < side; k ++) {
				Vector3 pos = {i, j, k};
				ELEMENTAT(cube, i, j, k) = createBlock(pos, side);
			}
		}
	}
	return cube;
}

void destroyCube(Cube *cube) {
	for (int i = 0; i < cube->side; i++) {
		for (int j = 0; j < cube->side; j++) {
			for (int k = 0; k < cube->side; k ++) {
				free(ELEMENTAT(cube, i, j, k));
			}
		}
	}
	free(cube->blocks);
	free(cube);
}

int rotateBlock(Block* block,  Rotation rotation) {
	Color temp;
	int status = 0;
	switch (rotation) {
		case CLOCKWISE:
			temp = block->colors.up;
			block->colors.up = block->colors.left;
			block->colors.left = block->colors.down;
			block->colors.down = block->colors.right;
			block->colors.right = temp;
			break;
		case COUNTERCLOCKWISE:
			temp = block->colors.up;
			block->colors.up = block->colors.right;
			block->colors.right = block->colors.down;
			block->colors.down = block->colors.left;
			block->colors.left = temp;
			break;
		case DOWNWARDS:
			temp = block->colors.up;
			block->colors.up = block->colors.back;
			block->colors.back = block->colors.down;
			block->colors.down = block->colors.front;
			block->colors.front = temp;
			break;
		case LEFTWARDS:
			temp = block->colors.back;
			block->colors.back = block->colors.left;
			block->colors.left = block->colors.front;
			block->colors.front = block->colors.right;
			block->colors.right = temp;
			break;
		case RIGHTWARDS:
			temp = block->colors.front;
			block->colors.front = block->colors.left;
			block->colors.left = block->colors.back;
			block->colors.back = block->colors.right;
			block->colors.right = temp;
			break;
		case UPWARDS:
			temp = block->colors.up;
			block->colors.up = block->colors.front;
			block->colors.front = block->colors.down;
			block->colors.down = block->colors.back;
			block->colors.back = temp;
			break;
		default:
			printf("Somehow you had a not valid case");
			status = 1;
			break;
	}
	return status;
}

void rotateXYplaneDown(Cube* cube, int plane) {
	Block* temp[cube->side][cube->side];

	for (int i = 0; i < cube->side; i++) {
		for (int j = 0; j < cube->side; j++) {
			temp[j][cube->side-i-1] = ELEMENTAT(cube, i, j, plane);
		}
	}

	for (int i = 0; i < cube->side; i++) {
		for (int j = 0; j < cube->side; j++) {
			ELEMENTAT(cube, i, j, plane) = temp[i][j];
			rotateBlock(ELEMENTAT(cube, i, j, plane), DOWNWARDS);
		}
	}
}

void rotateXYplaneUp(Cube* cube, int plane) {
	Block* temp[cube->side][cube->side];

	for (int i = 0; i < cube->side; i++) {
		for (int j = 0; j < cube->side; j++) {
			temp[cube->side-j-1][i] = ELEMENTAT(cube, i, j, plane);
		}
	}

	for (int i = 0; i < cube->side; i++) {
		for (int j = 0; j < cube->side; j++) {
			ELEMENTAT(cube, i, j, plane) = temp[i][j];
			rotateBlock(ELEMENTAT(cube, i, j, plane), UPWARDS);
		}
	}
}

void rotateXZplaneRight(Cube* cube, int plane) {
	Block* temp[cube->side][cube->side];

	for (int i = 0; i < cube->side; i++) {
		for (int j = 0; j < cube->side; j++) {
			temp[j][cube->side-i-1] = ELEMENTAT(cube, i, plane, j);
		}
	}

	for (int i = 0; i < cube->side; i++) {
		for (int j = 0; j < cube->side; j++) {
			ELEMENTAT(cube, i, plane, j) = temp[i][j];
			rotateBlock(ELEMENTAT(cube, i, plane, j), RIGHTWARDS);
		}
	}
}

void rotateXZplaneLeft(Cube* cube, int plane) {
	Block* temp[cube->side][cube->side];

	for (int i = 0; i < cube->side; i++) {
		for (int j = 0; j < cube->side; j++) {
			temp[cube->side-j-1][i] = ELEMENTAT(cube, i, plane, j);
		}
	}

	for (int i = 0; i < cube->side; i++) {
		for (int j = 0; j < cube->side; j++) {
			ELEMENTAT(cube, i, plane, j) = temp[i][j];
			rotateBlock(ELEMENTAT(cube, i, plane, j), LEFTWARDS);
		}
	}
}

void rotateYZplaneClockwise(Cube* cube, int plane) {
	Block* temp[cube->side][cube->side];

	for (int i = 0; i < cube->side; i++) {
		for (int j = 0; j < cube->side; j++) {
			temp[j][cube->side-i-1] = ELEMENTAT(cube, plane, i, j);
		}
	}

	for (int i = 0; i < cube->side; i++) {
		for (int j = 0; j < cube->side; j++) {
			ELEMENTAT(cube, plane, i, j) = temp[i][j];
			rotateBlock(ELEMENTAT(cube, plane, i, j), CLOCKWISE);
		}
	}
}

void rotateYZplaneCounterClockwise(Cube* cube, int plane) {
	Block* temp[cube->side][cube->side];

	for (int i = 0; i < cube->side; i++) {
		for (int j = 0; j < cube->side; j++) {
			temp[cube->side-j-1][i] = ELEMENTAT(cube, plane, i, j);
		}
	}

	for (int i = 0; i < cube->side; i++) {
		for (int j = 0; j < cube->side; j++) {
			ELEMENTAT(cube, plane, i, j) = temp[i][j];
			rotateBlock(ELEMENTAT(cube, plane, i, j), COUNTERCLOCKWISE);
		}
	}
}

void executeMove(Cube* cube, Move move) {
	Move moves[12] = {FRONT, FRONT_P, UP, UP_P, LEFT, LEFT_P, BACK, BACK_P, DOWN, DOWN_P, RIGHT, RIGHT_P};

	int status = 0;
	if (move == moves[0]) {
		rotateYZplaneClockwise(cube, cube->side-1);
	} else if (move == moves[1]) {
		rotateYZplaneCounterClockwise(cube, cube->side-1);
	} else if (move == moves[2]) {
		rotateXZplaneLeft(cube, cube->side-1);
	} else if (move == moves[3]) {
		rotateXZplaneRight(cube, cube->side-1);
	} else if (move == moves[4]) {
		rotateXYplaneDown(cube, cube->side-1);
	} else if (move == moves[5]) {
		rotateXYplaneUp(cube, cube->side-1);
	} else if (move == moves[6]) {
		rotateYZplaneCounterClockwise(cube, 0);
	} else if (move == moves[7]) {
		rotateYZplaneClockwise(cube, 0);
	} else if (move == moves[8]) {
		rotateXZplaneRight(cube, 0);
	} else if (move == moves[9]) {
		rotateXZplaneLeft(cube, 0);
	} else if (move == moves[10]) {
		rotateXYplaneUp(cube, 0);
	} else if (move == moves[11]) {
		rotateXYplaneDown(cube, 0);
	} else {
		printf("You managed to break it");
		status = 2;
	}
}

void shuffle(Cube* cube) {
	int previous_previous = -1;
	int previous_set = -1; // 0 = F, 1 = U, 2 = L, 3 = B, 4 = D, 5 = R
	int chosen_set;
	int chosen_form;

	for (int i = 0; i < 25; i++) {
		chosen_set = rand() % 6;
		while (chosen_set == previous_set || chosen_set % 3 ==  previous_previous) {
			chosen_set = rand() % 6;
		}
		chosen_form = rand() % 3;
		if (chosen_form == 2) {
			executeMove(cube, chosen_set * 2); // double move
			executeMove(cube, chosen_set * 2);
		} else {
			executeMove(cube, chosen_set * 2 + chosen_form);
		}

		previous_previous = previous_set;
	}
	printf("\n");
}

void rotateCube(Cube* cube, Rotation rotation, RotationAnimation* anim) {
	switch (rotation) {
		case CLOCKWISE:
			if (anim->axis.y == 1) {
				anim->layer = - anim->layer;
				anim->angle = - anim->angle;
			}
			anim->axis = (Vector3) {anim->axis.x, anim->axis.z, anim->axis.y};
			for (int i = 0; i < cube->side; i++){
				rotateYZplaneClockwise(cube, i);
			}
			break;
		case COUNTERCLOCKWISE:
			if (anim->axis.z == 1) {
				anim->layer = - anim->layer;
				anim->angle = - anim->angle;
			}
			anim->axis = (Vector3) {anim->axis.x, anim->axis.z, anim->axis.y};
			for (int i = 0; i < cube->side; i++){
				rotateYZplaneCounterClockwise(cube, i);
			}
			break;
		case DOWNWARDS:
			if (anim->axis.x == 1) {
				anim->layer = - anim->layer;
				anim->angle = - anim->angle;
			}
			anim->axis = (Vector3) {anim->axis.y, anim->axis.x, anim->axis.z};
			for (int i = 0; i < cube->side; i++){
				rotateXYplaneDown(cube, i);
			}
			break;
		case UPWARDS:
			if (anim->axis.y == 1) {
				anim->layer = - anim->layer;
				anim->angle = - anim->angle;
			}
			anim->axis = (Vector3) {anim->axis.y, anim->axis.x, anim->axis.z};
			for (int i = 0; i < cube->side; i++){
				rotateXYplaneUp(cube, i);
			}
			break;
		case LEFTWARDS:
			if (anim->axis.z == 1) {
				anim->layer = - anim->layer;
				anim->angle = - anim->angle;
			}
			anim->axis = (Vector3) {anim->axis.z, anim->axis.y, anim->axis.x};
			for (int i = 0; i < cube->side; i++){
				rotateXZplaneLeft(cube, i);
			}
			break;
		case RIGHTWARDS:
			if (anim->axis.x == 1) {
				anim->layer = - anim->layer;
				anim->angle = - anim->angle;
			}
			anim->axis = (Vector3) {anim->axis.z, anim->axis.y, anim->axis.x};
			for (int i = 0; i < cube->side; i++){
				rotateXZplaneRight(cube, i);
			}
			break;
		default:
			printf("You broke it :(\n");
			break;

	}
};
