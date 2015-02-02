#ifndef __TEST_SCANNER_H__
#define __TEST_SCANNER_H__

#include "test_base.h"
#include "scanner.h"
#include <string>

class Test_Scanner : public Test_Base {
    private:
        std::string& directoryPath;
        Scanner scanner;
    public:
        void init();
        void test();
        Test_Scanner(std::string& directoryPath);
};

#endif
