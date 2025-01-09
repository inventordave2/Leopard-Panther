// SIMPLE_LEXER_H
#ifndef DAVELIB_SIMPLE_LEXER_H
#define DAVELIB_SIMPLE_LEXER_H

// GLOBALS

typedef struct LexInstance	{

	int TOK_TYPE;
	int TOK_REGEX;

	char* lexRulesFileName;
	char*** tokenRules;
	int numRules;

	char* sourceCodeFileName;
	char* sourceCode;
	int strlen_sourceCode;

	int carat;
	char*** tokens;
	int tokensCount;

	// init :  sizeof(char*) * n * max_num_segments * max_num entries in a segment
	char**** productionRules; //[][][]
	char* parseRulesFileName;
	int numProductionRules;

	int (*lex)( struct LexInstance* );
	

	struct ParserInstance* parser;



} LexInstance;


int extend( void* _ );

char*** initRuleSetArray( int numRules );
char*** initTokenResultsArray( int assumpt );


//// LEXER
// FUNCTIONS
struct LexInstance* initLex( char* sc, char* lr );
int lex( struct LexInstance* );
char* patternMatch( char* str, struct LexInstance* );
void push( char* token_type, char* literal, struct LexInstance* );
char* checkType( char* token, LexInstance* Lexer );




#endif // DAVELIB_SIMPLE_LEXER_H

