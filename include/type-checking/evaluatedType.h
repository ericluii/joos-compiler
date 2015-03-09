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
    ET_NULL,
    ET_OBJECT,
    // denotes an array to some class/interface
    ET_OBJECTARRAY,
    // denotes that type hasn't been confirmed
    ET_NOTEVALUATED,
};

#endif
