#include "rubikscubeparts.h"
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
	if (position.z == side-1) {
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
	cube->cursor = malloc(sizeof(Cursor));
	cube->cursor->row = side - 1;
	cube->cursor->column = side - 1;
	cube->cursor->enabled = false;
	cube->cursor->scale = 0.9f;
	cube->cursor->direction = -1;
	return cube;
}

void resetCube(Cube* cube, int side) {
	// Clear existing data
	for (int i = 0; i < cube->side; i++) {
		for (int j = 0; j < cube->side; j++) {
			for (int k = 0; k < cube->side; k ++) {
				free(ELEMENTAT(cube, i, j, k));
			}
		}
	}
	free(cube->blocks);
	free(cube->cursor);

	// Insert new data
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
	cube->cursor = malloc(sizeof(Cursor));
	cube->cursor->row = side - 1;
	cube->cursor->column = side - 1;
	cube->cursor->enabled = false;
	cube->cursor->scale = 0.9f;
	cube->cursor->direction = -1;
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
	free(cube->cursor);
	free(cube);
}

Block* copyBlock(Block* block) {
	Block* newBlock = malloc(sizeof(Block));
	newBlock->colors = block->colors;
	return newBlock;
}

Cube* copyCube(Cube* cube) {
	Cube* cube2 = malloc(sizeof(Cube));
	cube2->side = cube->side;
	cube2->blocks = malloc(cube->side * cube->side * cube->side * sizeof(Block*));
	for (int i = 0; i < cube->side; i++) {
		for (int j = 0; j < cube->side; j++) {
			for (int k = 0; k < cube->side; k ++) {
				Vector3 pos = {i, j, k};
				ELEMENTAT(cube2, i, j, k) = copyBlock(ELEMENTAT(cube, i, j, k));
			}
		}
	}
	cube2->cursor = NULL;
	return cube2;
}
