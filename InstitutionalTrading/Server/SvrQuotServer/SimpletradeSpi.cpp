
#include "SimpletradeSpi.h"
#include "PlatformCTPQuotImpl.h"
#include "..\\SvrDBOpr\\Interface_SvrDBOpr.h"
#include "FileOpr.h"
#include "MyXML.h"


#if 0
#define LOG_INFO(fmt, ...)  ;
#else
#define LOG_INFO(fmt, ...)  CFileOpr::getObj().writelocallogDaily("log","PlatformCTPQuot", "[%s][%d]:"fmt, __FILE__, __LINE__, __VA_ARGS__);
#endif

//登录交易CTP event
HANDLE g_hEvent = CreateEvent(NULL, true, false, NULL);
//extern bool bLoginCTPTrade;


SimpletradeSpi::SimpletradeSpi(CThostFtdcTraderApi *pUserApi) :
m_pUserApi(pUserApi) 
{
	m_vecInstruemnts.clear();
	m_mapInstruemnts.clear();
}

void SimpletradeSpi::OnFrontConnected()
{
	 memset(&m_reqUserLogin,0,sizeof(m_reqUserLogin));

	string BrokerID=CMyXML("Account.xml").GetNodeContent2("","root","BrokerID");
	strcpy(m_reqUserLogin. BrokerID, BrokerID.c_str());

	string UserID=CMyXML("Account.xml").GetNodeContent2("","root","CTPUserID");
	strcpy(m_reqUserLogin.UserID, UserID.c_str());

	string CTPPassword=CMyXML("Account.xml").GetNodeContent2("","root","CTPPassword");
	strcpy(m_reqUserLogin.Password, CTPPassword.c_str());

	// 发出登陆请求
	m_pUserApi->ReqUserLogin(&m_reqUserLogin, 0);
		LOG_INFO(" SimpletradeSpi::OnFrontConnected()，连接交易成功 发出登陆请求，m_pUserApi->ReqUserLogin;");
}
void SimpletradeSpi::OnFrontDisconnected(int nReason)
{
	// 当发生这个情况后，API会自动重新连接，客户端可不做处理
	printf("OnFrontDisconnected.\n");
	LOG_INFO(" SimpletradeSpi::OnFrontDisconnected()，连接交易失败;");
   
}
void SimpletradeSpi::OnRspUserLogin(CThostFtdcRspUserLoginField	*pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool	bIsLast)
{
	printf("OnRspUserLogin:\n");
	printf("ErrorCode=[%d], ErrorMsg=[%s]\n",
		pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
	LOG_INFO(" SimpletradeSpi::OnRspUserLogin()，ErrorCode=[%d], ErrorMsg=[%s]\n;",
		pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	m_mapInstruemnts.clear();
	if (pRspInfo->ErrorID != 0) 
	{
		// 端登失败，客户端需进行错误处理
		printf("Failed to login, errorcode=%d ,errormsg=%s,requestid=%d ,chain=%d", pRspInfo->ErrorID, pRspInfo->ErrorMsg,
			nRequestID, bIsLast);

		////通知主线程登录交易失败
		//Stru_NotifyEvent lDataEventLoginCTPFail(EventLoginCTPFail,0);
		//CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEventLoginCTPFail);


		////设置一个每1分钟触发一次的定时器 用于重新登录交易CTP 
		//if(!g_ReLoginTime)
		//    g_ReLoginTime = SetTimer(NULL,NULL,1*60*1000,(TIMERPROC)(NULL));
		SetEvent(g_hEvent);
		return;


		//if (m_mapInstruemnts.size()>0)
		//{
		//	LOG_INFO(" 已保存当日合约，返回");
		//	SetEvent(g_hEvent);
		//    return;
		//}
		//else
		//{
		//	// 再次发出登陆请求
		//	m_pUserApi->ReqUserLogin(&m_reqUserLogin, 0);
		//	LOG_INFO(" SimpletradeSpi::OnFrontConnected()，连接交易成功 再次发出登陆请求，m_pUserApi->ReqUserLogin;");
		//}
		
	}

	// 端登成功,做后续工作

		SaveTradingDay2DB(pRspUserLogin->TradingDay);
	//......
	//请求查询合约
	CThostFtdcQryInstrumentField QryInstrument;
	memset(&QryInstrument, 0, sizeof(QryInstrument));	
	m_pUserApi->ReqQryInstrument(&QryInstrument, 0);
	LOG_INFO(" SimpletradeSpi::OnRspUserLogin()，请求查询合约 ReqQryInstrument");
}
///请求查询合约响应
void SimpletradeSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//LOG_INFO(" SimpletradeSpi::OnRspQryInstrument");
	if(pInstrument)
	{
		//LOG_INFO("SimpletradeSpi::OnRspQryInstrument 查询到合约： %s\n", pInstrument->InstrumentID);
	
		m_vecInstruemnts.push_back(*pInstrument);
		m_mapInstruemnts[pInstrument->InstrumentID]=*pInstrument;
		if(pInstrument->ProductClass != THOST_FTDC_PC_Combination )
		CPlatformCTPQuotImpl::GetInstance().SubscribeMarketData( pInstrument->InstrumentID);



		if(bIsLast)
		{
			//SaveInstrument2DB(m_vecInstruemnts);

			/*std::map<std::string,std::set<SOCKET>> mapIns2Socket;
			CPlatformCTPQuotImpl::GetInstance().GetSubscribedInst2Socket(mapIns2Socket);
			vector<PlatformStru_InstrumentInfo>::iterator it = m_vecInstruemnts.begin();
			while(it != m_vecInstruemnts.end())
			{
				std::map<std::string,std::set<SOCKET>>::iterator its = find(*it);
				if (its == mapIns2Socket.end())
				{
					mapIns2Socket
				}

			}*/
			//收完合约后，SetEvent，使api实例能被释放
			SetEvent(g_hEvent);
		}
	}
}
bool SimpletradeSpi::SaveInstrument2DB(vector<PlatformStru_InstrumentInfo>& nVecInstruemnts)
{
	char strName[128];
	char strTime[128];

	int nErrorCode=CF_ERROR_SUCCESS;
	std::string szTableName = "INSTRUMENT";
	std::string lsDeleteSql = "";

	SYSTEMTIME st;
	GetSystemTime(&st);
	memset(strName, 0, sizeof(strName));
	memset(strTime, 0, sizeof(strTime));
	sprintf(strTime,"%.4d%.2d%.2d",st.wYear,st.wMonth,st.wDay);
	sprintf(strName,"%s_%.4d%.2d%.2d",szTableName.c_str(),st.wYear,st.wMonth,st.wDay);

	if (CInterface_SvrDBOpr::getObj().CreatInsTable_PreDay(strName,nErrorCode ))
	{	
	    return CInterface_SvrDBOpr::getObj().SaveInstruments(strTime,nVecInstruemnts,nErrorCode,szTableName);
	}
	else
		return false;
}
void SimpletradeSpi::GetInstrument(map<std::string,PlatformStru_InstrumentInfo>& outData)
{
	outData=m_mapInstruemnts;
}
bool SimpletradeSpi::SaveTradingDay2DB(std::string strTradingday)
{
	int nErrorCode = CF_ERROR_SUCCESS;

	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));

	/*sprintf(szBuffer, "select * from AQUOT_TRIGGER t where t.tradingday =\'%s\'", strTradingday.c_str());
	std::vector<std::vector<_variant_t>> vNode;
	if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode) || vNode.size() == 0)
	{
		memset(szBuffer, 0, sizeof(szBuffer));
		sprintf(szBuffer,"insert into AQUOT_TRIGGER values(\'%s\',sysdate)",strTradingday.c_str());
	    if (CInterface_SvrDBOpr::getObj().Excute( szBuffer,nErrorCode ))
	    {
			return true;
	    }
		else
			return false;
	    
		
	}
	else
	{      
		return true;
	}*/
	sprintf(szBuffer,"insert into AQUOT_TRIGGER values(\'%s\',sysdate)",strTradingday.c_str());
	if (CInterface_SvrDBOpr::getObj().Excute( szBuffer,nErrorCode ))
	{
		return true;
	}
	else
		return false;

}