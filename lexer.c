
#include <stdlib.h>
#include <stdio.h>
// LEXER & PARSER Includes. 
#include "./stringy/stringy.h"
#include "./colour/colour.h"
#include "lexer.h"
#include "parser.h"
#include "file.h"

// W STOOP'S REGEX LIBRARY
#include "regex_w/wregex.h"
#include "regex_w/wrx_prnt.h"

// LEXER:: Match substring from start to a regex-defined TOKEN_TYPE.
char* patternMatch( char* str, struct LexInstance* lexer )	{

	int e, ep;
	wregex_t *r;
	wregmatch_t *subm;

	char* pattern;
	char* token_type;

	int x = 0;
	while( x < lexer->numRules )	{

		token_type = lexer->tokenRules[x][0];
		pattern = lexer->tokenRules[x][1];

		r = wrx_comp(pattern, &e, &ep);

		if(!r) {

			/**
			fprintf(stderr,
				"\n[%s:%d] ERROR......: %s\n%s\n%d\n",
				lexer->sourceCodeFileName,
				lexer->carat,
				wrx_error(e),
				pattern,
				ep );
			*/
			
			fprintf( stderr, "%sUnable to generate Regex object from pattern: %s'%s'%s\n", FG_BRIGHT_RED, FG_BRIGHT_YELLOW, pattern, NORMAL );
			token_type = NULL;
			break;
		}

		if(r->n_subm > 0) {

			subm = calloc(sizeof *subm, r->n_subm);
			if(!subm) {

				fprintf(stderr, "Error: out of memory (submatches). Exiting Program.\n");
				wrx_free(r);
				exit(EXIT_FAILURE);
			}
		}
		else
			subm = NULL;
	
		e = wrx_exec(r, str, &subm, &r->n_subm);

		if(e < 0)
			fprintf(stderr, "Error: %s\n", wrx_error(e));
		
		if( e > 0 )
			break;
		
		free(subm);
		wrx_free(r);

		x++;
	}
	
	if( e==0 )
		token_type = NULL;
	// If we got here, wregex has either matched the pattern to the rule, or has failed to find a match in the Ruleset.
	return token_type;
}

char* checkType( char* token, LexInstance* Lexer )	{

	char* type = getstring( "NT" );

	int i;
	for( i=0; i<Lexer->numRules; i++ )
		if( strcmp( token, Lexer->tokens[i][1] )==0 )
			return ++type;

	return type;
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
		str += 1; // (to pass over the delimeter char)
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
	char* match;
	char* match_bkp;
	
	for( i=0; i<lexer->strlen_sourceCode; i++ )	{

		lexer->carat = i;
		
		c = lexer->sourceCode[i];
		
		/**
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

			case '\\':
				lexer->filelen_expansion++;
				lexer->isControlSequence = 1;
				_[k++] = '\\';
				c = '\\';
				break;
				
			default:
				lexer->isControlSequence = 0;
				break;
		}
		*/
		_[k++] = c;
		_[k] = '\0';

		if( !strcmp( _, " " ) )	{

			match = getstring( "SPACE" );
			k = 0;
			push( match, _, lexer );
			continue;
		}
		
		else if( (match = patternMatch( _,lexer ))!=NULL )	{

			error = 0;
			match_bkp = match;
			continue;
		}
		else	{
			
			if( error==1 ) // no match from substring yet.
				continue;

			error = 1;
			
			if( lexer->isControlSequence )	{
				
				--k;
			}
			
			_[ --k ] = '\0';
			k = 0;
			i--;
			
			push( match_bkp, _, lexer );
			match_bkp = NULL;
			continue;
		}
	}

	// filelen_expansion will now hold an amount of strlen_sourceCode expansion through expanding whitespace control-codes from 1 narrow char
	// to two. As these do not require extra storage space visa vie the lexer->sourceCode buffer, and lexer-carat is based on the sourceFile
	// string, I will just keep this tracker for
	
	if( match_bkp != NULL )	{
		
		push( match_bkp, _, lexer );
	}
	
	return !error;
}

void push( char* token_type, char* literal, struct LexInstance* lexer )	{

	char** entry = (char**)malloc( sizeof(char*) * 2 );
	
	char* type = getstring( token_type );
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
	lexInstance->filelen_expansion = 0; // tracks expansion of whitespace narrow chars in source file (\t\n\r) to 2 printable chars '\\' and [rnt]

	lexInstance->tokens = (char***) calloc( lexInstance->strlen_sourceCode, 2 * sizeof(char*) );
	lexInstance->tokensCount = 0;
	lexInstance->lex = lex;

	unsigned max_num_rules = 512;
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
	for( i=0; i<fc_lex.lineCount; i++ )	{

		lexInstance->tokenRules[i] = (char**) calloc( 2, sizeof(char*) );

		line = fc_lex.lines[i];
		//tokenRules[i][TOK_TYPE] = strtok(line, "\t");
		
		found = strtok( line," \t\r\n");
		
		if( found==NULL)
		{
			break;
		}
		
		lexInstance->tokenRules[i][lexInstance->TOK_TYPE] = getstring( found );

		found = strtok( NULL, " \t\r\n" );

		if( found==NULL )	{

			lexInstance->tokenRules[i][lexInstance->TOK_TYPE] = NULL;
			break;
		}

		lexInstance->tokenRules[i][lexInstance->TOK_REGEX] = getstring( found );
	
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

