// FILE_C

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "./../stringy/stringy.h"
#include "./fileywiley.h"

static struct FileContext readFile( char* fn );
static struct FileContents read_f_split( char* fn, char delim );
static char* getline_file( char* fn, int lineNum );


void InitFileyWiley()	{

	fileywiley = (struct FileyWiley*)malloc( sizeof( struct FileyWiley ) );
	fileywiley->readFile = readFile;
	fileywiley->read_f_split = read_f_split;
}



static struct FileContext readFile( char* fn )	{
	
	struct FileContext fc;
	fc.fileContents = (char*)calloc( 1, 1 );
	//char*,int,FILE*
	
	unsigned x = 0;
	
	FILE* f = fopen( fn, "r" );
	
	if( f!= NULL )	{
		
		if( fseek( f, 0, SEEK_END ) )	{
		
			fc.length = -1;
			fclose ( f );			
		}
		
		fc.length = ftell( f );
		
		fc.fn = fn;
		char* _ = (char*)malloc( fc.length+1 );
	
		fgets( _, fc.length, f );
	
		fclose( f );
	
		_[ fc.length ] = '\0';
		free( fc.fileContents );
		fc.fileContents = _;
	}
	
	return fc;
}

static struct FileContents read_f_split( char* fn, char delim )	{
	
	unsigned x = 0;
	unsigned y = 0;
	unsigned z = 0;
	unsigned w = 0;
	
	while( fn[x]!='\0' )	{
	
		z++;
		if( fn[x]==delim )	{
			
			y++;
			
			if( z>w )
				w = z;
			
			z = 0;
		}
		
		
		x++;
	}
	
	struct FileContents fc;
	fc.lineCount = y;
	
	fc.lines = (char**)malloc( y * sizeof(char*) );
	
	x = 0;
	y = 0;
	z = 0;
	
	fc.lines[z] = (char*)malloc( w+1 );
	char* line = fc.lines[z];
	
	while( fn[x]!='\0' )	{
		
		if( fn[x]!=delim )	{
			
			line[y] = fn[x];
			y++;
		}
		else	{
			
			line[y] = '\0';
			y = 0;
			line = fc.lines[z] = (char*)malloc( w+1 );
			z++;
		}
		
		x++;
	}
	
	return fc;
}

static char** split( char* line, char delim )	{

	char** _;

	char* t = (char*) malloc( strlen( line )+1 );
	
	int x, y = 0, z = 0;
	for( x=0; x<strlen( line ); x++ )	{

		if( line[x] == delim )	{

			t[ z ] = '\0';
			_[ y++ ] = stringy->getstring( t );
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

static char* getline_file( char* fn, int lineNum )	{

	// struct FileContents read_f_split( char* fn, char* delim );
	static struct FileContents fc;
	static char* fname = NULL;

	if( fname==NULL )
		fname = (char*)calloc( 1,1 );

	if( strcmp( fn, fname ) != 0 )	{

		free( fname );
		fname = NULL;
		
		fname = stringy->getstring( fn );
		fc = read_f_split( fname, '\n' );
	}

	return fc.lines[ lineNum ];
}

