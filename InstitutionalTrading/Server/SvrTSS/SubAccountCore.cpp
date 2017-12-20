
#include "SubAccountCore.h"
#include "CommonStruct.h"

CSubAccountCore::CSubAccountCore(void)
{
}

CSubAccountCore::~CSubAccountCore(void)
{
}

bool CSubAccountCore::AppendPlotID(const string& strPlotID)
{
	set<string>::iterator it = m_setPlotID.find(strPlotID);
	if(it != m_setPlotID.end())
		return false;

	m_setPlotID.insert(strPlotID);
	return true;
}

bool CSubAccountCore::RemovePlotID(const string& strPlotID)
{
	set<string>::iterator it = m_setPlotID.find(strPlotID);
	if(it == m_setPlotID.end())
		return false;

	m_setPlotID.erase(it);
	return true;
}

bool CSubAccountCore::AppendPlotDirect(INSTPLOTDIRECT& plotDirect)
{
	if(m_setPlotID.find(plotDirect.plotID) == m_setPlotID.end())
		return false;

	map<string, map<string, int>>::iterator it;
	map<string, int>::iterator itPlot;

	it = m_mapInstPlots.find(plotDirect.instID);
	if(it == m_mapInstPlots.end()) {
		map<string, int> mapPlot;
		mapPlot.insert(make_pair(plotDirect.plotID, plotDirect.nDirectMode));
		m_mapInstPlots.insert(make_pair(plotDirect.instID, mapPlot));
	}
	else {
		it->second[plotDirect.plotID] = plotDirect.nDirectMode;
	}

	return true;
}

bool CSubAccountCore::CalcCtrl(string& strInstID, int& nRetCtrl)
{
	map<string, map<string, int>>::iterator it;
	map<string, int>::iterator itPlot;

	it = m_mapInstPlots.find(strInstID);
	if(it != m_mapInstPlots.end()) {
		map<string, int>& mapPlot = it->second;
		int nBuyEmbar = 0, nSellEmbar = 0;
		for(itPlot = mapPlot.begin(); itPlot != mapPlot.end(); itPlot++) {
			if(itPlot->second>0)
				nSellEmbar++;
			else if(itPlot->second<0)
				nBuyEmbar++;
		}
		if(nBuyEmbar>0 && nSellEmbar>0)
			nRetCtrl = STRATEGY_FORBID_BOTH_SIDE;
		else if(nBuyEmbar>0 && nSellEmbar==0)
			nRetCtrl = STRATEGY_FORBID_BUY;
		else if(nBuyEmbar==0 && nSellEmbar>0)
			nRetCtrl = STRATEGY_FORBID_SELL;
		else
			nRetCtrl = STRATEGY_NO_FORBID;
		return true;
	}
	else
		return false;
}
