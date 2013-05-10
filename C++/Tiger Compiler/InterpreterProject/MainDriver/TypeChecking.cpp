/**************************************************************
       Team:  Matthew Metzger, Nijyar Muhammet, Kris O'Mealy
 Programmer:  Kris O'Mealy
    Program:  Project 6
   Due Date:  2012-12-04 04:00
 Instructor:  Dr. Zhijiang Dong
**************************************************************/

#include "TypeChecking.h"

using namespace symbol;

namespace semantics
{
	const string		TypeChecking::breakSign = "breaksign";

		//insert a variable into the var/function symbol table
	void TypeChecking::insertVar(string name, symbol::SymTabEntry entry)
	{
		string			msg;
		stringstream	ss(msg);

		if ( env.getVarEnv()->localContains(name) )
		{
			symbol::SymTabEntry		old = env.getVarEnv()->lookup(name);
			ss << "variable " << name << " is already defined at line " << old.node->getLineno();
			error(entry.node, ss.str());
		}
		else
			env.getVarEnv()->insert( name, entry );
	}

	//insert a function into the var/function symbol table
	void TypeChecking::insertFunc(string name, symbol::SymTabEntry entry)
	{
		string			msg;
		stringstream	ss(msg);

		if ( env.getVarEnv()->localContains(name) )
		{
			symbol::SymTabEntry		old = env.getVarEnv()->lookup(name);
			ss << "function " << name << " is already defined at line " << old.node->getLineno();
			error(entry.node, ss.str());
		}
		else
			env.getVarEnv()->insert( name, entry );
	}

	//insert a type into the type symbol table
	void TypeChecking::insertType(string name, symbol::SymTabEntry entry)
	{
		string			msg;
		stringstream	ss(msg);

		if ( env.getTypeEnv()->localContains(name) )
		{
			symbol::SymTabEntry		old = env.getTypeEnv()->lookup(name);
			ss << "variable " << name << " is already defined at line " << old.node->getLineno();
			error(entry.node, ss.str());
		}
		else
			env.getTypeEnv()->insert( name, entry );
	}

	const types::Type* TypeChecking::visit(const Absyn *v)
	{
		if ( dynamic_cast<const Exp *>(v) != NULL )
			return visit( dynamic_cast<const Exp *>(v) );
		else if ( dynamic_cast<const Var *>(v) != NULL )
			return visit( dynamic_cast<const Var *>(v) );
		else if ( dynamic_cast<const Dec *>(v) != NULL )
			return visit( dynamic_cast<const Dec *>(v) );
		else
			throw runtime_error("invalid node");
	}

	const types::Type* TypeChecking::visit(const Exp *e)
	{
		if (dynamic_cast<const OpExp*>(e) != NULL)			return visit((const OpExp*)e);
		else if (dynamic_cast<const VarExp*>(e) != NULL)	return visit((const VarExp*)e);
		else if (dynamic_cast<const NilExp*>(e) != NULL)	return visit((const NilExp*)e);
		else if (dynamic_cast<const IntExp*>(e) != NULL)	return visit((const IntExp*)e);
		else if (dynamic_cast<const StringExp*>(e) != NULL) return visit((const StringExp*)e);
		else if (dynamic_cast<const CallExp*>(e) != NULL)	return visit((const CallExp*)e);
		else if (dynamic_cast<const RecordExp*>(e) != NULL) return visit((const RecordExp*)e);
		else if (dynamic_cast<const SeqExp*>(e) != NULL)	return visit((const SeqExp*)e);
		else if (dynamic_cast<const AssignExp*>(e) != NULL) return visit((const AssignExp*)e);
		else if (dynamic_cast<const IfExp*>(e) != NULL)		return visit((const IfExp*)e);
		else if (dynamic_cast<const WhileExp*>(e) != NULL)	return visit((const WhileExp*)e);
		else if (dynamic_cast<const ForExp*>(e) != NULL)	return visit((const ForExp*)e);
		else if (dynamic_cast<const BreakExp*>(e) != NULL)	return visit((const BreakExp*)e);
		else if (dynamic_cast<const LetExp*>(e) != NULL)	return visit((const LetExp*)e);
		else if (dynamic_cast<const ArrayExp*>(e) != NULL)	return visit((const ArrayExp*)e);
		else throw new runtime_error("ExpType.visit(Exp*)");
	}

	const types::Type* TypeChecking::visit(const Var *v)
	{
		if (dynamic_cast<const SimpleVar *>(v) != NULL)			return visit((const SimpleVar *) v);
		else if (dynamic_cast<const FieldVar *>(v) != NULL)		return visit((const FieldVar *) v);
		else if (dynamic_cast<const SubscriptVar *>(v) != NULL) return visit((const SubscriptVar *)v);
		else throw new runtime_error("ExpType.visit(Var*)");
	}

	const types::Type* TypeChecking::visit(const Ty *t)
	{
		if (dynamic_cast<const NameTy *>(t) != NULL)			return visit((const NameTy *) t);
		else if (dynamic_cast<const ArrayTy *>(t) != NULL)		return visit((const ArrayTy *) t);
		else if (dynamic_cast<const RecordTy *>(t) != NULL)		return visit((const RecordTy *)t);
		else throw new runtime_error("ExpType.visit(Ty*)");
	}

	const types::Type* TypeChecking::visit(const Dec *d)
	{
		if (dynamic_cast<const TypeDec *>(d) != NULL)			return visit((const TypeDec *) d);
		else if (dynamic_cast<const VarDec *>(d) != NULL)		return visit((const VarDec *) d);
		else if (dynamic_cast<const FunctionDec *>(d) != NULL)	return visit((const FunctionDec *)d);
		else throw new runtime_error("ExpType.visit(Dec*)");
	}

	const types::Type* TypeChecking::visit(const SimpleVar *v)
	{
		if ( !env.getVarEnv()->contains(v->getName()) )
		{
			error( v, "SimpleVar visit: undefined variable");
			//undeclared variables is treated as INT variable
			insertVar( v->getName(), 
					   SymTabEntry(env.getVarEnv()->getLevel(),
								   new types::INT(),
								   v) );
			return new types::INT();
		}
		else
		{
			const types::Type*	t = env.getVarEnv()->lookup(v->getName()).info;

			if (dynamic_cast<const types::FUNCTION *>(t) != NULL )
			{
				error( v, "SimpleVar visit: function with the same name exists");
				//undeclared variables is treated as INT variable
				return new types::INT();
			}
			return t->actual();
		}
	}

	const types::Type* TypeChecking::visit(const FieldVar *v)
	{
		//add your implementation here
		//syntax: lvalue.fieldname
		/*
		algorithm:
			1. Perform type checking on lvalue, and get its data type (say t)
			2. if t is not a record type
					report an error
					return INT
				else
					find the field with the given fieldname in record type t
					if such field is found
						return the type of the field
					else
						report an error
						return INT
		*/

		const types::Type* t = visit(v->getVar());
		const types::Type *rt = new types::INT();
		if(dynamic_cast<const types::RECORD *>(t) == NULL)
		{
			error( v, "FieldVar visit: lvalue is not a record type variable");
			// rt is still an INT return new types::INT();
		}
		else
		{
			if( env.getVarEnv()->contains(v->getField()) )
			{
				rt = env.getVarEnv()->lookup(v->getField()).info->actual();
				// rt is now the actual type.
			}
			else
			{
				error( v, "FieldVar visit: fieldname is undefined");
				// rt is still an INT return new types::INT();
			}
		}

		return rt;
	}

	const types::Type* TypeChecking::visit(const SubscriptVar *v)
	{
		//add your implementation here
		//syntax: lvalue[index_exp]
		/*
		algorithm:
			1. Perform type checking on lvalue, and get its data type (say t)
			2. if t is not a ARRAY type
					report an error
					return INT
				else
					perform type checking on index_exp and get its data type, say ti
					if ( ti is an INT) 
						return the type of array element (can be found in t)
					else
						report an error
						return INT
		*/

		const types::Type *t = visit(v->getVar());
		const types::Type *ti = visit(v->getIndex());
		const types::Type *rt = new types::INT();

		if(dynamic_cast<const types::ARRAY *>(t) == NULL)
		{
			error( v, "SubscriptVar visit: lvalue is not an array type variable");
			// rt is still an INT return new types::INT();
		}

		if( ti->coerceTo( new types::INT() ) )
		{
			rt = t->actual();
			//rt is now the actual type of t.
		}
		else
		{
			error( v->getIndex(), "SubscriptVar visit: index expression is not an INT");
			// rt is still an INT return new types::INT();
		}

		return rt;
	}


	const types::Type* TypeChecking::visit(const OpExp *e)
	{
		//add your implementation here
		//syntax: left_exp Operator right_exp
		/*
		algorithm:
			1. Perform type checking on left_exp and get its data type (say lt)
			2. Perform type checking on right_exp and get its data type (say rt)
			3. if Operator is one of +, -, *, /
					if lt is not an INT, report an error
					if rt is not an INT, report an error
					return INT
			4. else if Operator is one of >, >=, <, or <=
					if lt is not an INT/STRING, report an error
					if rt is not an INT/STRING, report an error
					if ( lt and rt are not compatible ) //you need to check both directions
						report an error
					return INT
			5. else if Operator is one of =, <>
					if lt/rt is not one of the INT, STRING, ARRAY, RECORD, NIL
						report an error
					if ( lt and rt are not compatible )
						report an error
					if ( both lt and rt are NIL )
						report an error
					return INT
		*/

		const types::Type *lt = visit( e->getLeft() );
		const types::Type *rt = visit( e->getRight() );

		if ( e->getOper() == e->PLUS  ||
			 e->getOper() == e->MINUS ||
			 e->getOper() == e->MUL   ||
			 e->getOper() == e->DIV )
		{
			if ( dynamic_cast<const types::INT *>(lt) == NULL )
				error(e->getLeft(),"OpExp visit: Left exp is not an INT.");
			if ( dynamic_cast<const types::INT *>(rt) == NULL )
				error(e->getRight(),"OpExp visit: Right exp is not an INT.");
		}
		else if( e->getOper() == e->GT ||
				 e->getOper() == e->GE ||
				 e->getOper() == e->LT ||
				 e->getOper() == e->LE )
		{
			if ( dynamic_cast<const types::INT *>(lt) == NULL && 
				 dynamic_cast<const types::STRING *>(lt) == NULL )
			{
				error(e->getLeft(),"OpExp visit: Left exp is not an INT or a STRING.");
			}
			if ( dynamic_cast<const types::INT *>(rt) == NULL && 
				 dynamic_cast<const types::STRING *>(rt) == NULL)
			{
				error(e->getRight(),"OpExp visit: Right exp is not an INT or a STRING.");
			}
			if ( ! lt->coerceTo(rt) && ! rt->coerceTo(lt) )
			{
				error(e->getLeft(),"OpExp visit: Left and Right exp are not compatible.");
			}
		}
		else if( e->getOper() == e->EQ ||
				 e->getOper() == e->NE )
		{
			if ( dynamic_cast<const types::INT *>(lt) == NULL &&
				 dynamic_cast<const types::STRING *>(lt) == NULL &&
				 dynamic_cast<const types::ARRAY *>(lt) == NULL &&
				 dynamic_cast<const types::RECORD *>(lt) == NULL &&
				 dynamic_cast<const types::NIL *>(lt) == NULL )
			{
				error(e->getLeft(),"OpExp visit: Left exp is not an INT, STRING, ARRAY, RECORD, or NIL.");
			}
			if ( dynamic_cast<const types::INT *>(rt) == NULL &&
				 dynamic_cast<const types::STRING *>(rt) == NULL &&
				 dynamic_cast<const types::ARRAY *>(rt) == NULL &&
				 dynamic_cast<const types::RECORD *>(rt) == NULL &&
				 dynamic_cast<const types::NIL *>(rt) == NULL )
			{
				error(e->getRight(),"OpExp visit: Right exp is not an INT, STRING, ARRAY, RECORD, or NIL.");
			}
			if ( ! lt->coerceTo(rt) && ! rt->coerceTo(lt) )
			{
				error(e->getLeft(),"OpExp visit: Left and Right exp are not compatible.");
			}
			if ( dynamic_cast<const types::NIL *>(lt) != NULL && 
				 dynamic_cast<const types::NIL *>(rt) != NULL )
			{
				error(e->getLeft(),"OpExp visit: Left and Right exp cannot both be NIL.");
			}
		}
		else
		{
			error(e,"OpExp visit: invalid operator");
		}

		return new types::INT();
	}

	const types::Type* TypeChecking::visit(const VarExp *e)
	{
		const types::Type*		t = visit( e->getVar() );
		return t->actual();
	}

	const types::Type* TypeChecking::visit(const NilExp *e)
	{
		return new types::NIL();
	}

	const types::Type* TypeChecking::visit(const IntExp *e)
	{
		return new types::INT();
	}

	const types::Type* TypeChecking::visit(const StringExp *e)
	{
		return new types::STRING();
	}

	const types::Type* TypeChecking::visit(const CallExp *e)
	{
		//add your implementation here
		//syntax: fname(exp1, exp2, ..., expn)
		/*
		algorithm:
			1. lookup the symbol table to find the type of name fname (say t)
			2. if t doesn't exist, report an error and return INT
			3. if t is not a FUNCTION, report an error and return INT
			4.	if the number of expressions is not equal to the number of parameters
					return an error
			5. for i=0 to n
					perform type checking on expi, and get its data type ti
					if ti cannot be converted to the type of ith function parameter
						report an error
			6. return the result type of the function (can be found in t)
		*/
		
		const types::Type *t = NULL;
		const types::Type *rt = new types::INT();

		if ( ! env.getVarEnv()->contains(e->getFunc()) )
		{
			error(e,"CallExp visit: function name not defined");
			// rt is still and INT return new types::INT();
		}
		else
		{
			t = env.getVarEnv()->lookup(e->getFunc()).info->actual();
			if ( dynamic_cast<const types::FUNCTION *>(t) == NULL )
			{
				error(e,"CallExp visit: called name is defined as a non-function variable");
				// rt is still and INT return new types::INT();
			}
			else
			{
				//TODO: Figure this one out
				//if ( e.length != t.length )
				//{
				//	error(e,"CallExp visit: wrong number of arguments.");
				//}
				const types::FUNCTION *func = (types::FUNCTION *)t->actual();
				int flen = func->getFieldType().size();
				int count = 0;

				if(e->getArgs() != NULL)
				{
					const absyn::ExpList *args = e->getArgs();
					const types::Type *ti = NULL;

					while(args != NULL) 
					{
						ti = visit( args->getHead() );
						//if ( ! ti->coerceTo( ? ) )
						args = args->getRest();
						count++;
					}

					if ( count != flen )
					{
						error(e,"CallExp visit: wrong number of parameters.");
					}
					else
					{
						rt = t->actual();
					}
				}
			}
		}
		return rt;
	}

	const types::Type* TypeChecking::visit(const RecordExp *e)
	{
		//add your implementation here
		//syntax: record_type {f1=exp1, ..., fn=expn}
		/*
		algorithm:
			1. lookup the type symbol table to find type definition (say t)
				for record_type (it is an identifier)
			2. if record_type doesn't exists, report an error and return INT;
			3. if t is not a RECORD, report an error, and return INT
			4. for each fi=expi in the list
					if fi is not a correct name, report an error
					else
						perform type checking on expi, and get its type ti
						if ti is not compatible with the type of field fi,
							report an error;
			5. if ( n is not equal to the number of fields in the record definition )
					report an error
			(PAY ATTENTION TO EMPTY RECORD!!)
			6. return t;
		*/
		//return NULL;
		const types::Type *t = NULL;
		if ( ! env.getTypeEnv()->contains(e->getType()) )
		{
			error(e, "RecordExp visit: undefined type");
			return new types::INT();
		}
		else
		{
			t = env.getTypeEnv()->lookup(e->getType()).info;

			if ( dynamic_cast<const types::RECORD *>(t) != NULL )
			{
				error(e, "RecordExp visit: not a record type");
				return new types::INT();
			}

			if(e->getFields() != NULL)
			{
				const absyn::FieldExpList *fields = e->getFields();
				
				const types::Type *di = NULL;
				const types::Type *fi = NULL;

				while( fields != NULL )
				{
					if( fields->getInit() != NULL )
					{
						if ( ! env.getTypeEnv()->contains(fields->getName()) )
						{
							error(fields,"RecordExp visit: parameter has an invalid type.");
							break;
						}
						
						di = env.getTypeEnv()->lookup(fields->getName()).info;
						fi = visit( fields->getInit() );
						if ( ! fi->coerceTo(di) )
						{
							error(fields->getInit(),"RecordExp visit: Expression not compatible with field.");
						}
					}
					if ( fields->getRest() == NULL )
					{
						error(fields,"RecordExp visit: Call does not have enough parameters.");
					}
					fields = fields->getRest();
				}
			}
		}
		return t;
	}

	const types::Type* TypeChecking::visit(const SeqExp *e)
	{
		//add your implementation here
		//syntax: exp1; exp2; exp3; ....; expn
		/*
		algorithm:
			1. for i=0 to n
					perform type checking on expi, and get its data type t
			2. return t;
		*/
		const types::Type *t = new types::INT();
		if ( e != NULL && e->getList() != NULL )
		{
			const absyn::ExpList *list = e->getList();
			while ( list != NULL )
			{
				if ( list->getHead() != NULL )
					t = visit( list->getHead() );
				list = list->getRest();
			}
		}
		return t->actual();
	}

	const types::Type* TypeChecking::visit(const AssignExp *e)
	{
		//add your implementation here
		//syntax: lvalue := exp
		/*
		algorithm:
			1. perform the type checking on lvalue and get its type t
			2. if t doesn't exist; report an error
			3. if t represents a FUNCTION, report an error
			4. Perform type checking on exp, and get its type te
			5. if te is not compatible with t, report an error;
			6. return VOID
		*/
		
		const types::Type *t = visit(e->getVar());
		if ( t == NULL )
		{
			error(e->getVar(), "AssignExp visit: lvalue Variable not defined");
		}
		if ( dynamic_cast<const types::FUNCTION *>(t) != NULL )
		{
			error(e->getVar(), "AssignExp visit: lvalue variable is a function");
		}

		const types::Type *te = visit(e->getExp());
		if ( ! te->coerceTo(t) )
		{
			error(e->getExp(), "AssignExp visit: Expression is not compatible with lvalue type.");
		}

		return new types::VOID();
	}

	const types::Type* TypeChecking::visit(const IfExp *e)
	{
		//add your implementation here
		//syntax: if test then
		//				exp1
		//			else
		//				exp2

		/*
		algorithm
			1. Perform type checking on test expression and get its data type tt;
			2. if tt is not an INT, report an error
			3. perform type checking on exp1 and get its data type t1
			4. if it is a if-then statement (no else)
					if t1 is not VOID, report an error
					return VOID
			5. else (it is a if-then-else statement)
					perform type checking on exp2, and get its data type t2
					if ( t1 can be converted to t2)
						return t2
					else if t2 can be converted to t1
						return t1
					report an error
					return t1
		*/
		const types::Type *tt = visit( e->getTest() );
		const types::Type *t1 = visit( e->getThenClause() );
		const types::Type *t2 = NULL;
		const types::Type *rt = new types::VOID();

		if ( ! tt->coerceTo(new const types::INT()) )
			error(e->getTest(),"IfExp visit: test type is not an INT");
		

		if ( e->getElseClause() == NULL )
		{
			if ( dynamic_cast<const types::VOID *>(t1) == NULL )
				error(e,"IfExp visit: then clause is not a VOID type");
		}
		else
		{
			t2 = visit( e->getElseClause() );
			if ( t1->coerceTo(t2) )
			{
				rt = t2;
			}
			else if ( t2->coerceTo(t1) )
			{
				rt = t1;
			}
			else
			{
				error(e->getElseClause(),"IfExp visit: Then and Else clauses do not have compatible types");
				rt = t1;
			}
		}
		
		return rt->actual();
	}

	const types::Type* TypeChecking::visit(const WhileExp *e)
	{
		//add your implementation here
		//syntax: while test do exp1
		/*
		algorithm
			1. perform type checking on test, and get its data type tt;
			2. perform type checking on exp1, and get its data type t1;
			3. If tt is not an INT, report an error
			4. if t1 is not an VOID, report an error
			5. return VOID
		*/

		const types::Type *tt = visit( e->getTest() );
		const types::Type *t1 = visit( e->getBody() );

		if ( ! tt->coerceTo(new types::INT()) )
			error(e->getTest(),"WhileExp visit: Test clause is not an INT type.");
		if ( ! t1->coerceTo(new types::VOID()) )
			error(e->getBody(),"WhileExp visit: Exp clause is not a VOID type.");

		return new types::VOID();
	}

	const types::Type* TypeChecking::visit(const ForExp *e)
	{
		//add your implementation here
		//syntax: for vname = exp1 to exp2 do exp3
		/*
		algorithm
			1. create a new scope for var/function symbol table
			2. perform type checking on (vname = exp1), which is treated as a
				varaible declaration; 
			3. lookup var/function symbol table to get the data type t1 variable vname;
			3. if t1 is not INT, report an error
			4. perform type checking on exp2, and get its data type t2;
			5. if t2 is not INT, report an error
			6. perform type checking on exp3, and get its data type t3;
			7. if t3 is not VOID, report an error
			8. end the scope of var/function symbol table
			9. return VOID

		BONUS: No modification to loop variable
		algorithm:
		1. Everytime entering a for loop
				create a new scope for lv
				insert the loop variable into lv
		2. Every time leaving a for loop
				destroy the scope for lv
		3. In visit(AssignExp)
				if Var in the assignment expression is a simple var
					retrieve information (say v1) of var from the var/function symbol table
					retrieve information (say v2) of var from the lv symbol table
					if v1 and v2 points to the same node in the AbstractSyntaxTree,
						report an error
		*/

		env.getVarEnv()->beginScope();
		lv.beginScope();

		const types::Type *t1 = visit(e->getVar());
		const types::Type *t2 = visit(e->getHi());
		const types::Type *t3 = visit(e->getBody());

		if( dynamic_cast<const types::INT *>(t1) == NULL )
			error(e->getVar(),"ForExp visit: loop variable expression is not an INT type.");
		if( dynamic_cast<const types::INT *>(t2) == NULL )
			error(e->getHi(),"ForExp visit: hi expression is not an INT type.");
		if( dynamic_cast<const types::VOID *>(t3) == NULL )
			error(e->getBody(),"ForExp visit: body expression is not a VOID type.");

		env.getVarEnv()->endScope();
		lv.endScope();

		return new types::VOID();
	}

	const types::Type* TypeChecking::visit(const BreakExp *e)
	{
		//add your implementation here
		/*
		return VOID if you don't want bonus points
		
		algorithm:
		1. create a symbol table say lv (lv is actually a member data of class TypeChecking;
		2. everytime entering a loop:
				create a new scope for lv,
				insert breakSign into lv, its data type is INT
		3. everytime exiting a loop;
				destroy the scope for lv
		4. everytime entering a function declaration
				create a new scope for lv,
				insert breakSign into lv, its data type is VOID
		5. everytime exiting a function;
				destroy the scope for lv
		6. in visit(BreakExp)
				lookup lv symbol table to find the definition of breakSign
				and get its data type t

				if t is VOID, report an error
		*/

		error(e, "BreakExp visit: Not implemented yet");
		return new types::VOID();
	}

	const types::Type* TypeChecking::visit(const LetExp *e)
	{
		//add your implementation here
		//syntax: let DecList in Exp end
		/*
		algorithm
			1. create a new scope for BOTH symbol tables
			2. for each decl in the declaration list
					perform type checking on decl;
			3. perform type checking on exps, and get is data type t;
			4. end the scope for BOTH symbol tables
			5. return t;
		*/

		//TODO: Check this one
		
		env.getTypeEnv()->beginScope();
		env.getVarEnv()->beginScope();

		const types::Type *t = NULL;
		if ( e->getDecs() != NULL )
		{
			const absyn::DecList *list = e->getDecs();
			while ( list != NULL )
			{
				if ( list->getHead() != NULL )
				{
					t = visit( list->getHead() );
				}
				list = list->getRest();
			}
		}

		if ( e->getBody() != NULL )
		{
			t = visit(e->getBody());
		}
		else
		{
			t = new types::INT();
		}

		env.getTypeEnv()->endScope();
		env.getVarEnv()->endScope();

		return t->actual();
	}

	const types::Type* TypeChecking::visit(const ArrayExp *e)
	{
		//add your implementation here
		//syntax: array_type [exp1] of exp2
		//syntax: type [size] of init
		/*
		algorithm:
			1. lookup the type symbol table to find the data type t of 
				array_type
			2. if t doesn't exist, report an error
			3. if t is not ARRAY, report an error
			4. perform type checking on size and get its data type s;
			5. if s is not INT, report an error
			6. if t is an ARRAY
			7.		perform type checking on init, and get is data type i;
			8.		if i cannot be converted to t, report an error
			9.		return t;
			10. else
			11.		return ARRAY of INT
		*/

		const types::Type *t = NULL;
		const types::Type *s = visit(e->getSize());
		const types::Type *i = visit(e->getInit());
		const types::Type *rt = new types::ARRAY(new types::INT());

		if ( ! env.getTypeEnv()->contains(e->getType()) ) 
		{
			error(e,"ArrayExp visit: type name not defined");
		}
		else 
		{
			t = env.getTypeEnv()->lookup(e->getType()).info->actual();
			if ( dynamic_cast <const types::ARRAY *>(t) == NULL )
			{
				error(e,"ArrayExp visit: type is not an array type");
			}
			if ( dynamic_cast <const types::INT *>(s) == NULL )
			{
				error(e->getSize(),"ArrayExp visit: size is not an INT");
			}
			if ( i->coerceTo(t) )
			{
				rt = t;
			}
			else
			{
				error(e->getInit(), "ArrayExp visit: init is not compatible with type");
			}
		}
		return rt;
	}

	const types::Type* TypeChecking::visit(const FunctionDec *d)
	{
		//add your implementation here
		//syntax: function fname(p1:type1, ..., pn:typen) : rtype = exp1
		/*
		algorithm:
			1. if fname is already defined in the current scope of var/function
				symbol table, report an error
			3. insert the fname into current scope of var/function symbol table 
				(make sure FUNCTION is constructed correctly, and if rtype is provided, 
				perform type checking on rtype to get its definition)
			4. create a new scope for var/function symbol table
			5. for each pi:typei in the parameter list
					lookup pi in the current scope of var/function symbol table,
					if it already exists, report an error
					
					lookup the type symbol table for typei and get its data type ti
					if ti doesn't exist, report an error

					insert pi into the current scope,
			6. perform type checking on exp1, and get its data type t1;
			7. if ( rtype is not provided )
					if t1 is not VOID, report an error
			8. else
					if t1 cannot be converted to rtype, report an error
			9. end the scope for var/function symbol table
			10. return NULL
		*/

		if ( env.getVarEnv()->localContains(d->getName()) )
		{
			error(d, "FunctionDec visit: function name already defined at this scope");
		}

		if ( d->getResult() != NULL )
		{
			if ( ! env.getTypeEnv()->contains(d->getResult()->getName()) )
			{
				error(d->getResult(),"FunctionDec visit: return type name not defined");
			}
			else
			{
				insertFunc(d->getName(), 
						   symbol::SymTabEntry(env.getVarEnv()->getLevel(), 
											   env.getTypeEnv()->lookup(d->getResult()->getName()).info, 
											   d));
			}	
		}
		else
		{
			insertFunc(d->getName(), 
					   symbol::SymTabEntry(env.getVarEnv()->getLevel(), 
										   new types::VOID, d));
		}

		env.getVarEnv()->beginScope();
		if(d->getParams() != NULL)
		{
			const absyn::FieldList* list = d->getParams();

			while( list != NULL )
			{
				if ( env.getVarEnv()->localContains(list->getName()) )
				{
					error(list, "FunctionDec visit: parameter variable name already defined in this scope");
				}
				
				if( ! env.getTypeEnv()->contains(list->getType()) )
				{
					error(list, "FunctionDec visit: undefined parameter type");
				}
				else
				{
					insertVar(list->getName(), 
							  symbol::SymTabEntry(env.getVarEnv()->getLevel(),
												  env.getTypeEnv()->lookup(list->getType()).info, 
												  list));
				}
				list = list->getRest();
			}
		}
		
		const types::Type *t1 = visit(d->getBody());
		if ( d->getResult() == NULL )
		{
			if ( dynamic_cast<const types::VOID *>(t1) == NULL )
				error(d->getBody(),"FunctionDec visit: body does not result in a Void Type.");
		}
		else
		{
			if ( ! t1->coerceTo(env.getTypeEnv()->lookup(d->getResult()->getName()).info) )
				error(d->getResult(),"FunctionDec visit: body is not compatible with return type");
		}

		env.getVarEnv()->endScope();

		return NULL;
	}

	const types::Type* TypeChecking::visit(const VarDec *d)
	{
		//add your implementation here
		//syntax: var name [: type] := initExp
		/*
		algorithm:
			1. lookup the var/function symbol table to find its definition
			2. if definition does exist, report an error
			3. perform type checking on initExp and get its data type ti
			4. if Type is provided
					lookup the type symbol table to get its definition tt
					if tt doesn't exist
						report an error
					else if ti cannot be converted to tt
						report an error
					else
						insert vname into the var/function symbol table
			5. else  //Type is not provided
					insert vname into the var/function symbol table

			6. return NULL
		*/

		if ( env.getVarEnv()->localContains(d->getName()) )
		{
			error(d, "VarDec visit: variable already defined in this scope.");
		}

		const types::Type *ti = visit(d->getInit());

		if ( d->getType() != NULL )
		{
			
			if ( ! env.getTypeEnv()->contains(d->getType()->getName()) )
			{
				error(d->getType(), "VarDec visit: type not defined.");
			}
			else
			{
				const types::Type *tt = env.getTypeEnv()->lookup(d->getType()->getName()).info;
				if ( ! ti->coerceTo(tt) )
				{
					error(d->getType(), "VarDec visit: incompatible types");
				}
				else
				{
					insertVar( d->getName(), 
							   SymTabEntry(env.getVarEnv()->getLevel(), 
										   (types::Type *)ti, d) );
				}	
			}
		}
		else
		{
			insertVar( d->getName(), 
					   SymTabEntry(env.getVarEnv()->getLevel(), 
								   new types::VOID, d) );
		}
		
		return NULL;
	}

	const types::Type* TypeChecking::visit(const TypeDec *d)
	{
		const types::Type*		type;
		types::NAME*			name = new types::NAME( d->getName() );
	
		//find type redefine in the consecutive type declarations 
		const absyn::TypeDec*	td = d->getNext();
		while ( td != NULL ) {
			if ( td->getName() == d->getName() )
				error( td, "TypeDec visit: type redefined" );
			td = td->getNext();
		}

		name->bind( new types::INT() );	//just for avoiding the self loop, later it
							//will be replaced by actual value
		
		insertType(d->getName(), SymTabEntry(env.getVarEnv()->getLevel(), name, d));	

		if ( d->getNext() != NULL )
			visit( d->getNext() );
		type = visit( d->getTy() );

		name->bind( (types::Type *)type );
		env.getTypeEnv()->lookup(d->getName()) = SymTabEntry(env.getVarEnv()->getLevel(),
																name,
																d);	

		if ( name->isLoop() ) {
			error( d, "TypeDec visit: illegal cycle found in type definition" );
			name->bind( new types::INT() );
		}

		return NULL;
	}


	const types::Type* TypeChecking::visit(const NameTy *t)
	{
		if ( !(env.getTypeEnv()->contains(t->getName())) )
		{
			error(t, "NameTy visit: undefined type name");
			return new types::INT();
		}

		return env.getTypeEnv()->lookup(t->getName()).info;
	}

	const types::Type* TypeChecking::visit(const RecordTy *t)
	{
		const absyn::FieldList*		fl = t->getFields();

		if ( fl == NULL ) {
			//empty record
			return new types::RECORD( "", NULL, NULL );
		}
		else {
			types::RECORD		*r = NULL, *tail = NULL, *head = NULL;
	
			while ( fl != NULL ) {
				if ( !env.getTypeEnv()->contains(fl->getType()) )
					r = new types::RECORD(	fl->getName(),
											new types::INT(),
											NULL );
				else
					r = new types::RECORD(	fl->getName(),
											env.getTypeEnv()->lookup(fl->getType()).info,
											NULL );
				if ( head == NULL )
					head = tail = r;
				else {
					tail->setRest(r);
					tail = r;	
				}
				fl = fl->getRest();
			}
			return head;
		}	
	}

	const types::Type* TypeChecking::visit(const ArrayTy *t)
	{
		if ( !(env.getTypeEnv()->contains(t->getName())) )
		{
			error(t, "ArrayTy visit: undefined type");
			return new types::INT();
		}

		return new types::ARRAY( env.getTypeEnv()->lookup(t->getName()).info );
	}
	
} // end of namespace semantics
