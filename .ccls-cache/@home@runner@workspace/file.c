// FILE_C

struct FileContext readFile( char* fn )	{

	struct FileContext fileContext;

	FILE* f;
	f = fopen( fn, "r" );
	fseek( f, 0, SEEK_END );
	int flen = ftell( f );
	fseek( f, 0, SEEK_SET );

	fileContext.fileContents = (char*) calloc( flen, sizeof(char) );

	int x = 0;
	c = fgetc( f );

	while( c != EOF )	{

		fileContext.fileContents[ x++ ]  = c;
		c = fgetc( f );
	}

	fileContext.fileContents[ x ] = '\0';
	fclose( f );

	fileContext.f = f;
	fileContext.length = flen;

	return fileContext;
}

