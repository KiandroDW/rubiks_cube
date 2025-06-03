#include "rubikscube.h"
#include "raylib.h"
#include "rotatecube.h"
#include "rubikscubeparts.h"
#include <stdio.h>
#include <stdlib.h>

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
		while ((chosen_axis == previous_axis && chosen_layer == previous_layer) || (cube->side % 2 == 1 && chosen_layer == (cube->side-1) / 2)) {
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

void updateCursor(Cube* cube) {
	Cursor* cursor = cube->cursor;
	cursor->scale += cursor->direction * 0.01f;
	if (cursor->scale <= 0.7f || cursor->scale >= 0.9f) {
		cursor->direction *= -1;
	}
}

void rightMove(Cube* cube, MoveQueue* queue, bool* clockwise) {
	Move move;
	move.axis = (Vector3) {0, 0, 1};
	if (cube->cursor->enabled) {
		move.layer = cube->cursor->column;
	} else {
		move.layer = 0;
	}
	if (*clockwise == false) {
		move.direction = 1;
		addElement(move, queue);
	} else {
		move.direction = -1;
		addElement(move, queue);
	}
}

void leftMove(Cube* cube, MoveQueue* queue, bool* clockwise) {
	Move move;
	move.axis = (Vector3) {0, 0, 1};
	if (cube->cursor->enabled) {
		move.layer = cube->cursor->column;
	} else {
		move.layer = cube->side - 1;
	}
	if (*clockwise == false) {
		move.direction = -1;
		addElement(move, queue);
	} else {
		move.direction = 1;
		addElement(move, queue);
	}
}

void downMove(Cube* cube, MoveQueue* queue, bool* clockwise) {
	Move move;
	move.axis = (Vector3) {0, 1, 0};
	if (cube->cursor->enabled) {
		move.layer = cube->cursor->column;
	} else {
		move.layer = 0;
	}
	if (*clockwise == false) {
		move.direction = 1;
		addElement(move, queue);
	} else {
		move.direction = -1;
		addElement(move, queue);
	}
}

void upMove(Cube* cube, MoveQueue* queue, bool* clockwise) {
	Move move;
	move.axis = (Vector3) {0, 1, 0};
	if (cube->cursor->enabled) {
		move.layer = cube->cursor->column;
	} else {
		move.layer = cube->side - 1;
	}
	if (*clockwise == false) {
		move.direction = -1;
		addElement(move, queue);
	} else {
		move.direction = 1;
		addElement(move, queue);
	}
}

void backMove(Cube* cube, MoveQueue* queue, bool* clockwise) {
	Move move;
	move.axis = (Vector3) {1, 0, 0};
	move.layer = 0;
	if (*clockwise == false) {
		move.direction = 1;
		addElement(move, queue);
	} else {
		move.direction = -1;
		addElement(move, queue);
	}
}

void frontMove(Cube* cube, MoveQueue* queue, bool* clockwise) {
	Move move;
	move.axis = (Vector3) {1, 0, 0};
	move.layer = cube->side - 1;
	if (*clockwise == false) {
		move.direction = -1;
		addElement(move, queue);
	} else {
		move.direction = 1;
		addElement(move, queue);
	}
}

void rotateCubeX(Cube* cube, RotationAnimation* anim, bool* clockwise) {
	if (!*clockwise) {
		rotateCube(cube, COUNTERCLOCKWISE, anim);
	} else {
		rotateCube(cube, CLOCKWISE, anim);
	}
}

void rotateCubeY(Cube* cube, RotationAnimation* anim, bool* clockwise) {
	if (!*clockwise) {
		rotateCube(cube, RIGHTWARDS, anim);
	} else {
		rotateCube(cube, LEFTWARDS, anim);
	}
}

void rotateCubeZ(Cube* cube, RotationAnimation* anim, bool* clockwise) {
	if (!*clockwise) {
		rotateCube(cube, UPWARDS, anim);
	} else {
		rotateCube(cube, DOWNWARDS, anim);
	}
}

void moveCursor(Cube* cube, int amountX, int amountY) {
	if (cube->cursor->enabled) {
		cube->cursor->column += amountX;
		cube->cursor->row += amountY;
		if (cube->cursor->column > cube->side - 1) {
			cube->cursor->column = 0;
		} else if (cube->cursor->column < 0) {
			cube->cursor->column = cube->side - 1;
		}
		if (cube->cursor->row > cube->side - 1) {
			cube->cursor->row = 0;
		} else if (cube->cursor->row < 0) {
			cube->cursor->row = cube->side - 1;
		}
	}
}
