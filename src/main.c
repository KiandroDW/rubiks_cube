#include "buttons.h"
#include "drawrubikscube.h"
#include "queue.h"
#include "raylib.h"
#include "rotatecube.h"
#include "rubikscube.h"
#include "rubikscubeparts.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

void handleKeys(Cube* cube, MoveQueue* queue, RotationAnimation* anim, bool* clockwise) {
	if(IsKeyPressed(KEY_LEFT_SHIFT) || IsKeyPressed(KEY_RIGHT_SHIFT)) {
		*clockwise = !*clockwise;
	}
	int key = GetKeyPressed();
	switch (key) {
		case KEY_RIGHT:
			moveCursor(cube, -1, 0);
			break;
		case KEY_LEFT:
			moveCursor(cube, 1, 0);
			break;
		case KEY_DOWN:
			moveCursor(cube, 0, -1);
			break;
		case KEY_UP:
			moveCursor(cube, 0, 1);
			break;
		case KEY_C:
			cube->cursor->enabled = !cube->cursor->enabled;
			break;
		case KEY_R:
			rightMove(cube, queue, clockwise);
			break;
		case KEY_L:
			leftMove(cube, queue, clockwise);
			break;
		case KEY_U:
			upMove(cube, queue, clockwise);
			break;
		case KEY_D:
			downMove(cube, queue, clockwise);
			break;
		case KEY_F:
			frontMove(cube, queue, clockwise);
			break;
		case KEY_B:
			backMove(cube, queue, clockwise);
			break;
		case KEY_X:
			rotateCubeX(cube, anim, clockwise);
			break;
		case KEY_Y:
			rotateCubeY(cube, anim, clockwise);
			break;
		case KEY_W:
			rotateCubeZ(cube, anim, clockwise);
			break;
		case KEY_SPACE:
			shuffle(cube, queue);
			break;
	}
}

int main() {
	srand(time(NULL));
	// Initialization
	const int screenWidth = 1800;
	const int screenHeight = 900;

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
	cube = createCube(3);
	RotationAnimation* rotationAnimation = malloc(sizeof(RotationAnimation));
	rotationAnimation->rotating = false;
	rotationAnimation->side = 0;
	rotationAnimation->delay = 0;

	MoveQueue* queue = initQueue();
	bool clockwise = true;

	Button* buttons = callButtons();

	// Main game loop
	while (!WindowShouldClose()) {
		handleKeys(cube, queue, rotationAnimation, &clockwise);
		Button* selected = NULL;
		for (int i = 0; i < 19; i++) {
			if (hovering(&buttons[i], cube->cursor->enabled)) {
				selected = &buttons[i];
				break;
			}
		}
		if (selected != NULL) {
			SetMouseCursor(4);
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				selected->onClick(cube, queue, rotationAnimation, &clockwise);
			}
		} else {
			SetMouseCursor(0);
		}
		updateRotation(rotationAnimation);
		updateCursor(cube);

		if (rotationAnimation->delay == 0 && queue->start != NULL) {
			rotationAnimation->delay = 7;
			startRotation(rotationAnimation, popElement(queue));
		}

		if (rotationAnimation->finished) {
			rotationAnimation->finished = false;
			decodeMove(cube, rotationAnimation);
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
		if (distance <  cube->side * 2.3f)
			distance = cube->side * 2.3f;
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

			drawRubiksCube(cube, rotationAnimation, yaw, pitch);

			EndMode3D();

			DrawFPS(20, 20);

			for (int i = 0; i < 19; i++) {
				drawButton(&buttons[i], cube->side, clockwise, cube->cursor->enabled);
			}

			drawQueue(queue);
		}
		EndDrawing();
	}

	CloseWindow();
	destroyCube(cube);
	return 0;
}
