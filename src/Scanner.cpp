#include "Scanner.h"
#include <iostream>
#include <cassert>
#include <string>

Scanner::Scanner(std::vector<Dfa*> pdfas)
{
	dfas = pdfas;
}

std::vector<Token*> *Scanner::Scan(std::ifstream& file)
{
	std::vector<Token*> *tokens =  new std::vector<Token*>();
	int c; //character read from file is represented as an int
	
	int numDfas = this->dfas.size();
	int errorCount = 0;
	std::vector<int> errorFlags(numDfas, 0);
	DFA_STATES result = DS_ACCEPT; //TODO remove initialization once transmission return value is modified
	
	//Values for the token currently being scanned
	TOKEN_TYPE type = TT_WORD; //TODO remove initialization once transmission return value is modified
	std::string lexime = "";
	int tokenLine = 0; //TODO count newlines
	int tokenCollumn = 0; //TODO count characters
	
	std::string additionalInput = ""; //not needed for Simplified MM
	
	while(file)
	{
		assert(errorCount <= numDfas); //sanity check
		if(errorCount == numDfas)
		{
			tokens->push_back(new Token(type, lexime, std::pair <unsigned int, unsigned int>(tokenLine, tokenCollumn)));
			
			//reset all the things
			lexime = "";
			lexime += (char)c;
			for(int i = 0; i < numDfas; i++)
			{
				dfas[i]->resetDfa();
				errorFlags[i] = 0;
			}
			errorCount = 0;		
		}
		else
		{
			//TODO? check additional input. Not needed for Simplified MM
			c = file.get();
			if(c < -1 || c > 127) //EOF is represented as -1. ASCII is in the range 0-127
			{
				std::cerr << "Invalid character with code: " << c << "\n Input files must contain only ASCII characters";
			}
		}
		
		for(int i = 0; i < numDfas; i++) //Dfas must be ordered in reverse priority. Dfas that are later in the list will have priority over earlier Dfas
		{
			if(errorFlags[i] == 0)
			{
				//result = dfas[i]->transition((char)c);
				switch(result)
				{
					case DS_ACCEPT:
						//TODO store value in type. Need to know return type to write this
						//type = TT_WORD;
						break;
					case DS_ERROR:
						errorFlags[i] = 1;
						errorCount++;
						break;
					default:
						break;
				}
			}
		}
		
		if(errorCount < numDfas)
		{
			lexime += (char)c;
		}
		//std::cout << (char)c;
	}
	
	return tokens;
}
