#ifndef __TEST_A4_H__
#define __TEST_A4_H__

#include "test_base.h"
#include "parser.h"
#include "scanner.h"
#include <string>

class Test_A4 : public Test_Base {
    private:
        std::string& directoryPath;
        std::string& stdlibPath;
        Scanner scanner;
        Parser* parser;
    public:
        void init();
        void test();

        Test_A4(std::string& stdlibPath, std::string& directoryPath);
};

#endif
