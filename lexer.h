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
	int filelen_expansion;

	int carat;
	char*** tokens;
	int tokensCount;
	
	int isControlSequence;

	// init :  sizeof(char*) * n * max_num_segments * max_num entries in a segment
	char**** productionRules; //[][][]
	char* parseRulesFileName;
	int numProductionRules;

	int (*lex)( struct LexInstance* );
	

	struct ParserInstance* parser;



} LexInstance;

extern int extend( void* _ );

extern char*** initRuleSetArray( int numRules );
extern char*** initTokenResultsArray( int assumpt );

//// LEXER
// FUNCTIONS
extern struct LexInstance* initLex( char* sc, char* lr );
extern int lex( struct LexInstance* );
extern Token scanstring( char* str, struct LexInstance* );
extern void push( char* token_type, char* literal, struct LexInstance* );
extern char* checkType( char* token, LexInstance* Lexer );

#endif // DAVELIB_SIMPLE_LEXER_H

