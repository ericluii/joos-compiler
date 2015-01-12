#ifndef __TEST_DFA_H__
#define __TEST_DFA_H__

#include "test_base.h"
#include "validNumberDfa.h"
#include <iostream>

class Test_Dfa: public Test_Base {
    public:
        void init()
        {
            test_name = "DFA State Test";
            test_description = "This test tries different character sequences and checks that\n"
                                "the states are correctly maintained in each DFA.";
        }

        void test()
        {
            std::cout << test_name << ": " << test_description << std::endl;
            std::cout << "---------------------------------------------------------------------------------------" << std::endl;

            ValidNumberDfa vn_dfa;
            checkTrue("ValidNumberDfa", !vn_dfa.onAcceptingState(),
                      "Empty String", "");

            vn_dfa.transition('0');
            checkTrue("ValidNumberDfa", !vn_dfa.onAcceptingState(),
                      "Starts with 0", "0");

            vn_dfa.transition('a');
            checkTrue("ValidNumberDfa", !vn_dfa.onAcceptingState(),
                      "Maintains error state", "0a");

            vn_dfa.transition('1');
            checkTrue("ValidNumberDfa", !vn_dfa.onAcceptingState(),
                      "Maintains error state", "0a1");

            vn_dfa.resetDfa();
            vn_dfa.transition('1');
            checkTrue("ValidNumberDfa", vn_dfa.onAcceptingState(),
                      "Valid Number", "1");

            vn_dfa.transition('5');
            checkTrue("ValidNumberDfa", vn_dfa.onAcceptingState(),
                      "Valid Number", "15");

            vn_dfa.transition('a');
            checkTrue("ValidNumberDfa", !vn_dfa.onAcceptingState(),
                      "Invalid Number", "15a");

            std::cout << "---------------------------------------------------------------------------------------" << std::endl;
        }
};

#endif
