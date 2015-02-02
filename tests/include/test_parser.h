#ifndef __TEST_PARSER_H__
#define __TEST_PARSER_H__

#include "test_base.h"
#include "parser.h"
#include "scanner.h"
#include <string>

class Test_Parser : public Test_Base {
    private:
        std::string& directoryPath;
        Scanner scanner;
        Parser* parser;
    public:
        void init();
        void test();

        Test_Parser(std::string& directoryPath);
};

#endif
