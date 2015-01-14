#ifndef Scanner_h
#define Scanner_h

#include "dfa.h"
#include "token.h"
#include "states.h"
#include <vector>
#include <fstream>

class Scanner
{
	std::vector<Dfa*> dfas;
public:
	std::vector<Token> *Scan(std::ifstream& file);
	Scanner(std::vector<Dfa*> pdfas);
};

#endif

