# lexerparser

To use Lexer, 

\#include "./../path/to/lexer/repo/lexer.h"

make -C ./../path/to/lexer/repo/lexer/ lexer

You can do:

make -C ./../path/to/lexer/repo/lexer/ lexer CC=clang
for clang, and CC=tcc for TCC, etc...
 
You then need to create a LexInstance, which implies you can have multiple lexers within a given process life-time, possibly targetting different source-languages.

struct LexInstance\* lexer = initLex( source_string_filename, lexfile_filename );


