#include "multiCommentDfa.h"
#include "states.h"

TOKEN_TYPE MultiCommentDfa::endTokenType = TT_INVALID;

int checkSlashMulti(char c, int current_state)
{
	if(c == '/')
	{
		if(current_state == DS_START)
		{
			return DS_SLASHFIRST;
		}
		if(current_state == DS_STARSECOND)
		{
			MultiCommentDfa::endTokenType = TT_COMMENT;
			return DS_ACCEPT;
		}
	}
	if(current_state == DS_STARSECOND)
	{
		return DS_MIDCOMMENT;
	}
	return DS_ERROR;
}

int checkStar(char c, int current_state)
{
	if(c == '*') 
	{
		if(current_state == DS_SLASHFIRST)
		{
			return DS_MIDCOMMENT;
		}
		else
		{
			return DS_STARSECOND;
		}
	}
	if(current_state == DS_MIDCOMMENT)
	{
		return DS_MIDCOMMENT;
	}
	return DS_ERROR;
}

void MultiCommentDfa::initDfa() {
    dfa[DS_ERROR] = std::make_pair(DS_ERROR, &error);
    dfa[DS_START] = std::make_pair(DS_RUNNING, &checkSlashMulti);
    dfa[DS_SLASHFIRST] = std::make_pair(DS_RUNNING, &checkStar);
	dfa[DS_STARSECOND] = std::make_pair(DS_RUNNING, &checkSlashMulti);
	dfa[DS_MIDCOMMENT] = std::make_pair(DS_RUNNING, &checkStar);
    dfa[DS_ACCEPT] = std::make_pair(DS_ACCEPT, &error);
}

TOKEN_TYPE MultiCommentDfa::getTokenType() {
    if(dfa[current_state].first != DS_ACCEPT) {
        return TT_INVALID;
    }

    return endTokenType;
}

MultiCommentDfa::MultiCommentDfa() : Dfa() {
    initDfa();
}