#include "test_dfa_impl.h"
#include "test_scanner.h"
#include "test_parser.h"
#include "test_a2.h"
#include "test_a3.h"
#include "test_a4.h"
#include "test_a5.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

void run_test(Test_Base* test)
{
    test->init();
    test->test();
    delete test;
}

int main(int argc, char** argv) {
    Test_Base base;

    // Required Dependencies
    std::string a1TestPath = "tests/a1";
    std::string stdlibPath = "tests/stdlib";
    std::string a2TestPath = "tests/a2";
    std::string a3TestPath = "tests/a3";
    std::string a4TestPath = "tests/a4";
    std::string a5TestPath = "tests/a5";

    // Add tests to this vector if you want
    // to add it to the test sequence
    std::vector<Test_Base *> test_list;
    test_list.push_back(new Test_Dfa);
    test_list.push_back(new Test_Scanner(a1TestPath));
    test_list.push_back(new Test_Parser(a1TestPath));
    test_list.push_back(new Test_A2(stdlibPath, a2TestPath));
    test_list.push_back(new Test_A3(stdlibPath, a3TestPath));
    test_list.push_back(new Test_A4(stdlibPath, a4TestPath));
    test_list.push_back(new Test_A5(stdlibPath, a5TestPath));

    for_each(test_list.begin(), test_list.end(), run_test);
    std::cout << base;
    return base.getTestFails();
}
