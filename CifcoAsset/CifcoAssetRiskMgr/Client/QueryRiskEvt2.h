#pragma once
#include "CDataInfo.h"
//#include "CFinancialProductIDListDlg.h"
//#include "CAssetMgmtOrganIDTreeListDlg.h"
//#include "COrganAndAccountIDTreeListDlg.h"
# define MsgStatus_All    10

struct RiskEventKey
{
	RiskEventIDType			nRiskEventID;
	RiskEventSubIDType		nRiskEventSubID;
	bool operator<(const RiskEventKey item) const
	{
		if(nRiskEventID > item.nRiskEventID)
			return false;
		else if(nRiskEventID < item.nRiskEventID)
			return true;

		if(nRiskEventSubID > item.nRiskEventSubID)
			return false;
		else if(nRiskEventSubID < item.nRiskEventSubID)
			return true;

		
		return false;
	}
};

class QueryRiskEvt2
{
public:
	QueryRiskEvt2(void);
	~QueryRiskEvt2(void);
	//订阅事件
	static void SubscribeEvt(unsigned int hWnd);
	//退订事件
	static void UnsubscribeEvt(unsigned int hWnd);

	//向服务器请求查询风险事件
    void ReqQueryRiskEvt(std::set<int> setAccntID, std::string strDateStart, std::string strDateEnd );
	//处理风险事件消息
    bool ProcessQueryRiskEvtPkg(const char* pPkg,std::map<int, RiskEvent>& mapRiskEvent,int fieldcnt);


public:
	//基础数据
	CDataInfo* m_pdataInfo;
	/*std::map<RiskEventKey, RiskEvent> mapRiskEvent;*/
	//CAssetMgmtOrganIDTreeListDlg^     dlgMgmtOrganIDTreeList;
	//CFinancialProductIDListDlg^       dlgFinancialProductIDList;
	//COrganAndAccountIDTreeListDlg^    dlgOrganAndAccountIDTreeList;

};
