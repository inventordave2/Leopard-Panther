# Makefile for Lexer & Parser, written in C. Bison/Flex-free.

# Vars.
CC=gcc
DEBUG=-g -DDEBUG
OPTIM=-O0

.PHONY: clean clean-all


regex: regex.c regex.h
	$(CC) $(DEBUG) $(OPTIM) -c regex.c -o regex.o
	
filemethods: file.c file.h
	$(CC) $(DEBUG) $(OPTIM) -c file.c -o filemethods.o

lexertest: lexertest.c lexertest.h
	$(CC) $(DEBUG) $(OPTIM) -c lexertest.c -o lexertest.o

lexer: lexer.c lexer.h
	$(CC) $(DEBUG) $(OPTIM) -c lexer.c -o lexer.o

parser: parser.c parser.h
	$(CC) $(DEBUG) $(OPTIM) -c parser.c -o parser.o

test: lexandparsepkg
	$(CC) $(DEBUG) $(OPTIM) regex.o filemethods.o lexer.o parser.o lexertest.o ./../stringy/stringyd.o ./../colour/colourd.o ./../wernee/regex_w/regexd.o  -o test.exe


lexandparsepkg: lexer parser lexertest filemethods regex

	make -C ./../colour colourd
	make -C ./../stringy stringyd
	make -C ./../wernee/regex_w libd 

clean:
	rm -f *.o
	rm -f *.a
	rm -f test.exe
	
clean-all: clean
	make -C ./../colour/ clean
	make -C ./../stringy/ clean
	
