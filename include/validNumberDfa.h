#ifndef __DIGITSDFA_H__
#define __DIGITSDFA_H__

#include "dfa.h"

class ValidNumberDfa : public Dfa {
  private:
    void initDfa();
  public:
    ValidNumberDfa();
};

#endif
