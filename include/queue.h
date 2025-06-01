#ifndef MOVEQUEUE
#define MOVEQUEUE

#include "raylib.h"
#include "rubikscubeparts.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct MoveQueueElement{
	Move value;
	struct MoveQueueElement* next;
} MoveQueueElement;

typedef struct {
	MoveQueueElement* start;
} MoveQueue;

static MoveQueue* initQueue() {
	MoveQueue* queue = malloc(sizeof(MoveQueue));
	queue->start = NULL;
	return queue;
}

static void addElement(Move value, MoveQueue* queue) {
	MoveQueueElement* current = queue->start;
	if (current != NULL) {
		while (current->next != NULL) {
			current = current->next;
		}
	}
	MoveQueueElement* newQueue = malloc(sizeof(MoveQueueElement));
	newQueue->value = value;
	newQueue->next = NULL;
	if (current == NULL) {
		queue->start = newQueue;
	} else {
		current->next = newQueue;
	}
}

static Move popElement(MoveQueue* queue) {
	if (queue->start != NULL) {
		MoveQueueElement* popped = queue->start;
		queue->start = popped->next;
		Move value = popped->value;
		free(popped);
		return value;
	} else {
		return (Move) {0};
	}
}

static void _freeElement(MoveQueueElement* element) {
	_freeElement(element->next);
	free(element);
}

static void freeQueue(MoveQueue* queue) {
	_freeElement(queue->start);
	free(queue);
}

static void _translateMove(char* result, Move* move) {
	if (move->axis.x) {
		if (move->direction == 1) {
			sprintf(result, "%d%s", move->layer, "F");
		} else {
			sprintf(result, "%d%s", move->layer, "B");
		}
	} else if (move->axis.y) {
		if (move->direction == 1) {
			sprintf(result, "%d%s", move->layer, "U");
		} else {
			sprintf(result, "%d%s", move->layer, "D");
		}
	} else {
		if (move->direction == 1) {
			sprintf(result, "%d%s", move->layer, "L");
		} else {
			sprintf(result, "%d%s", move->layer, "R");
		}
	}
}

static void DrawQueue(MoveQueue* queue) {
	MoveQueueElement* current = queue->start;
	int counter = 0;
	char text[5];
	while (current != NULL && counter < 20) {
		_translateMove(text, &current->value);
		DrawText(text, 900 + 75*counter, 30, 30, LIGHTGRAY);
		current = current->next;
		counter++;
	}
}

#endif
