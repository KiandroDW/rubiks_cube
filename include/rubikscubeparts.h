#ifndef RUBIKSCUBEPARTS
#define RUBIKSCUBEPARTS

#include "raylib.h"

typedef struct {
	int row;
	int column;
	bool enabled;
	float scale;
	int direction;
} Selection;

typedef struct {
	Color back;
	Color down;
	Color front;
	Color left;
	Color right;
	Color up;
} Colors;

typedef struct {
	Colors colors;
} Block;

typedef struct {
	int side;
	Block** blocks;
	Selection* selection;
} Cube;

typedef struct {
	Vector3 axis;
	int layer;
	int direction;
} Move;

typedef enum {
	CLOCKWISE,
	COUNTERCLOCKWISE,
	DOWNWARDS,
	LEFTWARDS,
	RIGHTWARDS,
	UPWARDS
} Rotation;

#endif
