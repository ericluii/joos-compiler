#!/usr/bin/python

import sys

def main():
    numRules = 0
    numStates = 0
    numActions = 0
 
    headerFileName = sys.argv[2].split('/')
    headerFileName = headerFileName[len(headerFileName)-1]
    with open(sys.argv[1]) as parserFile:
        numRules = int(parserFile.readline())
        with open(sys.argv[3],'w') as sourceFile:
            sourceFile.write("#include \"" + headerFileName + "\"\n\n")
            sourceFile.write("std::string rulesAndActions [] = {\n")

            sourceFile.write('\t"'+parserFile.readline().rstrip('\n')+'"')
            for i in xrange(1, numRules):
                sourceFile.write(',\n\t"'+parserFile.readline().rstrip('\n')+'"')

            numStates = int(parserFile.readline())
            numActions = int(parserFile.readline())
            for i in xrange(0, numActions):
                sourceFile.write(',\n\t"'+parserFile.readline().rstrip('\n')+'"')

            sourceFile.write("\n};")

    with open(sys.argv[2],'w') as headerFile:
        headerFile.write("#ifndef __PARSERRULESANDACTIONS_H__\n")
        headerFile.write("#define __PARSERRULESANDACTIONS_H__\n\n")
        headerFile.write("#include <string>\n")

        headerFile.write("#define NUM_RULES " + str(numRules) + "\n")
        headerFile.write("#define NUM_STATES " + str(numStates) + "\n")
        headerFile.write("#define NUM_ACTIONS " + str(numActions) + "\n\n")
        headerFile.write("extern std::string rulesAndActions[];\n\n")
        headerFile.write("#endif")

if __name__ == "__main__":
    if len(sys.argv) != 4:
        sys.stderr.write('Incorrect number of arguments.\n\
                          Usage: python readParserRulesAndTable.py *rules and table file* *.h file* *.cpp file*')
    else:
        main()
