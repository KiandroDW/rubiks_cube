#include "drawrubikscube.h"
#include "raylib.h"
#include "rubikscube.h"
#include <math.h>
#include <stdio.h>

int main(void) {
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
	Cube* cube = createCube();
	int counter = 0;

	// Main game loop
	while (!WindowShouldClose()) {
		counter++;
		if (counter > 60) {
			executeMove(cube, GetRandomValue(0, 11));
			counter = 0;
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

			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;
		}

		float scroll = GetMouseWheelMove();
		distance -= scroll;
		/* if (distance <  2.0f) */
		/* 	distance = 2.0f; */
		if (distance > 20.0f)
			distance = 20.0f;


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

			DrawRubiksCube(cube);

			EndMode3D();
		}
		EndDrawing();
	}

	CloseWindow();
	destroyCube(cube);
	return 0;
}
