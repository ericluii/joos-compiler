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
	int c = -2; //character read from file is represented as an int
	
	int numDfas = this->dfas.size();
	int errorCount = 0;
	std::vector<int> errorFlags(numDfas, 0);
	std::pair<DFA_STATES, TOKEN_TYPE> result;
	
	//Values for the token currently being scanned
	TOKEN_TYPE type = TT_INVALID;
	std::string lexime = "";
	int tokenLine = 0; 
	int tokenCollumn = 0; 
	
	int currentLine = 0;
	int currentColumn = -1;
	
	//std::string additionalInput = ""; //not needed for Simplified MM. Full MM is not currently implemented
	
	while(file)
	{
		if(c == -2)
		{
			//TODO? check additional input. Not needed for Simplified MM
			c = file.get();
			currentColumn++;
			if(c < -1 || c > 127) //EOF is represented as -1. ASCII is in the range 0-127
			{
				std::cerr << "Invalid character with code: " << c << "\n Input files must contain only ASCII characters";
			}
			if(c == 10) //line feed character LF. This will work for Unix and Windows newlines. Will not work for certain obscure opperating systems
			{
				currentLine++;
				currentColumn = -1;
			}
		}
		
		for(int i = 0; i < numDfas; i++) //Dfas must be ordered in reverse priority. Dfas that are later in the list will have priority over earlier Dfas
		{
			if(errorFlags[i] == 0)
			{
				result = dfas[i]->transition((char)c);
				switch(result.first)
				{
					case DS_ACCEPT:
						type = result.second;
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
		
		//std::cout << (char)c;
		assert(errorCount <= numDfas); //sanity check
		if(errorCount == numDfas)
		{
			if(type != TT_INVALID && type != TT_COMMENT)
			{
				tokens->push_back(new Token(type, lexime, std::pair <unsigned int, unsigned int>(tokenLine, tokenCollumn)));
			}
			//reset all the things
			type = TT_INVALID;
			lexime = "";
			if(c <= 32 || c == 127) //eliminates whitespace and control characters. This seemed more reasonable than creating an entire dfa for the same purpose
			{
				c = -2;
			}
			for(int i = 0; i < numDfas; i++)
			{
				dfas[i]->resetDfa();
				errorFlags[i] = 0;
			}
			errorCount = 0;	
			tokenLine = currentLine;
			tokenCollumn = currentColumn;
		}
		else
		{
			lexime += (char)c;
			c = -2;
		}
	}
	
	return tokens;
}
