# Makefile for Lexer & Parser, written in C. Bison/Flex-free.

# Vars.
CC=gcc
DEBUG=-g -DDEBUG
OPTIM=-O0

.PHONY: clean clean-all

regex: ./regex.c ./regex.h
	gnumake -C ./../regex_w/ libd
	$(CC) $(DEBUG) $(OPTIM) -c ./regex.c -o ./regex.o

lexertest: ./lexertest.c ./lexertest.h
	$(CC) $(DEBUG) $(OPTIM) -c ./lexertest.c -o ./lexertest.o

lexer: stringycolorfiles regex ./lexer.c ./lexer.h
	$(CC) $(DEBUG) $(OPTIM) -c ./lexer.c -o ./lexer.o

parser: ./parser.c ./parser.h
	$(CC) $(DEBUG) $(OPTIM) -c ./parser.c -o ./parser.o

test: lexpkg
	$(CC) $(DEBUG) $(OPTIM) ./file.o ./lexer.o ./lexertest.o ./regex.o ./../colour/colour_d.o ./../stringy/stringyd.o ./../regex_w/regexd.o -o ./test.exe

stringycolorfiles:
	gnumake -C ./../colour colour_d
	gnumake -C ./../stringy stringyd 
	$(CC) $(DEBUG) $(OPTIM) ./fileywiley.c -c -o file.o
	
	
lexpkg: lexer parser lexertest regex stringycolorfiles
	
clean:
	del -f ./*.o
	del -f ./*.a
	del -f ./test.exe
	
clean-all: clean
	gnumake -C ./../colour/ clean
	gnumake -C ./../stringy/ clean

