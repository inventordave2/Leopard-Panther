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


