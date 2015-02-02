#!/usr/bin/python

import os
import sys

def main():
    first = True
    with open(sys.argv[2], 'w') as writeTo:
        listdir = os.listdir(sys.argv[1])
        writeTo.write("#ifndef __TESTFILES_H__\n")
        writeTo.write("#define __TESTFILES_H__\n")
        writeTo.write("#define NUM_FILES " + str(len(listdir)) + "\n\n")
        writeTo.write("const char TestFiles [] = {\n")
        for files in os.listdir(sys.argv[1]):
            if first:
                writeTo.write('\t"'+files+'"')
                first = False
            else:
                writeTo.write(',\n\t"'+files+'"')

        writeTo.write('\n};\n\n#endif')

if __name__ == "__main__":
    if len(sys.argv) == 1:
        sys.stderr.write('Need an argument that specifies the directory and file to write to')
        sys.stderr.write('Usage: python listTestFiles.py *name of directory* *name of file to write to*')
    else:
        main()
