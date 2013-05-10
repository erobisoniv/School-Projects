#include <iostream>
#include <sstream>
using namespace std;

namespace symbol
{

	//get the level of current scope
	template<class Entry>
	int SymbolTable<Entry>::getLevel() const
	{
		return level;
	};

	//Save the information of current scope to the string dumpinfo
	//It should be called at the beginning of the endScope function.
	template<class Entry>
	void SymbolTable<Entry>::dump()
	{
		int			level = tables.size();
		string		indent;
		HashTable	current = *(tables.begin());
		HashTable::iterator	it = current.begin();

		for(int i=0; i<level; i++)
			indent += "    ";
		string			info;
		stringstream	ss(info);

		for(; it != current.end(); ++it)
			ss << indent << it->first << endl;
		ss << indent << "-------------------------------" << endl;
		dumpinfo += ss.str();
	};

	//to remove all scopes from the hashtable list so that
	//all remaining scopes will be dumped.
	template<class Entry>
	SymbolTable<Entry>::~SymbolTable()
	{
		//delete all remaining scopes 
		for(unsigned int i=0; i<tables.size(); i++)
			endScope();

		cout << "*******************Symbol Table************************" << endl;
		cout << dumpinfo << endl;
		cout << "*******************Symbol Table************************" << endl;
	};


	/****************************
	Provide implementation of all other member functions here
	****************************/

	//check if a lexeme is contained in the symbol table list
	//search from the tail to head
	template<class Entry>
	bool SymbolTable<Entry>::contains(string s)
	{
		Iterator l_it = tables.begin(); //list iterator

		HashTable	current = *(tables.begin()); //temporary hash table
		HashTable::iterator	it = current.begin(); //hash table iterator

		for(l_it; l_it != tables.end(); ++l_it)
		{
			current = *(l_it);
			it = current.begin();
			//return true if lexeme is found in table
			for(it; it != current.end(); ++it)
			{
				if( it->first == s )
					return true;
			}
		}
		return false;
	};

	//similar to contains, but only search in the current scope
	template<class Entry>
	bool SymbolTable<Entry>::localContains(string s)
	{
		HashTable	current = *(tables.begin()); //temporary hash table
		HashTable::iterator	it = current.begin(); //hash table iterator

		//return true if lexeme is found
		for(it; it != current.end(); ++it)
		{
			if( it->first == s )
				return true;
		}
		
		return false;

		
	};

	//similar to contains, but only search in the global scope
	template<class Entry>
	bool SymbolTable<Entry>::globalContains(string s)
	{
		HashTable	current = *(tables.end()); //temporary hash table
		HashTable::iterator	it = current.begin(); //hash table iterator

		//return true if lexeme is found
		for(it; it != current.end(); ++it)
		{
			if( it->first == s )
				return true;
		}
		
		return false;
	};

	//Retrieve the value associated with the given lexeme in the hashtable list
	//search from the head to tail
	//an exception is thrown if the lexeme doesn't exist
	template<class Entry>
	Entry& SymbolTable<Entry>::lookup(string lexeme, int blevel)
	{
		if ( blevel == -1)
			blevel = getLevel();
		//skip levels between blevel and getLevel()
		int skip = getLevel() - blevel;
		for(Iterator it=tables.begin(); it!=tables.end(); it++)
		{
			//skip these levels
			if ( skip > 0 )
			{
				skip --;
				continue;
			}
			HashTable &current = *it;
			HashTable::iterator vit = current.find(lexeme);
			if ( vit != current.end() )
			{
				return current[lexeme]; //found the lexeme
			}
		}
		throw runtime_error("The given Lexeme " + lexeme + " doesn't exist!") ;
	}

	//Retrieve the value associated with the given lexeme in the global level only
	//an exception is thrown if the lexeme doesn't exist
	template<class Entry>
	Entry&  SymbolTable<Entry>::globalLookup(string s)
	{
		HashTable	&current = *(tables.end()); //pointer to hash table
		HashTable::iterator	it = current.find(s); //hash table iterator

		if( it != current.end() )
		{
			if( it->first == s )
				return it->second;
		}
		
		throw runtime_error("undefined variable");
	};

	//insert a lexeme and binder to the current scope, i.e. the last hashtable in the list
	//if it exists, an exception will be thrown
	template<class Entry>
	void SymbolTable<Entry>::insert(string lexeme, const Entry value)
	{
		HashTable	&current = *(tables.begin()); //pointer to hash table
		HashTable::iterator	it = current.begin(); //hash table iterator
		//if lexeme is found, throw runtime error
		for(it; it != current.end(); ++it)
		{
			if( it->first == lexeme )
				throw runtime_error("variable already defined");
		}

		current[lexeme] = value; //lexeme wasn't found, insert into table
	};

	//create a new scope as the current scope
	template<class Entry>
	void SymbolTable<Entry>::beginScope()
	{
		level++; //update level

		HashTable a;
		tables.push_front(a); //insert new table into list
	};

	//destroy the current scope, and its parent becomes the current scope
	template<class Entry>
	void SymbolTable<Entry>::endScope()
	{
		dump();
		level--; //update level

		tables.pop_front(); //remove table from front of list
	};


}

 //end of namespace Environment



