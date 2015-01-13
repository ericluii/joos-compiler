#include "dfa.h"
#include <string>

class CharStringLiteralDfa : public Dfa {
  private:
    void initDfa();
    // the characters that can be used in an escape sequence
    string escapeSequence = "btnfr\"'\\";
    unsigned int octalCounter = 0;
    unsigned int octalLimit = 3;
  public:
    CharStringLiteralDfa();
}
