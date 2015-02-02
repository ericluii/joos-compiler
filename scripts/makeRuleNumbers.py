#!/usr/bin/python

def main():
    print "#ifndef __RULENUMBERS_H__"
    print "#define __RULENUMBERS_H__"
    with open(sys.argv[1]) as rules:
