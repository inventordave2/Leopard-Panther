#ifndef _DAVEFILEETC_
#define _DAVEFILEETC_

typedef struct FileContents {

	int lineCount;
	char** lines;

} FileContents;

typedef struct FileContext	{

	char* fileContents;
	int length;

	FILE* f;

} FileContext;


struct FileContext readFile( char* fn );
struct FileContents read_f_split( char* fn, char* delim );

char** split( char* line, char delim );
char* getline_file( char* fn, int lineNum );

#endif

