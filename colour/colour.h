#ifndef DAVELIB_COLOUR_H
#define DAVELIB_COLOUR_H 


extern char ansivt;
#define colouriser colorMode
#define colourizer colouriser

// The Current Active Colour Selections.
extern char* ANSIVT_FG;
extern char* ANSIVT_BG;

// These are constant def's, the number of colours available in VT100 Emulation.
#define MAX_FG_COLORS 16
#define MAX_BG_COLORS 16

extern char* getVTCodeString( char* );
extern char ResetAnsiVtCodes(char);
extern char* SetVT( char*, char* ); //fg and bg colour.
extern char* fmt( int sc ); // eg, +i, +b, -i, -b, and so on.
extern char* fg( char* );
extern char* bg( char* );


typedef struct _ANSI_ {	

	char* ansivt;
	char* ANSIVT_FG;
	char* ANSIVT_BG;

	char (*RVC)(char);
	char* (*getCodeString)(char*);
	char* (*SetVT)( char*, char* ); //fg and bg colour.

	char* (*fg)( char* );
	char* (*bg)( char* );
	char* (*f)( char* ); // eg, +i, +b, -i, -b, and so on.

	char* * ANSIVT_CTABLE;

} _ANSI_;

extern struct _ANSI_* ANSI;

extern struct _ANSI_* colorMode();
extern struct _ANSI_* ActivateColorConsole();
extern void Init_ANSIVT_CTABLE( struct _ANSI_* );
#define colouriser colorMode
#define colourizer colouriser

// The Current Active Colour Selections.
extern char* ANSIVT_FG;
extern char* ANSIVT_BG;

// These are constant def's, the number of colours available in VT100 Emulation.
#define MAX_FG_COLORS 16
#define MAX_BG_COLORS 16

// Get VT100 escape sequence based on indice.
extern char* getVTCodeString( char* );
extern char* SetVT( char*, char* ); // Set FG and BG colour. Below are single-setting functions.
extern char* fmt( int sc ); // eg, +i, +b, -i, -b, and so on.

// For VT100 escape sequences that require an 'n' value, for a count for example. Pass the intrinsic code template-string along with a value 'v' to replace 'n'
// (which is actually '*' in the internal strings in this library).
char* setVal( char* _, short v );

// Set FG or BG Colour.
extern char* fg( char* );
extern char* bg( char* );

// resets code. if Arg = 1, populates control-code strings. If Arg=0, blanks them, resulting in no VT100 colourization.
extern char resetAnsiVtCodes(char);


// generates the ansivt-formatted output string.
extern char* ANSIVT( char* str, char** cc, long long int *offsets );

#define VTCODEWIDTH 8

extern char* FG_BLACK;
extern char* FG_RED;
extern char* FG_GREEN;
extern char* FG_YELLOW;
extern char* FG_BLUE;
extern char* FG_MAGENTA;
extern char* FG_CYAN;
extern char* FG_WHITE;

extern char* FG_BRIGHT_BLACK;
extern char* FG_BRIGHT_RED;
extern char* FG_BRIGHT_GREEN;
extern char* FG_BRIGHT_YELLOW;
extern char* FG_BRIGHT_BLUE;
extern char* FG_BRIGHT_MAGENTA;
extern char* FG_BRIGHT_CYAN;
extern char* FG_BRIGHT_WHITE;

extern char* BG_BLACK;
extern char* BG_RED;
extern char* BG_GREEN;
extern char* BG_YELLOW;
extern char* BG_BLUE;
extern char* BG_MAGENTA;
extern char* BG_CYAN;
extern char* BG_WHITE;

extern char* BG_BRIGHT_BLACK;
extern char* BG_BRIGHT_RED;
extern char* BG_BRIGHT_GREEN;
extern char* BG_BRIGHT_YELLOW;
extern char* BG_BRIGHT_BLUE;
extern char* BG_BRIGHT_MAGENTA;
extern char* BG_BRIGHT_CYAN;
extern char* BG_BRIGHT_WHITE;

extern char* NORMAL;

extern char* BOLD; // \e[1m
extern char* UNDERLINE; // \e[4m
extern char* TAB; // \eI
extern char* LINEFEED; // \eJ
extern char* CARRAIGERETURN; // \eM
extern char* CRSRBACKSPACE; // \eH
extern char* BELL; // \eG

extern char* CLS; // \e[2J
extern char* CLEAR_LINE; // \e[2K
extern char* CLEAR_LINE_CURSOR_RIGHT; // \e[0K
extern char* CLEAR_LINE_CURSOR_LEFT; // \e[1K
extern char* MOV_CRSR_TOPLEFT; // \033[H , \e[H
extern char* CLEAR_TO_REST_OF_LINE; // \e[K
extern char* CLEAR_TO_REST_OF_SCREEN; // \e[J
extern char* MOV_CRSR_DOWN_N; // \e[<n>B
extern char* MOV_CRSR_UP_N; // \e[<n>A
extern char* MOV_CRSR_LEFT_COLS; // \e[<n>D
extern char* MOV_CRSR_RIGHT_COLS; // \e[<n>C

extern char* SET_COLS_TO_132; // \e[?3h
extern char* SET_COLS_TO_80; // \e[?31

#endif
