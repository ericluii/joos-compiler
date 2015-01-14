#!/usr/bin/python
import string
import sys

alpha = list(string.ascii_uppercase)

def main(argv):
    depth = 1
    words = [line.strip() for line in open(argv[1])]
    words_copy = words
    states = []

    while words:
        words = filter(lambda x: len(x) >= depth, words)
        
        for word in words:
            if "DS_" + word[:depth].upper() not in states:
                states.append("DS_" + word[:depth].upper())

        depth += 1

    if argv[0] == "-s":
        for state in states:
            print state + ","

    if argv[0] == "-t":
        for state in states:
            if state[3:].lower() in words_copy:
                print "TT" + state[2:] + ","

    if argv[0] == "-g":
        print "#include \"states.h\""
        print "#include \"REPLACE.h\""
        print ""
        print "int pyGen(char c, int current_state)"
        print "{"
        print "    switch (current_state) {"
        print "        case DS_START:"
        for letter in alpha:
            if "DS_" + letter in states:
                print "            if (c == '" + letter.lower() + "') { return DS_" + letter + "; }";
        print "            return DS_ERROR;"
        for state in states:
            print "        case " + state + ":"
            
            for letter in alpha:
                if state + letter in states:
                    print "            if (c == '" + letter.lower() + "') { return " + state + letter + "; }"
            print "            return DS_ERROR;"
        print "        default:"
        print "            return DS_ERROR;"
        print "    }"
        print "}"
        print ""
        print "TOKEN_TYPE REPLACE::getTokenType()"
        print "{"
        print "    if (dfa[current_state].first != DS_ACCEPT) {"
        print "        return TT_INVALID;"
        print "    }"
        print ""
        for state in states:
            if state[3:].lower() in words_copy:
                print "    if (current_state == " + state + ") { return TT" + state[2:] + "; }"
        print "    return TT_INVALID;"
        print "}"
        print ""
        print "void REPLACE::initDfa()"
        print "{"
        print "    dfa[DS_ERROR] = std::make_pair(DS_ERROR, &error);"
        print "    dfa[DS_START] = std::make_pair(DS_RUNNING, &pyGen);"
        for state in states:
            if state[3:].lower() in words_copy:
                print "    dfa[" + state + "] = std::make_pair(DS_ACCEPT, &pyGen);"
            else:
                print "    dfa[" + state + "] = std::make_pair(DS_RUNNING, &pyGen);"
        print "}"
        print ""
        print "REPLACE::REPLACE() : Dfa()"
        print "{"
        print "    initDfa();"
        print "}"

if __name__ == "__main__":
    main(sys.argv[1:])
