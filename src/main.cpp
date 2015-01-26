#include <iostream>
#include <fstream>
#include <vector>
#include "Scanner.h"
#include "identifierDfa.h"
#include "validNumberDfa.h"
#include "operatorDfa.h"
#include "separatorDfa.h"
#include "charStringLiteralDfa.h"
#include "keywordDfa.h"
#include "singleCommentDfa.h"
#include "multiCommentDfa.h"

int main(int argc, char *argv[])
{
	//Dfas must be ordered in reverse priority. Dfas that are later in the list will have priority over earlier Dfas
	std::vector<Dfa*> dfas;
	dfas.push_back(new SingleCommentDfa());
	dfas.push_back(new MultiCommentDfa());
	dfas.push_back(new IdentifierDfa());
	dfas.push_back(new ValidNumberDfa());
	dfas.push_back(new OperatorDfa());
	dfas.push_back(new SeparatorDfa());
	dfas.push_back(new CharStringLiteralDfa());
	dfas.push_back(new KeywordDfa());

	Scanner scanner(dfas);
	std::ifstream file;
	std::vector<std::vector<Token*> *> tokens;
	for(int i = 1; i <= argc; i++)
	{
		file.open(argv[i], std::ifstream::in);
		tokens.push_back(scanner.Scan(file));
		file.close();
	}
	std::cout << "Print out tokens to prove it worked :D\n\n";
	for(int i = 0; i < argc; i++)
	{
		for(unsigned int j = 0; j < tokens[i]->size(); j++)
		{
			std::cout << tokens[i]->at(j)->getString() << "\n";
		}
	}
}
