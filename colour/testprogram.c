// TESTPROGRAM_C

#include <stdio.h>
#include "colour.h"

int main( int argc, char **argv )	{

	colorMode();
	printf( "ANSI->ansivt is set to %s%d%s.\n", FG_BRIGHT_YELLOW, ANSI->ansivt, NORMAL );
	return 0;
}

