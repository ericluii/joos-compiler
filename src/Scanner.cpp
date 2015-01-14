#include "Scanner.h"
#include <iostream>
#include <cassert>

Scanner::Scanner(std::vector<Dfa*> pdfas)
{
	dfas = pdfas;
}

std::vector<Token> *Scanner::Scan(std::ifstream& file)
{
	std::vector<Token> *tokens =  new std::vector<Token>();
	int c; //character read from file is represented as an int
	
	int numDfas = this->dfas.size();
	int errorCount;
	std::vector<int> errorFlags(numDfas, 0);
	
	while(file)
	{
		assert(errorCount <= numDfas);
		if(errorCount == numDfas)
		{
			//reset all the things
		}
		else
		{
			c = file.get();
			if(c < -1 || c > 127)
			{
				std::cerr << "Invalid character with code: " << c << "\n Input files must contain only ASCII characters";
			}
		}
		
		
		DFA_STATES result = DS_ACCEPT;
		for(int i = 0; i < numDfas; i++)
		{
			if(errorFlags[i] == 0)
			{
				//result = dfas[i]->transition((char)c);
				switch(result)
				{
					case DS_ACCEPT:
						//store value
						break;
					case DS_ERROR:
						errorFlags[i] = 1;
						errorCount++;
						break;
					default:
						//TODO
						break;
				}
			}
		}
		std::cout << (char)c;
	}
	
	return tokens;
}
