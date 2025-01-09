# Makefile for Lexer & Parser, written in C. Bison/Flez-free.

# Vars.
CC=gcc
DEBUG=-g -DDEBUG
OPTIM=-O0

.PHONY: clean clean-all

filemethods: file.c file.h
	$(CC) $(DEBUG) $(OPTIM) -c file.c -o filemethods.o

lexertest: lexertest.c lexertest.h
	$(CC) $(DEBUG) $(OPTIM) -c lexertest.c -o lexertest.o

lexer: lexer.c lexer.h
	$(CC) $(DEBUG) $(OPTIM) -c lexer.c -o lexer.o

parser: parser.c parser.h
	$(CC) $(DEBUG) $(OPTIM) -c parser.c -o parser.o

test: lexer parser lexertest aplibpkg
	$(CC) $(DEBUG) $(OPTIM)  aplibpkg.a  -o test.exe

aplibpkg: lexer parser lexertest

	make -C ./colour colourd
	make -C ./stringy stringyd

	ar -rcs aplibpkg.a ./stringy/stringyd.o ./colour/colourd.o lexer.o parser.o lexertest.o

clean:
	rm -f *.o
	rm -f *.a
	rm -f test.exe
	
clean-all: clean
	make -C ./colour/ clean
	make -C ./stringy/ clean

