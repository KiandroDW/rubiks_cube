#include "rubikscube.h"
#include "rubikscubeparts.h"
#include "solver.h"
#include <bits/types/stack_t.h>
#include <raylib.h>

#define RIGHT(n) (Move) {(Vector3) {0, 0, 1}, 0, -1}
#define LEFT(n) (Move) {(Vector3) {0, 0, 1}, n-1, 1}
#define RIGHT_PRIME(n) (Move) {(Vector3) {0, 0, 1}, 0, 1}
#define LEFT_PRIME(n) (Move) {(Vector3) {0, 0, 1}, n-1, -1}
#define DOWN(n) (Move) {(Vector3) {0, 1, 0}, 0, -1}
#define UP(n) (Move) {(Vector3) {0, 1, 0}, n-1, 1}
#define DOWN_PRIME(n) (Move) {(Vector3) {0, 1, 0}, 0, 1}
#define UP_PRIME(n) (Move) {(Vector3) {0, 1, 0}, n-1, -1}
#define BACK(n) (Move) {(Vector3) {1, 0, 0}, 0, -1}
#define FRONT(n) (Move) {(Vector3) {1, 0, 0}, n-1, 1}
#define BACK_PRIME(n) (Move) {(Vector3) {1, 0, 0}, 0, 1}
#define FRONT_PRIME(n) (Move) {(Vector3) {1, 0, 0}, n-1, -1}
#define SIDE(n) (Move) {(Vector3) {1, 0, 0}, n, 1}
#define SIDE_PRIME(n) (Move) {(Vector3) {1, 0, 0}, n, -1}
#define EDGE(n) (Move) {(Vector3) {0, 1, 0}, n, 1}
#define EDGE_PRIME(n) (Move) {(Vector3) {0, 1, 0}, n, -1}
#define MIDDLE(n) (Move) {(Vector3) {0, 0, 1}, n, 1}
#define MIDDLE_PRIME(n) (Move) {(Vector3) {0, 0, 1}, n, -1}

#define EQ(c1, c2) (c1.r == c2.r && c1.g == c2.g && c1.b == c2.b && c1.a == c2.a)
#define VEQ(v1, v2) (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z)

void solverMove(Cube* cube, MoveQueue* queue, Move move) {
	executeMove(cube, move);
	addElement(move, queue);
}

void edgeSwap(Cube* cube, MoveQueue* queue) {
	solverMove(cube, queue, RIGHT(cube->side));
	solverMove(cube, queue, UP(cube->side));
	solverMove(cube, queue, RIGHT_PRIME(cube->side));
	solverMove(cube, queue, UP_PRIME(cube->side));
	solverMove(cube, queue, RIGHT_PRIME(cube->side));
	solverMove(cube, queue, FRONT(cube->side));
	solverMove(cube, queue, RIGHT(cube->side));
	solverMove(cube, queue, RIGHT(cube->side));
	solverMove(cube, queue, UP_PRIME(cube->side));
	solverMove(cube, queue, RIGHT_PRIME(cube->side));
	solverMove(cube, queue, UP_PRIME(cube->side));
	solverMove(cube, queue, RIGHT(cube->side));
	solverMove(cube, queue, UP(cube->side));
	solverMove(cube, queue, RIGHT_PRIME(cube->side));
	solverMove(cube, queue, FRONT_PRIME(cube->side));
}

void cornerSwap(Cube* cube, MoveQueue* queue) {
	solverMove(cube, queue, RIGHT(cube->side));
	solverMove(cube, queue, UP_PRIME(cube->side));
	solverMove(cube, queue, RIGHT_PRIME(cube->side));
	solverMove(cube, queue, UP_PRIME(cube->side));
	solverMove(cube, queue, RIGHT(cube->side));
	solverMove(cube, queue, UP(cube->side));
	solverMove(cube, queue, RIGHT_PRIME(cube->side));
	solverMove(cube, queue, FRONT_PRIME(cube->side));
	solverMove(cube, queue, RIGHT(cube->side));
	solverMove(cube, queue, UP(cube->side));
	solverMove(cube, queue, RIGHT_PRIME(cube->side));
	solverMove(cube, queue, UP_PRIME(cube->side));
	solverMove(cube, queue, RIGHT_PRIME(cube->side));
	solverMove(cube, queue, FRONT(cube->side));
	solverMove(cube, queue, RIGHT(cube->side));
}

typedef struct MoveStackElement{
	Move value;
	struct MoveStackElement* next;
} MoveStackElement;

typedef struct {
	MoveStackElement* start;
} MoveStack;

static MoveStack* initStack() {
	MoveStack* stack = malloc(sizeof(MoveStack));
	stack->start = NULL;
	return stack;
}

static void addStackElement(Move value, MoveStack* stack) {
	MoveStackElement* current = stack->start;
	MoveStackElement* newStack = malloc(sizeof(MoveStackElement));
	newStack->value = value;
	if (current != NULL) {
		newStack->next = current;
	} else {
		newStack->next = NULL;
	}
	stack->start = newStack;
}

static Move popStackElement(MoveStack* stack) {
	if (stack->start != NULL) {
		MoveStackElement* popped = stack->start;
		stack->start = popped->next;
		Move value = popped->value;
		value.direction = - value.direction; // Undoing a move is the inverse direction
		free(popped);
		return value;
	} else {
		return (Move) {0};
	}
}

static void _freeStackElement(MoveStackElement* element) {
	if (element != NULL) {
		_freeStackElement(element->next);
		free(element);
	}
}

static void freeStack(MoveStack* stack) {
	_freeStackElement(stack->start);
	free(stack);
}

Block* findWrongEdgePiece(Cube* cube) {
	Vector3 testVec = {1, 2, 2};
	if (!VEQ(ELEMENTAT(cube, 1, 2, 2)->correctPos, testVec) || !EQ(ELEMENTAT(cube, 1, 2, 2)->colors.left, ORANGE)) {
		Block* fakeBlock = malloc(sizeof(Block));
		fakeBlock->correctPos = (Vector3) {1, 2, 2};
		fakeBlock->colors.up = WHITE;
		//("Used element 1 UL\n");
		return fakeBlock;
	}
	testVec = (Vector3) {1, 0, 2};
	if (!VEQ(ELEMENTAT(cube, 1, 0, 2)->correctPos, testVec) || !EQ(ELEMENTAT(cube, 1, 0, 2)->colors.left, ORANGE)) {
		Block* fakeBlock = malloc(sizeof(Block));
		fakeBlock->correctPos = (Vector3) {1, 0, 2};
		fakeBlock->colors.up = YELLOW;
		//("Used element 2 DL\n");
		return fakeBlock;
	}
	testVec = (Vector3) {0, 1, 2};
	if (!VEQ(ELEMENTAT(cube, 0, 1, 2)->correctPos, testVec) || !EQ(ELEMENTAT(cube, 0, 1, 2)->colors.left, ORANGE)) {
		Block* fakeBlock = malloc(sizeof(Block));
		fakeBlock->correctPos = (Vector3) {0, 1, 2};
		fakeBlock->colors.up = BLUE;
		//("Used element 3 BL\n");
		return fakeBlock;
	}
	testVec = (Vector3) {2, 1, 2};
	if (!VEQ(ELEMENTAT(cube, 2, 1, 2)->correctPos, testVec) || !EQ(ELEMENTAT(cube, 2, 1, 2)->colors.left, ORANGE)) {
		Block* fakeBlock = malloc(sizeof(Block));
		fakeBlock->correctPos = (Vector3) {2, 1, 2};
		fakeBlock->colors.up = GREEN;
		//("Used element 4 FL\n");
		return fakeBlock;
	}
	testVec = (Vector3) {0, 0, 1};
	if (!VEQ(ELEMENTAT(cube, 0, 0, 1)->correctPos, testVec) || !EQ(ELEMENTAT(cube, 0, 0, 1)->colors.down, YELLOW)) {
		Block* fakeBlock = malloc(sizeof(Block));
		fakeBlock->correctPos = (Vector3) {0, 0, 1};
		fakeBlock->colors.up = YELLOW;
		//("Used element 5 DB\n");
		return fakeBlock;
	}
	testVec = (Vector3) {2, 0, 1};
	if (!VEQ(ELEMENTAT(cube, 2, 0, 1)->correctPos, testVec) || !EQ(ELEMENTAT(cube, 2, 0, 1)->colors.down, YELLOW)) {
		Block* fakeBlock = malloc(sizeof(Block));
		fakeBlock->correctPos = (Vector3) {2, 0, 1};
		fakeBlock->colors.up = YELLOW;
		//("Used element 6 DF\n");
		return fakeBlock;
	}
	testVec = (Vector3) {1, 0, 0};
	if (!VEQ(ELEMENTAT(cube, 1, 0, 0)->correctPos, testVec) || !EQ(ELEMENTAT(cube, 1, 0, 0)->colors.down, YELLOW)) {
		Block* fakeBlock = malloc(sizeof(Block));
		fakeBlock->correctPos = (Vector3) {1, 0, 0};
		fakeBlock->colors.up = YELLOW;
		//("Used element 7 DR\n");
		return fakeBlock;
	}
	testVec = (Vector3) {0, 1, 0};
	if (!VEQ(ELEMENTAT(cube, 0, 1, 0)->correctPos, testVec) || !EQ(ELEMENTAT(cube, 0, 1, 0)->colors.right, RED)) {
		Block* fakeBlock = malloc(sizeof(Block));
		fakeBlock->correctPos = (Vector3) {0, 1, 0};
		fakeBlock->colors.up = RED;
		//("Used element 8 BR\n");
		return fakeBlock;
	}
	testVec = (Vector3) {2, 1, 0};
	if (!VEQ(ELEMENTAT(cube, 2, 1, 0)->correctPos, testVec) || !EQ(ELEMENTAT(cube, 2, 1, 0)->colors.right, RED)) {
		Block* fakeBlock = malloc(sizeof(Block));
		fakeBlock->correctPos = (Vector3) {2, 1, 0};
		fakeBlock->colors.up = RED;
		//("Used element 9 FR\n");
		return fakeBlock;
	}
	testVec = (Vector3) {0, 2, 1};
	if (!VEQ(ELEMENTAT(cube, 0, 2, 1)->correctPos, testVec) || !EQ(ELEMENTAT(cube, 0, 2, 1)->colors.up, WHITE)) {
		Block* fakeBlock = malloc(sizeof(Block));
		fakeBlock->correctPos = (Vector3) {0, 2, 1};
		fakeBlock->colors.up = BLUE;
		//("Used element 10 UB\n");
		return fakeBlock;
	}
	testVec = (Vector3) {2, 2, 1};
	if (!VEQ(ELEMENTAT(cube, 2, 2, 1)->correctPos, testVec) || !EQ(ELEMENTAT(cube, 2, 2, 1)->colors.up, WHITE)) {
		Block* fakeBlock = malloc(sizeof(Block));
		fakeBlock->correctPos = (Vector3) {2, 2, 1};
		fakeBlock->colors.up = GREEN;
		//("Used element 11 UF\n");
		return fakeBlock;
	}
	//("Done\n");
	return 0;
}

Block* findWrongCornerPiece(Cube* cube) {
	Vector3 testVec = {2, 0, 0};
	if (!VEQ(ELEMENTAT(cube, 2, 0, 0)->correctPos, testVec) || !EQ(ELEMENTAT(cube, 2, 0, 0)->colors.down, YELLOW)) {
		Block* fakeBlock = malloc(sizeof(Block));
		fakeBlock->correctPos = (Vector3) {2, 0, 0};
		fakeBlock->colors.left = YELLOW;
		//("Used element 1 DRF\n");
		return fakeBlock;
	}
	testVec = (Vector3) {2, 0, 2};
	if (!VEQ(ELEMENTAT(cube, 2, 0, 2)->correctPos, testVec) || !EQ(ELEMENTAT(cube, 2, 0, 2)->colors.down, YELLOW)) {
		Block* fakeBlock = malloc(sizeof(Block));
		fakeBlock->correctPos = (Vector3) {2, 0, 2};
		fakeBlock->colors.left = YELLOW;
		//("Used element 2 DLF\n");
		return fakeBlock;
	}
	testVec = (Vector3) {0, 0, 2};
	if (!VEQ(ELEMENTAT(cube, 0, 0, 2)->correctPos, testVec) || !EQ(ELEMENTAT(cube, 0, 0, 2)->colors.down, YELLOW)) {
		Block* fakeBlock = malloc(sizeof(Block));
		fakeBlock->correctPos = (Vector3) {0, 0, 2};
		fakeBlock->colors.left = YELLOW;
		//("Used element 3 DLB\n");
		return fakeBlock;
	}
	testVec = (Vector3) {0, 0, 0};
	if (!VEQ(ELEMENTAT(cube, 0, 0, 0)->correctPos, testVec) || !EQ(ELEMENTAT(cube, 0, 0, 0)->colors.down, YELLOW)) {
		Block* fakeBlock = malloc(sizeof(Block));
		fakeBlock->correctPos = (Vector3) {0, 0, 0};
		fakeBlock->colors.left = YELLOW;
		//("Used element 4 DRB\n");
		return fakeBlock;
	}
	testVec = (Vector3) {2, 2, 0};
	if (!VEQ(ELEMENTAT(cube, 2, 2, 0)->correctPos, testVec) || !EQ(ELEMENTAT(cube, 2, 2, 0)->colors.front, GREEN)) {
		Block* fakeBlock = malloc(sizeof(Block));
		fakeBlock->correctPos = (Vector3) {2, 2, 0};
		fakeBlock->colors.left = GREEN;
		//("Used element 5 ULF\n");
		return fakeBlock;
	}
	testVec = (Vector3) {0, 2, 0};
	if (!VEQ(ELEMENTAT(cube, 0, 2, 0)->correctPos, testVec) || !EQ(ELEMENTAT(cube, 0, 2, 0)->colors.up, WHITE)) {
		Block* fakeBlock = malloc(sizeof(Block));
		fakeBlock->correctPos = (Vector3) {0, 2, 0};
		fakeBlock->colors.left = WHITE;
		//("Used element 6 ULB\n");
		return fakeBlock;
	}
	testVec = (Vector3) {2, 2, 2};
	if (!VEQ(ELEMENTAT(cube, 2, 2, 2)->correctPos, testVec) || !EQ(ELEMENTAT(cube, 2, 2, 2)->colors.left, ORANGE)) {
		Block* fakeBlock = malloc(sizeof(Block));
		fakeBlock->correctPos = (Vector3) {2, 2, 2};
		fakeBlock->colors.left = ORANGE;
		//("Used element 7 ULF\n");
		return fakeBlock;
	}
	//("Done\n");
	return 0;
}

void solveCube(Cube* originalCube, MoveQueue* queue) {
	Cube* cube = copyCube(originalCube);
	Block* buffer = ELEMENTAT(cube, 1, 2, 0);
	MoveStack* setupStack = initStack();
	bool freeNext = false;
	int max_moves = 200;
	/* if (buffer->correctPos.x != 2 || buffer->correctPos.y != 2 || buffer->correctPos.z != 0) { */
	while (max_moves > 0){
		max_moves--;
		bool end = false;
		if (EQ(buffer->colors.up, YELLOW)) {
			if (buffer->correctPos.z == 1) {
				if (buffer->correctPos.x == 0) {
					addStackElement(DOWN(cube->side), setupStack);
					solverMove(cube, queue, DOWN(cube->side));
				} else {
					addStackElement(DOWN_PRIME(cube->side), setupStack);
					solverMove(cube, queue, DOWN_PRIME(cube->side));
				}
			} else if (buffer->correctPos.z == 0) {
				addStackElement(DOWN(cube->side), setupStack);
				addStackElement(DOWN(cube->side), setupStack);
				solverMove(cube, queue, DOWN(cube->side));
				solverMove(cube, queue, DOWN(cube->side));
			}
			addStackElement(LEFT(cube->side), setupStack);
			addStackElement(LEFT(cube->side), setupStack);
			solverMove(cube, queue, LEFT(cube->side));
			solverMove(cube, queue, LEFT(cube->side));
			edgeSwap(cube, queue);
			while(setupStack->start != NULL) {
				Move reverting = popStackElement(setupStack);
				solverMove(cube, queue, reverting);
			}
		} else if (buffer->correctPos.y == 1) {
			if (EQ(buffer->colors.up, GREEN)) {
				if (buffer->correctPos.z == 0) {
					addStackElement(EDGE(1), setupStack);
					addStackElement(EDGE(1), setupStack);
					addStackElement(LEFT(cube->side), setupStack);
					solverMove(cube, queue, EDGE(1));
					solverMove(cube, queue, EDGE(1));
					solverMove(cube, queue, LEFT(cube->side));
				} else {
					addStackElement(LEFT_PRIME(cube->side), setupStack);
					solverMove(cube, queue, LEFT_PRIME(cube->side));
				}
			} else if (EQ(buffer->colors.up, ORANGE)) {
				if (buffer->correctPos.x == 2) {
					addStackElement(EDGE(1), setupStack);
					addStackElement(LEFT(cube->side), setupStack);
					solverMove(cube, queue, EDGE(1));
					solverMove(cube, queue, LEFT(cube->side));
				} else {
					addStackElement(EDGE_PRIME(1), setupStack);
					addStackElement(LEFT_PRIME(cube->side), setupStack);
					solverMove(cube, queue, EDGE_PRIME(1));
					solverMove(cube, queue, LEFT_PRIME(cube->side));
				}
			} else if (EQ(buffer->colors.up, BLUE)) {
				if (buffer->correctPos.z == 0) {
					addStackElement(EDGE(1), setupStack);
					addStackElement(EDGE(1), setupStack);
					addStackElement(LEFT_PRIME(cube->side), setupStack);
					solverMove(cube, queue, EDGE(1));
					solverMove(cube, queue, EDGE(1));
					solverMove(cube, queue, LEFT_PRIME(cube->side));
				} else {
					addStackElement(LEFT(cube->side), setupStack);
					solverMove(cube, queue, LEFT(cube->side));
				}
			} else {
				if (buffer->correctPos.x == 2) {
					addStackElement(EDGE(1), setupStack);
					addStackElement(LEFT_PRIME(cube->side), setupStack);
					solverMove(cube, queue, EDGE(1));
					solverMove(cube, queue, LEFT_PRIME(cube->side));
				} else {
					addStackElement(EDGE_PRIME(1), setupStack);
					addStackElement(LEFT(cube->side), setupStack);
					solverMove(cube, queue, EDGE_PRIME(1));
					solverMove(cube, queue, LEFT(cube->side));
				}
			}
			edgeSwap(cube, queue);
			while(setupStack->start != NULL) {
				Move reverting = popStackElement(setupStack);
				solverMove(cube, queue, reverting);
			}
		} else if (buffer->correctPos.z == 1) {
			if (EQ(buffer->colors.up, GREEN)) {
				addStackElement(MIDDLE(1), setupStack);
				solverMove(cube, queue, MIDDLE(1));
				if (buffer->correctPos.y == 0) {
					addStackElement(DOWN(cube->side), setupStack);
					solverMove(cube, queue, DOWN(cube->side));
				} else {
					addStackElement(DOWN_PRIME(cube->side), setupStack);
					solverMove(cube, queue, DOWN_PRIME(cube->side));
				}
			} else if (EQ(buffer->colors.up, WHITE)) {
				addStackElement(MIDDLE(1), setupStack);
				addStackElement(MIDDLE(1), setupStack);
				solverMove(cube, queue, MIDDLE(1));
				solverMove(cube, queue, MIDDLE(1));
				if (buffer->correctPos.x == 0) {
					addStackElement(DOWN_PRIME(cube->side), setupStack);
					solverMove(cube, queue, DOWN_PRIME(cube->side));
				} else {
					addStackElement(DOWN(cube->side), setupStack);
					solverMove(cube, queue, DOWN(cube->side));
				}
			} else if (EQ(buffer->colors.up, BLUE)) {
				addStackElement(MIDDLE_PRIME(1), setupStack);
				solverMove(cube, queue, MIDDLE_PRIME(1));
				if (buffer->correctPos.y == 0) {
					addStackElement(DOWN_PRIME(cube->side), setupStack);
					solverMove(cube, queue, DOWN_PRIME(cube->side));
				} else {
					addStackElement(DOWN(cube->side), setupStack);
					solverMove(cube, queue, DOWN(cube->side));
				}
			} else {
				//("Cant be reached\n");
			}
			addStackElement(LEFT(cube->side), setupStack);
			addStackElement(LEFT(cube->side), setupStack);
			solverMove(cube, queue, LEFT(cube->side));
			solverMove(cube, queue, LEFT(cube->side));
			edgeSwap(cube, queue);
			while(setupStack->start != NULL) {
				Move reverting = popStackElement(setupStack);
				solverMove(cube, queue, reverting);
			}
		} else {
			if (EQ(buffer->colors.up, ORANGE)) {
				if (buffer->correctPos.y == 0) {
					addStackElement(LEFT_PRIME(cube->side), setupStack);
					solverMove(cube, queue, LEFT_PRIME(cube->side));
				} else {
					addStackElement(LEFT(cube->side), setupStack);
					solverMove(cube, queue, LEFT(cube->side));
				}
				addStackElement(EDGE(1), setupStack);
				addStackElement(LEFT(cube->side), setupStack);
				solverMove(cube, queue, EDGE(1));
				solverMove(cube, queue, LEFT(cube->side));
			} else if (EQ(buffer->colors.up, RED)) {
				if (buffer->correctPos.y == 0) {
					addStackElement(DOWN_PRIME(cube->side), setupStack);
					addStackElement(DOWN_PRIME(cube->side), setupStack);
					addStackElement(LEFT_PRIME(cube->side), setupStack);
					addStackElement(EDGE(1), setupStack);
					addStackElement(LEFT(cube->side), setupStack);
					solverMove(cube, queue, DOWN_PRIME(cube->side));
					solverMove(cube, queue, DOWN_PRIME(cube->side));
					solverMove(cube, queue, LEFT_PRIME(cube->side));
					solverMove(cube, queue, EDGE(1));
					solverMove(cube, queue, LEFT(cube->side));
				} else {
					end = true;
				}
			} else if (EQ(buffer->colors.up, WHITE)) {
				if (buffer->correctPos.z == 0) {
					end = true;
				}
			} else {
				//("Cant be reached\n");
			}
			if (!end) {
				edgeSwap(cube, queue);
				while(setupStack->start != NULL) {
					Move reverting = popStackElement(setupStack);
					solverMove(cube, queue, reverting);
				}
			}
		}
		if (freeNext) {
			free(buffer);
			freeNext = false;
		}
		if (end) {
			buffer = findWrongEdgePiece(cube);
			freeNext = true;
			if (buffer == 0) {
				break;
			}
		} else {
			buffer = ELEMENTAT(cube, 1, 2, 0);
		}
	}
	buffer = ELEMENTAT(cube, 0, 2, 2);
	freeNext = false;
	max_moves = 50;
	while (max_moves > 0){
	/* if (1==1) { */
		max_moves--;
		bool end = false;
		if (EQ(buffer->colors.left, YELLOW)) {
			if (buffer->correctPos.z == 2) {
				addStackElement(DOWN(cube->side), setupStack);
				solverMove(cube, queue, DOWN(cube->side));
				if (buffer->correctPos.x == 0) {
					addStackElement(DOWN(cube->side), setupStack);
					solverMove(cube, queue, DOWN(cube->side));
				}
			} else if (buffer->correctPos.z == 0 && buffer->correctPos.x == 0) {
				addStackElement(DOWN_PRIME(cube->side), setupStack);
				solverMove(cube, queue, DOWN_PRIME(cube->side));
			}
			cornerSwap(cube, queue);
			while(setupStack->start != NULL) {
				Move reverting = popStackElement(setupStack);
				solverMove(cube, queue, reverting);
			}
		} else if (EQ(buffer->colors.left, GREEN)) {
			if (buffer->correctPos.z == 2) {
				addStackElement(FRONT(cube->side), setupStack);
				solverMove(cube, queue, FRONT(cube->side));
				if (buffer->correctPos.y == 0) {
					addStackElement(FRONT(cube->side), setupStack);
					solverMove(cube, queue, FRONT(cube->side));
				}
			} else if (buffer->correctPos.z == 0 && buffer->correctPos.y == 0) {
				addStackElement(FRONT_PRIME(cube->side), setupStack);
				solverMove(cube, queue, FRONT_PRIME(cube->side));
			}
			addStackElement(RIGHT_PRIME(cube->side), setupStack);
			solverMove(cube, queue, RIGHT_PRIME(cube->side));
			cornerSwap(cube, queue);
			while(setupStack->start != NULL) {
				Move reverting = popStackElement(setupStack);
				solverMove(cube, queue, reverting);
			}
		} else if (EQ(buffer->colors.left, RED)) {
			if (buffer->correctPos.x == 0) {
				addStackElement(RIGHT_PRIME(cube->side), setupStack);
				solverMove(cube, queue, RIGHT_PRIME(cube->side));
				if (buffer->correctPos.y == 0) {
					addStackElement(RIGHT_PRIME(cube->side), setupStack);
					solverMove(cube, queue, RIGHT_PRIME(cube->side));
				}
			} else if (buffer->correctPos.y == 0 && buffer->correctPos.x == 2) {
				addStackElement(RIGHT(cube->side), setupStack);
				solverMove(cube, queue, RIGHT(cube->side));
			}
			addStackElement(FRONT(cube->side), setupStack);
			solverMove(cube, queue, FRONT(cube->side));
			cornerSwap(cube, queue);
			while(setupStack->start != NULL) {
				Move reverting = popStackElement(setupStack);
				solverMove(cube, queue, reverting);
			}
		} else if (EQ(buffer->colors.left, BLUE)) {
			if (buffer->correctPos.z == 0) {
				addStackElement(RIGHT(cube->side), setupStack);
				solverMove(cube, queue, RIGHT(cube->side));
				if (buffer->correctPos.y == 2) {
					addStackElement(DOWN_PRIME(cube->side), setupStack);
					solverMove(cube, queue, DOWN_PRIME(cube->side));
				}
			} else if (buffer->correctPos.z == 2 && buffer->correctPos.y == 0) {
				addStackElement(DOWN(cube->side), setupStack);
				addStackElement(FRONT_PRIME(cube->side), setupStack);
				solverMove(cube, queue, DOWN(cube->side));
				solverMove(cube, queue, FRONT_PRIME(cube->side));
			} else {
				end = true;
			}
			if (!end) {
				cornerSwap(cube, queue);
				while(setupStack->start != NULL) {
					Move reverting = popStackElement(setupStack);
					solverMove(cube, queue, reverting);
				}
			}
		} else if (EQ(buffer->colors.left, ORANGE)) {
			if (buffer->correctPos.x == 2) {
				addStackElement(FRONT_PRIME(cube->side), setupStack);
				solverMove(cube, queue, FRONT_PRIME(cube->side));
				if (buffer->correctPos.y == 2) {
					addStackElement(DOWN(cube->side), setupStack);
					solverMove(cube, queue, DOWN(cube->side));
				}
			} else if (buffer->correctPos.y == 0 && buffer->correctPos.x == 0) {
				addStackElement(DOWN_PRIME(cube->side), setupStack);
				addStackElement(RIGHT(cube->side), setupStack);
				solverMove(cube, queue, DOWN_PRIME(cube->side));
				solverMove(cube, queue, RIGHT(cube->side));
			} else {
				end = true;
			}
			if (!end) {
				cornerSwap(cube, queue);
				while(setupStack->start != NULL) {
					Move reverting = popStackElement(setupStack);
					solverMove(cube, queue, reverting);
				}
			}
		} else {
			if (buffer->correctPos.z == 0) {
				addStackElement(RIGHT(cube->side), setupStack);
				addStackElement(RIGHT(cube->side), setupStack);
				solverMove(cube, queue, RIGHT(cube->side));
				solverMove(cube, queue, RIGHT(cube->side));
				if (buffer->correctPos.x == 2) {
					addStackElement(DOWN_PRIME(cube->side), setupStack);
					solverMove(cube, queue, DOWN_PRIME(cube->side));
				}
			} else if (buffer->correctPos.z == 2 && buffer->correctPos.x == 2) {
				addStackElement(FRONT(cube->side), setupStack);
				addStackElement(FRONT(cube->side), setupStack);
				solverMove(cube, queue, FRONT(cube->side));
				solverMove(cube, queue, FRONT(cube->side));
			} else {
				end = true;
			}
			if (!end) {
				cornerSwap(cube, queue);
				while(setupStack->start != NULL) {
					Move reverting = popStackElement(setupStack);
					solverMove(cube, queue, reverting);
				}
			}
		}
		if (freeNext) {
			free(buffer);
			freeNext = false;
		}
		if (end) {
			buffer = findWrongCornerPiece(cube);
			freeNext = true;
			if (buffer == 0) {
				break;
			}
		} else {
			buffer = ELEMENTAT(cube, 0, 2, 2);
		}
	}
	destroyCube(cube);
	freeStack(setupStack);
}
