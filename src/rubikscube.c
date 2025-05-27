#include "rubikscube.h"
#include <stdlib.h>

Block* createBlock(Vector3 position) {
	/*
	 *	Righthanded system: x = forwards, y = upwards, z = leftwards
	 */
	Colors colors;
	if (position.x == 1) {
		colors.right = GREEN;
	} else if (position.x == -1) {
		colors.left = BLUE;
	}
	if (position.y == 1) {
		colors.back = WHITE;
	} else if (position.y == -1) {
		colors.front = YELLOW;
	}
	if (position.z == 1) {
		colors.right = ORANGE;
	} else if (position.z == -1) {
		colors.right = RED;
	}
	Block* block = malloc(sizeof(Block));
	block->colors = colors;
	block->position = position;
	return block;
}

Cube* createCube() {
	Cube* cube = malloc(sizeof(Cube));
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			for (int k = -1; k <= 1; k ++) {
				Vector3 pos = {i, j, k};
				cube->blocks[i][j][k] = createBlock(pos);
			}
		}
	}
	return cube;
}

void destroyCube(Cube *cube) {
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			for (int k = -1; k <= 1; k ++) {
				free(cube->blocks[i][j][k]);
			}
		}
	}
	free(cube);
}
