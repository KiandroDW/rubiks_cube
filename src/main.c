#include "drawrubikscube.h"
#include "queue.h"
#include "raylib.h"
#include "rotatecube.h"
#include "rubikscube.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void DrawOverlay(int size) {
	const char* labels[9] = {
        "X", "Y", "Z",
        "B", "U", "F",
        "L", "D", "R"
    };
	const char* bigText = "Clockwise";
	float spacing = 10;
	float smallRectSize = 75;

	float bigRectWidth = 3 * smallRectSize + 2 * spacing;
	float bigRectHeight = 75;
	float bigRectX = 1800 - bigRectWidth - spacing;
	float bigRectY = 900 - bigRectHeight - spacing - 3 * (smallRectSize + spacing);
	DrawRectangleRounded((Rectangle){ bigRectX, bigRectY, bigRectWidth, bigRectHeight }, 0.3f, 10, LIGHTGRAY);
	int bigTextWidth = MeasureText(bigText, 25);
	DrawText(bigText, bigRectX + bigRectWidth / 2 - bigTextWidth / 2, bigRectY + bigRectHeight / 2 - 10, 25, DARKGRAY);

	float startX = 1800 - 3 * (smallRectSize + spacing);
	float startY = 900 - 3 * (smallRectSize + spacing);

	for (int row = 0; row < 3; row++) {
		for (int col = 0; col < 3; col++) {
			int index = row * 3 + col;
			float x = startX + col * (smallRectSize + spacing);
			float y = startY + row * (smallRectSize + spacing);
			DrawRectangleRounded((Rectangle){ x, y, smallRectSize, smallRectSize }, 0.2f, 10, SKYBLUE);

			int textWidth = MeasureText(labels[index], 25);
			DrawText(labels[index], x + smallRectSize / 2 - textWidth / 2, y + smallRectSize / 2 - 10, 25, DARKBLUE);
		}
	}

	// Cube size
	char res[4];
	sprintf(res, "%d", size);
	DrawRectangleRounded((Rectangle) {10, 730, 200, 75}, 0.3f, 10, LIGHTGRAY);
	DrawText("-", 30, 750, 40, BLACK);
	DrawText("+", 170, 750, 40, BLACK);
	int textWidth = MeasureText(res, 40);
	DrawText(res, 105 - textWidth / 2, 750, 40, BLACK);

	// Shuffle button
	sprintf(res, "%d", size);
	DrawRectangleRounded((Rectangle) {10, 815, 200, 75}, 0.3f, 10, SKYBLUE);
	textWidth = MeasureText("Shuffle", 40);
	DrawText("Shuffle", 105 - textWidth / 2, 835, 40, DARKBLUE);
}

int main(int argc, char* argv[]) {
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
	if (argc > 1) {
		cube = createCube(atoi(argv[1]));
	} else {
		cube = createCube(3);
	}
	RotationAnimation* rotationAnimation = malloc(sizeof(RotationAnimation));
	rotationAnimation->rotating = false;
	rotationAnimation->side = 0;
	rotationAnimation->delay = 0;

	MoveQueue* queue = initQueue();

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
			Move move;
			move.axis = (Vector3) {0, 0, 1};
			if (cube->selection->enabled) {
				move.layer = cube->selection->column;
			} else {
				move.layer = 0;
			}
			if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
				move.direction = 1;
				addElement(move, queue);
			} else {
				move.direction = -1;
				addElement(move, queue);
			}
		}
		if(IsKeyPressed(KEY_L)) {
			Move move;
			move.axis = (Vector3) {0, 0, 1};
			if (cube->selection->enabled) {
				move.layer = cube->selection->column;
			} else {
				move.layer = cube->side - 1;
			}
			if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
				move.direction = -1;
				addElement(move, queue);
			} else {
				move.direction = 1;
				addElement(move, queue);
			}
		}
		if(IsKeyPressed(KEY_U)) {
			Move move;
			move.axis = (Vector3) {0, 1, 0};
			if (cube->selection->enabled) {
				move.layer = cube->selection->row;
			} else {
				move.layer = cube->side - 1;
			}
			if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
				move.direction = -1;
				addElement(move, queue);
			} else {
				move.direction = 1;
				addElement(move, queue);
			}
		}
		if(IsKeyPressed(KEY_D)) {
			Move move;
			move.axis = (Vector3) {0, 1, 0};
			if (cube->selection->enabled) {
				move.layer = cube->selection->row;
			} else {
				move.layer = 0;
			}
			if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
				move.direction = 1;
				addElement(move, queue);
			} else {
				move.direction = -1;
				addElement(move, queue);
			}
		}
		if(IsKeyPressed(KEY_F)) {
			Move move;
			move.axis = (Vector3) {1, 0, 0};
			move.layer = cube->side - 1;
			if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
				move.direction = -1;
				addElement(move, queue);
			} else {
				move.direction = 1;
				addElement(move, queue);
			}
		}
		if(IsKeyPressed(KEY_B)) {
			Move move;
			move.axis = (Vector3) {1, 0, 0};
			move.layer = 0;
			if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
				move.direction = 1;
				addElement(move, queue);
			} else {
				move.direction = -1;
				addElement(move, queue);
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
			shuffle(cube, queue);
		}

		if (rotationAnimation->delay == 0 && queue->start != NULL) {
			rotationAnimation->delay = 7;
			StartRotation(rotationAnimation, popElement(queue));
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

			DrawRubiksCube(cube, rotationAnimation, yaw, pitch);

			EndMode3D();

			DrawFPS(20, 20);

			DrawOverlay(cube->side);

			DrawQueue(queue);

		}
		EndDrawing();
	}

	CloseWindow();
	destroyCube(cube);
	return 0;
}
