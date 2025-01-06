// LEXERTEST_C

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../stringy.h"
#include "../colour.h"
#include "../gcollect/gc.h"
#include "lexer.h"

#ifndef report
#define report printf
#endif
#ifndef print
#define print printf
#endif

#include "lexertest.h"
int main( int argc, char** argv )	{

	volatile struct GC* gc = initGC( 100 );
	colorMode();
	
	print( "%sWelcome to %sDaveLib%s's testversion of a %sC--%s lexer.%s\n", FG_BRIGHT_BLUE, FG_YELLOW, FG_BRIGHT_BLUE, FG_BRIGHT_YELLOW, FG_BRIGHT_BLUE, NORMAL );

	char* sc;
	if( argc>1 )
		sc = getstring( argv[1] );
	else
		sc = "lexer.c";

	char* lr;
	if( argc>2 )
		lr = getstring( argv[2] );
	else
		lr = "c--.lex";
	
	struct LexInstance* lexer = initLex( sc, lr );
	
	int success = lex( lexer );

	if( !success )	{
	report( "Lexer is reporting failure to lex file %s.\n", sc );
	
	int x;
	for( x=0; x<lexer->tokensCount; x++ )	{

		if( lexer->tokens[x][0]==NULL )
			break;
		
		report( "%sToken Type: %s%s\n%sToken Literal: %s%s%s\n", FG_BRIGHT_YELLOW, FG_BRIGHT_GREEN, FG_BRIGHT_YELLOW, lexer->tokens[x][0], \
				FG_BRIGHT_GREEN, lexer->tokens[x][1], NORMAL );
	}

	return 0;
}

