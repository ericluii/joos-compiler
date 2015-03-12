#ifndef __EVALUATEDTYPE_H__
#define __EVALUATEDTYPE_H__

enum EVALUATED_TYPE {
    ET_INT = 0,
    ET_SHORT,
    ET_BYTE,
    ET_CHAR,
    ET_BOOLEAN,
    ET_INTARRAY,
    ET_SHORTARRAY,
    ET_BYTEARRAY,
    ET_CHARARRAY,
    ET_BOOLEANARRAY,
    // DO NOT CHANGE THE ORDER OF THE ABOVE
    // denotes null type when expression
    // consists of only a Primary which is a LiteralOrThis that is null
    ET_NULL,
    // denotes void type
    ET_VOID,
    // denotes that type hasn't been confirmed
    ET_NOTEVALUATED,
    ET_OBJECT,
    // denotes an array to some class/interface
    ET_OBJECTARRAY,
};

#endif
