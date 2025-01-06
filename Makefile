# Makefile for Lexer & Parser, written in C. Bison/Flez-free.

# Vars.
CC=gcc
DEBUG=-g -DDEBUG
OPTIM=-O0

.PHONY: clean clean-all

lexertest: lexertest.c lexertest.h
	$(CC) $(DEBUG) $(OPTIM) -c lexertest.c -o lexertest.o

lexer: lexer.c lexer.h
	$(CC) $(DEBUG) $(OPTIM) -c lexer.c -o lexer.o

parser: parser.c parser.h
	$(CC) $(DEBUG) $(OPTIM) -c -o parser.o

test: lexer parser lexertest aplib_pkg
	$(CC) $(DEBUG) $(OPTIM) aplibpkg.a -o test.exe

aplib_pkg:
	make ../algae/ algae
	make ../ colour
	make ../ stringy
	make ../ io
	make ../ lib
	make ../ gcollect/ gcd

	ar -r ../aplib.o ../stringy.o ../colour.o ../lib.o ../io.o ../gcollect/gcd.o lexer.o parser.o lexertest.o -o aplibpkg.a

clean:
	rm -f *.o
	rm -f *.a
	rm -f test.exe
	
clean-all: clean
	make ../algae/		clean
	make ../colour		clean
	make ../stringy		clean
	make ../io			clean
	make ../lib			clean
	make ../gcollect/ 	clean 

