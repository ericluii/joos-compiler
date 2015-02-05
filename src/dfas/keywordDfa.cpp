#include "states.h"
#include "keywordDfa.h"

int pyGen(char c, int current_state)
{
    switch (current_state) {
        case DS_START:
            if (c == 'a') { return DS_A; }
            if (c == 'b') { return DS_B; }
            if (c == 'c') { return DS_C; }
            if (c == 'e') { return DS_E; }
            if (c == 'f') { return DS_F; }
            if (c == 'i') { return DS_I; }
            if (c == 'n') { return DS_N; }
            if (c == 'p') { return DS_P; }
            if (c == 'r') { return DS_R; }
            if (c == 's') { return DS_S; }
            if (c == 't') { return DS_T; }
            if (c == 'v') { return DS_V; }
            if (c == 'w') { return DS_W; }
            return DS_ERROR;
        case DS_A:
            if (c == 'b') { return DS_AB; }
            if (c == 's') { return DS_AS; }
            return DS_ERROR;
        case DS_B:
            if (c == 'o') { return DS_BO; }
            if (c == 'r') { return DS_BR; }
            if (c == 'y') { return DS_BY; }
            return DS_ERROR;
        case DS_C:
            if (c == 'h') { return DS_CH; }
            if (c == 'l') { return DS_CL; }
            return DS_ERROR;
        case DS_E:
            if (c == 'l') { return DS_EL; }
            if (c == 'n') { return DS_EN; }
            if (c == 'x') { return DS_EX; }
            return DS_ERROR;
        case DS_F:
            if (c == 'i') { return DS_FI; }
            if (c == 'o') { return DS_FO; }
            return DS_ERROR;
        case DS_I:
            if (c == 'f') { return DS_IF; }
            if (c == 'm') { return DS_IM; }
            if (c == 'n') { return DS_IN; }
            return DS_ERROR;
        case DS_N:
            if (c == 'a') { return DS_NA; }
            if (c == 'e') { return DS_NE; }
            if (c == 'u') { return DS_NU; }
            return DS_ERROR;
        case DS_P:
            if (c == 'a') { return DS_PA; }
            if (c == 'r') { return DS_PR; }
            if (c == 'u') { return DS_PU; }
            return DS_ERROR;
        case DS_R:
            if (c == 'e') { return DS_RE; }
            return DS_ERROR;
        case DS_S:
            if (c == 'h') { return DS_SH; }
            if (c == 't') { return DS_ST; }
            return DS_ERROR;
        case DS_T:
            if (c == 'h') { return DS_TH; }
            return DS_ERROR;
        case DS_V:
            if (c == 'o') { return DS_VO; }
            return DS_ERROR;
        case DS_W:
            if (c == 'h') { return DS_WH; }
            return DS_ERROR;
        case DS_AB:
            if (c == 's') { return DS_ABS; }
            return DS_ERROR;
        case DS_AS:
            if (c == 's') { return DS_ASS; }
            return DS_ERROR;
        case DS_BO:
            if (c == 'o') { return DS_BOO; }
            return DS_ERROR;
        case DS_BR:
            if (c == 'e') { return DS_BRE; }
            return DS_ERROR;
        case DS_BY:
            if (c == 't') { return DS_BYT; }
            return DS_ERROR;
        case DS_CH:
            if (c == 'a') { return DS_CHA; }
            return DS_ERROR;
        case DS_CL:
            if (c == 'a') { return DS_CLA; }
            return DS_ERROR;
        case DS_EL:
            if (c == 's') { return DS_ELS; }
            return DS_ERROR;
        case DS_EN:
            if (c == 'u') { return DS_ENU; }
            return DS_ERROR;
        case DS_EX:
            if (c == 't') { return DS_EXT; }
            return DS_ERROR;
        case DS_FI:
            if (c == 'n') { return DS_FIN; }
            return DS_ERROR;
        case DS_FO:
            if (c == 'r') { return DS_FOR; }
            return DS_ERROR;
        case DS_IF:
            return DS_ERROR;
        case DS_IM:
            if (c == 'p') { return DS_IMP; }
            return DS_ERROR;
        case DS_IN:
            if (c == 's') { return DS_INS; }
            if (c == 't') { return DS_INT; }
            return DS_ERROR;
        case DS_NA:
            if (c == 't') { return DS_NAT; }
            return DS_ERROR;
        case DS_NE:
            if (c == 'w') { return DS_NEW; }
            return DS_ERROR;
        case DS_PA:
            if (c == 'c') { return DS_PAC; }
            return DS_ERROR;
        case DS_PR:
            if (c == 'o') { return DS_PRO; }
            return DS_ERROR;
        case DS_PU:
            if (c == 'b') { return DS_PUB; }
            return DS_ERROR;
        case DS_RE:
            if (c == 't') { return DS_RET; }
            return DS_ERROR;
        case DS_SH:
            if (c == 'o') { return DS_SHO; }
            return DS_ERROR;
        case DS_ST:
            if (c == 'a') { return DS_STA; }
            return DS_ERROR;
        case DS_TH:
            if (c == 'i') { return DS_THI; }
            return DS_ERROR;
        case DS_VO:
            if (c == 'i') { return DS_VOI; }
            return DS_ERROR;
        case DS_WH:
            if (c == 'i') { return DS_WHI; }
            return DS_ERROR;
        case DS_NU:
            if (c == 'l') { return DS_NUL; }
            return DS_ERROR;
        case DS_ABS:
            if (c == 't') { return DS_ABST; }
            return DS_ERROR;
        case DS_ASS:
            if (c == 'e') { return DS_ASSE; }
            return DS_ERROR;
        case DS_BOO:
            if (c == 'l') { return DS_BOOL; }
            return DS_ERROR;
        case DS_BRE:
            if (c == 'a') { return DS_BREA; }
            return DS_ERROR;
        case DS_BYT:
            if (c == 'e') { return DS_BYTE; }
            return DS_ERROR;
        case DS_CHA:
            if (c == 'r') { return DS_CHAR; }
            return DS_ERROR;
        case DS_CLA:
            if (c == 's') { return DS_CLAS; }
            return DS_ERROR;
        case DS_ELS:
            if (c == 'e') { return DS_ELSE; }
            return DS_ERROR;
        case DS_ENU:
            if (c == 'm') { return DS_ENUM; }
            return DS_ERROR;
        case DS_EXT:
            if (c == 'e') { return DS_EXTE; }
            return DS_ERROR;
        case DS_FIN:
            if (c == 'a') { return DS_FINA; }
            return DS_ERROR;
        case DS_FOR:
            return DS_ERROR;
        case DS_IMP:
            if (c == 'l') { return DS_IMPL; }
            if (c == 'o') { return DS_IMPO; }
            return DS_ERROR;
        case DS_INS:
            if (c == 't') { return DS_INST; }
            return DS_ERROR;
        case DS_INT:
            if (c == 'e') { return DS_INTE; }
            return DS_ERROR;
        case DS_NAT:
            if (c == 'i') { return DS_NATI; }
            return DS_ERROR;
        case DS_NEW:
            return DS_ERROR;
        case DS_PAC:
            if (c == 'k') { return DS_PACK; }
            return DS_ERROR;
        case DS_PRO:
            if (c == 't') { return DS_PROT; }
            return DS_ERROR;
        case DS_PUB:
            if (c == 'l') { return DS_PUBL; }
            return DS_ERROR;
        case DS_RET:
            if (c == 'u') { return DS_RETU; }
            return DS_ERROR;
        case DS_SHO:
            if (c == 'r') { return DS_SHOR; }
            return DS_ERROR;
        case DS_STA:
            if (c == 't') { return DS_STAT; }
            return DS_ERROR;
        case DS_THI:
            if (c == 's') { return DS_THIS; }
            return DS_ERROR;
        case DS_VOI:
            if (c == 'd') { return DS_VOID; }
            return DS_ERROR;
        case DS_WHI:
            if (c == 'l') { return DS_WHIL; }
            return DS_ERROR;
        case DS_NUL:
            if (c == 'l') { return DS_NULL; }
            return DS_ERROR;
        case DS_ABST:
            if (c == 'r') { return DS_ABSTR; }
            return DS_ERROR;
        case DS_ASSE:
            if (c == 'r') { return DS_ASSER; }
            return DS_ERROR;
        case DS_BOOL:
            if (c == 'e') { return DS_BOOLE; }
            return DS_ERROR;
        case DS_BREA:
            if (c == 'k') { return DS_BREAK; }
            return DS_ERROR;
        case DS_BYTE:
            return DS_ERROR;
        case DS_CHAR:
            return DS_ERROR;
        case DS_CLAS:
            if (c == 's') { return DS_CLASS; }
            return DS_ERROR;
        case DS_ELSE:
            return DS_ERROR;
        case DS_ENUM:
            return DS_ERROR;
        case DS_EXTE:
            if (c == 'n') { return DS_EXTEN; }
            return DS_ERROR;
        case DS_FINA:
            if (c == 'l') { return DS_FINAL; }
            return DS_ERROR;
        case DS_IMPL:
            if (c == 'e') { return DS_IMPLE; }
            return DS_ERROR;
        case DS_IMPO:
            if (c == 'r') { return DS_IMPOR; }
            return DS_ERROR;
        case DS_INST:
            if (c == 'a') { return DS_INSTA; }
            return DS_ERROR;
        case DS_INTE:
            if (c == 'r') { return DS_INTER; }
            return DS_ERROR;
        case DS_NATI:
            if (c == 'v') { return DS_NATIV; }
            return DS_ERROR;
        case DS_PACK:
            if (c == 'a') { return DS_PACKA; }
            return DS_ERROR;
        case DS_PROT:
            if (c == 'e') { return DS_PROTE; }
            return DS_ERROR;
        case DS_PUBL:
            if (c == 'i') { return DS_PUBLI; }
            return DS_ERROR;
        case DS_RETU:
            if (c == 'r') { return DS_RETUR; }
            return DS_ERROR;
        case DS_SHOR:
            if (c == 't') { return DS_SHORT; }
            return DS_ERROR;
        case DS_STAT:
            if (c == 'i') { return DS_STATI; }
            return DS_ERROR;
        case DS_THIS:
            return DS_ERROR;
        case DS_VOID:
            return DS_ERROR;
        case DS_WHIL:
            if (c == 'e') { return DS_WHILE; }
            return DS_ERROR;
        case DS_NULL:
            return DS_ERROR;
        case DS_ABSTR:
            if (c == 'a') { return DS_ABSTRA; }
            return DS_ERROR;
        case DS_ASSER:
            if (c == 't') { return DS_ASSERT; }
            return DS_ERROR;
        case DS_BOOLE:
            if (c == 'a') { return DS_BOOLEA; }
            return DS_ERROR;
        case DS_BREAK:
            return DS_ERROR;
        case DS_CLASS:
            return DS_ERROR;
        case DS_EXTEN:
            if (c == 'd') { return DS_EXTEND; }
            return DS_ERROR;
        case DS_FINAL:
            return DS_ERROR;
        case DS_IMPLE:
            if (c == 'm') { return DS_IMPLEM; }
            return DS_ERROR;
        case DS_IMPOR:
            if (c == 't') { return DS_IMPORT; }
            return DS_ERROR;
        case DS_INSTA:
            if (c == 'n') { return DS_INSTAN; }
            return DS_ERROR;
        case DS_INTER:
            if (c == 'f') { return DS_INTERF; }
            return DS_ERROR;
        case DS_NATIV:
            if (c == 'e') { return DS_NATIVE; }
            return DS_ERROR;
        case DS_PACKA:
            if (c == 'g') { return DS_PACKAG; }
            return DS_ERROR;
        case DS_PROTE:
            if (c == 'c') { return DS_PROTEC; }
            return DS_ERROR;
        case DS_PUBLI:
            if (c == 'c') { return DS_PUBLIC; }
            return DS_ERROR;
        case DS_RETUR:
            if (c == 'n') { return DS_RETURN; }
            return DS_ERROR;
        case DS_SHORT:
            return DS_ERROR;
        case DS_STATI:
            if (c == 'c') { return DS_STATIC; }
            return DS_ERROR;
        case DS_WHILE:
            return DS_ERROR;
        case DS_ABSTRA:
            if (c == 'c') { return DS_ABSTRAC; }
            return DS_ERROR;
        case DS_ASSERT:
            return DS_ERROR;
        case DS_BOOLEA:
            if (c == 'n') { return DS_BOOLEAN; }
            return DS_ERROR;
        case DS_EXTEND:
            if (c == 's') { return DS_EXTENDS; }
            return DS_ERROR;
        case DS_IMPLEM:
            if (c == 'e') { return DS_IMPLEME; }
            return DS_ERROR;
        case DS_IMPORT:
            return DS_ERROR;
        case DS_INSTAN:
            if (c == 'c') { return DS_INSTANC; }
            return DS_ERROR;
        case DS_INTERF:
            if (c == 'a') { return DS_INTERFA; }
            return DS_ERROR;
        case DS_NATIVE:
            return DS_ERROR;
        case DS_PACKAG:
            if (c == 'e') { return DS_PACKAGE; }
            return DS_ERROR;
        case DS_PROTEC:
            if (c == 't') { return DS_PROTECT; }
            return DS_ERROR;
        case DS_PUBLIC:
            return DS_ERROR;
        case DS_RETURN:
            return DS_ERROR;
        case DS_STATIC:
            return DS_ERROR;
        case DS_ABSTRAC:
            if (c == 't') { return DS_ABSTRACT; }
            return DS_ERROR;
        case DS_BOOLEAN:
            return DS_ERROR;
        case DS_EXTENDS:
            return DS_ERROR;
        case DS_IMPLEME:
            if (c == 'n') { return DS_IMPLEMEN; }
            return DS_ERROR;
        case DS_INSTANC:
            if (c == 'e') { return DS_INSTANCE; }
            return DS_ERROR;
        case DS_INTERFA:
            if (c == 'c') { return DS_INTERFAC; }
            return DS_ERROR;
        case DS_PACKAGE:
            return DS_ERROR;
        case DS_PROTECT:
            if (c == 'e') { return DS_PROTECTE; }
            return DS_ERROR;
        case DS_ABSTRACT:
            return DS_ERROR;
        case DS_IMPLEMEN:
            if (c == 't') { return DS_IMPLEMENT; }
            return DS_ERROR;
        case DS_INSTANCE:
            if (c == 'o') { return DS_INSTANCEO; }
            return DS_ERROR;
        case DS_INTERFAC:
            if (c == 'e') { return DS_INTERFACE; }
            return DS_ERROR;
        case DS_PROTECTE:
            if (c == 'd') { return DS_PROTECTED; }
            return DS_ERROR;
        case DS_IMPLEMENT:
            if (c == 's') { return DS_IMPLEMENTS; }
            return DS_ERROR;
        case DS_INSTANCEO:
            if (c == 'f') { return DS_INSTANCEOF; }
            return DS_ERROR;
        case DS_INTERFACE:
            return DS_ERROR;
        case DS_PROTECTED:
            return DS_ERROR;
        case DS_IMPLEMENTS:
            return DS_ERROR;
        case DS_INSTANCEOF:
            return DS_ERROR;
        default:
            return DS_ERROR;
    }
}

TOKEN_TYPE KeywordDfa::getTokenType()
{
    if (dfa[current_state].first != DS_ACCEPT) {
        return TT_INVALID;
    }

    if (current_state == DS_IF) { return TT_IF; }
    if (current_state == DS_FOR) { return TT_FOR; }
    if (current_state == DS_INT) { return TT_INT; }
    if (current_state == DS_NEW) { return TT_NEW; }
    if (current_state == DS_BYTE) { return TT_BYTE; }
    if (current_state == DS_CHAR) { return TT_CHAR; }
    if (current_state == DS_ELSE) { return TT_ELSE; }
    if (current_state == DS_ENUM) { return TT_ENUM; }
    if (current_state == DS_THIS) { return TT_THIS; }
    if (current_state == DS_VOID) { return TT_VOID; }
    if (current_state == DS_NULL) { return TT_NULL; }
    if (current_state == DS_BREAK) { return TT_BREAK; }
    if (current_state == DS_CLASS) { return TT_CLASS; }
    if (current_state == DS_FINAL) { return TT_FINAL; }
    if (current_state == DS_SHORT) { return TT_SHORT; }
    if (current_state == DS_WHILE) { return TT_WHILE; }
    if (current_state == DS_ASSERT) { return TT_ASSERT; }
    if (current_state == DS_IMPORT) { return TT_IMPORT; }
    if (current_state == DS_NATIVE) { return TT_NATIVE; }
    if (current_state == DS_PUBLIC) { return TT_PUBLIC; }
    if (current_state == DS_RETURN) { return TT_RETURN; }
    if (current_state == DS_STATIC) { return TT_STATIC; }
    if (current_state == DS_BOOLEAN) { return TT_BOOLEAN; }
    if (current_state == DS_EXTENDS) { return TT_EXTENDS; }
    if (current_state == DS_PACKAGE) { return TT_PACKAGE; }
    if (current_state == DS_ABSTRACT) { return TT_ABSTRACT; }
    if (current_state == DS_INTERFACE) { return TT_INTERFACE; }
    if (current_state == DS_PROTECTED) { return TT_PROTECTED; }
    if (current_state == DS_IMPLEMENTS) { return TT_IMPLEMENTS; }
    if (current_state == DS_INSTANCEOF) { return TT_INSTANCEOF; }
    return TT_INVALID;
}

void KeywordDfa::initDfa()
{
    dfa[DS_ERROR] = std::make_pair(DS_ERROR, &dfa_error);
    dfa[DS_START] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_A] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_B] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_C] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_E] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_F] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_I] = std::make_pair(DS_RUNNING, &pyGen);
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
    dfa[DS_CH] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_CL] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_EL] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_EN] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_EX] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_FI] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_FO] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_IF] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_IM] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_IN] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_NA] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_NE] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_PA] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_PR] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_PU] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_RE] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_SH] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_ST] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_TH] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_VO] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_WH] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_NU] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_ABS] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_ASS] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_BOO] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_BRE] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_BYT] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_CHA] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_CLA] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_ELS] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_ENU] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_EXT] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_FIN] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_FOR] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_IMP] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_INS] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_INT] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_NAT] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_NEW] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_PAC] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_PRO] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_PUB] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_RET] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_SHO] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_STA] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_THI] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_VOI] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_WHI] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_NUL] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_ABST] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_ASSE] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_BOOL] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_BREA] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_BYTE] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_CHAR] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_CLAS] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_ELSE] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_ENUM] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_EXTE] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_FINA] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_IMPL] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_IMPO] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_INST] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_INTE] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_NATI] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_PACK] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_PROT] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_PUBL] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_RETU] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_SHOR] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_STAT] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_THIS] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_VOID] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_WHIL] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_NULL] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_ABSTR] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_ASSER] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_BOOLE] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_BREAK] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_CLASS] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_EXTEN] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_FINAL] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_IMPLE] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_IMPOR] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_INSTA] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_INTER] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_NATIV] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_PACKA] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_PROTE] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_PUBLI] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_RETUR] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_SHORT] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_STATI] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_WHILE] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_ABSTRA] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_ASSERT] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_BOOLEA] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_EXTEND] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_IMPLEM] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_IMPORT] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_INSTAN] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_INTERF] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_NATIVE] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_PACKAG] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_PROTEC] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_PUBLIC] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_RETURN] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_STATIC] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_ABSTRAC] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_BOOLEAN] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_EXTENDS] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_IMPLEME] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_INSTANC] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_INTERFA] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_PACKAGE] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_PROTECT] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_ABSTRACT] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_IMPLEMEN] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_INSTANCE] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_INTERFAC] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_PROTECTE] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_IMPLEMENT] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_INSTANCEO] = std::make_pair(DS_RUNNING, &pyGen);
    dfa[DS_INTERFACE] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_PROTECTED] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_IMPLEMENTS] = std::make_pair(DS_ACCEPT, &pyGen);
    dfa[DS_INSTANCEOF] = std::make_pair(DS_ACCEPT, &pyGen);
}

KeywordDfa::KeywordDfa() : Dfa()
{
    initDfa();
}
