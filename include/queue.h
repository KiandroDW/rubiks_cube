#ifndef MOVEQUEUE
#define MOVEQUEUE

#include "rubikscubeparts.h"
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

#endif
