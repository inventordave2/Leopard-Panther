// PARSER_C

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "parser.h"


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

