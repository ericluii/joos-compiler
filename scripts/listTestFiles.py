#!/usr/bin/python

import os
import sys

def main():
    first = True
    assignment = sys.argv[4].upper()
    headerFileName = sys.argv[2].split('/')
    headerFileName = headerFileName[len(headerFileName)-1]
    with open(sys.argv[2], 'w') as writeTo:
        listdir = os.listdir(sys.argv[1])
        writeTo.write("#ifndef __"+assignment+"TESTFILES_H__\n")
        writeTo.write("#define __"+assignment+"TESTFILES_H__\n")
        writeTo.write("#define "+assignment+"_NUM_FILES " + str(len(listdir)) + "\n\n")
        writeTo.write("#include <string>\n\n")
        writeTo.write("extern std::string "+assignment.lower()+"TestFiles[];\n\n")
        writeTo.write("#endif")

    with open(sys.argv[3], 'w') as writeTo:
        writeTo.write("#include \""+headerFileName+"\"\n\n")
        writeTo.write("std::string "+assignment.lower()+"TestFiles [] = {\n")
        for files in os.listdir(sys.argv[1]):
            if first:
                writeTo.write('\t"'+files+'"')
                first = False
            else:
                writeTo.write(',\n\t"'+files+'"')

        writeTo.write('\n};')

if __name__ == "__main__":
    if len(sys.argv) != 5:
        sys.stderr.write('Need an argument that specifies the directory and file to write to')
        sys.stderr.write('Usage: python listTestFiles.py *name of directory* *header file name* *source file name* \
                         *assignment number e.g a1*')
    else:
        main()
