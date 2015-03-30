#include <sstream>
#include "test_base.h"

int Test_Base::test_count;
std::vector<std::string> Test_Base::test_fails;

void Test_Base::printPass(std::string name, std::string description)
{
#ifndef _WIN32
    std::cout << "\033[0;32m[PASSED] " << test_name << " - " << name << " : " << description << "\033[0m" << std::endl;
#else
    std::cout << "[PASSED] " << test_name << " - " << name << " : " << description << std::endl;
#endif
}

void Test_Base::printFail(std::string name, std::string description)
{
#ifndef _WIN32
    std::cout << "\033[0;31m[FAILED] " << test_name << " - " << name << " : " << description << "\033[0m" << std::endl;
#else
    std::cout << "[FAILED] " << test_name << " - " << name << " : " << description << std::endl;
#endif
}

void Test_Base::checkTrue(std::string name, bool value,
                          std::string description, std::string raw_input)
{
    test_count++;

    if (!value) {
        std::stringstream ss;
        ss << name << " : " << description << " : " << raw_input << " : " << value;
        test_fails.push_back(ss.str());

        printFail(name, description);
    } else {
        printPass(name, description);
    }
}

void Test_Base::checkRange(std::string name, int value, int min, int max,
                           std::string description, std::string raw_input)
{
    test_count++;

    if (value < min || value > max) {
        std::stringstream ss;
        ss << name << " : " << description << " : " << raw_input << " : " << value << " [" << min << ", " << max << "]";
        test_fails.push_back(ss.str());

        printFail(name, description);
    } else {
        printPass(name, description);
    }
}
