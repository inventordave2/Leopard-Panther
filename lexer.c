// DAVELIB_LEXER_C

#include "../davelib/io.h"
#include "../stringy/stringy.h"
#include "../davelib/lib.h"
#include "../colour/colour.h"

// LEXER & PARSER Includes. 
#include "lexer.h"
#include "parser.h"

// W STOOP'S REGEX LIBRARY
#include "../wernee/regex_w/wregex.h"
#include "../wernee/regex_w/wrx_prnt.h"

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

			fprintf(stderr,
				"\n[%s:%d] ERROR......: %s\n%s\n%d\n",
				lexer->sourceCodeFileName,
				lexer->carat,
				wrx_error(e),
				pattern,
				ep );

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
		
		if( e <= 0 )
			token_type = NULL;
		else
			break;
		
		free(subm);
		wrx_free(r);

		x++;
	}
	
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

struct FileContents read_f_split( char* fn, char* delim )	{

	FILE* f;
	f = fopen( fn, "r" );
	fseek( f, 0, SEEK_END );
	int flen = ftell( f );
	fseek( f, 0, SEEK_SET );
	
	struct FileContents fc;
	fc.lineCount = 0;
	fc.lines = (char**) malloc( sizeof(char*) * (flen+1) );
	// char** fc->lines
	// int fc->lineCount

	char* line = (char*) malloc( 1024 );
	char c;
	int x, y;
	x = 0, y = 0;
	c = fgetc( f );
	
	while( c != EOF )	{

		if( c==delim[0] )	{
		
			line[x] = '\0';
			x = 0;
			fc.lines[y] = getstring( line );
			y++;
		}
		else
			line[x++] = c;

		line[ x ] = '\0';

		c = fgetc( f );
	}

	fclose( f );

	return fc;
}





char* getline_file( char* fn, int lineNum )	{

	// struct FileContents read_f_split( char* fn, char* delim );
	static struct FileContents fc;
	static char* fname = NULL;

	if( strcmp( fn, fname ) != 0 )	{

		fname = getstring( fn );
		fc = read_f_split( fname, "\n" );
	}
	
	return fc.lines[ lineNum ];
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

		if( e > 0 )
			stringList[x] = subm[1];
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
	
	for( i=0; i<lexer->strlen_sourceCode; i++ )	{

		lexer->carat = i;
		
		c = lexer->sourceCode[i];
		_[k++] = c;
		_[k] = '\0';
		
		if( (match = patternMatch( _,lexer ))==NULL )	{

			error = 1;
			continue;
		}
		else	{
			
			k = 0;
			error = 0;
			push( match, _, lexer );
		}
	}

	return !error;
}

void push( char* token_type, char* literal, struct LexInstance* lexer )	{

	lexer->tokens[lexer->tokensCount][0] = getstring( token_type );
	lexer->tokens[lexer->tokensCount][1] = getstring( literal );

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
	for( i=0; i<fc_lex.lineCount; i++ )	{

		line = fc_lex.lines[i];
		//tokenRules[i][TOK_TYPE] = strtok(line, "\t");
		
		found = strtok( line,"\t");
		if( found==NULL)
		{
			printf( "Invalid format for '%s' lex file, @ line '%d'.\n", lr, i+1 );
			return lexInstance;
		}
		else
			lexInstance->tokenRules[i][lexInstance->TOK_TYPE] = getstring( found );

		offset = strlen( found );
		free( found );
		
		line += offset;
		lexInstance->tokenRules[i][lexInstance->TOK_REGEX] = getstring( line );

		line -= offset; // need to reset ptr to allocated location before freeing.
		free( line );
	}

	// LEXINSTANCE prepared. Return the Active Lex Instance.
	return lexInstance;
}

int Parse( struct LexInstance* lexer	)	{

	int x, x2, x3, x4;
	int flag;

	char* line;
	char** segments;
	char** terms;
	char* term;
	
	char* ruleName;
	char* prRule;

	char* prsegment;
	char* token_type;
	char* token;
	
	// lexInstance->productionRules = (char****) calloc( sizeof(char*), max_num_rules * max_num_segments * max_num_entries_in_a_segment );
	// char**** productionRules; //[][][]
	// [ruleNum][segmentNum][entryInSegment]

	int max_num_rules = 512;
	int max_num_segments = 64;
	int max_num_entries_in_a_segment = 32;
	
	for( x=0; x<max_num_rules; x++ )	{

		line = getline_file( lexer->parseRulesFileName,x );
		ruleName = match_string( "^([a-zA-Z_0-9]+)\\:", line );
		lexer->productionRules[x][0][0] = getstring( ruleName );
		free( ruleName );

		segments = split( line, '|' );

		for( x2=0; x2<max_num_segments; x2++ )	{

			terms = split( segments[ x2 ], ' ' );
			
			for( x3=0; x3<max_num_entries_in_a_segment; x3++ )	{

				term = terms[ x3 ];
				lexer->productionRules[x][x2][x3] = term;
				free( term );
			}

			free( terms );
		}

		free( line );
		free( segments );
	}
				
	// INIT
	getNextProductionRuleSegment( NULL );

	// FOR_EACH lexed token in the Lexer stream.
	
	for( x=0; x<lexer->tokensCount; x++ )	{

		
		token_type = lexer->tokens[x][0];
		token = lexer->tokens[x][1];
		
		// if the PR entry is a non-terminal, we need to recursively stack (LIFO), until we find a terminal definition.
		// if that terminal definition matches an entry in a production-rule segment, we need to wait until the full production-rule section
		// is matched by sequential tokens.

		section_scan:

		prsegment = getNextProductionRuleSegment( lexer );
		
		if( prsegment == NULL )	{
			
			// out of production rule segments.
			break;
		}
		
		unsigned y=0;
		char* _ = prsegment[0];

		if( _ == NULL )	{
			
			// This edge case shouldn't arise, but it constitutes a production rule nonterminal/terminal decleration at the
			// beginning of a production rule section/segment that is NULL.

			fprintf( stderr, "%s%sDEBUG MSG:%s %sAn Edge Case has arisen. A production rule has a NULL nonterminal/terminal decleration \
				at the beginning of one of it's segments. Rule Name: %s'%s%s%s'%s.\n", BG_GREEN, FG_BRIGHT_YELLOW, NORMAL, FG_BRIGHT_BLUE,\
				FG_WHITE, FG_BRIGHT_GREEN, *getNextProductionRuleSegment( (void*)1 ), FG_WHITE, NORMAL );
			
			break;
		}

		x2 = x;
		flag = 1; // assume match will be found in section/segment.
		while( _ != NULL )	{

			if( strcmp(_,token_type) )	{

				flag = 0; // match not found.
				x2 = x;
				break;	  // skip to next section
			}
			else	{

				// token match to token_type in current prSegment.
				_ = prsegment[ ++y ];


				token_type = lexer->tokens[++x2][0];		
				token = lexer->tokens[++x2][1];
						
				continue;
			}
		}
		
		if( flag!=1 )	{	
			// unable to match to entry (token) in segment.
			goto section_scan;
		}

		// ELSE, reset PoductionRules' static scanner, set latest PR-type, and continue anew from next token in stream.
		prsegment = getNextProductionRuleSegment( NULL );

		char*** collection = (char***) malloc( sizeof(char*) * (x2-x) * 2 );
		unsigned j;
		for( j=x; j<x2; j++ )	{

			collection[j-x] = lexer->tokens[j];
		}

		prRule = getNextProductionRuleSegment( (void*)1 );
		
		PushParserStack( prRule, collection, j+1, lexer->parser );

		x = x2;
		flag = 0;

		// tautological, included for clarity.
		continue;
	}

	if( lexer->tokensCount != x )	{

		fprintf( stderr, "%squickparse failed to complete parsing of source file '%s' at token '%d'%s\n", \
			FG_BRIGHT_RED, lexer->sourceCodeFileName, x-1, NORMAL );
		return 0;
	}

	printf( "%sHuzzah! Quickparse completed parsing of source file '%s'%s\n.", FG_GREEN, lexer->sourceCodeFileName, NORMAL );
	return 1;
}

char** split( char* line, char delim )	{

	int i, k;

	char** results = (char**) calloc( sizeof(char*), 32 );
	int strlen_line = strlen( line );

	int in_quotes = 0;
	int in_singlequotes = 0;
	int escaped = 0;
	k=0; 
	for( i=0; i<strlen_line; i++ )	{

		if( (line[i] == delim) && (escaped==0) && (in_quotes==0) )			{

			line[i] = '\0';
			results[k++] = getstring( line );
			line[i] = delim;
			line += (i+1);
			i = 0;
			strlen_line = strlen( line );

			continue;
		}

		if( line[i] == '"' &&escaped==0 )
			in_quotes==0 ? in_quotes=1 : in_quotes=0;
		
		if( line[i] == '\'' && escaped==0 && in_singlequotes==1 )
			in_singlequotes=0;
		else if( line[i] == '\'' && escaped==0 && in_singlequotes==0 )
			in_singlequotes=1;

		if( line[i] == '\'' && in_singlequotes==1 )
				in_singlequotes=0;

		if( escaped==1 )
			escaped=0;
		else if( line[i] == '\\' )
			escaped=1;
 
	}

	return results;
}

struct ParserInstance* InitParserInstance(void)	{
	struct ParserInstance* parser = (struct ParserInstance*) calloc( 1, sizeof(struct ParserInstance) );
	
	parser->lexer = NULL;
	parser->parse = NULL;

	parser->Root = NULL;

	parser->AddNode = AddNode;
	
}

void PushParserStack( char* prRule, char*** collection, int amount 
, struct ParserInstance* parser )	{

	struct CSTNode* node = initNode( getstring(prRule) );
	
	node->descendents = (struct CSTNode**) malloc( sizeof(struct CSTNode*) * amount );
	node->numDescendents = amount;

	char* _;
	struct CSTNode* subNode;
	int x, y;
	x=0, y=0;
	for( x=0; x<amount; x++ )	{

		char* tt = collection[x][0];
		subNode = createNode( tt );

		if( checkType( tt )=="T" )	{

			#ifndef TOKEN_LITERAL
			#define TOKEN_LITERAL 1
			#endif
			// Terminal.
			// This has the side-effect that if any
			// leaf nodes upon parse completion
			// are not Terminals, the parse
			// has failed.

			subNode->isTerminal = 1;
			subNode->termStr = getstring( collection[x][1] );
			
		}
		else	{
			// NonTerminal.
			subNode->isTerminal = 0;
		}

		//subNode->ancestor = node;
		//node->descendents[x] = subNode;
		AddNode( subNode, node );
	}

	return;
}

char** getNextProductionRuleSegment( struct LexInstance* lexer )	{

	static unsigned x = 0;
	static unsigned y = 1;
	static char* prRule = NULL;
	
	char** prSegment;

	if( lexer==NULL )	{
		// reset statics.
		x = 0, y = 1;

		if( prRule != NULL )
			free( prRule );
		
		prRule = getstring( "" );
		return NULL;
	}

	if( lexer==(void*)1 )	{

		// return prRule;		
		return &prRule;
	}
	
	

	if( !!strcmp( prRule,lexer->productionRules[x][0][0] ) )
		prRule = lexer->productionRules[x][0][0];

	checkAgain:
	
	// char**** productionRules [n] [segment] [each NT/T type]
	
	
	prSegment = lexer->productionRules[x][y++];
	
	if( prSegment == NULL )	{
	// no more production rules to provide a segment from.

		x = 0;
		y = 1;

		if( prRule != NULL )	{
			
			free( prRule );
			prRule = NULL;
		}
		
		return NULL;
	}
	
	if( prSegment[0] == NULL )	{

		if( prRule != NULL )
			free( prRule );

		x++;
		y = 1;
		prRule = lexer->productionRules[x][0][0];
		// Alt. syntax: prRule = *lexer->productionRules[x][0];
		
		goto checkAgain;
	}

	return prSegment;
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

struct CSTNode* initNode( char* nodeName )	{

	struct CSTNode* _ = (struct CSTNode*) calloc( sizeof(struct CSTNode),1 );
	
	_->nodeName = getstring( nodeName );
	_->ancestor = NULL;
	_->descendents = NULL;
	_->numDescendents = 0;

	_->termStr = NULL;
	_->isTerminal = 0;

	return _;
}

