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

    if argv[0] == "-i":
        print "dfa[DS_ERROR] = std::make_pair(DS_ERROR, &error);"
        print "dfa[DS_START] = std::make_pair(DS_RUNNING, &pyGen);"
        for state in states:
            if state[3:].lower() in words_copy:
                print "dfa[" + state + "] = std::make_pair(DS_ACCEPT, &pyGen);"
            else:
                print "dfa[" + state + "] = std::make_pair(DS_RUNNING, &pyGen);"

    if argv[0] == "-g":
        print "switch (current_state) {"
        print "    case DS_START:"
        for letter in alpha:
            if "DS_" + letter in states:
                print "        if (c == '" + letter.lower() + "') {"
                print "            return DS_" + letter + ";"
                print "        }"
        print "        return DS_ERROR;"
        for state in states:
            print "    case " + state + ":"
            
            for letter in alpha:
                if state + letter in states:
                    print "        if (c == '" + letter.lower() + "') {"
                    print "            return " + state + letter + ";"
                    print "        }"

            print "        return DS_ERROR;"
        print "    default:"
        print "        return DS_ERROR;"
        print "}"

if __name__ == "__main__":
    main(sys.argv[1:])
