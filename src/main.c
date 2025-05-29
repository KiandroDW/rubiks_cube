#include "drawrubikscube.h"
#include "raylib.h"
#include "rubikscube.h"
#include <math.h>

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

	// Main game loop
	while (!WindowShouldClose()) {
		if(IsKeyPressed(KEY_R)) {
			if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
				executeMove(cube, RIGHT_P, pitch);
			} else {
				executeMove(cube, RIGHT, pitch);
			}
		}
		if(IsKeyPressed(KEY_L)) {
			if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
				executeMove(cube, LEFT_P, pitch);
			} else {
				executeMove(cube, LEFT, pitch);
			}
		}
		if(IsKeyPressed(KEY_U)) {
			if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
				executeMove(cube, UP_P, pitch);
			} else {
				executeMove(cube, UP, pitch);
			}
		}
		if(IsKeyPressed(KEY_D)) {
			if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
				executeMove(cube, DOWN_P, pitch);
			} else {
				executeMove(cube, DOWN, pitch);
			}
		}
		if(IsKeyPressed(KEY_F)) {
			if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
				executeMove(cube, FRONT_P, pitch);
			} else {
				executeMove(cube, FRONT, pitch);
			}
		}
		if(IsKeyPressed(KEY_B)) {
			if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
				executeMove(cube, BACK_P, pitch);
			} else {
				executeMove(cube, BACK, pitch);
			}
		}
		if(IsKeyPressed(KEY_X)) {
			if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
				rotateCube(cube, COUNTERCLOCKWISE);
			} else {
				rotateCube(cube, CLOCKWISE);
			}
		}
		if(IsKeyPressed(KEY_Y)) {
			if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
				rotateCube(cube, LEFTWARDS);
			} else {
				rotateCube(cube, RIGHTWARDS);
			}
		}
		if(IsKeyPressed(KEY_W)) {
			if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
				rotateCube(cube, DOWNWARDS);
			} else {
				rotateCube(cube, UPWARDS);
			}
		}
		if(IsKeyPressed(KEY_SPACE)) {
			shuffle(cube);
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
				rotateCube(cube, RIGHTWARDS);
			} else if (yaw > 135) {
				yaw = 45;
				rotateCube(cube, LEFTWARDS);
			}

			if (pitch > 89)
				pitch = 89.0f;
			if (pitch < -89)
				pitch = -89.f;

		}

		float scroll = GetMouseWheelMove();
		distance -= scroll;
		if (distance <  2.0f)
			distance = 2.0f;
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
