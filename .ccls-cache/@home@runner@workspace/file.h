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


extern struct FileContext readFile( char* fn );
extern struct FileContents read_f_split( char* fn, char* delim );

extern char** split( char* line, char delim );
extern char* getline_file( char* fn, int lineNum );

#endif

