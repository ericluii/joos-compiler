#ifndef __TEST_DFA_H__
#define __TEST_DFA_H__

#include "test_base.h"
#include "validNumberDfa.h"
#include "keywordDfa.h"
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

            test_vn_dfa();
            test_kw_dfa();

            std::cout << "---------------------------------------------------------------------------------------" << std::endl;
        }
    private:
        void test_vn_dfa() {
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

        }

        void test_kw_dfa() {
            KeywordDfa kw_dfa;
            checkTrue("KeywordDfa", !kw_dfa.onAcceptingState(),
                      "Empty String", "");

            std::string str = "0123456789";
            for(char& c : str) {
                kw_dfa.transition(c);
                checkTrue("KeywordDfa", !kw_dfa.onAcceptingState(),
                          "Number Test 0-9", std::to_string(c));
                kw_dfa.resetDfa();
            }

            str = "abstrac";
            for(char& c : str) {
                kw_dfa.transition(c);
                checkTrue("KeywordDfa", !kw_dfa.onAcceptingState(),
                          "word abstrac", std::to_string(c));
            }

            kw_dfa.transition('t');
            checkTrue("KeywordDfa", kw_dfa.onAcceptingState(),
                      "Keyword abstract", "abstract");

            kw_dfa.transition('a');
            checkTrue("KeywordDfa", !kw_dfa.onAcceptingState(),
                      "Keyword plus more", "abstracta");

            kw_dfa.resetDfa();
            str = "fina";
            for(char& c : str) {
                kw_dfa.transition(c);
                checkTrue("KeywordDfa", !kw_dfa.onAcceptingState(),
                          "word fina", std::to_string(c));
            }

            kw_dfa.transition('l');
            checkTrue("KeywordDfa", kw_dfa.onAcceptingState(),
                      "Keyword final - substring of finally", "final");

            kw_dfa.transition('l');
            checkTrue("KeywordDfa", !kw_dfa.onAcceptingState(),
                      "Keyword final + l - substring of finally", "finall");

            kw_dfa.transition('y');
            checkTrue("KeywordDfa", kw_dfa.onAcceptingState(),
                      "Keyword finally - contains final", "finally");
        }
};

#endif
