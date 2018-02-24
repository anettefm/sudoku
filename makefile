CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -std=c99 -lm

run: sudoku
		./sudoku "test1.txt"

sudoku: sudoku.c
	$(CC)  $^ -o $@  $(CFLAGS)


.PHONY: clean debug

clean:
	rm -f $(BINARIES) *.o

debug: sudoku.c
	$(CC)  $^ -o $@  $(CFLAGS) -DDEBUG -g
