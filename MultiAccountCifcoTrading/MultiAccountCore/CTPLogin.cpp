#include "StdAfx.h"
#include "CTPLogin.h"
#include "wx/file.h"
#include "wx/textfile.h"
#include "../inc/Module-Misc/GLobalFunc.h"
#define INSTRUMENT_FILENAME "Instruments.txt"
#define INSTRUMENT_FORMAT_STRING "%s,%s,%s,%s,%s,%c,%d,%d,%d,%d,%d,%d,%d,%f,%s,%s,%s,%s,%s,%c,%d,%c,%c,%f,%f"
#define SERVERPARAM_FILENAME "ServerParam.xml"
#define DEFINE_THUNK(tag)\
int CTPLogin::##tag##Thunk(const GUIModuleID GID,const AbstractBusinessData& data)\
{\
	IPlatformSingleSvr* pSvr=static_cast<IPlatformSingleSvr*>(data.pPlatformSvr);\
	CTPLogin* pLogin=CTPLogin::GetCTPLogin(pSvr);\
	if(pLogin==NULL) return -1;\
	return pLogin->##tag##CallBackFunc(GID,data);\
}
#define TOP_ELEMENT_EQU(text,tag)\
	if(strcmp(pFieldElement->Value(),#text)==0&&pFieldElement->GetText())\
{\
	if(DEFAULT_SVR()) \
	DEFAULT_SVR()->GetServerParamCfg().tag=pFieldElement->GetText();\
}
#define TOP_ELEMENT_SAVE(text,tag)\
	if(DEFAULT_SVR())\
{\
	TiXmlElement *pElement = new TiXmlElement(#text);\
	TiXmlText *pContent = new TiXmlText(DEFAULT_SVR()->GetServerParamCfg().tag.c_str());\
	pElement->LinkEndChild(pContent);\
	RootElement->LinkEndChild(pElement);\
}
static map<IPlatformSingleSvr*,CTPLogin*> g_Svr2Instance;
static map<int,CTPLogin*> g_TimerID2Svr;
extern PlatformSvrMgr* g_pPlatformMgr;
DEFINE_THUNK(QrySettlementInfo)
DEFINE_THUNK(QryInstrumentList)
DEFINE_THUNK(QryTradingAccount)
DEFINE_THUNK(QryTrade)
DEFINE_THUNK(QryInvestorPositionDetail)
DEFINE_THUNK(QryInvestorPosition)
DEFINE_THUNK(QryInvestorPositionCombineDetail)
DEFINE_THUNK(QryOrder)


void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	CTPLogin* pLogin=g_TimerID2Svr[idEvent];
	if(pLogin)pLogin->OnTimer();
}

CTPLogin::CTPLogin(IPlatformSingleSvr* pSvr,LoginCallback* cback):
m_pSvr(pSvr),m_TickCount(0),m_LoginCallback(cback),
m_bQryEnd(false),m_bfirstLTI(true),m_bResultLTI(false),m_bfirstCRM(true)
{
	m_TimerID=SetTimer(NULL,0,TIMER_MILLISECONDS,TimerProc);
	g_Svr2Instance[pSvr]=this;
	g_TimerID2Svr[m_TimerID]=this;
	m_sReady=TIMER_QRYSETTLEMENTINFO;
	m_Step=TIMER_QRYSETTLEMENTINFO;
}
CTPLogin::~CTPLogin()
{
	KillTimer(NULL,m_TimerID);
	map<IPlatformSingleSvr*,CTPLogin*>::iterator itA;
	for (itA=g_Svr2Instance.begin();itA!=g_Svr2Instance.end();)
	{
		if(itA->second==this)
			itA=g_Svr2Instance.erase(itA);
		else
			itA++;
	}
	map<int,CTPLogin*>::iterator itB;
	for (itB=g_TimerID2Svr.begin();itB!=g_TimerID2Svr.end();)
	{
		if(itB->second==this)
			itB=g_TimerID2Svr.erase(itB);
		else
			itB++;
	}
}
CTPLogin* CTPLogin::GetCTPLogin(IPlatformSingleSvr* pSvr)
{
	return g_Svr2Instance[pSvr];
}
void CTPLogin::OnTimer()
{
	if(m_bQryEnd) return;
	m_LoginCallback->SetProgressPosition(m_pSvr,TIMER_FINISH,m_Step+1);   
	ServiceStatus ss=m_pSvr?m_pSvr->GetCurrentStatus():SS_NOLOGIN;
	switch (ss)
	{
	case SS_NOLOGIN:
	case SS_LOGINFAILED:
		m_sReady=TIMER_QRYSETTLEMENTINFO;
		m_Step=m_sReady;
		m_LoginCallback->DoLoginFail(m_pSvr);
		break;
	case SS_LOGINING:
		m_TickCount++;
		if(m_TickCount>=TIMER_OVERTIME*500/TIMER_MILLISECONDS)
		{
			m_sReady=TIMER_QRYSETTLEMENTINFO;
			m_Step=m_sReady;
			m_LoginCallback->DoLoginFail(m_pSvr,LOADSTRING(USERLOGINDLG_LOGINTIMEOUT));
		}
		break;
	case SS_LOGINED:
		DoLoginSuccess();
		break;
	default:
		break;
	}
}
int CTPLogin::SetParticipantInfo(const std::string& BrokerID, 
								 const std::string& UserID, 
								 const std::string& UserPW, 
								 const std::string& OneTimePassword)
{
	if(m_pSvr==NULL)
		return -1;
	return m_pSvr->SetParticipantInfo(BrokerID,UserID,UserPW,OneTimePassword);
}
int CTPLogin::SetTradeFrontAddrs(const std::vector<std::string>& TradeAddr)
{
	if(m_pSvr==NULL)
		return -1;
	return m_pSvr->SetTradeFrontAddrs(TradeAddr);
}
int CTPLogin::SetQuotFrontAddrs(const std::vector<std::string>& QuotAddr)
{
	if(m_pSvr==NULL)
		return -1;
	return m_pSvr->SetQuotFrontAddrs(QuotAddr);
}
int	CTPLogin::SetProxyConnectStr(const std::string& ProxyType,const std::string& ProxyUser,
					   const std::string& ProxyPwd,const std::string& ProxyAddr,const std::string& ProxyPort)
{
	string strProxyConnect;
	//格式为"socks5://admin:admin@10.10.11.52:1080";
	if(ProxyType=="socks5")
	{
		strProxyConnect+=ProxyType;
		if(!ProxyType.empty())
		{
			strProxyConnect+="://";
			if(ProxyType=="socks5")
			{
				strProxyConnect+=ProxyUser;
				strProxyConnect+=":";
				strProxyConnect+=ProxyPwd;
				strProxyConnect+="@";
			}
			strProxyConnect+=ProxyAddr;
			if(!ProxyAddr.empty())
			{
				strProxyConnect+=":";
				strProxyConnect+=ProxyPort;
			}
		}
	}
	return SetProxyConnectStr(strProxyConnect);
}
int	CTPLogin::SetProxyConnectStr(const std::string& str)
{
	if(m_pSvr==NULL)
		return -1;
	return m_pSvr->SetProxyConnectStr(str);
}
int CTPLogin::Login(const std::string& InvestorID, 
					const std::string& InvestorPW, 
					const std::string& InvestorNewPWD)
{
	if(m_pSvr==NULL)
		return -1;
	m_LoginCallback->SetPromptText(m_pSvr,LOADSTRING(USERLOGINDLG_LOGINING));
	return m_pSvr->Login(InvestorID,InvestorPW,InvestorNewPWD);
}

void CTPLogin::DoLoginSuccess()
{
	if(m_pSvr==NULL) return;
	switch(m_sReady)
	{
	case TIMER_ERROR://数据同步中，某一个环节发生错误，
		{
			m_sReady=TIMER_QRYSETTLEMENTINFO;
			m_Step=m_sReady;
			m_pSvr->UnSubscribeBusinessData(BID_RspQrySettlementInfo, GID_LOGIN_DLG);
			m_LoginCallback->DoLoginFail(m_pSvr,LOADSTRING(USERLOGINDLG_QRYTIMEOUT));
		}
		break;
	case TIMER_QRYSETTLEMENTINFO://开始查询投资者结算结果
		m_pSvr->NotifyStartStopInitQry(true);      //通知底层，开始初始化查询，期间不要查费率
		if(!m_pSvr->IsSettlementInfoConfirmed())
		{ 
			m_LoginCallback->SetPromptText(m_pSvr,LOADSTRING(USERLOGINDLG_QRYSTL));
			m_MutexLock.Lock();
			m_pSvr->SubscribeBusinessData(BID_RspQrySettlementInfo, GID_LOGIN_DLG, QrySettlementInfoThunk);
			PlatformStru_QrySettlementInfo tmp;
			memset(&tmp, 0, sizeof(tmp));
			if(m_pSvr->ReqQrySettlementInfo(tmp)==0)
				m_sReady = TIMER_READY;
			m_MutexLock.Unlock();
		}
		else
		{
			m_sReady=TIMER_QRYSETTLEMENTINFO+1;
			m_Step=m_sReady;
		}
		break;
	case TIMER_QRYINSTRUMENT://开始查询合约列表
		if(!LoadTodayInstruments())
		{
			m_LoginCallback->SetPromptText(m_pSvr,LOADSTRING(USERLOGINDLG_QRYINST));
			m_MutexLock.Lock();
			 m_pSvr->SubscribeBusinessData(BID_RspQryInstrument, GID_LOGIN_DLG, QryInstrumentListThunk);
			PlatformStru_QryInstrument QryInstrument;
			memset(&QryInstrument, 0, sizeof(QryInstrument));
			if(m_pSvr&&m_pSvr->ReqQryInstrument(QryInstrument)==0)
				m_sReady = TIMER_READY;
			m_MutexLock.Unlock();
		}
		else
		{
			m_sReady=TIMER_QRYINSTRUMENT+1;
			m_Step=m_sReady;
		}
		break;
	case TIMER_QRYTRADINGACCOUNT://开始查询资金
		m_LoginCallback->SetPromptText(m_pSvr,LOADSTRING(USERLOGINDLG_QRYFUND));
		//加载本地保存的当天的保证金率和手续费率
		if(m_pSvr&&m_pSvr->HaveFileCache())
		{
			LoadCommissionRateAndMarginRate("cam.xml");
			// 添加生成容错合约列表
			{
				map<string, string> mapConstractIDVariety;
				MakeConstractIDVarietyMap(mapConstractIDVariety);
				 m_pSvr->SetInstrumentVarietyMap(mapConstractIDVariety);
			}

			if(LoadTodayInstruments())
			{
				//修改成只订阅主力合约和必要合约行情
				//std::set<std::string> tmpInstrumentID;
				// m_pSvr->GetInstrumentList(tmpInstrumentID);
				//int ret;
				//for (std::set<std::string>::iterator it = tmpInstrumentID.begin(); it != tmpInstrumentID.end(); ++it) 
				//{
				//       
				//          ret=m_pSvr->SubscribeQuot(it->c_str());       
				//}
			}
			else
			{
				SaveTodayInstruments();
			}
		}
		
		{
			m_MutexLock.Lock();
			m_pSvr->SubscribeBusinessData(BID_RspQryTradingAccount, GID_LOGIN_DLG, QryTradingAccountThunk);
			PlatformStru_QryTradingAccount QryTradingAccount;
			if(m_pSvr->ReqQryTradingAccount(QryTradingAccount)==0)
				m_sReady = TIMER_READY;    
			m_MutexLock.Unlock();
		}
		break;
	case TIMER_QRYTRADE://开始查询成交记录
		m_LoginCallback->SetPromptText(m_pSvr,LOADSTRING(USERLOGINDLG_QRYTRADER));
		
		{
			m_MutexLock.Lock();
			m_pSvr->SubscribeBusinessData(BID_RspQryTrade, GID_LOGIN_DLG, QryTradeThunk);
			PlatformStru_QryTrade QryTrade;
			if(m_pSvr->ReqQryTrade(QryTrade)==0)
				m_sReady = TIMER_READY;
			m_MutexLock.Unlock();
		}
		break;
	case TIMER_QRYINVESTORPOSITIONDETAIL:
		m_LoginCallback->SetPromptText(m_pSvr,LOADSTRING(USERLOGINDLG_QRYPOSDETA));
		
		{
			m_MutexLock.Lock();
			m_pSvr->SubscribeBusinessData(BID_RspQryInvestorPositionDetail, GID_LOGIN_DLG, QryInvestorPositionDetailThunk);
			PlatformStru_QryInvestorPositionDetail QryInvestorPositionDetail;
			if(m_pSvr->ReqQryInvestorPositionDetail(QryInvestorPositionDetail)==0)
				m_sReady = TIMER_READY;
			m_MutexLock.Unlock();
		}
		break;
	case TIMER_QRYINVESTORPOSITION:
		m_LoginCallback->SetPromptText(m_pSvr,LOADSTRING(USERLOGINDLG_QRYPOS));
		 
		{
			m_MutexLock.Lock();
			m_pSvr->SubscribeBusinessData(BID_RspQryInvestorPosition, GID_LOGIN_DLG, QryInvestorPositionThunk);
			PlatformStru_QryInvestorPosition QryInvestorPosition;
			if(m_pSvr->ReqQryInvestorPosition(QryInvestorPosition)==0)
				m_sReady = TIMER_READY;
			m_MutexLock.Unlock();
		}
		break;
	case TIMER_QRYINVESTORPOSITIONCOMBINEDETAIL://开始查询持仓组合合约明细
#ifdef _USE_COMBOINSTRUMENT
		m_LoginCallback->SetPromptText(m_pSvr,LOADSTRING(USERLOGINDLG_QRYPOSCOMB));
		
		{
			m_MutexLock.Lock();
			m_pSvr->SubscribeBusinessData(BID_RspQryInvestorPositionComb, GID_LOGIN_DLG, QryInvestorPositionCombineDetailThunk);
			PlatformStru_QryInvestorPositionCombineDetail QryInvestorPositionCombineDetail;
			if(m_pSvr->ReqQryInvestorPositionCombineDetail(QryInvestorPositionCombineDetail)==0)
				m_sReady = TIMER_READY;
			m_MutexLock.Unlock();
		}
#else
		m_sReady=TIMER_QRYINVESTORPOSITIONCOMBINEDETAIL+1;
		m_Step=m_sReady;
#endif
		break;
	case TIMER_QRYORDER://开始查询报单信息
		m_LoginCallback->SetPromptText(m_pSvr,LOADSTRING(USERLOGINDLG_QRYORDER));
		m_MutexLock.Lock();
		 m_pSvr->SubscribeBusinessData(BID_RspQryOrder, GID_LOGIN_DLG, QryOrderThunk);
		PlatformStru_QryOrder tmp;
		memset(&tmp, 0, sizeof(tmp));
		if(m_pSvr&&m_pSvr->ReqQryOrder(tmp)==0)
			m_sReady = TIMER_READY;
		m_MutexLock.Unlock();
		break;
	case INITQRY_QRYCONTRACTBANK:
		m_LoginCallback->SetPromptText(m_pSvr);
		PlatformStru_QryContractBank  QryContractBank;
		memset(&QryContractBank,0,sizeof(QryContractBank));
		if(m_pSvr&&m_pSvr->ReqQryContractBank(QryContractBank)==0)
			m_sReady = TIMER_READY;        
		m_sReady=INITQRY_QRYCONTRACTBANK+1;
		m_Step=m_sReady;
		break;
	case TIMER_FINISH://数据同步全部OK
		{
			//结算确认
			if(!(m_pSvr->IsSettlementInfoConfirmed()))
			{
				m_pSvr->ReqSettlementInfoConfirm();
			}
			m_LoginCallback->SetPromptText(m_pSvr,LOADSTRING(USERLOGINDLG_FINSIH));
			 m_pSvr->UnSubscribeBusinessData(BID_RspQrySettlementInfo, GID_LOGIN_DLG);
			 m_pSvr->UnSubscribeBusinessData(BID_RspQryInstrument, GID_LOGIN_DLG);
			 m_pSvr->UnSubscribeBusinessData(BID_RspQryTradingAccount, GID_LOGIN_DLG);
			 m_pSvr->UnSubscribeBusinessData(BID_RspQryTrade, GID_LOGIN_DLG);
			 m_pSvr->UnSubscribeBusinessData(BID_RspQryInvestorPositionDetail, GID_LOGIN_DLG);
#ifdef _USE_COMBOINSTRUMENT
			 m_pSvr->UnSubscribeBusinessData(BID_RspQryInvestorPositionComb, GID_LOGIN_DLG);
#endif
			 m_pSvr->UnSubscribeBusinessData(BID_RspQryInvestorPosition, GID_LOGIN_DLG);
			 m_pSvr->UnSubscribeBusinessData(BID_RspQryOrder, GID_LOGIN_DLG);
			 m_bQryEnd=true;
			 m_LoginCallback->OnQryFinish(m_pSvr);
			 m_pSvr->SetQryFinished(true);
			 m_pSvr->NotifyStartStopInitQry(false);      //通知底层，初始化查询结束，可以查费率了
		}
		break;
	default://超时处理
		{
			m_sReady++;
			if(m_sReady>TIMER_OVERTIME*1000/TIMER_MILLISECONDS+TIMER_READY)
			{
				m_sReady=TIMER_ERROR;
			}
		}
		break;
	}
}
int CTPLogin::QrySettlementInfoCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data)
{
	MUTEX_GURAD(m_MutexLock);
	if(data.BID!=BID_RspQrySettlementInfo || data.Size!=sizeof(DataRspQrySettlementInfo))
		return 0;	
	DataRspQrySettlementInfo& refData = *(DataRspQrySettlementInfo*)(const_cast<AbstractBusinessData*>(&data));	
	if(refData.RspInfoField.ErrorID==CustomErrorID)//查询无记录
	{
		m_sReady=TIMER_QRYSETTLEMENTINFO+1;
		m_Step=m_sReady;
		return 0;
	}
	else if(refData.RspInfoField.ErrorID!=0)
	{
		m_sReady=TIMER_ERROR;
		return -1;
	}

	if(refData.bIsLast)
	{
		m_sReady=TIMER_QRYSETTLEMENTINFO+1;
		m_Step=m_sReady;
	}

	return 0;
}
int CTPLogin::QryInstrumentListCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data)
{
	MUTEX_GURAD(m_MutexLock);
	if(data.BID!=BID_RspQryInstrument || data.Size!=sizeof(DataRspQryInstrument))
		return 0;	
	DataRspQryInstrument& refData = *(DataRspQryInstrument*)(const_cast<AbstractBusinessData*>(&data));	
	if(refData.RspInfoField.ErrorID==CustomErrorID)//查询无记录,几乎不可能发生
	{
		m_sReady=TIMER_QRYINSTRUMENT+1;
		m_Step=m_sReady;
		return 0;
	}
	if(refData.RspInfoField.ErrorID!=0)
	{
		m_sReady=TIMER_ERROR;
		return -1;
	}
	if(refData.bIsLast)
	{
		m_sReady=TIMER_QRYINSTRUMENT+1;
		m_Step=m_sReady;
	}
	return 0;
}
int CTPLogin::QryTradingAccountCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data)
{
	MUTEX_GURAD(m_MutexLock);
	if(data.BID!=BID_RspQryTradingAccount || data.Size!=sizeof(DataRspQryTradingAccount))
		return 0;	
	DataRspQryTradingAccount& refData = *(DataRspQryTradingAccount*)(const_cast<AbstractBusinessData*>(&data));	
	if(refData.RspInfoField.ErrorID==CustomErrorID)//查询无记录
	{
		m_sReady=TIMER_QRYTRADINGACCOUNT+1;
		m_Step=m_sReady;
		return 0;
	}
	if(refData.RspInfoField.ErrorID!=0)
	{
		m_sReady=TIMER_ERROR;
		return -1;
	}

	if(refData.bIsLast)
	{
		m_sReady=TIMER_QRYTRADINGACCOUNT+1;
		m_Step=m_sReady;
	}
	return 0;
}
int CTPLogin::QryTradeCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data)
{
	MUTEX_GURAD(m_MutexLock);
	if(data.BID!=BID_RspQryTrade || data.Size!=sizeof(DataRspQryTrade))
		return 0;	
	DataRspQryTrade& refData = *(DataRspQryTrade*)(const_cast<AbstractBusinessData*>(&data));	
	if(refData.RspInfoField.ErrorID==CustomErrorID)//查询无记录
	{
		m_sReady=TIMER_QRYTRADE+1;
		m_Step=m_sReady;
		return 0;
	}
	if(refData.RspInfoField.ErrorID!=0)
	{
		m_sReady=TIMER_ERROR;
		return -1;
	}
	if(refData.bIsLast)
	{
		m_sReady=TIMER_QRYTRADE+1;
		m_Step=m_sReady;
	}
	return 0;
}
int CTPLogin::QryInvestorPositionDetailCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data)
{
	MUTEX_GURAD(m_MutexLock);
	if(data.BID!=BID_RspQryInvestorPositionDetail || data.Size!=sizeof(DataRspQryInvestorPositionDetail))
		return 0;	
	DataRspQryInvestorPositionDetail& refData = *(DataRspQryInvestorPositionDetail*)(const_cast<AbstractBusinessData*>(&data));	
	if(refData.RspInfoField.ErrorID==CustomErrorID)//查询无记录
	{
		m_sReady=TIMER_QRYINVESTORPOSITIONDETAIL+1;
		m_Step=m_sReady;
		return 0;
	}
	if(refData.RspInfoField.ErrorID!=0)
	{
		m_sReady=TIMER_ERROR;
		return -1;
	}
	if(refData.bIsLast)
	{
		m_sReady=TIMER_QRYINVESTORPOSITIONDETAIL+1;
		m_Step=m_sReady;
	}
	return 0;
}
int CTPLogin::QryInvestorPositionCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data)
{
	MUTEX_GURAD(m_MutexLock);
	if(data.BID!=BID_RspQryInvestorPosition || data.Size!=sizeof(DataRspQryInvestorPosition))
		return 0;	
	DataRspQryInvestorPosition& refData = *(DataRspQryInvestorPosition*)(const_cast<AbstractBusinessData*>(&data));	
	if(refData.RspInfoField.ErrorID==CustomErrorID)//查询无记录
	{
		m_sReady=TIMER_QRYINVESTORPOSITION+1;
		m_Step=m_sReady;
		return 0;
	}
	if(refData.RspInfoField.ErrorID!=0)
	{
		m_sReady=TIMER_ERROR;
		return -1;
	}
	if(refData.bIsLast)
	{
		m_sReady=TIMER_QRYINVESTORPOSITION+1;
		m_Step=m_sReady;
	}
	return 0;
}

int CTPLogin::QryInvestorPositionCombineDetailCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data)
{
	MUTEX_GURAD(m_MutexLock);
	if(data.BID!=BID_RspQryInvestorPositionComb || data.Size!=sizeof(DataRspQryInvestorPositionComb))
		return 0;	
	DataRspQryInvestorPositionComb& refData = *(DataRspQryInvestorPositionComb*)(const_cast<AbstractBusinessData*>(&data));	
	if(refData.RspInfoField.ErrorID==CustomErrorID)//查询无记录
	{
		m_sReady=TIMER_QRYINVESTORPOSITIONCOMBINEDETAIL+1;
		m_Step=m_sReady;
		return 0;
	}
	if(refData.RspInfoField.ErrorID!=0)
	{
		m_sReady=TIMER_ERROR;
		return -1;
	}
	if(refData.bIsLast)
	{
		m_sReady=TIMER_QRYINVESTORPOSITIONCOMBINEDETAIL+1;
		m_Step=m_sReady;
	}
	return 0;
}


int CTPLogin::QryOrderCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data)
{
	MUTEX_GURAD(m_MutexLock);
	if(data.BID!=BID_RspQryOrder || data.Size!=sizeof(DataRspQryOrder))
		return 0;	
	DataRspQryOrder& refData = *(DataRspQryOrder*)(const_cast<AbstractBusinessData*>(&data));	
	if(refData.RspInfoField.ErrorID==CustomErrorID)//查询无记录
	{
		m_sReady=TIMER_QRYORDER+1;
		m_Step=m_sReady;
		return 0;
	}
	else if(refData.RspInfoField.ErrorID!=0)
	{
		m_sReady=TIMER_ERROR;
		return -1;
	}
	if(refData.bIsLast)
	{
		m_sReady=TIMER_QRYORDER+1;
		m_Step=m_sReady;
	}
	return 0;
}
void CTPLogin::SplitwxString(wxString& strLine,std::vector<std::string>& vec)
{
	int startpos=0,endpos=0;
	wxString strTrueLine=strLine;
	if(strLine.Right(1)!=",")
		strTrueLine+=",";
	while(true)
	{
		endpos=strTrueLine.find(',',startpos);
		if(endpos<0)
		{
			break;
		}
		vec.push_back(strTrueLine.Mid(startpos,endpos-startpos).c_str());
		startpos=endpos+1;   
	}

}
bool CTPLogin::LoadSaveServerParam(bool bLoad)
{
	std::string strAppPath;
	bool bReturn=false;
	GlobalFunc::GetPath(strAppPath);
	strAppPath+=SERVERPARAM_FILENAME;
	if(bLoad)
	{
		//初始化值
		TiXmlDocument myDocument(strAppPath.c_str()); 
		if(myDocument.LoadFile())
		{
			TiXmlElement* RootElement=myDocument.RootElement();
			if(RootElement==NULL) return false;
			int currentitem=0,languageid=0;
			RootElement->Attribute("current",&currentitem);
#ifdef _USE_MULTI_LANGUAGE
			RootElement->Attribute("language",&languageid);	
			if(languageid<LANGUAGE_MIN||languageid>LANGUAGE_MAX)
			{
				LCID lcid=GetSystemDefaultLCID();
				switch(lcid)
				{
				case MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_TRADITIONAL):
				case MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_HONGKONG):				
				case MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_MACAU):
					languageid=LANGUAGE_TRADITION;
					break;
				case MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED):
				case MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SINGAPORE):
					languageid=LANGUAGE_CHINESE;
					break;
				default:
					languageid=LANGUAGE_ENGLISH;
				}
			}
			CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
			if( pMgr != NULL) 
			{
				pMgr->SetLanguageId(languageid);
			}
#else
			languageid=LANGUAGE_CHINESE;
#endif
			if(DEFAULT_SVR()) DEFAULT_SVR()->GetServerParamCfg().current=currentitem;
			if(DEFAULT_SVR()) DEFAULT_SVR()->GetServerParamCfg().LanguageId=languageid;
			TiXmlElement* pFieldElement=RootElement->FirstChildElement();
			while(pFieldElement)
			{
				TOP_ELEMENT_EQU(UniServer,UniserverAddr)
					TOP_ELEMENT_EQU(VersionStr,VersionStr)
					TOP_ELEMENT_EQU(ProxyType,ProxyType)
					TOP_ELEMENT_EQU(ProxyAddr,ProxyAddr)
					TOP_ELEMENT_EQU(ProxyPort,ProxyPort)
					TOP_ELEMENT_EQU(ProxyUser,ProxyUser)
					TOP_ELEMENT_EQU(ProxyPwd,ProxyPwd)
					if(strcmp(pFieldElement->Value(),"Server")==0)
					{
						//一个服务器
						SERVER_PARAM_CFG_ITEM serveritem;
						serveritem.CTPSimulate=0;
						TiXmlElement* pItemElement=pFieldElement->FirstChildElement();
						while(pItemElement)
						{
							if(strcmp(pItemElement->Value(),"Name")==0&&pItemElement->GetText())
								serveritem.ServerName=pItemElement->GetText();
							if(strcmp(pItemElement->Value(),"CTPSimulate")==0&&pItemElement->GetText())
								serveritem.CTPSimulate=atoi(pItemElement->GetText());                        
							if(strcmp(pItemElement->Value(),"BrokerID")==0&&pItemElement->GetText())
								serveritem.BrokerID=pItemElement->GetText();
							if(strcmp(pItemElement->Value(),"InvestorID")==0&&pItemElement->GetText())
								serveritem.InvestorID=pItemElement->GetText();
							if(strcmp(pItemElement->Value(),"Trading")==0)
							{
								TiXmlElement* pTradingItemElement=pItemElement->FirstChildElement();
								while(pTradingItemElement)
								{
									serveritem.TradeAddr.push_back(pTradingItemElement->GetText());
									pTradingItemElement=pTradingItemElement->NextSiblingElement();
								}
							}
							if(strcmp(pItemElement->Value(),"MarketData")==0)
							{
								TiXmlElement* pMarketDataItemElement=pItemElement->FirstChildElement();
								while(pMarketDataItemElement)
								{
									serveritem.QuotAddr.push_back(pMarketDataItemElement->GetText());
									pMarketDataItemElement=pMarketDataItemElement->NextSiblingElement();
								}
							}
							pItemElement=pItemElement->NextSiblingElement();

						}
						if(DEFAULT_SVR()) DEFAULT_SVR()->GetServerParamCfg().vec.push_back(serveritem);
						bReturn=true;
					}

					pFieldElement=pFieldElement->NextSiblingElement();
			}
		}
	}
	else if(DEFAULT_SVR())
	{
		TiXmlDocument myDocument;
		TiXmlElement *RootElement = new TiXmlElement("ServerParam");
		if(DEFAULT_SVR())
		{
			RootElement->SetAttribute("current",DEFAULT_SVR()->GetServerParamCfg().current);
			RootElement->SetAttribute("language",DEFAULT_SVR()->GetServerParamCfg().LanguageId);
		}
		myDocument.LinkEndChild(RootElement);
		TOP_ELEMENT_SAVE(UniServer,UniserverAddr)
			TOP_ELEMENT_SAVE(VersionStr,VersionStr)
			TOP_ELEMENT_SAVE(ProxyType,ProxyType)
			TOP_ELEMENT_SAVE(ProxyAddr,ProxyAddr)
			TOP_ELEMENT_SAVE(ProxyPort,ProxyPort)
			TOP_ELEMENT_SAVE(ProxyUser,ProxyUser)
			TOP_ELEMENT_SAVE(ProxyPwd,ProxyPwd)
			for(int index=0;index<(int)DEFAULT_SVR()->GetServerParamCfg().vec.size();index++)
			{
				SERVER_PARAM_CFG_ITEM& cfgitem=DEFAULT_SVR()->GetServerParamCfg().vec[index];

				TiXmlElement *pServerElement = new TiXmlElement("Server");
				RootElement->LinkEndChild(pServerElement);
				{
					TiXmlElement *pElement = new TiXmlElement("Name");
					TiXmlText *pContent = new TiXmlText(cfgitem.ServerName.c_str());
					pElement->LinkEndChild(pContent);
					pServerElement->LinkEndChild(pElement);
				}
				{
					TiXmlElement *pElement = new TiXmlElement("CTPSimulate");
					wxString str;str.Printf("%d",cfgitem.CTPSimulate);
					TiXmlText *pContent = new TiXmlText(str.c_str());
					pElement->LinkEndChild(pContent);
					pServerElement->LinkEndChild(pElement);
				}
				{
					TiXmlElement *pElement = new TiXmlElement("BrokerID");
					TiXmlText *pContent = new TiXmlText(cfgitem.BrokerID.c_str());
					pElement->LinkEndChild(pContent);
					pServerElement->LinkEndChild(pElement);
				}
				{
					TiXmlElement *pElement = new TiXmlElement("InvestorID");
					TiXmlText *pContent = new TiXmlText(cfgitem.InvestorID.c_str());
					pElement->LinkEndChild(pContent);
					pServerElement->LinkEndChild(pElement);
				}
				{
					TiXmlElement *pTradingElement = new TiXmlElement("Trading");
					pServerElement->LinkEndChild(pTradingElement);
					for(int item=0;item<(int)cfgitem.TradeAddr.size();item++)
					{
						TiXmlElement *pUnitElement = new TiXmlElement("item");
						TiXmlText *pContent = new TiXmlText(cfgitem.TradeAddr[item].c_str());
						pUnitElement->LinkEndChild(pContent);
						pTradingElement->LinkEndChild(pUnitElement);
					}
				}
				{
					TiXmlElement *pMarketDataElement = new TiXmlElement("MarketData");
					pServerElement->LinkEndChild(pMarketDataElement);
					for(int item=0;item<(int)cfgitem.QuotAddr.size();item++)
					{
						TiXmlElement *pUnitElement = new TiXmlElement("item");
						TiXmlText *pContent = new TiXmlText(cfgitem.QuotAddr[item].c_str());
						pUnitElement->LinkEndChild(pContent);
						pMarketDataElement->LinkEndChild(pUnitElement);
					}
				}
			}
			myDocument.SaveFile(strAppPath.c_str());
	}
	return bReturn;
}
void CTPLogin::SetOneInstrument(wxString& strLine)
{
	PlatformStru_InstrumentInfo Infofield;
	memset(&Infofield,0,sizeof(PlatformStru_InstrumentInfo));
	std::vector<std::string>  tokenvector; 
	SplitwxString(strLine,tokenvector);  
	if(tokenvector.size()<25) return;
	int index=0;
	strcpy(Infofield.InstrumentID,tokenvector[index++].c_str());
	strcpy(Infofield.ExchangeID,tokenvector[index++].c_str());
	strcpy(Infofield.InstrumentName,tokenvector[index++].c_str());
	strcpy(Infofield.ExchangeInstID,tokenvector[index++].c_str());
	strcpy(Infofield.ProductID,tokenvector[index++].c_str());
	Infofield.ProductClass=tokenvector[index++][0];
	Infofield.DeliveryYear=atoi(tokenvector[index++].c_str());
	Infofield.DeliveryMonth=atoi(tokenvector[index++].c_str());
	Infofield.MaxMarketOrderVolume=atoi(tokenvector[index++].c_str());
	Infofield.MinMarketOrderVolume=atoi(tokenvector[index++].c_str());
	Infofield.MaxLimitOrderVolume=atoi(tokenvector[index++].c_str());
	Infofield.MinLimitOrderVolume=atoi(tokenvector[index++].c_str());
	Infofield.VolumeMultiple=atoi(tokenvector[index++].c_str());
	Infofield.PriceTick=atof(tokenvector[index++].c_str());
	strcpy(Infofield.CreateDate,tokenvector[index++].c_str());
	strcpy(Infofield.OpenDate,tokenvector[index++].c_str());
	strcpy(Infofield.ExpireDate,tokenvector[index++].c_str());
	strcpy(Infofield.StartDelivDate,tokenvector[index++].c_str());
	strcpy(Infofield.EndDelivDate,tokenvector[index++].c_str());
	Infofield.InstLifePhase=tokenvector[index++][0];
	Infofield.IsTrading=atoi(tokenvector[index++].c_str());
	Infofield.PositionType=tokenvector[index++][0];
	Infofield.PositionDateType=tokenvector[index++][0];
	Infofield.LongMarginRatio=atof(tokenvector[index++].c_str());
	Infofield.ShortMarginRatio=atof(tokenvector[index++].c_str());

	PlatformStru_InstrumentInfo Info2;
	Info2=Infofield;
	if(m_pSvr) m_pSvr->SetInstrumentInfo(Infofield.InstrumentID,Info2);
}
bool CTPLogin::LoadTodayInstruments()
{
	if((!m_pSvr)||(!m_pSvr->HaveFileCache()))
		return m_bResultLTI;
	if(m_bfirstLTI)
	{
		m_bfirstLTI=false;
		std::string strAppPath;
		GlobalFunc::GetPath(strAppPath);
		strAppPath+=INSTRUMENT_FILENAME;
		if(wxFileExists(strAppPath.c_str()))
		{
			wxTextFile file;
			wxString strLine;
			if(file.Open(strAppPath.c_str()))
			{
				size_t linecount=file.GetLineCount();
				if(linecount>0)
				{
					wxString strAll;
					wxDateTime t_now=wxDateTime::Now();
					strAll.Printf(DATE_FORMAT,t_now.GetYear(),t_now.GetMonth(),t_now.GetDay());
					std::vector<std::string> vec;
					SplitwxString(file[0],vec);
					bool bSameDay=false,bSameServer=false;
					if(!vec.empty())bSameDay=(vec[0]==strAll.c_str());
					if(vec.size()>1&&m_pSvr)bSameServer=(atoi(vec[1].c_str())==m_pSvr->GetServerParamCfg().current);
					if(bSameDay&&bSameServer)//同一天
					{
						for (size_t i=1;i<linecount;i++)
						{
							SetOneInstrument(file[i]);
						}
						m_bResultLTI=true;
					}
				}      
			}
		}
	}
	return m_bResultLTI;
}
void CTPLogin::SaveTodayInstruments()
{
	if(!m_pSvr) return;
	std::string strAppPath;
	GlobalFunc::GetPath(strAppPath);
	strAppPath+=INSTRUMENT_FILENAME;

	std::set<std::string> tmpInstrumentID;
	tmpInstrumentID.clear();
	if(m_pSvr&&m_pSvr->GetInstrumentList(tmpInstrumentID)>0)
	{
		wxFile file;
		wxString strAll,strLine;
		if(file.Open(strAppPath.c_str(),wxFile::write))
		{
			PlatformStru_InstrumentInfo Infofield;
			wxDateTime t_now=wxDateTime::Now();
			strAll.Printf(DATE_FORMAT,t_now.GetYear(),t_now.GetMonth(),t_now.GetDay());
			strLine.Printf(",%d\r\n",m_pSvr->GetServerParamCfg().current);
			strAll+=strLine;
			for (std::set<std::string>::iterator it = tmpInstrumentID.begin(); it != tmpInstrumentID.end(); ++it) 
			{
				memset(&Infofield,0,sizeof(Infofield));
				if(m_pSvr->GetInstrumentInfo( it->c_str(), Infofield)==0)
				{
					strLine.Printf(INSTRUMENT_FORMAT_STRING,
						Infofield.InstrumentID,
						Infofield.ExchangeID,
						Infofield.InstrumentName,
						Infofield.ExchangeInstID,
						Infofield.ProductID,
						Infofield.ProductClass,
						Infofield.DeliveryYear,
						Infofield.DeliveryMonth,
						Infofield.MaxMarketOrderVolume,
						Infofield.MinMarketOrderVolume,
						Infofield.MaxLimitOrderVolume,
						Infofield.MinLimitOrderVolume,
						Infofield.VolumeMultiple,
						Infofield.PriceTick,
						Infofield.CreateDate,
						Infofield.OpenDate,
						Infofield.ExpireDate,
						Infofield.StartDelivDate,
						Infofield.EndDelivDate,
						Infofield.InstLifePhase,
						Infofield.IsTrading,
						Infofield.PositionType,
						Infofield.PositionDateType,
						Infofield.LongMarginRatio,
						Infofield.ShortMarginRatio
						);
					strAll+=strLine;
					strAll+="\r\n";
				}
			} 
			file.Write(strAll.c_str(),strAll.Length());
			file.Close();
		}

	}

}
bool CTPLogin::LoadCommissionRate(TiXmlElement* pRoot)
{
	TiXmlElement* pFieldElement=pRoot->FirstChildElement();
	PlatformStru_InstrumentCommissionRate field;
	memset(&field,0,sizeof(field));
	//获取BrokerID和InvestorID
	field.InvestorRange=THOST_FTDC_IR_Single;
	string strInvestorID,strBrokerID;
	if(m_pSvr) m_pSvr->GetBrokerIDUserID(strInvestorID,strBrokerID);
	strcpy(field.BrokerID,strBrokerID.c_str());
	strcpy(field.InvestorID,strInvestorID.c_str());

	wxString tagname,tagvalue;
	while(pFieldElement)
	{
		if(pFieldElement->GetText())
			strcpy(field.InstrumentID,pFieldElement->GetText());
		TiXmlAttribute* attributeName=pFieldElement->FirstAttribute();
		while(attributeName)
		{
			tagname=attributeName->Name()?attributeName->Name():wxEmptyString;
			if(tagname=="ORBM")       field.OpenRatioByMoney=attributeName->DoubleValue();
			else if(tagname=="ORBV")  field.OpenRatioByVolume=attributeName->DoubleValue();
			else if(tagname=="CRBM")  field.CloseRatioByMoney=attributeName->DoubleValue();
			else if(tagname=="CRBV")  field.CloseRatioByVolume=attributeName->DoubleValue();
			else if(tagname=="CTRBM")  field.CloseTodayRatioByMoney=attributeName->DoubleValue();
			else if(tagname=="CTRBV")  field.CloseTodayRatioByVolume=attributeName->DoubleValue();
			attributeName=attributeName->Next();
		}
		if(m_pSvr) m_pSvr->SetCommissionRate(field.InstrumentID,field);
		pFieldElement=pFieldElement->NextSiblingElement();
	}
	return true;
}
bool CTPLogin::LoadMarginRate(TiXmlElement* pRoot)
{
	TiXmlElement* pFieldElement=pRoot->FirstChildElement();
	PlatformStru_InstrumentMarginRate field;
	memset(&field,0,sizeof(field));
	//获取BrokerID和InvestorID
	string strInvestorID="",strBrokerID="";
	if(m_pSvr) m_pSvr->GetBrokerIDUserID(strInvestorID,strBrokerID);
	field.InvestorRange=THOST_FTDC_IR_Single;
	strcpy(field.BrokerID,strBrokerID.c_str());
	strcpy(field.InvestorID,strInvestorID.c_str());
	wxString tagname,tagvalue;
	while(pFieldElement)
	{
		if(pFieldElement->GetText())
			strcpy(field.InstrumentID,pFieldElement->GetText());
		TiXmlAttribute* attributeName=pFieldElement->FirstAttribute();
		while(attributeName)
		{
			tagname=attributeName->Name()?attributeName->Name():wxEmptyString;
			if(tagname=="LMBM")       field.LongMarginRatioByMoney=attributeName->DoubleValue();
			else if(tagname=="LMBV")  field.LongMarginRatioByVolume=attributeName->DoubleValue();
			else if(tagname=="SMBM")  field.ShortMarginRatioByMoney=attributeName->DoubleValue();
			else if(tagname=="SMBV")  field.ShortMarginRatioByVolume=attributeName->DoubleValue();
			attributeName=attributeName->Next();
		}
		if(m_pSvr) m_pSvr->SetMarginRate(field.InstrumentID,field);
		pFieldElement=pFieldElement->NextSiblingElement();
	}
	return true;
}
bool CTPLogin::LoadCommissionRateAndMarginRate(wxString strFileName)
{
	//保证只运行一次
	if(m_bfirstCRM)
	{
		m_bfirstCRM=false;
	}
	else 
		return false;
	std::string apppath=strFileName.c_str();
	GlobalFunc::GetPath(apppath);
	TiXmlDocument* pDocument=new TiXmlDocument(apppath.c_str());
	TiXmlDocument& myDocument=*pDocument; 
	bool retval=false;
	if(myDocument.LoadFile())
	{
		TiXmlElement* RootElement=myDocument.RootElement();
		if(RootElement)
		{
			wxString strDate,strRoot=RootElement->Value()?RootElement->Value():wxEmptyString;
			if(strRoot.CompareTo("Rate")==0)
			{
				const char* dateval=RootElement->Attribute("date");
				const char* idval=RootElement->Attribute("id");
				string strTradeDay="";
				if(m_pSvr) strTradeDay=m_pSvr->GetTradingDay("");
				if(!strTradeDay.empty()&&dateval&&idval)
				{
					string strInvestorID="",strBrokerID="";
					if(m_pSvr) m_pSvr->GetBrokerIDUserID(strInvestorID,strBrokerID);
					if(dateval==strTradeDay&&strInvestorID==idval)//当天，读入文件
					{
						TiXmlElement* pCommissionRateElement=RootElement->FirstChildElement();
						if(pCommissionRateElement)
						{
							if(LoadCommissionRate(pCommissionRateElement)) 
							{
								TiXmlElement* pMarginRateElement=pCommissionRateElement->NextSiblingElement();
								if(pMarginRateElement)
								{
									retval=LoadMarginRate(pMarginRateElement);
								}
							}

						}         
					}
				}
			}
		}
	}
	wxDELETE(pDocument);
	return retval;

}
BOOL CTPLogin::MakeConstractIDVarietyMap(map<string, string>& mapConstractIDVariety)
{

	set<string> InstrumentIDArray;
	InstrumentIDArray.clear();
	if(!m_pSvr||m_pSvr->GetInstrumentList(InstrumentIDArray)<=0)
		return FALSE;

	int nRet = 0;
	string strInstrumentID;
	wxString strNewID;
	string strExchangeID;
	int nCurrTenYear=0;
	int nCurrMonth=0;
	PlatformStru_InstrumentInfo tInstrumentField;
	SYSTEMTIME sysTime;

	GetLocalTime(&sysTime);
	nCurrTenYear = (sysTime.wYear%100)/10;
	nCurrMonth = sysTime.wMonth;

	// 生成郑商所4位日期及全小写容差合约
	for(set<string>::iterator it = InstrumentIDArray.begin(); 
		it != InstrumentIDArray.end(); ++it) {
			strInstrumentID = *it;
			///获取指定合约信息
			memset(&tInstrumentField,0,sizeof(tInstrumentField));
			nRet = m_pSvr->GetInstrumentInfo(strInstrumentID, tInstrumentField);
			if(nRet == 0) {
				strExchangeID = tInstrumentField.ExchangeID;
				if(strExchangeID.compare("CZCE")==0) {
					int nPos = 0;
					BOOL bIsFound = FALSE;
					while(nPos < (int)strlen(tInstrumentField.InstrumentID)) {
						if(tInstrumentField.InstrumentID[nPos]>='0' 
							&& tInstrumentField.InstrumentID[nPos]<='9') {
								bIsFound = TRUE;
								break;
						}
						nPos++;
					}
					if(bIsFound) {
						int nMonth = 0;
						char strName[16];
						char strYear[16];
						char strMonth[16];
						ZeroMemory(strName, sizeof(strName));
						ZeroMemory(strYear, sizeof(strYear));
						ZeroMemory(strMonth, sizeof(strMonth));
						strncpy(strName, tInstrumentField.InstrumentID, nPos);
						strncpy(strYear+1, tInstrumentField.InstrumentID+nPos, 1);
						strncpy(strMonth, tInstrumentField.InstrumentID+nPos+1, 2);
						nMonth = atoi(strMonth);
						if(nCurrMonth < nMonth) {
							strYear[0] = nCurrTenYear+'0';
						}
						else {
							if(strYear[1]=='9') {
								strYear[0] = nCurrTenYear+1+'0';
							}
							else {
								strYear[0] = nCurrTenYear+'0';
							}
						}
						strNewID = strName;
						strNewID += strYear;
						strNewID += strMonth;
						//strNewID.UpperCase();
						//mapConstractIDVariety[strNewID.c_str()] = strInstrumentID;
						strNewID.LowerCase();
						mapConstractIDVariety[strNewID.c_str()] = strInstrumentID;
					}
				}
			}
	}
	// 生成全小写容差合约
	for(set<string>::iterator it = InstrumentIDArray.begin(); 
		it != InstrumentIDArray.end(); ++it) {
			strInstrumentID = *it;
			strNewID = strInstrumentID;
			//strNewID.UpperCase();
			//mapConstractIDVariety[strNewID.c_str()] = strInstrumentID;
			strNewID.LowerCase();
			mapConstractIDVariety[strNewID.c_str()] = strInstrumentID;
	}

	return TRUE;
}