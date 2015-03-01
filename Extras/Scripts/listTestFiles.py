#!/usr/bin/python

import os
import sys
import subprocess

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
        writeTo.write("#include <vector>\n")
        writeTo.write("#include <string>\n\n")
        writeTo.write("extern std::vector<std::vector<std::string>> "+assignment.lower()+"TestFiles;\n\n")
        writeTo.write("#endif")

    with open(sys.argv[3], 'w') as writeTo:
        writeTo.write("#include \""+headerFileName+"\"\n\n")
        writeTo.write("std::vector<std::vector<std::string>> "+assignment.lower()+"TestFiles {\n")

        last_dir = "";
        p = subprocess.Popen('find tests/' + assignment.lower() + ' -iname *.java', shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        for files in p.stdout.readlines():
            if files == ".DS_Store":
                continue

            f = files[files.find(assignment.lower()) + 3:-1]
            dir_idx = f.find('/')
            if dir_idx == -1:
                this_dir = ""
            else:
                this_dir = f[:dir_idx + 1]

            if first and (this_dir != last_dir or this_dir == ""):
                writeTo.write('{\n');
            elif this_dir != last_dir or this_dir == "":
                writeTo.write('\n}, {\n');
                first = True
            last_dir = this_dir

            if first:
                writeTo.write('\t"'+ f +'"')
                first = False
            else:
                writeTo.write(',\n\t"'+ f +'"')

        retval = p.wait();

        writeTo.write('}\n};')

if __name__ == "__main__":
    if len(sys.argv) != 5:
        sys.stderr.write('Need an argument that specifies the directory and file to write to')
        sys.stderr.write('Usage: python listTestFiles.py *name of directory* *header file name* *source file name* \
                         *assignment number e.g a1*')
    else:
        main()
