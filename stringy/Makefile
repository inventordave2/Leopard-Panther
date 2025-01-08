# Makefile for stringy

# Vars.
stringyFiles=stringy.c stringy.h
debugTags=-g -DDEBUG


.PHONY: clean
stringy: $(stringyFiles)
	gcc -O3 -c -o stringy.o  stringy.c

stringyd: $(stringyFiles)
	gcc $(debugTags) -O0 -c -o stringyd.o  stringy.c

clean:
	rm -f *.o

