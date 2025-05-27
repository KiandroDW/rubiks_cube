#include "rubikscube.h"
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
	int status = 0;
	switch (move) {
		case RIGHT_P:
			rotateXYplaneDown(cube, 0);
			break;
		case LEFT:
			rotateXYplaneDown(cube, 2);
			break;
		case LEFT_P:
			rotateXYplaneUp(cube, 2);
			break;
		case RIGHT:
			rotateXYplaneUp(cube, 0);
			break;
		case UP:
			rotateXZplaneLeft(cube, 2);
			break;
		case DOWN_P:
			rotateXZplaneLeft(cube, 0);
			break;
		case DOWN:
			rotateXZplaneRight(cube, 0);
			break;
		case UP_P:
			rotateXZplaneRight(cube, 2);
			break;
		case FRONT:
			rotateYZplaneClockwise(cube, 2);
			break;
		case BACK_P:
			rotateYZplaneClockwise(cube, 0);
			break;
		case BACK:
			rotateYZplaneCounterClockwise(cube, 0);
			break;
		case FRONT_P:
			rotateYZplaneCounterClockwise(cube, 2);
			break;
		default:
			printf("You managed to break it");
			status = 2;
			break;
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
