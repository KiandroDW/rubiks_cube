all:
	gcc src/*.c -I include/ -lraylib -lm

run: all
	./a.out

.PHONEY: all run
