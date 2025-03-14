// COLOUR_C

// A lightweight colourization subroutine set.
// Utilizes ANSI/VT100 control-codes.
// See function 'char* ResetAnsiVtCodes(char* f)' below to see how simple the implementation is.

// PLATFORM TARGETS
#ifdef WIN32
#include <windows.h>
#endif

// STDLIB INC'S
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../stringy/stringy.h"
#include "colour.h"

//-----------------------

char ansivt;

char* FG_BLACK;
char* FG_RED;
char* FG_GREEN;
char* FG_YELLOW;
char* FG_BLUE;
char* FG_MAGENTA;
char* FG_CYAN;
char* FG_WHITE;

char* FG_BRIGHT_BLACK;
char* FG_BRIGHT_RED;
char* FG_BRIGHT_GREEN;
char* FG_BRIGHT_YELLOW;
char* FG_BRIGHT_BLUE;
char* FG_BRIGHT_MAGENTA;
char* FG_BRIGHT_CYAN;
char* FG_BRIGHT_WHITE;

char* BG_BLACK;
char* BG_RED;
char* BG_GREEN;
char* BG_YELLOW;
char* BG_BLUE;
char* BG_MAGENTA;
char* BG_CYAN;
char* BG_WHITE;

char* BG_BRIGHT_BLACK;
char* BG_BRIGHT_RED;
char* BG_BRIGHT_GREEN;
char* BG_BRIGHT_YELLOW;
char* BG_BRIGHT_BLUE;
char* BG_BRIGHT_MAGENTA;
char* BG_BRIGHT_CYAN;
char* BG_BRIGHT_WHITE;

char* NORMAL;

char* BOLD; // \e[1m
char* UNDERLINE; // \e[4m
char* TAB; // \eI
char* LINEFEED; // \eJ
char* CARRAIGERETURN; // \eM
char* CRSRBACKSPACE; // \eH
char* BELL; // \eG

char* CLS;
char* CLEAR_LINE;
char* CLEAR_LINE_CURSOR_RIGHT;
char* CLEAR_LINE_CURSOR_LEFT;
char* MOV_CRSR_TOPLEFT; // \e[H
char* CLEAR_TO_REST_OF_LINE; // \e[K
char* CLEAR_TO_REST_OF_SCREEN; // \e[J
char* MOV_CRSR_DOWN_N;
char* MOV_CRSR_UP_N;
char* MOV_CRSR_LEFT_COLS; // \e[<n>D
char* MOV_CRSR_RIGHT_COLS; // \e[<n>C
char* SET_COLS_TO_132; // \e[?3h
char* SET_COLS_TO_80; // \e[?31

char* setVal( char* _, short v )	{

	char* str;
	short i = 0;

	while( _[i] != '*' )
		i++;

	_[i] = '\0';
	str = getstring( _ );
	_[i] = '*';
	strcat( str, int2str(v) );
	_+=i+1;
	strcat( str,_ );

	return str;
}

char* ANSIVT( char* str, char** cc, long long int* offsets )	{

	unsigned long long int str_width = strlen(str);
	unsigned long long int width = str_width + (strlen(*cc)*VTCODEWIDTH);
	char* _ = malloc( width+1 );
	char* bucket = malloc( width+1 );
	char* vtcodestr;

	unsigned long long int q=0;
	unsigned long long int t=0;
	while( t<str_width ){

		long long int p;
		for( p=0; p< (*offsets); p++ ) // offsets are relative.
			bucket[p] = str[t++];

		bucket[p] = '\0';
		offsets++;

		safecat( _,bucket );
		vtcodestr = getVTCodeString( *cc++ );
		safecat( _,vtcodestr );

		free( vtcodestr );

		++cc;
		++q;

		if( **cc == '\0' )	{

			unsigned long long int y = strlen(_);
			for( unsigned long long int x=t;x<str_width; x++ )
				_[y++]=str[x];

			_[y] = '\0';			
			break;
		}
	}

	return _;
}

// ANSI/VT Global Refs.
char* ANSIVT_FG;
char* ANSIVT_BG;
struct _ANSI_* ANSI;
// Just use 'ANSI = colorMode();' in your program, presumably at the head of your main() function/entry-point.

// FUNCTIONS
char ResetAnsiVtCodes(char f)	{

	// ..... INIT ALL THE STRING PTRS.

	BOLD = malloc( 8 * sizeof(char) );// 
	UNDERLINE = malloc( 8 * sizeof(char) ); // 
	TAB = malloc( 8 * sizeof(char) ); // 
	LINEFEED = malloc( 8 * sizeof(char) ); // 
	CARRAIGERETURN = malloc( 8 * sizeof(char) ); // 
	CRSRBACKSPACE = malloc( 8 * sizeof(char) ); // 
	BELL = malloc( 8 * sizeof(char) ); // 

	CLS = malloc( 8 * sizeof(char) );
	MOV_CRSR_TOPLEFT = malloc( 8 * sizeof(char) ); 
	CLEAR_TO_REST_OF_LINE = malloc( 8 * sizeof(char) ); 
	MOV_CRSR_DOWN_N = malloc( 8 * sizeof(char) );
	MOV_CRSR_UP_N = malloc( 8 * sizeof(char) );
	MOV_CRSR_LEFT_COLS = malloc( 8 * sizeof(char) );
	MOV_CRSR_RIGHT_COLS = malloc( 8 * sizeof(char) );

	CLEAR_LINE = malloc( 8 * sizeof(char) );
	CLEAR_LINE_CURSOR_RIGHT = malloc( 8 * sizeof(char) );
	CLEAR_LINE_CURSOR_LEFT = malloc( 8 * sizeof(char) );
	CLEAR_TO_REST_OF_SCREEN = malloc( 8 * sizeof(char) );
	SET_COLS_TO_132 = malloc( 8 * sizeof(char) );
	SET_COLS_TO_80 = malloc( 8 * sizeof(char) );


	FG_BLACK = malloc ( 8 * sizeof(char) );
	FG_BLUE = malloc ( 8 * sizeof(char) );
	FG_BRIGHT_BLACK = malloc ( 8 * sizeof(char) );
	FG_BRIGHT_BLUE = malloc ( 8 * sizeof(char) );
	FG_BRIGHT_CYAN = malloc ( 8 * sizeof(char) );
	FG_BRIGHT_GREEN = malloc ( 8 * sizeof(char) );
	FG_BRIGHT_MAGENTA = malloc ( 8 * sizeof(char) );
	FG_BRIGHT_RED = malloc ( 8 * sizeof(char) );
	FG_BRIGHT_WHITE = malloc ( 8 * sizeof(char) );
	FG_BRIGHT_YELLOW = malloc ( 8 * sizeof(char) );
	FG_CYAN = malloc ( 8 * sizeof(char) );
	FG_GREEN = malloc ( 8 * sizeof(char) );
	FG_MAGENTA = malloc ( 8 * sizeof(char) );
	FG_RED = malloc ( 8 * sizeof(char) );
	FG_WHITE = malloc ( 8 * sizeof(char) );
	FG_YELLOW = malloc ( 8 * sizeof(char) );
	NORMAL = malloc ( 8 * sizeof(char) );

	BG_BLACK = malloc ( 8 * sizeof(char) );
	BG_BLUE = malloc ( 8 * sizeof(char) );
	BG_BRIGHT_BLACK = malloc ( 8 * sizeof(char) );
	BG_BRIGHT_BLUE = malloc ( 8 * sizeof(char) );
	BG_BRIGHT_CYAN = malloc ( 8 * sizeof(char) );
	BG_BRIGHT_GREEN = malloc ( 8 * sizeof(char) );
	BG_BRIGHT_MAGENTA = malloc ( 8 * sizeof(char) );
	BG_BRIGHT_RED = malloc ( 8 * sizeof(char) );
	BG_BRIGHT_WHITE = malloc ( 8 * sizeof(char) );
	BG_BRIGHT_YELLOW = malloc ( 8 * sizeof(char) );
	BG_CYAN = malloc ( 8 * sizeof(char) );
	BG_GREEN = malloc ( 8 * sizeof(char) );
	BG_MAGENTA = malloc ( 8 * sizeof(char) );
	BG_RED = malloc ( 8 * sizeof(char) );
	BG_WHITE = malloc ( 8 * sizeof(char) );
	BG_YELLOW = malloc ( 8 * sizeof(char) );	

	// .....
	int t=0;
	if(f == 0)	{

		#ifdef cm0
		repeat:
		#endif

		strcpy((char*)FG_BLACK, "");
		strcpy((char*)FG_RED, "");
		strcpy((char*)FG_GREEN, "");
		strcpy((char*)FG_YELLOW, "");
		strcpy((char*)FG_BLUE, "");
		strcpy((char*)FG_MAGENTA, "");
		strcpy((char*)FG_CYAN, "");
		strcpy((char*)FG_WHITE, "");

		strcpy((char*)FG_BRIGHT_BLACK, "");
		strcpy((char*)FG_BRIGHT_RED, "");
		strcpy((char*)FG_BRIGHT_GREEN, "");
		strcpy((char*)FG_BRIGHT_YELLOW, "");
		strcpy((char*)FG_BRIGHT_BLUE, "");
		strcpy((char*)FG_BRIGHT_MAGENTA, "");
		strcpy((char*)FG_BRIGHT_CYAN, "");
		strcpy((char*)FG_BRIGHT_WHITE, "");

		strcpy((char*)BG_BLACK, "");
		strcpy((char*)BG_RED, "");
		strcpy((char*)BG_GREEN, "");
		strcpy((char*)BG_YELLOW, "");
		strcpy((char*)BG_BLUE, "");
		strcpy((char*)BG_MAGENTA, "");
		strcpy((char*)BG_CYAN, "");
		strcpy((char*)BG_WHITE, "");

		strcpy((char*)BG_BRIGHT_BLACK, "");
		strcpy((char*)BG_BRIGHT_RED, "");
		strcpy((char*)BG_BRIGHT_GREEN, "");
		strcpy((char*)BG_BRIGHT_YELLOW, "");
		strcpy((char*)BG_BRIGHT_BLUE, "");
		strcpy((char*)BG_BRIGHT_MAGENTA, "");
		strcpy((char*)BG_BRIGHT_CYAN, "");
		strcpy((char*)BG_BRIGHT_WHITE, "");

		strcpy((char*)NORMAL, "");

		strcpy((char*)BOLD, ""); // 
		strcpy((char*)UNDERLINE, ""); // 
		strcpy((char*)TAB, ""); // 
		strcpy((char*)LINEFEED, ""); // 
		strcpy((char*)CARRAIGERETURN, ""); // 
		strcpy((char*)CRSRBACKSPACE, ""); // 
		strcpy((char*)BELL, ""); // 

		strcpy((char*)CLS, "");
		strcpy((char*)MOV_CRSR_TOPLEFT, ""); 
		strcpy((char*)CLEAR_TO_REST_OF_LINE, ""); 
		strcpy((char*)MOV_CRSR_DOWN_N, "");
		strcpy((char*)MOV_CRSR_UP_N, "");
		strcpy((char*)MOV_CRSR_LEFT_COLS, "");
		strcpy((char*)MOV_CRSR_RIGHT_COLS, "");

		strcpy((char*)CLEAR_LINE, "");
		strcpy((char*)CLEAR_LINE_CURSOR_RIGHT, "");
		strcpy((char*)CLEAR_LINE_CURSOR_LEFT, "");
		strcpy((char*)CLEAR_TO_REST_OF_SCREEN, "");
		strcpy((char*)SET_COLS_TO_132, "");
		strcpy((char*)SET_COLS_TO_80, "");

		if( t==1 )
			goto ret_stmt;
	}

	else if(f == 1)	{

		#ifndef cm0
		// Visibility determined by cmd-line compile switch -Dcm0
		// See Makefile target 'nocolour' for a clue.

		ANSIVT_FG = FG_BRIGHT_YELLOW;
		ANSIVT_BG = BG_BRIGHT_BLUE;

		strcpy((char*)FG_BLACK, "\e[30m");
		strcpy((char*)FG_RED, "\e[31m");
		strcpy((char*)FG_GREEN, "\e[32m");
		strcpy((char*)FG_YELLOW, "\e[33m");
		strcpy((char*)FG_BLUE, "\e[34m");
		strcpy((char*)FG_MAGENTA, "\e[35m");
		strcpy((char*)FG_CYAN, "\e[36m");
		strcpy((char*)FG_WHITE, "\e[37m");

		strcpy((char*)FG_BRIGHT_BLACK, "\e[90m");
		strcpy((char*)FG_BRIGHT_RED, "\e[91m");
		strcpy((char*)FG_BRIGHT_GREEN, "\e[92m");
		strcpy((char*)FG_BRIGHT_YELLOW, "\e[93m");
		strcpy((char*)FG_BRIGHT_BLUE, "\e[94m");
		strcpy((char*)FG_BRIGHT_MAGENTA, "\e[95m");
		strcpy((char*)FG_BRIGHT_CYAN, "\e[96m");
		strcpy((char*)FG_BRIGHT_WHITE, "\e[97m");

		strcpy((char*)BG_BLACK, "\e[40m");
		strcpy((char*)BG_RED, "\e[41m");
		strcpy((char*)BG_GREEN, "\e[42m");
		strcpy((char*)BG_YELLOW, "\e[43m");
		strcpy((char*)BG_BLUE, "\e[44m");
		strcpy((char*)BG_MAGENTA, "\e[45m");
		strcpy((char*)BG_CYAN, "\e[46m");
		strcpy((char*)BG_WHITE, "\e[47m");

		strcpy((char*)BG_BRIGHT_BLACK, "\e[100m");
		strcpy((char*)BG_BRIGHT_RED, "\e[101m");
		strcpy((char*)BG_BRIGHT_GREEN, "\e[102m");
		strcpy((char*)BG_BRIGHT_YELLOW, "\e[103m");
		strcpy((char*)BG_BRIGHT_BLUE, "\e[104m");
		strcpy((char*)BG_BRIGHT_MAGENTA, "\e[105m");
		strcpy((char*)BG_BRIGHT_CYAN, "\e[106m");
		strcpy((char*)BG_BRIGHT_WHITE, "\e[107m");

		strcpy((char*)NORMAL, "\e[0m");


		strcpy((char*)BOLD, "\e[1m"); // 
		strcpy((char*)UNDERLINE, "\e[4m"); // 
		strcpy((char*)TAB, "\eI"); // 
		strcpy((char*)LINEFEED, "\eJ"); // 
		strcpy((char*)CARRAIGERETURN, "\eM"); // 
		strcpy((char*)CRSRBACKSPACE, "\eH"); // 
		strcpy((char*)BELL, "\eG"); // 

		strcpy((char*)CLS, "\e[2J");
		strcpy((char*)MOV_CRSR_TOPLEFT, "\e[H"); 
		strcpy((char*)CLEAR_TO_REST_OF_LINE, "\e[K"); 
		strcpy((char*)MOV_CRSR_DOWN_N, "\e[*B");
		strcpy((char*)MOV_CRSR_UP_N, "\e[*A");
		strcpy((char*)MOV_CRSR_LEFT_COLS, "\e[*D");
		strcpy((char*)MOV_CRSR_RIGHT_COLS, "\e[*C");

		strcpy((char*)CLEAR_LINE, "\e[2K");
		strcpy((char*)CLEAR_LINE_CURSOR_RIGHT, "\e[0K");
		strcpy((char*)CLEAR_LINE_CURSOR_LEFT, "\e[1K");
		strcpy((char*)CLEAR_TO_REST_OF_SCREEN, "\e[J");
		strcpy((char*)SET_COLS_TO_132, "\e[?3h");
		strcpy((char*)SET_COLS_TO_80, "\e[?31");

		#else
		f=0;
		t=1;

		goto repeat;
		#endif
	}

	ret_stmt:
	return (ansivt = f);
}

char* fg( char* c )	{

	if(c!=0)
		ANSI->SetVT( c,(char*)0 );

	return ANSI->ANSIVT_FG;
}

char* bg( char* c )	{

	if(c!=0)
		ANSI->SetVT( (char* )0,c );

	return ANSI->ANSIVT_BG;
}

char* SetVT( char* fg, char* bg )	{

	char* fg_ = 0;
	char* bg_ = 0;

	int i;

	//FG:
	if( fg==(char* )0 )
		goto BG;

	for( i=0; i<16; i++ ){

		if( streq( fg, ANSI->ANSIVT_CTABLE[i][0] ) )	{

			fg_ = getstring( ANSI->ANSIVT_CTABLE[i][1] );
			printf( "%s", fg_ );
			ANSI->ANSIVT_FG = fg;
			ANSI->fg( fg_ );
			break;
		}
	}

	BG:
	if( bg==(char* )0 )
		goto NEXT;

	for( i=16; i<(16*2); i++ ){

		if( streq( bg, ANSI->ANSIVT_CTABLE[i][0] ) ){

			bg_ = getstring( ANSI->ANSIVT_CTABLE[i][1] );
			printf( "%s", bg_ );
			ANSI->ANSIVT_BG = bg;
			ANSI->bg( bg_ );
			break;
		}
	}

	NEXT:
	
	char* avtfg;
	char* avtbg;
	
	if( ANSIVT_FG==NULL )
		avtfg = "";
	else
		avtfg = ANSIVT_FG;
	
	if( ANSIVT_BG==NULL )
		avtbg = "";
	else
		avtbg = ANSIVT_BG;
	
	char* _ = malloc( strlen( "12345678" )+strlen( "12345678" ) + strlen("//") );

	_[0] = '\0';

	if( strlen( avtfg ) )	
		safecat( _, ANSI->ANSIVT_FG );
	else
		safecat( _, "Default" );

	safecat( _, "//" );	

	if( strlen( avtbg ) ) 
	safecat( _,ANSI->ANSIVT_BG );
	else
	safecat( _,"Default" );	

	return _;
}


void Init_ANSIVT_CTABLE( _ANSI_* ANSIobj ){

	char*** ANSIVT_CTABLE = ANSIobj->ANSIVT_CTABLE;

	uint32_t i=0;	
	//FG
	ANSIVT_CTABLE[i][0] = getstring( "black" ); // BASE + i + (0*4)
	ANSIVT_CTABLE[i][1] = FG_BLACK;

	i++;
	ANSIVT_CTABLE[i][0]= getstring( "red" );
	ANSIVT_CTABLE[i][1]= FG_RED;

	i++;
	ANSIVT_CTABLE[i][0]= getstring( "green" );
	ANSIVT_CTABLE[i][1]= FG_GREEN;

	i++;
	ANSIVT_CTABLE[i][0]= getstring( "yellow" );
	ANSIVT_CTABLE[i][1]= FG_YELLOW;

	i++;
	ANSIVT_CTABLE[i][0]= getstring( "blue" );
	ANSIVT_CTABLE[i][1]= FG_BLUE;

	i++;
	ANSIVT_CTABLE[i][0]= getstring( "magenta" );
	ANSIVT_CTABLE[i][1]= FG_MAGENTA;

	i++;
	ANSIVT_CTABLE[i][0]= getstring( "cyan" );
	ANSIVT_CTABLE[i][1]= FG_CYAN;

	i++;
	ANSIVT_CTABLE[i][0]= getstring( "white" );
	ANSIVT_CTABLE[i][1]= FG_WHITE;

	i++;
	ANSIVT_CTABLE[i][0]= getstring( "bright_black" );
	ANSIVT_CTABLE[i][1]= FG_BRIGHT_BLACK;

	i++;
	ANSIVT_CTABLE[i][0]= getstring( "bright_red" );
	ANSIVT_CTABLE[i][1]= FG_BRIGHT_RED;

	i++;
	ANSIVT_CTABLE[i][0]= getstring( "bright_green" );
	ANSIVT_CTABLE[i][1]= FG_BRIGHT_GREEN;

	i++;
	ANSIVT_CTABLE[i][0]= getstring( "bright_yellow" );
	ANSIVT_CTABLE[i][1]= FG_BRIGHT_YELLOW;

	i++;
	ANSIVT_CTABLE[i][0]= getstring( "bright_blue" );
	ANSIVT_CTABLE[i][1]= FG_BRIGHT_BLUE;

	i++;
	ANSIVT_CTABLE[i][0]= getstring( "bright_magenta" );
	ANSIVT_CTABLE[i][1]= FG_BRIGHT_MAGENTA;

	i++;
	ANSIVT_CTABLE[i][0]= getstring( "bright_cyan" );
	ANSIVT_CTABLE[i][1]= FG_BRIGHT_CYAN;

	i++;
	ANSIVT_CTABLE[i][0]= getstring( "bright_white" );
	ANSIVT_CTABLE[i][1]= FG_BRIGHT_WHITE;

	i++;
	// BG
	ANSIVT_CTABLE[i][0]= getstring( "black" );
	ANSIVT_CTABLE[i][1]= BG_BLACK;

	i++;
	ANSIVT_CTABLE[i][0]= getstring( "red" );
	ANSIVT_CTABLE[i][1]= BG_RED;

	i++;
	ANSIVT_CTABLE[i][0]= getstring( "green" );
	ANSIVT_CTABLE[i][1]= BG_GREEN;

	i++;
	ANSIVT_CTABLE[i][0]= getstring( "yellow" );
	ANSIVT_CTABLE[i][1]= BG_YELLOW;

	i++;
	ANSIVT_CTABLE[i][0]= getstring( "blue" );
	ANSIVT_CTABLE[i][1]= BG_BLUE;

	i++;
	ANSIVT_CTABLE[i][0]= getstring( "magenta" );
	ANSIVT_CTABLE[i][1]= BG_MAGENTA;

	i++;
	ANSIVT_CTABLE[i][0]= getstring( "cyan" );
	ANSIVT_CTABLE[i][1]= BG_CYAN;

	i++;
	ANSIVT_CTABLE[i][0]= getstring( "white" );
	ANSIVT_CTABLE[i][1]= BG_WHITE;

	i++;
	ANSIVT_CTABLE[i][0]= getstring( "bright_black" );
	ANSIVT_CTABLE[i][1]= BG_BRIGHT_BLACK;

	i++;
	ANSIVT_CTABLE[i][0]= getstring( "bright_red" );
	ANSIVT_CTABLE[i][1]= BG_BRIGHT_RED;

	i++;
	ANSIVT_CTABLE[i][0]= getstring( "bright_green" );
	ANSIVT_CTABLE[i][1]= BG_BRIGHT_GREEN;

	i++;
	ANSIVT_CTABLE[i][0]= getstring( "bright_yellow" );
	ANSIVT_CTABLE[i][1]= BG_BRIGHT_YELLOW;

	i++;
	ANSIVT_CTABLE[i][0]= getstring( "bright_blue" );
	ANSIVT_CTABLE[i][1]= BG_BRIGHT_BLUE;

	i++;
	ANSIVT_CTABLE[i][0]= getstring( "bright_magenta" );
	ANSIVT_CTABLE[i][1]= BG_BRIGHT_MAGENTA;

	i++;
	ANSIVT_CTABLE[i][0]= getstring( "bright_cyan" );
	ANSIVT_CTABLE[i][1]= BG_BRIGHT_CYAN;

	i++;
	ANSIVT_CTABLE[i][0]= getstring( "bright_white" );
	ANSIVT_CTABLE[i][1]= BG_BRIGHT_WHITE;

	i++;
	// RESET
	ANSIVT_CTABLE[i][0] = getstring( "default" );
	ANSIVT_CTABLE[i][1] = NORMAL;

	i++;
	// i contains a count of all the colour codes. Should be 33.
	ANSIobj->fg( "white" );
	ANSIobj->bg( "black" );

	return;
}

_ANSI_* colorMode()	{

	int color = 0;
	int x = 0;

	if( ansivt != 0 )	{

		/**
		// FOR PRE- RECENT VERSIONS OF WINDOWS 10, AND WINDOWS 11. To use in absence of built-in ANSI/VT
		// support, uncomment this block, and comment out the line after (not inside) this comment block:
		// color = 1;

		#ifdef WIN32

			// Assumes the target OS is Windows.
			#include <windows.h>
			#define STD_OUTPUT_HANDLE ((DWORD)-11)
			#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
			HANDLE StdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
			color = (int) SetConsoleMode(
				StdHandle,
				0x0001 | 0x0002 | ENABLE_VIRTUAL_TERMINAL_PROCESSING
			);

		#else
			// Assumes the target OS is Linux.
			color = 1;
		#endif

		*/

		color = 1;

		x = ( color==0 ? 0 : 1 );

		ResetAnsiVtCodes(x);

		if( x )	{

			ANSI = ActivateColorConsole();

			ANSIVT_FG = FG_BRIGHT_YELLOW;
			ANSIVT_BG = BG_BRIGHT_BLUE;
		}

	}
	else
		ResetAnsiVtCodes( 0 );

	if( x==0 )		
		printf( "ANSI/VT Support Not Available in this Win32-API console process.\n" );
	else
		printf( "%sCongratulations! ANSI/VT mode is functioning.%s\n", FG_BRIGHT_GREEN, NORMAL );

	return ANSI;
}

_ANSI_* ActivateColorConsole()	{


	ANSI = (_ANSI_*) malloc( sizeof(_ANSI_) );

	ANSI->RVC = ResetAnsiVtCodes;
	ANSI->getCodeString = getVTCodeString;

	ANSI->SetVT = SetVT;
	ANSI->fg = fg;	
	ANSI->bg = bg;

	ANSI->ANSIVT_FG = "default";
	ANSI->ANSIVT_BG = "default";

	char** twodarray = (char**)malloc( (37*2) * sizeof(char* ) );
	ANSI->ANSIVT_CTABLE = &twodarray;	
	//Init_ANSIVT_CTABLE();

	// Init_ANSIVT_CTABLE( ANSI );

	return ANSI;
}

char* getVTCodeString( char* cc )	{

	int i;
	for( i=0; i <33; i++ )	{

		if( !strcmp( ANSI->ANSIVT_CTABLE[i][0], cc)  )
			return ANSI->ANSIVT_CTABLE[i][1];
	}

	return (char*)NULL;
}
