#!/usr/bin/python

import sys

def main(argv):
    print "#ifndef __PARSERRULESANDACTIONS_H__"
    print "#define __PARSERRULESANDACTIONS_H__\n"
    print "#include <string>\n"
    print "std::string rulesAndActions [] = {"
    
    with open(argv[1]) as parserFile:
        numRules = parserFile.readline()
        print '\t"'+parserFile.readline().rstrip('\n')+'"',
        for i in xrange(1, int(numRules)):
            print ',\n\t"'+parserFile.readline().rstrip('\n')+'"',

        numStates = parserFile.readline()
        numActions = parserFile.readline()
        for i in xrange(0, int(numActions)):
            print ',\n\t"'+parserFile.readline().rstrip('\n')+'"',

    print "\n};\n"
    print "#define NUM_RULES ", numRules,
    print "#define NUM_STATES ", numStates,
    print "#define NUM_ACTIONS ", numActions
    print "#endif"

if __name__ == "__main__":
    main(sys.argv)
