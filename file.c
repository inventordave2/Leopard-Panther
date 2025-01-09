// FILE_C

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "./stringy/stringy.h"
#include "file.h"


char** split( char* line, char delim )	{

	char** _;

	char* t = (char*) malloc( strlen( line )+1 );
	
	int x, y = 0, z = 0;
	for( x=0; x<strlen( line ); x++ )	{

		if( line[x] == delim )	{

			t[ z ] = '\0';
			_[ y++ ] = getstring( t );
			z = 0;
			
			continue;
		}

		t[ z++ ] = line[ x ];
	}

	if( z!=0 )
		t[ z ] = '\0';
	
	_[ y ] = NULL;

	return _;
}

char* getline_file( char* fn, int lineNum )	{

	// struct FileContents read_f_split( char* fn, char* delim );
	static struct FileContents fc;
	static char* fname = NULL;

	if( strcmp( fn, fname ) != 0 )	{

		fname = getstring( fn );
		fc = read_f_split( fname, "\n" );
	}

	return fc.lines[ lineNum ];
}



struct FileContents read_f_split( char* fn, char* delim )	{

	FILE* f;
	f = fopen( fn, "r" );
	fseek( f, 0, SEEK_END );
	int flen = ftell( f );
	fseek( f, 0, SEEK_SET );

	struct FileContents fc;
	fc.lineCount = 0;
	fc.lines = (char**) malloc( sizeof(char*) * (flen+1) );
	// char** fc->lines
	// int fc->lineCount

	char* line = (char*) malloc( 1024 );
	char c;
	int x, y;
	x = 0, y = 0;
	c = fgetc( f );

	while( c != EOF )	{

		if( c==delim[0] )	{

			line[x] = '\0';
			x = 0;
			fc.lines[y] = getstring( line );
			y++;
		}
		else
			line[x++] = c;

		line[ x ] = '\0';

		c = fgetc( f );
	}

	fclose( f );

	return fc;
}



struct FileContext readFile( char* fn )	{

	struct FileContext fileContext;

	char c;
	FILE* f;
	f = fopen( fn, "r" );
	fseek( f, 0, SEEK_END );
	int flen = ftell( f );
	fseek( f, 0, SEEK_SET );

	fileContext.fileContents = (char*) calloc( flen, sizeof(char) );

	int x = 0;
	c = fgetc( f );

	while( c != EOF )	{

		fileContext.fileContents[ x++ ]  = c;
		c = fgetc( f );
	}

	fileContext.fileContents[ x ] = '\0';
	fclose( f );

	fileContext.f = f;
	fileContext.length = flen;

	return fileContext;
}

