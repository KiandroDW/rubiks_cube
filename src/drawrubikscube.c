#include "drawrubikscube.h"
#include "raylib.h"
#include "rlgl.h"
#include "rubikscube.h"
#include <stdbool.h>
#include <stdio.h>

#define COLISNULL(color) \
	(color.r == 0 && color.g == 0 && color.b == 0 && color.a == 0)

void DrawRubiksCube(Cube* cube,  RotationAnimation* anim) {
	const float planeHeight = 0.1f;
	const float planeLength = 0.9f;

	for (int i = 0; i < cube->side; i++) {
		for (int j = 0; j < cube->side; j++) {
			for (int k = 0; k < cube->side; k++) {
				rlPushMatrix();
				if (anim->rotating == true) {
					int layer = anim->layer;
					if (anim->axis.x && i == layer ||
						anim->axis.y && j == layer ||
						anim->axis.z && k == layer) {
						Vector3 pivot = {0};
						if (anim->axis.x != 0) pivot = (Vector3){anim->layer, 0, 0};
						if (anim->axis.y != 0) pivot = (Vector3){0, anim->layer, 0};
						if (anim->axis.z != 0) pivot = (Vector3){0, 0, anim->layer};

						rlTranslatef(pivot.x, pivot.y, pivot.z);
						rlRotatef(anim->angle, anim->axis.x, anim->axis.y, anim->axis.z);
						rlTranslatef(-pivot.x, -pivot.y, -pivot.z);
					}
				}
				Vector3 center = (Vector3) {i - (cube->side - 1) / 2.0f , j - (cube->side - 1) / 2.0f, k - (cube->side - 1) / 2.0f};
				DrawCube(center, 1.0f, 1.0f, 1.0f, BLACK);
				Block* block = ELEMENTAT(cube, i, j, k);
				Colors colors = block->colors;
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
				rlPopMatrix();
			}
		}
	}
}
