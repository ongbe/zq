#include "stdafx.h"
#include "../inc/Quotation/PhraseCalc.h"
#include <time.h>

bool CPhraseCalc::ChangePhrase(vector<SKLine *> &vecKLineSrc, EnumPhrase enumPhraseSource, vector<SKLine *> &vecKLineDest, EnumPhrase enumPhraseDest)
{
	if(enumPhraseSource == PHRASE_DAY &&  enumPhraseDest == PHRASE_WEEK)
		ChangePhrase_Day2Week(vecKLineSrc, vecKLineDest);
	return true;
}
bool CPhraseCalc::ChangePhrase_Day2Week(vector<SKLine *>& vecKLineSrc, vector<SKLine *>& vecKLineDest)
{
	return true;
}