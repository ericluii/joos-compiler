#include "states.h"
#include "keywordDfa.h"

int pyGen(char c, int current_state) {
    switch(current_state) {
        case DS_START:
            if (c == 'a') {
                return DS_A;
            }
            if (c == 'b') {
                return DS_B;
            }
            if (c == 'c') {
                return DS_C;
            }
            if (c == 'd') {
                return DS_D;
            }
            if (c == 'e') {
                return DS_E;
            }
            if (c == 'f') {
                return DS_F;
            }
            if (c == 'g') {
                return DS_G;
            }
            if (c == 'i') {
                return DS_I;
            }
            if (c == 'l') {
                return DS_L;
            }
            if (c == 'n') {
                return DS_N;
            }
            if (c == 'p') {
                return DS_P;
            }
            if (c == 'r') {
                return DS_R;
            }
            if (c == 's') {
                return DS_S;
            }
            if (c == 't') {
                return DS_T;
            }
            if (c == 'v') {
                return DS_V;
            }
            if (c == 'w') {
                return DS_W;
            }
            return DS_ERROR;
        case DS_A:
            if (c == 'b') {
                return DS_AB;
            }
            if (c == 's') {
                return DS_AS;
            }
            return DS_ERROR;
        case DS_B:
            if (c == 'o') {
                return DS_BO;
            }
            if (c == 'r') {
                return DS_BR;
            }
            if (c == 'y') {
                return DS_BY;
            }
            return DS_ERROR;
        case DS_C:
            if (c == 'a') {
                return DS_CA;
            }
            if (c == 'h') {
                return DS_CH;
            }
            if (c == 'l') {
                return DS_CL;
            }
            if (c == 'o') {
                return DS_CO;
            }
            return DS_ERROR;
        case DS_D:
            if (c == 'e') {
                return DS_DE;
            }
            if (c == 'o') {
                return DS_DO;
            }
            return DS_ERROR;
        case DS_E:
            if (c == 'l') {
                return DS_EL;
            }
            if (c == 'n') {
                return DS_EN;
            }
            if (c == 'x') {
                return DS_EX;
            }
            return DS_ERROR;
        case DS_F:
            if (c == 'i') {
                return DS_FI;
            }
            if (c == 'l') {
                return DS_FL;
            }
            if (c == 'o') {
                return DS_FO;
            }
            return DS_ERROR;
        case DS_I:
            if (c == 'f') {
                return DS_IF;
            }
            if (c == 'm') {
                return DS_IM;
            }
            if (c == 'n') {
                return DS_IN;
            }
            return DS_ERROR;
        case DS_G:
            if (c == 'o') {
                return DS_GO;
            }
            return DS_ERROR;
        case DS_L:
            if (c == 'o') {
                return DS_LO;
            }
            return DS_ERROR;
        case DS_N:
            if (c == 'a') {
                return DS_NA;
            }
            if (c == 'e') {
                return DS_NE;
            }
            return DS_ERROR;
        case DS_P:
            if (c == 'a') {
                return DS_PA;
            }
            if (c == 'r') {
                return DS_PR;
            }
            if (c == 'u') {
                return DS_PU;
            }
            return DS_ERROR;
        case DS_R:
            if (c == 'e') {
                return DS_RE;
            }
            return DS_ERROR;
        case DS_S:
            if (c == 'h') {
                return DS_SH;
            }
            if (c == 't') {
                return DS_ST;
            }
            if (c == 'u') {
                return DS_SU;
            }
            if (c == 'w') {
                return DS_SW;
            }
            if (c == 'y') {
                return DS_SY;
            }
            return DS_ERROR;
        case DS_T:
            if (c == 'h') {
                return DS_TH;
            }
            if (c == 'r') {
                return DS_TR;
            }
            return DS_ERROR;
        case DS_V:
            if (c == 'o') {
                return DS_VO;
            }
            return DS_ERROR;
        case DS_W:
            if (c == 'h') {
                return DS_WH;
            }
            return DS_ERROR;
        case DS_AB:
            if (c == 's') {
                return DS_ABS;
            }
            return DS_ERROR;
        case DS_AS:
            if (c == 's') {
                return DS_ASS;
            }
            return DS_ERROR;
        case DS_BO:
            if (c == 'o') {
                return DS_BOO;
            }
            return DS_ERROR;
        case DS_BR:
            if (c == 'e') {
                return DS_BRE;
            }
            return DS_ERROR;
        case DS_BY:
            if (c == 't') {
                return DS_BYT;
            }
            return DS_ERROR;
        case DS_CA:
            if (c == 's') {
                return DS_CAS;
            }
            if (c == 't') {
                return DS_CAT;
            }
            return DS_ERROR;
        case DS_CH:
            if (c == 'a') {
                return DS_CHA;
            }
            return DS_ERROR;
        case DS_CL:
            if (c == 'a') {
                return DS_CLA;
            }
            return DS_ERROR;
        case DS_CO:
            if (c == 'n') {
                return DS_CON;
            }
            return DS_ERROR;
        case DS_DE:
            if (c == 'f') {
                return DS_DEF;
            }
            return DS_ERROR;
        case DS_DO:
            if (c == 'u') {
                return DS_DOU;
            }
            return DS_ERROR;
        case DS_EL:
            if (c == 's') {
                return DS_ELS;
            }
            return DS_ERROR;
        case DS_EN:
            if (c == 'u') {
                return DS_ENU;
            }
            return DS_ERROR;
        case DS_EX:
            if (c == 't') {
                return DS_EXT;
            }
            return DS_ERROR;
        case DS_FI:
            if (c == 'n') {
                return DS_FIN;
            }
            return DS_ERROR;
        case DS_FL:
            if (c == 'o') {
                return DS_FLO;
            }
            return DS_ERROR;
        case DS_FO:
            if (c == 'r') {
                return DS_FOR;
            }
            return DS_ERROR;
        case DS_IF:
            return DS_ERROR;
        case DS_GO:
            if (c == 't') {
                return DS_GOT;
            }
            return DS_ERROR;
        case DS_IM:
            if (c == 'p') {
                return DS_IMP;
            }
            return DS_ERROR;
        case DS_IN:
            if (c == 's') {
                return DS_INS;
            }
            if (c == 't') {
                return DS_INT;
            }
            return DS_ERROR;
        case DS_LO:
            if (c == 'n') {
                return DS_LON;
            }
            return DS_ERROR;
        case DS_NA:
            if (c == 't') {
                return DS_NAT;
            }
            return DS_ERROR;
        case DS_NE:
            if (c == 'w') {
                return DS_NEW;
            }
            return DS_ERROR;
        case DS_PA:
            if (c == 'c') {
                return DS_PAC;
            }
            return DS_ERROR;
        case DS_PR:
            if (c == 'i') {
                return DS_PRI;
            }
            if (c == 'o') {
                return DS_PRO;
            }
            return DS_ERROR;
        case DS_PU:
            if (c == 'b') {
                return DS_PUB;
            }
            return DS_ERROR;
        case DS_RE:
            if (c == 't') {
                return DS_RET;
            }
            return DS_ERROR;
        case DS_SH:
            if (c == 'o') {
                return DS_SHO;
            }
            return DS_ERROR;
        case DS_ST:
            if (c == 'a') {
                return DS_STA;
            }
            if (c == 'r') {
                return DS_STR;
            }
            return DS_ERROR;
        case DS_SU:
            if (c == 'p') {
                return DS_SUP;
            }
            return DS_ERROR;
        case DS_SW:
            if (c == 'i') {
                return DS_SWI;
            }
            return DS_ERROR;
        case DS_SY:
            if (c == 'n') {
                return DS_SYN;
            }
            return DS_ERROR;
        case DS_TH:
            if (c == 'i') {
                return DS_THI;
            }
            if (c == 'r') {
                return DS_THR;
            }
            return DS_ERROR;
        case DS_TR:
            if (c == 'a') {
                return DS_TRA;
            }
            if (c == 'y') {
                return DS_TRY;
            }
            return DS_ERROR;
        case DS_VO:
            if (c == 'i') {
                return DS_VOI;
            }
            if (c == 'l') {
                return DS_VOL;
            }
            return DS_ERROR;
        case DS_WH:
            if (c == 'i') {
                return DS_WHI;
            }
            return DS_ERROR;
        case DS_ABS:
            if (c == 't') {
                return DS_ABST;
            }
            return DS_ERROR;
        case DS_ASS:
            if (c == 'e') {
                return DS_ASSE;
            }
            return DS_ERROR;
        case DS_BOO:
            if (c == 'l') {
                return DS_BOOL;
            }
            return DS_ERROR;
        case DS_BRE:
            if (c == 'a') {
                return DS_BREA;
            }
            return DS_ERROR;
        case DS_BYT:
            if (c == 'e') {
                return DS_BYTE;
            }
            return DS_ERROR;
        case DS_CAS:
            if (c == 'e') {
                return DS_CASE;
            }
            return DS_ERROR;
        case DS_CAT:
            if (c == 'c') {
                return DS_CATC;
            }
            return DS_ERROR;
        case DS_CHA:
            if (c == 'r') {
                return DS_CHAR;
            }
            return DS_ERROR;
        case DS_CLA:
            if (c == 's') {
                return DS_CLAS;
            }
            return DS_ERROR;
        case DS_CON:
            if (c == 's') {
                return DS_CONS;
            }
            if (c == 't') {
                return DS_CONT;
            }
            return DS_ERROR;
        case DS_DEF:
            if (c == 'a') {
                return DS_DEFA;
            }
            return DS_ERROR;
        case DS_DOU:
            if (c == 'b') {
                return DS_DOUB;
            }
            return DS_ERROR;
        case DS_ELS:
            if (c == 'e') {
                return DS_ELSE;
            }
            return DS_ERROR;
        case DS_ENU:
            if (c == 'm') {
                return DS_ENUM;
            }
            return DS_ERROR;
        case DS_EXT:
            if (c == 'e') {
                return DS_EXTE;
            }
            return DS_ERROR;
        case DS_FIN:
            if (c == 'a') {
                return DS_FINA;
            }
            return DS_ERROR;
        case DS_FLO:
            if (c == 'a') {
                return DS_FLOA;
            }
            return DS_ERROR;
        case DS_FOR:
            return DS_ERROR;
        case DS_GOT:
            if (c == 'o') {
                return DS_GOTO;
            }
            return DS_ERROR;
        case DS_IMP:
            if (c == 'l') {
                return DS_IMPL;
            }
            if (c == 'o') {
                return DS_IMPO;
            }
            return DS_ERROR;
        case DS_INS:
            if (c == 't') {
                return DS_INST;
            }
            return DS_ERROR;
        case DS_INT:
            if (c == 'e') {
                return DS_INTE;
            }
            return DS_ERROR;
        case DS_LON:
            if (c == 'g') {
                return DS_LONG;
            }
            return DS_ERROR;
        case DS_NAT:
            if (c == 'i') {
                return DS_NATI;
            }
            return DS_ERROR;
        case DS_NEW:
            return DS_ERROR;
        case DS_PAC:
            if (c == 'k') {
                return DS_PACK;
            }
            return DS_ERROR;
        case DS_PRI:
            if (c == 'v') {
                return DS_PRIV;
            }
            return DS_ERROR;
        case DS_PRO:
            if (c == 't') {
                return DS_PROT;
            }
            return DS_ERROR;
        case DS_PUB:
            if (c == 'l') {
                return DS_PUBL;
            }
            return DS_ERROR;
        case DS_RET:
            if (c == 'u') {
                return DS_RETU;
            }
            return DS_ERROR;
        case DS_SHO:
            if (c == 'r') {
                return DS_SHOR;
            }
            return DS_ERROR;
        case DS_STA:
            if (c == 't') {
                return DS_STAT;
            }
            return DS_ERROR;
        case DS_STR:
            if (c == 'i') {
                return DS_STRI;
            }
            return DS_ERROR;
        case DS_SUP:
            if (c == 'e') {
                return DS_SUPE;
            }
            return DS_ERROR;
        case DS_SWI:
            if (c == 't') {
                return DS_SWIT;
            }
            return DS_ERROR;
        case DS_SYN:
            if (c == 'c') {
                return DS_SYNC;
            }
            return DS_ERROR;
        case DS_THI:
            if (c == 's') {
                return DS_THIS;
            }
            return DS_ERROR;
        case DS_THR:
            if (c == 'o') {
                return DS_THRO;
            }
            return DS_ERROR;
        case DS_TRA:
            if (c == 'n') {
                return DS_TRAN;
            }
            return DS_ERROR;
        case DS_TRY:
            return DS_ERROR;
        case DS_VOI:
            if (c == 'd') {
                return DS_VOID;
            }
            return DS_ERROR;
        case DS_VOL:
            if (c == 'a') {
                return DS_VOLA;
            }
            return DS_ERROR;
        case DS_WHI:
            if (c == 'l') {
                return DS_WHIL;
            }
            return DS_ERROR;
        case DS_ABST:
            if (c == 'r') {
                return DS_ABSTR;
            }
            return DS_ERROR;
        case DS_ASSE:
            if (c == 'r') {
                return DS_ASSER;
            }
            return DS_ERROR;
        case DS_BOOL:
            if (c == 'e') {
                return DS_BOOLE;
            }
            return DS_ERROR;
        case DS_BREA:
            if (c == 'k') {
                return DS_BREAK;
            }
            return DS_ERROR;
        case DS_BYTE:
            return DS_ERROR;
        case DS_CASE:
            return DS_ERROR;
        case DS_CATC:
            if (c == 'h') {
                return DS_CATCH;
            }
            return DS_ERROR;
        case DS_CHAR:
            return DS_ERROR;
        case DS_CLAS:
            if (c == 's') {
                return DS_CLASS;
            }
            return DS_ERROR;
        case DS_CONS:
            if (c == 't') {
                return DS_CONST;
            }
            return DS_ERROR;
        case DS_CONT:
            if (c == 'i') {
                return DS_CONTI;
            }
            return DS_ERROR;
        case DS_DEFA:
            if (c == 'u') {
                return DS_DEFAU;
            }
            return DS_ERROR;
        case DS_DOUB:
            if (c == 'l') {
                return DS_DOUBL;
            }
            return DS_ERROR;
        case DS_ELSE:
            return DS_ERROR;
        case DS_ENUM:
            return DS_ERROR;
        case DS_EXTE:
            if (c == 'n') {
                return DS_EXTEN;
            }
            return DS_ERROR;
        case DS_FINA:
            if (c == 'l') {
                return DS_FINAL;
            }
            return DS_ERROR;
        case DS_FLOA:
            if (c == 't') {
                return DS_FLOAT;
            }
            return DS_ERROR;
        case DS_GOTO:
            return DS_ERROR;
        case DS_IMPL:
            if (c == 'e') {
                return DS_IMPLE;
            }
            return DS_ERROR;
        case DS_IMPO:
            if (c == 'r') {
                return DS_IMPOR;
            }
            return DS_ERROR;
        case DS_INST:
            if (c == 'a') {
                return DS_INSTA;
            }
            return DS_ERROR;
        case DS_INTE:
            if (c == 'r') {
                return DS_INTER;
            }
            return DS_ERROR;
        case DS_LONG:
            return DS_ERROR;
        case DS_NATI:
            if (c == 'v') {
                return DS_NATIV;
            }
            return DS_ERROR;
        case DS_PACK:
            if (c == 'a') {
                return DS_PACKA;
            }
            return DS_ERROR;
        case DS_PRIV:
            if (c == 'a') {
                return DS_PRIVA;
            }
            return DS_ERROR;
        case DS_PROT:
            if (c == 'e') {
                return DS_PROTE;
            }
            return DS_ERROR;
        case DS_PUBL:
            if (c == 'i') {
                return DS_PUBLI;
            }
            return DS_ERROR;
        case DS_RETU:
            if (c == 'r') {
                return DS_RETUR;
            }
            return DS_ERROR;
        case DS_SHOR:
            if (c == 't') {
                return DS_SHORT;
            }
            return DS_ERROR;
        case DS_STAT:
            if (c == 'i') {
                return DS_STATI;
            }
            return DS_ERROR;
        case DS_STRI:
            if (c == 'c') {
                return DS_STRIC;
            }
            return DS_ERROR;
        case DS_SUPE:
            if (c == 'r') {
                return DS_SUPER;
            }
            return DS_ERROR;
        case DS_SWIT:
            if (c == 'c') {
                return DS_SWITC;
            }
            return DS_ERROR;
        case DS_SYNC:
            if (c == 'h') {
                return DS_SYNCH;
            }
            return DS_ERROR;
        case DS_THIS:
            return DS_ERROR;
        case DS_THRO:
            if (c == 'w') {
                return DS_THROW;
            }
            return DS_ERROR;
        case DS_TRAN:
            if (c == 's') {
                return DS_TRANS;
            }
            return DS_ERROR;
        case DS_VOID:
            return DS_ERROR;
        case DS_VOLA:
            if (c == 't') {
                return DS_VOLAT;
            }
            return DS_ERROR;
        case DS_WHIL:
            if (c == 'e') {
                return DS_WHILE;
            }
            return DS_ERROR;
        case DS_ABSTR:
            if (c == 'a') {
                return DS_ABSTRA;
            }
            return DS_ERROR;
        case DS_ASSER:
            if (c == 't') {
                return DS_ASSERT;
            }
            return DS_ERROR;
        case DS_BOOLE:
            if (c == 'a') {
                return DS_BOOLEA;
            }
            return DS_ERROR;
        case DS_BREAK:
            return DS_ERROR;
        case DS_CATCH:
            return DS_ERROR;
        case DS_CLASS:
            return DS_ERROR;
        case DS_CONST:
            return DS_ERROR;
        case DS_CONTI:
            if (c == 'n') {
                return DS_CONTIN;
            }
            return DS_ERROR;
        case DS_DEFAU:
            if (c == 'l') {
                return DS_DEFAUL;
            }
            return DS_ERROR;
        case DS_DOUBL:
            if (c == 'e') {
                return DS_DOUBLE;
            }
            return DS_ERROR;
        case DS_EXTEN:
            if (c == 'd') {
                return DS_EXTEND;
            }
            return DS_ERROR;
        case DS_FINAL:
            if (c == 'l') {
                return DS_FINALL;
            }
            return DS_ERROR;
        case DS_FLOAT:
            return DS_ERROR;
        case DS_IMPLE:
            if (c == 'm') {
                return DS_IMPLEM;
            }
            return DS_ERROR;
        case DS_IMPOR:
            if (c == 't') {
                return DS_IMPORT;
            }
            return DS_ERROR;
        case DS_INSTA:
            if (c == 'n') {
                return DS_INSTAN;
            }
            return DS_ERROR;
        case DS_INTER:
            if (c == 'f') {
                return DS_INTERF;
            }
            return DS_ERROR;
        case DS_NATIV:
            if (c == 'e') {
                return DS_NATIVE;
            }
            return DS_ERROR;
        case DS_PACKA:
            if (c == 'g') {
                return DS_PACKAG;
            }
            return DS_ERROR;
        case DS_PRIVA:
            if (c == 't') {
                return DS_PRIVAT;
            }
            return DS_ERROR;
        case DS_PROTE:
            if (c == 'c') {
                return DS_PROTEC;
            }
            return DS_ERROR;
        case DS_PUBLI:
            if (c == 'c') {
                return DS_PUBLIC;
            }
            return DS_ERROR;
        case DS_RETUR:
            if (c == 'n') {
                return DS_RETURN;
            }
            return DS_ERROR;
        case DS_SHORT:
            return DS_ERROR;
        case DS_STATI:
            if (c == 'c') {
                return DS_STATIC;
            }
            return DS_ERROR;
        case DS_STRIC:
            if (c == 't') {
                return DS_STRICT;
            }
            return DS_ERROR;
        case DS_SUPER:
            return DS_ERROR;
        case DS_SWITC:
            if (c == 'h') {
                return DS_SWITCH;
            }
            return DS_ERROR;
        case DS_SYNCH:
            if (c == 'r') {
                return DS_SYNCHR;
            }
            return DS_ERROR;
        case DS_THROW:
            if (c == 's') {
                return DS_THROWS;
            }
            return DS_ERROR;
        case DS_TRANS:
            if (c == 'i') {
                return DS_TRANSI;
            }
            return DS_ERROR;
        case DS_VOLAT:
            if (c == 'i') {
                return DS_VOLATI;
            }
            return DS_ERROR;
        case DS_WHILE:
            return DS_ERROR;
        case DS_ABSTRA:
            if (c == 'c') {
                return DS_ABSTRAC;
            }
            return DS_ERROR;
        case DS_ASSERT:
            return DS_ERROR;
        case DS_BOOLEA:
            if (c == 'n') {
                return DS_BOOLEAN;
            }
            return DS_ERROR;
        case DS_CONTIN:
            if (c == 'u') {
                return DS_CONTINU;
            }
            return DS_ERROR;
        case DS_DEFAUL:
            if (c == 't') {
                return DS_DEFAULT;
            }
            return DS_ERROR;
        case DS_DOUBLE:
            return DS_ERROR;
        case DS_EXTEND:
            if (c == 's') {
                return DS_EXTENDS;
            }
            return DS_ERROR;
        case DS_FINALL:
            if (c == 'y') {
                return DS_FINALLY;
            }
            return DS_ERROR;
        case DS_IMPLEM:
            if (c == 'e') {
                return DS_IMPLEME;
            }
            return DS_ERROR;
        case DS_IMPORT:
            return DS_ERROR;
        case DS_INSTAN:
            if (c == 'c') {
                return DS_INSTANC;
            }
            return DS_ERROR;
        case DS_INTERF:
            if (c == 'a') {
                return DS_INTERFA;
            }
            return DS_ERROR;
        case DS_NATIVE:
            return DS_ERROR;
        case DS_PACKAG:
            if (c == 'e') {
                return DS_PACKAGE;
            }
            return DS_ERROR;
        case DS_PRIVAT:
            if (c == 'e') {
                return DS_PRIVATE;
            }
            return DS_ERROR;
        case DS_PROTEC:
            if (c == 't') {
                return DS_PROTECT;
            }
            return DS_ERROR;
        case DS_PUBLIC:
            return DS_ERROR;
        case DS_RETURN:
            return DS_ERROR;
        case DS_STATIC:
            return DS_ERROR;
        case DS_STRICT:
            if (c == 'f') {
                return DS_STRICTF;
            }
            return DS_ERROR;
        case DS_SWITCH:
            return DS_ERROR;
        case DS_SYNCHR:
            if (c == 'o') {
                return DS_SYNCHRO;
            }
            return DS_ERROR;
        case DS_THROWS:
            return DS_ERROR;
        case DS_TRANSI:
            if (c == 'e') {
                return DS_TRANSIE;
            }
            return DS_ERROR;
        case DS_VOLATI:
            if (c == 'l') {
                return DS_VOLATIL;
            }
            return DS_ERROR;
        case DS_ABSTRAC:
            if (c == 't') {
                return DS_ABSTRACT;
            }
            return DS_ERROR;
        case DS_BOOLEAN:
            return DS_ERROR;
        case DS_CONTINU:
            if (c == 'e') {
                return DS_CONTINUE;
            }
            return DS_ERROR;
        case DS_DEFAULT:
            return DS_ERROR;
        case DS_EXTENDS:
            return DS_ERROR;
        case DS_FINALLY:
            return DS_ERROR;
        case DS_IMPLEME:
            if (c == 'n') {
                return DS_IMPLEMEN;
            }
            return DS_ERROR;
        case DS_INSTANC:
            if (c == 'e') {
                return DS_INSTANCE;
            }
            return DS_ERROR;
        case DS_INTERFA:
            if (c == 'c') {
                return DS_INTERFAC;
            }
            return DS_ERROR;
        case DS_PACKAGE:
            return DS_ERROR;
        case DS_PRIVATE:
            return DS_ERROR;
        case DS_PROTECT:
            if (c == 'e') {
                return DS_PROTECTE;
            }
            return DS_ERROR;
        case DS_STRICTF:
            if (c == 'p') {
                return DS_STRICTFP;
            }
            return DS_ERROR;
        case DS_SYNCHRO:
            if (c == 'n') {
                return DS_SYNCHRON;
            }
            return DS_ERROR;
        case DS_TRANSIE:
            if (c == 'n') {
                return DS_TRANSIEN;
            }
            return DS_ERROR;
        case DS_VOLATIL:
            if (c == 'e') {
                return DS_VOLATILE;
            }
            return DS_ERROR;
        case DS_ABSTRACT:
            return DS_ERROR;
        case DS_CONTINUE:
            return DS_ERROR;
        case DS_IMPLEMEN:
            if (c == 't') {
                return DS_IMPLEMENT;
            }
            return DS_ERROR;
        case DS_INSTANCE:
            if (c == 'o') {
                return DS_INSTANCEO;
            }
            return DS_ERROR;
        case DS_INTERFAC:
            if (c == 'e') {
                return DS_INTERFACE;
            }
            return DS_ERROR;
        case DS_PROTECTE:
            if (c == 'd') {
                return DS_PROTECTED;
            }
            return DS_ERROR;
        case DS_STRICTFP:
            return DS_ERROR;
        case DS_SYNCHRON:
            if (c == 'i') {
                return DS_SYNCHRONI;
            }
            return DS_ERROR;
        case DS_TRANSIEN:
            if (c == 't') {
                return DS_TRANSIENT;
            }
            return DS_ERROR;
        case DS_VOLATILE:
            return DS_ERROR;
        case DS_IMPLEMENT:
            if (c == 's') {
                return DS_IMPLEMENTS;
            }
            return DS_ERROR;
        case DS_INSTANCEO:
            if (c == 'f') {
                return DS_INSTANCEOF;
            }
            return DS_ERROR;
        case DS_INTERFACE:
            return DS_ERROR;
        case DS_PROTECTED:
            return DS_ERROR;
        case DS_SYNCHRONI:
            if (c == 'z') {
                return DS_SYNCHRONIZ;
            }
            return DS_ERROR;
        case DS_TRANSIENT:
            return DS_ERROR;
        case DS_IMPLEMENTS:
            return DS_ERROR;
        case DS_INSTANCEOF:
            return DS_ERROR;
        case DS_SYNCHRONIZ:
            if (c == 'e') {
                return DS_SYNCHRONIZE;
            }
            return DS_ERROR;
        case DS_SYNCHRONIZE:
            if (c == 'd') {
                return DS_SYNCHRONIZED;
            }
            return DS_ERROR;
        case DS_SYNCHRONIZED:
            return DS_ERROR;
        default:
            return DS_ERROR;
    }
}

void KeywordDfa::initDfa() { 
    dfa[DS_ERROR] = std::make_pair(DS_ERROR, &error);
    dfa[DS_START] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_A] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_B] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_C] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_D] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_E] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_F] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_I] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_G] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_L] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_N] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_P] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_R] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_S] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_T] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_V] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_W] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_AB] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_AS] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_BO] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_BR] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_BY] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_CA] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_CH] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_CL] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_CO] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_DE] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_DO] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_EL] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_EN] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_EX] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_FI] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_FL] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_FO] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_IF] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_GO] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_IM] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_IN] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_LO] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_NA] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_NE] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_PA] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_PR] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_PU] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_RE] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_SH] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_ST] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_SU] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_SW] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_SY] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_TH] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_TR] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_VO] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_WH] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_ABS] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_ASS] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_BOO] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_BRE] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_BYT] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_CAS] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_CAT] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_CHA] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_CLA] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_CON] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_DEF] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_DOU] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_ELS] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_ENU] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_EXT] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_FIN] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_FLO] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_FOR] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_GOT] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_IMP] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_INS] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_INT] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_LON] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_NAT] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_NEW] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_PAC] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_PRI] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_PRO] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_PUB] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_RET] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_SHO] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_STA] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_STR] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_SUP] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_SWI] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_SYN] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_THI] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_THR] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_TRA] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_TRY] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_VOI] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_VOL] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_WHI] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_ABST] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_ASSE] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_BOOL] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_BREA] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_BYTE] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_CASE] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_CATC] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_CHAR] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_CLAS] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_CONS] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_CONT] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_DEFA] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_DOUB] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_ELSE] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_ENUM] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_EXTE] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_FINA] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_FLOA] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_GOTO] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_IMPL] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_IMPO] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_INST] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_INTE] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_LONG] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_NATI] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_PACK] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_PRIV] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_PROT] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_PUBL] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_RETU] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_SHOR] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_STAT] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_STRI] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_SUPE] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_SWIT] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_SYNC] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_THIS] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_THRO] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_TRAN] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_VOID] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_VOLA] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_WHIL] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_ABSTR] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_ASSER] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_BOOLE] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_BREAK] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_CATCH] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_CLASS] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_CONST] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_CONTI] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_DEFAU] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_DOUBL] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_EXTEN] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_FINAL] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_FLOAT] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_IMPLE] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_IMPOR] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_INSTA] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_INTER] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_NATIV] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_PACKA] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_PRIVA] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_PROTE] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_PUBLI] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_RETUR] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_SHORT] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_STATI] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_STRIC] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_SUPER] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_SWITC] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_SYNCH] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_THROW] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_TRANS] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_VOLAT] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_WHILE] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_ABSTRA] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_ASSERT] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_BOOLEA] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_CONTIN] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_DEFAUL] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_DOUBLE] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_EXTEND] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_FINALL] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_IMPLEM] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_IMPORT] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_INSTAN] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_INTERF] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_NATIVE] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_PACKAG] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_PRIVAT] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_PROTEC] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_PUBLIC] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_RETURN] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_STATIC] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_STRICT] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_SWITCH] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_SYNCHR] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_THROWS] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_TRANSI] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_VOLATI] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_ABSTRAC] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_BOOLEAN] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_CONTINU] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_DEFAULT] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_EXTENDS] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_FINALLY] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_IMPLEME] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_INSTANC] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_INTERFA] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_PACKAGE] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_PRIVATE] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_PROTECT] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_STRICTF] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_SYNCHRO] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_TRANSIE] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_VOLATIL] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_ABSTRACT] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_CONTINUE] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_IMPLEMEN] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_INSTANCE] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_INTERFAC] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_PROTECTE] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_STRICTFP] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_SYNCHRON] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_TRANSIEN] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_VOLATILE] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_IMPLEMENT] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_INSTANCEO] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_INTERFACE] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_PROTECTED] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_SYNCHRONI] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_TRANSIENT] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_IMPLEMENTS] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_INSTANCEOF] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_SYNCHRONIZ] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_SYNCHRONIZE] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_SYNCHRONIZED] = std::make_pair(DS_ACCEPT, &pyGen);
}

KeywordDfa::KeywordDfa() : Dfa() {
    initDfa();
}
