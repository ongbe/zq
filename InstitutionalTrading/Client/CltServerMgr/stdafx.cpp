// stdafx.cpp : source file that includes just the standard includes
// CltServerMgr.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

CPopMsgWin* g_poPopMsg = NULL;

CPopMsgWin* _GetPopMsg()
{
	if(g_poPopMsg == NULL) {
		CPopMsgWin::Stru_Param param;
		param.bInitVisible = false;
        strncpy(param.szTitle,"管理系统消息",sizeof(param.szTitle)-1);
		g_poPopMsg = new CPopMsgWin(param);
	}
	return g_poPopMsg;
}

void _ReleasePopMsg()
{
	if(g_poPopMsg != NULL) {
		//::PostMessage(g_poPopMsg->GetSafeWnd(), WM_QUIT, 0, 0);
		// delete可能导致死锁
		//delete g_poPopMsg;
		g_poPopMsg = NULL;
	}
}

void _Replace_stdstring(std::string& strBig, const std::string& strsrc, const std::string &strdst)
{
	std::string::size_type pos = 0;
	while((pos = strBig.find(strsrc, pos)) != std::string::npos) {
		strBig.replace(pos, strsrc.length(), strdst);
		pos += strdst.length();
	}
}

int _MakeDateSeq(const std::string& strStartDate, const std::string& strEndDate, 
				  std::queue<std::string>& queueDate)
{
	int nCount = 0;
	std::string strNewStartDate(strStartDate);
	std::string strNewEndDate(strEndDate);

	_Replace_stdstring(strNewStartDate, "-", "");
	_Replace_stdstring(strNewEndDate, "-", "");

	int nNewStartDate = atoi(strNewStartDate.c_str());
	int nNewEndDate = atoi(strNewEndDate.c_str());
	struct tm timeNext, timeEnd;
	time_t tmNext = 0, tmEnd = 0;
	char csDate[32] = {0};

	memset(&timeNext, 0, sizeof(timeNext));
	memset(&timeEnd, 0, sizeof(timeEnd));

	timeNext.tm_year	=nNewStartDate/10000-1900;
	timeNext.tm_mon		=(nNewStartDate%10000)/100-1;
	timeNext.tm_mday	=nNewStartDate%100;
	timeNext.tm_hour	=0;
	timeNext.tm_min		=0;
	timeNext.tm_sec		=0;
	timeNext.tm_isdst	=0;	

	timeEnd.tm_year		=nNewEndDate/10000-1900;
	timeEnd.tm_mon		=(nNewEndDate%10000)/100-1;
	timeEnd.tm_mday		=nNewEndDate%100;
	timeEnd.tm_hour		=0;
	timeEnd.tm_min		=0;
	timeEnd.tm_sec		=0;
	timeEnd.tm_isdst	=0;	

	tmNext = mktime(&timeNext);
	tmEnd = mktime(&timeEnd);
	while(tmNext <= tmEnd) {
		timeNext = *localtime(&tmNext);
		memset(csDate, 0, sizeof(csDate));
		sprintf_s(csDate, sizeof(csDate)-1, "%04d-%02d-%02d", 
				timeNext.tm_year+1900, timeNext.tm_mon+1, timeNext.tm_mday);
		queueDate.push(std::string(csDate));
		timeNext.tm_mday += 1;
		tmNext = mktime(&timeNext);
		nCount++;
	}
	return nCount;
}