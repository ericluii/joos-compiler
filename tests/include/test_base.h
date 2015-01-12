#ifndef __TEST_BASE_H__
#define __TEST_BASE_H__

#include <iostream>
#include <string>
#include <vector>

class Test_Base {
    private:
        static int test_count;
        static std::vector<std::string> test_fails;

        void printPass(std::string name, std::string description);
        void printFail(std::string name, std::string description);
    protected:
        std::string test_name;
        std::string test_description;

        void checkTrue(std::string name, bool value, std::string description,
                       std::string raw_input);
        void checkRange(std::string name, int value, int min, int max,
                        std::string description, std::string raw_input);
    public:
        virtual ~Test_Base() {}
        virtual void init() {
            throw "Inheriting Test did not Implement.";
        };
        virtual void test() {
            throw "Inheriting Test did not Implement.";
        };

        int getTestFails()
        {
            return test_fails.size();
        }

        friend std::ostream& operator<< (std::ostream& os, const Test_Base& tb)
        {
            if (tb.test_fails.size() > 0) {
                os << "FAILED TEST SUMMARY:\n";
                os << "---------------------------------------------------------------------------------------\n";
                for (int i = 0; i < tb.test_fails.size(); i++) {
                    os << (i + 1) << ") " << tb.test_fails[i] << "\n";
                }
                os << "---------------------------------------------------------------------------------------\n";
            }
            os << "Passed " << (tb.test_count - tb.test_fails.size()) << " out of " << tb.test_count << " tests.\n";

            return os;
        }
};

#endif
