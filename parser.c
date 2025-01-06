// PARSER_C

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

