all:
	gcc src/*.c -I include/ -lraylib -lm src/solvers/blind.c

run: all
	./a.out

debug:
	gcc -g src/*.c -I include/ -lraylib -lm src/solvers/blind.c

.PHONEY: all run debug
