#ifndef BUTTONS
#define BUTTONS

#include "queue.h"
#include "rotatecube.h"
#include <raylib.h>

typedef struct {
	Rectangle rec;
	int type;
	void (*onClick)(Cube* cube, MoveQueue* queue, RotationAnimation* anim, bool* clockwise);
	Color color;
	bool pressed;
} Button;

void drawButton(Button* button, int size, bool clockwise, bool cursor);
bool hovering(Button* button, bool cursor);
Button* callButtons();
#endif
