#include "drawrubikscube.h"
#include "raylib.h"
#include "rotatecube.h"
#include "rubikscube.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	// Initialization
	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "Rubiks cube");

	// Define the camera
	Camera camera = {0};
	camera.position = (Vector3){0, 0, 10.0f};
	camera.target = (Vector3){0.0f, 0.0f, 0.0f};
	camera.up = (Vector3){0.0f, 1.0f, 0.0f};
	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;

	// Camera parameters
	float distance = 10.f;
	float yaw = 120.0f;
	float pitch = 30.0f;
	bool cursorLocked = false;

	Vector3 cubePosition = {0.0f, 0.0f, 0.0f};
	Vector2 cubeScreenPosition = {0.0f, 0.0f};

	SetTargetFPS(60);

	// Rubiks cube:
	Cube* cube;
	if (argc > 1) {
		cube = createCube(atoi(argv[1]));
	} else {
		cube = createCube(3);
	}
	RotationAnimation* rotationAnimation = malloc(sizeof(RotationAnimation));
	rotationAnimation->rotating = false;
	rotationAnimation->side = 0;

	// Main game loop
	while (!WindowShouldClose()) {
		UpdateRotation(rotationAnimation);
		UpdateSelection(cube);
		if(IsKeyPressed(KEY_RIGHT)) {
			cube->selection->column--;
			if (cube->selection->column < 0){
				cube->selection->column = cube->side - 1;
			}
		}
		if(IsKeyPressed(KEY_LEFT)) {
			cube->selection->column++;
			if (cube->selection->column >= cube->side){
				cube->selection->column = 0;
			}
		}
		if(IsKeyPressed(KEY_DOWN)) {
			cube->selection->row--;
			if (cube->selection->row < 0){
				cube->selection->row = cube->side - 1;
			}
		}
		if(IsKeyPressed(KEY_UP)) {
			cube->selection->row++;
			if (cube->selection->row >= cube->side){
				cube->selection->row = 0;
			}
		}
		if(IsKeyPressed(KEY_S)) {
			cube->selection->enabled = !cube->selection->enabled;
		}
		if(IsKeyPressed(KEY_R)) {
			if (rotationAnimation->rotating == false) {
				if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
					StartRotation(rotationAnimation, cube, RIGHT_P);
				} else {
					StartRotation(rotationAnimation, cube, RIGHT);
				}
			}
		}
		if(IsKeyPressed(KEY_L)) {
			if (rotationAnimation->rotating == false) {
				if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
					StartRotation(rotationAnimation, cube, LEFT_P);
				} else {
					StartRotation(rotationAnimation, cube, LEFT);
				}
			}
		}
		if(IsKeyPressed(KEY_U)) {
			if (rotationAnimation->rotating == false) {
				if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
					StartRotation(rotationAnimation, cube, UP_P);
				} else {
					StartRotation(rotationAnimation, cube, UP);
				}
			}
		}
		if(IsKeyPressed(KEY_D)) {
			if (rotationAnimation->rotating == false) {
				if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
					StartRotation(rotationAnimation, cube, DOWN_P);
				} else {
					StartRotation(rotationAnimation, cube, DOWN);
				}
			}
		}
		if(IsKeyPressed(KEY_F)) {
			if (rotationAnimation->rotating == false) {
				if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
					StartRotation(rotationAnimation, cube, FRONT_P);
				} else {
					StartRotation(rotationAnimation, cube, FRONT);
				}
			}
		}
		if(IsKeyPressed(KEY_B)) {
			if (rotationAnimation->rotating == false) {
				if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
					StartRotation(rotationAnimation, cube, BACK_P);
				} else {
					StartRotation(rotationAnimation, cube, BACK);
				}
			}
		}
		if(IsKeyPressed(KEY_X)) {
			if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
				rotateCube(cube, COUNTERCLOCKWISE, rotationAnimation);
			} else {
				rotateCube(cube, CLOCKWISE, rotationAnimation);
			}
		}
		if(IsKeyPressed(KEY_Y)) {
			if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
				rotateCube(cube, LEFTWARDS, rotationAnimation);
			} else {
				rotateCube(cube, RIGHTWARDS, rotationAnimation);
			}
		}
		if(IsKeyPressed(KEY_W)) {
			if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
				rotateCube(cube, DOWNWARDS, rotationAnimation);
			} else {
				rotateCube(cube, UPWARDS, rotationAnimation);
			}
		}
		if(IsKeyPressed(KEY_SPACE)) {
			shuffle(cube);
		}

		if (rotationAnimation->finished) {
			rotationAnimation->finished = false;
			DecodeMove(cube, rotationAnimation);
		}

		if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
			DisableCursor();
			cursorLocked = true;
		} else if (IsMouseButtonReleased(MOUSE_RIGHT_BUTTON)) {
			EnableCursor();
			cursorLocked = false;
		}

		if (cursorLocked) {
			Vector2 mouseDelta = GetMouseDelta();
			yaw -= mouseDelta.x * 0.5f;
			pitch += mouseDelta.y * 0.5f;

			if (yaw < 45) {
				yaw = 135;
				rotateCube(cube, RIGHTWARDS, rotationAnimation);
			} else if (yaw > 135) {
				yaw = 45;
				rotateCube(cube, LEFTWARDS, rotationAnimation);
			}

			if (pitch > 89)
				pitch = 89.0f;
			if (pitch < -89)
				pitch = -89.0f;
		}

		float scroll = GetMouseWheelMove();
		distance -= scroll * cube->side;
		if (distance <  cube->side * 1.2f)
			distance = cube->side * 1.2f;
		if (distance > cube->side * 6.0f)
			distance = cube->side * 6.0f;


		float yawRad = yaw * DEG2RAD;
		float pitchRad = pitch * DEG2RAD;

		camera.position.x =
			camera.target.x + distance * cosf(pitchRad) * sinf(yawRad);
		camera.position.y =
			camera.target.y + distance * sinf(pitchRad);
		camera.position.z =
			camera.target.z + distance * cosf(pitchRad) * cosf(yawRad);

		BeginDrawing();
		{
			ClearBackground(RAYWHITE);

			BeginMode3D(camera);

			DrawRubiksCube(cube, rotationAnimation);

			EndMode3D();
		}
		EndDrawing();
	}

	CloseWindow();
	destroyCube(cube);
	return 0;
}
