#include "singleCommentDfa.h"
#include "states.h"

TOKEN_TYPE SingleCommentDfa::endTokenType = TT_INVALID;

int checkSlashSingle(char c, int current_state)
{
	if(c == '/')
	{
		if(current_state == DS_START)
		{
			return DS_SLASHFIRST;
		}
		if(current_state == DS_SLASHFIRST)
		{
			SingleCommentDfa::endTokenType = TT_COMMENT;
			return DS_ACCEPT;
		}
	}
	
	return DS_ERROR;
}

int checkLineFeed(char c, int current_state)
{
	if(c == 10) //line feed character LF. This will work for Unix and Windows newlines. Will not work for certain obscure opperating systems
	{
		return DS_ERROR;
	}
	return DS_ACCEPT;
}

void SingleCommentDfa::initDfa() {
    dfa[DS_ERROR] = std::make_pair(DS_ERROR, &error);
    dfa[DS_START] = std::make_pair(DS_RUNNING, &checkSlashSingle);
    dfa[DS_SLASHFIRST] = std::make_pair(DS_RUNNING, &checkSlashSingle);
    dfa[DS_ACCEPT] = std::make_pair(DS_ACCEPT, &checkLineFeed);
}

TOKEN_TYPE SingleCommentDfa::getTokenType() {
    if(dfa[current_state].first != DS_ACCEPT) {
        return TT_INVALID;
    }

    return endTokenType;
}

SingleCommentDfa::SingleCommentDfa() : Dfa() {
    initDfa();
}