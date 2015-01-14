#ifndef __VALIDNUMBERDFA_H__
#define __VALIDNUMBERDFA_H__

#include "dfa.h"

class ValidNumberDfa final : public Dfa {
  private:
    void initDfa();
    TOKEN_TYPE getTokenType();
  public:
    ValidNumberDfa();
};

#endif
