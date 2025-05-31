#include "rubikscube.h"
#include "raylib.h"
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
	cube->selection = malloc(sizeof(Selection));
	cube->selection->row = side - 1;
	cube->selection->column = side - 1;
	cube->selection->enabled = false;
	cube->selection->scale = 0.9f;
	cube->selection->direction = -1;
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
	free(cube->selection);
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
			printf("Somehow you had a not valid case\n");
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
	if (move.axis.x == 1) {
		if (move.direction == 1) {
			rotateYZplaneClockwise(cube, move.layer);
		} else {
			rotateYZplaneCounterClockwise(cube, move.layer);
		}
	} else if (move.axis.y == 1) {
		if (move.direction == 1) {
			rotateXZplaneLeft(cube, move.layer);
		} else {
			rotateXZplaneRight(cube, move.layer);
		}
	} else {
		if (move.direction == 1) {
			rotateXYplaneDown(cube, move.layer);
		} else {
			rotateXYplaneUp(cube, move.layer);
		}
	}
}

void shuffle(Cube* cube, MoveQueue* queue) {
	int previous_axis = -1;
	int previous_layer = -1;
	int chosen_axis;
	int chosen_layer;
	int chosen_direction;
	Vector3 chosen_vector;

	for (int i = 0; i < cube->side * 10; i++) {
		chosen_axis = rand() % 3;
		chosen_layer = rand() % cube->side;
		while (chosen_axis == previous_axis && chosen_layer == previous_layer) {
			chosen_axis = rand() % 3;
			chosen_layer = rand() % cube->side;
		}
		if (chosen_axis == 0)
			chosen_vector = (Vector3) {1, 0, 0};
		if (chosen_axis == 1)
			chosen_vector = (Vector3) {0, 1, 0};
		if (chosen_axis == 2)
			chosen_vector = (Vector3) {0, 0, 1};

		chosen_direction = rand() % 2;
		if (chosen_direction == 0) {
			chosen_direction = 1;
		}

		Move selected_move = (Move) {chosen_vector, chosen_layer, chosen_direction};
		addElement(selected_move, queue);
	}
}

void rotateCube(Cube* cube, Rotation rotation, RotationAnimation* anim) {
	switch (rotation) {
		case CLOCKWISE:
			if (anim->axis.y == 1) {
				anim->layer = cube->side - anim->layer - 1;
				anim->angle = - anim->angle;
				anim->direction = - anim->direction;
			}
			anim->axis = (Vector3) {anim->axis.x, anim->axis.z, anim->axis.y};
			for (int i = 0; i < cube->side; i++){
				rotateYZplaneClockwise(cube, i);
			}
			break;
		case COUNTERCLOCKWISE:
			if (anim->axis.z == 1) {
				anim->layer = cube->side - anim->layer - 1;
				anim->angle = - anim->angle;
				anim->direction = - anim->direction;
			}
			anim->axis = (Vector3) {anim->axis.x, anim->axis.z, anim->axis.y};
			for (int i = 0; i < cube->side; i++){
				rotateYZplaneCounterClockwise(cube, i);
			}
			break;
		case DOWNWARDS:
			if (anim->axis.x == 1) {
				anim->layer = cube->side - anim->layer - 1;
				anim->angle = - anim->angle;
				anim->direction = - anim->direction;
			}
			anim->axis = (Vector3) {anim->axis.y, anim->axis.x, anim->axis.z};
			for (int i = 0; i < cube->side; i++){
				rotateXYplaneDown(cube, i);
			}
			break;
		case UPWARDS:
			if (anim->axis.y == 1) {
				anim->layer = cube->side - anim->layer - 1;
				anim->angle = - anim->angle;
				anim->direction = - anim->direction;
			}
			anim->axis = (Vector3) {anim->axis.y, anim->axis.x, anim->axis.z};
			for (int i = 0; i < cube->side; i++){
				rotateXYplaneUp(cube, i);
			}
			break;
		case LEFTWARDS:
			if (anim->axis.z == 1) {
				anim->layer = cube->side - anim->layer - 1;
				anim->angle = - anim->angle;
				anim->direction = - anim->direction;
			}
			anim->axis = (Vector3) {anim->axis.z, anim->axis.y, anim->axis.x};
			for (int i = 0; i < cube->side; i++){
				rotateXZplaneLeft(cube, i);
			}
			break;
		case RIGHTWARDS:
			if (anim->axis.x == 1) {
				anim->layer = cube->side - anim->layer - 1;
				anim->angle = - anim->angle;
				anim->direction = - anim->direction;
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

void UpdateSelection(Cube* cube) {
	Selection* selection = cube->selection;
	selection->scale += selection->direction * 0.01f;
	if (selection->scale <= 0.7f || selection->scale >= 0.9f) {
		selection->direction *= -1;
	}
}
