/*******************************
       Team:  Matthew Metzger, Nijyar Muhammet, Kris O'mealy
    Program:  Project 5
   Due Date:  2012-11-16 04:00
 Instructor:  Dr. Zhijiang Dong
	  Notes:  This is the file Matthew Metzger used in project 4
*******************************/

%debug
%verbose
%locations
%code requires {
#include <iostream>
#include "ErrorMsg.h"
#include <FlexLexer.h>
#include "Absyn.h"

int yylex(void); /* function prototype */
void yyerror(char *s);	//called by the parser whenever an eror occurs
void yyerror(int, int, char *s);	//called by the parser whenever an eror occurs

}

%union {
	int						ival;	//integer value of INT token
	string*					sval;	
	absyn::Exp*				exp;
	absyn::Var*				var;
	absyn::Ty*				ty;
	absyn::Dec*				dec;
	absyn::DecList*			declist;
	absyn::FieldList*		fieldlist;
	absyn::ExpList*			explist;
	absyn::FieldExpList*	fieldexplist;
	absyn::VarDec*			vardec;
	absyn::SeqExp*			seqexp;
		}

%{
void yyerror(YYLTYPE loc, char *s);
absyn::Exp*		root;	//pointer to the root of abstract syntax tree
%}

/**************
defines all tokens here
**************/
%token 
  COMMA COLON SEMICOLON LPAREN RPAREN LBRACK RBRACK 
  LBRACE RBRACE DOT 
  ARRAY IF THEN ELSE WHILE FOR TO DO LET IN END OF 
  BREAK NIL
  FUNCTION VAR TYPE

/*defines type of values associated to each nonterminal*/

%token <sval> ID STRING
%token <ival> INT
%type <exp>				exp program 
%type <var>				lvalue
%type <dec>				dec type_dec field_dec
%type <vardec>			var_dec
%type <declist>			dec_list decs
%type <fieldlist>		type_field type_fields
%type <explist>			exp_list
%type <seqexp>			exp_seq
%type <fieldexplist>	field_list 
%type <ty>				type

/****************
defines precedence level and associativity of operators here
***************/
%nonassoc ASSIGN
%nonassoc OR 
%nonassoc AND
%nonassoc EQ NE LT LE GT GE
%left PLUS MINUS
%left MUL DIV
%left UMINUS

%start program

%%

program	:	exp	
			{ $$ = $1; root=$$; }
		;


		/* literals */
exp		:	STRING	
			{$$ = new absyn::StringExp(@1.first_line, @1.first_column, *$1);}							
		|	INT										
			{$$ = new absyn::IntExp(@1.first_line, @1.first_column, $1);}
		|	NIL										
			{$$ = new absyn::NilExp(@1.first_line, @1.first_column);}	

		/* assignment */
		|	lvalue ASSIGN exp						
			{$$ = new absyn::AssignExp(@1.first_line, @1.first_column, $1, $3);}		
		
		/* variables, field, elements of an array */							
		|	lvalue									
			{$$ = new absyn::VarExp(@1.first_line, @1.first_column, $1);}

		/* operations */
		|	LPAREN exp_seq RPAREN					
			{$$ = $2;}
		|	exp PLUS exp							
			{$$ = new absyn::OpExp(@1.first_line, @1.first_column, $1, absyn::OpExp::PLUS, $3);}
		|	exp MINUS exp							
			{$$ = new absyn::OpExp(@1.first_line, @1.first_column, $1, absyn::OpExp::MINUS, $3);}
		|	exp MUL exp							
			{$$ = new absyn::OpExp(@1.first_line, @1.first_column, $1, absyn::OpExp::MUL, $3);}
		|	exp DIV exp							
			{$$ = new absyn::OpExp(@1.first_line, @1.first_column, $1, absyn::OpExp::DIV, $3);}
		|	exp EQ exp								
			{$$ = new absyn::OpExp(@1.first_line, @1.first_column, $1, absyn::OpExp::EQ, $3);}
		|	exp NE exp								
			{$$ = new absyn::OpExp(@1.first_line, @1.first_column, $1, absyn::OpExp::NE, $3);}
		|	exp LT exp								
			{$$ = new absyn::OpExp(@1.first_line, @1.first_column, $1, absyn::OpExp::LT, $3);}
		|	exp LE exp								
			{$$ = new absyn::OpExp(@1.first_line, @1.first_column, $1, absyn::OpExp::LE, $3);}
		|	exp GT exp								
			{$$ = new absyn::OpExp(@1.first_line, @1.first_column, $1, absyn::OpExp::GT, $3);}
		|	exp GE exp								
			{$$ = new absyn::OpExp(@1.first_line, @1.first_column, $1, absyn::OpExp::GE, $3);}
			/*Unary minus*/
		|	MINUS exp %prec UMINUS					
			{
			$$ = new absyn::OpExp(@1.first_line, @1.first_column, 
			new absyn::IntExp(@1.first_line, @1.first_column, 0), 
			absyn::OpExp::MINUS, $2);
			}
		|	exp AND exp								
			{$$ = new absyn::IfExp(@1.first_line, @1.first_column, $1, $3, (new absyn::IntExp(@3.first_line, @3.first_column, 0)));}
		|	exp OR exp								
			{$$ = new absyn::IfExp(@1.first_line, @1.first_column, $1, (new absyn::IntExp(@2.first_line, @2.first_column, 1)), $3);}

		/* function call */
		|	ID LPAREN exp_list RPAREN 				
			{$$ = new absyn::CallExp(@1.first_line, @1.first_column, *$1, $3);}

		/* array and records creation */
		|	ID LBRACE field_list RBRACE				
			{$$ = new absyn::RecordExp(@1.first_line, @1.first_column, *$1, $3);}
		|	ID LBRACK exp RBRACK OF exp				
			{$$ = new absyn::ArrayExp(@1.first_line, @1.first_column, *$1, $3, $6);}

		/* control structures */
		/*If-then expression*/
		|	IF exp THEN exp							
			{$$ = new absyn::IfExp(@1.first_line, @1.first_column, $2, $4);}
		/*If-then-else expression*/
		|	IF exp THEN exp ELSE exp				
			{$$ = new absyn::IfExp(@1.first_line, @1.first_column, $2, $4, $6);}
		/*While loop expression*/
		|	WHILE exp DO exp						
			{$$ = new absyn::WhileExp(@1.first_line, @1.first_column, $2, $4);}
		/*For loop expression*/
		|	FOR ID ASSIGN exp TO exp DO exp			
			{
			$$ = new absyn::ForExp(@1.first_line, @1.first_column, 
			(new absyn::VarDec(@2.first_line, @2.first_column, *$2, NULL, $4)), $6, $8);
			}
		/*Break expression*/
		|	BREAK									
			{$$ = new absyn::BreakExp(@1.first_line, @1.first_column);}
		/*Let-in-end expression*/
		|	LET decs IN exp_seq END					
			{$$ = new absyn::LetExp(@1.first_line, @1.first_column, $2, $4);}
		;	
			
			/*Sequence of semicolon separated expressions*/				
exp_seq		:	exp SEMICOLON exp_seq
				{$$ = new absyn::SeqExp(@1.first_line, @1.first_column, new absyn::ExpList($1, (absyn::ExpList*)($3->getList())));}
			|	exp
				{$$ = new absyn::SeqExp(@1.first_line, @1.first_column, new absyn::ExpList($1, NULL));}
			|   /* Empty Set */
				{$$ = NULL}		
			;	

			/*List of comma separated expressions (used for function parameters)*/
exp_list	:	exp COMMA exp_list
				{$$ = new absyn::ExpList($1, $3);}
			|	exp
				{$$ = new absyn::ExpList($1, NULL);}
			|	/* Empty Set */
				{$$ = NULL}	
			;	
	
			/*used in record creation*/
field_list	:	
				{$$ = NULL}	
			|	ID EQ exp
				{$$ = new absyn::FieldExpList(@1.first_line, @1.first_column, *$1, $3, NULL);}
			|	ID EQ exp COMMA field_list
				{$$ = new absyn::FieldExpList(@1.first_line, @1.first_column, *$1, $3, $5);}
			;

			/*used for the left side of assignment statements*/
lvalue		:	ID
				{$$ = new absyn::SimpleVar(@1.first_line, @1.first_column, *$1);}
			|	lvalue DOT ID
				{$$ = new absyn::FieldVar(@1.first_line, @1.first_column, $1, *$3);}
			|	lvalue LBRACK exp RBRACK
				{$$ = new absyn::SubscriptVar(@1.first_line, @1.first_column, $1, $3);}
			|	ID LBRACK exp RBRACK
				{$$ = new absyn::SubscriptVar(@1.first_line, @1.first_column, new absyn::SimpleVar(@1.first_line, @1.first_column, *$1), $3);}
			;

			/*mututally recursive definition for a list of declaration expressions*/
dec_list	:	dec dec_list	
							{	
				if ( dynamic_cast<absyn::TypeDec *>($1) != NULL ) {
					absyn::TypeDec		*t = (absyn::TypeDec *)$1;
					if ( dynamic_cast<const absyn::TypeDec *>($2->getHead()) != NULL ) {
						t->setNext( (absyn::TypeDec *)($2->getHead()) );
						$$ = new absyn::DecList(t, (absyn::DecList *)$2->getRest());
					}
					else
						$$ = new absyn::DecList($1, $2);		
				}
				else if ( dynamic_cast<absyn::FunctionDec *>($1) != NULL ) { 
					absyn::FunctionDec*		f = (absyn::FunctionDec *)$1;
					if ( dynamic_cast<const absyn::FunctionDec *>($2->getHead()) != NULL ) {
						f->setNext( (absyn::FunctionDec *)($2->getHead()) );
						$$ = new absyn::DecList(f, (absyn::DecList *)$2->getRest());
					}
					else
						$$ = new absyn::DecList($1, $2);		
				} 
				else 
					$$ = new absyn::DecList($1, $2);		
			}
			|	dec
				{$$ = new absyn::DecList($1, NULL);}
			;

		/*puts multiple declarations of possible different types together in one non-terminal*/
decs	:	dec_list	
			{ $$ = $1; }
		|
			{$$ = NULL}	
		;

		/*different types of declarations*/
dec		:	type_dec
			{$$ = $1}
		|	var_dec
			{$$ = $1}
		|	field_dec
			{$$ = $1}
		;

		/*used for new type creation*/
type_dec	:	TYPE ID EQ type	
			{$$ = new absyn::TypeDec(@1.first_line, @1.first_column, *$2, $4, NULL);}
		;

		/*used in new type creation*/
type	:	ID					
			{$$ = new absyn::NameTy(@1.first_line, @1.first_column, *$1);}
		|	LBRACE type_fields RBRACE
			{$$ = new absyn::RecordTy(@1.first_line, @1.first_column, $2);}
		|	ARRAY OF ID			
			{$$ = new absyn::ArrayTy(@1.first_line, @1.first_column, *$3);}
		;

			/*used for parameters in function declarations*/
type_fields	:	
				{$$ = NULL}
			|	type_field
				{$$ = $1}
			|	type_field COMMA type_fields
				{$$ = new absyn::FieldList(@1.first_line, @1.first_column, $1->getName(), $1->getType(), $3);}
			;

			/*parameter in function declaration*/
type_field	:	ID COLON ID
				{$$ = new absyn::FieldList(@1.first_line, @1.first_column, *$1, *$3, NULL);}
			;	
	
			/*variable declarations*/
var_dec		:	VAR ID COLON ID ASSIGN exp
				{$$ = new absyn::VarDec(@1.first_line, @1.first_column, *$2, (new absyn::NameTy(@4.first_line, @4.first_column, *$4)), $6);}
			|	VAR ID ASSIGN exp
				{$$ = new absyn::VarDec(@1.first_line, @1.first_column, *$2, NULL, $4);}
			;	

			/*Function declaration*/
field_dec	:	FUNCTION ID LPAREN type_fields RPAREN EQ exp
				{$$ = new absyn::FunctionDec(@1.first_line, @1.first_column, *$2, $4, NULL, $7, NULL);}
			|	FUNCTION ID LPAREN type_fields RPAREN COLON ID EQ exp
				{
				$$ = new absyn::FunctionDec(@1.first_line, @1.first_column, *$2, $4, 
				(new absyn::NameTy(@7.first_line, @7.first_column, *$7)), $9, NULL);
				}
			;

%%
extern yyFlexLexer	lexer;
int yylex(void)
{
	return lexer.yylex();
}

void yyerror(char *s)
{
	extern int	linenum;			//line no of current matched token
	extern int	colnum;
	extern void error(int, int, std::string);

	error(linenum, colnum, s);
}

void yyerror(int lineno, int column, char *s)
{
	extern void error(int, int, std::string);

	error(lineno, column, s);
}

void yyerror(YYLTYPE loc, char *s)
{
	extern void error(int, int, std::string);

	error(loc.first_line, loc.first_column, s);
}

/************
Add your own functions here.
************/