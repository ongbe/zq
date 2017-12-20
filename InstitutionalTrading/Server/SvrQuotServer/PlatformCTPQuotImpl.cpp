
#include "PlatformCTPQuotImpl.h"
#include "PlatformCTP.h"
#include "SvrQuotServerImpl.h"
#include "FileOpr.h"
#include "tools_util.h"
#include "SimulateQuotMgr.h"
#include "MyXML.h"
#include "SimpletradeSpi.h"
#include "..\\OpenSource\\tinyxml\\tinyxml.h"
#include <cmath>


#if 0
#define LOG_INFO(fmt, ...)  ;
#else
#define LOG_INFO(fmt, ...)  CFileOpr::getObj().writelocallogDaily("log","PlatformCTPQuot", "[%s][%d]:"fmt, __FILE__, __LINE__, __VA_ARGS__);
#endif
extern HANDLE g_hEvent;

#ifdef _SIMULATE_
extern bool g_bSimulateTimeAuto;
#endif

extern bool g_bSimulateAutoMode;
extern volatile bool bLoginCTPTrade;
CPlatformCTPQuotImpl* CPlatformCTPQuotImpl::m_pInstance=NULL;

DWORD ThreadProcess(void* lpParam)
{
    CPlatformCTPQuotImpl* pInst = (CPlatformCTPQuotImpl*)lpParam;
    time_t t1,t2;
    t1=time(NULL);

    char strName[128];
    char strTime[128];

    int nErrorCode=CF_ERROR_SUCCESS;
    std::string szTableName = "AQUOTINFO";
    SYSTEMTIME st;

    while(!pInst->GetQuit())
    {
        t2=time(NULL);
        if(difftime(t2,t1)>SAVE_TO_DB_INTERVAL)//每1分钟执行一次
        {
            vector<PlatformStru_DepthMarketData> vecData;
            if(!pInst->GetQueueQuot(vecData))
			{
				t1=t2;
				Sleep(100);
				continue;
			}	

            GetSystemTime(&st);
            memset(strName, 0, sizeof(strName));
            memset(strTime, 0, sizeof(strTime));
            sprintf(strTime,"%.4d%.2d%.2d",st.wYear,st.wMonth,st.wDay);
            sprintf(strName,"%s_%.4d%.2d%.2d",szTableName.c_str(),st.wYear,st.wMonth,st.wDay);

            //CInterface_SvrDBOpr::getObj().SaveDepthMarketData(szTableName,strTime,vecData,nErrorCode);
			//LOG_INFO("实时行情入库开始 count(%d)",(int)vecData.size());
            CInterface_SvrDBOpr::getObj().SaveDepthMarketData_PerDay(strName,strTime,vecData,nErrorCode);
            //LOG_INFO("实时行情入库结束");

            t1=t2;
        }

        Sleep(500);
    }



    return 0;
}
LONGLONG cputick=0;
int updatecnt=0;
DWORD ThreadProcessKLine(void* lpParam)
{
	char strName[128];
    char strTime[128];

    int nErrorCode=CF_ERROR_SUCCESS;
    std::string szTableName = QUOTTABLENAME;
	vector<PlatformStru_DepthMarketData> vecData;
    SYSTEMTIME st;

    time_t t1,t2;
    t1=time(NULL);

	int nCount,nRemainCount;

	CPlatformCTPQuotImpl* pInst = (CPlatformCTPQuotImpl*)lpParam;
	while(!pInst->GetQuit())
	{
		nCount = nRemainCount = 0;
		PlatformStru_DepthMarketData tempData;

		// 用日KLine重算周KLine和月KLine
		pInst->RecalcWeekMonthKLine();

		while(pInst->GetQueueQuotForKLine(tempData, nRemainCount)) 
		{
			pInst->SetDataInTime(&tempData);		

			vecData.push_back(tempData);

			t2=time(NULL);
			if(difftime(t2,t1)>SAVE_TO_DB_INTERVAL && vecData.size()>0)//每5分钟执行一次
			{
				GetSystemTime(&st);
				memset(strName, 0, sizeof(strName));
				memset(strTime, 0, sizeof(strTime));
				sprintf(strTime,"%.4d%.2d%.2d",st.wYear,st.wMonth,st.wDay);
				sprintf(strName,"%s_%.4d%.2d%.2d",szTableName.c_str(),st.wYear,st.wMonth,st.wDay);

				LOG_INFO("实时行情入库开始 count(%d)",(int)vecData.size());
				CInterface_SvrDBOpr::getObj().SaveDepthMarketData_PerDay(strName,strTime,vecData,nErrorCode);
				LOG_INFO("实时行情入库结束");

				t1=t2;
				vecData.clear();
			}
			
			nCount++;
			if(nCount>=100)
				break;
		}

		Sleep(100);
	

	//CPlatformCTPQuotImpl* pInst = (CPlatformCTPQuotImpl*)lpParam;

	////int i =0;
	//DWORD dwStartTick = ::GetTickCount();

	//while(!pInst->GetQuit())
	//{
	//	DWORD dwStartTick = 0;
	//	int nCount = 0;
	//	int nRemainCount = 0;
	//	PlatformStru_DepthMarketData tempData;
	//	dwStartTick = ::GetTickCount();
	//	cputick=0;
	//	updatecnt=0;

	//	// 用日KLine重算周KLine和月KLine
	//	pInst->RecalcWeekMonthKLine();

	//	nRemainCount = 0;
	//	while(pInst->GetQueueQuotForKLine(tempData, nRemainCount)) 
	//	{
	//		pInst->SetDataInTime(&tempData);
	//		nCount++;
	//		if(nCount>=100)
	//			break;
	//		nRemainCount = 0;
	//	}
	//	LONGLONG TickFreq=CTools_Win32::MyGetCpuTickFreq();
	//	if(nCount>0) {
	//		LOG_INFO("ThreadProcessKLine() LOOP : times[%d], nCount[%d], UpdateCount[%d], RemainCount[%d]\n",
	//			(int)(cputick*1000/TickFreq), nCount, updatecnt, nRemainCount);
	//			//::GetTickCount() - dwStartTick, nCount);
	//	}
	//	Sleep(800);
	//	//i++;
	//	//if (i>100000)
	//	//{
	//	//	Sleep(10);
	//	//	i ==0;
	//	//}
		
#if 0
		if(::GetTickCount() - dwStartTick > 2*60*1000) {
			int lError = 0;
			KLineQuery query = {0};

			strcpy(query.InstrumentID, "IF1501");
			query.nDataType = 2;
			CSvrQuotServerImpl::getObj().QueryKlineData(&query,lError,-1,0);

			strcpy(query.InstrumentID, "IF1501");
			query.nDataType = 1;
			query.nCycle = PHRASE_1MIN;
			query.szStartTime	= 1419560340;
			query.szEndTime		= 1419577080;
			CSvrQuotServerImpl::getObj().QueryKlineData(&query,lError,-1,0);

			query.nCycle = PHRASE_5MIN;
			query.szStartTime	= 1419560400;
			query.szEndTime		= 1419576600;
			CSvrQuotServerImpl::getObj().QueryKlineData(&query,lError,-1,0);

			query.nCycle = PHRASE_15MIN;
			query.szStartTime	= 1419561000;
			query.szEndTime		= 1419575400;
			CSvrQuotServerImpl::getObj().QueryKlineData(&query,lError,-1,0);

			query.nCycle = PHRASE_30MIN;
			query.szStartTime	= 1419561900;
			query.szEndTime		= 1419576300;
			CSvrQuotServerImpl::getObj().QueryKlineData(&query,lError,-1,0);

			query.nCycle = PHRASE_DAY;
			query.szStartTime	= 1419560280;
			query.szEndTime		= 1419574819;
			CSvrQuotServerImpl::getObj().QueryKlineData(&query,lError,-1,0);

			dwStartTick -= 1000;
		}
#endif
	}

	return 0;

}

CPlatformCTPQuotImpl& CPlatformCTPQuotImpl::GetInstance(void)
{
    g_mutexImpl.lock();
    if(!m_pInstance)
        m_pInstance=new CPlatformCTPQuotImpl();
    g_mutexImpl.unlock();

    return *m_pInstance;
}

void CPlatformCTPQuotImpl::DeleteInstance()
{
    g_mutexImpl.lock();
    if(NULL != m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }	
    g_mutexImpl.unlock();
}

CPlatformCTPQuotImpl::CPlatformCTPQuotImpl(const Stru_CTPConnectionParam& ConnParam,bool bPushCache,bool bUseComb,bool bCalcuCombQuotOnLegQuot)
:CPlatformCTPQuot(ConnParam,bPushCache,bUseComb,bCalcuCombQuotOnLegQuot)
,m_nQuit(0),m_hThread(NULL),m_idThread(0)
{
	m_bHasRecalcWeekMonthKLine = false;
	m_nTimeSpan = 10*60;

    //创建行情入库工作线程 
    //m_hThread=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)ThreadProcess,this,0,&m_idThread);
	//创建K线计算线程 
	//m_hThreadKLine=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)ThreadProcessKLine,this,0,&m_idThreadKLine);

	//从配置文件中读取用户名密码,交易所交易时间
	GetCfgInfo();

	m_LoginQuotTime =0;


	
	
    /*
    //Test
    std::set<SOCKET> sset;
    sset.insert(1);
    m_SubscribedInstrumentID2Socket["IF1401"]=sset;
    m_SubscribedInstrumentID2Socket["IF1402"]=sset;
    m_SubscribedInstrumentID2Socket["IF1403"]=sset;
    m_SubscribedInstrumentID2Socket["IF1406"]=sset;
    m_SubscribedInstrumentID2Socket["TF1403"]=sset;
    m_SubscribedInstrumentID2Socket["TF1406"]=sset;
    m_SubscribedInstrumentID2Socket["TF1409"]=sset;
    m_SubscribedInstrumentID2Socket["TF1406"]=sset;
    m_SubscribedInstrumentID2Socket["TF1406"]=sset;

    bool bEnter = false;
    if(!bEnter)
    {
        //开启线程从数据库读取每分钟的行情数据
        static bool bCreate = false;
        if(!bCreate)
        {   
            LOG_INFO("模拟行情服务器启动");
            SetSimulate(1);

            m_hLoadQuotThread=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)LoadQuotThreadProcess,this,0,&m_idLoadQuotThread);
            bCreate = true;
        }    

        time_t t2;
        static time_t t1=time(NULL);   
        t2=time(NULL);

        if(difftime(t2,t1)>1*60)
        {
            LOG_INFO("模拟行情服务器停止");
            SetSimulate(0);
            bEnter = true;
        }
    }
    */
}

CPlatformCTPQuotImpl::~CPlatformCTPQuotImpl(void)
{
    SetQuit(1);    

    //等待线程退出
    DWORD ExitCode;
    WaitForSingleObject((HANDLE)m_hThread,8000);
    if(GetExitCodeThread((HANDLE)m_hThread,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
        TerminateThread((HANDLE)m_hThread,0);
    CloseHandle(m_hThread);

    m_hThread=NULL;
    m_idThread=0;

	WaitForSingleObject((HANDLE)m_hThreadKLine,8000);
	if(GetExitCodeThread((HANDLE)m_hThreadKLine,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
		TerminateThread((HANDLE)m_hThreadKLine,0);
	CloseHandle(m_hThreadKLine);

	m_hThreadKLine=NULL;
	m_idThreadKLine=0;

    Clear();
}

void CPlatformCTPQuotImpl::Clear()
{
    CeasymutexGuard guard(m_mutexImpl);

    set<SOCKET> setSocketAll;
    map<string, set<SOCKET>>::iterator it=m_SubscribedInstrumentID2Socket.begin();
    for (;it!=m_SubscribedInstrumentID2Socket.end();it++)
    {
        set<SOCKET> setSocket = it->second;
        for(std::set<SOCKET>::const_iterator it_socket=setSocket.begin();it_socket!=setSocket.end();it_socket++)
            setSocketAll.insert(*it_socket);
    }

	map<string, set<SOCKET>>::iterator it2=m_AppSubscribedInstrumentID2Socket.begin();
	for (;it2!=m_AppSubscribedInstrumentID2Socket.end();it2++)
	{
		set<SOCKET> setSocket2 = it2->second;
		for(std::set<SOCKET>::const_iterator it_socket2=setSocket2.begin();it_socket2!=setSocket2.end();it_socket2++)
			setSocketAll.insert(*it_socket2);
	}

    for(std::set<SOCKET>::const_iterator it_socketall=setSocketAll.begin();it_socketall!=setSocketAll.end();it_socketall++)
    {
        closesocket(*it_socketall);
        LOG_INFO("Clear()，关闭socket %d",*it_socketall);
    }

    m_SubscribedInstrumentID2Socket.clear();
	m_AppSubscribedInstrumentID2Socket.clear();

    m_SendedSocket2Inst.clear();
	m_ValidSockets.clear();
	m_vecUsers.clear();
}
void CPlatformCTPQuotImpl::split(const std::string& s, char c, std::vector<std::string>& v) 
{
	string::size_type i = 0;
	string::size_type j = s.find(c);
	if(j == -1 && s.length() != 0)
	{
		v.push_back(s);
		return;
	}
	while (j != string::npos) 
	{
		v.push_back(s.substr(i, j-i));
		i = ++j;
		j = s.find(c, j);

		if (j == string::npos)
			v.push_back(s.substr(i, s.length( )));
	}
}
void CPlatformCTPQuotImpl::GetCfgInfo()
{

	CMyXML myxml("Account.xml");

	//用户名和密码
	vector<string> users =myxml.GetNodeContent3("root","Login","UserName");
	for(int i=0;i<(int)users.size();i++)
	{
		sLoginReq loginName;
		memset(&loginName,0,sizeof(loginName));
		//sscanf(users[i].c_str(), "%s-%s", loginName.msUserName, loginName.msPassWord);
		string strName(users[i]);
		std::size_t pos = users[i].find("-");  
		strName.erase(pos);
		std::string strPassword =users[i].substr(pos+1);
		strcpy(loginName.msUserName,strName.c_str());
		strcpy(loginName.msPassWord,strPassword.c_str());
		m_vecUsers.push_back(loginName);
	}

	//日，周、月K线存储间隔
	m_nTimeSpan = CMyXML("Account.xml").GetNodeContentInt2(3600,"root","Save2DBTimeSpan");

	

}
void CPlatformCTPQuotImpl::SetQuit(int nVal)
{
    int lnQuit = nVal;
    InterlockedExchange((volatile long*)(&m_nQuit),lnQuit);
}
bool CPlatformCTPQuotImpl::GetQuit()
{
	int lnQuit = 0;
	InterlockedExchange((volatile long*)(&lnQuit),m_nQuit);
	return lnQuit != 0;
}
bool CPlatformCTPQuotImpl::GetQueueQuot(std::vector<PlatformStru_DepthMarketData>& vecData)
{
    CeasymutexGuard guard(m_mutexImpl);

    while (!m_queueQuot.empty())
    {                
        PlatformStru_DepthMarketData targetData = m_queueQuot.front();
        m_queueQuot.pop();

        vecData.push_back(targetData);
    }

	return (vecData.size()>0)?true:false;
}
bool CPlatformCTPQuotImpl::GetQueueQuotForKLine( PlatformStru_DepthMarketData& outData, int& nRemainCount)
{
	//CeasymutexGuard guard(m_mutexKLine);
	CeasymutexGuard guard(m_mutexImpl);

	if(!m_queueForKline.empty())
	{                
		outData = m_queueForKline.front();
		m_queueForKline.pop();
		nRemainCount = (int)m_queueForKline.size();
		return true;

	} 
	return false;
}
void CPlatformCTPQuotImpl::GetSubscribedInst2Socket(std::map<std::string,std::set<SOCKET>>& vec)
{
	CeasymutexGuard guard(m_mutexImpl);
	
	vec = m_SubscribedInstrumentID2Socket;
}
void CPlatformCTPQuotImpl::SetSubscribedInst2Socket(std::map<std::string,std::set<SOCKET>>& vec)
{
    CeasymutexGuard guard(m_mutexImpl);

    m_SubscribedInstrumentID2Socket = vec;
}
void CPlatformCTPQuotImpl::GetAppSubscribedInst2Socket(std::map<std::string,std::set<SOCKET>>& vec)
{
	CeasymutexGuard guard(m_mutexImpl);

	vec = m_AppSubscribedInstrumentID2Socket;
}
void CPlatformCTPQuotImpl::SetAppSubscribedInst2Socket(std::map<std::string,std::set<SOCKET>>& vec)
{
	CeasymutexGuard guard(m_mutexImpl);

	m_AppSubscribedInstrumentID2Socket = vec;
}
void CPlatformCTPQuotImpl::GetCTPConnectParam(BrokerInfo& brokerInfo,std::map<std::string, std::vector<std::string>>& mapBrokerCode2AddrParam)
{
    brokerInfo = m_brokerInfo;
    mapBrokerCode2AddrParam = m_mapBrokerCode2AddrParam;
}
void CPlatformCTPQuotImpl::SetCTPConnectParam(BrokerInfo& brokerInfo,std::map<std::string, std::vector<std::string>>& mapBrokerCode2AddrParam)
{
    m_brokerInfo = brokerInfo;
    m_mapBrokerCode2AddrParam = mapBrokerCode2AddrParam;
}

bool CPlatformCTPQuotImpl::ReInitCtpConnect()
{  
    CeasymutexGuard guard(m_mutexImpl);
    LOG_INFO("=========》ReInitCtpConnect\n");

    //登录CTP
    for (std::map<std::string, std::vector<std::string>>::iterator it=m_mapBrokerCode2AddrParam.begin();it!=m_mapBrokerCode2AddrParam.end();it++)
    {
        Stru_CTPConnectionParam oParam(it->second,it->first,m_brokerInfo.szAccount,m_brokerInfo.szPassword);
        m_ConnParam = oParam;
        Login();

        return true;
    }

	return false;
}
int CPlatformCTPQuotImpl::SubscribeClientMarketData(const string& InstrumentID,int hSocket)
{
    CeasymutexGuard guard(m_mutexImpl);
    SubscribeMarketData(InstrumentID);

    int ret = -999; 
    //内存中如果有就先推送
#ifdef _SIMULATE_
    if(g_bSimulateTimeAuto)
    {
        PlatformStru_DepthMarketData data;
        if(GetLastQuot(InstrumentID,data))//模拟行情数据
        {
            CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_QuotEvent_Push,(void*)&data,sizeof(PlatformStru_DepthMarketData),0,0,CF_ERROR_SUCCESS);			
            LOG_INFO("模拟内存中有，直接发送->%d %s",hSocket,InstrumentID.c_str());
        }
    }
    else
    {
        map<string,CThostFtdcDepthMarketDataField> lastQuot; 
        GetLastQuotMap(lastQuot); 
        map<string,CThostFtdcDepthMarketDataField>::const_iterator it_data = lastQuot.find((string)InstrumentID);
        if( it_data!=lastQuot.end() )
        {
            PlatformStru_DepthMarketData data;
            CopyDepthMarketData( data, it_data->second );

            CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_QuotEvent_Push,(void*)&data,sizeof(PlatformStru_DepthMarketData),0,0,CF_ERROR_SUCCESS);			
            LOG_INFO("内存中有，直接发送->%d %s",hSocket,InstrumentID.c_str());
        }
    }
#else    
    if(!g_bSimulateAutoMode)//手动
    {
        PlatformStru_DepthMarketData data;
        if(GetLastQuot(InstrumentID,data))//模拟行情数据
        {
            CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_QuotEvent_Push,(void*)&data,sizeof(PlatformStru_DepthMarketData),0,0,CF_ERROR_SUCCESS);			
            LOG_INFO("模拟内存中有，直接发送->%d %s",hSocket,InstrumentID.c_str());
        }
    }
    else//自动
    {
        map<string,CThostFtdcDepthMarketDataField> lastQuot; 
        GetLastQuotMap(lastQuot); //实时行情数据
        map<string,CThostFtdcDepthMarketDataField>::const_iterator it_data = lastQuot.find((string)InstrumentID);
        if( it_data!=lastQuot.end() )
        {
            PlatformStru_DepthMarketData data;
            CopyDepthMarketData( data, it_data->second );

            CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_QuotEvent_Push,(void*)&data,sizeof(PlatformStru_DepthMarketData),0,0,CF_ERROR_SUCCESS);			
            LOG_INFO("内存中有，直接发送->%d %s",hSocket,InstrumentID.c_str());
        }
    }
#endif

    return ret;
}
int CPlatformCTPQuotImpl::PushLastQuot(const string& InstrumentID,int hSocket)
{
	
	map<string,CThostFtdcDepthMarketDataField> lastQuot; 
	GetLastQuotMap(lastQuot); //实时行情数据
	map<string,CThostFtdcDepthMarketDataField>::const_iterator it_data = lastQuot.find((string)InstrumentID);
	if( it_data!=lastQuot.end() )
	{
		PlatformStru_DepthMarketData data;
		CopyDepthMarketData( data, it_data->second );

		SimpleDepthMarketData quot={0};
		strncpy(quot.TradingDay,data.TradingDay,sizeof(quot.TradingDay)-1);
		strncpy(quot.InstrumentID,data.InstrumentID,sizeof(quot.InstrumentID)-1);
		quot.LastPrice =data.LastPrice;
		quot.OpenPrice = data.OpenPrice;
		quot.HighestPrice =data.HighestPrice;
		quot.LowestPrice = data.LowestPrice;
		quot.ClosePrice =data.ClosePrice;
		quot.Turnover =data.Turnover;
		quot.OpenInterest =data.OpenInterest;
		quot.PreSettlementPrice =data.PreSettlementPrice;
		quot.Volume =data.Volume;
		strncpy(quot.UpdateTime,data.UpdateTime,sizeof(quot.UpdateTime)-1);
		quot.BidPrice1 =data.BidPrice1;
		quot.BidVolume1 =data.BidVolume1;
		quot.AskPrice1 =data.AskPrice1;
		quot.AskVolume1 =data.AskVolume1;

		CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_SubscribeQuotEvent2_Rsp,(void*)&quot,sizeof(SimpleDepthMarketData),0,0,CF_ERROR_SUCCESS);			
		LOG_INFO("用户订阅行情后发送内存中最后一条行情，直接发送->%d %s",hSocket,InstrumentID.c_str());
	}
	return 0;
}
int CPlatformCTPQuotImpl::UnSubscribeAppClientMarketData(const string& InstrumentID, int hSocket)
{
	LOG_INFO("->退订行情");

	CeasymutexGuard guard(m_mutexImpl);
	//UnSubscribeMarketData(InstrumentID);

	int ret = -999;
	std::map<std::string,std::set<SOCKET>>::iterator it_inst=m_AppSubscribedInstrumentID2Socket.find(InstrumentID);
	if(it_inst!=m_AppSubscribedInstrumentID2Socket.end())
	{      
		string szInstrumentID = (it_inst->first);

		std::set<SOCKET>& sset = it_inst->second;            
		for(std::set<SOCKET>::iterator it_socket = sset.begin();it_socket!=sset.end();)
		{
			if (hSocket == (*it_socket))
			{     
				sset.erase(it_socket);    
				CThostFtdcSpecificInstrumentField field;
				strncpy(field.InstrumentID,InstrumentID.c_str(),sizeof(field.InstrumentID));
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_UnSubscribeQuotEvent2_Rsp,(void*)&field,sizeof(CThostFtdcSpecificInstrumentField),0,0,CF_ERROR_SUCCESS);			
				LOG_INFO("发送退订行情返回->%d %s",hSocket,InstrumentID.c_str());
				break;
			}
			else
			{
				++it_socket;
			}
		}

		if(sset.empty())
		{
			m_AppSubscribedInstrumentID2Socket.erase(it_inst);
			LOG_INFO("->Socket列表为空,删除合约 %s",InstrumentID.c_str());
		}
	}
	else
	{
        CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_UnSubscribeQuotEvent2_Rsp,NULL,0,0,0,CF_ERROR_NO_INSTRUMENTS);			
		LOG_INFO("CPlatformCTPQuotImpl::UnSubscribeAppClientMarketData 退订失败->%d %s",hSocket,InstrumentID.c_str());
	}

	return ret;
}
int CPlatformCTPQuotImpl::UnSubscribeClientMarketData(const string& InstrumentID, int hSocket)
{
    LOG_INFO("->退订行情");

    CeasymutexGuard guard(m_mutexImpl);
	//UnSubscribeMarketData(InstrumentID);

    int ret = -999;
    std::map<std::string,std::set<SOCKET>>::iterator it_inst=m_SubscribedInstrumentID2Socket.find(InstrumentID);
    if(it_inst!=m_SubscribedInstrumentID2Socket.end())
    {      
        string szInstrumentID = (it_inst->first);

        std::set<SOCKET>& sset = it_inst->second;            
        for(std::set<SOCKET>::iterator it_socket = sset.begin();it_socket!=sset.end();it_socket++)
        {
            if (hSocket == (*it_socket))
            {     
                sset.erase(it_socket);    
                CThostFtdcSpecificInstrumentField field;
                strncpy(field.InstrumentID,InstrumentID.c_str(),sizeof(field.InstrumentID));
                CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_UnSubscribeQuotEvent_Rsp,(void*)&field,sizeof(CThostFtdcSpecificInstrumentField),0,0,CF_ERROR_SUCCESS);			
                LOG_INFO("发送退订行情返回->%d %s",hSocket,InstrumentID.c_str());
                break;
            }
        }

        if(sset.empty())
        {
            m_SubscribedInstrumentID2Socket.erase(it_inst);
            LOG_INFO("->Socket列表为空,删除合约 %s",InstrumentID.c_str());
        }
    }

    return ret;
}

///登录请求响应
void CPlatformCTPQuotImpl::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{   
    CPlatformCTPQuot::OnRspUserLogin(pRspUserLogin, pRspInfo, nRequestID, bIsLast);

    //此处还需订阅的情况：服务与CTP断开连接期间，有客户端与服务连接并订阅合约（与CPlatformCTPQuot::OnRspUserLogin中的不同）时
    CeasymutexGuard guard(m_mutexImpl);
	std::map<std::string,std::set<SOCKET>>::iterator it_inst=m_SubscribedInstrumentID2Socket.begin();
	for(;it_inst!=m_SubscribedInstrumentID2Socket.end();it_inst++)
	{
		char *pInstrumentID = const_cast<char*>(it_inst->first.c_str());
		SubscribeMarketData(pInstrumentID);
    }  

	if(pRspUserLogin&&(!pRspInfo||pRspInfo->ErrorID==0))
	{
		m_LoginQuotTime = time(NULL);

	}
	else
	{
        m_LoginQuotTime = 0;
		LOG_INFO(" CPlatformCTPQuotImpl::OnRspUserLogin,m_LoginQuotTime = 0");

	}
	bLoginCTPTrade=false;
	LOG_INFO(" CPlatformCTPQuotImpl::OnRspUserLogin,bLoginCTPTrade=%s",bLoginCTPTrade==FALSE?"FALSE":"TRUE");

//     //创建每日行情表
// 	int nErrorCode=CF_ERROR_SUCCESS;
// 	std::string szTableName = "AQUOTINFO";
// 	char strName[128];
// 	memset(strName,0,sizeof(strName)-1);
// 
// 	if ( strlen(pRspUserLogin->TradingDay) != 0 )
// 	{	
// 		sprintf(strName,"%s_%s",szTableName.c_str(),pRspUserLogin->TradingDay);
// 
// 	    //CInterface_SvrDBOpr::getObj().CreatDepthMarketTable_PerDay(strName);
// 
// 		//创建每日K线表
// 		//CInterface_SvrDBOpr::getObj().CreatKLineTable_PerDay(pRspUserLogin->TradingDay);
// 	}
 
 	m_strTradingDay=pRspUserLogin->TradingDay;
 	if(m_strTradingDay.empty())
 		return;
 
 	m_tmCloseTime.tm_year = atoi(m_strTradingDay.substr(0, 4).c_str()) - 1900;
 	m_tmCloseTime.tm_mon  = atoi(m_strTradingDay.substr(4, 2).c_str()) - 1;
 	m_tmCloseTime.tm_mday = atoi(m_strTradingDay.substr(6, 2).c_str());
 	m_tmCloseTime.tm_hour = 15;
 	m_tmCloseTime.tm_min  = 0;
 	m_tmCloseTime.tm_sec  = 0;	
 
 	//交易时间
 
 	int nPathLen = 0;
 	char localPath[256];
 	memset(localPath, 0, 256);
 	nPathLen = GetModuleFileName( NULL, localPath, 256);
 	string filename=localPath;
 	size_t splitpos=filename.find_last_of('\\');
 	string strPath(filename.substr(0, splitpos+1).c_str());
 	strPath += "ExchangeTradingTime.xml";
 
 	TiXmlDocument* pXmlDocment = new TiXmlDocument(strPath.c_str());
 	if(pXmlDocment == NULL) 
 		return;
 
 	std::auto_ptr<TiXmlDocument> ptr( pXmlDocment );
 	if( !pXmlDocment->LoadFile() ) 
 		return;
 
 	TiXmlElement *root = pXmlDocment->RootElement();
 	if ( root == NULL ) 
 		return;
 
 	if( std::string(root->Value()) != "root")
 		return;
 
 
 	//各交易所交易时间配置文件
 
 	TiXmlElement *tList = root->FirstChildElement("tradingtime");
 	if(tList == NULL)
 		return;
 
 	//int nStartTime = 0, nEndTime = 0, nSubNum = 0;
 	//int nHour = 0, nMinite = 0;
 	//EXCHANGETIME subTime;
 
 	for(TiXmlElement *GroupItem = tList->FirstChildElement("group");
 		GroupItem; 
 		GroupItem = GroupItem->NextSiblingElement("group"))
 	{
 
 		const char *pGroupid = GroupItem->Attribute("id");
 		if(!pGroupid) 
 			continue;
 
 		string strID = pGroupid;
 		const char *pGrouptime = GroupItem->Attribute("time");
 		if(!pGrouptime)
 			continue;
 
 		vector<struct tm> vecRestTime;
 		string str = pGrouptime;
 		vector<string> vecTime;
 		split(str, ' ', vecTime);
 		for(int i = 0; i < (int)vecTime.size(); i++)
 		{
 			string strTime = vecTime[i];
 			vector<string> vecOne;
 			split(strTime, '-', vecOne);
 			if(vecOne.size() != 2)
 				break;
 
 			std::string strBegin = vecOne[0];
 			std::string strEnd   = vecOne[1];	
 
 			struct tm tmScale;
 			tmScale.tm_year = atoi(m_strTradingDay.substr(0, 4).c_str()) - 1900;
 			tmScale.tm_mon  = atoi(m_strTradingDay.substr(4, 2).c_str()) - 1;
 			tmScale.tm_mday = atoi(m_strTradingDay.substr(6, 2).c_str());
 			tmScale.tm_hour = atoi(strBegin.substr(0, 2).c_str());
 			tmScale.tm_min  = atoi(strBegin.substr(3, 2).c_str());
 			tmScale.tm_sec  = 0;		
 			vecRestTime.push_back(tmScale);
 
 			tmScale.tm_hour = atoi(strEnd.substr(0, 2).c_str());
 			tmScale.tm_min  = atoi(strEnd.substr(3, 2).c_str());
 			tmScale.tm_sec  = 0;		
 			vecRestTime.push_back(tmScale);
 		}
 		m_mapExchangeTime.insert(std::make_pair(strID,  vecRestTime));			
 	}
	


    
	//////登录CTP，取所有合约并订阅行情

	//CThostFtdcTraderApi *pUserApi =CThostFtdcTraderApi::CreateFtdcTraderApi();
	//SimpletradeSpi sh(pUserApi);
	//pUserApi->RegisterSpi(&sh);
	//LOG_INFO(" CPlatformCTPQuotImpl::OnRspUserLogin，登录交易 pUserApi->RegisterSpi(&sh);");

	//// 设置交易托管系统服务的地址，可以注册多个地址备用
	//string ServerAddr=string("tcp://")+CMyXML("Account.xml").GetNodeContent2("","root","CTPTradeServerAddress");

	//pUserApi->RegisterFront((char*)ServerAddr.c_str());
	////pUserApi->RegisterFront("tcp://58.246.40.180:26205");

	//// 使客户端开始与后台服务建立连接
	//pUserApi->Init();
	//LOG_INFO(" CPlatformCTPQuotImpl::OnRspUserLogin，登录交易 pUserApi->Init();");
	//// 客户端等待查询合约操作完成 
	//WaitForSingleObject(g_hEvent, INFINITE);
	////// 释放API实例
	////pUserApi->Release();
	////LOG_INFO(" CPlatformCTPQuotImpl::OnRspUserLogin，登录交易 pUserApi->Release();");

	//sh.GetInstrument(m_mapIns);
	//if (m_mapIns.size()>0)
	//{
	//	LOG_INFO(" 已保存当日合约");
	//	bLoginCTPTrade = true;
	//	SetIns(m_mapIns);
	//}
	//else
	//{

	//	bLoginCTPTrade = false;
	//}
}
void CPlatformCTPQuotImpl::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutexImpl);

    std::map<std::string,std::set<SOCKET>>::iterator it=m_SubscribedInstrumentID2Socket.find(pSpecificInstrument->InstrumentID);
    if(it!=m_SubscribedInstrumentID2Socket.end())
    {
        std::set<SOCKET> sset=it->second;
        for(std::set<SOCKET>::const_iterator it_socket=sset.begin();it_socket!=sset.end();it_socket++)
        {
            //服务器与CTP断开又成功登陆上订阅合约返回后，避免重复发送给所有的socke
            map<SOCKET, set<string>>::iterator it_sended = m_SendedSocket2Inst.find(*it_socket);            
            if(it_sended != m_SendedSocket2Inst.end())
            {
                set<string> vSet = it_sended->second;
                set<string>::iterator it_inst = vSet.find(pSpecificInstrument->InstrumentID);
                if(it_inst != vSet.end())
                    break;//如果找到先前此合约已经发送过的socket，就不用再往下执行了。
                else
                {
                    set<string>& vSet = it_sended->second;
                    vSet.insert(pSpecificInstrument->InstrumentID);
                }
            }
            else
            {
                set<string> vSet;
                vSet.insert(pSpecificInstrument->InstrumentID);
                m_SendedSocket2Inst.insert(make_pair(*it_socket,vSet));
            }

            if(pSpecificInstrument&&(!pRspInfo||pRspInfo->ErrorID==0))
            {
                //LOG_INFO("------>>>给第三方发送订阅行情返回,Instrument===%s hSocket=%d",pSpecificInstrument->InstrumentID,*it_socket);
                CInterface_SvrTcp::getObj().SendPkgData(*it_socket, Cmd_SubscribeQuotEvent_Rsp,(void*)pSpecificInstrument,sizeof(CThostFtdcSpecificInstrumentField),0,0,CF_ERROR_SUCCESS);			
            }
            else
            {
                //LOG_INFO("------>>>给第三方发送订阅行情返回,Instrument===%s hSocket=%d",pSpecificInstrument->InstrumentID,*it_socket);
                CInterface_SvrTcp::getObj().SendPkgData(*it_socket, Cmd_SubscribeQuotEvent_Rsp,(void*)pSpecificInstrument,sizeof(CThostFtdcSpecificInstrumentField),0,0,CF_ERROR_DATA_QUOT_SUBSCRIBE);
            }
        }
    }
    
    CPlatformCTPQuot::OnRspSubMarketData(pSpecificInstrument,pRspInfo,nRequestID,bIsLast);
}

vector<string> CPlatformCTPQuotImpl::GetSubscribedInstrumentIDs(void) const
{
    CeasymutexGuard guard(m_mutexImpl);

    return CPlatformCTPQuot::GetSubscribedInstrumentIDs();
}
void CPlatformCTPQuotImpl::SetExchangeID(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	map<std::string,PlatformStru_InstrumentInfo>::iterator it =   m_mapIns.find(pDepthMarketData->InstrumentID);
	if ( it!= m_mapIns.end())
	{
		strncpy(pDepthMarketData->ExchangeID,it->second.ExchangeID,sizeof(pDepthMarketData->ExchangeID)-1);
	}
	/*else
	{

		strncpy(pDepthMarketData->ExchangeID,"CFFEX",sizeof(pDepthMarketData->ExchangeID)-1);
	}*/
}

bool CPlatformCTPQuotImpl::SaveSettlementPrice2DB(CThostFtdcDepthMarketDataField *pDepthMarketData)
{

	int nErrorCode = CF_ERROR_SUCCESS;

	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));

	sprintf(szBuffer, "select * from AQUOT_SETTLEPRICE t where t.tradingday ='%s' and t.InstrumentID ='%s'", pDepthMarketData->TradingDay,pDepthMarketData->InstrumentID);
	std::vector<std::vector<_variant_t>> vNode;
	if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode) || vNode.size() == 0)
	{
		memset(szBuffer, 0, sizeof(szBuffer));
		sprintf(szBuffer,"insert into AQUOT_SETTLEPRICE values('%s','%s','%s', %f,sysdate)",pDepthMarketData->TradingDay,pDepthMarketData->InstrumentID,
			pDepthMarketData->UpdateTime,pDepthMarketData->SettlementPrice);
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
	}
	
}
bool CPlatformCTPQuotImpl::SaveVolatility2DB(Volatility* pVolatility)
{
	int nErrorCode = CF_ERROR_SUCCESS;

	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));

	sprintf(szBuffer, "select * from VOLATILITY_HISTORY t where t.tradingday ='%s' and t.InstrumentID ='%s'", pVolatility->TradingDay,pVolatility->InstrumentID);
	std::vector<std::vector<_variant_t>> vNode;
	if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode) || vNode.size() == 0)
	{
		memset(szBuffer, 0, sizeof(szBuffer));
		sprintf(szBuffer,"insert into VOLATILITY_HISTORY values('%s','%s',%f, %f,sysdate)",pVolatility->TradingDay,pVolatility->InstrumentID,
			pVolatility->VolatilityTwenty,pVolatility->VolatilitySixty);
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
	}
}
void CPlatformCTPQuotImpl::VolatilityCalc(std::string strInstrumentID,string strTradingday)
{
	int nErrorCode = CF_ERROR_SUCCESS;

	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	if (strTradingday.empty())
	    return;
	std::map< int,SettlementPriceCalc> mapSettlementPriceCalc;

	sprintf(szBuffer, "select * from (select * from AQUOT_SETTLEPRICE t where t.instrumentid = '%s' order by t.TRADINGDAY DESC) where rownum <= 61 ",strInstrumentID.c_str());
	std::vector<std::vector<_variant_t>> vNode;
	if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode) || vNode.size() == 0)
	{
		return ;
	}
	else
	{
		for ( UINT j = 0; j < vNode.size(); j++ )
		{
			SettlementPriceCalc temp;
			memset(&temp,0,sizeof(temp));

			std::vector<_variant_t> vValue = vNode[j];
			int i = 0;
			strcpy(temp.TradingDay, vValue[i++].operator _bstr_t());	
			strcpy(temp.InstrumentID, vValue[i++].operator _bstr_t());
			i++;
			temp.SettlementPrice= vValue[i++].dblVal;


			mapSettlementPriceCalc [j]=temp;

		}
	} 

	int nCount = mapSettlementPriceCalc.size()-1;

	//计算相邻时间段的价格变化值
	double sumtwenty=0.0 ,sumsixty=0.0,avetwenty=0.0,avesixty=0.0;

	for ( int i = 0; i < nCount; i++ )
	{

		mapSettlementPriceCalc[i].PriceChange = log(mapSettlementPriceCalc[i+1].SettlementPrice) -log(mapSettlementPriceCalc[i].SettlementPrice);

		if ( i<20)
		{
			sumtwenty += mapSettlementPriceCalc[i].PriceChange;
		}
		sumsixty += mapSettlementPriceCalc[i].PriceChange;
	}

   //	计算均值μ
	if ( nCount< 21 && nCount>0)
	{
		avetwenty =sumtwenty/nCount;
		avesixty = sumsixty/nCount;
	}
	else if( nCount >= 21 && nCount< 61)
	{
		avetwenty =sumtwenty/20;
		avesixty = sumsixty/nCount;
	}
	else if(nCount>= 61)
	{

		avetwenty =sumtwenty/20;
		avesixty = sumsixty/60;
	}
	
	//计算价格变化值的标准差σ
	sumtwenty=0.0;
	sumsixty=0.0;
	double dbVolatility20 = 0.0;
	double dbVolatility60 = 0.0;

	for ( int i = 0; i < nCount; i++ )
	{

		mapSettlementPriceCalc[i].StandardDeviation =pow((mapSettlementPriceCalc[i].PriceChange -avetwenty) ,2);

			if ( i<20)
			{
				sumtwenty += mapSettlementPriceCalc[i].StandardDeviation;
			}
			sumsixty += mapSettlementPriceCalc[i].StandardDeviation;
	}
	if( nCount< 21 && nCount>0)
	{
		dbVolatility20 =sqrt(sumtwenty/nCount) * sqrt((long double)250);
		dbVolatility60 = sqrt(sumsixty/nCount) * sqrt((long double)250);
	}
	else if( nCount >= 21 && nCount< 61)
	{
		dbVolatility20 =sqrt(sumtwenty/20) * sqrt((long double)250);
		dbVolatility60 = sqrt(sumsixty/nCount) * sqrt((long double)250);
	}
	else if( nCount>= 61)
	{

		dbVolatility20 =sqrt(sumtwenty/20) * sqrt((long double)250);
		dbVolatility60 = sqrt(sumsixty/60) * sqrt((long double)250);
	}

	Volatility vol;
	memset(&vol,0,sizeof(vol));
	strncpy(vol.InstrumentID,strInstrumentID.c_str(),sizeof(vol.InstrumentID));
	strncpy(vol.TradingDay,strTradingday.c_str(),sizeof(vol.TradingDay));
	vol.VolatilityTwenty =dbVolatility20;
	vol.VolatilitySixty =dbVolatility60;

	SaveVolatility2DB(&vol);

	return ;
}
void CPlatformCTPQuotImpl::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{    		


	//m_mutexKLine.lock();
	//m_mutexKLine.unlock();

    if(m_nQuit)
        return;

	CeasymutexGuard guard(m_mutexImpl);
    CPlatformCTPQuot::OnRtnDepthMarketData(pDepthMarketData);

	SetExchangeID(pDepthMarketData);
	strncpy(pDepthMarketData->TradingDay,m_strTradingDay.c_str(),sizeof(pDepthMarketData->TradingDay)-1);

    CThostFtdcDepthMarketDataField MD=*pDepthMarketData;
    PlatformStru_DepthMarketData data;
    CopyDepthMarketData( data, MD );

	m_queueQuot.push(data);
	m_queueForKline.push(data);


#ifdef _SIMULATE_
    //如果模拟行情启动时不再将当前的行情发送
    if(g_bSimulateTimeAuto)
    {
        if(util::isInvalidValue(pDepthMarketData->SettlementPrice))
            return;
        else//结算价要发送给报盘
        {
            std::map<std::string,std::set<SOCKET>>::iterator it=m_SubscribedInstrumentID2Socket.find(MD.InstrumentID);
            if(it!=m_SubscribedInstrumentID2Socket.end())
            {
                std::set<SOCKET> sset=it->second;
                for(std::set<SOCKET>::const_iterator it_socket=sset.begin();it_socket!=sset.end();it_socket++)
                {
                    CInterface_SvrTcp::getObj().SendPkgData(*it_socket, Cmd_QuotEvent_Push,(void*)&data,sizeof(PlatformStru_DepthMarketData),0,0,CF_ERROR_SUCCESS);			
                    LOG_INFO("发送->%d %s",*it_socket,data.InstrumentID);
                }
            }
        }
    }
#endif

    //如果是手动模式
    if(!g_bSimulateAutoMode)
        return;

    ///发送给订阅行情的用户
    std::map<std::string,std::set<SOCKET>>::iterator it=m_SubscribedInstrumentID2Socket.find(MD.InstrumentID);
    if(it!=m_SubscribedInstrumentID2Socket.end())
    {
        std::set<SOCKET> sset=it->second;
        for(std::set<SOCKET>::const_iterator it_socket=sset.begin();it_socket!=sset.end();it_socket++)
        {
            CInterface_SvrTcp::getObj().SendPkgData(*it_socket, Cmd_QuotEvent_Push,(void*)&data,sizeof(PlatformStru_DepthMarketData),0,0,CF_ERROR_SUCCESS);			
			LOG_INFO("发送->%d %s %s %.4f",*it_socket,data.InstrumentID,data.UpdateTime,data.LastPrice);
        }
    }
	///发送给订阅行情的app用户
	std::map<std::string,std::set<SOCKET>>::iterator it2=m_AppSubscribedInstrumentID2Socket.find(MD.InstrumentID);
	if(it2!=m_AppSubscribedInstrumentID2Socket.end())
	{
		std::set<SOCKET> sset=it2->second;
		for(std::set<SOCKET>::const_iterator it_socket2=sset.begin();it_socket2!=sset.end();it_socket2++)
		{
			SimpleDepthMarketData quot={0};
			strncpy(quot.TradingDay,data.TradingDay,sizeof(quot.TradingDay)-1);
			strncpy(quot.InstrumentID,data.InstrumentID,sizeof(quot.InstrumentID)-1);
			quot.LastPrice =data.LastPrice;
			quot.OpenPrice = data.OpenPrice;
			quot.HighestPrice =data.HighestPrice;
			quot.LowestPrice = data.LowestPrice;
			quot.ClosePrice =data.ClosePrice;
			quot.Turnover =data.Turnover;
			quot.OpenInterest =data.OpenInterest;
			quot.PreSettlementPrice =data.PreSettlementPrice;
			quot.Volume =data.Volume;
			strncpy(quot.UpdateTime,data.UpdateTime,sizeof(quot.UpdateTime)-1);
			quot.BidPrice1 =data.BidPrice1;
			quot.BidVolume1 =data.BidVolume1;
			quot.AskPrice1 =data.AskPrice1;
			quot.AskVolume1 =data.AskVolume1;

			CInterface_SvrTcp::getObj().SendPkgData(*it_socket2, Cmd_QuotEvent_Push,(void*)&quot,sizeof(SimpleDepthMarketData),0,0,CF_ERROR_SUCCESS);			
			LOG_INFO("APP发送->%d %s",*it_socket2,data.InstrumentID);
		}
	}

	//波动率计算
	if( !util::isInvalidValue(pDepthMarketData->SettlementPrice ))
	{
		if (SaveSettlementPrice2DB(pDepthMarketData))
		{
			VolatilityCalc(pDepthMarketData->InstrumentID,pDepthMarketData->TradingDay);
		}		
	}
}
void CPlatformCTPQuotImpl::DeleteClientSocketFromMap(int nSocket)
{
    CeasymutexGuard guard(m_mutexImpl);  
    LOG_INFO("->断开连接");

    std::map<std::string,std::set<SOCKET>>::iterator it_inst=m_SubscribedInstrumentID2Socket.begin();
    while(it_inst!=m_SubscribedInstrumentID2Socket.end())
    {       
        string szInstrumentID = (it_inst->first);
        std::set<SOCKET>& sset = it_inst->second; 
        for(std::set<SOCKET>::iterator it_socket = sset.begin();it_socket!=sset.end();it_socket++)
        {
            if (nSocket == (*it_socket))
            {     
                sset.erase(it_socket); 
                LOG_INFO("->删除套接字 %d",nSocket);
                break;
            }
        }  

        if(sset.empty())
        {
            it_inst = m_SubscribedInstrumentID2Socket.erase(it_inst);
            LOG_INFO("->Socket列表为空,删除合约 %s",szInstrumentID.c_str());
        }
        else
            it_inst++;
    }
	std::map<std::string,std::set<SOCKET>>::iterator it_inst2=m_AppSubscribedInstrumentID2Socket.begin();
	while(it_inst2!=m_AppSubscribedInstrumentID2Socket.end())
	{       
		string szInstrumentID = (it_inst2->first);
		std::set<SOCKET>& sset = it_inst2->second; 
		for(std::set<SOCKET>::iterator it_socket = sset.begin();it_socket!=sset.end();it_socket++)
		{
			if (nSocket == (*it_socket))
			{     
				sset.erase(it_socket); 
				LOG_INFO("->APP 删除套接字 %d",nSocket);
				break;
			}
		}  

		if(sset.empty())
		{
			it_inst2 = m_AppSubscribedInstrumentID2Socket.erase(it_inst2);
			LOG_INFO("->APP Socket列表为空,删除合约 %s",szInstrumentID.c_str());
		}
		else
			it_inst2++;
	}

    map<SOCKET, set<string>>::iterator it_sended = m_SendedSocket2Inst.find(nSocket);            
    if(it_sended != m_SendedSocket2Inst.end())
        m_SendedSocket2Inst.erase(it_sended);


	std::set<SOCKET>::iterator it;
	if((it = m_ValidSockets.find(nSocket)) != m_ValidSockets.end())
	{
		m_ValidSockets.erase(nSocket);
	}

}

void CPlatformCTPQuotImpl::CopyDepthMarketData( PlatformStru_DepthMarketData& data, const CThostFtdcDepthMarketDataField& depthMarketData )
{
    strcpy(data.TradingDay, depthMarketData.TradingDay);
    strcpy(data.InstrumentID, depthMarketData.InstrumentID);
    strcpy(data.ExchangeID, depthMarketData.ExchangeID);
    strcpy(data.ExchangeInstID, depthMarketData.ExchangeInstID);
    data.LastPrice = depthMarketData.LastPrice;
    data.PreSettlementPrice = depthMarketData.PreSettlementPrice;
    data.PreClosePrice = depthMarketData.PreClosePrice;
    data.PreOpenInterest = depthMarketData.PreOpenInterest;
    data.OpenPrice = depthMarketData.OpenPrice;
    data.HighestPrice = depthMarketData.HighestPrice;
    data.LowestPrice = depthMarketData.LowestPrice;
    data.Volume = depthMarketData.Volume;
    data.Turnover = depthMarketData.Turnover;
    data.OpenInterest = depthMarketData.OpenInterest;
    data.ClosePrice = depthMarketData.ClosePrice;
    data.SettlementPrice = depthMarketData.SettlementPrice;
    data.UpperLimitPrice = depthMarketData.UpperLimitPrice;
    data.LowerLimitPrice = depthMarketData.LowerLimitPrice;
    data.PreDelta = depthMarketData.PreDelta;
    data.CurrDelta = depthMarketData.CurrDelta;
    strcpy(data.UpdateTime, depthMarketData.UpdateTime);
    data.UpdateMillisec = depthMarketData.UpdateMillisec;
    data.BidPrice1 = depthMarketData.BidPrice1;
    data.BidVolume1 = depthMarketData.BidVolume1;
    data.AskPrice1 = depthMarketData.AskPrice1;
    data.AskVolume1 = depthMarketData.AskVolume1;
    data.BidPrice2 = depthMarketData.BidPrice2;
    data.BidVolume2 = depthMarketData.BidVolume2;
    data.AskPrice2 = depthMarketData.AskPrice2;
    data.AskVolume2 = depthMarketData.AskVolume2;
    data.BidPrice3 = depthMarketData.BidPrice3;
    data.BidVolume3 = depthMarketData.BidVolume3;
    data.AskPrice3 = depthMarketData.AskPrice3;
    data.AskVolume3 = depthMarketData.AskVolume3;
    data.BidPrice4 = depthMarketData.BidPrice4;
    data.BidVolume4 = depthMarketData.BidVolume4;
    data.AskPrice4 = depthMarketData.AskPrice4;
    data.AskVolume4 = depthMarketData.AskVolume4;
    data.BidPrice5 = depthMarketData.BidPrice5;
    data.BidVolume5 = depthMarketData.BidVolume5;
    data.AskPrice5 = depthMarketData.AskPrice5;
    data.AskVolume5 = depthMarketData.AskVolume5;
    data.AveragePrice = depthMarketData.AveragePrice;
}


///模拟深度行情通知
void CPlatformCTPQuotImpl::OnRtnDepthMarketDataSimulate(PlatformStru_DepthMarketData *pDepthMarketData)
{
    CeasymutexGuard guard(m_mutexImpl);

    ///发送给订阅行情的用户
    std::map<std::string,std::set<SOCKET>>::iterator it=m_SubscribedInstrumentID2Socket.find(pDepthMarketData->InstrumentID);
    if(it!=m_SubscribedInstrumentID2Socket.end())
    {
        std::set<SOCKET> sset=it->second;
        for(std::set<SOCKET>::const_iterator it_socket=sset.begin();it_socket!=sset.end();it_socket++)
        {
            CInterface_SvrTcp::getObj().SendPkgData(*it_socket, Cmd_QuotEvent_Push,(void*)pDepthMarketData,sizeof(PlatformStru_DepthMarketData),0,0,CF_ERROR_SUCCESS);			
            LOG_INFO("模拟发送->%d %s",*it_socket,pDepthMarketData->InstrumentID);
        }
    }
	std::map<std::string,std::set<SOCKET>>::iterator it2=m_AppSubscribedInstrumentID2Socket.find(pDepthMarketData->InstrumentID);
	if(it2!=m_AppSubscribedInstrumentID2Socket.end())
	{
		std::set<SOCKET> sset=it2->second;
		for(std::set<SOCKET>::const_iterator it_socket2=sset.begin();it_socket2!=sset.end();it_socket2++)
		{
			SimpleDepthMarketData quot={0};
			strncpy(quot.TradingDay,pDepthMarketData->TradingDay,sizeof(quot.TradingDay)-1);
			strncpy(quot.InstrumentID,pDepthMarketData->InstrumentID,sizeof(quot.InstrumentID)-1);
			quot.LastPrice =pDepthMarketData->LastPrice;
			quot.OpenPrice = pDepthMarketData->OpenPrice;
			quot.HighestPrice =pDepthMarketData->HighestPrice;
			quot.LowestPrice = pDepthMarketData->LowestPrice;
			quot.ClosePrice =pDepthMarketData->ClosePrice;
			quot.Turnover =pDepthMarketData->Turnover;
			quot.OpenInterest =pDepthMarketData->OpenInterest;
			quot.PreSettlementPrice =pDepthMarketData->PreSettlementPrice;
			quot.Volume =pDepthMarketData->Volume;
			strncpy(quot.UpdateTime,pDepthMarketData->UpdateTime,sizeof(quot.UpdateTime)-1);
			quot.BidPrice1 =pDepthMarketData->BidPrice1;
			quot.BidVolume1 =pDepthMarketData->BidVolume1;
			quot.AskPrice1 =pDepthMarketData->AskPrice1;
			quot.AskVolume1 =pDepthMarketData->AskVolume1;

			CInterface_SvrTcp::getObj().SendPkgData(*it_socket2, Cmd_QuotEvent_Push,(void*)&quot,sizeof(SimpleDepthMarketData),0,0,CF_ERROR_SUCCESS);			
			LOG_INFO("APP发送->%d %s",*it_socket2,pDepthMarketData->InstrumentID);
		}
	}
}

void CPlatformCTPQuotImpl::SetLastQuot(const PlatformStru_DepthMarketData& data)
{
    CeasymutexGuard guard(m_mutexImpl);

    map<string,PlatformStru_DepthMarketData>::iterator it = m_lastQuotImpl.find(string(data.InstrumentID));
    m_lastQuotImpl[string(data.InstrumentID)] = data;
}

bool CPlatformCTPQuotImpl::GetLastQuot(const string& InstrumentID, PlatformStru_DepthMarketData& outData)
{
    CeasymutexGuard guard(m_mutexImpl);

    map<string,PlatformStru_DepthMarketData>::iterator it =  m_lastQuotImpl.find(InstrumentID);
    if(it==m_lastQuotImpl.end())
    {
        memset(&outData,0,sizeof(outData));
        return false;
    }
    else
    {
        outData = it->second;
        return true;
    }

    return false;
}

int CPlatformCTPQuotImpl::IsValidUser(const std::string& strAccout, const std::string& strPwd)
{
	 CeasymutexGuard guard(m_mutexImpl);

	bool bAccountFind = false;
	bool bPwdMatch = false;
	std::vector<sLoginReq>::iterator it = m_vecUsers.begin();
	for ( ; it != m_vecUsers.end(); it++)
	{
		if ( strAccout == it->msUserName )
		{
			bAccountFind = true;
			if ( strPwd == it->msPassWord )
			{
				bPwdMatch = true;
				
				break;
			}
		}
	}

	if (!bAccountFind)
		return CF_ERROR_LOGIN_USER_NOT_EXIST;
	else
	{
		if(!bPwdMatch)
			return CF_ERROR_LOGIN_PASS_INVALIDATE;
		else
			return CF_ERROR_SUCCESS;
	}
}
void CPlatformCTPQuotImpl::SetValidSocket(int hSocket)
{
	CeasymutexGuard guard(m_mutexImpl);

	m_ValidSockets.insert(hSocket);
}
bool CPlatformCTPQuotImpl::IsValidSocket( int hSocket)
{
	CeasymutexGuard guard(m_mutexImpl);
	std::set<SOCKET>::iterator it;
	if((it = m_ValidSockets.find(hSocket)) != m_ValidSockets.end())
		return true;
	else
		return false;
}
bool CPlatformCTPQuotImpl::IsValidInstrument( std::string strIns)
{
	CeasymutexGuard guard(m_mutexImpl);
	map<std::string,PlatformStru_InstrumentInfo>::iterator it=   m_mapIns.find(strIns);
	if(it != m_mapIns.end())
		return true;
	else
		return false;
}
void CPlatformCTPQuotImpl::SetIns(map<std::string,PlatformStru_InstrumentInfo>& data)
{
	CeasymutexGuard guard(m_mutexImpl);
	m_mapIns =data;
}
void CPlatformCTPQuotImpl::OnFrontDisconnected(int nReason)
{ 
	CeasymutexGuard guard(m_mutexImpl);
     CPlatformCTPQuot::OnFrontDisconnected(nReason);
	 
	 LOG_INFO("CPlatformCTPQuotImpl::OnFrontDisconnected(与交易所失去TCP连接 )");
}
time_t CPlatformCTPQuotImpl::GetLoginQuotTime()
{
	CeasymutexGuard guard(m_mutexImpl);
	return m_LoginQuotTime;
}
//CThostFtdcTraderApi& CPlatformCTPQuotImpl::GetSHSpi()
//{
//	CeasymutexGuard guard(m_mutexImpl);
//	return *m_pUserApi;
//}

void CPlatformCTPQuotImpl::SetDataInTime(PlatformStru_DepthMarketData* pData)
{

	std::string strDate = m_strTradingDay;//如果行情里面没有日期，照样不能解决跨日期的问题
	std::string strTime		= pData->UpdateTime;
	std::string strFutureID = pData->InstrumentID;
	if(util::isInvalidValue(pData->LastPrice)) 		
	{		
		return;//无效值
	}	

	if(util::isInvalidValue(pData->AskPrice1))
		pData->AskPrice1 = 0;
	if(util::isInvalidValue(pData->BidPrice1))
		pData->BidPrice1 = 0;
	if(util::isInvalidValue(pData->Volume))
		pData->Volume = 0;
	if(util::isInvalidValue(pData->Turnover))
		pData->Turnover = 0;
	if(util::isInvalidValue(pData->OpenInterest))
		pData->OpenInterest = 0;
	if(strDate.length() != 8)
	{		
		return;
	}
	
	double dcClose = pData->PreSettlementPrice;
	if(dcClose <0.00001)
		dcClose = pData->OpenPrice;//如果没有昨结算，则用今日开盘价
	
	
	//SetInTimeData_TLine(pData);	
	SetInTimeData_KLine(pData);
}

BOOL CPlatformCTPQuotImpl::SetInTimeData_KLine(PlatformStru_DepthMarketData* pData)
{	
	
	if(m_mapExchangeTime.size() == 0)
	{			
		return false;
	}




	std::string strTime		= pData->UpdateTime;
	std::string strFutureID = pData->InstrumentID;
	struct tm tempTime = {0};

	KLineBase pKLine;
	//strncpy(pKLine.InstrumentID,pData->InstrumentID,sizeof(pKLine.InstrumentID)-1);
	tempTime.tm_year	= atoi(m_strTradingDay.substr(0, 4).c_str())-1900;
	tempTime.tm_mon	= atoi(m_strTradingDay.substr(4, 2).c_str())-1;
	tempTime.tm_mday	= atoi(m_strTradingDay.substr(6, 2).c_str());
	tempTime.tm_hour	= atoi(strTime.substr(0, 2).c_str());
	tempTime.tm_min	= atoi(strTime.substr(3, 2).c_str());
	tempTime.tm_sec	= atoi(strTime.substr(6, 2).c_str());
	//pKLine.dwTime.tm_wday	= 0;
	//pKLine.dwTime.tm_yday	= 0;
	pKLine.dwTime			= mktime(&tempTime);
	pKLine.dwOpenInterest	= pData->OpenInterest;
	pKLine.dwVolume			= pData->Volume;
	pKLine.fClosePrice		= pData->LastPrice;
	pKLine.fHighPrice		= pData->HighestPrice;
	pKLine.fLowPrice		= pData->LowestPrice;
	pKLine.fOpenPrice		= pData->OpenPrice;
	pKLine.fTurnover		= pData->Turnover;
	//if( !util::isInvalidValue(pData->SettlementPrice))
	//{
	//	pKLine.dJieSuan = pData->SettlementPrice;
	//}
	

	SetInTimeData_DiffSpan(string(pData->InstrumentID), pKLine, PHRASE_1MIN,pData->ExchangeID);	
    SetInTimeData_DiffSpan(string(pData->InstrumentID), pKLine, PHRASE_5MIN,pData->ExchangeID);	
	SetInTimeData_DiffSpan(string(pData->InstrumentID), pKLine, PHRASE_15MIN,pData->ExchangeID);
	SetInTimeData_DiffSpan(string(pData->InstrumentID), pKLine, PHRASE_30MIN,pData->ExchangeID);	
	SetInTimeData_DiffSpan(string(pData->InstrumentID), pKLine, PHRASE_60MIN,pData->ExchangeID);	
	SetInTimeData_DiffSpan(string(pData->InstrumentID), pKLine, PHRASE_DAY,pData->ExchangeID);
	SetInTimeData_DiffSpan(string(pData->InstrumentID), pKLine, PHRASE_WEEK,pData->ExchangeID);
	SetInTimeData_DiffSpan(string(pData->InstrumentID), pKLine, PHRASE_MONTH,pData->ExchangeID);

	m_mapKLineLast[strFutureID] = pKLine;
	return true;
}
BOOL CPlatformCTPQuotImpl::SaveKline2Cache(string& strInstrumentID, KLineBase& pKLine, EnumPhrase enumPhrase)
{

	switch(enumPhrase)
	{
	case PHRASE_1MIN:
		{
			CeasymutexGuard guard(m_mutexKLineM1);
			std::map<std::string,map<time_t,KLineBase>>::iterator it = m_vmapKLineMin1.find(strInstrumentID);
			if(it!=m_vmapKLineMin1.end())
			{
				map<time_t,KLineBase>& mapTime2SKLine = it->second;
				mapTime2SKLine[pKLine.dwTime]=pKLine;
			}
			else
			{
				map<time_t,KLineBase> mapTemp;
				mapTemp.insert(std::make_pair(pKLine.dwTime,pKLine));
				m_vmapKLineMin1.insert(std::make_pair(strInstrumentID,mapTemp));
			}

			break;
		}			
	case PHRASE_5MIN:
		{
			CeasymutexGuard guard(m_mutexKLineM5);
			std::map<std::string,map<time_t,KLineBase>>::iterator it = m_vmapKLineMin5.find(strInstrumentID);
			if(it!=m_vmapKLineMin5.end())
			{
				map<time_t,KLineBase>& mapTime2SKLine = it->second;
				mapTime2SKLine[pKLine.dwTime]=pKLine;
			}
			else
			{
				map<time_t,KLineBase> mapTemp;
				mapTemp.insert(std::make_pair(pKLine.dwTime,pKLine));
				m_vmapKLineMin5.insert(std::make_pair(strInstrumentID,mapTemp));
			}

			break;
		}
	case PHRASE_15MIN:
		{
			CeasymutexGuard guard(m_mutexKLineM15);
			std::map<std::string,map<time_t,KLineBase>>::iterator it = m_vmapKLineMin15.find(strInstrumentID);
			if(it!=m_vmapKLineMin15.end())
			{
				map<time_t,KLineBase>& mapTime2SKLine = it->second;
				mapTime2SKLine[pKLine.dwTime]=pKLine;
			}
			else
			{
				map<time_t,KLineBase> mapTemp;
				mapTemp.insert(std::make_pair(pKLine.dwTime,pKLine));
				m_vmapKLineMin15.insert(std::make_pair(strInstrumentID,mapTemp));
			}

			break;
		}
	case PHRASE_30MIN:
		{
			CeasymutexGuard guard(m_mutexKLineM30);
			std::map<std::string,map<time_t,KLineBase>>::iterator it = m_vmapKLineMin30.find(strInstrumentID);
			if(it!=m_vmapKLineMin30.end())
			{
				map<time_t,KLineBase>& mapTime2SKLine = it->second;
				mapTime2SKLine[pKLine.dwTime]=pKLine;
			}
			else
			{
				map<time_t,KLineBase> mapTemp;
				mapTemp.insert(std::make_pair(pKLine.dwTime,pKLine));
				m_vmapKLineMin30.insert(std::make_pair(strInstrumentID,mapTemp));
			}

			break;
		}
	case PHRASE_60MIN:
		{
			CeasymutexGuard guard(m_mutexKLineM60);
			std::map<std::string,map<time_t,KLineBase>>::iterator it = m_vmapKLineMin60.find(strInstrumentID);
			if(it!=m_vmapKLineMin60.end())
			{
				map<time_t,KLineBase>& mapTime2SKLine = it->second;
				mapTime2SKLine[pKLine.dwTime]=pKLine;
			}
			else
			{
				map<time_t,KLineBase> mapTemp;
				mapTemp.insert(std::make_pair(pKLine.dwTime,pKLine));
				m_vmapKLineMin60.insert(std::make_pair(strInstrumentID,mapTemp));
			}

			break;
		}
	case PHRASE_DAY:
		{
			CeasymutexGuard guard(m_mutexKLineDay);
			std::map<std::string,map<time_t,KLineBase>>::iterator it = m_vmapKLineDay.find(strInstrumentID);
			if(it!=m_vmapKLineDay.end())
			{
				map<time_t,KLineBase>& mapTime2SKLine = it->second;
				mapTime2SKLine[pKLine.dwTime]=pKLine;
			}
			else
			{
				map<time_t,KLineBase> mapTemp;
				mapTemp.insert(std::make_pair(pKLine.dwTime,pKLine));
				m_vmapKLineDay.insert(std::make_pair(strInstrumentID,mapTemp));
			}

			break;
		}
	case PHRASE_WEEK:
		{
			CeasymutexGuard guard(m_mutexKLineWeek);
			std::map<std::string,map<time_t,KLineBase>>::iterator it = m_vmapKLineWeek.find(strInstrumentID);
			if(it!=m_vmapKLineWeek.end())
			{
				map<time_t,KLineBase>& mapTime2SKLine = it->second;
				mapTime2SKLine[pKLine.dwTime]=pKLine;
			}
			else
			{
				map<time_t,KLineBase> mapTemp;
				mapTemp.insert(std::make_pair(pKLine.dwTime,pKLine));
				m_vmapKLineWeek.insert(std::make_pair(strInstrumentID,mapTemp));
			}

			break;
		}
	case PHRASE_MONTH:
		{
			CeasymutexGuard guard(m_mutexKLineMonth);
			std::map<std::string,map<time_t,KLineBase>>::iterator it = m_vmapKLineMonth.find(strInstrumentID);
			if(it!=m_vmapKLineMonth.end())
			{
				map<time_t,KLineBase>& mapTime2SKLine = it->second;
				mapTime2SKLine[pKLine.dwTime]=pKLine;
			}
			else
			{
				map<time_t,KLineBase> mapTemp;
				mapTemp.insert(std::make_pair(pKLine.dwTime,pKLine));
				m_vmapKLineMonth.insert(std::make_pair(strInstrumentID,mapTemp));
			}

			break;
		}
	}

	return true;
}
BOOL CPlatformCTPQuotImpl::SaveKline2DB(string& strInstrumentID, KLineBase& pKLine, EnumPhrase enumPhrase, std::string& exchangeID)
{

	int nErrorCode=CF_ERROR_SUCCESS;
	std::string lsDeleteSql = "";
	std::string szTableName = "";

	char strName[64];
	memset(strName,0,sizeof(strName));

	if (exchangeID.empty())
		return false;
	std::map<std::string,vector<struct tm>> ::iterator itTime=m_mapExchangeTime.find(exchangeID);
	if (itTime == m_mapExchangeTime.end())
		return true;

	KLineBase pTemp = pKLine;
	switch(enumPhrase)
	{
	case PHRASE_1MIN:
		{			
			map<std::string,KLineBase >::iterator itLast = m_maptmLast1.find(strInstrumentID);
			if (itLast == m_maptmLast1.end() ) {
				m_maptmLast1[strInstrumentID] = pKLine;
				UpdateKLineData(strInstrumentID,PHRASE_1MIN,pKLine,true,nErrorCode);
				return true;
			}

			if( difftime(pKLine.dwTime, mktime(&(m_tmCloseTime ))) >=0 )
				UpdateKLineData(strInstrumentID,PHRASE_1MIN,pKLine,false,nErrorCode);
			else {
				//if( itLast->second.dwTime.tm_year == pKLine.dwTime.tm_year && itLast->second.dwTime.tm_mon == pKLine.dwTime.tm_mon && itLast->second.dwTime.tm_mday == pKLine.dwTime.tm_mday
				//	&& itLast->second.dwTime.tm_hour == pKLine.dwTime.tm_hour && itLast->second.dwTime.tm_min == pKLine.dwTime.tm_min )
				//if( itLast->second.dwTime/60 == pKLine.dwTime/60 ) {
				if( difftime(pKLine.dwTime, itLast->second.dwTime) == 0) {
					//UpdateKLineData(strName,pKLine,nErrorCode,false,itLast->second);
				}
				else {
					UpdateKLineData(strInstrumentID,PHRASE_1MIN,itLast->second,true,nErrorCode);
				}
			}
			m_maptmLast1[strInstrumentID] = pKLine;
		}
		break;
	case PHRASE_5MIN:
		{
			map<std::string,KLineBase >::iterator itLast = m_maptmLast5.find(strInstrumentID);
			if (itLast == m_maptmLast5.end()) {
				m_maptmLast5[strInstrumentID] = pKLine;
				UpdateKLineData(strInstrumentID,PHRASE_5MIN,pKLine,true,nErrorCode);
				return true;
			}			

			if( difftime(pKLine.dwTime, mktime(&(m_tmCloseTime)) ) >=0 ) {
				UpdateKLineData(strInstrumentID,PHRASE_5MIN,pKLine,false,nErrorCode);
			}
			else {
				if( difftime(pKLine.dwTime, itLast->second.dwTime) == 0) {
					//UpdateKLineData(strName,pKLine,nErrorCode,false,itLast->second);
				}
				else {
					UpdateKLineData(strInstrumentID,PHRASE_5MIN,itLast->second,true,nErrorCode);
				}
			}
			m_maptmLast5[strInstrumentID] = pKLine;
		}
		break;
	case PHRASE_15MIN:
		{
			map<std::string,KLineBase >::iterator itLast = m_maptmLast15.find(strInstrumentID);
			if (itLast == m_maptmLast15.end() ) {
				m_maptmLast15[strInstrumentID] = pKLine;
				UpdateKLineData(strInstrumentID,PHRASE_15MIN,pKLine,true,nErrorCode);
				return true;
			}

			if( difftime(pKLine.dwTime, mktime(&(m_tmCloseTime))) >=0 ) {
				UpdateKLineData(strInstrumentID,PHRASE_15MIN,pKLine,false,nErrorCode);
			}
			else {
				if( difftime(pKLine.dwTime, itLast->second.dwTime) == 0) {
					//UpdateKLineData(strName,pKLine,nErrorCode,itLast->second);
				}
				else {
					UpdateKLineData(strInstrumentID,PHRASE_15MIN,itLast->second,true,nErrorCode);
				}
			}
			m_maptmLast15[strInstrumentID] = pKLine;
		}
		break;
	case PHRASE_30MIN:
		{
			map<std::string,KLineBase >::iterator itLast = m_maptmLast30.find(strInstrumentID);
			if (itLast == m_maptmLast30.end()) {
				m_maptmLast30[strInstrumentID] = pKLine; 
				UpdateKLineData(strInstrumentID,PHRASE_30MIN,pKLine,true,nErrorCode);
				return true;
			}

			if( difftime(pKLine.dwTime, mktime(&(m_tmCloseTime) )) >=0 ) {
				UpdateKLineData(strInstrumentID,PHRASE_30MIN,pKLine,false,nErrorCode);
			}
			else {
				if( difftime(pKLine.dwTime, itLast->second.dwTime) == 0) {
					//UpdateKLineData(strName,pKLine,nErrorCode,itLast->second);
				}
				else {
					UpdateKLineData(strInstrumentID,PHRASE_30MIN,itLast->second,true,nErrorCode);
				}
			}
			m_maptmLast30[strInstrumentID] = pKLine;
		}
		break;
	case PHRASE_60MIN:
		{
			map<std::string,KLineBase >::iterator itLast = m_maptmLast60.find(strInstrumentID);
			if (itLast == m_maptmLast60.end()) {
				m_maptmLast60[strInstrumentID] = pKLine;
				UpdateKLineData(strInstrumentID,PHRASE_60MIN,pKLine,true,nErrorCode);
				return true;
			}

			if( difftime(pKLine.dwTime,  mktime(&(m_tmCloseTime) )) >=0 )
				UpdateKLineData(strInstrumentID,PHRASE_60MIN,pKLine,false,nErrorCode);
			else {
				if( difftime(pKLine.dwTime, itLast->second.dwTime) == 0) {
					//UpdateKLineData(strName,pKLine,nErrorCode,itLast->second);
				}
				else {
					UpdateKLineData(strInstrumentID,PHRASE_60MIN,itLast->second,true,nErrorCode);
				}
			}
			m_maptmLast60[strInstrumentID] = pKLine;
		}
		break;
	case PHRASE_DAY:
		{
		   map<std::string,KLineBase >::iterator itLast = m_maptmLastDay.find(strInstrumentID);
		   if(itLast != m_maptmLastDay.end() && itLast->second.dwTime/(24*3600) == pKLine.dwTime/(24*3600)) {
			   if(pKLine.dwTime - mktime(&m_LastDBTimeDay) > m_nTimeSpan ) {
			       UpdateKLineData(strInstrumentID,PHRASE_DAY,pKLine,false,nErrorCode); 
			       m_LastDBTimeDay = *localtime(&pKLine.dwTime);
			   }
		   }
		   else {
			   UpdateKLineData(strInstrumentID,PHRASE_DAY,pKLine,true,nErrorCode);
			   m_LastDBTimeDay = *localtime(&pKLine.dwTime);
		   }
		   m_maptmLastDay[strInstrumentID] = pKLine;
		}
		break;
	case PHRASE_WEEK:
		{
			map<std::string,KLineBase >::iterator itLast = m_maptmLastWeek.find(strInstrumentID);
			if(itLast != m_maptmLastWeek.end()) {
				struct tm timeCurr = {0};
				struct tm timeLast = {0};
				timeLast = *localtime(&itLast->second.dwTime);
				timeCurr = *localtime(&pKLine.dwTime);
				if(IsOneWeek(timeLast, timeCurr)) {
					if(pKLine.dwTime - mktime(&m_LastDBTimeWeek) > m_nTimeSpan ) {
						UpdateKLineData(strInstrumentID,PHRASE_WEEK,pKLine,false,nErrorCode);
						m_LastDBTimeWeek = *localtime(&pKLine.dwTime);
					}
				}
				else {
					UpdateKLineData(strInstrumentID,PHRASE_WEEK,pKLine,true,nErrorCode);
					m_LastDBTimeWeek = *localtime(&pKLine.dwTime);
				}
			}
			else {
				UpdateKLineData(strInstrumentID,PHRASE_WEEK,pKLine,true,nErrorCode);
				m_LastDBTimeWeek = *localtime(&pKLine.dwTime);
			}
			m_maptmLastWeek[strInstrumentID] = pKLine;
		}	
		break;
	case PHRASE_MONTH:
		{
			map<std::string,KLineBase >::iterator itLast = m_maptmLastMonth.find(strInstrumentID);
			if(itLast != m_maptmLastMonth.end()) {
				struct tm timeCurr = {0};
				struct tm timeLast = {0};
				timeLast = *localtime(&itLast->second.dwTime);
				timeCurr = *localtime(&pKLine.dwTime);
				if(IsOneMonth(timeLast, timeCurr)) {
					if(pKLine.dwTime - mktime(&m_LastDBTimeMonth) > m_nTimeSpan ) {
						UpdateKLineData(strInstrumentID,PHRASE_MONTH,pKLine,false,nErrorCode);
						m_LastDBTimeMonth = *localtime(&pKLine.dwTime);
					}
				}
				else {
					UpdateKLineData(strInstrumentID,PHRASE_MONTH,pKLine,true,nErrorCode);
					m_LastDBTimeMonth = *localtime(&pKLine.dwTime);
				}
			}
			else {
				UpdateKLineData(strInstrumentID,PHRASE_MONTH,pKLine,true,nErrorCode);
				m_LastDBTimeMonth = *localtime(&pKLine.dwTime);
			}
			m_maptmLastMonth[strInstrumentID] = pKLine;
		}	
		break;
	}

	return true;
}

std::string CPlatformCTPQuotImpl::MakeTableName(string& strInstrumentID, EnumPhrase phrase)
{
	char szTableName[64];

	memset(szTableName, 0, sizeof(szTableName));

	if(phrase==PHRASE_1MIN || phrase==PHRASE_5MIN || phrase==PHRASE_15MIN || phrase==PHRASE_30MIN || phrase==PHRASE_60MIN) {
		sprintf_s(szTableName, sizeof(szTableName)-1, "KLineM%d_%s", phrase, strInstrumentID.c_str());
	}
	else if(phrase==PHRASE_DAY) {
		sprintf_s(szTableName, sizeof(szTableName)-1, "KLineDay_%s", strInstrumentID.c_str());
	}
	else if(phrase==PHRASE_WEEK) {
		sprintf_s(szTableName, sizeof(szTableName)-1, "KLineWeek_%s", strInstrumentID.c_str());
	}
	else if(phrase==PHRASE_MONTH) {
		sprintf_s(szTableName, sizeof(szTableName)-1, "KLineMonth_%s", strInstrumentID.c_str());
	}
	return szTableName;
}
BOOL CPlatformCTPQuotImpl::UpdateKLineData(string& strInstrumentID, EnumPhrase phrase, KLineBase& pKLine, bool bInsert, int& nErrorCode)
{
#if 1
	char szTableName[64];
	char szSQL[MAX_SQL_LENGTH];

	memset(szTableName, 0, sizeof(szTableName));

	//if(phrase==PHRASE_1MIN || phrase==PHRASE_5MIN || phrase==PHRASE_15MIN || phrase==PHRASE_30MIN || phrase==PHRASE_1MIN) {
	//	sprintf_s(szTableName, sizeof(szTableName)-1, "KLineM%d_%s", phrase, strInstrumentID.c_str());
	//}
	//else if(phrase==PHRASE_DAY) {
	//	sprintf_s(szTableName, sizeof(szTableName)-1, "KLineDay_%s", strInstrumentID.c_str());
	//}
	//else if(phrase==PHRASE_WEEK) {
	//	sprintf_s(szTableName, sizeof(szTableName)-1, "KLineWeek_%s", strInstrumentID.c_str());
	//}
	//else if(phrase==PHRASE_MONTH) {
	//	sprintf_s(szTableName, sizeof(szTableName)-1, "KLineMonth_%s", strInstrumentID.c_str());
	//}
	//else 
	//	return false;
	strncpy(szTableName, MakeTableName(strInstrumentID, phrase).c_str(), sizeof(szTableName)-1);

	LONGLONG t1=CTools_Win32::MyGetCpuTickCounter();
	std::set<std::string>::iterator it = m_setTableName.find(szTableName);
	if(it == m_setTableName.end()) {
		memset(szSQL, 0, sizeof(szSQL));
		sprintf(szSQL,	"create table %s"
						"(KLineTime          INTEGER PRIMARY KEY,"
						" OpenPrice          binary_double,"
						" HighestPrice       binary_double,"
						" LowestPrice        binary_double,"
						" Closeprice         binary_double,"
						" Turnover           binary_double,"
						" Volume             INTEGER,"
						" OpenInterest       INTEGER,"
						" InsertDBTime       DATE"
						") NOLOGGING",
						szTableName);
		CInterface_SvrDBOpr::getObj().Excute( szSQL, nErrorCode );
		m_setTableName.insert(szTableName);
	}

	memset(szSQL, 0, sizeof(szSQL));
	if(bInsert) {
		sprintf(szSQL,	"Insert Into %s Values(%I64d," 
						"TO_BINARY_DOUBLE('%g'), "
						"TO_BINARY_DOUBLE('%g'), "
						"TO_BINARY_DOUBLE('%g'), "
						"TO_BINARY_DOUBLE('%g'), "
						"TO_BINARY_DOUBLE('%g'), "
						"%u, %u, sysdate)",
						szTableName, pKLine.dwTime, 
						pKLine.fOpenPrice, pKLine.fHighPrice, 
						pKLine.fLowPrice, pKLine.fClosePrice, 
						pKLine.fTurnover, pKLine.dwVolume, pKLine.dwOpenInterest);
	}
	else {
		sprintf(szSQL,	"Update %s t Set "
						"OpenPrice = TO_BINARY_DOUBLE('%g'), "
						"HighestPrice = TO_BINARY_DOUBLE('%g'), "
						"LowestPrice = TO_BINARY_DOUBLE('%g'), "
						"Closeprice = TO_BINARY_DOUBLE('%g'), "
						"Turnover = TO_BINARY_DOUBLE('%g'), "
						"Volume = %u, OpenInterest = %u Where t.KLineTime = %I64d",
						szTableName, 
						pKLine.fOpenPrice, pKLine.fHighPrice, 
						pKLine.fLowPrice, pKLine.fClosePrice, 
						pKLine.fTurnover, pKLine.dwVolume, pKLine.dwOpenInterest, 
						pKLine.dwTime);
	}
	bool brlt=CInterface_SvrDBOpr::getObj().Excute( szSQL, nErrorCode );
	LONGLONG t2=CTools_Win32::MyGetCpuTickCounter();
	cputick+=(t2-t1);
	updatecnt++;
	return brlt;
#else
	char szSQL[MAX_SQL_LENGTH];
	memset(szSQL, 0, sizeof(szSQL));
	sprintf(szSQL, "begin PROC_KLineUpdate_SingleRow('%s', %u, %I64u, %g, %g, %g, %g, %g, %u, %u); end;",
			strInstrumentID.c_str(), phrase, pKLine.dwTime, 
			pKLine.fOpenPrice, pKLine.fHighPrice, 
			pKLine.fLowPrice, pKLine.fClosePrice, 
			pKLine.fTurnover, pKLine.dwVolume, pKLine.dwOpenInterest);
	LONGLONG t1=CTools_Win32::MyGetCpuTickCounter();
	bool brlt=CInterface_SvrDBOpr::getObj().Excute( szSQL, nErrorCode );
	LONGLONG t2=CTools_Win32::MyGetCpuTickCounter();
	cputick+=(t2-t1);
	updatecnt++;
	return brlt;
#endif
}

//BOOL CPlatformCTPQuotImpl::SaveKLineData(string& strName, KLineBase& pKLine, int nErrorCode)
//{
//	char szBuffer[MAX_SQL_LENGTH];
//	memset(szBuffer, 0, sizeof(szBuffer));
//	char strTime[64];
//	memset(strTime,0,sizeof(strTime));
//
//	sprintf_s(strTime,sizeof(strTime),"%04d%02d%02d %02d:%02d:%02d",1900+pKLine.dwTime.tm_year,1+pKLine.dwTime.tm_mon,pKLine.dwTime.tm_mday,pKLine.dwTime.tm_hour,pKLine.dwTime.tm_min,pKLine.dwTime.tm_sec);
//	sprintf(szBuffer,"insert into %s values('%s','%d',to_date('%s','YYYYMMDD HH24:MI:SS'), %g, %g, %g, %g, %g, %d, %d, %g,sysdate)",strName.c_str(),pKLine.InstrumentID,pKLine.eCycle,strTime,
//		pKLine.fOpenPrice,pKLine.fHighPrice,pKLine.fLowPrice,pKLine.fClosePrice,pKLine.fSumTradeVolume,pKLine.dwVolume,pKLine.dwHoldVolume,pKLine.dJieSuan);
//	if (CInterface_SvrDBOpr::getObj().Excute( szBuffer,nErrorCode ))
//	{
//		return true;
//	}
//	else
//		return false;
//}

//BOOL CPlatformCTPQuotImpl::SetInTimeData_TLine(PlatformStru_DepthMarketData* pData)
//{
//	
//	if(m_mapExchangeTime.size() == 0)
//	{		
//		return false;
//	}
//	
//
//
//	std::string strDate = m_strTradingDay.c_str();
//	std::string strTime		= pData->UpdateTime;
//	std::string strFutureID = pData->InstrumentID;
//
//	STLine pLine;
//	pLine.dwTime.tm_year	= atoi(m_strTradingDay.substr(0, 4).c_str())-1900;
//	pLine.dwTime.tm_mon  	= atoi(m_strTradingDay.substr(4, 2).c_str())-1;
//	pLine.dwTime.tm_mday	= atoi(m_strTradingDay.substr(6, 2).c_str());
//
//	pLine.dwTime.tm_hour	= atoi(strTime.substr(0, 2).c_str());
//	pLine.dwTime.tm_min	= atoi(strTime.substr(3, 2).c_str());
//	pLine.dwTime.tm_sec	= atoi(strTime.substr(6, 2).c_str());
//	pLine.dwTime.tm_wday	= 0;
//	pLine.dwTime.tm_yday	= 0;
//	pLine.fLastPrice = pData->LastPrice;
//	pLine.dwVolume	  = pData->Volume;	
//	pLine.dwVolumeMoney	  = pData->Turnover;
//	pLine.dwHoldVolume	      = pData->OpenInterest;
//
//	double dcClose = pData->PreSettlementPrice;
//	if(dcClose <0.00001)
//		dcClose = pData->OpenPrice;//如果没有昨结算，则用今日开盘价
//	pLine.dwTime.tm_wday	= 0;
//	pLine.dwTime.tm_yday	= 0;
//
//	BOOL bNotDelMemory = SetInTimeData_DiffSpan_TLine(pLine,pData->ExchangeID);
//	
//	return TRUE;
//}

//BOOL CPlatformCTPQuotImpl::SetInTimeData_DiffSpan_TLine(STLine& pTLine,std::string exchangeID)
//{
//	vector<struct tm> vecTimeScales;
//	vector<STLine> vecTLine;
//	if (exchangeID.empty())
//		return false;
//
//	STLine sTLineLast;
//	memset(&sTLineLast,0,sizeof(STLine));
//	std::map<std::string,STLine>::iterator itLast =	m_mapTLineLast.find(pTLine.InstrumentID);
//	if (itLast!= m_mapTLineLast.end())
//	{
//		sTLineLast =  itLast->second;
//	}
//
//	std::map<std::string,std::vector<struct tm>> ::iterator itTime=m_mapExchangeTime.find(exchangeID);
//	if (itTime == m_mapExchangeTime.end())
//		return true;
//	vecTimeScales= itTime->second;
//
//	std::map<std::string ,vector<STLine>>::iterator it = m_mapTLine.find(pTLine.InstrumentID);
//	if (it == m_mapTLine.end())
//	{
//		vector<STLine> vecTLine;
//		m_mapTLine[pTLine.InstrumentID] = vecTLine;
//		
//	}
//	else
//	{
//        vecTLine = it->second;
//	}
//	
//	
//
//	bool bSameMin = false;
//	STLine sTemp = pTLine;
//	struct tm TM = GetTimeInSpan_TLine(pTLine.dwTime, 60,vecTimeScales);//60分钟，3600秒
//	if ( TM.tm_year == 0)
//	{
//		return false;
//	}
//	pTLine.dwTime  = TM;//这样处理就需要先过滤重复行情
//	pTLine.dwTime.tm_sec   = 0;
//
//	if(vecTLine.size() == 0)
//	{
//		bSameMin = true;		
//		struct tm tm1 = vecTimeScales[0];
//		struct tm tm2 = pTLine.dwTime;
//		if(difftime(mktime(&tm1), mktime(&tm2)) !=0)
//		{
//			pTLine.dwVolume		= 0;
//			pTLine.dwVolumeMoney   = 0;
//		}
//		pTLine.dwTime  = TM;	
//		vecTLine.push_back(pTLine);	
//	}
//	else
//	{
//		STLine& pLast = vecTLine[0];
//
//		if(pLast.dwTime.tm_year == TM.tm_year
//			&& pLast.dwTime.tm_mon == TM.tm_mon 
//			&& pLast.dwTime.tm_mday == TM.tm_mday
//			&& pLast.dwTime.tm_hour == TM.tm_hour)
//		{			
//			/*	if(pLast.dwTime.tm_min == pTLine.dwTime.tm_min && pLast.dwTime.tm_sec == pTLine.dwTime.tm_sec)
//			{
//			bSameMin = false;//重复行情
//			m_sTLineLast = sTemp;
//			return bSameMin;
//			}*/
//			if(pLast.dwTime.tm_min == TM.tm_min )
//			{
//				bSameMin = false;
//				pLast.dwTime     = pTLine.dwTime;
//				pLast.fLastPrice = pTLine.fLastPrice;
//				if(sTLineLast.dwTime.tm_year != 0)
//				{
//					pLast.dwVolume  += pTLine.dwVolume - sTLineLast.dwVolume;
//					pLast.dwVolumeMoney += pTLine.dwVolumeMoney - sTLineLast.dwVolumeMoney;
//				}			
//			}
//		}	
//		int nTimeSpanTemp = difftime(mktime(&pLast.dwTime), mktime(&pTLine.dwTime));//刚加的，要测试一下
//		if(nTimeSpanTemp >0)
//		{
//			STLine& pLastFirst = vecTLine[vecTLine.size() - 1];
//			if(difftime(mktime(&(pLastFirst.dwTime)), mktime(&TM)) > 0)
//			{//比第一个数据还要早，主要是正对模拟行情处理
//				bSameMin = true;
//				if(sTLineLast.dwTime.tm_year != 0)
//				{
//					pTLine.dwVolume  = pTLine.dwVolume - sTLineLast.dwVolume;
//					pTLine.dwVolumeMoney = pTLine.dwVolumeMoney - sTLineLast.dwVolumeMoney;
//				}
//				else
//				{
//					pTLine.dwVolume		= 0;
//					pTLine.dwVolumeMoney   = 0;
//				}
//				vecTLine.push_back(pTLine);					
//			}
//			else
//			{			
//				vector<STLine>::iterator itLast = vecTLine.end();
//				for(vector<STLine>::iterator it = vecTLine.begin(); it!= vecTLine.end(); it++)
//				{
//					STLine& pLastTemp = *it;
//					if(pLastTemp.dwTime.tm_year == TM.tm_year
//						&& pLastTemp.dwTime.tm_mon == TM.tm_mon 
//						&& pLastTemp.dwTime.tm_mday == TM.tm_mday
//						&& pLastTemp.dwTime.tm_hour == TM.tm_hour
//						&& pLastTemp.dwTime.tm_min == TM.tm_min)
//					{//如果是按时间先加进去的则有可能出现这种情况
//						bSameMin = false;
//
//						pLastTemp.fLastPrice = pTLine.fLastPrice;
//						if(sTLineLast.dwTime.tm_year != 0)
//						{
//							pLastTemp.dwVolume  += pTLine.dwVolume - sTLineLast.dwVolume;
//							pLastTemp.dwVolumeMoney += pTLine.dwVolumeMoney - sTLineLast.dwVolumeMoney;
//						}							
//						break;
//					}
//					else if(difftime(mktime(&pLastTemp.dwTime), mktime(&TM)) < 0)
//					{//处于中间的某个数据，兼容模拟行情	
//						bSameMin = true;
//						//	if(itLast == m_vecTLine.end())
//						//		m_vecTLine.insert(m_vecTLine.begin(), pTLine);	
//						//	else
//						if( sTLineLast.dwTime.tm_year != 0)
//						{
//							pTLine.dwVolume  = pTLine.dwVolume - sTLineLast.dwVolume;
//							pTLine.dwVolumeMoney = pTLine.dwVolumeMoney - sTLineLast.dwVolumeMoney;
//						}
//						else
//						{
//							pTLine.dwVolume		= 0;
//							pTLine.dwVolumeMoney   = 0;
//						}
//						vecTLine.insert(it, pTLine);						
//						break;
//					}				
//					itLast = it;
//				}	
//			}			
//		}
//		else if(!bSameMin && nTimeSpanTemp < 0)
//		{
//			if(sTLineLast.dwTime.tm_year != 0)
//			{
//				pTLine.dwVolume  = pTLine.dwVolume - sTLineLast.dwVolume;
//				pTLine.dwVolumeMoney = pTLine.dwVolumeMoney - sTLineLast.dwVolumeMoney;
//			}
//			else
//			{
//				pTLine.dwVolume		= 0;
//				pTLine.dwVolumeMoney   = 0;
//			}
//			vecTLine.insert(vecTLine.begin(), pTLine);				
//			bSameMin = true;			
//		}
//	}
//
//	m_mapTLineLast[sTemp.InstrumentID] = sTemp;
//	return bSameMin;
//}
//struct tm CPlatformCTPQuotImpl::GetTimeInSpan_TLine(struct tm tmPara, int nSpan,vector<struct tm>& vecTimeScales)
//{
//
//	if(vecTimeScales.size() == 0)
//	{
//		struct tm tm1;
//		tm1.tm_year = 0;
//		return tm1;
//	}
//	time_t tmMax =   mktime(&vecTimeScales[vecTimeScales.size()-1]);
//	time_t tReturn;
//	//用这个值判断成功否
//	time_t tm11 =   mktime(&tmPara);
//	int nIndex = 0;	
//	for(int i= 0; i< (int)vecTimeScales.size(); i++)
//	{
//		if(difftime(mktime(&vecTimeScales[i]), mktime(&tmPara)) >0) 
//		{
//			nIndex = i;
//			break;
//		}
//		if(i == vecTimeScales.size() -1)
//			return vecTimeScales[i];//15:00:07 算成 15:00:00
//	}
//	if(nIndex == 0)
//	{		
//
//		//nIndex = nIndex +1;
//		struct tm Return;
//		memset( &Return,0,sizeof(tm));
//		return Return;
//
//	}
//	else if( nIndex%2==0)
//	{
//		time_t t1 = mktime( &vecTimeScales[nIndex-1]);
//		t1 -= 1;//
//		struct tm tmReturn = *localtime(&t1);
//		return GetTimeInSpan(tmReturn, nSpan,vecTimeScales);
//		//return vecTimeScales[nIndex-1];//11:30:10 算成11:30分
//	}
//	DWORD	dwCount = 0;
//	for(int i = 0; i<nIndex-1; i++)
//	{		
//		time_t t1 = mktime( &vecTimeScales[i] );
//		time_t t2 = mktime( &vecTimeScales[i+1]);
//		DWORD dw = abs(difftime(t1, t2));
//		dwCount += dw;
//		i++;
//	}
//	if(nIndex != 0)
//	{
//		DWORD dwn = dwCount%nSpan;
//		time_t t1 = mktime( &vecTimeScales[nIndex -1]);
//		time_t t3 = t1+dwn;
//		while(t3 <= tm11)
//		{
//			t3 += nSpan;
//		}
//		if(nIndex + 2 < (int)vecTimeScales.size())
//		{
//			time_t tNext = mktime( &vecTimeScales[nIndex]);
//			if(t3 > tNext)
//			{
//				t3 = t3- tNext + mktime(&vecTimeScales[nIndex+1]);//11:29:00在30分钟是应该是13:15（9:15开盘）
//			}			
//		}
//		tReturn = t3;	
//	}
//	if(tReturn > tmMax)
//		tReturn = tmMax;
//
//	struct tm tmReturn;
//	tmReturn.tm_year = 0;
//	tmReturn = *localtime(&tReturn);
//
//	return tmReturn;
//}

BOOL CPlatformCTPQuotImpl::SetInTimeData_DiffSpan(string& strInstrumentID, KLineBase& pKLine, EnumPhrase enumPhrase,std::string exchangeID)
{	
	if (exchangeID.empty())
	    return false;
	std::map<std::string,vector<struct tm>> ::iterator itTime=m_mapExchangeTime.find(exchangeID);
	if (itTime == m_mapExchangeTime.end())
		return true;

	KLineBase pTemp = pKLine;
	KLineBase KlineRet;
	memset( &KlineRet, 0, sizeof(KLineBase));
	bool bRet = true;

	//m_mutexKLine.lock();

	switch(enumPhrase)
	{
	case PHRASE_1MIN:
		{			
			std::map<std::string ,vector<KLineBase>>::iterator it = m_mapKLineMin1.find(strInstrumentID);
			if (it == m_mapKLineMin1.end())
			{
				vector<KLineBase>  vecKLineMin;
				m_mapKLineMin1[strInstrumentID] = vecKLineMin;
				bRet = SetInTimeData_MINS(strInstrumentID, vecKLineMin, pTemp, 60,itTime->second);//1
				if (bRet)
				{
					KlineRet = vecKLineMin[0];
				}
				
			}
			else
			{
				bRet =SetInTimeData_MINS(strInstrumentID, it->second, pTemp, 60,itTime->second);//1	
				if (bRet)
				{
					KlineRet = it->second[0];
				}
				
			}			

		}
		break;
	case PHRASE_5MIN:
		{
			std::map<std::string ,vector<KLineBase>>::iterator it = m_mapKLineMin5.find(strInstrumentID);
			if (it == m_mapKLineMin5.end())
			{
				vector<KLineBase> vecKLineMin;
				m_mapKLineMin5[strInstrumentID] = vecKLineMin;
				bRet =SetInTimeData_MINS(strInstrumentID, vecKLineMin, pTemp, 300,itTime->second);//5
				if (bRet)
				{
					KlineRet = vecKLineMin[0];
				}
				
			}
			else
			{
				bRet =SetInTimeData_MINS(strInstrumentID, it->second, pTemp, 300,itTime->second);//5	
				if (bRet)
				{
					KlineRet = (it->second)[0];
				}
				
			}
		}
		break;
	case PHRASE_15MIN:
		{
			std::map<std::string ,vector<KLineBase>>::iterator it = m_mapKLineMin15.find(strInstrumentID);
			if (it == m_mapKLineMin15.end())
			{
				vector<KLineBase> vecKLineMin;
				m_mapKLineMin15[strInstrumentID] = vecKLineMin;
				bRet =SetInTimeData_MINS(strInstrumentID, vecKLineMin, pTemp, 900,itTime->second);//15
				if (bRet)
				{
					KlineRet = vecKLineMin[0];
				}
				
			}
			else
			{
				bRet =SetInTimeData_MINS(strInstrumentID, it->second, pTemp, 900,itTime->second);//15
				if (bRet)
				{
					KlineRet = it->second[0];
				}
				
			}
		}
		break;
	case PHRASE_30MIN:
		{
			std::map<std::string ,vector<KLineBase>>::iterator it = m_mapKLineMin30.find(strInstrumentID);
			if (it == m_mapKLineMin30.end())
			{
				vector<KLineBase> vecKLineMin;
				m_mapKLineMin30[strInstrumentID] = vecKLineMin;
				bRet =SetInTimeData_MINS(strInstrumentID, vecKLineMin, pTemp, 1800,itTime->second);//30
				if (bRet)
				{
					KlineRet = vecKLineMin[0];
				}
				
			}
			else
			{
				bRet =SetInTimeData_MINS(strInstrumentID, it->second, pTemp, 1800,itTime->second);//30	
				if (bRet)
				{
					KlineRet = it->second[0];
				}
				
			}
		}
		break;
	case PHRASE_60MIN:
		{
			std::map<std::string ,vector<KLineBase>>::iterator it = m_mapKLineMin60.find(strInstrumentID);
			if (it == m_mapKLineMin60.end())
			{
				vector<KLineBase> vecKLineMin;
				m_mapKLineMin60[strInstrumentID] = vecKLineMin;
				bRet =SetInTimeData_MINS(strInstrumentID, vecKLineMin, pTemp, 3600,itTime->second);//60
				if (bRet)
				{
					KlineRet = vecKLineMin[0];
				}
				
			}
			else
			{
				bRet =SetInTimeData_MINS(strInstrumentID, it->second, pTemp, 3600,itTime->second);//60	
				if (bRet)
				{
					KlineRet = it->second[0];
				}
				
			}
		}
		break;
	case PHRASE_DAY:
		{
						
				//////////////////////////////////////////////////////////////////////////1149

				std::map<std::string ,vector<KLineBase>>::iterator it = m_mapKLineDay.find(strInstrumentID);
				if (it == m_mapKLineDay.end())
				{
					vector<KLineBase> vecKLineMin;
					m_mapKLineDay[strInstrumentID] = vecKLineMin;
					bRet =SetInTimeData_Day(strInstrumentID, vecKLineMin, pTemp,itTime->second);
					if (bRet)
					{
						KlineRet = vecKLineMin[0];
					}
				}
				else
				{
					bRet =SetInTimeData_Day(strInstrumentID, it->second, pTemp,itTime->second);	
					if (bRet)
					{
						KlineRet = it->second[0];
					}
				}
				//日，周，月线不需要时,分,秒;统一处理				
				/*KlineRet.dwTime.tm_hour = 0;
				KlineRet.dwTime.tm_min  = 0;
				KlineRet.dwTime.tm_sec  = 0;*/
		}
		break;
	case PHRASE_WEEK:
		{
							
				//////////////////////////////////////////////////////////////////////////1149

				std::map<std::string ,vector<KLineBase>>::iterator it = m_mapKLineWeek.find(strInstrumentID);
				if (it == m_mapKLineWeek.end())
				{
					vector<KLineBase> vecKLineMin;
					m_mapKLineWeek[strInstrumentID] = vecKLineMin;
					bRet =SetInTimeData_Week(strInstrumentID, vecKLineMin, pTemp,itTime->second);
					if (bRet)
					{
						KlineRet = vecKLineMin[0];
					}
				}
				else
				{
					bRet =SetInTimeData_Week(strInstrumentID, it->second, pTemp,itTime->second);	
					if (bRet)
					{
						KlineRet = it->second[0];
					}
				}
				//日，周，月线不需要时,分,秒;统一处理				
				/*KlineRet.dwTime.tm_hour = 0;
				KlineRet.dwTime.tm_min  = 0;
				KlineRet.dwTime.tm_sec  = 0;*/
			}	
		
		break;
	case PHRASE_MONTH:
		{
						
				//////////////////////////////////////////////////////////////////////////1149
	
				std::map<std::string ,vector<KLineBase>>::iterator it = m_mapKLineMonth.find(strInstrumentID);
				if (it == m_mapKLineMonth.end())
				{
					vector<KLineBase> vecKLineMin;
					m_mapKLineMonth[strInstrumentID] = vecKLineMin;
					bRet =SetInTimeData_Month(strInstrumentID, vecKLineMin, pTemp,itTime->second);
					if (bRet)
					{
						KlineRet = vecKLineMin[0];
					}
				}
				else
				{
					bRet =SetInTimeData_Month(strInstrumentID, it->second, pTemp,itTime->second);	
					if (bRet)
					{
						KlineRet = it->second[0];
					}
				}	
				//日，周，月线不需要时,分,秒;统一处理				
				/*KlineRet.dwTime.tm_hour = 0;
				KlineRet.dwTime.tm_min  = 0;
				KlineRet.dwTime.tm_sec  = 0;*/
			
		}
		break;
	}

	//m_mutexKLine.unlock();

	if (bRet)
	{
		//加到cache里
		SaveKline2Cache(strInstrumentID, KlineRet, enumPhrase);
		SaveKline2DB(strInstrumentID,KlineRet, enumPhrase,exchangeID);
	}
	

	return bRet;


}
BOOL CPlatformCTPQuotImpl::SetInTimeData_MINS(string& strInstrumentID, vector<KLineBase>& vecKLinePara, KLineBase& pKLine, int nTimeSpan ,vector<struct tm>& vecTimeScales)
{
	


	bool bSameMin = false;
	if(vecKLinePara.size() == 0)
	{

		struct tm tm1 = vecTimeScales[0];
		struct tm tm2 = *localtime(&pKLine.dwTime);
		// 这个地方逻辑上是不是有点问题？时间相等不做任何处理？
		if(difftime(mktime(&tm1), mktime(&tm2)) != 0)
		{
			pKLine.dwVolume		= 0;
			pKLine.fTurnover	= 0;
			pKLine.fOpenPrice	= pKLine.fClosePrice;
			pKLine.fHighPrice	= pKLine.fClosePrice;
			pKLine.fLowPrice	= pKLine.fClosePrice;
		}

		struct tm TM = GetTimeInSpan(*localtime(&pKLine.dwTime), nTimeSpan,vecTimeScales);
		if ( TM.tm_year == 0)
		{
			return false;
		}
		TM.tm_sec = 0;
		pKLine.dwTime = mktime(&TM);

		vecKLinePara.push_back(pKLine);
		return true;
	
	}
	else
	{
		KLineBase sKLineLast;
		struct tm sKLineLastTime = {0};
		memset( &sKLineLast,0,sizeof(KLineBase));
		std::map<std::string,KLineBase>::iterator itLast =	m_mapKLineLast.find(strInstrumentID);
		if (itLast!= m_mapKLineLast.end())
		{
			sKLineLast =  itLast->second;
			sKLineLastTime = *localtime(&sKLineLast.dwTime);
		}

		KLineBase& pLast = vecKLinePara[0];
		struct tm pLastTime = {0};
		struct tm TM = GetTimeInSpan(*localtime(&pKLine.dwTime), nTimeSpan,vecTimeScales);//15:15:09 就可以转换为15:15
		if ( TM.tm_year == 0)
		{
			return false;
		}
		pLastTime = *localtime(&pLast.dwTime);
		if(pLastTime.tm_year == TM.tm_year
			&& pLastTime.tm_mon == TM.tm_mon 
			&& pLastTime.tm_mday == TM.tm_mday)
		{//
			struct tm tm1 = pLastTime;
			struct tm tm2 = TM;
			int dwTimeSpan = difftime(mktime(&tm1), mktime(&tm2));
			if(dwTimeSpan==0)
			{//同一分钟	
				bSameMin = true;
				pLast.fClosePrice  = pKLine.fClosePrice;
				pLast.dwOpenInterest = pKLine.dwOpenInterest;
				//pLast.dJieSuan     = pKLine.dJieSuan;
				if(pLast.fHighPrice < pKLine.fClosePrice  )
					pLast.fHighPrice	= pKLine.fClosePrice;
				if(pLast.fLowPrice > pKLine.fClosePrice)
					pLast.fLowPrice	= pKLine.fClosePrice;

				if(sKLineLastTime.tm_year != 0)
				{
					pLast.dwVolume += pKLine.dwVolume - sKLineLast.dwVolume;				
					pLast.fTurnover	+= pKLine.fTurnover - sKLineLast.fTurnover;
				}
				
				return bSameMin;

			}
			else if(dwTimeSpan >0)
			{		
				KLineBase& pLastFirst = vecKLinePara[vecKLinePara.size() - 1];
				if(difftime(pLastFirst.dwTime, mktime(&TM)) > 0)
				{//比第一个数据还要早，主要是正对模拟行情处理
					if(sKLineLastTime.tm_year != 0)
					{
						pKLine.dwVolume  = pKLine.dwVolume - sKLineLast.dwVolume;
						pKLine.fTurnover = pKLine.fTurnover - sKLineLast.fTurnover;
					}
					else
					{
						pKLine.dwVolume		= 0;
						pKLine.fTurnover   = 0;
					}
					vecKLinePara.push_back(pKLine);					
				}
				else 
				{//实时行情加到目前数组的中间去
					vector<KLineBase>::iterator itLast = vecKLinePara.end();
					for(vector<KLineBase>::iterator it = vecKLinePara.begin(); it!= vecKLinePara.end(); it++)
					{
						KLineBase& pLastTemp = *it;
						struct tm pLastTempTime = {0};
						pLastTempTime = *localtime(&pLastTemp.dwTime);
						if(pLastTempTime.tm_year == TM.tm_year
							&& pLastTempTime.tm_mon == TM.tm_mon 
							&& pLastTempTime.tm_mday == TM.tm_mday
							&& pLastTempTime.tm_hour == TM.tm_hour
							&& pLastTempTime.tm_min == TM.tm_min)
						{//如果是按时间先加进去的则有可能出现这种情况
							bSameMin = true;

							pLastTemp.fClosePrice = pKLine.fClosePrice;
							if(pLastTemp.fHighPrice < pKLine.fClosePrice  )
								pLastTemp.fHighPrice = pKLine.fClosePrice;
							if(pLastTemp.fLowPrice > pKLine.fClosePrice)
								pLastTemp.fLowPrice	= pKLine.fClosePrice;
							if(sKLineLastTime.tm_year != 0)
							{
								pLastTemp.dwVolume  += pKLine.dwVolume - sKLineLast.dwVolume;
								pLastTemp.fTurnover += pKLine.fTurnover - sKLineLast.fTurnover;
							}						
							break;
						}
						if(difftime(pLastTemp.dwTime, mktime(&TM)) < 0)
						{		
							TM.tm_sec   = 0;
							pKLine.dwTime = mktime(&TM);
							if(sKLineLastTime.tm_year != 0)
							{
								pKLine.dwVolume  = pKLine.dwVolume - sKLineLast.dwVolume;
								pKLine.fTurnover = pKLine.fTurnover - sKLineLast.fTurnover;
							}
							//////////////////////////////////////////////////////////////////////////
							//对于插入数组的类型，最低最高价都应该新的计，以开盘价为准
							pKLine.fOpenPrice      = pKLine.fClosePrice;
							pKLine.fHighPrice		= pKLine.fClosePrice;
							pKLine.fLowPrice       = pKLine.fClosePrice;
							//////////////////////////////////////////////////////////////////////////
							vecKLinePara.insert(it, pKLine);
							break;
						}
						itLast = it;
					}	
				}
				return bSameMin;
			}
		}
		if(!bSameMin)
		{
			if(sKLineLastTime.tm_year != 0)
			{//
				pKLine.dwVolume     = pKLine.dwVolume - sKLineLast.dwVolume;				

				pKLine.fTurnover	= pKLine.fTurnover - sKLineLast.fTurnover;
				/* 廖说开盘价不用上一时间周期的收盘价修改之前;以后如果换成上一分钟的收盘价则应该把注解的打开，把后面的注掉就可以了
				pKLine.fOpenPrice      = sKLineLast.fClosePrice;
				pKLine.fHighPrice		= sKLineLast.fClosePrice;
				pKLine.fLowPrice       = sKLineLast.fClosePrice;
				*/
				pKLine.fOpenPrice      = pKLine.fClosePrice;
				pKLine.fHighPrice		= pKLine.fClosePrice;
				pKLine.fLowPrice       = pKLine.fClosePrice;
			}
			else
			{//刚开盘的第一笔
				struct tm tm1 = vecTimeScales[0];
				struct tm tm2 = *localtime(&pKLine.dwTime);
				if(difftime(mktime(&tm1), mktime(&tm2)) != 0)
				{
					pKLine.dwVolume		= 0;
					pKLine.fTurnover   = 0;
					pKLine.fOpenPrice      = pKLine.fClosePrice;
					pKLine.fHighPrice		= pKLine.fClosePrice;
					pKLine.fLowPrice       = pKLine.fClosePrice;					
				}
			}				

			TM.tm_sec   = 0;
			pKLine.dwTime = mktime(&TM);
			vecKLinePara.insert(vecKLinePara.begin(), pKLine);	
		}		
	}


	return true;
}

struct tm CPlatformCTPQuotImpl::GetTimeInSpan(struct tm tmPara, int nSpan ,vector<struct tm>& vecTimeScales)
{
	

	time_t tmMax =   mktime(&vecTimeScales[vecTimeScales.size()-1]);
	time_t tReturn;

	//用这个值判断成功否e
	time_t tm11 =   mktime(&tmPara);
	int nIndex = 0;	
	for(int i= 0; i< (int)vecTimeScales.size(); i++)
	{
		if(difftime(mktime(&vecTimeScales[i]), mktime(&tmPara)) >0) 
		{
			nIndex = i;
			break;
		}

		if(i == vecTimeScales.size() -1)
			return vecTimeScales[i];//15:00:07 算成 15:00:00
	}

	if(nIndex == 0)
	{	//原代码，行情时间小于开盘时间时处理为开盘后第一段时间内行情	
		/*time_t t1 = mktime( &vecTimeScales[0]);
		t1 += nSpan;
		struct tm tmReturn = *localtime(&t1);
		return tmReturn; */

		//修改为，行情时间小于开盘时间 剔除掉
		struct tm Return;
		memset( &Return,0,sizeof(tm));
		return Return;
		
	}
	else if( nIndex%2==0)
	{
		time_t t1 = mktime( &vecTimeScales[nIndex-1]);
		t1 -= 1;//
		struct tm tmReturn = *localtime(&t1);
		return GetTimeInSpan(tmReturn, nSpan,vecTimeScales);
		//	return vecTimeScales[nIndex-1];//11:30:10 算成11:30分
	}
	DWORD	dwCount = 0;
	for(int i = 0; i<nIndex-1; i++)
	{		
		time_t t1 = mktime( &vecTimeScales[i] );
		time_t t2 = mktime( &vecTimeScales[i+1]);
		DWORD dw = abs(difftime(t1, t2));
		dwCount += dw;
		i++;
	}
	if(nIndex != 0)
	{
		DWORD dwn = dwCount%nSpan;
		time_t t1 = mktime( &vecTimeScales[nIndex -1]);
		time_t t3 = t1-dwn;
		while(t3 <= tm11)
		{
			t3 += nSpan;
		}
		if(nIndex + 2 < (int)vecTimeScales.size())
		{
			time_t tNext = mktime( &vecTimeScales[nIndex]);
			if(t3 > tNext)
			{
				t3 = t3- tNext + mktime(&vecTimeScales[nIndex+1]);//11:29:00在30分钟是应该是13:15（9:15开盘）
			}			
		}
		tReturn = t3;	
	}
	if(tReturn > tmMax)
		tReturn = tmMax;

	struct tm tmReturn;
	tmReturn.tm_year = 0;
	tmReturn = *localtime(&tReturn);

	return tmReturn;
}

BOOL CPlatformCTPQuotImpl::SetInTimeData_Day(string& strInstrumentID, vector<KLineBase>& vecKLinePara, KLineBase& pKLine ,vector<struct tm>& vecTimeScales)
{
	if(difftime(mktime(&vecTimeScales[0]), pKLine.dwTime) > 0) 
        return false;
	if(util::isInvalidValue(pKLine.fHighPrice)
		||util::isInvalidValue(pKLine.fOpenPrice)
		||util::isInvalidValue(pKLine.fLowPrice))
		return false;

	struct tm pKLineTime = *localtime(&pKLine.dwTime);

	bool bSameMin = false;
	if(vecKLinePara.size() == 0)
	{
		vecKLinePara.push_back(pKLine);	
		//return true;
	}
	else
	{
		KLineBase& pLast = vecKLinePara[0];		
		struct tm pLastTime = *localtime(&pLast.dwTime);
		if(pLastTime.tm_year == pKLineTime.tm_year
			&& pLastTime.tm_mon == pKLineTime.tm_mon 
			&& pLastTime.tm_mday == pKLineTime.tm_mday)
		{			
			pLast.fClosePrice  = pKLine.fClosePrice;
			pLast.dwOpenInterest = pKLine.dwOpenInterest;
			//pLast.dJieSuan     = pKLine.dJieSuan;
			pLast.dwVolume     = pKLine.dwVolume;
			if(pLast.fHighPrice < pKLine.fClosePrice  )
				pLast.fHighPrice	= pKLine.fClosePrice;
			if(pLast.fLowPrice > pKLine.fClosePrice)
				pLast.fLowPrice	= pKLine.fClosePrice;
			pLast.fOpenPrice	= pLast.fOpenPrice;
			pLast.fTurnover		= pLast.fTurnover;
			bSameMin = true;
		}
		if(!bSameMin)
		{			
			vecKLinePara.insert(vecKLinePara.begin(), pKLine);				
		}
	}
	return true;
}
int CPlatformCTPQuotImpl::GetDayOfYear(struct tm tm1)
{
	int day=0;

	int month[13] = { 0, 31, 28, 31, 30, 31, 30,
		31, 31, 30, 31, 30, 31 };
	if(tm1.tm_year % 400 == 0 || tm1.tm_year % 100 != 0 && tm1.tm_year % 4 == 0) 
	{
		month[2] = 29;
	}
	if(tm1.tm_mon >= 1 && tm1.tm_mon <= 12 && tm1.tm_mday >= 1 && tm1.tm_mday <= month[tm1.tm_mon]) 
	{
		day = tm1.tm_mday;
		for(int i = 1; i < tm1.tm_mon; ++i) 
		{
			day += month[i];
		}
	}
	return day;
}
bool CPlatformCTPQuotImpl::IsOneWeek(struct tm tm1, struct tm tm2)
{
	
	if(tm1.tm_year != tm2.tm_year)
		return false;
	int nDay1 = GetDayOfYear(tm1);
	int nDay2 = GetDayOfYear(tm2);

	if(nDay1/7 +1 != nDay2/7+1)
		return false;

	return true;
}

bool CPlatformCTPQuotImpl::IsOneMonth(struct tm tm1, struct tm tm2)
{
	if((tm1.tm_year == tm2.tm_year) && (tm1.tm_mon == tm2.tm_mon))
		return true;
	return false;
}
BOOL CPlatformCTPQuotImpl::SetInTimeData_Week(string& strInstrumentID, vector<KLineBase>& vecKLinePara, KLineBase& pKLine ,vector<struct tm>& vecTimeScales)
{	
	if(difftime(mktime(&vecTimeScales[0]), pKLine.dwTime) > 0) 
		return false;
	if(util::isInvalidValue(pKLine.fHighPrice)
		||util::isInvalidValue(pKLine.fOpenPrice)
		||util::isInvalidValue(pKLine.fLowPrice))
		return false;

	bool bSameMin = false;
	if(vecKLinePara.size() == 0)
	{
		vecKLinePara.push_back(pKLine);		
	}
	else
	{
		bool hasFound = false;
		KLineBase sKLineLast;
		memset(&sKLineLast,0,sizeof(KLineBase));
		std::map<std::string,KLineBase>::iterator itLast =	m_mapKLineLast.find(strInstrumentID);
		if (itLast!= m_mapKLineLast.end())
		{
			sKLineLast =  itLast->second;
			hasFound = true;
		}

		KLineBase& pLast = vecKLinePara[0];	
		if(IsOneWeek(*localtime(&pLast.dwTime), *localtime(&pKLine.dwTime)))
		{			
			pLast.dwTime = pKLine.dwTime;
			pLast.fClosePrice  = pKLine.fClosePrice;
			pLast.dwOpenInterest = pKLine.dwOpenInterest;
			//pLast.dJieSuan     = pKLine.dJieSuan;
			if(pLast.fHighPrice < pKLine.fClosePrice  )
				pLast.fHighPrice	= pKLine.fClosePrice;
			if(pLast.fLowPrice > pKLine.fClosePrice)
				pLast.fLowPrice	= pKLine.fClosePrice;

			if(hasFound)
			{
				pLast.dwVolume  += pKLine.dwVolume - sKLineLast.dwVolume;				
				pLast.fTurnover	+= pKLine.fTurnover - sKLineLast.fTurnover;
			}
			bSameMin = true;			
		}
		if(!bSameMin)
		{
			vecKLinePara.insert(vecKLinePara.begin(), pKLine);				
		}
	}
	return true;
}
BOOL CPlatformCTPQuotImpl::SetInTimeData_Month(string& strInstrumentID, vector<KLineBase>& vecKLinePara, KLineBase& pKLine ,vector<struct tm>& vecTimeScales)
{
	if(difftime(mktime(&vecTimeScales[0]), pKLine.dwTime) > 0) 
		return false;

	if(util::isInvalidValue(pKLine.fHighPrice)
		||util::isInvalidValue(pKLine.fOpenPrice)
		||util::isInvalidValue(pKLine.fLowPrice))
		return false;

	bool bSameMin = false;
	if(vecKLinePara.size() == 0)
	{		
		vecKLinePara.push_back(pKLine);		
	}
	else
	{
		bool hasFound = false;
		KLineBase sKLineLast;
		memset(&sKLineLast,0,sizeof(KLineBase));
		std::map<std::string,KLineBase>::iterator itLast =	m_mapKLineLast.find(strInstrumentID);
		if (itLast!= m_mapKLineLast.end())
		{
			sKLineLast =  itLast->second;
			hasFound = true;
		}

		KLineBase& pLast = vecKLinePara[0];	
		if(IsOneMonth(*localtime(&pLast.dwTime), *localtime(&pKLine.dwTime)))
		{
			pLast.dwTime = pKLine.dwTime;
			pLast.fClosePrice  = pKLine.fClosePrice;
			pLast.dwOpenInterest = pKLine.dwOpenInterest;
			//pLast.dJieSuan     = pKLine.dJieSuan;
			if(pLast.fHighPrice < pKLine.fClosePrice  )
				pLast.fHighPrice	= pKLine.fClosePrice;
			if(pLast.fLowPrice > pKLine.fClosePrice)
				pLast.fLowPrice	= pKLine.fClosePrice;
			if(hasFound)
			{
				pLast.dwVolume  += pKLine.dwVolume - sKLineLast.dwVolume;				
				pLast.fTurnover	+= pKLine.fTurnover - sKLineLast.fTurnover;
			}
			bSameMin = true;
		}
		if(!bSameMin)
		{
			vecKLinePara.insert(vecKLinePara.begin(), pKLine);	
		}
	}

	return true;
}
//void CPlatformCTPQuotImpl::GetMapExchangeTime()
//{
//
//}

string CPlatformCTPQuotImpl::GetTradingDay()
{
	return m_strTradingDay;
}

void CPlatformCTPQuotImpl::SetKLineMin1(std::map<std::string,map<time_t,KLineBase>> mapKLine)
{
	CeasymutexGuard guard(m_mutexKLineM1);
	m_vmapKLineMin1=mapKLine;
}
void CPlatformCTPQuotImpl::SetKLineMin5(std::map<std::string,map<time_t,KLineBase>> mapKLine)
{
	CeasymutexGuard guard(m_mutexKLineM5);
	m_vmapKLineMin5=mapKLine;
}
void CPlatformCTPQuotImpl::SetKLineMin15(std::map<std::string,map<time_t,KLineBase>> mapKLine)
{
	CeasymutexGuard guard(m_mutexKLineM15);
	m_vmapKLineMin15=mapKLine;
}
void CPlatformCTPQuotImpl::SetKLineMin30(std::map<std::string,map<time_t,KLineBase>> mapKLine)
{
	CeasymutexGuard guard(m_mutexKLineM30);
	m_vmapKLineMin30=mapKLine;
}
void CPlatformCTPQuotImpl::SetKLineMin60(std::map<std::string,map<time_t,KLineBase>> mapKLine)
{
	CeasymutexGuard guard(m_mutexKLineM60);
	m_vmapKLineMin60=mapKLine;
}
void CPlatformCTPQuotImpl::SetKLineDay(std::map<std::string,map<time_t,KLineBase>> mapKLine)
{
	CeasymutexGuard guard(m_mutexKLineDay);
	m_vmapKLineDay=mapKLine;
}
void CPlatformCTPQuotImpl::SetKLineWeek(std::map<std::string,map<time_t,KLineBase>> mapKLine)
{
	CeasymutexGuard guard(m_mutexKLineWeek);
	m_vmapKLineWeek=mapKLine;
}
void CPlatformCTPQuotImpl::SetKLineMonth(std::map<std::string,map<time_t,KLineBase>> mapKLine)
{
	CeasymutexGuard guard(m_mutexKLineMonth);
	m_vmapKLineMonth=mapKLine;
}

bool CPlatformCTPQuotImpl::GetKLineMin1(KLineQuery* pData,vector<KLineBase>& outData,vector<TLineBase>& outTLData)
{
	CeasymutexGuard guard(m_mutexKLineM1);
	if(pData->nDataType==2)//分时线 今日所有数据
	{
		char szStartDate[9] ,szStartTime[9];
		memset(szStartDate,0,sizeof(szStartDate));	strncpy(szStartDate,m_strTradingDay.c_str(),8); 
		memset(szStartTime,0,sizeof(szStartTime));	strncpy(szStartTime,"09:00:00",8);

		CEasyTime tStartTime;
		tStartTime.settime(szStartDate,szStartTime);

		std::map<std::string,map<time_t,KLineBase>>::iterator it = m_vmapKLineMin1.find(pData->InstrumentID);
		if(it!=m_vmapKLineMin1.end())
		{
			map<time_t,KLineBase> vTemp = it->second;
			for (map<time_t,KLineBase>::iterator it_data = vTemp.begin(); it_data != vTemp.end(); it_data++)
			{
				if(it_data->first < tStartTime.getutc())
					continue;

				TLineBase lineBase;
				lineBase.dwTime = it_data->second.dwTime;
				lineBase.fLastPrice = it_data->second.fClosePrice;
				lineBase.dwVolume = it_data->second.dwVolume;
				lineBase.dwOpenInterest = it_data->second.dwOpenInterest;
				lineBase.fTurnover = it_data->second.fTurnover;
				outTLData.push_back(lineBase);
			}
		}
	}else if(pData->nDataType==1)//K线
	{
		std::map<std::string,map<time_t,KLineBase>>::iterator it = m_vmapKLineMin1.find(pData->InstrumentID);
		if(it!=m_vmapKLineMin1.end())
		{
			map<time_t,KLineBase> vTemp = it->second;

			map<time_t,KLineBase>::const_iterator it_lower = vTemp.lower_bound(pData->szStartTime);
			map<time_t,KLineBase>::const_iterator it_upper = vTemp.upper_bound(pData->szEndTime);
			if(it_lower == it_upper)
				return false;

			for (; it_lower != it_upper; it_lower++)
				outData.push_back(it_lower->second);
		}
	}


	return (outData.size()>0 || outTLData.size()>0);

}
bool CPlatformCTPQuotImpl::GetKLineMin5(KLineQuery* pData,vector<KLineBase>& outData,vector<TLineBase>& outTLData)
{
	CeasymutexGuard guard(m_mutexKLineM5);
	if(pData->nDataType==2)//分时线 今日所有数据
	{
		std::map<std::string,map<time_t,KLineBase>>::iterator it = m_vmapKLineMin5.find(pData->InstrumentID);
		if(it!=m_vmapKLineMin5.end())
		{
			map<time_t,KLineBase> vTemp = it->second;
			for (map<time_t,KLineBase>::iterator it_data = vTemp.begin(); it_data != vTemp.end(); it_data++)
				outData.push_back(it_data->second);
		}
	}else if(pData->nDataType==1)//K线
	{

			std::map<std::string,map<time_t,KLineBase>>::iterator it = m_vmapKLineMin5.find(pData->InstrumentID);
			if(it!=m_vmapKLineMin5.end())
			{
				map<time_t,KLineBase> vTemp = it->second;

				map<time_t,KLineBase>::const_iterator it_lower = vTemp.lower_bound(pData->szStartTime);
				map<time_t,KLineBase>::const_iterator it_upper = vTemp.upper_bound(pData->szEndTime);
				if(it_lower == it_upper)
					return false;

				for (; it_lower != it_upper; it_lower++)
					outData.push_back(it_lower->second);
			}

	}

	return outData.size()>0;

}
bool CPlatformCTPQuotImpl::GetKLineMin15(KLineQuery* pData,vector<KLineBase>& outData,vector<TLineBase>& outTLData)
{
	CeasymutexGuard guard(m_mutexKLineM15);
	if(pData->nDataType==2)//分时线 今日所有数据
	{
		std::map<std::string,map<time_t,KLineBase>>::iterator it = m_vmapKLineMin15.find(pData->InstrumentID);
		if(it!=m_vmapKLineMin15.end())
		{
			map<time_t,KLineBase> vTemp = it->second;
			for (map<time_t,KLineBase>::iterator it_data = vTemp.begin(); it_data != vTemp.end(); it_data++)
				outData.push_back(it_data->second);
		}
	}else if(pData->nDataType==1)//K线
	{
		
			std::map<std::string,map<time_t,KLineBase>>::iterator it = m_vmapKLineMin15.find(pData->InstrumentID);
			if(it!=m_vmapKLineMin15.end())
			{
				map<time_t,KLineBase> vTemp = it->second;

				map<time_t,KLineBase>::const_iterator it_lower = vTemp.lower_bound(pData->szStartTime);
				map<time_t,KLineBase>::const_iterator it_upper = vTemp.upper_bound(pData->szEndTime);
				if(it_lower == it_upper)
					return false;

				for (; it_lower != it_upper; it_lower++)
					outData.push_back(it_lower->second);
			}

	}
	return outData.size()>0;
}
bool CPlatformCTPQuotImpl::GetKLineMin30(KLineQuery* pData,vector<KLineBase>& outData,vector<TLineBase>& outTLData)
{
	CeasymutexGuard guard(m_mutexKLineM30);
	if(pData->nDataType==2)//分时线 今日所有数据
	{
		std::map<std::string,map<time_t,KLineBase>>::iterator it = m_vmapKLineMin30.find(pData->InstrumentID);
		if(it!=m_vmapKLineMin30.end())
		{
			map<time_t,KLineBase> vTemp = it->second;
			for (map<time_t,KLineBase>::iterator it_data = vTemp.begin(); it_data != vTemp.end(); it_data++)
				outData.push_back(it_data->second);
		}
	}else if(pData->nDataType==1)//K线
	{
		
			std::map<std::string,map<time_t,KLineBase>>::iterator it = m_vmapKLineMin30.find(pData->InstrumentID);
			if(it!=m_vmapKLineMin30.end())
			{
				map<time_t,KLineBase> vTemp = it->second;

				map<time_t,KLineBase>::const_iterator it_lower = vTemp.lower_bound(pData->szStartTime);
				map<time_t,KLineBase>::const_iterator it_upper = vTemp.upper_bound(pData->szEndTime);
				if(it_lower == it_upper)
					return false;

				for (; it_lower != it_upper; it_lower++)
					outData.push_back(it_lower->second);
			}

	}
	return outData.size()>0;

}
bool CPlatformCTPQuotImpl::GetKLineMin60(KLineQuery* pData,vector<KLineBase>& outData,vector<TLineBase>& outTLData)
{
	CeasymutexGuard guard(m_mutexKLineM60);
	if(pData->nDataType==2)//分时线 今日所有数据
	{
		std::map<std::string,map<time_t,KLineBase>>::iterator it = m_vmapKLineMin60.find(pData->InstrumentID);
		if(it!=m_vmapKLineMin60.end())
		{
			map<time_t,KLineBase> vTemp = it->second;
			for (map<time_t,KLineBase>::iterator it_data = vTemp.begin(); it_data != vTemp.end(); it_data++)
				outData.push_back(it_data->second);
		}
	}else if(pData->nDataType==1)//K线
	{
		
			std::map<std::string,map<time_t,KLineBase>>::iterator it = m_vmapKLineMin60.find(pData->InstrumentID);
			if(it!=m_vmapKLineMin60.end())
			{
				map<time_t,KLineBase> vTemp = it->second;

				map<time_t,KLineBase>::const_iterator it_lower = vTemp.lower_bound(pData->szStartTime);
				map<time_t,KLineBase>::const_iterator it_upper = vTemp.upper_bound(pData->szEndTime);
				if(it_lower == it_upper)
					return false;

				for (; it_lower != it_upper; it_lower++)
					outData.push_back(it_lower->second);
			}

	}
	return outData.size()>0;
}
bool CPlatformCTPQuotImpl::GetKLineDay(KLineQuery* pData,vector<KLineBase>& outData,vector<TLineBase>& outTLData)
{
	CeasymutexGuard guard(m_mutexKLineDay);
	if(pData->nDataType==2)//分时线 今日所有数据
	{
		std::map<std::string,map<time_t,KLineBase>>::iterator it = m_vmapKLineDay.find(pData->InstrumentID);
		if(it!=m_vmapKLineDay.end())
		{
			map<time_t,KLineBase> vTemp = it->second;
			for (map<time_t,KLineBase>::iterator it_data = vTemp.begin(); it_data != vTemp.end(); it_data++)
				outData.push_back(it_data->second);
		}
	}else if(pData->nDataType==1)//K线
	{
		
			std::map<std::string,map<time_t,KLineBase>>::iterator it = m_vmapKLineDay.find(pData->InstrumentID);
			if(it!=m_vmapKLineDay.end())
			{
				map<time_t,KLineBase> vTemp = it->second;

				map<time_t,KLineBase>::const_iterator it_lower = vTemp.lower_bound(pData->szStartTime);
				map<time_t,KLineBase>::const_iterator it_upper = vTemp.upper_bound(pData->szEndTime);
				if(it_lower == it_upper)
					return false;

				for (; it_lower != it_upper; it_lower++)
					outData.push_back(it_lower->second);
			}
		
	}
	return outData.size()>0;
}
bool CPlatformCTPQuotImpl::GetKLineWeek(KLineQuery* pData,vector<KLineBase>& outData,vector<TLineBase>& outTLData)
{
	CeasymutexGuard guard(m_mutexKLineWeek);
	if(pData->nDataType==2)//分时线 今日所有数据
	{
		std::map<std::string,map<time_t,KLineBase>>::iterator it = m_vmapKLineWeek.find(pData->InstrumentID);
		if(it!=m_vmapKLineWeek.end())
		{
			map<time_t,KLineBase> vTemp = it->second;
			for (map<time_t,KLineBase>::iterator it_data = vTemp.begin(); it_data != vTemp.end(); it_data++)
				outData.push_back(it_data->second);
		}
	}else if(pData->nDataType==1)//K线
	{
		
			std::map<std::string,map<time_t,KLineBase>>::iterator it = m_vmapKLineWeek.find(pData->InstrumentID);
			if(it!=m_vmapKLineWeek.end())
			{
				map<time_t,KLineBase> vTemp = it->second;

				map<time_t,KLineBase>::const_iterator it_lower = vTemp.lower_bound(pData->szStartTime);
				map<time_t,KLineBase>::const_iterator it_upper = vTemp.upper_bound(pData->szEndTime);
				if(it_lower == it_upper)
					return false;

				for (; it_lower != it_upper; it_lower++)
					outData.push_back(it_lower->second);
			}
		
	}

	return outData.size()>0;
}
bool CPlatformCTPQuotImpl::GetKLineMonth(KLineQuery* pData,vector<KLineBase>& outData,vector<TLineBase>& outTLData)
{
	CeasymutexGuard guard(m_mutexKLineMonth);
	if(pData->nDataType==2)//分时线 今日所有数据
	{
		std::map<std::string,map<time_t,KLineBase>>::iterator it = m_vmapKLineMonth.find(pData->InstrumentID);
		if(it!=m_vmapKLineMonth.end())
		{
			map<time_t,KLineBase> vTemp = it->second;
			for (map<time_t,KLineBase>::iterator it_data = vTemp.begin(); it_data != vTemp.end(); it_data++)
				outData.push_back(it_data->second);
		}
	}else if(pData->nDataType==1)//K线
	{
		
			std::map<std::string,map<time_t,KLineBase>>::iterator it = m_vmapKLineMonth.find(pData->InstrumentID);
			if(it!=m_vmapKLineMonth.end())
			{
				map<time_t,KLineBase> vTemp = it->second;

				map<time_t,KLineBase>::const_iterator it_lower = vTemp.lower_bound(pData->szStartTime);
				map<time_t,KLineBase>::const_iterator it_upper = vTemp.upper_bound(pData->szEndTime);
				if(it_lower == it_upper)
					return false;

				for (; it_lower != it_upper; it_lower++)
					outData.push_back(it_lower->second);
			}
		
	}
	return outData.size()>0;
}

bool CPlatformCTPQuotImpl::RecalcWeekMonthKLine()
{
	if(m_bHasRecalcWeekMonthKLine)
		return false;
	if(m_strTradingDay.empty())
		return false;
	if(m_vmapKLineDay.empty())
		return false;

	m_bHasRecalcWeekMonthKLine = true;

	time_t times = 0;
	int nDate = atoi(m_strTradingDay.c_str());
	struct tm tradingdayTime = {0};
	struct tm weekTime = {0};
	struct tm monthTime = {0};

	tradingdayTime.tm_year = nDate/10000 - 1900;
	tradingdayTime.tm_mon = ((nDate/100)%100)-1;
	tradingdayTime.tm_mday = nDate%100;
	
	for(int i=0; i<7; i++) {
		times = mktime(&tradingdayTime);
		times -= i*24*3600;
		weekTime = *localtime(&times);
		if(weekTime.tm_wday==1)
			break;
	}

	monthTime = tradingdayTime;
	monthTime.tm_mday = 1;

	{
		CeasymutexGuard guard(m_mutexKLineDay);
		CeasymutexGuard guard2(m_mutexKLineWeek);
		std::map<std::string,std::map<time_t, KLineBase>>::iterator it;
		std::map<time_t, KLineBase>::iterator itKLine;
		for(it = m_vmapKLineDay.begin(); it != m_vmapKLineDay.end(); it++) {
			
			bool bFirst = true;
			time_t startTime = mktime(&weekTime);
			KLineBase kLineSum;
			memset(&kLineSum, 0, sizeof(kLineSum));
			
			for(itKLine = it->second.begin(); itKLine != it->second.end(); itKLine++) {
				if(itKLine->first >= startTime)
					break;
			}
			for(; itKLine != it->second.end(); itKLine++) {
				if(bFirst) {
					kLineSum = itKLine->second;
					bFirst = false;
				}
				else {
					if(kLineSum.fHighPrice < itKLine->second.fHighPrice)
						kLineSum.fHighPrice = itKLine->second.fHighPrice;
					if(kLineSum.fLowPrice > itKLine->second.fLowPrice)
						kLineSum.fLowPrice = itKLine->second.fLowPrice;
					kLineSum.fClosePrice = itKLine->second.fClosePrice;
					kLineSum.fTurnover += itKLine->second.fTurnover;
					kLineSum.dwVolume += itKLine->second.dwVolume;
					kLineSum.dwOpenInterest = itKLine->second.dwOpenInterest;
				}
			}

			if(!bFirst) {
				std::map<std::string, vector<KLineBase>>::iterator itK = m_mapKLineWeek.find(it->first);
				if (itK == m_mapKLineWeek.end()) {
					vector<KLineBase> vecKLineMin;
					vecKLineMin.push_back(kLineSum);
					m_mapKLineWeek[it->first] = vecKLineMin;
				}
				else {
					itK->second.push_back(kLineSum);
				}
			}
		} 

	} 

	{
		CeasymutexGuard guard(m_mutexKLineDay);
		CeasymutexGuard guard2(m_mutexKLineMonth);
		std::map<std::string,std::map<time_t, KLineBase>>::iterator it;
		std::map<time_t, KLineBase>::iterator itKLine;
		for(it = m_vmapKLineDay.begin(); it != m_vmapKLineDay.end(); it++) {
			
			bool bFirst = true;
			time_t startTime = mktime(&monthTime);
			KLineBase kLineSum;
			memset(&kLineSum, 0, sizeof(kLineSum));
			
			for(itKLine = it->second.begin(); itKLine != it->second.end(); itKLine++) {
				if(itKLine->first >= startTime)
					break;
			}
			for(; itKLine != it->second.end(); itKLine++) {
				if(bFirst) {
					kLineSum = itKLine->second;
					bFirst = false;
				}
				else {
					if(kLineSum.fHighPrice < itKLine->second.fHighPrice)
						kLineSum.fHighPrice = itKLine->second.fHighPrice;
					if(kLineSum.fLowPrice > itKLine->second.fLowPrice)
						kLineSum.fLowPrice = itKLine->second.fLowPrice;
					kLineSum.fClosePrice = itKLine->second.fClosePrice;
					kLineSum.fTurnover += itKLine->second.fTurnover;
					kLineSum.dwVolume += itKLine->second.dwVolume;
					kLineSum.dwOpenInterest = itKLine->second.dwOpenInterest;
				}
			}

			if(!bFirst) {
				std::map<std::string, vector<KLineBase>>::iterator itK = m_mapKLineMonth.find(it->first);
				if (itK == m_mapKLineMonth.end()) {
					vector<KLineBase> vecKLineMin;
					vecKLineMin.push_back(kLineSum);
					m_mapKLineMonth[it->first] = vecKLineMin;
				}
				else {
					itK->second.push_back(kLineSum);
				}
			}
		} 
	}

	return true;
}
