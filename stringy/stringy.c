// STRINGY_C
// DAVE'S STRING LIBRARY, PACKAGED WITH DAVELIB //

// INC'S
#include <ctype.h>
#include <stdarg.h>


#include "../colour/colour.h"
#include "stringy.h"


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



int cmp( char* a, char* b ) { // returns true (1) if the 2 c-strings match, as it should...

	return ( strcmp(a, b) == 0 );
}

/** // DUPLICATE
void rotate( int* argc, char* argv[] )	{

	for( int i=0; i < (*argc - 1); i++ )
		argv[i] = argv[i+1];

	if( *argc>0 )
		argv[*argc - 1] = '\0';

	*argc -= 1;
}
*/
char* flipstr( char* in, char* out )	{

	unsigned len = strlen(in);

	for( signed i = len - 1; i >= 0; i-- )
		*(out++) = *(in+i);

	*out = '\0';

	return out;
}

signed input( char* prmpt, char* buff, size_t sz )	{

	int ch;//, extra;

	// Get line with buffer overrun protection.
	if ( prmpt != NULL ) {

		printf( "%s ", prmpt );
		fflush ( stdout );
	}

	fgets( buff, sz, stdin );

	if( strlen(buff)==0 )
		return 0;

	// If it was too long, there'll be no newline. In that case, we flush
	// to end of line so that excess doesn't affect the next call.

	if ( buff[strlen(buff)-1] != '\n' ) {

		while ( ((ch = getchar()) != '\n') && (ch != EOF) )
			;
	}

	//buff[strlen(buff)] = '\0';

	if( cmp(buff,"quit") || cmp(buff,"q") )		
		return +2;

	return +1;
}

signed inputNum( char* prmpt, char* buff, __attribute__((unused))size_t sz )	{

	if( input(prmpt, buff, 1) != +1 )
		return 0;

	if ( atoi(buff) == 0 )
		return 0;

	return +1;
}


/**
	char* strc(char*str1, char*str2);
	char* strci(char*str1, char*str2);

	char* strd(char*str1, char*str2);
	char* strdi(char*str1, char*str2);

	void strp(char*str1, char*str2);
	void strpi(char*str1, char*str2);

	void strpd(char*str1, char*str2);
	void strpdi(char*str1, char*str2);
*/

/**
	STRC (str1, str2)	Returns a (char*) of str1 + str2. (+ is a concatenation operator)

*/
char* strc( char* str1, char* str2 )	{

		// Heap-allocated storage for 'r' can only be freed if the handle to it is
		// preserved. Embedding the strc( function as an argument in a calling function call
		// will mean the reference is lost to the caller, thus the memory might never be freed.

		char* r = (char*)malloc( strlen(str1) + strlen(str2) + 1 );

		strcpy( r, str1 );
		strcat( r, str2 );

		return r;
}

char* strci( char* str1, char* str2 )	{

	char* output = (char*)malloc( strlen(str1) + strlen(str2) + 1 );

	strcpy( output, str1 );
	strcat( output, str2 );
	strcat( output, NORMAL );

	return output;
}

char* strd( char* str1, char* str2 )	{

		// Heap-allocated storage for 'r' can only be freed if the handle to it is
		// preserved. Embedding the strc( function as an argument in a calling function call
		// will mean the reference is lost, thus the memory will never be freed.

		char* r = (char*)malloc( strlen(str1) + strlen(str2) + 1 );

		r = strc( str1, str2 );

		free( str1 );
		free( str2 );

		return r;
}

char* strdi( char* str1, char* str2 )	{

		// Heap-allocated storage for 'r' can only be freed if the handle to it is
		// preserved. Embedding the strc( function as an argument in a calling function call
		// will mean the reference is lost, thus the memory will never be freed.

		char* r = (char*)malloc( strlen(str1) + strlen(str2) + 1 );

		r = strci( str1, str2 );

		free( str1 );
		free( str2 );

		return r;
}

/**
	STRP(str1, str2)	- PRINT 2 cstrings to STDOUT -

	Concatenates, and prints the 2 cstrings to STDOUT.

	str1, str2 are preserved.

*/
void strp( char* str1, char* str2 )	{

		char* r = (char*)malloc( strlen(str1) + strlen(str2) + 1 );
		r = strc( str1, str2 );

		printf( "%s", r );
		free( r );
}

void strpi( char* str1, char* str2 )	{

		char* r = (char*)malloc( strlen(str1) + strlen(str2) + 1 );
		r = strci( str1, str2 );

		printf( "%s", r );
		free( r );
}

/**
	STRPD(str1, str2)	- PRINT, then free() str1 & str2 (DELETE) -

	Concatenates, and prints the 2 strcings to STDOUT, but then
	frees/deletes the references to str1 & str2.

*/
void strpd( char* str1, char* str2 )	{

		char* r = (char*)malloc( strlen(str1) + strlen(str2) + 1 );
		r = strd( str1, str2 );

		free( str1 );
		free( str2 );

		printf( "%s", r );
		free( r );
}

void strpdi( char* str1, char* str2 )	{

		char* r = (char*)malloc( strlen(str1) + strlen(str2) + 1 );
		r = strdi( str1, str2 );

		free( str1 );
		free( str2 );

		printf( "%s", r );
		free( r );
}



uint8_t CC_NULL = 0;

uint8_t CC_FG_BLACK = 1;
uint8_t CC_FG_RED = 2;
uint8_t CC_FG_GREEN = 3;
uint8_t CC_FG_YELLOW = 4;
uint8_t CC_FG_BLUE = 5;
uint8_t CC_FG_MAGENTA = 6;
uint8_t CC_FG_CYAN = 7;
uint8_t CC_FG_WHITE = 8;

uint8_t CC_FG_BRIGHT_BLACK = 9;
uint8_t CC_FG_BRIGHT_RED = 10;
uint8_t CC_FG_BRIGHT_GREEN = 11;
uint8_t CC_FG_BRIGHT_YELLOW = 12;
uint8_t CC_FG_BRIGHT_BLUE = 13;
uint8_t CC_FG_BRIGHT_MAGENTA = 14;
uint8_t CC_FG_BRIGHT_CYAN = 15;
uint8_t CC_FG_BRIGHT_WHITE = 16;

uint8_t CC_BG_BLACK = 17;
uint8_t CC_BG_RED = 18;
uint8_t CC_BG_GREEN = 19;
uint8_t CC_BG_YELLOW = 20;
uint8_t CC_BG_BLUE = 21;
uint8_t CC_BG_MAGENTA = 22;
uint8_t CC_BG_CYAN = 23;
uint8_t CC_BG_WHITE = 24;

uint8_t CC_BG_BRIGHT_BLACK = 25;
uint8_t CC_BG_BRIGHT_RED = 26;
uint8_t CC_BG_BRIGHT_GREEN = 27;
uint8_t CC_BG_BRIGHT_YELLOW = 28;
uint8_t CC_BG_BRIGHT_BLUE = 29;
uint8_t CC_BG_BRIGHT_MAGENTA = 30;
uint8_t CC_BG_BRIGHT_CYAN = 31;
uint8_t CC_BG_BRIGHT_WHITE = 32;

uint8_t CC_NORMAL = 33;

uint8_t CC_BOLD = 34;
uint8_t CC_ITALIC = 35;
uint8_t CC_UNDERLINE = 36;
uint8_t CC_STRIKETHROUGH = 37;
uint8_t CC_CAPSLOCK_ON = 38;
uint8_t CC_CAPSLOCK_OFF = 39;

uint8_t CC_UNSWITCH; // for rolling-back most recent control instruction.

uint8_t CC_HIDDEN_ON;
uint8_t CC_HIDDEN_OFF;
uint8_t CC_HIDDEN_MASKTYPE_BG;
uint8_t CC_HIDDEN_MASKTYPE_COLOR; // if selected "colour_h" subsystem needs to know whch color.



// STRCMP: Returns 0 for match, +n for larger LHS, -n for larger RHS.

int strcmp_( char* LHS, char* RHS ){

if( strlen(LHS)==strlen(RHS) ){
for( unsigned long long int i=0; i<strlen(LHS); i++ ){
if( LHS[i]==RHS[i] )
	continue;
	else return LHS[i]-RHS[i];}}
else
	return strlen(LHS)-strlen(RHS);

return 0; }



// SANITIZESTR: General method for pre-processing of an input c-string (safety).
char* sanitizeStr( char* A )	{

	char * B = (char *) malloc( strlen(A)+1 ) ;
	char * _ = B;

	while((*A) != '\0')	{

		switch(*A)	{

			case 92: // \ backslash
			case 34: // " double-quote
			case 42: // * star
			case 47: // / forward-slash
			case 58: // : colon
			case 60: // < lt
			case 62: // > gt
			case 63: // ? question-mark
			case 124:// | pipe

				// printf( "Invalid char ('%c') (%d).\n", *_, *_ );
				// Uncomment above line to see reporting to stdout of detected invalid chars in the input string.
				*B = '.'; // Arbitrary printable-char to replace invalid char with.
				break;

			default:
				*B = *A; 
				//printf( "char ok: '%c'\n", (*str) );
				break;	
		}

		++A;
		++B;
	}

	*B = '\0';

	return _;
}
// SEQ: Sensible version of strcmp(a,b) which returns 1 on a match, not 0.
int seq(char *a, char *b){
return streq( a,b );
}

int streq( char* a, char* b ){

	char _a = 0, _b = 0;
	if( a==(char*)0 ){
		a = getstring( "\0" );
		_a = 1;
	}
	if( b==(char*)0 ){
		b = getstring( "\0" );
		_b = 1;
	}

	int _ = !strcmp( a,b );

	if( _a ) free(a);
	if( _b ) free(b);
	return _;
}


uint8_t cmp_dstr( char* a, char* b )	{

	if( a == NULL )
		printf( "Warning. Arg 'a' in cmp_dstr (\"%s\":%d) is a NULL ptr.\n", __FILE__, __LINE__ );
	if( b == NULL )
		printf( "Warning. Arg 'b' in cmp_dstr (\"%s\":%d) is a NULL ptr.\n", __FILE__, __LINE__ );

	fflush( stdout );

	int i = 0;

	while( a[i++]=='0' )
		++a;

	i = 0;
	while( b[i++]=='0' )
		++b;

	int len_a = strlen( a );
	int len_b = strlen( b );

	if( len_a<len_b )
		return -1;

	if( len_a>len_b )
		return +1;

	for( int test=0; test<len_a; test++ )
		if( a[test]>b[test] )
			return +1;
		else if( a[test]<b[test] )
			return -1;

	return 0;	
}

// EXTRA FNCS
void PrintASCIITable(char start, char end)	{


	if( (start<32 || end<32) )	{

		printf("A scurrilous attempt was made to print the non-printable ascii characters below codepoint 32. Or even those new-fangled extended-ascii characters above codepoint 127. This is an outrage, and the function is immediately returning!");
		return;
	}

	if( start>end )	{

		char temp;
		temp = start;
		start = end;
		end = temp;
	}

	for( char i=start; ; i+=5 )	{

		if( i>end )
		i = end;

		char j=i;
		signed int k = (end-i);

		if( k>4 )
		k=4;

		switch(k)
		{
			case 4:
			printf("[%d]:=(%c)\t", j, j);
			++j;
			case 3:
			printf("[%d]:=(%c)\t", j, j);
			++j;
			case 2:
			printf("[%d]:=(%c)\t", j, j);
			++j;
			case 1:
			printf("[%d]:=(%c)\t", j, j);
			++j;
			case 0:
			printf("[%d]:=(%c)", j, j);
			default:
			printf( "\n" );
			break;
		}

		if(i==end)
		return;
	}

	return;
}


// INT -> CSTR
char* itoad(int v)	{

	// snprintf (buff, sizeof(buf), "%d",n); // print int 'n' into the char[] buffer
	char * _ = (char*) malloc(16) ;

	snprintf ( _, 16, "%d", v );

	return _;
}


// CAT(INT NUM_STRINGS, ...)
// 1ST ARG IS NUMBER OF SUBSEQUENT CSTRING ARGS - VARIADIC
// DEPS: SAFECAT()
char * cat(int c, char * s, ...)	{

	va_list ap;

	char * _;	
	_ = getstring( s );

	va_start(ap, s);
	for(uint8_t i=0; i<c; i++)	{

		_ = safecat(_, (char *)va_arg(ap, char*));
	}
	va_end(ap);

	//DEBUG_MSG("exiting cat()")
	return _;
}

char* int2str(int v)	{

	char* _ = (char*) malloc( (sizeof(int)*8)+1 ) ;
	sprintf( _, "%d", v );

	return _;
}

#include <limits.h>
int str2int(char* input)	{

	if( cmp_dstr( int2str(INT_MAX), input )<0 )
		return 0; // The input string represents a number too large!

	int len = strlen(input), i = 0, result = 0;

	if (input[0] == '-')
		i = 1;

	for(; i<len; i++)
		result = result * 10 + ( input[i] - '0' );

	if (input[0] == '-')
		result = 0 - result;

	return result;
}


void addnult( char* _, unsigned long long int i ){

	_[i] = '\0';
}


char* getstring( char* in ){

	unsigned long long int str_length = strlen( in );
	char* _ = (char*) malloc( str_length+1 ) ;

	unsigned long long int i;
	for( i=0; i<str_length; i++ )
		_[i] = in[i];

	_[i] = '\0';

	return _;
}




#define ERR_NP_CHAR 1
char * safecat( char* s, char* rhs )	{

	// SAFETY
	if( (s==NULL) || (rhs==NULL) )
		return s;

	// check s, rhs, for nonprintable chars (0-31)

	//unsigned int i = 0;
	char ch = '\0', chr = '\0';
	char * _s = s; char * _rhs = rhs;

	do	{

		if( (ch = _s[0]) )	{

			if( ch < (char)32 )
				return (char *)ERR_NP_CHAR;
		} // dangling-else!
		else
			--_s;

		if( (chr = _rhs[0]) )	{

			if( chr < (char)32 )
				return (char *)ERR_NP_CHAR;
		} // dangling-else!
		else
			--_rhs;

		++_s;
		++_rhs;

	} while (ch != '\0' || chr != '\0');

	char* _;

	_ = getstring( s );
	strcat(_, rhs);

	free(s);
	s = getstring( _ );

	free(_);
	// Do safety check on composite string _, before returning the cstring pointer to the caller.
	// ... 1. remove any non-alphabetical control codes, though printable.

	return s;
}
