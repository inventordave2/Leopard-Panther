char* match_string( char* pattern, char* str )	{

	int e, ep;
	wregex_t *r;
	wregmatch_t *subm;

	char* token;

	r = wrx_comp(pattern, &e, &ep);

	if(!r) {

		fprintf(stderr,
			"\n[%s:%d] ERROR......: %s\n%s\n%d\n",
			lexer->sourceCodeFileName,
			lexer->carat,
			wrx_error(e),
			pattern,
			ep );

		token = NULL;
		break;
	}

	if(r->n_subm > 0) {

		subm = calloc(sizeof *subm, r->n_subm);
		if(!subm) {

			fprintf(stderr, "Error: out of memory (submatches). Exiting Program.\n");
			wrx_free(r);
			exit(EXIT_FAILURE);
		}
	}
	else
		subm = NULL;

	e = wrx_exec(r, str, &subm, &r->n_subm);

	if(e < 0)
		fprintf(stderr, "Error: %s\n", wrx_error(e));
	
	else if( e <= 0 )
		token_type = NULL;
	else	{
		
		token = (char*)malloc( subm[1].end - subm[1].beg + 1 );
		strncpy( token, subm[1].beg, subm[1].end - subm[1].beg );
		token[ subm[1].end - subm[1].beg ] = '\0';
	}
	
	free(subm);
	wrx_free(r);
	
	// If we got here, wregex has either matched the pattern to the rule, or has failed to find a match.
	return token;
}

