#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 

#include "./stringy/stringy.h"
#include "./colour/colour.h"

#include "lexer.h" 
#include "parser.h" 
#include "lexertest.h" 

int main( int argc, char** argv )	{

	ansivt = 1;
	colorMode();
	
	print( "%sWelcome to %sDaveLib%s's testversion of a %sC--%s lexer.%s\n", \
			FG_BRIGHT_BLUE, FG_YELLOW, FG_BRIGHT_BLUE, FG_BRIGHT_YELLOW, FG_BRIGHT_BLUE, NORMAL );

	char* sc;
	if( argc>1 )
		sc = getstring( argv[1] );
	else
		sc = "test.c";

	char* lr;
	if( argc>2 )
		lr = getstring( argv[2] );
	else
		lr = "c--.lex";
	
	char* teststr;
	if( argc>3 )	{
		teststr = getstring( "" );
		strcat( teststr, getstring(argv[3]) );
		strcat( teststr, "" );
		//getstring( argv[3] );
	}
	else
		teststr = NULL;"[0-9]+\\.[0-9]*";
	
	struct LexInstance* lexer = initLex( sc, lr );
	

	// QUICKTEST
	
	if( teststr != NULL )	{
		
		char* result = patternMatch( teststr, lexer );
		
		if( result!= NULL )
			printf( "result = %s\n", result );
		else
			printf( "result is NULL.\n" );
		exit( 0 );
	
	}
	//

	
	int success = lex( lexer );

	if( !success )	{
		
		printf( "Lexer is reporting failure to lex file '%s'.\n", sc );
	}
	else
		printf( "Lexer %sSUCCESSFULL!!%s\n", FG_BRIGHT_GREEN, NORMAL );

	int x;
	for( x=0; x<lexer->tokensCount; x++ )	{

		if( lexer->tokens[x][0]==NULL )
			break;
		
		printf( "%sToken Type: %s\nToken Literal: %s\n", lexer->tokens[x][0], lexer->tokens[x][1] );
		
		fflush( stdout );
	}

	return 0;
}

