#include "test_dfa_impl.h"
#include <iostream>
#include <vector>

void run_test(Test_Base* test)
{
    test->init();
    test->test();
    delete test;
}

int main() {
    Test_Base base;

    // Add tests to this vector if you want
    // to add it to the test sequence
    std::vector<Test_Base *> test_list;
    test_list.push_back(new Test_Dfa);

    for_each(test_list.begin(), test_list.end(), run_test);

    std::cout << base;
    return base.getTestFails();
}
