#pragma once
#include "stdafx.h"
//周期转换的函数，以后可能会用到，暂时不做考虑


class   CPhraseCalc
{
public:	
	bool CPhraseCalc::ChangePhrase(vector<SKLine *> &vecKLineSrc, EnumPhrase enumPhraseSource, vector<SKLine *> &vecKLineDest, EnumPhrase enumPhraseDest);
protected:
	bool ChangePhrase_Day2Week(vector<SKLine *>& vecKLineSrc, vector<SKLine *>& vecKLineDest);
	
};
