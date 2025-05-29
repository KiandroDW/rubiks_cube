#include "rubikscube.h"
#include "rotatecube.h"
#include <stdio.h>
#include <stdlib.h>

Block* createBlock(Vector3 position) {
	/*
	 *	Righthanded system: x = forwards, y = upwards, z = leftwards
	 */
	Colors colors = {0};
	if (position.x == 1) {
		colors.front = GREEN;
	} else if (position.x == -1) {
		colors.back = BLUE;
	}
	if (position.y == 1) {
		colors.up = WHITE;
	} else if (position.y == -1) {
		colors.down = YELLOW;
	}
	if (position.z == 1) {
		colors.left = ORANGE;
	} else if (position.z == -1) {
		colors.right = RED;
	}
	Block* block = malloc(sizeof(Block));
	block->colors = colors;
	return block;
}

Cube* createCube() {
	Cube* cube = malloc(sizeof(Cube));
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			for (int k = -1; k <= 1; k ++) {
				Vector3 pos = {i, j, k};
				cube->blocks[i+1][j+1][k+1] = createBlock(pos);
			}
		}
	}
	return cube;
}

void destroyCube(Cube *cube) {
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			for (int k = -1; k <= 1; k ++) {
				free(cube->blocks[i+1][j+1][k+1]);
			}
		}
	}
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
	Block* temp[3][3];

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			temp[j][2-i] = cube->blocks[i][j][plane];
		}
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cube->blocks[i][j][plane] = temp[i][j];
			rotateBlock(cube->blocks[i][j][plane], DOWNWARDS);
		}
	}
}

void rotateXYplaneUp(Cube* cube, int plane) {
	Block* temp[3][3];

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			temp[2-j][i] = cube->blocks[i][j][plane];
		}
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cube->blocks[i][j][plane] = temp[i][j];
			rotateBlock(cube->blocks[i][j][plane], UPWARDS);
		}
	}
}

void rotateXZplaneRight(Cube* cube, int plane) {
	Block* temp[3][3];

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			temp[j][2-i] = cube->blocks[i][plane][j];
		}
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cube->blocks[i][plane][j] = temp[i][j];
			rotateBlock(cube->blocks[i][plane][j], RIGHTWARDS);
		}
	}
}

void rotateXZplaneLeft(Cube* cube, int plane) {
	Block* temp[3][3];

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			temp[2-j][i] = cube->blocks[i][plane][j];
		}
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cube->blocks[i][plane][j] = temp[i][j];
			rotateBlock(cube->blocks[i][plane][j], LEFTWARDS);
		}
	}
}

void rotateYZplaneClockwise(Cube* cube, int plane) {
	Block* temp[3][3];

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			temp[j][2-i] = cube->blocks[plane][i][j];
		}
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cube->blocks[plane][i][j] = temp[i][j];
			rotateBlock(cube->blocks[plane][i][j], CLOCKWISE);
		}
	}
}

void rotateYZplaneCounterClockwise(Cube* cube, int plane) {
	Block* temp[3][3];

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			temp[2-j][i] = cube->blocks[plane][i][j];
		}
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cube->blocks[plane][i][j] = temp[i][j];
			rotateBlock(cube->blocks[plane][i][j], COUNTERCLOCKWISE);
		}
	}
}

void executeMove(Cube* cube, Move move) {
	Move moves[12] = {FRONT, FRONT_P, UP, UP_P, LEFT, LEFT_P, BACK, BACK_P, DOWN, DOWN_P, RIGHT, RIGHT_P};

	int status = 0;
	if (move == moves[0]) {
		rotateYZplaneClockwise(cube, 2);
	} else if (move == moves[1]) {
		rotateYZplaneCounterClockwise(cube, 2);
	} else if (move == moves[2]) {
		rotateXZplaneLeft(cube, 2);
	} else if (move == moves[3]) {
		rotateXZplaneRight(cube, 2);
	} else if (move == moves[4]) {
		rotateXYplaneDown(cube, 2);
	} else if (move == moves[5]) {
		rotateXYplaneUp(cube, 2);
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
			rotateYZplaneClockwise(cube, 0);
			rotateYZplaneClockwise(cube, 1);
			rotateYZplaneClockwise(cube, 2);
			break;
		case COUNTERCLOCKWISE:
			if (anim->axis.z == 1) {
				anim->layer = - anim->layer;
				anim->angle = - anim->angle;
			}
			anim->axis = (Vector3) {anim->axis.x, anim->axis.z, anim->axis.y};
			rotateYZplaneCounterClockwise(cube, 0);
			rotateYZplaneCounterClockwise(cube, 1);
			rotateYZplaneCounterClockwise(cube, 2);
			break;
		case DOWNWARDS:
			if (anim->axis.x == 1) {
				anim->layer = - anim->layer;
				anim->angle = - anim->angle;
			}
			anim->axis = (Vector3) {anim->axis.y, anim->axis.x, anim->axis.z};
			rotateXYplaneDown(cube, 0);
			rotateXYplaneDown(cube, 1);
			rotateXYplaneDown(cube, 2);
			break;
		case UPWARDS:
			if (anim->axis.y == 1) {
				anim->layer = - anim->layer;
				anim->angle = - anim->angle;
			}
			anim->axis = (Vector3) {anim->axis.y, anim->axis.x, anim->axis.z};
			rotateXYplaneUp(cube, 0);
			rotateXYplaneUp(cube, 1);
			rotateXYplaneUp(cube, 2);
			break;
		case LEFTWARDS:
			if (anim->axis.z == 1) {
				anim->layer = - anim->layer;
				anim->angle = - anim->angle;
			}
			anim->axis = (Vector3) {anim->axis.z, anim->axis.y, anim->axis.x};
			rotateXZplaneLeft(cube, 0);
			rotateXZplaneLeft(cube, 1);
			rotateXZplaneLeft(cube, 2);
			break;
		case RIGHTWARDS:
			if (anim->axis.x == 1) {
				anim->layer = - anim->layer;
				anim->angle = - anim->angle;
			}
			anim->axis = (Vector3) {anim->axis.z, anim->axis.y, anim->axis.x};
			rotateXZplaneRight(cube, 0);
			rotateXZplaneRight(cube, 1);
			rotateXZplaneRight(cube, 2);
			break;
		default:
			printf("You broke it :(\n");
			break;

	}
};
