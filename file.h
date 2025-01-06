

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

char** split( char* line, char delim );

