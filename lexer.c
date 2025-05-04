#include <stdlib.h>
#include <stdio.h>
// LEXER & PARSER Includes. 
#include "./../stringy/stringy.h"
#include "./../colour/colour.h"
#include "./../fileywiley/fileywiley.h"

#include "regex.h"
#include "lexer.h"
#include "parser.h"

// W STOOP'S REGEX LIBRARY
#include "./../wernee/regex_w/wregex.h"
#include "./../wernee/regex_w/wrx_prnt.h"

#define LEXERDIAGNOSTICSMSG( msgCode )

// Match substring from start to a regex-defined TOKEN_TYPE.
Token scanstring( char* str, struct LexInstance* ilexer )	{

	static struct LexInstance* lexer = NULL;
	static char* sc = NULL;
	static char* lexrules = NULL;
	
	if( !ilexer )	{
		
		lexer = initLex( sc, lexrules );
		if( !lexer )	{
			
			LEXERDIAGNOSTICSMSG( INIT_LEXER_FAILED )
			return GetEmptyToken();
		}
	}
	else
		lexer = ilexer;
	
	
	int e, ep;
	wregex_t *r;
	wregmatch_t *subm;

	Token token;

	int x = 0;
	while( x < lexer->numRules )	{


		token = MatchStringToPattern( lexer->tokenRules[x][1], str );

		if( token.pattern!=NULL )
			break;

		x++;
	}
	
	if( token.pattern!=NULL )
		token.token_name = getstring( lexer->tokenRules[x][0] );

	// If we got here, wregex has either matched the pattern to the rule, or has failed to find a match in the Ruleset.
	return token;
}

char** getStringList( char* str, char* pattern )	{

	int e, ep, x;
	wregex_t *r;
	wregmatch_t *subm;

	char** stringList = (char**) calloc( sizeof(char*), 32 );

	e = 0;
	ep = 0;
	x = 0;
	while( 1 )	{

		if( (e < 1) || (x == 32) )
			break;

		r = wrx_comp(pattern, &e, &ep);

		if(!r) {

			fprintf(stderr,
				"%sError attempting to generate a WREGEX context:%s\n\
				msg: '%s',\n\
				pattern: '%s',\n\
				ep: '%d'\n",
				FG_BRIGHT_RED,
				NORMAL,
				wrx_error(e),
				pattern,
				ep );

			break;
		}

		if(r->n_subm > 0) {

			subm = calloc(sizeof *subm, r->n_subm);
			if(!subm) {

				fprintf(stderr,
					"Error: out of memory (for storing submatches). Exiting Program at line '%d' in file '%s'.\n",
					__LINE__+4,
					__FILE__ );
				
				wrx_free(r);
				exit(EXIT_FAILURE);
			}
		}
		else
			subm = NULL;
	
		e = wrx_exec(r, str, &subm, &r->n_subm);

		if( e > 0 )	{

			char* _ = (char*)malloc( subm[1].end - subm[1].beg + 1 );
			strncpy( _, subm[1].beg, subm[1].end - subm[1].beg );
			_[ subm[1].end - subm[1].beg ] = '\0';
			stringList[x] = _;
		}
		else if(e < 0)
			fprintf(stderr, "Error: %s\n", wrx_error(e));
		
		if( subm != NULL )
			free( subm );

		wrx_free( r );
		str += strlen( stringList[x] );
		x++;
	}

	return stringList;
}

// LEXER::LEX() : [in] A pointer to a LexInstance Context.
int lex( struct LexInstance* lexer )	{

	int i;
	int k = 0;
	char c;
	
	int error = 1;
	char* _ = (char*) calloc( 256, 1 );
	
	Token token;
	Token match_bkp = GetEmptyToken();
	
	for( i=0; i<lexer->strlen_sourceCode; i++ )	{

		lexer->carat = i;
		
		c = lexer->sourceCode[i];

		switch( c )	{
		
			case '\r':
				lexer->filelen_expansion++;
				lexer->isControlSequence = 1;
				_[k++] = '\\';
				c = 'r';
				break;
				
			case '\n':
				lexer->filelen_expansion++;
				lexer->isControlSequence = 1;
				_[k++] = '\\';
				c = 'n';
				break;
				
			case '\t':
				lexer->filelen_expansion++;
				lexer->isControlSequence = 1;
				_[k++] = '\\';
				c = 't';
				break;
				
			case '\e':
				lexer->filelen_expansion++;
				lexer->isControlSequence = 1;
				_[k++] = '\\';
				c = 'e';
				break;

			default:
				lexer->isControlSequence = 0;
				break;
		}
		
		_[k++] = c;
		_[k] = '\0';
		
		if( k>64 )
			fprintf( stderr, "The string being populated at '%s':%d %s('%s')%s has reached 64 glyphs.\n", __FILE__, __LINE__, FG_BRIGHT_BLUE, _, NORMAL ), exit(0); 
		token = scanstring( _,lexer );
		
		if( token.pattern != NULL )	{

			error = 0;
			FreeToken( &match_bkp );
			match_bkp = token;
			
			continue;
		}
		else	{
			
			if( error==0 )	{
				
				error = 1;
				_[k-1] = '\0';
				push( match_bkp.token_name, getstring( _ ), lexer );
				k = 0;
				--i;
				continue;
			}
		}

	}

	// filelen_expansion will now hold an amount of strlen_sourceCode expansion through expanding whitespace control-codes from 1 narrow char
	// to two. As these do not require extra storage space visa vie the lexer->sourceCode buffer, and lexer-carat is based on the sourceFile
	// string, I will just keep this tracker for metadata.
	
	return !error;
}

void push( char* token_name, char* literal, struct LexInstance* lexer )	{

	char** entry = (char**)malloc( sizeof(char*) * 2 );
	
	char* type = getstring( token_name );
	char* lit = getstring( literal );
	
	entry[0] = type;
	entry[1] = lit;
	
	lexer->tokens[lexer->tokensCount] = entry;

	lexer->tokensCount++;

	return;
}

struct LexInstance* initLex( char* sc, char* lr )	{

	// HELPER VARS.
	struct FileContext fc_source = readFile( sc );
	struct FileContents fc_lex = read_f_split( lr, "\n" );

	// GENERATE LEXER INSTANCE STATE.
	struct LexInstance* lexInstance = (struct LexInstance*) calloc( 1, sizeof(LexInstance) );
	lexInstance->TOK_TYPE = 0;
	lexInstance->TOK_REGEX = 1;

	lexInstance->lexRulesFileName = lr;
	lexInstance->tokenRules = (char***) calloc( fc_lex.lineCount, 2 * sizeof(char*) );
	lexInstance->numRules = fc_lex.lineCount;
	
	lexInstance->sourceCodeFileName = sc;
	lexInstance->sourceCode = fc_source.fileContents;
	lexInstance->strlen_sourceCode = fc_source.length;
	lexInstance->filelen_expansion = 0; // tracks expansion of narrow chars in source file (such as \e\t\n\r) to 2 printable chars '\\' and [rnt]

	lexInstance->tokens = (char***) calloc( lexInstance->strlen_sourceCode, 2 * sizeof(char*) );
	lexInstance->tokensCount = 0;
	lexInstance->lex = lex;

	unsigned max_num_rules = 65536;
	unsigned max_num_segments = 64;
	unsigned max_num_entries_in_a_segment = 32;
	
	lexInstance->productionRules = (char****) calloc( sizeof(char*), max_num_rules * max_num_segments * max_num_entries_in_a_segment );
	// char**** productionRules; //[][][]
	// [ruleNum][segmentNum][entryInSegment]
	

	// POPULATE THE TOKENIZER RULESET.
	int offset;
	char* line;
	char* found;
	int i;
	void* ptr;
	Token tokenData = GetEmptyToken();
	
	for( i=0; i<fc_lex.lineCount; i++ )	{

		lexInstance->tokenRules[i] = (char**) calloc( 2, sizeof(char*) );

		line = fc_lex.lines[i];
		//tokenRules[i][TOK_TYPE] = strtok(line, "\t");
		

		tokenData = MatchStringToPattern( "^[A-Za-z]+[A-Za-z_0-9]*", line );
		lexInstance->tokenRules[i][lexInstance->TOK_TYPE] = getstring( tokenData.literal );
		
		line += tokenData.length;
		FreeToken( &tokenData );
		
		char* rgxpatt = trim( line );
		
		lexInstance->tokenRules[i][lexInstance->TOK_REGEX] = rgxpatt;
	}

	// LEXINSTANCE prepared. Return the Active Lex Instance.
	return lexInstance;
}

int extend( void* self )	{

	struct CSTNode* node = (struct CSTNode*) self;

	int success = 0;
	int numEntries;
	int newNumOfEntries = 1;
	if( success )
		numEntries = newNumOfEntries;

	return success;
}

