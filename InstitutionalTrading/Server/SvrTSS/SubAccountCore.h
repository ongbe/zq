#pragma once

#include <string>
#include <map>
#include <set>
#include "PlotCtrlCommon.h"

using namespace std;

class CSubAccountCore
{
public:
	CSubAccountCore(void);
	virtual ~CSubAccountCore(void);

	bool AppendPlotID(const string& strPlotID);
	bool RemovePlotID(const string& strPlotID);
	bool AppendPlotDirect(INSTPLOTDIRECT& plotDirect);

	bool CalcCtrl(string& strInstID, int& nRetCtrl);

protected:
	//bool CalcAndMarkCtrl2TraderMdl();
	//bool MarkCtrl2TraderMdl(int nDealer, string strInstID, char cDirect, int nCtrl);

protected:
	set<string>		m_setPlotID;
	map<string, map<string, int>>	m_mapInstPlots;
};
