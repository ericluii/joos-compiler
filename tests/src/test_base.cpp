#include "test_base.h"

int Test_Base::test_count;
std::vector<std::string> Test_Base::test_fails;

void Test_Base::printPass(std::string name, std::string description)
{
    std::cout << "[PASSED] " << test_name << " - " << name << " : " << description<< std::endl;
}

void Test_Base::printFail(std::string name, std::string description)
{
    std::cout << "[FAILED] " << test_name << " - " << name << " : " << description << std::endl;
}

void Test_Base::checkTrue(std::string name, bool value,
                          std::string description, std::string raw_input)
{
    test_count++;

    if (!value) {
        test_fails.push_back(name + " : " + description + " : " + raw_input + " : " +
                             std::to_string(value));
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
        test_fails.push_back(name + " : " + description + " : " + raw_input + " : " +
                             std::to_string(value) + " [" + std::to_string(min)
                             + ", " + std::to_string(max) + "]");
        printFail(name, description);
    } else {
        printPass(name, description);
    }
}