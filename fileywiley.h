// FILEYWILEY_H

#ifndef DAVELIB_FILEYWILEY_H
#define DAVELIB_FILEYWILEY_H

typedef struct FileContents {

	char* fn;
	int lineCount;
	char** lines;

} FileContents;

typedef struct FileContext	{

	char* fn;
	char* fileContents;
	int length;

} FileContext;

typedef struct FileyWiley	{
	
	struct FileContext (*readFile)( char* fn );
	struct FileContents (*read_f_split)( char* fn, char delim );
	
	char* (*getline_file)( char* fn, int lineNum );
	
	
} FileyWiley;

extern struct FileyWiley* fileywiley;
void InitFileyWiley();

#endif

