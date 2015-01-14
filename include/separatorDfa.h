#include "dfa.h"
#include <string>

class SeparatorDfa final : public Dfa {
    private:
        void initDfa();
    public:
        SeparatorDfa();
        
        static std::string separators;
};
