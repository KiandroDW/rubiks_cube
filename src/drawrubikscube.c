#include "drawrubikscube.h"
#include <stdio.h>

#define COLISNULL(color) \
	(color.r == 0 && color.g == 0 && color.b == 0 && color.a == 0)

void DrawRubiksCube(Cube* cube) {
	/* DrawCube((Vector3){0, 0, 0}, 3.0f, 3.0f, 3.0f, BLACK); // body */

	const float planeHeight = 0.1f;
	const float planeLength = 0.9f;

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			for (int k = -1; k <= 1; k++) {
				Block* block = cube->blocks[i+1][j+1][k+1];
				Colors colors = block->colors;
				Vector3 center = block->position;
				Vector3 planeCenter = center;

				planeCenter.x -= 0.5;
				if (!COLISNULL(colors.back)) {
					// Back
					DrawCube(planeCenter, planeHeight, planeLength, planeLength, colors.back);
				}
				planeCenter.x += 1;
				if (!COLISNULL(colors.front)) {
					// Front
					DrawCube(planeCenter, planeHeight, planeLength, planeLength, colors.front);
				}
				planeCenter = center; // Back to center
				planeCenter.y -= 0.5;
				if (!COLISNULL(colors.down)) {
					// Down
					DrawCube(planeCenter, planeLength, planeHeight, planeLength, colors.down);
				}
				planeCenter.y += 1;
				if (!COLISNULL(colors.up)) {
					// Up
					DrawCube(planeCenter, planeLength, planeHeight, planeLength, colors.up);
				}
				planeCenter = center; // Back to center
				planeCenter.z -= 0.5;
				if (!COLISNULL(colors.right)) {
					// Right
					DrawCube(planeCenter, planeLength, planeLength, planeHeight, colors.right);
				}
				planeCenter.z += 1;
				if (!COLISNULL(colors.left)) {
					// Left
					DrawCube(planeCenter, planeLength, planeLength, planeHeight, colors.left);
				}
			}
		}
	}
}
