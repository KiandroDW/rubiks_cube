#include "drawrubikscube.h"
#include "raylib.h"
#include "rlgl.h"
#include "rubikscube.h"
#include <stdbool.h>

#define COLISNULL(color) \
	(color.r == 0 && color.g == 0 && color.b == 0 && color.a == 0)

void DrawLayer(Cube* cube, RotationAnimation* anim, int x_0, int dx, int y_0, int dy, int z_0, int dz) {
	const float planeHeight = 0.1f;
	const float planeLength = 0.9f;
	int selected_row = cube->selection->row;
	int selected_column = cube->selection->column;

	for (int i = x_0; i < x_0 + dx; i++) {
		for (int j = y_0; j < y_0 + dy; j++) {
			for (int k = z_0; k < z_0 + dz; k++) {

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
						rlRotatef(-anim->angle, anim->axis.x, anim->axis.y, anim->axis.z);
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
					if (cube->selection->enabled && i == cube->side - 1 && j == selected_row && k == selected_column) {
						float scale = cube->selection->scale;
						DrawCube(planeCenter, planeHeight, planeLength * scale, planeLength * scale, colors.front);
					} else {
						DrawCube(planeCenter, planeHeight, planeLength, planeLength, colors.front);
					}
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

void DrawRubiksCube(Cube* cube,  RotationAnimation* anim, int yaw, int pitch) {
	DrawLayer(cube, anim, cube->side - 1, 1, 0, cube->side, 0, cube->side);
	if (yaw > 90)
		DrawLayer(cube, anim, 0, cube->side, 0, cube->side, 0, 1);
	if (yaw < 90)
		DrawLayer(cube, anim, 0, cube->side, 0, cube->side, cube->side - 1, 1);
	if (pitch > 0)
		DrawLayer(cube, anim, 0, cube->side, cube->side - 1, 1, 0, cube->side);
	if (pitch < 0)
		DrawLayer(cube, anim, 0, cube->side, 0, 1, 0, cube->side);

	if (anim->rotating) {
		if (anim->axis.x) {
			DrawLayer(cube, anim, anim->layer, 1, 0, cube->side, 0, cube->side);
			if (anim->layer > 0)
				DrawLayer(cube, anim, anim->layer - 1, 1, 0, cube->side, 0, cube->side);
			// You can never look from the back, so it doesnt render that layer
		} else if (anim->axis.y) {
			DrawLayer(cube, anim, 0, cube->side, anim->layer, 1, 0, cube->side);
			if (anim->layer > 0)
				DrawLayer(cube, anim, 0, cube->side, anim->layer - 1, 1, 0, cube->side);
			if (anim->layer < cube->side - 1)
				DrawLayer(cube, anim, 0, cube->side, anim->layer + 1, 1, 0, cube->side);
		} else {
			DrawLayer(cube, anim, 0, cube->side, 0, cube->side, anim->layer, 1);
			if (anim->layer > 0)
				DrawLayer(cube, anim, 0, cube->side, 0, cube->side, anim->layer - 1, 1);
			if (anim->layer < cube->side - 1)
				DrawLayer(cube, anim, 0, cube->side, 0, cube->side, anim->layer + 1, 1);
		}
	}
}
