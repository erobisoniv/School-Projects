//Name: Matthew Metzger
//Date: 11/27/2011
//Purpose:  Prompts for number of productions, productions, word to parse, and 
//		  determines if word is in the language based on the given productions		  
#include <iostream>
using namespace std;

const int MAX_PRODUCTIONS = 10;
const int MAX_CHARACTERS = 5;

int main()
{
	int n_prod; //number of productions
	int letters = 0; //number of letters in word to parse
	char productions[MAX_PRODUCTIONS][3]; //productions
	char word[MAX_CHARACTERS]; //word to parse
	char temp_word[MAX_CHARACTERS + 1]; //current char to get to in word
	int temp_letters = 1;
	char temp; //temp value to hold chars as they are read in
	int i, j, k;
	bool language = true;
	
	cout << "Please input the number of productions: " << endl;
	cin >> n_prod;
	cout << "Please input the productions: " << endl;
	
	//read in productions
	for(i = 0; i < n_prod; i++)
	{
		cin.ignore(1);
		for(j = 0; j < 3; j++)
		{
		cin >> noskipws >> temp;
		productions[i][j] = temp;
		}
	}
	
	cout << "Please input the word to parse:" << endl;
	
	//read in word to parse
	cin.ignore(1);
	i = 0;
	while(i <= 5)
	{
		cin >> temp;
		if(temp != '\n')
		{
			word[i] = temp;
			i++;
			letters++;
		}
		else
			i = 6;
	}
	
	cout << "The derivation is: " << endl;
	
	temp_word[0] = 'S'; //initialize temp_word[0]
	cout << temp_word[0] << endl; //display starting production
	for(i = 0; i < letters; i++)
	{
		for(j = 0; j < n_prod; j++)
			if(productions[j][0] == temp_word[i])
				if(productions[j][1] == word[i])
				{
					temp_word[i] = productions[j][1];
					temp_word[(i + 1)] = productions[j][2];
					for(k = 0; k <= (i + 1); k++)
						cout << temp_word[k];
					cout << endl;
					if(temp_word[(i + 1)] != ' ')
						temp_letters++;
					j = n_prod;
				}
	}
	
	cout << endl;

	for(i = 0; i < temp_letters; i++)
	{
		if(temp_word[i] != word[i])
			language = false;
	}

	if(language == false)
		cout << "The word is not in the language" << endl;
	else
		cout << "The word is in the language" << endl;
	
	return 0;
}


