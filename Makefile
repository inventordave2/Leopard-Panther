# Makefile for Lexer & Parser, written in C. Bison/Flex-free.

# Vars.
CC=./gcc
DEBUG=-g -DDEBUG
OPTIM=-O0

.PHONY: clean clean-all

regex: ./regex.c ./regex.h
	make -C ./../wernee/regex_w libd
	$(CC) $(DEBUG) $(OPTIM) -c ./regex.c -o ./regex.o

lexertest: ./lexertest.c ./lexertest.h
	$(CC) $(DEBUG) $(OPTIM) -c ./lexertest.c -o ./lexertest.o

lexer: stringycolorfiles regex ./lexer.c ./lexer.h
	$(CC) $(DEBUG) $(OPTIM) -c ./lexer.c -o ./lexer.o

parser: ./parser.c ./parser.h
	$(CC) $(DEBUG) $(OPTIM) -c ./parser.c -o ./parser.o

test: lexpkg
	$(CC) $(DEBUG) $(OPTIM) ./lexer.o ./lexertest.o  ./regex.o ./../fileywiley/fwd.o ./../colour/colourd.o ./../stringy/stringyd.o ./../wernee/regex_w/regexd.o -o ./test.exe

stringycolorfiles:
	make -C ./../colour colourd
	make -C ./../stringy stringyd 
	make -C ./../fileywiley/ fwd
	
lexpkg: lexer lexertest regex
	
clean:
	rm -f ./*.o
	rm -f ./*.a
	rm -f ./test.exe
	
clean-all: clean
	make -C ./../colour/ clean
	make -C ./../stringy/ clean
	make -C ./../fileywiley/ clean

