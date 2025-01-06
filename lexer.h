// SIMPLE_LEXER_H
#ifndef DAVELIB_SIMPLE_LEXER_H
#define DAVELIB_SIMPLE_LEXER_H

// GLOBALS

#ifdef strcmp
#define strcmp_bkp strcmp
#undef strcmp
#endif

#define strcmp strcmp__

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

// CSTNode for NonTerminals.
typedef struct GrammarUnit	{

	int num_tokens; // from lexer
	char** tokens;

} GrammarUnit;

typedef struct CSTNode	{

	char* nodeName; // OPTIONAL ID-STRING, NONTERMINAL DESIGNATION.

	void* ancestor; 	// typeof( struct CSTNode* ), back-reference

	char* termStr;
	signed isTerminal; 
	// if this is FAlSE, 0, then the below struct members must be populated, as the
	// descendents will be nonterminals.

	void** descendents;
	signed numDescendents;

}	CSTNode;



unsigned strcmp__( char* str1, char* str2 )	{

	while( (*str1++ != 0) || (*str2++ != 0) )
		if( *str2 == *str1 )
			continue;
		else
			return 0;

	if( *str2 == *str1 == 0 )
		return +1;
	
	return 0;
}

char** split( char* line, char delim );


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

