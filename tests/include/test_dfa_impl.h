#ifndef __TEST_DFA_H__
#define __TEST_DFA_H__

#include "states.h"
#include "test_base.h"
#include "token.h"
#include "validNumberDfa.h"
#include "keywordDfa.h"
#include <iostream>

std::pair<DFA_STATES, TOKEN_TYPE> e_pair(DFA_STATES ds, TOKEN_TYPE tt)
{
    return std::make_pair(ds, tt);
}

class Test_Dfa final: public Test_Base {
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
        void test_vn_dfa()
        {
            ValidNumberDfa vn_dfa;
            checkTrue("ValidNumberDfa", vn_dfa.getStatus() == e_pair(DS_RUNNING, TT_INVALID),
                      "Empty String", "");

            vn_dfa.transition('0');
            checkTrue("ValidNumberDfa", vn_dfa.getStatus() == e_pair(DS_ACCEPT, TT_NUM),
                      "Starts with 0", "0");

            vn_dfa.transition('a');
            checkTrue("ValidNumberDfa", vn_dfa.getStatus() == e_pair(DS_ERROR, TT_INVALID),
                      "Maintains error state", "0a");

            vn_dfa.transition('1');
            checkTrue("ValidNumberDfa", vn_dfa.getStatus() == e_pair(DS_ERROR, TT_INVALID),
                      "Maintains error state", "0a1");

            vn_dfa.resetDfa();
            vn_dfa.transition('1');
            checkTrue("ValidNumberDfa", vn_dfa.getStatus() == e_pair(DS_ACCEPT, TT_NUM),
                      "Valid Number", "1");

            vn_dfa.transition('5');
            checkTrue("ValidNumberDfa", vn_dfa.getStatus() == e_pair(DS_ACCEPT, TT_NUM),
                      "Valid Number", "15");

            vn_dfa.transition('a');
            checkTrue("ValidNumberDfa", vn_dfa.getStatus() == e_pair(DS_ERROR, TT_INVALID),
                      "Invalid Number", "15a");
        }

        void test_kw_dfa()
        {
            KeywordDfa kw_dfa;
            checkTrue("KeywordDfa", kw_dfa.getStatus() == e_pair(DS_RUNNING, TT_INVALID),
                      "Empty String", "");

            std::string str = "0123456789";
            for(char& c : str) {
                kw_dfa.transition(c);
                checkTrue("KeywordDfa", kw_dfa.getStatus() == e_pair(DS_ERROR, TT_INVALID),
                          "Number Test 0-9", std::to_string(c));
                kw_dfa.resetDfa();
            }

            str = "abstrac";
            for(char& c : str) {
                kw_dfa.transition(c);
                checkTrue("KeywordDfa", kw_dfa.getStatus() == e_pair(DS_RUNNING, TT_INVALID),
                          "word abstrac", std::to_string(c));
            }

            kw_dfa.transition('t');
            checkTrue("KeywordDfa", kw_dfa.getStatus() == e_pair(DS_ACCEPT, TT_ABSTRACT),
                      "Keyword abstract", "abstract");

            kw_dfa.transition('a');
            checkTrue("KeywordDfa", kw_dfa.getStatus() == e_pair(DS_ERROR, TT_INVALID),
                      "Keyword plus more", "abstracta");

            kw_dfa.transition('1');
            checkTrue("KeywordDfa", kw_dfa.getStatus() == e_pair(DS_ERROR, TT_INVALID),
                      "Error State Retained", "abstracta1");

            kw_dfa.resetDfa();
            str = "fina";
            for(char& c : str) {
                kw_dfa.transition(c);
                checkTrue("KeywordDfa", kw_dfa.getStatus() == e_pair(DS_RUNNING, TT_INVALID),
                          "word fina", std::to_string(c));
            }

            kw_dfa.transition('l');
            checkTrue("KeywordDfa", kw_dfa.getStatus() == e_pair(DS_ACCEPT, TT_FINAL),
                      "Keyword final - substring of finally", "final");
        }
};

#endif
