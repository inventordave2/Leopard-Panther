// PARSER_C

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "./../stringy/stringy.h"
#include "./fileywiley.h"
#include "./../colour/colour.h"
#include "./regex.h"
#include "./lexer.h"

#include "./parser.h"


struct Parser* _parser_ = (struct Parser*)NULL;

Segment EmptySegment()	{
	
	Segment seg = (Segment) calloc( 1,sizeof(Segment) );
	return seg;
}


Segment GetSegment( NonTerminal NT, SegmentCounter segNumber, Parser* parser )	{

	signed i = GetPRIndex( NT, parser );
	if( i<0 )
		return (Segment)NULL;
	
	unsigned x = 0;
	
	while( x < segNumber )
		if( parser->PRs[i][x++]==NULL )
			return (Segment)NULL;
	
	Segment seg_strs = parser->PRs[i][segNumber];
	
	if( seg_strs==NULL )
		return (Segment)NULL;
	
	return (Segment) seg_strs;
}

SegmentCounter NumSegments( NonTerminal NONTERMINAL, Parser* parser )	{

	int x = 0;
	int i;
	for( i=0; i<parser->numPRs; i++ )	{
		
		if( !strcmp(NONTERMINAL,parser->NTs[i]) )	{
			
			x = 0;
			while( parser->PRs[i][x] != NULL )	{
				
				x++;
			}
			
			break;
		}
	}
	
	return (SegmentCounter)x;
}

Segment GetNextNonTerminalSegment( NonTerminal NONTERMINAL, SegmentCounter* SegCount, Parser* parser )	{
	
	Segment seg;
	
	if( *SegCount <= NumSegments( NONTERMINAL, parser ) )	{
		
		seg = GetSegment( NONTERMINAL, *SegCount, parser );
		*SegCount += 1;
		return seg;
	}
	
	return EmptySegment();
}


bool MATCHTERMINAL( Token token, char* TOKEN_NAME )	{
	
	if( strcmp( token.token_name, TOKEN_NAME )==0 /** Match */ )
		return TRUE;
	else
		return FALSE;
}


Collection* RECURSE( Token* token, NonTerminal NONTERMINAL, SegmentCounter segCount, Parser* parser )	{

	Segment SEGMENT;
	
	while( SEGMENT = GetNextNonTerminalSegment( NONTERMINAL, &segCount, parser ) )	{

		Collection* Data = SCAN( token, NONTERMINAL, SEGMENT, parser );
		if( Data != NULL )
			return Data; // equivalent to NONTERMINAL_SUCCESS
	}
	
	return NULL;
}

Item CreateItem( int TYPE, char* tnt, void* tok_or_seg_collection )	{
	
	Item item;
	
	item.TYPE = TYPE;
	item.tnt = tnt;
	item.tok_or_seg_collection = tok_or_seg_collection;
	
	return item;
}

void PUSH( Collection* c, int type, TerminalOrNonTerminal tnt, void* tok_or_seg_collection, Parser* parser )	{
	
	if( c==NULL )	{
	
		c = (Collection*) calloc( 1, sizeof(struct Collection) );
		
		if( c==NULL )	{
			
			fprintf( stderr, "Unable to create Collection* object in fnc PUSH at %s:%d. Exiting.\n", __FILE__, __LINE__ );
			exit( 1 );
		}
	}
	
	
	int i = 0;
	if( type==TYPE_TERMINAL )	{
		
		c->_[c->count] = CreateItem( TYPE_TERMINAL, tnt, tok_or_seg_collection );
		c->count++;
		return;
	}

	// type==TYPE_NONTERMINAL
	
	c->_[c->count] = CreateItem( TYPE_NONTERMINAL, tnt, tok_or_seg_collection );
	c->count++;
	
	return;	
}

char* GetTokenAt( Segment SEGMENT, int intra_seg_offset, struct Parser* parser )	{
	
	// regex for | not in braces.   
}


Collection* SCAN( Token* token, NonTerminal NONTERMINAL, Segment SEGMENT, struct Parser* parser )	{
			
	Collection* collection = NULL;
	Collection* seg_collection = NULL;
	
	NonTerminal segNONTERMINAL;
	Terminal	segTERMINAL;
	
	bool SEGMENT_MATCH = FALSE;
	
	signed i = 0;
	while( 1 )	{
		
		segTERMINAL = NULL;
		segNONTERMINAL = NULL;
		
		if( IsTerminal( SEGMENT[i], parser->lexer ) )
			segTERMINAL = stringy->getstring( SEGMENT[i] );
		else
			segNONTERMINAL = stringy->getstring( SEGMENT[i] );
		
		if( MATCH( *token, segTERMINAL )	)	{
			
			PUSH( collection, TYPE_TERMINAL, segTERMINAL, token, parser );
		}
		else if( (seg_collection = RECURSE( token, segNONTERMINAL, NewSegmentCounter(), parser ) ) )	{
			
			SEGMENT_MATCH = TRUE;
			PUSH( collection, TYPE_NONTERMINAL, segNONTERMINAL, CopyCollection( seg_collection ), parser );
			break;
		}
	
		i++;
	}
	
	if( !SEGMENT_MATCH )
		return (Collection*)NULL;
	
	return collection;
}


signed GetPRIndex( NonTerminal NT, Parser* parser )	{

		signed i;
		
		for( i=0; i< parser->numPRs; i++ )	{
			
			if( !strcmp(parser->NTs[i],NT) )
				return i;
		}
		
		return -1;
}


inline bool MATCH( Token token, Terminal terminal )	{
	
	if( !strcmp( token.token_name, terminal ) )
		return 1;
	
	return 0;
	
}

int Parse( struct Parser* parser )	{

	struct LexInstance* lexer = parser->lexer;
	
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
	
	// lexInstance->productionRules = (char****) calloc( sizeof(char*), max_num_rules * max_num_segments * MAX_NUM_ENTRIES_IN_A_SEGMENT );
	// char**** productionRules; //[][][]
	// [ruleNum][segmentNum][entryInSegment]

	int max_num_rules = 512;
	
	
	for( x=0; x<max_num_rules; x++ )	{

		line = getline_file( parser->CFG,x );

		segments = split( line, '|', MAX_NUM_SEGMENTS );
	// if '|' follows an opening bracket, such as (, {or [, but before a matching closing bracket ), }, or ], remerge subsequence.
		// char* joined = stitch( char* str1, char* str2 );
		/**
		
		int i;
		char* joined;
		char* newstring = getstring( "" );
		for( i=0; i<numStrings-1; i+=2 )	{
			
			joined = stitch( strs[i], strs[i+1] );
			safecat( newstring, joined );
			free( joined );
		}
		
		if( i==numStrings-1 )	{
			
			joined = stitch( joined, strs[numStrings-1] );
			safecat( newstring, joined );
			free( joined );
		}
		
		*/
		for( x2=0; x2<MAX_NUM_SEGMENTS; x2++ )	{

			if( segments[ x2 ] == NULL )
				break;
			
			terms = stringy->split( segments[ x2 ], ' ', MAX_NUM_ENTRIES_IN_A_SEGMENT );
			
			for( x3=0; x3<MAX_NUM_ENTRIES_IN_A_SEGMENT; x3++ )	{

				term = terms[ x3 ];
				if( term==NULL )
					break;
				
				parser->PRs[x][x2][x3] = term;
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
	
	for( x=0; x<parser->lexer->tokensCount; x++ )	{

		
		token_type = lexer->tokens[x][0];
		token = lexer->tokens[x][1];
		
		// if the PR entry is a non-terminal, we need to recursively stack (LIFO), until we find a terminal definition.
		// if that terminal definition matches an entry in a production-rule segment, we need to wait until the full production-rule section
		// is matched by sequential tokens.

		section_scan:

		prsegment = getNextProductionRuleSegment( parser );
		
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

struct Parser* NewParser(void)	{

	struct Parser* parser = (struct Parser*) calloc( 1, sizeof(struct Parser) );
	
	parser->lexer	= NULL;
	parser->parse	= Parse;
	
	parser->Root	= NULL;

	parser->CFG		= NULL;

	parser->NTs		= NULL;
	parser->numPRs	= 0;
	parser->PRs		= NULL;
	// char***

	parser->AddNode = AddNode;
	parser->AddLeaf = AddLeaf;
	parser->PushParserStack = PushParserStack;
	
	return parser;
}

#define SetParserTag( parser, tag ) parser->tag = getstring( tag );


int CalcBlankLineCount( char** lineArray )	{
	
	char* _;
	int x = 0;
	while( lineArray != NULL )	{

		if( strlen( _ = trim(lineArray) )==0 )
			x++;
		
		free( _ );
		lineArray++;
	}
	
	return x;
}

CFGFile ScanCFG( char* cfgfn )	{

	CFGFile cfg_file;
	//{ int numPRs, char** NTs, char**** segments }
	
	cfg_file.numPRs = 0;
	cfg_file.NTs = (NonTerminal*) calloc( MAX_NUM_PRODUCTION_RULES+1,sizeof(NonTerminal) );
	struct FileContext filecontext;
	// struct members:
	// 	int lineCount;
	//char** lines;
	
	filecontext = readFile( cfgfn );

	

	unsigned x = 0;
	unsigned i;
	char* _ = calloc( 1,1 );

	if( filecontext.length > 0 )	{
		
		// 1. Either the string (apart from whistespace-only strings) is a NONTERMINAL (PR Name), or a pipe-delimeted set of segments associated with the rule. These pipe-delimeted segments may contain whitespace, and '|' chars, and bracketed content ( { and } especially will contain whitespace.
			
		// 2. Use Lexer to tokenize.
		LexerInstance* lexr = initLex( cfgfn, "./c--.cfg" );
			
			int lexResult = lexr->lex( lexr );
			int i;
			if( lexResult )	{

				for( i=0; i<lexr->tokensCount; i++ )	{
					
					if( !IsTerminal(lexr->tokens[i][0], lexr) )	{
						
						int len = strlen( lexr->tokens[i][lexr->TOK_REGEX] );
						if( lexr->tokens[i][lexr->TOK_REGEX][ len-1 ] == ':' )	{
							
							char* NON_TERMINAL_RULENAME = getstring( lexr->tokens[i][lexr->TOK_REGEX] );
							NON_TERMINAL_RULENAME[ len-1] = '\0';
							
							parser->NTs[ parser->numPRs ] = NON_TERMINAL_RULENAME;
							parser->numPRs++;							
						}
						else	{
							
							char* NT = getstring( lexr->tokens[i][lexr->TOK_REGEX] );
							// PRs[NT_index][SEGMENTS][n_or_nt];
							
							int x = 0;
							int y = 0;
							
							while( parser->PRs[ parser->numPRs-1 ][x] != NULL )
								x++;
							
							x--;
							
							while( parser->PRs[ parser->numPRs-1 ][x][y] != NULL )
								y++;
							
							y--;
							
							parser->PRs[ parser->numPRs-1 ][x][y] = NT;
						}
					}
					else	{ /** TERMINAL */
				}
			}
	}


	
	cfg_file.numPRs = x;
	
	char** NTs2 = cfg_file.NTs;
	
	if( x<MAX_NUM_PRODUCTION_RULES )	{

		NTs2 = realloc( cfg_file.NTs, (x+1)*sizeof(NonTerminal) );
		
		if( NTs2 == NULL )	{
			
			fprintf( stderr, "Failed to realloc() ptr at %s:%d. Exiting.\n", __FILE__, __LINE__-4 );
			exit( 1 );
		}
	}
	
	cfg_file.NTs = NTs2;
	
	free( filecontents.lines );
	
	return cfg_file;
}
char** special_split( char* str, char delim, int max_num_segments )	{
	
	char* brackets_open = getstring( "([{" );
	char* brackets_close = getstring( ")]}" );
	assert( strlen(brackets_open)==strlen(brackets_close) );
	
	char* escape_chars = getstring( "\\" );
	
	int** bc = (int**) calloc( strlen(brackets_open), sizeof(int) );
	char** results = (char**) calloc( max_num_segments, sizeof(TerminalOrNonTerminal) );

	char* _ = str;
	char* prev = str;
	bool esc_flag = 0;
	unsigned n = 0;
	
	while( *_ != '\0' )	{
		
		for( int z=0; z<strlen(escape_chars); z++ )
			if( escape_chars[z] == *_ )	{
				
				esc_flag = (esc_flag==0?1:0);
				goto cont;
			}
			
		for( int z=0; z<strlen(brackets_open); z++ )
			if( brackets_open[z] == *_&& !esc_flag )
				bc[z]++;
			else if( brackets_close[z] == *_ && !esc_flag )
				bc[z]--;

		if( *_ == delim && !esc_flag )	{
			
			int test = 0;
			for( int z=0; z<strlen(brackets_open); z++ )	{

				if( bc[z]!=0 )	{
	
					test++;
				}
			}

			if( test == 0 )	{

				results[ n ] = stringy->substring( str, prev-str, _ - prev );
				prev = _;
				n++;
			}
		}

		_++;
		
		esc_flag = 0;
		
		cont:
		
		continue;
	}
	
	results[ n ] = NULL;
	return results;
}
// 

void InitParser( Parser* parser, char* cfg, LexInstance* lexer )	{
	
	parser->lexer	= lexer;
	parser->CFG		= cfg;
	
	CFGFile cfgFile = ScanCFG( cfg );
	
	parser->numPRs	= cfgFile.numPRs;
	parser->NTs		= cfgFile.NTs;
	parser->PRs		= &cfgFile.segments; // [NT_index][segNumber][nt_or_t]
	
	
}

bool IsTerminal( TerminalOrNonTerminal tt, struct LexInstance* lexer )	{
	
	int i;
	char* _ = NULL;
	for( i=0; i<lexer->numRules; i++ )	{
		
		if( !strcmp( lexer->tokenRules[i][0], tt ) )	{
			
			_ = lexer->tokenRules[i][0];
			break;
		}
	}
	
	if( _==NULL )
		return FALSE;
	
	return TRUE;
}

void PushParserStack( char* prRule, char*** collection, int amount, struct Parser* parser )	{

	struct CSTNode* node = initNode( getstring(prRule) );

	char* _;
	struct CSTNode* subNode;
	int x, y;
	x=0, y=0;
	for( x=0; x<amount; x++ )	{

		char* tt = collection[x][0];
		subNode = initNode( tt );

		if( IsTerminal( tt, parser->lexer ) )	{

			// Terminal.
			// This has the side-effect that if any
			// leaf nodes upon parse completion
			// are not Terminals, the parse
			// has failed.
			
			SetNodeTerminal( subNode, collection[x][0], collection[x][1] );
		}
		else	{
			// NonTerminal.
			subNode->isTerminal = 0;
			subNode->NTID = prRule;
			node->descendents = (void**)(struct CSTNode**) calloc( amount, sizeof(struct CSTNode*) );
		}

		//subNode->ancestor = node;
		//node->descendents[x] = subNode;
		parser->AddNode( subNode, node );
	}

	return;
}

struct CSTNode* initNode( char* nodeName )	{

	struct CSTNode* _ = (struct CSTNode*) calloc( 1, sizeof(struct CSTNode*) );
	
	_->nodeName = getstring( nodeName );
	_->ancestor = NULL;
	_->descendents = NULL;
	_->numDescendents = 0;

	_->literal = NULL;
	_->isTerminal = 0;
	_->NTID = NULL;

	return _;
}

void AddLeaf( struct CSTNode* node, struct CSTNode* ancestor )	{

	node->isTerminal = +1;

	if( node->ID == NULL )
		node->ID = getstring( "[ NULL Terminal Node passed to AddLeaf() ]" );

	node->ancestor = (void*) ancestor;
 	node->descendents = NULL;
	node->numDescendents = 0;
	
	return;
}

void AddNode( struct CSTNode* node, struct CSTNode* ancestor )	{

	node->isTerminal = 0;
	
	node->ancestor = (void*) ancestor;
	int x = ancestor->numDescendents;

	ancestor->descendents[ x+1 ] = node;
	ancestor->descendents[ x+2 ] = NULL;
	ancestor->numDescendents++;

	return;
}

Segment getNextProductionRuleSegment( struct Parser* parser )	{

	static unsigned x = 0;
	static unsigned y = 0;
	static NonTerminal prRule = NULL;

	Segment prSegment;

	if( parser==NULL )	{
		// reset statics.
		x = 0, y = 0;

		if( prRule != NULL )
			free( prRule );

		prRule = getstring( "" );
		return NULL;
	}

	if( parser==(void*)1 )	{

		// return prRule;		
		return &prRule;
	}

	if( strcmp( prRule,parser->NTs[x] )==1 ) // NONMATCH
		prRule = parser->NTs[x];

	checkAgain:

	prSegment = parser->PRs[x][y++];

	if( prSegment == NULL )	{
	// no more production rules to provide a segment from.

		x = 0;
		y = 0;

		if( prRule != NULL )	{

			free( prRule );
			prRule = NULL;
		}

		return NULL;
	}

	return prSegment;
}

