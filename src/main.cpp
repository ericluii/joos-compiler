#include <iostream>
#include <fstream>
#include <vector>
#include "Scanner.h"
#include "validNumberDfa.h"

int main(int argc, char *argv[])
{
	//Dfas must be ordered in reverse priority. Dfas that are later in the list will have priority over earlier Dfas
	std::vector<Dfa*> dfas;
	dfas.push_back(new ValidNumberDfa());

	Scanner scanner(dfas);
	std::ifstream file;
	std::vector<Token*> *tokens;
	for(int i = 1; i <= argc; i++)
	{
		file.open(argv[i], std::ifstream::in);
		tokens = scanner.Scan(file);
		file.close();
	}
    std::cout << "Hello World!";
}
