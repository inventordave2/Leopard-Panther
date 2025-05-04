// REGEX_C

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// W STOOP'S REGEX LIBRARY
#include "./../wernee/regex_w/wregex.h"
#include "./../wernee/regex_w/wrx_prnt.h"

#include "./../stringy/stringy.h"
#include "./regex.h"

Token GetEmptyToken()	{

	Token token;
	token.literal = NULL;
	token.length = 0;
	token.token_name = NULL;
	token.pattern = NULL;
	token.offset = 0;
	token.source = NULL;
	
	return token;
}

/**
A 3rd party regex engine (rather than the embedded default regex engine used by my lexerparser, the wernerstoop regex library, may not
populate a Token object with anything more than token->literal, which is a matched substring to a given regex pattern.
*/
int IsTokenEmpty( Token* token )	{

	if( token->literal == NULL )
		return 1;
	
	return 0;
}


void FreeToken( Token* token )	{
	
	if( token->literal != NULL )	{
		
		free( token->literal );
		token->literal = NULL;
	}
	
	token->length = 0;
	
	if( token->token_name != NULL )	{
		
		free( token->token_name );
		token->token_name = NULL;
	}
	
	if( token->pattern != NULL )
		free( token->pattern );
	
	token->pattern = NULL;
	
	token->offset = 0;
	
	/**
	token->source *MUST NOT BE FREED HERE. IT IS SIMPLY A REFERENCE TO A LANGUAGE STRING, WHICH MAY BE STILL IN USE.*
	*/
	token->source = NULL;
	
	return;
}

Token MatchStringToPattern( char* pattern, char* str )	{

	int e, ep;
	wregex_t *r;
	wregmatch_t *subm;

	Token tok;

	r = wrx_comp(pattern, &e, &ep);

	if(!r) {

		fprintf(stderr,
			"\n[%s:%d] ERROR: %s\npattern: '%s'\nwregex error code: %d\n",
			__FILE__,
			__LINE__,
			wrx_error(e),
			pattern,
			ep );

		tok = GetEmptyToken();
		return tok;
	}

	if(r->n_subm > 0) {

		subm = calloc(sizeof *subm, r->n_subm);
		if(!subm) {

			fprintf(stderr, "Error: out of memory (submatches).\n");
			wrx_free(r);
			tok = GetEmptyToken();
			return tok;
		}
	}
	else
		subm = NULL;

	e = wrx_exec(r, str, &subm, &r->n_subm);

	if(e < 0)
		fprintf(stderr, "Error: %s\n", wrx_error(e));
	
	if( e <= 0 )
		tok = GetEmptyToken();
	else	{
		
		
		tok.literal = calloc( 256, CODEPAGE_ATOMIC_WIDTH );
		
		unsigned long long begin = (unsigned long long)subm[0].beg;
		unsigned long long term  = (unsigned long long)subm[0].end;
		unsigned long long str_addr = (unsigned long long)str;
		
		tok.literal = getsubstring( str+(begin-str_addr), 0, term-begin-1 );
		tok.offset = begin - str_addr;
		tok.length = (term - begin);
		tok.pattern = getstring( pattern );
		tok.token_name = getstring( tok.literal );
		tok.source = str; // do not copy, just the reference. Sources tend to be entire source-code files.
	}
	
	free(subm);
	wrx_free(r);
	
	// If we got here, wregex has either matched the pattern to the rule, or has failed to find a match.
	return tok;
}

