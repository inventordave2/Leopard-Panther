// PARSER_C

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "./../stringy/stringy.h"
#include "./../fileywiley/fileywiley.h"
#include "./../colour/colour.h"
#include "regex.h"
#include "lexer.h"
#include "parser.h"

int Parse( struct LexInstance* lexer	)	{

	int x, x2, x3, x4;
	int flag;

	char* line;
	char** segments;
	char** terms;
	char* term;
	
	char* ruleName;
	char* prRule;

	char** prsegment;
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

		prRule = *getNextProductionRuleSegment( (void*)1 );
		
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
	
	node->descendents = (void**)(struct CSTNode**) malloc( sizeof(struct CSTNode*) * amount );
	node->numDescendents = amount;

	char* _;
	struct CSTNode* subNode;
	int x, y;
	x=0, y=0;
	for( x=0; x<amount; x++ )	{

		char* tt = collection[x][0];
		subNode = initNode( tt );

		if( checkType( tt, parser->lexer )=="T" )	{

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

void AddLeaf( struct CSTNode* node, struct CSTNode* ancestor )	{

	node->isTerminal = +1;

	if( node->termStr == NULL )
		node->termStr = getstring( "[NULL String Node passed to AddLeaf() ]" );

	node->ancestor = (void*) ancestor;
 	node->descendents = NULL;
	node->numDescendents = 0;
	
	return;
}

void AddNode( struct CSTNode* node, struct CSTNode* ancestor )	{

	node->ancestor = (void*) ancestor;
	int x = ancestor->numDescendents;

	ancestor->descendents[ x+1 ] = node;
	ancestor->descendents[ x+2 ] = NULL;
	ancestor->numDescendents++;

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

