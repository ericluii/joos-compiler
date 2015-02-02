#!/usr/bin/python

import sys

def main():
    numRules = 0
    numStates = 0
    numActions = 0
    ignore = 0

    ruleHeaderFileName = sys.argv[2].split('/')
    ruleHeaderFileName = ruleHeaderFileName[len(ruleHeaderFileName)-1]
    actionHeaderFileName = sys.argv[4].split('/')
    actionHeaderFileName = actionHeaderFileName[len(actionHeaderFileName)-1]
    with open(sys.argv[1]) as parserFile:
        ignore = int(parserFile.readline())
        for i in xrange(0, ignore):
            parserFile.readline()

        ignore = int(parserFile.readline())+1
        for i in xrange(0, ignore):
            parserFile.readline()

        numRules = int(parserFile.readline())
        with open(sys.argv[3],'w') as sourceFile:
            sourceFile.write("#include \"" + ruleHeaderFileName + "\"\n\n")
            sourceFile.write("std::string parserRules [] = {\n")

            sourceFile.write('\t"'+parserFile.readline().rstrip('\n')+'"')
            for i in xrange(1, numRules):
                sourceFile.write(',\n\t"'+parserFile.readline().rstrip('\n')+'"')
            
            sourceFile.write("\n};")
        
        numStates = int(parserFile.readline())
        numActions = int(parserFile.readline())

        with open(sys.argv[5],'w') as sourceFile:
            sourceFile.write("#include \"" + actionHeaderFileName + "\"\n\n")
            sourceFile.write("std::string parserActions [] = {\n")
           
            sourceFile.write('\t"'+parserFile.readline().rstrip('\n')+'"')
            for i in xrange(1, numActions):
                sourceFile.write(',\n\t"'+parserFile.readline().rstrip('\n')+'"')
            
            sourceFile.write("\n};")

    with open(sys.argv[2],'w') as headerFile:
        headerFile.write("#ifndef __PARSERRULES_H__\n")
        headerFile.write("#define __PARSERRULES_H__\n\n")
        headerFile.write("#include <string>\n")

        headerFile.write("#define NUM_RULES " + str(numRules) + "\n\n")
        headerFile.write("extern std::string parserRules[];\n\n")
        headerFile.write("#endif")
    
    with open(sys.argv[4],'w') as headerFile:
        headerFile.write("#ifndef __PARSERACTIONS_H__\n")
        headerFile.write("#define __PARSERACTIONS_H__\n\n")
        headerFile.write("#include <string>\n")

        headerFile.write("#define NUM_STATES " + str(numStates) + "\n")
        headerFile.write("#define NUM_ACTIONS " + str(numActions) + "\n\n")
        headerFile.write("extern std::string parserActions[];\n\n")
        headerFile.write("#endif")

if __name__ == "__main__":
    if len(sys.argv) != 6:
        sys.stderr.write('Incorrect number of arguments.\n\
                          Usage: python readParserRulesAndTable.py \
                          *rulesAndActions file* *rules.h file* *rules.cpp* file* *actions.h* *actions.cpp*')
    else:
        main()
