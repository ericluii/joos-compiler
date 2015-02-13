#ifndef __WEED_NO_REPEAT_MODIFIERS_H__
#define __WEED_NO_REPEAT_MODIFIERS_H__

#include "weed.h"

class NoRepeatModifiers : public Weed {
    protected:
        unsigned int hasMultipleSameModifier(ParseTree* node) {
            bool hasPublic = false;
            bool hasProtected = false;
            bool hasAbstract = false;
            bool hasStatic = false;
            bool hasFinal = false;
            bool hasNative = false;

            while(true) {
                switch(node->rule) {
                    case MEMBER_MOD:
                    case MODIFIERS_MOD:
                    case MEMBER_MOD_EXPAND:
                        node = node->children[0];
                        break;
                    case MODIFIERS_LIST:
                    case MEMBER_MOD_LIST:
                        switch(node->children[1]->rule) {
                            case MOD_PUBLIC:
                            case MEMBER_MOD_PUBLIC:
                                if(!hasPublic) {
                                    hasPublic = true;
                                    break;
                                } else {
                                    return 1;
                                }
                            case MOD_ABSTRACT:
                            case MEMBER_MOD_ABSTRACT:
                                if(!hasAbstract) {
                                    hasAbstract = true;
                                    break;
                                } else {
                                    return 1;
                                }
                            case MOD_FINAL:
                            case MEMBER_MOD_FINAL:
                                if(!hasFinal) {
                                    hasFinal = true;
                                    break;
                                } else {
                                    return 1;
                                }
                            case MEMBER_MOD_STATIC:
                                if(!hasStatic) {
                                    hasStatic = true;
                                    break;
                                } else {
                                    return 1;
                                }
                            case MEMBER_MOD_PROTECTED:
                                if(!hasProtected) {
                                    hasProtected = true;
                                    break;
                                } else {
                                    return 1;
                                }
                            case MEMBER_MOD_NATIVE:
                                if(!hasNative) {
                                    hasNative = true;
                                    break;
                                } else {
                                    return 1;
                                }
                        }
                        node = node->children[0];
                        break;
                    case MOD_PUBLIC:
                    case MEMBER_MOD_PUBLIC:
                        if(hasPublic) {
                            return 1;
                        } else {
                            return 0;
                        }
                    case MOD_ABSTRACT:
                    case MEMBER_MOD_ABSTRACT:
                        if(hasAbstract) {
                            return 1;
                        } else {
                            return 0;
                        }
                    case MOD_FINAL:
                    case MEMBER_MOD_FINAL:
                        if(hasFinal) {
                            return 1;
                        } else {
                            return 0;
                        }
                    case MEMBER_MOD_STATIC:
                        if(hasStatic) {
                            return 1;
                        } else {
                            return 0;
                        }
                    case MEMBER_MOD_PROTECTED:
                        if(hasProtected) {
                            return 1;
                        } else {
                            return 0;
                        }
                    case MEMBER_MOD_NATIVE:
                        if(hasNative) {
                            return 1;
                        } else {
                            return 0;
                        }
                    default:
                        return 0;
                }
            }
            return 0;
        }
};

#endif
