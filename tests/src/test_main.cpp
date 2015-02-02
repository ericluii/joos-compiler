#include "test_dfa_impl.h"
#include "test_scanner.h"
#include "test_parser.h"
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

    // Add tests to this vector if you want
    // to add it to the test sequence
    std::vector<Test_Base *> test_list;
    // test_list.push_back(new Test_Dfa);
    std::string directoryPath;
    if(argc == 2) {
        directoryPath = argv[1];
        //test_list.push_back(new Test_Scanner(directoryPath));
        test_list.push_back(new Test_Parser(directoryPath));
    }
    
    for_each(test_list.begin(), test_list.end(), run_test);

    std::cout << base;
    return base.getTestFails();
}
