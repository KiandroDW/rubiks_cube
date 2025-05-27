#include "raylib.h"

typedef struct {
	Color back;
	Color down;
	Color front;
	Color left;
	Color right;
	Color up;
} Colors;

typedef struct {
	Vector3 position;
	Colors colors;
} Block;

typedef struct {
	Block* blocks[3][3][3];
} Cube;

Block* createBlock(Vector3 position);
Cube* createCube();
void destroyCube(Cube* cube);
