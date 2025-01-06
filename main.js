var YaccRules = new Array();

/*

ws: [\w]
VARIABLE_NAME: [a-zA-Z_]+[a-zA-Z_0-9]*
STATEMENT_LIST: STATEMENT | STATEMENT STATEMENT_LIST
FNC_BLOCK: '{' [\w]* STATEMENT_LIST [\w]*  '}'
FNC_DEF: ["function"|"FUNCTION"] [\w]+ VARIABLE_NAME [\w]* '(' [\w]* FNC_ARG_LIST [\w]* ')' [\w]* FNC_BLOCK

FNC_ARG_LIST: VARIABLE_NAME | VARIABLE_NAME ',' FNC_ARG_LIST

VARIABLE_DECL: "var"|"VAR"|"let" VARIABLE_NAME ('=' VALUE)? ';'* 

FNC_VARIABLE: VARIABLE_NAME '(' FNC_ARG_LIST? ')'
VALUE: LITERAL | VARIABLE_NAME | FNC_VARIABLE

NUM: [0-9a-fA-F]+[\.]?[0-9a-fA_F]

CHARSET_NSQ:
CHARSET_NDQ:

OBJ_NOTATION: VARIABLE_NAME ':' (VARIABLE_NAME|FNC_VARIABLE|LITERAL) ','?
OBJ_LITERAL_INNER: STATEMENT_LIST? OBJ_NOTATION+ OBJ_LITERAL_INNER | OBJ_NOTATION+ STATEMENT_LIST? OBJ_LITERAL_INNER 
OBJ_LITERAL: '{' OBJ_LITERAL_INNER '}'
STRING: "'" CHARSET_NSQ "'" | "\"" CHARSET_NDQ "\""
LITERAL: NUM | STRING | OBJ_LITERAL
*/

function wollfereast()	{
// Nabbed from a poster on Stackoverflow.com called "Wollfer-East" at https://stackoverflow.com/questions/5989315/regex-for-match-replacing-javascript-comments-both-multiline-and-inline

var ADW_GLOBALS = new Object
ADW_GLOBALS = {
  quotations : /((["'])(?:(?:\\\\)|\\\2|(?!\\\2)\\|(?!\2).|[\n\r])*\2)/,
  multiline_comment : /(\/\*(?:(?!\*\/).|[\n\r])*\*\/)/,
  single_line_comment : /(\/\/[^\n\r]*[\n\r]+)/,
  regex_literal : /(?:\/(?:(?:(?!\\*\/).)|\\\\|\\\/|[^\\]\[(?:\\\\|\\\]|[^]])+\])+\/)/,
  html_comments : /(<!--(?:(?!-->).)*-->)/,
  regex_of_doom : ''
}
ADW_GLOBALS.regex_of_doom = new RegExp(
  '(?:' + ADW_GLOBALS.quotations.source + '|' + 
  ADW_GLOBALS.multiline_comment.source + '|' + 
  ADW_GLOBALS.single_line_comment.source + '|' + 
  '((?:=|:)\\s*' + ADW_GLOBALS.regex_literal.source + ')|(' + 
  ADW_GLOBALS.regex_literal.source + '[gimy]?\\.(?:exec|test|match|search|replace|split)\\(' + ')|(' + 
  '\\.(?:exec|test|match|search|replace|split)\\(' + ADW_GLOBALS.regex_literal.source + ')|' +
  ADW_GLOBALS.html_comments.source + ')' , 'g'
);

changed_text = code_to_test.replace(ADW_GLOBALS.regex_of_doom, function(match, $1, $2, $3, $4, $5, $6, $7, $8, offset, original){
  if (typeof $1 != 'undefined') return $1;
  if (typeof $5 != 'undefined') return $5;
  if (typeof $6 != 'undefined') return $6;
  if (typeof $7 != 'undefined') return $7;
  return '';
})	
	
}

function initYR( yaccfile )	{

	var YRFileContents = yaccfile.split( "\n" );
	
	for( var i=0; i<YRFileContents.length; i++ ){
	
		let fline = YRFileContents[i];

//		([a-zA-Z_]+[a-zA-Z_0-9]*)\:
//		let ruleName = / ... /;
//		let rules = / ... /;
		
		YaccRules[i] = new Array( 2 );
		YaccRules[i][0] = ruleName;
		YaccRules[i][1] = rules;
	}

	return;
}

function get( NTT )	{
	
	for( var i=0; i<YaccRules.length; i++ )
		if( YaccRules[i][0]==NTT )
			return YaccRules[i][1];
		
	return [];
}


let stackPointer = 0;
function nextToken()	{
	
	if( stack.length<=stackPointer )
		return false;
	
	let r = stack[stackPointer++];
	
	return r;
}


function Parse( NTT )	{

	let T = undefined;
	let prules = get( NTT ).split(" | ");
	
	for( var i=0; i<prules.length; i++ ){
		
		let rule = prules[i].split(" ");
		
		for( var k=0; k<rule.length; k++ ){

			let entry = rule[k];
			if( entry.trim()=="" )
			continue;
			
			if( (T=nextToken() )[pattern] == entry )			continue;
		
			return false;
		}
		
		return NTT;
	}
}


let FileContent = 'not loaded yet';
let lfc = 'no lex file';

let stack = 0;

function runlexer( t )	{
	
	let U = new Utils();
	
	if( t==undefined ){
	let s1 = U.c( "brightYellow", "gray" );
	var t = 0;
	console.log(s1, `Please run runlexer(t) with a Token offset to display that Lex'd token. Invoking 'runlexer( 0 )'.`);
	
	return runlexer(t);
	}
	
	//const { fs } = require('node:fs');
	//let scFileName = "aplib.c"
	let scContents = FileContent; //loadSourceFile( scFileName, fs );
	
	let lexer = new Lexer(scContents)
	lexer.init()
	
	stack = lexer.lex()	
	
	console.log('\x1b[36m%s\x1b[0m', "String: '" + stack[ t ].str + "'");
	console.log('\x1b[35m%s\x1b[0m', "TOKEN:  '" + stack[ t ].pattern + "'");
	
	
	
	let s1 = U.c("brightYellow", "brightBlue", "rv")
	//console.log(s1, "Quick Test!");
	
	//console.log("TEST1: ");
	//console.log(U._colTest(), "Test!!!")
	
	return lexer;
}

class Lexer	{

	//const { fs } = require('node:fs');
	//const fs = require('node:fs');	
	
	LexPatterns = []
	LexTokens = []
	LexTokens_rev = []
	Stack = []
	s = ''
	//_fs = fs;
	fs = undefined;
	statusMsg = ''
	errorRecord = {}
	
	init()	{
	
		//this.prepLexFile(lfc);
		let _ = new this.installLexRules(this)
		//console.log("TEST4");
	}
	
	constructor(s, lexRules)	{
		
		this.s = s || 'You forgot to provide a language string to tokenize, silly!'
		//this.fs = fs;
		
		if(lexRules)
			this.installLexRules = lexRules;
			
		else
			this.installLexRules = installLexRules;
		
		//init();
	}
	
	
	lex(s)	{
		
		if(this.LexPatterns.length<1)	{
		
			console.error("%cNo patterns & token types registered with this Lexer!", new Utils().error)
			return;
		}
		
		if(s)
			this.s = s
		
		s = String(this.s)
		
		let match = false
		let matches = []
		let IN = new String(s)
		
		let curr_s_length_matched = false
		let curr_s_length_matches = [{str: 'Null Entry', pattern: /\./ }]
		let i = 1
		let t_str = '';
		
		while(s)	{

			matches = []

			if(curr_s_length_matched)	{

				this.Stack.push(curr_s_length_matches.pop())
				//console.info(`%cPushed token: '${Stack[Stack.length-1].str}', (${Stack[Stack.length-1].pattern.toString()}) onto Stack.`, new Utils().info)

				curr_s_length_matched = false
				curr_s_length_matches = []

				if(s.length-(i-1)<=0)	{

					this.statusMsg = 'SUCCESS'
					this.errorRecord = {}
					
					console.log("%cSUCCESS: Lexer tokenized " + this.Stack.length + " tokens.", new Utils().success)
					return this.Stack;
				}

				s = s.substring(i-1)
				i = 1;
			}


			do	{

				let j = 0
				t_str = s.substring(0, i)
				match = false

				if(s.length<i)
					j = Number.MAX_SAFE_INTEGER - 1

				for (; j < this.LexPatterns.length; j++)
					if(this.LexPatterns[j].test(t_str))	{

						//console.log(`%cMatched pattern ${LexTokens[j]} via string: ${t_str}`, new Utils().success)
						match = true
						break;
					}

				if(match)	{

					matches.push({ str: t_str, pattern: this.LexTokens[j] })
					i++
				}

			} while(match)

			if(matches.length>0)	{

				let k = matches.length - 1
				
				let curr_len = matches[k].str.length
				let prev_len = curr_len

				let r_prev = null
				let r = null

				for(; k > -1; k--)	{

					
					r = matches.pop()
					curr_len = r.str.length

					if (prev_len == curr_len)	{

						r_prev = r
						prev_len = r.str.length
					}
					else
						break;
				}

				// r_prev contains "1st" & longest entry/match, unless r_prev
				// is null, in which case "r" contains the hit.
				if(!r_prev)
					curr_s_length_matches.push(r)
				else
					curr_s_length_matches.push(r_prev)

				
				curr_s_length_matched = true
				matches = [];
			}

			else { // No Matches, so Abort

				this.statusMsg = 'ERROR'
				console.error("%cError! Invalid String!", new Utils().error)
				return (this.errorRecord = { 'offset': IN.length - s.length, 'char': s[0] });
			}
		}
		
		return this.stack;
	}
	
	
    installToken(pattern, tokenStr) {

        this.LexPatterns.push(new RegExp(`^${pattern}$`))
        this.LexTokens.push(tokenStr)
        this.LexTokens_rev[tokenStr] = this.LexPatterns.length - 1
            
        return this.LexPatterns.length - 1;
    }

    prepLexFile(lfc) {

		let lines = lfc.split("\n")

		for(var i = 0; i < lines.length; i++)   {
			
			let line = lines[i]
			if(line.trim()=='')
				continue
			
			let lr = line.split("$$")

			this.installToken(lr[0].trim(), lr[1].trim()); 
		}
		
		return true;
    }
}

class installLexRules	{
	
	constructor(lexer)	{
		
		this.lexer = lexer;

		this.installToken("[\\/][\\/][\\w\\W\\s\\S]*[\\n\\r]", "SL_COMMENT");
		this.installToken("[\\/]+[\\*]+[\\n\\S\\s]*[\\*]+\\/", "ML_COMMENT");
		
		this.installToken("[\\n\\r]", "NL")		
		this.installToken("[\\s]", "WS")
		this.installToken("[\\t]", "TAB")

		this.installToken("var", "VAR_KWD")
		this.installToken("continue", "CONT_KWD")
		this.installToken("break", "BREAK_KWD")
		this.installToken("function", "FNC_KWD")
		this.installToken("while", "WHILE_KWD")
		this.installToken("for", "FOR_KWD")
		this.installToken("if", "IF_KWD")
		this.installToken("do", "DO_KWD")
		this.installToken("return", "RETURN_KWD")
		this.installToken("this", "THIS_KWD")

		this.installToken("[a-zA-Z_](([a-zA-Z_0-9])+)?", "VARIABLE")

		this.installToken("[0-9]+", "NUM")
		this.installToken("[0-9]+([\\.][0-9+])", "FP")

		this.installToken("[\\+]", "PLUS")
		this.installToken("[\\-]", "MINUS")
		this.installToken("[\\*]", "MUL")
		this.installToken("[\\/]", "DIV")
		this.installToken("[\\^]", "EXP")

		this.installToken("===", "STRICT_EQ")
		this.installToken("[\\=][\\=]", "EQ")
		this.installToken("[\\=]", "EQUALS")

		this.installToken("[\\>]", "GT")
		this.installToken("[\\<]", "LT")

		this.installToken("[\\(]", "OPEN_BRACK")
		this.installToken("[\\)]", "CLOSE_BRACK")
		this.installToken("[\\[]", "OPEN_SQ_BRACK")
		this.installToken("[\\]]", "CLOSE_SQ_BRACK")
		this.installToken("[\\{]", "OPEN_CURLY_BRACK")
		this.installToken("[\\}]", "CLOSE_CURLY_BRACK")
		this.installToken("[\\\\]", "BACK_SLASH")

		this.installToken("[\\?]", "Q_MARK")
		this.installToken("[\\$]", "DOLLAR")
		this.installToken("[\\#]", "HASH")
		this.installToken("[\\%]", "MODULO")


		this.installToken("[\\.]", "PERIOD")//
		this.installToken("[\\;]", "SCOLON")
		this.installToken("[\\:]", "COLON")
		this.installToken("[\\,]", "COMMA")

		this.installToken('[\\"]', "DBL_QUOTE")
		this.installToken("[\\']", "S_QUOTE")
		this.installToken("[\\`]", "BACK_TICK")

		this.installToken("[\\!]", "NOT")
		this.installToken("[\\&]", "AMP")
		this.installToken("[\\|]", "OR")
		this.installToken("[\\~]", "BITWISE_NOT")

		return this.lexer;
	}
	
	installToken(pattern, tokenStr)	{

		//console.log("TEST2")

		let wrapped_pattern = new RegExp( `^${pattern}$` )//
		this.lexer.LexPatterns.push(wrapped_pattern)
		this.lexer.LexTokens.push(tokenStr)
		this.lexer.LexTokens_rev[tokenStr] = this.lexer.LexPatterns.length - 1
		
		//console.log("TEST3")
		return this.lexer.LexPatterns.length - 1;
	};
}

class Utils	{
	
	constructor()	{
		
	}

	success="background: green; color: yellow;"
	error = "background: red; color: yellow;"
	warn  = "background: yellow; color: black;"
	info  = "background: blue; color: yellow;"
	
	black = {fg:30, bg:40, rgb:{r:0,g:0,b:0}}
	green = {fg:32, bg:42, rgb:{r:0,g:255,b:0}}
	red =	{fg:31, bg:41, rgb:{r:255,g:0,b:0}}
	yellow ={fg:33, bg:43}
	blue =	{fg:34, bg:44}
	magenta ={fg:35, bg:45}
	cyan =	{fg:36, bg:46}
	white =	{fg:37, bg:47}
	gray =	{fg:90, bg:100}
	brightRed ={fg:91, bg:101}
	brightGreen ={fg:92, bg:102}
	brightYellow ={fg:93, bg:103}
	brightBlue = {fg:94, bg:104}
	brightMagenta ={fg:95, bg:105}
	brightCyan ={fg:96, bg:106}
	brightWhite = {fg:97, bg:107}
	
	//_col1 = (16 + 36 * + R + 6 * G + B)
	// where r,g and b are 0 -> 5.
	// 2
	// https://en.wikipedia.org/wiki/ANSI_escape_code#Colors
	/*
	
	// 8-bit:
	ESC[38;5;⟨n⟩m Select foreground color
ESC[48;5;⟨n⟩m Select background color
  0-  7:  standard colors (as in ESC [ 30–37 m)
  8- 15:  high intensity colors (as in ESC [ 90–97 m)
 16-231:  6 × 6 × 6 cube (216 colors): 16 + 36 × r + 6 × g + b (0 ≤ r, g, b ≤ 5)
232-255:  grayscale from black to white in 24 steps
	
	*/

	_colTest()	{
		
		// `\x1b[0m`
		// 3-4 bit
		return "\x1b[48;5;"+this._col1+"m" + "%s\x1b[0m";
	}
	
	c(fg,bg,opt)	{
		
		// \x1b[36m%s\x1b[0m
		// 0m resets all colors
		// \x1b is the ESC[ prefix for selecting a color
		// see: https://en.wikipedia.org/wiki/ANSI_escape_code#Colors
		
		if(!(fg.length>0))	{
			
			switch(fg)	{
				
				case 0: fg = "black"; break;
				case 1: fg = "red"; break;
				case 2: fg = "green"; break;
				case 3: fg = "yellow"; break;
				case 4: fg = "blue"; break;
				case 5: fg = "magenta"; break;
				case 6: fg = "cyan"; break;
				case 7: fg = "white"; break;
				case 8: fg = "gray"; break;
				case 9: fg = "brightRed"; break;
				case 10:fg = "brightGreen"; break;
				case 11:fg = "brightYellow"; break;
				case 12:fg = "brightBlue"; break;
				case 13:fg = "brightMagenta"; break;
				case 14:fg = "brightCyan"; break;
				case 15:fg = "brightWhite"; break;
			
				default:
					fg = "white"; break;
			}		
		}

		if(!(bg.length>0))	{
			
			switch(bg)	{
				
				case 0: bg = "black"; break;
				case 1: bg = "red"; break;
				case 2: bg = "green"; break;
				case 3: bg = "yellow"; break;
				case 4: bg = "blue"; break;
				case 5: bg = "magenta"; break;
				case 6: bg = "cyan"; break;
				case 7: bg = "white"; break;
				case 8: bg = "gray"; break;
				case 9: bg = "brightRed"; break;
				case 10:bg = "brightGreen"; break;
				case 11:bg = "brightYellow"; break;
				case 12:bg = "brightBlue"; break;
				case 13:bg = "brightMagenta"; break;
				case 14:bg = "brightCyan"; break;
				case 15:bg = "brightWhite"; break;
			
				default:
					bg = "white"; break;
			}		
		}		
		
		let _fg = 7, _bg = 0;
		
		try	{
			_fg = this[fg].fg
			_bg = this[bg].bg
		}
		catch(e)	{
			
			// Dodgy colour refs passed in.
			console.log("\x1b["+this['brightYellow'].fg+"m"
			+ "\x1b["+this['brightBlue'].bg+"m"
			+ "%s" + "\x1b[0m",
			"The color values '"+fg+"' and '"+bg+"' are not accepted as a FG/BG color-pair by Utils. Is there a typo in one of them?");
			
			_fg = this["white"].fg, _bg = this["black"].bg;
		}
		
		let fStr = '';
		
		if(opt)	{
			/**
			Underscore = "\x1b[4m"
			Blink = "\x1b[5m"
			Reverse = "\x1b[7m"
			*/
			switch(opt)	{
				
				case "us": fStr += "\x1b[4m"; break;
				case "bl": fStr += "\x1b[5m"; break; // NB: blink and reverse don't work Node.js cmd-line/Windows 10
				case "rv": fStr += "\x1b[7m"; break;
				default: break;
			}
		}
			
		
		return (this.lfs = fStr + `\x1b[${_fg}m` + `\x1b[${_bg}m` + "%s" + `\x1b[0m`);
	}
	
	stub()	{
	
		return [];
	}
	
	swap(a, b)	{
		
		return { b, a };
	} // syntax: { b, a } = new Utils().swap(a, b) // but wouldn't { b, a } = { a, b } work as an inline?
	
	max(a, b)	{
		
		if(a>=b)
			return a;
		
		return b;
	}
	
	min(a, b)	{
		
		if(a<=b)
			return a;
		
		return b;
	}
	
	abs(a)	{
		
		if((a<0.0)||(a==-0.0))
			return -a;
		
		return a;
	}	
}



var str2 = "// Lexer.js\n\nimport fs from \"fs\";\n\n\nexport class Lexer\t{\n\t\n\tLexPatterns = []\n\tLexTokens = []\n\tLexTokens_rev = []\n\tStack = []\n\ts = \'\'\n\t_fs = fs\n\t\n\tstatusMsg = \'\'\n\terrorRecord = {}\n\t\n\tconstructor(s, lexRules)\t{\n\t\t\n\t\tthis.s = s || \'You forgot to provide a language string to tokenize, silly!\'\n\t\t\n\t\tif(lexRules)\n\t\t\tthis.installLexRules = lexRules;\n\t\t\t\n\t\telse\n\t\t\tthis.installLexRules = installLexRules;\n\t\t\n\t\t//(new this.installLexRules(this));\n\t}\n\t\n\tlex(s)\t{\n\t\t\n\t\tif(this.LexPatterns.length<1)\t{\n\t\t\n\t\t\tconsole.error(\"%cNo patterns & token types registered with this Lexer!\", new Utils().info)\n\t\t\treturn;\n\t\t}\n\t\t\n\t\tif(s)\n\t\t\tthis.s = s\n\t\t\n\t\ts = String(this.s)\n\t\t\n\t\tlet match = false\n\t\tlet matches = []\n\t\tlet IN = new String(s)\n\t\t\n\t\tlet curr_s_length_matched = false\n\t\tlet curr_s_length_matches = [{str: \'Null Entry\', pattern: /\./ }]\n\t\tlet i = 1\n\t\tlet t_str = \'\';\n\t\t\n\t\twhile(s)\t{\n\n\t\t\tmatches = []\n\n\t\t\tif(curr_s_length_matched)\t{\n\n\t\t\t\tthis.Stack.push(curr_s_length_matches.pop())\n\t\t\t\t//console.info(`%cPushed token: \'${Stack[Stack.length-1].str}\', (${Stack[Stack.length-1].pattern.toString()}) onto Stack.`, new Utils().info)\n\n\t\t\t\tcurr_s_length_matched = false\n\t\t\t\tcurr_s_length_matches = []\n\n\t\t\t\tif(s.length-(i-1)<=0)\t{\n\n\t\t\t\t\tthis.statusMsg = \'SUCCESS\'\n\t\t\t\t\tthis.errorRecord = {}\n\t\t\t\t\t\n\t\t\t\t\tconsole.log(\"%cSUCCESS: Parsed \" + this.Stack.length + \" tokens.\", new Utils().success)\n\t\t\t\t\treturn this.Stack;\n\t\t\t\t}\n\n\t\t\t\ts = s.substring(i-1)\n\t\t\t\ti = 1;\n\t\t\t}\n\n\n\t\t\tdo\t{\n\n\t\t\t\tlet j = 0\n\t\t\t\tt_str = s.substring(0, i)\n\t\t\t\tmatch = false\n\n\t\t\t\tif(s.length<i)\n\t\t\t\t\tj = Number.MAX_SAFE_INTEGER - 1\n\n\t\t\t\tfor (; j < this.LexPatterns.length; j++)\n\t\t\t\t\tif(this.LexPatterns[j].test(t_str))\t{\n\n\t\t\t\t\t\t//console.log(`%cMatched pattern ${LexTokens[j]} via string: ${t_str}`, new Utils().success)\n\t\t\t\t\t\tmatch = true\n\t\t\t\t\t\tbreak;\n\t\t\t\t\t}\n\n\t\t\t\tif(match)\t{\n\n\t\t\t\t\tmatches.push({ str: t_str, pattern: this.LexTokens[j] })\n\t\t\t\t\ti++\n\t\t\t\t}\n\n\t\t\t} while(match)\n\n\t\t\tif(matches.length>0)\t{\n\n\t\t\t\tlet k = matches.length - 1\n\t\t\t\tlet curr_len = matches[k].str.length\n\t\t\t\tlet prev_len = curr_len\n\n\t\t\t\tlet r_prev = null\n\t\t\t\tlet r = null\n\n\t\t\t\tfor(; k > -1; k--)\t{\n\n\t\t\t\t\t\n\t\t\t\t\tr = matches.pop()\n\t\t\t\t\tcurr_len = r.str.length\n\n\t\t\t\t\tif (prev_len == curr_len)\t{\n\n\t\t\t\t\t\tr_prev = r\n\t\t\t\t\t\tprev_len = r.str.length\n\t\t\t\t\t}\n\t\t\t\t\telse\n\t\t\t\t\t\tbreak;\n\t\t\t\t}\n\n\t\t\t\t// r_prev contains \"1st\" & longest entry/match, unless r_prev\n\t\t\t\t// is null, in which case \"r\" contains the hit.\n\t\t\t\tif(!r_prev)\n\t\t\t\t\tcurr_s_length_matches.push(r)\n\t\t\t\telse\n\t\t\t\t\tcurr_s_length_matches.push(r_prev)\n\n\t\t\t\t\n\t\t\t\tcurr_s_length_matched = true\n\t\t\t\tmatches = [];\n\t\t\t}\n\n\t\t\telse { // No Matches, so Abort\n\n\t\t\t\tthis.statusMsg = \'ERROR\'\n\t\t\t\tconsole.error(\"%cError! Invalid String!\", new Utils().error)\n\t\t\t\treturn (this.errorRecord = { \'offset\': IN.length - s.length, \'char\': s[0] });\n\t\t\t}\n\t\t}\n\t\t\n\t}\n\t\n\t\n    installToken(pattern, tokenStr) {\n\n        this.LexPatterns.push(new RegExp(`^${pattern}\$`))\n        this.LexTokens.push(tokenStr)\n        this.LexTokens_rev[tokenStr] = this.LexPatterns.length - 1\n            \n        return this.LexPatterns.length - 1;\n    }\n\t\n    loadLexFile(fn) {\n\n        this._fs.open(fn, \"r\", function(err, file)    {\n\n            if(err) {\n                return undefined;\n            }\n        });\n\n        this._fs.readFile(fn, function(err, data) {\n\n            if(err)\n                return `Error: ${err}`;\n\n            let lines = data.split(\"\n\")\n\n            for(var i = 0; i < lines.length; i++)   {\n                \n                let line = lines[i]\n\t\t\t\tif(line.trim()==\'\')\n\t\t\t\t\tcontinue\n\t\t\t\t\n                let lr = line.split(\"$$\")\n\n                this.installToken(lr[0].trim(), lr[1].trim()); \n            }\n\t\t\t\n\t\t\treturn true;\n        });\n    }\n}\n\n\nexport class installLexRules\t{\n\t\n\tconstructor(lexer)\t{\n\t\t\n\t\tthis.lexer = lexer;\n\t\t\n\t\tthis.installToken(\"[\\s]+\", \"WS\")\n\t\tthis.installToken(\"[\\t]\", \"TAB\")\n\t\tthis.installToken(\"[\\n]\", \"NL\")\n\t\t\n\t\tthis.installToken(\"[a-zA-Z_](([a-zA-Z_0-9])+)?\", \"VARIABLE\")\n\n\t\tthis.installToken(\"var\", \"VAR_KWD\")\n\t\tthis.installToken(\"continue\", \"CONT_KWD\")\n\t\tthis.installToken(\"break\", \"BREAK_KWD\")\n\t\tthis.installToken(\"function\", \"FNC_KWD\")\n\t\tthis.installToken(\"while\", \"WHILE_KWD\")\n\t\tthis.installToken(\"for\", \"FOR_KWD\")\n\t\tthis.installToken(\"if\", \"IF_KWD\")\n\t\tthis.installToken(\"do\", \"DO_KWD\")\n\n\t\tthis.installToken(\"[0-9]+\", \"NUM\")\n\t\tthis.installToken(\"[0-9]+([\\.][0-9+])\", \"FP\")\n\n\t\tthis.installToken(\"[\\+]\", \"PLUS\")\n\t\tthis.installToken(\"[\\-]\", \"MINUS\")\n\t\tthis.installToken(\"[\\*]\", \"MUL\")\n\t\tthis.installToken(\"[/]\", \"DIV\")\n\t\tthis.installToken(\"[\\^]\", \"EXP\")\n\n\t\tthis.installToken(\"===\", \"STRICT_EQ\")\n\t\tthis.installToken(\"==\", \"EQ\")\n\t\tthis.installToken(\"[\\=]\", \"EQUALS\")\n\n\t\tthis.installToken(\"[\\>]\", \"GT\")\n\t\tthis.installToken(\"[\\<]\", \"LT\")\n\n\t\tthis.installToken(\"[\\(]\", \"OPEN_BRACK\")\n\t\tthis.installToken(\"[\\)]\", \"CLOSE_BRACK\")\n\t\tthis.installToken(\"[\\[]\", \"OPEN_SQ_BRACK\")\n\t\tthis.installToken(\"[\\]]\", \"CLOSE_SQ_BRACK\")\n\t\tthis.installToken(\"[\\{]\", \"OPEN_CURLY_BRACK\")\n\t\tthis.installToken(\"[\\}]\", \"CLOSE_CURLY_BRACK\")\n\t\tthis.installToken(\"[\\\\]\", \"BACK_SLASH\")\n\n\t\tthis.installToken(\"[\\?]\", \"Q_MARK\")\n\t\tthis.installToken(\"[\\$]\", \"DOLLAR\")\n\t\tthis.installToken(\"[\\#]\", \"HASH\")\n\t\tthis.installToken(\"[\%]\", \"PERCENT\")\n\n\n\t\tthis.installToken(\"[\\.]\", \"PERIOD\")\n\t\tthis.installToken(\"[;]\", \"SCOLON\")\n\t\tthis.installToken(\"[\\:]\", \"COLON\")\n\t\tthis.installToken(\"[\\,]\", \"COMMA\")\n\n\t\tthis.installToken(\'\"\', \"DBL_QUOTE\")\n\t\tthis.installToken(\"[\\']\", \"S_QUOTE\")\n\t\tthis.installToken(\"[\`]\", \"BACK_TICK\")\n\n\t\tthis.installToken(\"[\\!]\", \"NOT\")\n\t\tthis.installToken(\"\\&\", \"AMP\")\n\t\tthis.installToken(\"\\|\", \"OR\")\n\t\tthis.installToken(\"\\~\", \"BITWISE_NOT\")\n\n\t\tthis.installToken(\"//[\S\s]*[\n]+\", \"SL_COMMENT\");\n\t\tthis.installToken(\"/[\*][\*]*[\n\S\s]*[\*][/]\", \"ML_COMMENT\");\n\t}\n\t\n\t\n\tinstallToken(pattern, tokenStr)\t{\n\t\t\n\t\t//console.log(\"TEST2\")\n\t\t\n\t\tlet wrapped_pattern = new RegExp(`^${pattern}\$`)\n\t\tthis.lexer.LexPatterns.push(wrapped_pattern)\n\t\tthis.lexer.LexTokens.push(tokenStr)\n\t\tthis.lexer.LexTokens_rev[tokenStr] = this.lexer.LexPatterns.length - 1\n\t\t\n\t\t//console.log(\"TEST3\")\n\t\treturn this.lexer.LexPatterns.length - 1;\n\t};\n}\n\n\nexport class Utils\t{\n\t\n\tconstructor()\t{\n\t\t\n\t}\n\n\tsuccess=\"background: green; color: yellow;\"\n\terror = \"background: red; color: yellow;\"\n\twarn  = \"background: yellow; color: black;\"\n\tinfo  = \"background: blue; color: yellow;\"\n\t\n\tstub()\t{\n\t\n\t\treturn [];\n\t}\n\t\n\tswap(a, b)\t{\n\t\t\n\t\treturn { b, a };\n\t} // syntax: { b, a } = new Utils().swap(a, b) // but wouldn\'t { b, a } = { a, b } work as an inline?\n\t\n\tmax(a, b)\t{\n\t\t\n\t\tif(a>=b)\n\t\t\treturn a;\n\t\t\n\t\treturn b;\n\t}\n\t\n\tmin(a, b)\t{\n\t\t\n\t\tif(a<=b)\n\t\t\treturn a;\n\t\t\n\t\treturn b;\n\t}\n\t\n\tabs(a)\t{\n\t\t\n\t\tif((a<0.0)||(a==-0.0))\n\t\t\treturn -a;\n\t\t\n\t\treturn a;\n\t}\t\n}\n";
