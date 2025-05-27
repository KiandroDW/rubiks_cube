#include "rubikscube.h"
#include <stdlib.h>

Block* createBlock(Vector3 position) {
	/*
	 *	Righthanded system: x = forwards, y = upwards, z = leftwards
	 */
	Colors colors;
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
	block->position = position;
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
