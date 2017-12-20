#pragma once

#include <windows.h>
#include <map>
#include <set>
#include <queue>
#include "easymutex.h"
#include "PlotCtrlCommon.h"
#include "SubAccountCore.h"

using namespace std;

class CDealerPlotCenter
{
public:
	CDealerPlotCenter(void);
	virtual ~CDealerPlotCenter(void);

	void LockObject() {
		m_mutex.lock();
	};
	void UnlockObject() {
		m_mutex.unlock();
	};

	bool Init(map<string, set<string>>& mapDealerPlots);
	void Release();

	bool AddDealer(const string& strAccountID, set<string>& setPlots);
	bool DeleteDealer(const string& strAccountID);
	bool AddPlot2Dealer(const string& strPlotID, set<string>& setAccounts);
	bool DeletePlot2Dealer(const string& strPlotID, set<string>& setAccounts);

	bool AppendPlotDirect(IMPORTPLOTDIRECT& plotDirect, set<string>& setDealer);
	bool AppendPlotDirect(queue<IMPORTPLOTDIRECT>& plotDirect, set<string>& setDealer);

	bool CalcCtrl(string& nDealer, string& strInstID, int& nRetCtrl);

protected:
	// 清空Dealer存储map，内部无加锁。
	void ClearSubAccount();
	BOOL ConvDirect(const IMPORTPLOTDIRECT& in, INSTPLOTDIRECT& out);

protected:
	Ceasymutex					m_mutex;
	map<string, CSubAccountCore*>	m_mapDealer;
	map<string, set<string>>		m_mapPlot2Dealer;
	map<string, int>				m_mapConvDirect;

};
