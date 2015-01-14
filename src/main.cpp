#include <iostream>
#include <fstream>
#include <vector>
#include "Scanner.h"
#include "validNumberDfa.h"

int main(int argc, char *argv[])
{
	std::vector<Dfa*> dfas;
	Dfa *validNumberDfa = new ValidNumberDfa(); 
	dfas.push_back(validNumberDfa);

	Scanner scanner(dfas);
	std::ifstream file;
	std::vector<Token> *tokens;
	for(int i = 1; i <= argc; i++)
	{
		file.open(argv[i], std::ifstream::in);
		tokens = scanner.Scan(file);
		file.close();
	}
    std::cout << "Hello World!";
}
