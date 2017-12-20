
#include "DealerPlotCenter.h"

CDealerPlotCenter::CDealerPlotCenter(void)
{
	m_mapConvDirect.insert(make_pair(g_pcsPlotDirect[1], 1));
	m_mapConvDirect.insert(make_pair(g_pcsPlotDirect[2], 0));
	m_mapConvDirect.insert(make_pair(g_pcsPlotDirect[3], -1));
}

CDealerPlotCenter::~CDealerPlotCenter(void)
{
}

void CDealerPlotCenter::ClearSubAccount()
{
	map<string, CSubAccountCore*>::iterator it;
	
	for(it = m_mapDealer.begin(); it != m_mapDealer.end(); it++) {
		if(it->second != NULL)
			delete it->second;
	}
	m_mapPlot2Dealer.clear();
}

BOOL CDealerPlotCenter::ConvDirect(const IMPORTPLOTDIRECT& in, INSTPLOTDIRECT& out)
{
	BOOL bRet = FALSE;
	map<string, int>::iterator it = m_mapConvDirect.find(in.direct);
	if(it != m_mapConvDirect.end()) {
		strncpy(out.instID, in.instID, sizeof(out.instID)-1);
		strncpy(out.plotID, in.plotID, sizeof(out.plotID)-1);
		out.nDirectMode = it->second;
		bRet = TRUE;
	}
	return bRet;
}

bool CDealerPlotCenter::Init(map<string, set<string>>& mapDealerPlots)
{

	map<string, set<string>>::iterator it;
	set<string>::iterator itID;
	map<string, set<string>>::iterator itPlot;

	LockObject();

	m_mapDealer.clear();
	ClearSubAccount();
	
	UnlockObject();

	for(it = mapDealerPlots.begin(); it != mapDealerPlots.end(); it++) {
		
#if 1

		AddDealer(it->first, it->second);

#else

		// 用函数AddDealer代替下面的代码

		CSubAccountCore* pCore = new CSubAccountCore();

		for(itID = it->second.begin(); itID != it->second.end(); itID++) {
			pCore->AppendPlotID(*itID);
		}
		m_mapDealer.insert(make_pair(it->first, pCore));

		for(itID = it->second.begin(); itID != it->second.end(); itID++) {
			itPlot = m_mapPlot2Dealer.find(*itID);
			if(itPlot == m_mapPlot2Dealer.end()) {
				set<string> setDealer;
				setDealer.insert(it->first);
				m_mapPlot2Dealer.insert(make_pair(*itID, setDealer));
			}
			else {
				itPlot->second.insert(it->first);
			}
		}

#endif

	}
	

	return true;
}

bool CDealerPlotCenter::AddDealer(const string& strAccountID, set<string>& setPlots)
{
	map<string, CSubAccountCore*>::iterator it;
	map<string, set<string>>::iterator itPlot;
	set<string>::iterator itID;

	LockObject();

	it = m_mapDealer.find(strAccountID);
	if(it == m_mapDealer.end()) {

		CSubAccountCore* pCore = new CSubAccountCore();

		for(itID = setPlots.begin(); itID != setPlots.end(); itID++) {
			pCore->AppendPlotID(*itID);
		}
		m_mapDealer.insert(make_pair(strAccountID, pCore));

		for(itID = setPlots.begin(); itID != setPlots.end(); itID++) {
			itPlot = m_mapPlot2Dealer.find(*itID);
			if(itPlot == m_mapPlot2Dealer.end()) {
				set<string> setDealer;
				setDealer.insert(strAccountID);
				m_mapPlot2Dealer.insert(make_pair(*itID, setDealer));
			}
			else {
				itPlot->second.insert(strAccountID);
			}
		}
	}

	UnlockObject();

	return true;
}

bool CDealerPlotCenter::DeleteDealer(const string& strAccountID)
{
	map<string, CSubAccountCore*>::iterator it;
	map<string, set<string>>::iterator itPlot;
	set<string>::iterator itID;

	LockObject();

	it = m_mapDealer.find(strAccountID);
	if(it != m_mapDealer.end()) {
		m_mapDealer.erase(it);

		for(itPlot = m_mapPlot2Dealer.begin(); itPlot != m_mapPlot2Dealer.end(); itPlot++) {
			itPlot->second.erase(strAccountID);
		}
	}

	UnlockObject();

	return true;
}

bool CDealerPlotCenter::AddPlot2Dealer(const string& strPlotID, set<string>& setAccounts)
{
	map<string, CSubAccountCore*>::iterator it;
	map<string, set<string>>::iterator itPlot;
	set<string>::iterator itID;

	LockObject();

	for(itID = setAccounts.begin(); itID != setAccounts.end(); itID++) {
		it = m_mapDealer.find(*itID);
		if(it == m_mapDealer.end()) 
			continue;
		if(it->second != NULL) {
			it->second->AppendPlotID(strPlotID);
		}
		itPlot = m_mapPlot2Dealer.find(strPlotID);
		if(itPlot == m_mapPlot2Dealer.end()) {
			set<string> setDealer;
			setDealer.insert(*itID);
			m_mapPlot2Dealer.insert(make_pair(strPlotID, setDealer));
		}
		else {
			itPlot->second.insert(*itID);
		}
	}

	UnlockObject();

	return true;
}

bool CDealerPlotCenter::DeletePlot2Dealer(const string& strPlotID, set<string>& setAccounts)
{
	map<string, CSubAccountCore*>::iterator it;
	map<string, set<string>>::iterator itPlot;
	set<string>::iterator itID;

	LockObject();

	for(itID = setAccounts.begin(); itID != setAccounts.end(); itID++) {
		it = m_mapDealer.find(*itID);
		if(it == m_mapDealer.end()) 
			continue;
		if(it->second != NULL) {
			it->second->RemovePlotID(strPlotID);
		}
		itPlot = m_mapPlot2Dealer.find(strPlotID);
		if(itPlot != m_mapPlot2Dealer.end()) {
			itPlot->second.erase(*itID);
		}
	}

	UnlockObject();

	return true;
}

void CDealerPlotCenter::Release()
{
	LockObject();
	ClearSubAccount();
	UnlockObject();
}

bool CDealerPlotCenter::AppendPlotDirect(IMPORTPLOTDIRECT& plotDirect, set<string>& setDealer)
{
	map<string, CSubAccountCore*>::iterator itDealer;
	map<string, set<string>>::iterator it;
	set<string>::iterator itID;
	
	LockObject();

	it = m_mapPlot2Dealer.find(plotDirect.plotID);
	if(it != m_mapPlot2Dealer.end()) {
		for(itID = it->second.begin(); itID != it->second.end(); itID++) {
			itDealer = m_mapDealer.find(*itID);
			if(itDealer != m_mapDealer.end()) {
				if(itDealer->second != NULL) {
					INSTPLOTDIRECT out = {0};
					memset(&out, 0, sizeof(out));
					if(ConvDirect(plotDirect, out))
						itDealer->second->AppendPlotDirect(out);
				}
			}
		}
		setDealer = it->second;
	}

	UnlockObject();
	return true;
}

bool CDealerPlotCenter::AppendPlotDirect(queue<IMPORTPLOTDIRECT>& plotDirect, set<string>& setDealer)
{
	while(!plotDirect.empty()) {
		AppendPlotDirect(plotDirect.front(), setDealer);
		plotDirect.pop();
	}
	return true;
}

bool CDealerPlotCenter::CalcCtrl(string& nDealer, string& strInstID, int& nRetCtrl)
{
	map<string, CSubAccountCore*>::iterator it;
	bool bRet = false;

	LockObject();

	it = m_mapDealer.find(nDealer);
	if(it != m_mapDealer.end()) {
		if(it->second != NULL)
			bRet = it->second->CalcCtrl(strInstID, nRetCtrl);
	}

	UnlockObject();
	return bRet;
}
