#include "states.h"
#include "bannedKeywordDfa.h"

int pyGen2(char c, int current_state)
{
    switch (current_state) {
        case DS_START:
            if (c == 'c') { return DS_C; }
            if (c == 'd') { return DS_D; }
            if (c == 'f') { return DS_F; }
            if (c == 'g') { return DS_G; }
            if (c == 'l') { return DS_L; }
            if (c == 'p') { return DS_P; }
            if (c == 's') { return DS_S; }
            if (c == 't') { return DS_T; }
            if (c == 'v') { return DS_V; }
            return DS_ERROR;
        case DS_C:
            if (c == 'a') { return DS_CA; }
            if (c == 'o') { return DS_CO; }
            return DS_ERROR;
        case DS_G:
            if (c == 'o') { return DS_GO; }
            return DS_ERROR;
        case DS_D:
            if (c == 'e') { return DS_DE; }
            if (c == 'o') { return DS_DO; }
            return DS_ERROR;
        case DS_F:
            if (c == 'i') { return DS_FI; }
            if (c == 'l') { return DS_FL; }
            return DS_ERROR;
        case DS_L:
            if (c == 'o') { return DS_LO; }
            return DS_ERROR;
        case DS_S:
            if (c == 't') { return DS_ST; }
            if (c == 'u') { return DS_SU; }
            if (c == 'w') { return DS_SW; }
            if (c == 'y') { return DS_SY; }
            return DS_ERROR;
        case DS_P:
            if (c == 'r') { return DS_PR; }
            return DS_ERROR;
        case DS_T:
            if (c == 'h') { return DS_TH; }
            if (c == 'r') { return DS_TR; }
            return DS_ERROR;
        case DS_V:
            if (c == 'o') { return DS_VO; }
            return DS_ERROR;
        case DS_CA:
            if (c == 's') { return DS_CAS; }
            if (c == 't') { return DS_CAT; }
            return DS_ERROR;
        case DS_CO:
            if (c == 'n') { return DS_CON; }
            return DS_ERROR;
        case DS_GO:
            if (c == 't') { return DS_GOT; }
            return DS_ERROR;
        case DS_DE:
            if (c == 'f') { return DS_DEF; }
            return DS_ERROR;
        case DS_DO:
            if (c == 'u') { return DS_DOU; }
            return DS_ERROR;
        case DS_FI:
            if (c == 'n') { return DS_FIN; }
            return DS_ERROR;
        case DS_FL:
            if (c == 'o') { return DS_FLO; }
            return DS_ERROR;
        case DS_LO:
            if (c == 'n') { return DS_LON; }
            return DS_ERROR;
        case DS_SW:
            if (c == 'i') { return DS_SWI; }
            return DS_ERROR;
        case DS_SY:
            if (c == 'n') { return DS_SYN; }
            return DS_ERROR;
        case DS_PR:
            if (c == 'i') { return DS_PRI; }
            return DS_ERROR;
        case DS_TR:
            if (c == 'a') { return DS_TRA; }
            if (c == 'y') { return DS_TRY; }
            return DS_ERROR;
        case DS_SU:
            if (c == 'p') { return DS_SUP; }
            return DS_ERROR;
        case DS_VO:
            if (c == 'l') { return DS_VOL; }
            return DS_ERROR;
        case DS_TH:
            if (c == 'r') { return DS_THR; }
            return DS_ERROR;
        case DS_ST:
            if (c == 'r') { return DS_STR; }
            return DS_ERROR;
        case DS_CAT:
            if (c == 'c') { return DS_CATC; }
            return DS_ERROR;
        case DS_CAS:
            if (c == 'e') { return DS_CASE; }
            return DS_ERROR;
        case DS_CON:
            if (c == 's') { return DS_CONS; }
            if (c == 't') { return DS_CONT; }
            return DS_ERROR;
        case DS_GOT:
            if (c == 'o') { return DS_GOTO; }
            return DS_ERROR;
        case DS_DEF:
            if (c == 'a') { return DS_DEFA; }
            return DS_ERROR;
        case DS_DOU:
            if (c == 'b') { return DS_DOUB; }
            return DS_ERROR;
        case DS_FIN:
            if (c == 'a') { return DS_FINA; }
            return DS_ERROR;
        case DS_FLO:
            if (c == 'a') { return DS_FLOA; }
            return DS_ERROR;
        case DS_LON:
            if (c == 'g') { return DS_LONG; }
            return DS_ERROR;
        case DS_SWI:
            if (c == 't') { return DS_SWIT; }
            return DS_ERROR;
        case DS_SYN:
            if (c == 'c') { return DS_SYNC; }
            return DS_ERROR;
        case DS_PRI:
            if (c == 'v') { return DS_PRIV; }
            return DS_ERROR;
        case DS_TRA:
            if (c == 'n') { return DS_TRAN; }
            return DS_ERROR;
        case DS_SUP:
            if (c == 'e') { return DS_SUPE; }
            return DS_ERROR;
        case DS_VOL:
            if (c == 'a') { return DS_VOLA; }
            return DS_ERROR;
        case DS_TRY:
            return DS_ERROR;
        case DS_THR:
            if (c == 'o') { return DS_THRO; }
            return DS_ERROR;
        case DS_STR:
            if (c == 'i') { return DS_STRI; }
            return DS_ERROR;
        case DS_CATC:
            if (c == 'h') { return DS_CATCH; }
            return DS_ERROR;
        case DS_CASE:
            return DS_ERROR;
        case DS_CONS:
            if (c == 't') { return DS_CONST; }
            return DS_ERROR;
        case DS_GOTO:
            return DS_ERROR;
        case DS_CONT:
            if (c == 'i') { return DS_CONTI; }
            return DS_ERROR;
        case DS_DEFA:
            if (c == 'u') { return DS_DEFAU; }
            return DS_ERROR;
        case DS_DOUB:
            if (c == 'l') { return DS_DOUBL; }
            return DS_ERROR;
        case DS_FINA:
            if (c == 'l') { return DS_FINAL; }
            return DS_ERROR;
        case DS_FLOA:
            if (c == 't') { return DS_FLOAT; }
            return DS_ERROR;
        case DS_LONG:
            return DS_ERROR;
        case DS_SWIT:
            if (c == 'c') { return DS_SWITC; }
            return DS_ERROR;
        case DS_SYNC:
            if (c == 'h') { return DS_SYNCH; }
            return DS_ERROR;
        case DS_PRIV:
            if (c == 'a') { return DS_PRIVA; }
            return DS_ERROR;
        case DS_TRAN:
            if (c == 's') { return DS_TRANS; }
            return DS_ERROR;
        case DS_SUPE:
            if (c == 'r') { return DS_SUPER; }
            return DS_ERROR;
        case DS_VOLA:
            if (c == 't') { return DS_VOLAT; }
            return DS_ERROR;
        case DS_THRO:
            if (c == 'w') { return DS_THROW; }
            return DS_ERROR;
        case DS_STRI:
            if (c == 'c') { return DS_STRIC; }
            return DS_ERROR;
        case DS_CATCH:
            return DS_ERROR;
        case DS_CONST:
            return DS_ERROR;
        case DS_CONTI:
            if (c == 'n') { return DS_CONTIN; }
            return DS_ERROR;
        case DS_DEFAU:
            if (c == 'l') { return DS_DEFAUL; }
            return DS_ERROR;
        case DS_DOUBL:
            if (c == 'e') { return DS_DOUBLE; }
            return DS_ERROR;
        case DS_FINAL:
            if (c == 'l') { return DS_FINALL; }
            return DS_ERROR;
        case DS_FLOAT:
            return DS_ERROR;
        case DS_SWITC:
            if (c == 'h') { return DS_SWITCH; }
            return DS_ERROR;
        case DS_SYNCH:
            if (c == 'r') { return DS_SYNCHR; }
            return DS_ERROR;
        case DS_PRIVA:
            if (c == 't') { return DS_PRIVAT; }
            return DS_ERROR;
        case DS_TRANS:
            if (c == 'i') { return DS_TRANSI; }
            return DS_ERROR;
        case DS_SUPER:
            return DS_ERROR;
        case DS_VOLAT:
            if (c == 'i') { return DS_VOLATI; }
            return DS_ERROR;
        case DS_THROW:
            if (c == 's') { return DS_THROWS; }
            return DS_ERROR;
        case DS_STRIC:
            if (c == 't') { return DS_STRICT; }
            return DS_ERROR;
        case DS_CONTIN:
            if (c == 'u') { return DS_CONTINU; }
            return DS_ERROR;
        case DS_DEFAUL:
            if (c == 't') { return DS_DEFAULT; }
            return DS_ERROR;
        case DS_DOUBLE:
            return DS_ERROR;
        case DS_FINALL:
            if (c == 'y') { return DS_FINALLY; }
            return DS_ERROR;
        case DS_SWITCH:
            return DS_ERROR;
        case DS_SYNCHR:
            if (c == 'o') { return DS_SYNCHRO; }
            return DS_ERROR;
        case DS_PRIVAT:
            if (c == 'e') { return DS_PRIVATE; }
            return DS_ERROR;
        case DS_TRANSI:
            if (c == 'e') { return DS_TRANSIE; }
            return DS_ERROR;
        case DS_VOLATI:
            if (c == 'l') { return DS_VOLATIL; }
            return DS_ERROR;
        case DS_STRICT:
            if (c == 'f') { return DS_STRICTF; }
            return DS_ERROR;
        case DS_THROWS:
            return DS_ERROR;
        case DS_CONTINU:
            if (c == 'e') { return DS_CONTINUE; }
            return DS_ERROR;
        case DS_DEFAULT:
            return DS_ERROR;
        case DS_FINALLY:
            return DS_ERROR;
        case DS_SYNCHRO:
            if (c == 'n') { return DS_SYNCHRON; }
            return DS_ERROR;
        case DS_PRIVATE:
            return DS_ERROR;
        case DS_TRANSIE:
            if (c == 'n') { return DS_TRANSIEN; }
            return DS_ERROR;
        case DS_VOLATIL:
            if (c == 'e') { return DS_VOLATILE; }
            return DS_ERROR;
        case DS_STRICTF:
            if (c == 'p') { return DS_STRICTFP; }
            return DS_ERROR;
        case DS_CONTINUE:
            return DS_ERROR;
        case DS_SYNCHRON:
            if (c == 'i') { return DS_SYNCHRONI; }
            return DS_ERROR;
        case DS_TRANSIEN:
            if (c == 't') { return DS_TRANSIENT; }
            return DS_ERROR;
        case DS_VOLATILE:
            return DS_ERROR;
        case DS_STRICTFP:
            return DS_ERROR;
        case DS_SYNCHRONI:
            if (c == 'z') { return DS_SYNCHRONIZ; }
            return DS_ERROR;
        case DS_TRANSIENT:
            return DS_ERROR;
        case DS_SYNCHRONIZ:
            if (c == 'e') { return DS_SYNCHRONIZE; }
            return DS_ERROR;
        case DS_SYNCHRONIZE:
            if (c == 'd') { return DS_SYNCHRONIZED; }
            return DS_ERROR;
        case DS_SYNCHRONIZED:
            return DS_ERROR;
        default:
            return DS_ERROR;
    }
}

TOKEN_TYPE BannedKeywordDfa::getTokenType()
{
    if (dfa[current_state].first != DS_ABORT) {
        return TT_INVALID;
    }

    if (current_state == DS_DO) { return TT_DO; }
    if (current_state == DS_TRY) { return TT_TRY; }
    if (current_state == DS_CASE) { return TT_CASE; }
    if (current_state == DS_GOTO) { return TT_GOTO; }
    if (current_state == DS_LONG) { return TT_LONG; }
    if (current_state == DS_CATCH) { return TT_CATCH; }
    if (current_state == DS_CONST) { return TT_CONST; }
    if (current_state == DS_FLOAT) { return TT_FLOAT; }
    if (current_state == DS_SUPER) { return TT_SUPER; }
    if (current_state == DS_THROW) { return TT_THROW; }
    if (current_state == DS_DOUBLE) { return TT_DOUBLE; }
    if (current_state == DS_SWITCH) { return TT_SWITCH; }
    if (current_state == DS_THROWS) { return TT_THROWS; }
    if (current_state == DS_DEFAULT) { return TT_DEFAULT; }
    if (current_state == DS_FINALLY) { return TT_FINALLY; }
    if (current_state == DS_PRIVATE) { return TT_PRIVATE; }
    if (current_state == DS_CONTINUE) { return TT_CONTINUE; }
    if (current_state == DS_VOLATILE) { return TT_VOLATILE; }
    if (current_state == DS_STRICTFP) { return TT_STRICTFP; }
    if (current_state == DS_TRANSIENT) { return TT_TRANSIENT; }
    if (current_state == DS_SYNCHRONIZED) { return TT_SYNCHRONIZED; }
    return TT_INVALID;
}

void BannedKeywordDfa::initDfa()
{
    dfa[DS_ERROR] = std::make_pair(DS_ERROR, &dfa_error);
    dfa[DS_START] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_C] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_G] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_D] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_F] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_L] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_S] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_P] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_T] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_V] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_CA] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_CO] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_GO] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_DE] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_DO] = std::make_pair(DS_ABORT, &pyGen2);
    dfa[DS_FI] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_FL] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_LO] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_SW] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_SY] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_PR] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_TR] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_SU] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_VO] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_TH] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_ST] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_CAT] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_CAS] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_CON] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_GOT] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_DEF] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_DOU] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_FIN] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_FLO] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_LON] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_SWI] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_SYN] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_PRI] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_TRA] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_SUP] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_VOL] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_TRY] = std::make_pair(DS_ABORT, &pyGen2);
    dfa[DS_THR] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_STR] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_CATC] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_CASE] = std::make_pair(DS_ABORT, &pyGen2);
    dfa[DS_CONS] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_GOTO] = std::make_pair(DS_ABORT, &pyGen2);
    dfa[DS_CONT] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_DEFA] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_DOUB] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_FINA] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_FLOA] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_LONG] = std::make_pair(DS_ABORT, &pyGen2);
    dfa[DS_SWIT] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_SYNC] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_PRIV] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_TRAN] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_SUPE] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_VOLA] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_THRO] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_STRI] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_CATCH] = std::make_pair(DS_ABORT, &pyGen2);
    dfa[DS_CONST] = std::make_pair(DS_ABORT, &pyGen2);
    dfa[DS_CONTI] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_DEFAU] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_DOUBL] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_FINAL] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_FLOAT] = std::make_pair(DS_ABORT, &pyGen2);
    dfa[DS_SWITC] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_SYNCH] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_PRIVA] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_TRANS] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_SUPER] = std::make_pair(DS_ABORT, &pyGen2);
    dfa[DS_VOLAT] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_THROW] = std::make_pair(DS_ABORT, &pyGen2);
    dfa[DS_STRIC] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_CONTIN] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_DEFAUL] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_DOUBLE] = std::make_pair(DS_ABORT, &pyGen2);
    dfa[DS_FINALL] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_SWITCH] = std::make_pair(DS_ABORT, &pyGen2);
    dfa[DS_SYNCHR] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_PRIVAT] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_TRANSI] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_VOLATI] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_STRICT] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_THROWS] = std::make_pair(DS_ABORT, &pyGen2);
    dfa[DS_CONTINU] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_DEFAULT] = std::make_pair(DS_ABORT, &pyGen2);
    dfa[DS_FINALLY] = std::make_pair(DS_ABORT, &pyGen2);
    dfa[DS_SYNCHRO] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_PRIVATE] = std::make_pair(DS_ABORT, &pyGen2);
    dfa[DS_TRANSIE] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_VOLATIL] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_STRICTF] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_CONTINUE] = std::make_pair(DS_ABORT, &pyGen2);
    dfa[DS_SYNCHRON] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_TRANSIEN] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_VOLATILE] = std::make_pair(DS_ABORT, &pyGen2);
    dfa[DS_STRICTFP] = std::make_pair(DS_ABORT, &pyGen2);
    dfa[DS_SYNCHRONI] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_TRANSIENT] = std::make_pair(DS_ABORT, &pyGen2);
    dfa[DS_SYNCHRONIZ] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_SYNCHRONIZE] = std::make_pair(DS_RUNNING, &pyGen2);
    dfa[DS_SYNCHRONIZED] = std::make_pair(DS_ABORT, &pyGen2);
}

BannedKeywordDfa::BannedKeywordDfa() : Dfa()
{
    initDfa();
}
