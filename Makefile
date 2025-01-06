
lexertest: lexertest.c lexertest.h
	gcc -g -DDEBUG -O -c lexertest -o lexertest.o

lexer: lexer.c lexer.h
	gcc -g -DDEBUG -O -c lexer.c -o lexer.o

test: lexer lexertest aplib_pkg
	gcc -g -DDEBUG -O aplibpkg.a -o test.exe

aplib_pkg:
	make ../ aplib
	make ../ colour
	make ../ stringy
	make ../ io
	make ../ lib
	make ../gcollect/ gcd

	ar -r ../aplib.o ../stringy.o ../colour.o ../lib.o ../io.o ../gcollect/gcd.o lexer.o lexertest.o -o aplibpkg.a

clean:
	rm -f *.o
	rm -f *.a
	rm -f test.exe

