// PARSER_H

#ifndef DAVELIBE_PARSER_H_
#define DAVELIBE_PARSER_H_

#define MAX_NUM_SEGMENTS 32
#define MAX_NUM_ENTRIES_IN_A_SEGMENT 32

typedef char* NonTerminal;
typedef char* Terminal;
typedef char* TerminalOrNonTerminal;

typedef double Matrix[4][4];

/** Perhaps a C-- modification could be to allow static (locally-scoped) typdefs, which C only supports by the programmer not exporting the 
struct definition in a .h file, but within a single C TU, with no external scope to access it from any other logical unit. 
Potential syntax could be: 'typedef static struct myStructClass;'. 
C-- could allow static (global) initialisation, such as 'myStructClass structInstance = { ..., ..., ... };' at global-scope 
(for instance, inside a .h header file scanned at compile-time, before linking).
*/

typedef char** Segment;
typedef int SegmentCounter;

#define TYPE_NONTERMINAL 1
#define TYPE_TERMINAL 0

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef bool
#define bool unsigned char
#endif

SegmentCounter NewSegmentCounter(void)	{
	
	SegmentCounter x = 0;
	return x;
}
 
extern Segment EmptySegment();

#define SetNodeTerminal( node, (char*) t, (char*) literal ) \
node->ID = t; \
node->isTerminal = 1; \
node->literal = literal; \
node->NTID = NULL; \
node->descendents = NULL; \
node->numDescendents = 0; \
\

// DATA STRUCTURES
typedef struct CFGFile	{
	
	NonTerminal* NTs;
	Segment* segments;
	int numPRs;
	//{ int numPRs, char** NTs, char**** segments }
} CFGFile;


typedef struct CSTNode	{

	char* nodeName; // OPTIONAL ID-STRING, NONTERMINAL DESIGNATION.

	signed isTerminal; 
	// if this is FAlSE, 0, then the 'descendents and 'numDescendents' struct members need to be populated, as the
	// descendents will be nonterminals.

	char* literal; // may be a composite of multiple Token.literal strings. ( A single ' ' between each substring for readability)
	NonTerminal NTID; // if isTerminal == FALSE, this would contain the NONTERMINAL name (from the CFG).
	Terminal ID;
	void* ancestor; 	// typeof( struct CSTNode* ), back-reference
	void** descendents;
	signed numDescendents;

} CSTNode;

typedef struct Parser	{

	char* tag;
	
	struct LexInstance* lexer;

	// Example Potential Usage:
	// struct ParseInstance parseInstance = newParser( );
	// int a = parseInstance.parse( &parseInstance );
	int (*parse)( struct Parser* self );

	char* CFG; // filename of .cfg production rules file.
	int numPRs; // number of production rules.
	
	char** NTs;
	
	char**** PRs;
	// PRs[NT_index][SEGMENTS][n_or_nt];

	struct CSTNode* Root;

	void (*AddNode)( struct CSTNode* node, struct CSTNode* ancestor );

	void (*AddLeaf)( struct CSTNode* node, struct CSTNode* leaf );
	
	void (*PushParserStack)( char* prRule, char*** collection, int amount, struct Parser* );
} Parser;

typedef struct Item	{
	
	int TYPE;
	TerminalOrNonTerminal tnt;
	void* tok_or_seg_collection;
	
} Item;

typedef struct Collection	{

	int count;
	Item _[];
	
} Collection;

#define set_current_parser(parser) _parser_ = parser;

// FUNCTIONS

extern signed GetPRIndex( NonTerminal NT, Parser* parser );
#define GetPRIndexE( NT ) GetPRIndex( NT, _parser_ )

extern bool IsTerminal( TerminalOrNonTerminal tt, struct LexInstance* lexer );
#define IsTerminalE( tt ) IsTerminal( tt, _parser_->lexer )

extern bool MATCH( Token token, Terminal terminal );

extern int Parse( struct Parser* );
#define Parse_E() Parse( _parser_ )

extern SegmentCounter NumSegments( NonTerminal NONTERMINAL, Parser* parser );
#define bNumSegmentsE( NONTERMINAL ) NumSegments( NONTERMINAL, _parser_ )

extern Segment GetSegment( NonTerminal, SegmentCounter, Parser* );
#define GetSegmentE( NT, segNumber ) GetSegment( NT, segNumber, _parser_ )

extern Segment GetNextNonTerminalSegment( NonTerminal NONTERMINAL, SegmentCounter* SegCount, Parser* parser );
#define GetNextNonTerminalSegmentE( NONTERMINAL, SegCount ) GetNextNonTerminalSegment( NONTERMINAL, SegCount, _parser_ )

extern Collection* RECURSE( Token* token, NonTerminal NONTERMINAL, SegmentCounter segCount, Parser* parser );
#define RECURSE_E( token, NONTERMINAL, segCount ) RECURSE( token, NONTERMINAL, segCount, _parser_ )

void PUSH( Collection* c, int type, TerminalOrNonTerminal tnt, void* tok_or_seg_collection, Parser* parser );
#define PUSHE( c, type, tnt, tok_or_seg_collection ) PUSH( c, tnt, tok_or_seg_collection, _parser_ )

Collection* SCAN( Token* token, NonTerminal NONTERMINAL, Segment SEGMENT, Parser* parser );
#define SCANE( token, NONTERMINAL, SEGMENT ) SCAN( token, NONTERMINAL, SEGMENT, _parser_ )

extern void AddNode( struct CSTNode*, struct CSTNode* ancestor );
extern void AddNode( struct CSTNode* node, struct CSTNode* ancestor );
extern void AddLeaf( struct CSTNode* node, struct CSTNode* ancestor );
extern void PushParserStack( char* prRule, char*** collection, int amount, struct Parser* );
#define PushParserStackE( prRule, collection, amount ) PushParserStack( prRule, collection, amount, _parser_ )

extern char** getNextProductionRuleSegment( struct Parser* parser );
#define getNextProductionRuleSegmentE() getNextProductionRuleSegment( _parser_ )

// INITIALIZATION FUNCTIONS
extern struct CSTNode* initNode( char* nodeName );
extern struct Parser* InitParserStack(void);

#endif

