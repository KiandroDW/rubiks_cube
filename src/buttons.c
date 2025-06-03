#include "buttons.h"
#include "rubikscube.h"
#include "rubikscubeparts.h"
#include <math.h>
#include <raylib.h>
#include <stdio.h>

const char* text[16] = {
	"X",
	"Y",
	"Z",
	"F",
	"U",
	"B",
	"L",
	"D",
	"R",
	"Shuffle",
	"Counter-Clockwise",
	"Clockwise",
	"Enable cursor",
	"Disable cursor",
	"-",
	"+",
};

void onClickPass(Cube* cube, MoveQueue* queue, RotationAnimation* anim, bool* clockwise) {
	return;
}

void toggleCursor(Cube* cube, MoveQueue* queue, RotationAnimation* anim, bool* clockwise) {
	cube->cursor->enabled = !cube->cursor->enabled;
}

void toggleRotation(Cube* cube, MoveQueue* queue, RotationAnimation* anim, bool* clockwise) {
	*clockwise = ! (*clockwise);
}

void shuffleCube(Cube* cube, MoveQueue* queue, RotationAnimation* anim, bool* clockwise) {
	shuffle(cube, queue);
}

void rightMoveAction(Cube* cube, MoveQueue* queue, RotationAnimation* anim, bool* clockwise) {
	rightMove(cube, queue, clockwise);
}

void leftMoveAction(Cube* cube, MoveQueue* queue, RotationAnimation* anim, bool* clockwise) {
	leftMove(cube, queue, clockwise);
}

void downMoveAction(Cube* cube, MoveQueue* queue, RotationAnimation* anim, bool* clockwise) {
	downMove(cube, queue, clockwise);
}

void upMoveAction(Cube* cube, MoveQueue* queue, RotationAnimation* anim, bool* clockwise) {
	upMove(cube, queue, clockwise);
}

void backMoveAction(Cube* cube, MoveQueue* queue, RotationAnimation* anim, bool* clockwise) {
	backMove(cube, queue, clockwise);
}

void frontMoveAction(Cube* cube, MoveQueue* queue, RotationAnimation* anim, bool* clockwise) {
	frontMove(cube, queue, clockwise);
}

void zMove(Cube* cube, MoveQueue* queue, RotationAnimation* anim, bool* clockwise) {
	rotateCubeZ(cube, anim, clockwise);
}

void yMove(Cube* cube, MoveQueue* queue, RotationAnimation* anim, bool* clockwise) {
	rotateCubeY(cube, anim, clockwise);
}

void xMove(Cube* cube, MoveQueue* queue, RotationAnimation* anim, bool* clockwise) {
	rotateCubeX(cube, anim, clockwise);
}

void increseDimension(Cube* cube, MoveQueue* queue, RotationAnimation* anim, bool* clockwise) {
	int size = cube->side;
	resetCube(cube, size + 1);
	while (queue->start != NULL) {
		popElement(queue);
	}
	anim->rotating = false;
	anim->finished = false;
}

void decreaseDimension(Cube* cube, MoveQueue* queue, RotationAnimation* anim, bool* clockwise) {
	int size = cube->side;
	if (size > 1) {
		resetCube(cube, size - 1);
		while (queue->start != NULL) {
			popElement(queue);
		}
		anim->rotating = false;
		anim->finished = false;
	}
}

void cursorRight(Cube* cube, MoveQueue* queue, RotationAnimation* anim, bool* clockwise) {
	moveCursor(cube, -1, 0);
}

void cursorLeft(Cube* cube, MoveQueue* queue, RotationAnimation* anim, bool* clockwise) {
	moveCursor(cube, 1, 0);
}

void cursorUp(Cube* cube, MoveQueue* queue, RotationAnimation* anim, bool* clockwise) {
	moveCursor(cube, 0, 1);
}

void cursorDown(Cube* cube, MoveQueue* queue, RotationAnimation* anim, bool* clockwise) {
	moveCursor(cube, 0, -1);
}

Button buttons[19] = {
	(Button) {
		// cursor disabled
		(Rectangle) {1578, 530, 212, 64},
		-12,
		&toggleCursor,
		GREEN
	},
	(Button) {
		// clockwise rotation
		(Rectangle) {1578, 604, 212, 64},
		-11,
		&toggleRotation,
		LIGHTGRAY
	},
	(Button) {
		// shuffle
		(Rectangle) {10, 826, 170, 64},
		-9,
		&shuffleCube,
		SKYBLUE
	},
	(Button) {
		// R
		(Rectangle) {1726, 826, 64, 64},
		-8,
		&rightMoveAction,
		SKYBLUE
	},
	(Button) {
		// D
		(Rectangle) {1652, 826, 64, 64},
		-7,
		&downMoveAction,
		SKYBLUE
	},
	(Button) {
		// L
		(Rectangle) {1578, 826, 64, 64},
		-6,
		&leftMoveAction,
		SKYBLUE
	},
	(Button) {
		// B
		(Rectangle) {1726, 752, 64, 64},
		-5,
		&backMoveAction,
		SKYBLUE
	},
	(Button) {
		// U
		(Rectangle) {1652, 752, 64, 64},
		-4,
		&upMoveAction,
		SKYBLUE
	},
	(Button) {
		// F
		(Rectangle) {1578, 752, 64, 64},
		-3,
		&frontMoveAction,
		SKYBLUE
	},
	(Button) {
		// Z
		(Rectangle) {1726, 678, 64, 64},
		-2,
		&zMove,
		SKYBLUE
	},
	(Button) {
		// Y
		(Rectangle) {1652, 678, 64, 64},
		-1,
		&yMove,
		SKYBLUE
	},
	(Button) {
		// X
		(Rectangle) {1578, 678, 64, 64},
		0,
		&xMove,
		SKYBLUE
	},
	(Button) {
		// cube size
		(Rectangle) {10, 752, 170, 64},
		1,
		&onClickPass,
		LIGHTGRAY
	},
	// These have to be drawn after cube size button
	(Button) {
		// + button
		(Rectangle) {116, 752, 64, 64},
		-15,
		&increseDimension,
		(Color) {0, 0, 0, 0}
	},
	(Button) {
		// - button
		(Rectangle) {10, 752, 64, 64},
		-14,
		&decreaseDimension,
		(Color) {0, 0, 0, 0}
	},
	(Button) {
		// ->
		(Rectangle) {1726, 456, 64, 64},
		2,
		&cursorRight,
		SKYBLUE
	},
	(Button) {
		// v
		(Rectangle) {1652, 456, 64, 64},
		3,
		&cursorDown,
		SKYBLUE
	},
	(Button) {
		// <-
		(Rectangle) {1578, 456, 64, 64},
		4,
		&cursorLeft,
		SKYBLUE
	},
	(Button) {
		// ^
		(Rectangle) {1652, 382, 64, 64},
		5,
		&cursorUp,
		SKYBLUE
	},
};

Button* callButtons() {
	return buttons;
}

void drawButtonBackground(Button* button) {
	Rectangle rec = button->rec;
	Rectangle bgrec = rec;
	Color bgcol = {
		button->color.r * 0.8f,
		button->color.g * 0.8f,
		button->color.b * 0.8f,
		button->color.a
	};
	if (button->pressed) {
		rec.x += 2;
		rec.y += 2;
		bgrec.width += 1;
		bgrec.height += 1;
		bgrec.x += 2;
		bgrec.y += 2;
		DrawRectangleRounded(bgrec, 0.3f, 10, bgcol);
		DrawRectangleRounded(rec, 0.3f, 10, button->color);
	} else {
		bgrec.width += 2;
		bgrec.height += 2;
		bgrec.x++;
		bgrec.y++;
		DrawRectangleRounded(bgrec, 0.3f, 10, bgcol);
		DrawRectangleRounded(rec, 0.3f, 10, button->color);
	}
}

void drawButtonText(Button* button, const char* text) {
	int textWidth = MeasureText(text, 22);
	Color textCol = {
		button->color.r * 0.5f,
		button->color.g * 0.5f,
		button->color.b * 0.5f,
		255
	};
	DrawText(
		text,
		button->rec.x + button->rec.width / 2 - textWidth / 2,
		button->rec.y + button->rec.height / 2 - 10,
		22,
		textCol
	);
}

void drawArrow(Vector2 start, Vector2 end, float arrowSize, Color color)
{
    DrawLineEx(start, end, 2.0f, color);

    Vector2 direction = { end.x - start.x, end.y - start.y };
    float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
    if (length == 0) return; // Avoid division by zero

    direction.x /= length;
    direction.y /= length;

    Vector2 perp = { -direction.y, direction.x };

    Vector2 tip = end;
    Vector2 baseRight = {
        tip.x - direction.x * arrowSize + perp.x * arrowSize * 0.5f,
        tip.y - direction.y * arrowSize + perp.y * arrowSize * 0.5f
    };
    Vector2 baseLeft = {
        tip.x - direction.x * arrowSize - perp.x * arrowSize * 0.5f,
        tip.y - direction.y * arrowSize - perp.y * arrowSize * 0.5f
    };

    DrawTriangle(tip, baseLeft, baseRight, color);
}

void drawButton(Button* button, int size, bool clockwise, bool cursor) {
	if (cursor && button->type == -12) {
		button->type = -13;
		button->color = RED;
	} else if (! cursor && button->type == -13) {
		button->type = -12;
		button->color = GREEN;
	}
	if (clockwise && button->type == -10) {
		button->type = -11;
	} else if (! clockwise && button->type == -11) {
		button->type = -10;
	}
	if (button->type <= 1 || cursor) {
		drawButtonBackground(button);
	}
	if (button->type <= 0) {
		// All normal text buttons
		drawButtonText(button, text[-button->type]);
	} else if (button->type == 1) {
		// Cube size text
		char res[4];
		sprintf(res, "%d", size);
		drawButtonText(button, res);
	} else if (cursor) {
		// Arrow buttons (only when cursor is enabled)
		Color arrowCol = {
			button->color.r * 0.5f,
			button->color.g * 0.5f,
			button->color.b * 0.5f,
			255
		};
		if (button->type == 2) {
			// ->
			drawArrow((Vector2) {button->rec.x + 10, button->rec.y + 31}, (Vector2) {button->rec.x + button->rec.width - 10, button->rec.y + 31}, 15.0f, arrowCol);
		} else if (button->type == 3) {
			// v
			drawArrow((Vector2) {button->rec.x + 31, button->rec.y + 10}, (Vector2) {button->rec.x + 31, button->rec.y + button->rec.height - 10}, 15.0f, arrowCol);
		} else if (button->type == 4) {
			// <-
			drawArrow((Vector2) {button->rec.x + button->rec.width - 10, button->rec.y + 31}, (Vector2) {button->rec.x + 10, button->rec.y + 31}, 15.0f, arrowCol);
		} else if (button->type == 5) {
			// ^
			drawArrow((Vector2) {button->rec.x + 31, button->rec.y + button->rec.height - 10}, (Vector2) {button->rec.x + 31, button->rec.y + 10}, 15.0f, arrowCol);
		}
	}
}

bool hovering(Button* button, bool cursor) {
	button->pressed = false;
	if (button->type == 1 || (!cursor && button->type > 1)) {
		return false;
	}
	Vector2 mousePos = GetMousePosition();
	if (mousePos.x >= button->rec.x && mousePos.x <= button->rec.x + button->rec.width && mousePos.y >= button->rec.y && mousePos.y <= button->rec.y + button->rec.height) {
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			button->pressed = true;
		}
		return true;
	}
	return false;
}
