#include "stdafx.h"
#include "SendData.h"
#include "DataManager.h"

CSendData* CSendData::m_SendData=NULL;
CSendData::CSendData(void)
{
	ReadTodaySaveData();
	m_hWorkerThread =new CToolThread(ThreadFunc, this);
	InitializeCriticalSection(&m_CS);
	
}

CSendData::~CSendData(void)
{
	if(m_hWorkerThread)
	{
		delete m_hWorkerThread;
		m_hWorkerThread = NULL;
	}	

	RecvData reqData;
	while( m_cmdMsgQueue.GetMsg(reqData) )
	{
		if ( NULL != reqData.pData )
		{
			delete[] reqData.pData;
		}
	}
	RecvDataMsg reqDataMsg;
	while( m_cmdClientMsgQueue.GetMsg(reqDataMsg) )
	{
		if ( NULL != reqDataMsg.pData )
		{
			delete[] reqDataMsg.pData;
		}
	}
	DeleteCriticalSection(&m_CS);
}
void CSendData::Init(void)
{
	if(m_SendData)
	{
		Release();
	}
	m_SendData=new CSendData();
}
void CSendData::Release(void)
{
	if(m_SendData)
	{
		delete m_SendData;
		m_SendData = NULL;
	}
}
CSendData* CSendData::Get()
{
	return m_SendData;
}

DWORD CSendData::ThreadFunc( void* pData )
{
	CSendData* pWorker = (CSendData*)pData;
	if(pWorker)
		pWorker->Process();
	return 0;
}

void CSendData::Process()
{
	if (NULL==m_hWorkerThread)
	{
		return;
	}
	while(!m_hWorkerThread->IsNeedExit())
	{
		RecvData reqData;
		RecvDataMsg reqDataMsg;
		if(GetClientMsg(reqDataMsg))
		{//监控终端的请求
			SendClientData(reqDataMsg);
		}
		else if (GetMsg(reqData))
		{//从交易端过来数据，转送客户端数据
			writeLog(reqData);
			SendData(reqData);
			if(reqData.head.cmdid != 0x18020001)
			{				
				CalcData(reqData);
				SendCaclData(reqData);
				SendCountData();	
			}
			//这里还要trace输出.....
			if(reqData.pData)
			{
				delete []reqData.pData;
				reqData.pData = NULL;
			}
		}
		else
		{
			//空闲时自动执行动作
			Sleep(10);
			continue;
		}
	}
}
void CSendData::SendUDPData(int nCmdID, SOCKET hSocket,Stru_IPAddress RemoteAddr,const void* pData,int DataLen)
{
	Stru_UniPkgUDPHead head;
	head.cmdid = nCmdID;
	head.len = DataLen;

	int nTotalLen = sizeof(Stru_UniPkgUDPHead)+DataLen +4;
	char* pBuf = new char[nTotalLen];
	memset(pBuf, 0, sizeof(nTotalLen));
	auto_ptr<char> ptrBuf( pBuf );
	if ( NULL == pBuf )
	{
		return ;
	}			
	memcpy(pBuf, &head, sizeof(Stru_UniPkgUDPHead));
	memcpy(pBuf+sizeof(Stru_UniPkgUDPHead), pData, DataLen);
	TAIL tail = TailID_UniPkg;
	memcpy(pBuf+sizeof(Stru_UniPkgUDPHead)+DataLen, &tail, sizeof(TAIL));

	CSocketBase::SendDataUdp(hSocket, RemoteAddr,pBuf,sizeof(Stru_UniPkgUDPHead)+DataLen +4);	
}
void CSendData::CalcData(RecvData& cData)
{
	ServerOrder order;
	order.cmdid = cData.head.cmdid;
	order.len	= cData.head.len;
	order.seq   = cData.head.seq;
	order.userdata1 = cData.head.userdata1;
	order.userdata2 = cData.head.userdata2;
	order.userdata3 = cData.head.userdata3;
	order.userdata4 = cData.head.userdata4;
	order.nUTCTime  = cData.head.nUTCTime;
	order.nMiliSecond = cData.head.nMiliSecond;

	CDataManager::Get()->AppendServerOrder(order);
	CDataManager::Get()->AddStatisticsData(order);
	CDataManager::Get()->CalcDelay(order);	

	CDataManager::Get()->AddStatisticsData_Min(order);
}
void CSendData::SendClientData(RecvDataMsg& reqDataMsg)
{
	if(reqDataMsg.head.cmdid == CMDID_HEARTBEAT)
	{
		int nTime = 0;
		CDataManager::Get()->SetClinetUnConnectTime(reqDataMsg.RemoteAddr, reqDataMsg.hSocket, nTime);	

		Stru_UniPkgUDPHead Pkg;
		Pkg.cmdid = CMDID_HEARTBEATRSP;
		Pkg.len = 0;
		char pBuf[sizeof(Stru_UniPkgUDPHead)+0+4];
		memcpy(pBuf, &Pkg, sizeof(Stru_UniPkgUDPHead));	
		TAIL tail = TailID_UniPkg;
		memcpy(pBuf+sizeof(Stru_UniPkgUDPHead)+Pkg.len, &tail, sizeof(TAIL));

		CSocketBase::SendDataUdp(reqDataMsg.hSocket,reqDataMsg.RemoteAddr,pBuf,sizeof(Stru_UniPkgUDPHead)+0+4);			
	}
	else if(reqDataMsg.head.cmdid == MONITOR_GETSTATISITIC)
	{
		std::vector<StatisticsData> vecData;
		CDataManager::Get()->GetAllStatics_Min(vecData);	
		if(vecData.size()>0)
		{
			int nSendCount = 500;
			if(vecData.size() >nSendCount)
			{	//包太大的情况下分包发送
				int nSize = vecData.size()/nSendCount+1;
				int nLast = vecData.size()%nSendCount;
				for(int n = 0; n < nSize; n++)
				{    
					if(nLast != 0 && n == nSize -1)
					{
						SendUDPData(MONITOR_GETSTATISITIC_Rsp, reqDataMsg.hSocket,  reqDataMsg.RemoteAddr,&vecData[n*nSendCount], nLast);
					}
					else
						SendUDPData(MONITOR_GETSTATISITIC_Rsp, reqDataMsg.hSocket,  reqDataMsg.RemoteAddr,&vecData[n*nSendCount],nSendCount * sizeof(StatisticsData));
				}
			}
			else
				SendUDPData(MONITOR_GETSTATISITIC_Rsp, reqDataMsg.hSocket,  reqDataMsg.RemoteAddr,&vecData[0],vecData.size() * sizeof(StatisticsData));
			
		}
		else
			SendUDPData(MONITOR_GETSTATISITIC_Rsp, reqDataMsg.hSocket,  reqDataMsg.RemoteAddr,NULL,0);

	}
}
void CSendData::SendCaclData(RecvData& cData)
{
	StatisticsData data;	
	time_t timeT = (time_t)cData.head.nUTCTime-120;//往前退一秒，因为所有计数都往后推一秒
	struct tm TM = *localtime(&timeT);	
	struct tm  tmTime = CDataManager::Get()->GetTimeInSpan(TM,  60);
	time_t t1 = mktime( &tmTime);
	DWORD dwTime = t1;
	bool bFind = CDataManager::Get()->GetStatisticsData_Min(dwTime, data);
	if(bFind == false)
		return;

	Stru_UniPkgUDPHead head;
	head.cmdid = MONITOR_GETSTATISITIC_Push;
	head.len = sizeof(StatisticsData);

	char pBuf[sizeof(Stru_UniPkgUDPHead)+sizeof(StatisticsData)+4];
	memcpy(pBuf, &head, sizeof(Stru_UniPkgUDPHead));
	memcpy(pBuf+sizeof(Stru_UniPkgUDPHead), &data, sizeof(StatisticsData));
	TAIL tail = TailID_UniPkg;
	memcpy(pBuf+sizeof(Stru_UniPkgUDPHead)+sizeof(StatisticsData), &tail, sizeof(TAIL));

	std::map<SClientKey, int> mapClientUnConnectTime;
	CDataManager::Get()->GetAllUnconnetTime(mapClientUnConnectTime);	
	std::map<SClientKey, int>::iterator it = mapClientUnConnectTime.begin();
	while(it != mapClientUnConnectTime.end())
	{
		const SClientKey& key = it->first;
		CSocketBase::SendDataUdp(key.hSocket,key.RemoteAddr,pBuf,sizeof(Stru_UniPkgUDPHead)+sizeof(StatisticsData)+4);	
		it++;
	}	
}
void CSendData::SendCountData()
{
	StatisticsData data;
	CDataManager::Get()->GetCountStatics(data);
	

	Stru_UniPkgUDPHead head;
	head.cmdid = MONITOR_SENDCALCDATA_Rsp;
	head.len = sizeof(StatisticsData);

	char pBuf[sizeof(Stru_UniPkgUDPHead)+sizeof(StatisticsData)+4];
	memcpy(pBuf, &head, sizeof(Stru_UniPkgUDPHead));
	memcpy(pBuf+sizeof(Stru_UniPkgUDPHead), &data, sizeof(StatisticsData));
	TAIL tail = TailID_UniPkg;
	memcpy(pBuf+sizeof(Stru_UniPkgUDPHead)+sizeof(StatisticsData), &tail, sizeof(TAIL));

	std::map<SClientKey, int> mapClientUnConnectTime;
	CDataManager::Get()->GetAllUnconnetTime(mapClientUnConnectTime);	
	std::map<SClientKey, int>::iterator it = mapClientUnConnectTime.begin();
	while(it != mapClientUnConnectTime.end())
	{
		const SClientKey& key = it->first;
		CSocketBase::SendDataUdp(key.hSocket,key.RemoteAddr,pBuf,sizeof(Stru_UniPkgUDPHead)+sizeof(StatisticsData)+4);	
		it++;
	}	
}
void CSendData::AddMsg( const RecvData& data )
{
	EnterCriticalSection(&m_CS);
	m_cmdMsgQueue.AddMsg(data);
	LeaveCriticalSection(&m_CS);
}

bool CSendData::GetMsg( RecvData& data )
{
	bool bRet = false;
	EnterCriticalSection(&m_CS);
	bRet = m_cmdMsgQueue.GetMsg(data);
	LeaveCriticalSection(&m_CS);
	return bRet;
}
void CSendData::AddClientMsg( const RecvDataMsg& data )
{
	EnterCriticalSection(&m_CS);
	m_cmdClientMsgQueue.AddMsg(data);
	LeaveCriticalSection(&m_CS);
}

bool CSendData::GetClientMsg( RecvDataMsg& data )
{
	bool bRet = false;
	EnterCriticalSection(&m_CS);
	bRet = m_cmdClientMsgQueue.GetMsg(data);
	LeaveCriticalSection(&m_CS);
	return bRet;
}

void CSendData::SendData(RecvData& cData)
{
	int nTotalLen = sizeof(Stru_UniPkgUDPHead)+cData.head.len+sizeof(TAIL);
	char* pBuf = new char[nTotalLen];
	memset(pBuf, 0, sizeof(nTotalLen));
	auto_ptr<char> ptrBuf( pBuf );
	if ( NULL == pBuf )
	{
		return ;
	}
	memcpy(pBuf, &cData.head, sizeof(Stru_UniPkgUDPHead));

	memcpy(pBuf+sizeof(Stru_UniPkgUDPHead), cData.pData, cData.head.len);

	//Stru_UniPkgUDPHead test;

	//memcpy(&test, pBuf, sizeof(Stru_UniPkgUDPHead));

	TAIL tail = TailID_UniPkg;
	memcpy(pBuf+sizeof(Stru_UniPkgUDPHead)+cData.head.len, &tail, sizeof(TAIL));

	std::map<SClientKey, int> mapClientUnConnectTime;
	CDataManager::Get()->GetAllUnconnetTime(mapClientUnConnectTime);	
	std::map<SClientKey, int>::iterator it = mapClientUnConnectTime.begin();
	while(it != mapClientUnConnectTime.end())
	{
		const SClientKey& key = it->first;
		CSocketBase::SendDataUdp(key.hSocket,key.RemoteAddr,pBuf,nTotalLen);	
		it++;
	}		
}
void CSendData::AppendErrorString(char* str,  CThostFtdcRspInfoField* pData, char* strOutPut)
{
	if(str == NULL || strOutPut == NULL)
		return;

	if(pData)
	{
		memset(strOutPut, 0, sizeof(strOutPut));
		sprintf(strOutPut,"%s;[%d %s]", str, pData->ErrorID, pData->ErrorMsg);
	}	
	else
		strcpy(strOutPut, str);
}
void CSendData::writeLog(RecvData& cData)
{
	char BufHead[512];
	memset(BufHead, 0, sizeof(BufHead));	
	sprintf(BufHead,"CmdID:[%d]; Seq:[%d]; Len:[%d]; userData1:[%d]; userData2:[%d]; userData3:[%d]; userData4:[%d]; utc:[%d]; milliSecond:[%d]", 
		cData.head.cmdid,  cData.head.seq, cData.head.len, cData.head.userdata1, cData.head.userdata2,
		cData.head.userdata3, cData.head.userdata4, cData.head.nUTCTime, cData.head.nMiliSecond);
	if(cData.head.cmdid != 0x18020001)
	{
		Monitor_info("%s", BufHead);
	}
	else
	{
		char BufHead[512];
		memset(BufHead, 0 , sizeof(BufHead));
		if(cData.pData)
		{
			Stru_Watch *pWatch = (Stru_Watch *)cData.pData;
			if(pWatch)
			{
				sprintf(BufHead,"CPU: %02.1f%%, \tMemory: %5I64dM, \tAvailDiskSpace: %5I64dG\n", pWatch->CpuUsage/10.0, pWatch->Memory/1024/1024, 
					pWatch->DiskSpace/1024/1024/1024);
				MonitorCpu_info("%s", BufHead);
			}
		}
	}
	


/*	char Buff[512];
	memset(Buff, 0, sizeof(Buff));	
	
	switch (cData.head.cmdid)
	{
		case FFT_API_START:
			break;
			///用户登录请求
		case FTTCMD_ReqUserLogin:
			{
				CThostFtdcReqUserLoginField* pData = (CThostFtdcReqUserLoginField*)cData.pData;
				if(pData)								
					sprintf(Buff,"%s;UserID;[%s]", BufHead, pData->UserID);				
				else
					strcpy(Buff, BufHead);
				break;
			}
		case FTTCMD_ReqUserLogin_Rsp:
			{
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break;
			}

		///登出请求
		case FTTCMD_ReqUserLogout:
			{
				CThostFtdcUserLogoutField* pData = (CThostFtdcUserLogoutField*)cData.pData;
				if(pData)								
					sprintf(Buff,"%s;UserID;[%s]", BufHead, pData->UserID);
				else
					strcpy(Buff, BufHead);
				break; 
			}
		case FTTCMD_ReqUserLogout_Rsp: 
			{
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///用户口令更新请求
		case FTTCMD_ReqUserPasswordUpdate: 
			{ 
				PlatformStru_UserPasswordUpdate* pData = (PlatformStru_UserPasswordUpdate*)cData.pData;		
				if(pData)							
					sprintf(Buff,"%s;UserID;[%s]", BufHead, pData->UserID);				
				else
					strcpy(Buff, BufHead);
				break; 
			} 
		case FTTCMD_ReqUserPasswordUpdate_Rsp: 
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///资金账户口令更新请求
		case FTTCMD_ReqTradingAccountPasswordUpdate: 
			{
				PlatformStru_TradingAccountPasswordUpdate* pData = (PlatformStru_TradingAccountPasswordUpdate*)cData.pData;
				if(pData)							
					sprintf(Buff,"%s;AccountID;[%s]", BufHead, pData->AccountID);
				else
					strcpy(Buff, BufHead);
				break;
			}
		case FTTCMD_ReqTradingAccountPasswordUpdate_Rsp:
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}


		///报单录入请求
		case FTTCMD_ReqOrderInsert: 
			{ 
				PlatformStru_InputOrder* pData = (PlatformStru_InputOrder*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;InvestorID:[%s]; InstrumentID:[%s]; OrderRef:[%s]", BufHead, pData->InvestorID, pData->InstrumentID, pData->OrderRef);
				else
					strcpy(Buff, BufHead);

				break; 
			} 
		case FTTCMD_ReqOrderInsert_Rsp: 
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///预埋单录入请求
		case FTTCMD_ReqParkedOrderInsert: 
			{ 
				PlatformStru_ParkedOrder* pData = (PlatformStru_ParkedOrder*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;UserID:[%s]; InstrumentID:[%s]; OrderRef:[%s]", BufHead, pData->UserID, pData->InstrumentID, pData->OrderRef);
				else
					strcpy(Buff, BufHead);
				break; 
			} 
		case FTTCMD_ReqParkedOrderInsert_Rsp: 
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///预埋撤单录入请求
		case FTTCMD_ReqParkedOrderAction: 
			{
				PlatformStru_ParkedOrderAction* pData = (PlatformStru_ParkedOrderAction*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;InvestorID:[%s]; InstrumentID:[%s]; OrderRef:[%s]; FrontID:[%d]; SessionID:[%d]", BufHead, pData->Thost.InvestorID, pData->Thost.InstrumentID, pData->Thost.OrderRef, pData->Thost.FrontID, pData->Thost.SessionID);
				else
					strcpy(Buff, BufHead);
				break; 
			} 
		case FTTCMD_ReqParkedOrderAction_Rsp: 
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///报单操作请求
		case FTTCMD_ReqOrderAction: 
			{ 
				PlatformStru_InputOrderAction* pData = (PlatformStru_InputOrderAction*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;InvestorID:[%s]; InstrumentID:[%s]; OrderRef:[%s]; FrontID:[%d]; SessionID:[%d]", BufHead, pData->Thost.InvestorID, pData->Thost.InstrumentID, pData->Thost.OrderRef, pData->Thost.FrontID, pData->Thost.SessionID);
				else
					strcpy(Buff, BufHead);
				break; 
			}
		case FTTCMD_ReqOrderAction_Rsp: 
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///查询最大报单数量请求
		case FTTCMD_ReqQueryMaxOrderVolume: 
			{ 
				PlatformStru_QueryMaxOrderVolume* pData = (PlatformStru_QueryMaxOrderVolume*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;InvestorID:[%s]; InstrumentID:[%s];", BufHead, pData->Thost.InvestorID, pData->Thost.InstrumentID);
				else
					strcpy(Buff, BufHead);
				break; 
			}
		case FTTCMD_ReqQueryMaxOrderVolume_Rsp: 
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///投资者结算结果确认
		case FTTCMD_ReqSettlementInfoConfirm: 
			{
				CThostFtdcSettlementInfoConfirmField* pData = (CThostFtdcSettlementInfoConfirmField*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;InvestorID:[%s]", BufHead, pData->InvestorID);
				else
					strcpy(Buff, BufHead);
				break; 
			}
		case FTTCMD_ReqSettlementInfoConfirm_Rsp: 
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///请求删除预埋单
		case FTTCMD_ReqRemoveParkedOrder: 
			{ 
				PlatformStru_RemoveParkedOrder* pData = (PlatformStru_RemoveParkedOrder*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;InvestorID:[%s]", BufHead, pData->Thost.InvestorID);
				else
					strcpy(Buff, BufHead);
				break;
			}
		case FTTCMD_ReqRemoveParkedOrder_Rsp: 
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///请求删除预埋撤单
		case FTTCMD_ReqRemoveParkedOrderAction: 
			{ 
				PlatformStru_RemoveParkedOrderAction* pData = (PlatformStru_RemoveParkedOrderAction*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;InvestorID:[%s]; ParkedOrderActionID:[%s]", BufHead, pData->Thost.InvestorID, pData->Thost.ParkedOrderActionID);
				else
					strcpy(Buff, BufHead);
				break; 
			}
		case FTTCMD_ReqRemoveParkedOrderAction_Rsp: 
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///询价录入请求
		case FTTCMD_ReqForQuoteInsert: 
			{
				PlatformStru_InputForQuoteField* pData = (PlatformStru_InputForQuoteField*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;InvestorID:[%s]; InstrumentID:[%s]", BufHead, pData->InvestorID, pData->InstrumentID);
				else
					strcpy(Buff, BufHead);
				break;
			}
		case FTTCMD_ReqForQuoteInsert_Rsp: 
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///执行宣告录入请求
		case FTTCMD_ReqExecOrderInsert: 
			{ 
				PlatformStru_InputExecOrderField* pData = (PlatformStru_InputExecOrderField*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;InvestorID:[%s]; InstrumentID:[%s]", BufHead, pData->InvestorID, pData->InstrumentID);
				else
					strcpy(Buff, BufHead);
				break; 
			}
		case FTTCMD_ReqExecOrderInsert_Rsp: 
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///执行宣告操作请求
		case FTTCMD_ReqExecOrderAction: 
			{ 
				PlatformStru_InputExecOrderActionField* pData = (PlatformStru_InputExecOrderActionField*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;InvestorID:[%s]; InstrumentID:[%s]; FrontID:[%d]; SessionID:[%d]", BufHead, pData->InvestorID, pData->InstrumentID, pData->FrontID, pData->SessionID);
				else
					strcpy(Buff, BufHead);
				break; 
			}
		case FTTCMD_ReqExecOrderAction_Rsp: 
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///请求查询报单
		case FTTCMD_ReqQryOrder: 
			{ 
				PlatformStru_QryOrder* pData = (PlatformStru_QryOrder*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;InvestorID:[%s]; InstrumentID:[%s]; OrderSysID:[%d]", BufHead, pData->Thost.InvestorID, pData->Thost.InstrumentID, pData->Thost.OrderSysID);
				else
					strcpy(Buff, BufHead);
				break; 
			}
		case FTTCMD_ReqQryOrder_Rsp: 
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///请求查询成交
		case FTTCMD_ReqQryTrade: 
			{ 
				PlatformStru_QryTrade* pData = (PlatformStru_QryTrade*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;InvestorID:[%s]; InstrumentID:[%s]", BufHead, pData->Thost.InvestorID, pData->Thost.InstrumentID);
				else
					strcpy(Buff, BufHead);
				break; 
			}
		case FTTCMD_ReqQryTrade_Rsp: 
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///请求查询投资者持仓
		case FTTCMD_ReqQryInvestorPosition: 
			{ 
				PlatformStru_QryInvestorPosition* pData = (PlatformStru_QryInvestorPosition*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;InvestorID:[%s]; InstrumentID:[%s]", BufHead, pData->InvestorID, pData->InstrumentID);
				else
					strcpy(Buff, BufHead);
				break; 
			}
		case FTTCMD_ReqQryInvestorPosition_Rsp: 
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///请求查询资金账户
		case FTTCMD_ReqQryTradingAccount: 
			{ 
				PlatformStru_QryTradingAccount* pData = (PlatformStru_QryTradingAccount*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;InvestorID:[%s]", BufHead, pData->InvestorID);
				else
					strcpy(Buff, BufHead);
				break; 
			}
		case FTTCMD_ReqQryTradingAccount_Rsp: 
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///请求查询投资者
		case FTTCMD_ReqQryInvestor: 
			{
				PlatformStru_QryInvestor* pData = (PlatformStru_QryInvestor*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;InvestorID:[%s]", BufHead, pData->Thost.InvestorID);
				else
					strcpy(Buff, BufHead);
				break; 
			}
		case FTTCMD_ReqQryInvestor_Rsp: 
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///请求查询合约保证金率
		case FTTCMD_ReqQryInstrumentMarginRate: 
			{ 
				CThostFtdcQryInstrumentMarginRateField* pData = (CThostFtdcQryInstrumentMarginRateField*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;InvestorID:[%s]; InstrumentID:[%s]; HedgeFlag:[%c]", BufHead, pData->InvestorID, pData->InstrumentID, pData->HedgeFlag);
				else
					strcpy(Buff, BufHead);
				break; 
			}
		case FTTCMD_ReqQryInstrumentMarginRate_Rsp: 
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///请求查询合约手续费率
		case FTTCMD_ReqQryInstrumentCommissionRate: 
			{ 
				CThostFtdcQryInstrumentCommissionRateField* pData = (CThostFtdcQryInstrumentCommissionRateField*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;InvestorID:[%s]; InstrumentID:[%s]", BufHead, pData->InvestorID, pData->InstrumentID);
				else
					strcpy(Buff, BufHead);

				break; 
			}
		case FTTCMD_ReqQryInstrumentCommissionRate_Rsp: 
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///请求查询交易所
		case FTTCMD_ReqQryExchange: 
			{ 
				CThostFtdcQryExchangeField* pData = (CThostFtdcQryExchangeField*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;ExchangeID:[%s]", BufHead, pData->ExchangeID);
				else
					strcpy(Buff, BufHead);
				break; 
			}
		case FTTCMD_ReqQryExchange_Rsp: 
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///请求查询产品
		case FTTCMD_ReqQryProduct: 
			{ 
				PlatformStru_QryProduct* pData = (PlatformStru_QryProduct*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;ProductID:[%s];ProductClass:[%s]", BufHead, pData->Thost.ProductID, pData->Thost.ProductClass);
				else
					strcpy(Buff, BufHead);
				break;
			}
		case FTTCMD_ReqQryProduct_Rsp: 
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///请求查询合约
		case FTTCMD_ReqQryInstrument: 
			{ 
				PlatformStru_QryInstrument* pData = (PlatformStru_QryInstrument*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;InstrumentID:[%s]", BufHead, pData->Thost.InstrumentID);
				else
					strcpy(Buff, BufHead);
				break; 
			}
		case FTTCMD_ReqQryInstrument_Rsp: 
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///请求查询投资者结算结果
		case FTTCMD_ReqQrySettlementInfo: 
			{ 
				PlatformStru_QrySettlementInfo* pData = (PlatformStru_QrySettlementInfo*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;InvestorID:[%s];TradingDay:[%s]", BufHead, pData->Thost.InvestorID, pData->Thost.TradingDay);
				else
					strcpy(Buff, BufHead);
				break;
			}
		case FTTCMD_ReqQrySettlementInfo_Rsp: 
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///请求查询投资者持仓明细
		case FTTCMD_ReqQryInvestorPositionDetail: 
			{ 
				PlatformStru_QryInvestorPositionDetail* pData = (PlatformStru_QryInvestorPositionDetail*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;InvestorID:[%s];InstrumentID:[%s]", BufHead, pData->InvestorID, pData->InstrumentID);
				else
					strcpy(Buff, BufHead);
				break; 
			}
		case FTTCMD_ReqQryInvestorPositionDetail_Rsp: 
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///请求查询客户通知
		case FTTCMD_ReqQryNotice: 
			{ 
				PlatformStru_QryNotice* pData = (PlatformStru_QryNotice*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;BrokerID:[%s]", BufHead, pData->Thost.BrokerID);
				else
					strcpy(Buff, BufHead);
				break; 
			}
		case FTTCMD_ReqQryNotice_Rsp: 
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///请求查询结算信息确认
		case FTTCMD_ReqQrySettlementInfoConfirm: 
			{
				PlatformStru_QrySettlementInfoConfirm* pData = (PlatformStru_QrySettlementInfoConfirm*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;InvestorID:[%s]", BufHead, pData->Thost.InvestorID);
				else
					strcpy(Buff, BufHead);
				break; 
			}
		case FTTCMD_ReqQrySettlementInfoConfirm_Rsp: 
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///请求查询投资者持仓明细
		case FTTCMD_ReqQryInvestorPositionCombineDetail: 
			{ 
				PlatformStru_QryInvestorPositionCombineDetail* pData = (PlatformStru_QryInvestorPositionCombineDetail*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;InvestorID:[%s];CombInstrumentID:[%s]", BufHead, pData->InvestorID, pData->CombInstrumentID);
				else
					strcpy(Buff, BufHead);
				break;
			}
		case FTTCMD_ReqQryInvestorPositionCombineDetail_Rsp: 
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///请求查询保证金监管系统经纪公司资金账户密钥
		case FTTCMD_ReqQryCFMMCTradingAccountKey: 
			{ 
				CThostFtdcQryCFMMCTradingAccountKeyField* pData = (CThostFtdcQryCFMMCTradingAccountKeyField*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;InvestorID:[%s];", BufHead, pData->InvestorID);
				else
					strcpy(Buff, BufHead);
				break; 
			}
		case FTTCMD_ReqQryCFMMCTradingAccountKey_Rsp:
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///请求查询期权交易成本
		case FTTCMD_ReqQryOptionInstrTradeCost:
			{ 
				CThostFtdcQryOptionInstrTradeCostField* pData = (CThostFtdcQryOptionInstrTradeCostField*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;InvestorID:[%s];InstrumentID:[%s]", BufHead, pData->InvestorID, pData->InstrumentID);
				else
					strcpy(Buff, BufHead);
				break; 
			}
		case FTTCMD_ReqQryOptionInstrTradeCost_Rsp:
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///请求查询期权合约手续费
		case FTTCMD_ReqQryOptionInstrCommRate:
			{ 
				CThostFtdcQryOptionInstrCommRateField* pData = (CThostFtdcQryOptionInstrCommRateField*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;InvestorID:[%s];InstrumentID:[%s]", BufHead, pData->InvestorID, pData->InstrumentID);
				else
					strcpy(Buff, BufHead);
				break;
			}
		case FTTCMD_ReqQryOptionInstrCommRate_Rsp:
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///请求查询执行宣告
		case FTTCMD_ReqQryExecOrder:
			{
				CThostFtdcQryOptionInstrTradeCostField* pData = (CThostFtdcQryOptionInstrTradeCostField*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;InvestorID:[%s];InstrumentID:[%s]", BufHead, pData->InvestorID, pData->InstrumentID);
				else
					strcpy(Buff, BufHead);
				break; 
			}
		case FTTCMD_ReqQryExecOrder_Rsp:
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///请求查询转帐流水
		case FTTCMD_ReqQryTransferSerial:
			{ 
				PlatformStru_QryTransferSerial* pData = (PlatformStru_QryTransferSerial*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;InvestorID:[%s]", BufHead, pData->Thost.AccountID);
				else
					strcpy(Buff, BufHead);
				break; 
			}
		case FTTCMD_ReqQryTransferSerial_Rsp:
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///请求查询银期签约关系
		case FTTCMD_ReqQryAccountregister:
			{ 
				PlatformStru_QryAccountRegister* pData = (PlatformStru_QryAccountRegister*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;InvestorID:[%s]", BufHead, pData->Thost.AccountID);
				else
					strcpy(Buff, BufHead);
				break;
			}
		case FTTCMD_ReqQryAccountregister_Rsp:
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///请求查询签约银行
		case FTTCMD_ReqQryContractBank:
			{
				PlatformStru_QryContractBank* pData = (PlatformStru_QryContractBank*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;BankID:[%s]", BufHead, pData->Thost.BankID);
				else
					strcpy(Buff, BufHead);
				break;
			}
		case FTTCMD_ReqQryContractBank_Rsp:
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///请求查询预埋单
		case FTTCMD_ReqQryParkedOrder:
			{ 
				PlatformStru_QryParkedOrder* pData = (PlatformStru_QryParkedOrder*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;InvestorID:[%s];InstrumentID:[%s]", BufHead, pData->Thost.InvestorID, pData->Thost.InstrumentID);
				else
					strcpy(Buff, BufHead);
				break; 
			}
		case FTTCMD_ReqQryParkedOrder_Rsp:
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///请求查询预埋撤单
		case FTTCMD_ReqQryParkedOrderAction:
			{ 
				PlatformStru_QryParkedOrderAction* pData = (PlatformStru_QryParkedOrderAction*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;InvestorID:[%s];InstrumentID:[%s]", BufHead, pData->Thost.InvestorID, pData->Thost.InstrumentID);
				else
					strcpy(Buff, BufHead);
				break;
			}
		case FTTCMD_ReqQryParkedOrderAction_Rsp:
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///请求查询交易通知
		case FTTCMD_ReqQryTradingNotice:
			{
				CThostFtdcQryTradingNoticeField* pData = (CThostFtdcQryTradingNoticeField*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;InvestorID:[%s]", BufHead, pData->InvestorID);
				else
					strcpy(Buff, BufHead);
				break; 
			}
		case FTTCMD_ReqQryTradingNotice_Rsp:
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///请求查询经纪公司交易参数
		case FTTCMD_ReqQryBrokerTradingParams:
			{ 
				CThostFtdcQryBrokerTradingParamsField* pData = (CThostFtdcQryBrokerTradingParamsField*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;InvestorID:[%s]", BufHead, pData->InvestorID);
				else
					strcpy(Buff, BufHead);
				break; 
			}
		case FTTCMD_ReqQryBrokerTradingParams_Rsp:
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///期货发起银行资金转期货请求
		case FTTCMD_ReqFromBankToFutureByFuture:
			{
				PlatformStru_ReqTransfer* pData = (PlatformStru_ReqTransfer*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;BankID:[%s]", BufHead, pData->Thost.BankID);
				else
					strcpy(Buff, BufHead);
				break;
			}
		case FTTCMD_ReqFromBankToFutureByFuture_Rsp:
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///期货发起期货资金转银行请求
		case FTTCMD_ReqFromFutureToBankByFuture:
			{ 
				PlatformStru_ReqTransfer* pData = (PlatformStru_ReqTransfer*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;BankID:[%s]", BufHead, pData->Thost.BankID);
				else
					strcpy(Buff, BufHead);
				break; 
			}
		case FTTCMD_ReqFromFutureToBankByFuture_Rsp:
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

		///期货发起查询银行余额请求
		case FTTCMD_ReqQueryBankAccountMoneyByFuture:
			{ 
				PlatformStru_ReqQueryAccount* pData = (PlatformStru_ReqQueryAccount*)cData.pData;
				if(pData)
					sprintf(Buff,"%s;BankID:[%s]", BufHead, pData->Thost.BankID);
				else
					strcpy(Buff, BufHead);
				break;
			}
		case FTTCMD_ReqQueryBankAccountMoneyByFuture_Rsp:
			{ 
				AppendErrorString(BufHead, (CThostFtdcRspInfoField*)cData.pData, Buff);
				break; 
			}

	//////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

	//仿CTP的Spi函数命令字

		case FFT_SPI_START:
			{ break; } 

		///登录请求响应
		case FTTCMD_OnRspUserLogin_RON:
			{
				if(cData.head.len)
				{
					DataRonUserLogin* pData = (DataRonUserLogin*)cData.pData;
					if(pData)
						sprintf(Buff,"%s;UserID:[%s]", BufHead, pData->UserLoginField.UserID );
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///登出请求响应
		case FTTCMD_OnRspUserLogout_RON:
			{
				if(cData.head.len)
				{
					DataRonUserLogout* pData = (DataRonUserLogout*)cData.pData;
					if(pData)
						sprintf(Buff,"%s;UserID:[%s]", BufHead, pData->UserLogoutField.UserID );
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///用户口令更新请求响应
		case FTTCMD_OnRspUserPasswordUpdate_RON:
			{
				if(cData.head.len)
				{
					DataRonUserPasswordUpdate* pData = (DataRonUserPasswordUpdate*)cData.pData;
					if(pData)
						sprintf(Buff,"%s;UserID:[%s]", BufHead, pData->UserPasswordUpdateField.UserID );
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///资金账户口令更新请求响应
		case FTTCMD_OnRspTradingAccountPasswordUpdate_RON:
			{
				if(cData.head.len)
				{
					DataRonTradingAccountPasswordUpdate* pData = (DataRonTradingAccountPasswordUpdate*)cData.pData;
					if(pData)
						sprintf(Buff,"%s;AccountID:[%s]", BufHead, pData->TradingAccountPasswordUpdateField.AccountID );
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///报单录入请求响应
		case FTTCMD_OnRspOrderInsert_RON: 
			{
				if(cData.head.len)
				{
					DataRonOrderInsert* pData = (DataRonOrderInsert*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;InvestorID:[%s]; InstrumentID:[%s]; OrderRef:[%s]", BufHead, pData->InputOrderField.InvestorID, pData->InputOrderField.InstrumentID, pData->InputOrderField.OrderRef);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///预埋单录入请求响应
		case FTTCMD_OnRspParkedOrderInsert_RON: 
			{
				if(cData.head.len)
				{
					DataRonParkedOrderInsert* pData = (DataRonParkedOrderInsert*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;InvestorID:[%s]; InstrumentID:[%s]; OrderRef:[%s]", 
						BufHead, 
						pData->ParkedOrder.InvestorID, 
						pData->ParkedOrder.InstrumentID, 
						pData->ParkedOrder.OrderRef);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///预埋撤单录入请求响应
		case FTTCMD_OnRspParkedOrderAction_RON: 
			{
				if(cData.head.len)
				{
					DataRonParkedOrderAction* pData = (DataRonParkedOrderAction*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;InvestorID:[%s]; InstrumentID:[%s]; OrderRef:[%s]; FrontID:[%d]; SessionID:[%d]", 
						BufHead, 
						pData->ParkedOrderAction.InvestorID, 
						pData->ParkedOrderAction.InstrumentID, 
						pData->ParkedOrderAction.OrderRef,
						pData->ParkedOrderAction.FrontID,
						pData->ParkedOrderAction.SessionID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///报单操作请求响应
		case FTTCMD_OnRspOrderAction_RON: 
			{
				if(cData.head.len)
				{
					DataRonOrderAction* pData = (DataRonOrderAction*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;InvestorID:[%s]; InstrumentID:[%s]; OrderRef:[%s]; FrontID:[%d]; SessionID:[%d]", 
						BufHead, 
						pData->InputOrderActionField.Thost.InvestorID, 
						pData->InputOrderActionField.Thost.InstrumentID, 
						pData->InputOrderActionField.Thost.OrderRef,
						pData->InputOrderActionField.Thost.FrontID,
						pData->InputOrderActionField.Thost.SessionID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///查询最大报单数量响应
		case FTTCMD_OnRspQueryMaxOrderVolume_RON: 
			{
				if(cData.head.len)
				{
					DataRonQueryMaxOrderVolume* pData = (DataRonQueryMaxOrderVolume*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;InvestorID:[%s]; InstrumentID:[%s]", 
						BufHead, 
						pData->MaxOrderVolumeField.InvestorID, 
						pData->MaxOrderVolumeField.InstrumentID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///投资者结算结果确认响应
		case FTTCMD_OnRspSettlementInfoConfirm_RON: 
			{
				if(cData.head.len)
				{
					DataRonSettlementInfoConfirm* pData = (DataRonSettlementInfoConfirm*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;InvestorID:[%s]", 
						BufHead, 
						pData->SettlementInfoConfirmField.InvestorID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///删除预埋单响应
		case FTTCMD_OnRspRemoveParkedOrder_RON: 
			{
				if(cData.head.len)
				{
					DataRonRemoveParkedOrder* pData = (DataRonRemoveParkedOrder*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;InvestorID:[%s]", 
						BufHead, 
						pData->RemoveParkedOrder.InvestorID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///删除预埋撤单响应
		case FTTCMD_OnRspRemoveParkedOrderAction_RON: 
			{
				if(cData.head.len)
				{
					DataRonRemoveParkedOrderAction* pData = (DataRonRemoveParkedOrderAction*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;InvestorID:[%s]", 
						BufHead, 
						pData->RemoveParkedOrderAction.InvestorID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///执行宣告录入请求响应
		case FTTCMD_OnRspExecOrderInsert_RON: 
			{
				if(cData.head.len)
				{
					DataRonExecOrderInsert* pData = (DataRonExecOrderInsert*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;InvestorID:[%s]", 
						BufHead, 
						pData->InputExecOrderField.InvestorID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///执行宣告操作请求响应
		case FTTCMD_OnRspExecOrderAction_RON: 
			{
				if(cData.head.len)
				{
					DataRonExecOrderAction* pData = (DataRonExecOrderAction*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;InvestorID:[%s]", 
						BufHead, 
						pData->InputExecOrderActionField.InvestorID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///请求查询报单响应
		case FTTCMD_OnRspQryOrder_RON: 
			{
				if(cData.head.len)
				{
					DataRonQryOrder* pData = (DataRonQryOrder*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;InvestorID:[%s]; InstrumentID:[%s]; OrderRef:[%s]; FrontID:[%d]; SessionID:[%d]", 
						BufHead, 
						pData->OrderField.InvestorID, 
						pData->OrderField.InstrumentID, 
						pData->OrderField.OrderRef,
						pData->OrderField.FrontID,
						pData->OrderField.SessionID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}


		///请求查询成交响应
		case FTTCMD_OnRspQryTrade_RON: 
			{
				if(cData.head.len)
				{
					DataRonQryTrade* pData = (DataRonQryTrade*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;InvestorID:[%s]; InstrumentID:[%s]; OrderRef:[%s]; OrderSysID:[%s]", 
						BufHead, 
						pData->TradeField.InvestorID, 
						pData->TradeField.InstrumentID, 
						pData->TradeField.OrderRef,
						pData->TradeField.OrderSysID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///请求查询投资者持仓响应
		case FTTCMD_OnRspQryInvestorPosition_RON: 
			{
				if(cData.head.len)
				{
					DataRonQryInvestorPosition* pData = (DataRonQryInvestorPosition*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;InvestorID:[%s]; InstrumentID:[%s]", 
						BufHead, 
						pData->InvestorPositionField.InvestorID, 
						pData->InvestorPositionField.InstrumentID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///请求查询资金账户响应
		case FTTCMD_OnRspQryTradingAccount_RON: 
			{
				if(cData.head.len)
				{
					DataRonQryTradingAccount* pData = (DataRonQryTradingAccount*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;InvestorID:[%s]", 
						BufHead, 
						pData->TradingAccountField.AccountID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///请求查询投资者响应
		case FTTCMD_OnRspQryInvestor_RON: 
			{
				if(cData.head.len)
				{
					DataRonQryInvestor* pData = (DataRonQryInvestor*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;InvestorID:[%s]", 
						BufHead, 
						pData->InvestorField.InvestorID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///请求查询合约保证金率响应
		case FTTCMD_OnRspQryInstrumentMarginRate_RON: 
			{
				if(cData.head.len)
				{
					DataRonQryInstrumentMarginRate* pData = (DataRonQryInstrumentMarginRate*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;InvestorID:[%s]; InstrumentID:[%s]", 
						BufHead, 
						pData->InstrumentMarginRateField.InvestorID, 
						pData->InstrumentMarginRateField.InstrumentID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///请求查询合约手续费率响应
		case FTTCMD_OnRspQryInstrumentCommissionRate_RON: 
			{
				if(cData.head.len)
				{
					DataRonQryInstrumentCommissionRate* pData = (DataRonQryInstrumentCommissionRate*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;InvestorID:[%s]; InstrumentID:[%s]", 
						BufHead, 
						pData->InstrumentCommissionRateField.InvestorID, 
						pData->InstrumentCommissionRateField.InstrumentID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///请求查询交易所响应
		case FTTCMD_OnRspQryExchange_RON: 
			{
				if(cData.head.len)
				{
					DataRonQryExchange* pData = (DataRonQryExchange*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;ExchangeName:[%s]", 
						BufHead, 
						pData->ExchangeField.ExchangeName);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///请求查询产品响应
		case FTTCMD_OnRspQryProduct_RON:
			{
				if(cData.head.len)
				{
					DataRonQryProduct* pData = (DataRonQryProduct*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;ProductName:[%s]", 
						BufHead, 
						pData->ProductField.ProductName);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}
		///请求查询合约响应
		case FTTCMD_OnRspQryInstrument_RON: 
			{
				if(cData.head.len)
				{
					DataRonQryInstrument* pData = (DataRonQryInstrument*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;InstrumentName:[%s]", 
						BufHead, 
						pData->InstrumentField.InstrumentName);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///请求查询投资者结算结果响应
		case FTTCMD_OnRspQrySettlementInfo_RON:
			{
				if(cData.head.len)
				{
					DataRonQrySettlementInfo* pData = (DataRonQrySettlementInfo*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;InvestorID:[%s]", 
						BufHead, 
						pData->SettlementInfoField.InvestorID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///请求查询投资者持仓明细响应
		case FTTCMD_OnRspQryInvestorPositionDetail_RON:
			{
				if(cData.head.len)
				{
					DataRonQryInvestorPositionDetail* pData = (DataRonQryInvestorPositionDetail*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;InvestorID:[%s];InstrumentID:[%s]", 
						BufHead, 
						pData->InvestorPositionDetailField.InvestorID,
						pData->InvestorPositionDetailField.InstrumentID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///请求查询客户通知响应
		case FTTCMD_OnRspQryNotice_RON: 
			{
				if(cData.head.len)
				{
					DataRonQryNotice* pData = (DataRonQryNotice*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;SequenceLabel:[%s]", 
						BufHead, 
						pData->NoticeField.SequenceLabel);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///请求查询结算信息确认响应
		case FTTCMD_OnRspQrySettlementInfoConfirm_RON:
			{
				if(cData.head.len)
				{
					DataRonQrySettlementInfoConfirm* pData = (DataRonQrySettlementInfoConfirm*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;InvestorID:[%s]", 
						BufHead, 
						pData->SettlementInfoConfirm.InvestorID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///请求查询投资者持仓明细响应
		case FTTCMD_OnRspQryInvestorPositionCombineDetail_RON:
			{
				if(cData.head.len)
				{
					DataRonQryInvestorPositionCombDetail* pData = (DataRonQryInvestorPositionCombDetail*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;InvestorID:[%s]", 
						BufHead, 
						pData->InvestorPositionCombineDetailField.InvestorID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///查询保证金监管系统经纪公司资金账户密钥响应
		case FTTCMD_OnRspQryCFMMCTradingAccountKey_RON:
			{
				if(cData.head.len)
				{
					DataRonTradingAccountKey* pData = (DataRonTradingAccountKey*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;AccountID:[%s]", 
						BufHead, 
						pData->TradingAccountKeyField.AccountID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///请求查询期权交易成本响应
		case FTTCMD_OnRspQryOptionInstrTradeCost_RON:
			{
				if(cData.head.len)
				{
					DataRonQryOptionInstrTradeCost* pData = (DataRonQryOptionInstrTradeCost*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;InvestorID:[%s]", 
						BufHead, 
						pData->InstrumentMarginRateField.InvestorID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///请求查询期权合约手续费响应
		case FTTCMD_OnRspQryOptionInstrCommRate_RON: 
			{
				if(cData.head.len)
				{
					DataRonQryOptionInstrCommRate* pData = (DataRonQryOptionInstrCommRate*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;InvestorID:[%s]; InstrumentID:[%s]", 
						BufHead, 
						pData->InstrumentCommissionRateField.InvestorID,
						pData->InstrumentCommissionRateField.InstrumentID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///请求查询执行宣告响应
		case FTTCMD_OnRspQryExecOrder_RON: 
			{
				if(cData.head.len)
				{
					DataRonQryExecOrder* pData = (DataRonQryExecOrder*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;InvestorID:[%s]; InstrumentID:[%s]", 
						BufHead, 
						pData->ExecOrderField.InvestorID,
						pData->ExecOrderField.InstrumentID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///请求查询转帐流水响应
		case FTTCMD_OnRspQryTransferSerial_RON: 
			{
				if(cData.head.len)
				{
					DataRonQryTransferSerial* pData = (DataRonQryTransferSerial*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;InvestorID:[%s]; BankID:[%s]", 
						BufHead, 
						pData->TransferSerialField.InvestorID,
						pData->TransferSerialField.BankID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///请求查询银期签约关系响应
		case FTTCMD_OnRspQryAccountregister_RON: 
			{
				if(cData.head.len)
				{
					DataRonQryAccountregister* pData = (DataRonQryAccountregister*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;AccountID:[%s]; BankID:[%s]", 
						BufHead, 
						pData->Accountregister.AccountID,
						pData->Accountregister.BankID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///错误应答
		case FTTCMD_OnRspError_RON: 
			{
				if(cData.head.len)
				{
					DataRonError* pData = (DataRonError*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;ErrorID:[%d]; ErrorMsg:[%s]", 
						BufHead, 
						pData->RspInfoField.ErrorID,
						pData->RspInfoField.ErrorMsg);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}


		///报单通知
		case FTTCMD_OnRtnOrder_RON: 
			{
				if(cData.head.len)
				{
					PlatformStru_OrderInfo* pData = (PlatformStru_OrderInfo*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;InvestorID:[%s]; InstrumentID:[%s]; OrderRef:[%s]; FrontID:[%d]; SessionID:[%d]", 
						BufHead, 
						pData->InvestorID, 
						pData->InstrumentID, 
						pData->OrderRef,
						pData->FrontID,
						pData->SessionID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}
		///成交通知
		case FTTCMD_OnRtnTrade_RON:
			{
				if(cData.head.len)
				{
					PlatformStru_TradeInfo* pData = (PlatformStru_TradeInfo*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;InvestorID:[%s]; InstrumentID:[%s]; OrderRef:[%s]; OrderSysID:[%s]", 
						BufHead, 
						pData->InvestorID, 
						pData->InstrumentID, 
						pData->OrderRef,
						pData->OrderSysID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///报单录入错误回报
		case FTTCMD_OnErrRtnOrderInsert_RON:
			{
				if(cData.head.len)
				{
					DataErrRonOrderInsert* pData = (DataErrRonOrderInsert*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;InvestorID:[%s]; InstrumentID:[%s]", 
						BufHead, 
						pData->InputOrderField.InvestorID, 
						pData->InputOrderField.InstrumentID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///报单操作错误回报
		case FTTCMD_OnErrRtnOrderAction_RON: 
			{
				if(cData.head.len)
				{
					DataErrRonOrderAction* pData = (DataErrRonOrderAction*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;InvestorID:[%s]; InstrumentID:[%s]", 
						BufHead, 
						pData->OrderActionField.InvestorID, 
						pData->OrderActionField.InstrumentID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}


		///合约交易状态通知
		case FTTCMD_OnRtnInstrumentStatus_RON:
			{
				if(cData.head.len)
				{
					CThostFtdcInstrumentStatusField* pData = (CThostFtdcInstrumentStatusField*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s; InstrumentID:[%s]", 
						BufHead, 						
						pData->InstrumentID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///交易通知
		case FTTCMD_OnRtnTradingNotice_RON:
			{
				if(cData.head.len)
				{
					CThostFtdcTradingNoticeInfoField* pData = (CThostFtdcTradingNoticeInfoField*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;InvestorID:[%s]; SequenceNo:[%s]", 
						BufHead, 
						pData->InvestorID, 
						pData->SequenceNo);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///执行宣告通知
		case FTTCMD_OnRtnExecOrder_RON:
			{
				if(cData.head.len)
				{
					PlatformStru_ExecOrderField* pData = (PlatformStru_ExecOrderField*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;InvestorID:[%s]; InstrumentID:[%s]", 
						BufHead, 
						pData->InvestorID, 
						pData->InstrumentID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///执行宣告录入错误回报
		case FTTCMD_OnErrRtnExecOrderInsert_RON:
			{
				if(cData.head.len)
				{
					DataErrRonExecOrderInsert* pData = (DataErrRonExecOrderInsert*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;InvestorID:[%s]; InstrumentID:[%s]", 
						BufHead, 
						pData->InputExecOrderField.InvestorID, 
						pData->InputExecOrderField.InstrumentID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}
		///执行宣告操作错误回报
		case FTTCMD_OnErrRtnExecOrderAction_RON: 
			{
				if(cData.head.len)
				{
					DataErrRonExecOrderAction* pData = (DataErrRonExecOrderAction*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;InvestorID:[%s]; InstrumentID:[%s]", 
						BufHead, 
						pData->ExecOrderActionField.InvestorID, 
						pData->ExecOrderActionField.InstrumentID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///询价录入请求响应
		case FTTCMD_OnRspForQuoteInsert_RON: 
			{
				if(cData.head.len)
				{
					DataRonForQuoteInsert* pData = (DataRonForQuoteInsert*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;InvestorID:[%s]; InstrumentID:[%s]", 
						BufHead, 
						pData->InputForQuoteField.InvestorID, 
						pData->InputForQuoteField.InstrumentID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///询价录入错误回报
		case FTTCMD_OnErrRtnForQuoteInsert_RON:
			{
				if(cData.head.len)
				{
					DataErrRonForQuoteInsert* pData = (DataErrRonForQuoteInsert*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;InvestorID:[%s]; InstrumentID:[%s]", 
						BufHead, 
						pData->InputForQuoteField.InvestorID, 
						pData->InputForQuoteField.InstrumentID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///请求查询签约银行响应
		case FTTCMD_OnRspQryContractBank_RON:
			{
				if(cData.head.len)
				{
					DataRonQryContractBank* pData = (DataRonQryContractBank*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;BankName:[%s]", 
						BufHead, 
						pData->ContractBankField.BankName);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///请求查询预埋单响应
		case FTTCMD_OnRspQryParkedOrder_RON: 
			{
				if(cData.head.len)
				{
					DataRonQryParkedOrder* pData = (DataRonQryParkedOrder*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;UserID:[%s]", 
						BufHead, 
						pData->ParkedOrder.UserID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}
		///请求查询预埋撤单响应
		case FTTCMD_OnRspQryParkedOrderAction_RON: 
			{
				if(cData.head.len)
				{
					DataRonQryParkedOrderAction* pData = (DataRonQryParkedOrderAction*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;InvestorID:[%s];InstrumentID:[%s]", 
						BufHead, 
						pData->ParkedOrderAction.InvestorID,
						pData->ParkedOrderAction.InstrumentID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///请求查询交易通知响应
		case FTTCMD_OnRspQryTradingNotice_RON: 
			{
				if(cData.head.len)
				{
					DataRonQryTradingNotice* pData = (DataRonQryTradingNotice*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;InvestorID:[%s];SequenceNo:[%s]", 
						BufHead, 
						pData->TradingNoticeField.InvestorID,
						pData->TradingNoticeField.SequenceNo);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///请求查询经纪公司交易参数响应
		case FTTCMD_OnRspQryBrokerTradingParams_RON: 
			{
				if(cData.head.len)
				{
					DataRonQryBrokerTradingParams* pData = (DataRonQryBrokerTradingParams*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;InvestorID:[%s]", 
						BufHead, 
						pData->BrokerTradingParams.InvestorID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///期货发起银行资金转期货通知
		case FTTCMD_OnRtnFromBankToFutureByFuture_RON: 
			{
				if(cData.head.len)
				{
					CThostFtdcRspTransferField* pData = (CThostFtdcRspTransferField*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;AccountID:[%s];BankID:[%s]", 
						BufHead, 
						pData->AccountID, pData->BankID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}


		///期货发起期货资金转银行通知
		case FTTCMD_OnRtnFromFutureToBankByFuture_RON: 
			{
				if(cData.head.len)
				{
					CThostFtdcRspTransferField* pData = (CThostFtdcRspTransferField*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;AccountID:[%s];BankID:[%s]", 
						BufHead, 
						pData->AccountID, pData->BankID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///期货发起查询银行余额通知
		case FTTCMD_OnRtnQueryBankBalanceByFuture_RON: 
			{
				if(cData.head.len)
				{
					CThostFtdcNotifyQueryAccountField* pData = (CThostFtdcNotifyQueryAccountField*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;AccountID:[%s];BankID:[%s]", 
						BufHead, 
						pData->AccountID, pData->BankID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}
		///期货发起银行资金转期货应答
		case FTTCMD_OnRspFromBankToFutureByFuture_RON:
			{
				if(cData.head.len)
				{
					DataRonFromBankToFutureByFuture* pData = (DataRonFromBankToFutureByFuture*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;AccountID:[%s];BankID:[%s]", 
						BufHead, 
						pData->TransferField.AccountID, pData->TransferField.BankID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///期货发起期货资金转银行应答
		case FTTCMD_OnRspFromFutureToBankByFuture_RON:
			{
				if(cData.head.len)
				{
					DataRonFromFutureToBankByFuture* pData = (DataRonFromFutureToBankByFuture*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;AccountID:[%s];BankID:[%s]", 
						BufHead, 
						pData->TransferField.AccountID,
						pData->TransferField.BankID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}

		///期货发起查询银行余额应答
		case FTTCMD_OnRspQueryBankAccountMoneyByFuture_RON: 
			{
				if(cData.head.len)
				{
					DataRonQueryBankAccountMoneyByFuture* pData = (DataRonQueryBankAccountMoneyByFuture*)cData.pData;
					if(pData)						
						sprintf(Buff,"%s;AccountID:[%s];BankID:[%s]", 
						BufHead, 
						pData->QueryAccountField.AccountID,
						pData->QueryAccountField.BankID);
					else
						strcpy(Buff, BufHead);
				}
				else
					strcpy(Buff, BufHead);
				break; 
			}


		default:
			break;
	}
	
	Monitor_info("%s", Buff);
*/
}

void CSendData::ReadTodaySaveData()
{
	SYSTEMTIME st;
	GetSystemTime(&st);

	char localPath[256];
	memset(localPath, 0, 256);
	GetModuleFileName( NULL, localPath, 256);
	std::string filename=localPath;
	size_t splitpos=filename.find_last_of('\\');
	char strDir[256];
	memset(strDir, 0, sizeof(256));
	sprintf(strDir,"%slog\\Monitor%.4d%.2d%.2d.log",filename.substr(0, splitpos+1).c_str(), st.wYear,st.wMonth,st.wDay);


	CTextFileRead myfile(strDir);
	if(!myfile.IsOpen())
	{		
		return;
	}
	std::string strAll;
	myfile.Read(strAll);
	vector<string> vec;
	split(strAll, '\n', vec);
	for(int i =0; i < (int)vec.size(); i++)
	{			
		ServerOrder order;
		std::string strOne = vec[i];
		vector<string> vecOne;
		split(strOne, ';', vecOne);
		if(vecOne.size() != 9)
			continue;
		
		std::string strData ="";
		std::string str = vecOne[0];//CmdID:[%d]		
		int nPos = str.find('[');
		if(nPos != -1)
		{
			std::string strData =	str.substr(nPos+1, str.length()-nPos-2).c_str();
			order.cmdid = atoi(strData.c_str());
		}
		
		str = vecOne[1];//Seq:[%d]
		nPos = str.find('[');
		if(nPos != -1)
		{
			std::string strData =	str.substr(nPos+1, str.length()-nPos-2).c_str();
			order.seq = atoi(strData.c_str());
		}

		str = vecOne[2];//Len:[%d]
		nPos = str.find('[');
		if(nPos != -1)
		{
			std::string strData =	str.substr(nPos+1, str.length()-nPos-2).c_str();
			order.len = atoi(strData.c_str());
		}

		str = vecOne[3];//userData1:[%d]
		nPos = str.find('[');
		if(nPos != -1)
		{
			std::string strData =	str.substr(nPos+1, str.length()-nPos-2).c_str();
			order.userdata1 = atoi(strData.c_str());
		}

		str = vecOne[4];//userData2:[%d]
		nPos = str.find('[');
		if(nPos != -1)
		{
			std::string strData =	str.substr(nPos+1, str.length()-nPos-2).c_str();
			order.userdata2 = atoi(strData.c_str());
		}

		str = vecOne[5];//userData3:[%d]
		nPos = str.find('[');
		if(nPos != -1)
		{
			std::string strData =	str.substr(nPos+1, str.length()-nPos-2).c_str();
			order.userdata3 = atoi(strData.c_str());
		}	
		
		str = vecOne[6];//userData4:[%d]
		nPos = str.find('[');
		if(nPos != -1)
		{
			std::string strData =	str.substr(nPos+1, str.length()-nPos-2).c_str();
			order.userdata4 = atoi(strData.c_str());
		}			

		str = vecOne[7];//utc:[%d]
		nPos = str.find('[');
		if(nPos != -1)
		{
			std::string strData =	str.substr(nPos+1, str.length()-nPos-2).c_str();
			order.nUTCTime = atoi(strData.c_str());
		}		
		
		str = vecOne[8];//milliSecond:[%d]
		nPos = str.find('[');
		if(nPos != -1)
		{
			std::string strData =	str.substr(nPos+1, str.length()-nPos-2).c_str();
			order.nMiliSecond = atoi(strData.c_str());
		}		

		CDataManager::Get()->AppendServerOrder(order);
		CDataManager::Get()->AddStatisticsData(order);
		CDataManager::Get()->CalcDelay(order);	

		CDataManager::Get()->AddStatisticsData_Min(order);

	}	

}
void CSendData::split(const string& s, char c, vector<string>& v) 
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