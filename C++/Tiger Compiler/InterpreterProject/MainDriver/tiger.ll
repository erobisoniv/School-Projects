/*************************************************************
**************************************************************
       Team:  Matthew Metzger, Nijyar Muhammet, Kris O'mealy
    Program:  Project 5
   Due Date:  2012-11-016 04:00
 Instructor:  Dr. Zhijiang Dong
      Notes:  This is the file we used in Project 3
*************************************************************
************************************************************/


%option yylineno
%option noyywrap
%option c++

%{
#include <iostream>
#include <string>
#include <sstream>
#include "tiger.tab.hh"
#include "ErrorMsg.h"

using std::string;
using std::stringstream;

ErrorMsg			errormsg;	//error handler

int		comment_depth = 0;	// depth of the nested comment
string	value = "";			// the value of current string

int		beginLine=-1;		//beginning line no of a string or comment
int		beginCol=-1;		//beginning column no of a string or comment

int		linenum = 1;		//beginning line no of the current matched token
int		colnum = 1;			//beginning column no of the current matched token
int		tokenCol = 1;		//column no after the current matched token

//the following defines actions that will be taken automatically after 
//each token match. It is used to update colnum and tokenCol automatically.

#define YY_USER_ACTION {yylloc.first_line = yylineno; yylloc.first_column = colnum; colnum=colnum+yyleng; yylloc.last_column=colnum; yylloc.last_line = yylineno;}

int string2int(string);			//convert a string to integer value
void newline(void);				//trace the line #
void error(int, int, string);	//output the error message referring to the current token
%}

ALPHA		[A-Za-z]
DIGIT		[0-9]
INT			[0-9]+
IDENTIFIER	{ALPHA}(({ALPHA}|{DIGIT}|"_")*)

%x	COMMENT
%x	STR
%x  MULTSTR

%%

" "				{}
\t				{}
\b				{}
\n				{ newline(); }


"*"				{ return MUL; }
"/"				{ return DIV; }
"+"				{ return PLUS; }
"-"				{ return MINUS; }
"="				{ return EQ; }
"<>"			{ return NE; }
">" 			{ return GT; }
"<" 			{ return LT; }
">=" 			{ return GE; }
"<=" 			{ return LE; }
"&"				{ return AND; }
"|"				{ return OR; }
":="			{ return ASSIGN; }

"("				{ return LPAREN; }
")"				{ return RPAREN; }
"["				{ return LBRACK; }
"]"				{ return RBRACK; }
"{"				{ return LBRACE; }
"}"				{ return RBRACE; }

":"				{ return COLON; }
";"				{ return SEMICOLON; }
","				{ return COMMA; }
"."				{ return DOT; }


while			{ return WHILE; }
for				{ return FOR; }
to				{ return TO; }
break			{ return BREAK; }
let				{ return LET; }
in				{ return IN; }
end				{ return END; }
function 		{ return FUNCTION; }
var				{ return VAR; }
type			{ return TYPE; }
array			{ return ARRAY; }
if				{ return IF; }
then			{ return THEN; }
else			{ return ELSE; }
do				{ return DO; }
of				{ return OF; }
nil				{ return NIL; }

{IDENTIFIER} 	{ value = YYText(); yylval.sval = new string(value); return ID; }
{INT}		 	{ yylval.ival = string2int(YYText()); return INT; }

"/*"				{	/* entering comment */
						comment_depth++;
						beginLine = linenum;
						beginCol = colnum;
						BEGIN(COMMENT);	
						//cout << "Begin comments at " << linenum << ":" << colnum << " depth: " << comment_depth << endl;
					}
<COMMENT>.*"/*"		{	/* nested comment */
						comment_depth++;
						//cout << "Begin nested comments at " << linenum << ":" << colnum << " depth: " << comment_depth << endl;
					}
<COMMENT>[^*\n]*		{	/* eat anything that's not a '*' */ }
<COMMENT>"*"+[^*/\n]*	{	/* eat up '*'s not followed by '/'s */	}
<COMMENT>\n				{ newline(); }
<COMMENT>"*"+"/"	{	/* close of a comment */
                        //cout << "Close comment at " << linenum << ":" << colnum << " depth: " << comment_depth;
						comment_depth--;
						//cout << " enddepth: " << comment_depth << endl;
						if ( comment_depth== 0 )
						{
							BEGIN(INITIAL);	
							//cout << "Comments at " << beginLine << ":" << beginCol << " depth: " << comment_depth << endl;
						}
					}
<COMMENT><<EOF>>	{	/* unclosed comments */
						error(beginLine, beginCol, "unclosed comments");
						yyterminate();
					}

\"					{ /* entering a String */ 
						beginLine = linenum;
						beginCol = colnum;
						value = "";
						BEGIN(STR);	
					}
<STR>[^\"\\\n]*		{ /* append anything that's not a double quote, newline, or slash */
						value += YYText();
					}
<STR>\\n			{ /* catch newline escape sequences */
						value += "\n"; 
					}
<STR>\\t			{ /* catch tab escape sequences */
						value += "\t"; 
					}
<STR>\\\"			{ /* catch quote escape sequences */
						value += '\"';
					}
<STR>\\\\			{ /* catch slash escape sequences */
						value += "\\"; 
					}
<STR>\\[0-9]{3}		{ /* catch ASCII code escape sequences */
						string tmpstr = YYText();
						int tmpcode = string2int(tmpstr.substr(1,3));
						//cout << "Temp string: " << tmpstr << " Temp int: " << tmpcode << endl;
						/* check for validity of standard ASCII character codes */
						if ( tmpcode > 0 && tmpcode < 128 )
							value += tmpcode; 
						else
							error(linenum, colnum, "Illegal ASCII code");
					}

<STR>\\[" "|\t]*\n	{ /* enter the multi-line condition */
						newline();
						BEGIN(MULTSTR);
					}
<MULTSTR>[" "|\t]*\\	{ /* string was broken properly, return to STR */ 
							BEGIN(STR);
						}
<MULTSTR>[" "|\t]*[^\\]		{ /* Improperly broken string */
								error(beginLine, beginCol, "improperly broken string over multiple lines");
								BEGIN(STR);
							}
<STR>\\.            { /* catch illegal escape sequence */
						error(linenum, colnum, "illegal escape sequence");
					}
<STR>\"				{ /* close String */ 
						yylval.sval = new string(value);
						BEGIN(INITIAL);
						return STRING;
					}
<STR>\n				{ /* unclosed string at end of line */
						error(beginLine, beginCol, "unclosed string");
						newline();
						yylval.sval = new string(value);
						BEGIN(INITIAL);
						return STRING;
					}
<STR><<EOF>>		{ /* unclosed string at end of file */
						error(beginLine, beginCol, "unclosed string");
						yyterminate();
					}

<<EOF>>			{	yyterminate(); }
.				{	error(linenum, colnum, string(YYText()) + " illegal token");}

%%

int string2int( string val )
{
	stringstream	ss(val);
	int				retval;

	ss >> retval;

	return retval;
}

void newline()
{
	linenum ++;
	colnum = 1;
	tokenCol = 1;
}

void error(int line, int col, string msg)
{
	errormsg.error(line, col, msg);
}
