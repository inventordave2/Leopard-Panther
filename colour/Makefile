# Makefile for 'colour' lib.

.PHONY: all test clean

all: colour test

colour: colour.c colour.h asciidefs.h
	gcc -g -DDEBUG -c colour.c -o colour.o

colourd: colour.c colour.h
	gcc -g -O0 -c colour.c -o colourd.o

test: colour testprogram.c testprogram.h
	gcc -g -DDEBUG -O0 colour.o testprogram.c -o testcolor.exe

clean:
	rm -f colour.o
	rm -f testcolor.exe

