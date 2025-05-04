// REGEX_H

#ifndef LEXERPARSER_REGEX_H
#define LEXERPARSER_REGEX_H

#ifndef CODEPAGE_ATOMIC_WIDTH
#define CODEPAGE_ATOMIC_WIDTH 1
#endif


typedef struct Token	{
	
	char* literal;
	int length;
	
	char* pattern;
	char* token_name;
	
	unsigned long long int offset;
	char* source;
	
} Token;

extern Token GetEmptyToken();
extern void FreeToken( Token* );
extern int IsTokenEmpty( Token* );

extern Token MatchStringToPattern( char* pattern, char* str );

#endif

