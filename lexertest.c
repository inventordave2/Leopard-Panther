#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 

#include "./../stringy/stringy.h"
#include "./../colour/colour.h"

#include "regex.h"
#include "lexer.h" 
#include "parser.h" 
#include "lexertest.h" 

int main( int argc, char** argv )	{

	ansivt = 1;
	colorMode();

	print( "%sWelcome to %sDaveLib%s's testversion of a %sC--%s lexer.%s\n", \
			FG_BRIGHT_BLUE, FG_YELLOW, FG_BRIGHT_BLUE, FG_BRIGHT_YELLOW, FG_BRIGHT_BLUE, NORMAL );

	char* sc = NULL;
	char* lexerrules = NULL;
	
	char* teststr = NULL;
	
	if( argc==1 )	{
	
		#ifndef	MAX_FILE_PATH_LENGTH
		#define MAX_FILE_PATH_LENGTH 261
		#endif
		
		char pstr[] = "%s#%s \0";
		char* prmpt = (char*) calloc( strlen(pstr)+1 , CODEPAGE_ATOMIC_WIDTH );

		sprintf( prmpt, pstr, FG_BRIGHT_YELLOW, NORMAL );
		printf( "Please enter the filename of a C or Javascript, or C-- source code file.\n" );
		char* in = (char*) calloc( MAX_FILE_PATH_LENGTH+1, CODEPAGE_ATOMIC_WIDTH );
		signed result = input( prmpt, in, MAX_FILE_PATH_LENGTH+1 );
		
		if( result==+2 )	{

			printf( "Exiting...\n" );
			exit(0);
		}
		
		if( result==+1 )	{
			
			sc = in;
		}

		printf( "Please enter the filename of a lex file for the tokenization process.\n" );
		in = (char*) calloc( MAX_FILE_PATH_LENGTH+1, CODEPAGE_ATOMIC_WIDTH );
		result = input( prmpt, in, MAX_FILE_PATH_LENGTH + 1 );
		
		if( result==+2 )	{
			
			printf( "Exiting...\n" );
			exit(0);
		}
		
		if( result==+1 )	{
			
			lexerrules = in;
		}
	}
		
	if( argc>1 )
		sc = getstring( argv[1] );

	if( argc>2 )
		lexerrules = getstring( argv[2] );

	
	struct LexInstance* lexer = initLex( sc, lexerrules );

	// QUICKTEST
	
	if( teststr != NULL )	{
		
		
		Token result = scanstring( teststr, lexer );
		
		if( result.pattern != NULL )
			printf( "result = %s\n", result.token_name );
		else
			printf( "result is NULL.\n" );
		exit( 0 );
	
	}

	int success = lex( lexer );

	if( !success )	{
		
		printf( "Lexer is reporting failure to lex file '%s'.\n", sc );
	}
	else
		printf( "Lexer %sSUCCESSFULL!!%s\n", FG_BRIGHT_GREEN, NORMAL );

	#define PRINT_TOKEN_SEQUENCE 0
	if( PRINT_TOKEN_SEQUENCE )	{
		
		int x;
		for( x=0; x<lexer->tokensCount; x++ )	{

			if( lexer->tokens[x][0]==NULL )
				break;
			
			printf( "%sToken Type%s: '%s'\n%sToken Literal%s: '%s'%s\n", FG_YELLOW, NORMAL, lexer->tokens[x][0], FG_BRIGHT_BLUE, NORMAL, lexer->tokens[x][1], NORMAL );
			
			fflush( stdout );
		}
	}
	return 0;
}

