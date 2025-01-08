// STRINGY_H
// STRING METHODS //

#ifndef DAVELIB_STRINGY_V1
#define DAVELIB_STRINGY_V1

// INC'S
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define uint8_t signed short int
extern uint8_t CC_NULL;

extern uint8_t CC_FG_BLACK;
extern uint8_t CC_FG_RED;
extern uint8_t CC_FG_GREEN;
extern uint8_t CC_FG_YELLOW;
extern uint8_t CC_FG_BLUE;
extern uint8_t CC_FG_MAGENTA;
extern uint8_t CC_FG_CYAN;
extern uint8_t CC_FG_WHITE;

extern uint8_t CC_FG_BRIGHT_BLACK;
extern uint8_t CC_FG_BRIGHT_RED;
extern uint8_t CC_FG_BRIGHT_GREEN;
extern uint8_t CC_FG_BRIGHT_YELLOW;
extern uint8_t CC_FG_BRIGHT_BLUE;
extern uint8_t CC_FG_BRIGHT_MAGENTA;
extern uint8_t CC_FG_BRIGHT_CYAN;
extern uint8_t CC_FG_BRIGHT_WHITE;

extern uint8_t CC_BG_BLACK;
extern uint8_t CC_BG_RED;
extern uint8_t CC_BG_GREEN;
extern uint8_t CC_BG_YELLOW;
extern uint8_t CC_BG_BLUE;
extern uint8_t CC_BG_MAGENTA;
extern uint8_t CC_BG_CYAN;
extern uint8_t CC_BG_WHITE;

extern uint8_t CC_BG_BRIGHT_BLACK;
extern uint8_t CC_BG_BRIGHT_RED;
extern uint8_t CC_BG_BRIGHT_GREEN;
extern uint8_t CC_BG_BRIGHT_YELLOW;
extern uint8_t CC_BG_BRIGHT_BLUE;
extern uint8_t CC_BG_BRIGHT_MAGENTA;
extern uint8_t CC_BG_BRIGHT_CYAN;
extern uint8_t CC_BG_BRIGHT_WHITE;

extern uint8_t CC_NORMAL;

extern uint8_t CC_BOLD;
extern uint8_t CC_ITALIC;
extern uint8_t CC_UNDERLINE;
extern uint8_t CC_STRIKETHROUGH;
extern uint8_t CC_CAPSLOCK_ON;
extern uint8_t CC_CAPSLOCK_OFF;

extern uint8_t CC_UNSWITCH; // for rolling-back most recent control instruction.

extern uint8_t CC_HIDDEN_ON;
extern uint8_t CC_HIDDEN_OFF;
extern uint8_t CC_HIDDEN_MASKTYPE_BG;
extern uint8_t CC_HIDDEN_MASKTYPE_COLOR; // if selected "colour_h" subsystem needs to know whch color.

void PrintASCIITable(char start, char end);

// SAFE-CONCATENATE
extern char* safecat(char* s, char* rhs);

extern char* int2str(int v);
extern int str2int(char*);

// SUGAR
extern int streq( char*,char* ); // returns true (1) if the 2 c-strings match, as it should...
int seq(char *a, char *b); // actually returns 0 for non-match, 1 for match.

extern uint8_t cmp_dstr( char*, char* );

#define strcmp strcmp_
extern int strcmp_( char* LHS, char* RHS );

// ADD NULL-TERMINATOR TO CHAR*, REQUIRES INDEX
#define addnullt( _,i ) _[i]='\0';

// Basic string-safety check, removes verboten narrow-chars as defined in function body.
extern char* sanitizeStr( char* );

// places string at 'in' into 'out' in reverse sequence.
char* flipstr(char* in, char* out); 

// ITOA() FUNCTION "ITOAD()"
extern char* itoad(int v);

// CONCATENATE (VARIADIC FNC)
extern char* cat(int c, char * s, ...);

// STRING-SAFETY METHODS
extern char* getstring( char* in );
extern void addnult( char*, unsigned long long int );




	// Simple cstring library. Helper functions for:
	//
	// Concatenating 2 strings, as again but also destroying input string references/heap-allocated memory.
	// As above, but also print to stdout
	//
	// strc, strci, strd, strdi
	//
	// strp, strpi, strpd, strpdi
	//

	// Also, stdin functions:
	//
	// input, inputNum

	/**
		STRC (str1, str2)	Returns a (char*) of str1 + str2. (+ is a concatenation operator)

	*/
	char* strc(char*str1, char*str2);
	char* strci(char*str1, char*str2);

	char* strd(char*str1, char*str2);
	char* strdi(char*str1, char*str2);

	/**
		STRP(str1, str2)	- PRINT 2 strcings to STDOUT -

		Concatenates, and prints the 2 strcings to STDOUT.

		str1, str2 are preserved.

	*/
	void strp(char*str1, char*str2);
	void strpi(char*str1, char*str2);

	/**
		STRPD(str1, str2)	- PRINT, then free() str1 & str2 (DELETE) -

		Concatenates, and prints the 2 strcings to STDOUT, but then
		frees/deletes the references to str1 & str2.

	*/
	void strpd(char*str1, char*str2);
	void strpdi(char*str1, char*str2);

/**
#ifndef FS_ENUM_STATUS
#define FS_ENUM_STATUS
enum	{

	OK, // 1
	TOO_LONG, // -1
	NO_INPUT, // 0

	SUCCESS_, // +1
	ERROR_, // -1

	QUIT // +2

} status;
#endif
*/
	signed input(char*prmpt, char*buff, size_t sz);
	signed inputNum(char*prmpt, char*buff, size_t sz);



#endif
