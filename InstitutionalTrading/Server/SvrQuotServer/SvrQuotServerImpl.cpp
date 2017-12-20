#include "SvrQuotServer.h"
#include "SvrQuotServerImpl.h"
#include "easymutex.h"
#include "CommonPkg.h"
#include "CommonErrorCode.h"
#include "CommonDef.h"
#include "tools_ansi.h"
#include "MyXML.h"
#include "PlatformCTPQuotImpl.h"

CSvrQuotServerImpl* CSvrQuotServerImpl::m_pObj=NULL;
extern HANDLE g_hEvent;// 等待连接交易服务器查询合约并成功返回事件，否则加载K线历史数据会失败
DWORD DeleteQuotThreadProc(void* lpParam)
{   
    LOG_INFO("删除交易日的线程成功进入"); 

    CSvrQuotServerImpl* pObj = (CSvrQuotServerImpl*)lpParam;
    if(pObj==NULL)
        return 0;

    pObj->DeleteTradingDayData_ex();
	/*pObj->DeleteQuotTriggerData_ex();*/
    
    LOG_INFO("删除交易日的线程成功退出");

    return 0;
}

/*
//服务器启动,加载K线历史数据从今日往前算,
1分历史数据1个月 5分钟3个月 15分钟6个月 30分钟12个月 60分钟24个月 
日线、周线、月线取数据库所有数据全部读出，从今日往前的。

注：不是严格意义上的计算
	 没有兼容证券行情 
	 以月(30天)为单位
*/
DWORD CSvrQuotServerImpl::LoadKLineData()
{	
	string szInstID,szTableName;
	char szSql[256];
	int nErrorCode = CF_ERROR_SUCCESS;
	std::vector<std::vector<_variant_t>> vNode;

	std::map<time_t,KLineBase> mapTime2SKLine;
	std::map<std::string,map<time_t,KLineBase>>  mapKLine;

	vector<string> vInstID = CPlatformCTPQuotImpl::GetInstance().GetSubscribedInstrumentIDs();

	EnumPhrase nPhrase;	
	
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);

    CEasyTime tEndTime;
    tEndTime.settime(sysTime.wYear, sysTime.wMonth, sysTime.wDay, 0, 0, 0);

	time_t tStart=0, tEnd = tEndTime.getutc();

	for(int nCycle = 1; nCycle < 9; nCycle++)
	{
		if(nCycle==1)      {nPhrase=PHRASE_1MIN;  tStart = tEnd - 30*24*3600;}//向前推一个月
		else if(nCycle==2) {nPhrase=PHRASE_5MIN;  tStart = tEnd - 90*24*3600;}//向前推三个月
		else if(nCycle==3) {nPhrase=PHRASE_15MIN; tStart = tEnd - 180*24*3600;}//向前推六个月
		else if(nCycle==4) {nPhrase=PHRASE_30MIN; tStart = tEnd - 360*24*3600;}//向前推十二个月
		else if(nCycle==5) {nPhrase=PHRASE_60MIN; tStart = tEnd - 720*24*3600;}//向前推二十四个月
		else if(nCycle==6) nPhrase=PHRASE_DAY;
		else if(nCycle==7) nPhrase=PHRASE_WEEK;
		else if(nCycle==8) nPhrase=PHRASE_MONTH;

		mapKLine.clear();
		for (vector<string>::iterator it = vInstID.begin(); it != vInstID.end(); it++)
		{
			szInstID = *it;
			szTableName = CPlatformCTPQuotImpl::GetInstance().MakeTableName(szInstID,nPhrase);	

			if(nPhrase >= PHRASE_1MIN && nPhrase <= PHRASE_60MIN)//分时线加载
				sprintf(szSql,"select * from %s t where t.KLINETIME >= %I64d and t.KLINETIME<= %I64d order by t.KLINETIME",szTableName.c_str(),tStart,tEnd);
			else //日、周、月线全部加载
				sprintf(szSql,"select * from %s t order by t.KLINETIME",szTableName.c_str());
			
			mapTime2SKLine.clear();
			vNode.clear();
			if ( !CInterface_SvrDBOpr::getObj().QueryData(szSql, vNode, nErrorCode) || vNode.size() == 0)
			{
				LOG_INFO("%s NO DATA",szSql);
			}
			else
			{
				LOG_INFO("%s CONUT(%d)",szSql,(int)(vNode.size()));			
				for ( UINT j = 0; j < vNode.size(); j++ )
				{
					std::vector<_variant_t> vValue = vNode[j];

					KLineBase data;
					data.dwTime = vValue[0].intVal;						//时间，从1900年1月1日开始的秒数
					data.fOpenPrice = vValue[1].dblVal;					// 开盘价
					data.fHighPrice = vValue[2].dblVal;					// 最高价
					data.fLowPrice = vValue[3].dblVal;					// 最低价
					data.fClosePrice = vValue[4].dblVal;				// 收盘价
					data.fTurnover = vValue[5].dblVal;					//成交金额
					data.dwVolume = vValue[6].intVal;					//成交量
					data.dwOpenInterest = vValue[7].intVal;				//持仓量


					mapTime2SKLine.insert(make_pair(data.dwTime,data));
				}
				mapKLine.insert(make_pair(szInstID,mapTime2SKLine));
			}
		}

		switch(nPhrase)
		{
			case PHRASE_1MIN:CPlatformCTPQuotImpl::GetInstance().SetKLineMin1(mapKLine);break;
			case PHRASE_5MIN:CPlatformCTPQuotImpl::GetInstance().SetKLineMin5(mapKLine);break;
			case PHRASE_15MIN:CPlatformCTPQuotImpl::GetInstance().SetKLineMin15(mapKLine);break;
			case PHRASE_30MIN:CPlatformCTPQuotImpl::GetInstance().SetKLineMin30(mapKLine);break;
			case PHRASE_60MIN:CPlatformCTPQuotImpl::GetInstance().SetKLineMin60(mapKLine);break;
			case PHRASE_DAY:CPlatformCTPQuotImpl::GetInstance().SetKLineDay(mapKLine);break;
			case PHRASE_WEEK:CPlatformCTPQuotImpl::GetInstance().SetKLineWeek(mapKLine);break;
			case PHRASE_MONTH:CPlatformCTPQuotImpl::GetInstance().SetKLineMonth(mapKLine);break;
			default:break;
		}	
	}

	return 0;
}
DWORD CSvrQuotServerImpl::LoadKLineDataThreadProc(LPVOID lpParam)
{	
	CSvrQuotServerImpl *pMain = (CSvrQuotServerImpl *)lpParam;
	LOG_INFO("加载历史K线数据线程成功进入"); 

	LoadKLineData();
	pMain->m_bKLineDataThreadLoading=false;
	LOG_INFO("加载历史K线数据线程成功退出");

	return 0;
}

void CSvrQuotServerImpl::CreateDeleteThread()
{
    //创建删除交易日的线程  
    m_hThread=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)DeleteQuotThreadProc,this,0,&m_idThread);   
}

void CSvrQuotServerImpl::CreateLoadKLineDataThread()
{
	WaitForSingleObject(g_hEvent, INFINITE);
	m_hLoadKLineDataThread=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)LoadKLineDataThreadProc,this,0,&m_idLoadKLineDataThread);
}

CSvrQuotServerImpl::CSvrQuotServerImpl(void)
{
    m_hThread=NULL;
    m_idThread=0;

	m_hLoadKLineDataThread=NULL;
	m_idLoadKLineDataThread=0;

    LoadServerReinitTime();
    LoadQuotDataSaveTime();

#ifdef _DeteleQuot_
	CreateDeleteThread();
#endif

#ifdef _LoadKLineData_
	CreateLoadKLineDataThread();
#endif
}

CSvrQuotServerImpl::~CSvrQuotServerImpl(void)
{
    //等待线程退出
    DWORD ExitCode;
    WaitForSingleObject((HANDLE)m_hThread,8000);
    if(GetExitCodeThread((HANDLE)m_hThread,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
        TerminateThread((HANDLE)m_hThread,0);
    CloseHandle(m_hThread);

    m_hThread=NULL;
    m_idThread=0;

    WaitForSingleObject((HANDLE)m_hLoadKLineDataThread,8000);
    if(GetExitCodeThread((HANDLE)m_hLoadKLineDataThread,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
        TerminateThread((HANDLE)m_hLoadKLineDataThread,0);
    CloseHandle(m_hLoadKLineDataThread);

    m_hLoadKLineDataThread=NULL;
    m_idLoadKLineDataThread=0;
}

CSvrQuotServerImpl& CSvrQuotServerImpl::getObj(void)
{
    g_mutex.lock();
    if (NULL == m_pObj)    
        m_pObj = new CSvrQuotServerImpl();
    g_mutex.unlock();
    return *m_pObj;
}

void CSvrQuotServerImpl::deleteObj()
{
    g_mutex.lock();
    if(NULL != m_pObj)
    {
        delete m_pObj;
        m_pObj = NULL;
    }	
    g_mutex.unlock();
}

//接口
//查询“某年某月某日某时某分”历史行情接口 
bool CSvrQuotServerImpl::QueryQuotData(const std::string strDateTime, 
                                   std::vector<PlatformStru_DepthMarketData_EX>& vMarketData ,
                                   int& nErrorCode )
{
    if(strDateTime.length()!=strlen(DATETIMESTYLE))
    {
        nErrorCode = -100;
        return false;
    }


	int szTime[3] = {0}; 
    char szValiDatedate[9],szFirstUpdateTime[9],szNextUpdateTime[9];
    
    memset(szTime,0,sizeof(szTime));    
    memset(szValiDatedate,0,sizeof(szValiDatedate));
    memset(szFirstUpdateTime,0,sizeof(szFirstUpdateTime));
    memset(szNextUpdateTime,0,sizeof(szNextUpdateTime));

    //20140225 10:50:00
	strncpy(szValiDatedate, strDateTime.c_str(), 8);
	strncpy(szFirstUpdateTime, strDateTime.c_str()+9, 8);
    sscanf_s(szFirstUpdateTime,"%2d:%2d:%2d",&szTime[0],&szTime[1],&szTime[2]);


    char szTableName[128];
    memset(szTableName,0,sizeof(szTableName));
    sprintf(szTableName,"%s_%s",QUOTTABLENAME,szValiDatedate);
 
    char szBuffer[MAX_SQL_LENGTH];    
    std::vector<std::vector<_variant_t>> vNode;

	sprintf(szNextUpdateTime,"%.2d:%.2d:59",szTime[0],szTime[1]);

    memset(szBuffer, 0, sizeof(szBuffer));
    sprintf(szBuffer, "select * from \"%s\" t where t.updatetime >= \'%s\' and \'%s\' and t.validatedate = \'%s\' order by t.updatetime",szTableName,szFirstUpdateTime,szNextUpdateTime,szValiDatedate);
    //sprintf(szBuffer, "select * from \"%s\" t where t.updatetime between \'%s\' and \'%s\' and t.tradingday = \'%s\' order by t.updatetime",szTableName,szFirstUpdateTime,szNextUpdateTime,szValiDatedate);
    //sprintf(szBuffer, "select * from \"%s\" t where t.updatetime between \'%s\' and \'%s\' order by t.updatetime",szTableName,szFirstUpdateTime,szNextUpdateTime);

    if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode) || vNode.size() == 0)
        return false;
    else
    {
        LOG_INFO("取一分钟数据 %s CONUT(%d)",szBuffer,(int)(vNode.size()));

        for ( UINT j = 0; j < vNode.size(); j++ )
        {
            std::vector<_variant_t> vValue = vNode[j];

            PlatformStru_DepthMarketData_EX depthMarketData_ex;

            strcpy(depthMarketData_ex.TradingDay, vValue[0].operator _bstr_t());	
            strcpy(depthMarketData_ex.InstrumentID, vValue[1].operator _bstr_t());
            strcpy(depthMarketData_ex.ExchangeID, vValue[2].operator _bstr_t());
            depthMarketData_ex.LastPrice = vValue[4].dblVal;
            depthMarketData_ex.PreSettlementPrice = vValue[5].dblVal;
            depthMarketData_ex.PreClosePrice = vValue[6].dblVal;
            depthMarketData_ex.PreOpenInterest = vValue[7].dblVal;
            depthMarketData_ex.OpenPrice = vValue[8].dblVal;
            depthMarketData_ex.HighestPrice = vValue[9].dblVal;
            depthMarketData_ex.LowestPrice = vValue[10].dblVal;
            depthMarketData_ex.Volume = vValue[11].intVal;
            depthMarketData_ex.Turnover = vValue[12].dblVal;
            depthMarketData_ex.OpenInterest = vValue[13].dblVal;
            depthMarketData_ex.ClosePrice = vValue[14].dblVal;
            depthMarketData_ex.SettlementPrice = vValue[15].dblVal;
            depthMarketData_ex.UpperLimitPrice = vValue[16].dblVal;
            depthMarketData_ex.LowerLimitPrice = vValue[17].dblVal;
            strcpy(depthMarketData_ex.UpdateTime, vValue[20].operator _bstr_t());
            depthMarketData_ex.UpdateMillisec = vValue[21].intVal;
            depthMarketData_ex.BidPrice1 = vValue[22].dblVal;
            depthMarketData_ex.BidVolume1 = vValue[23].intVal;
            depthMarketData_ex.AskPrice1 = vValue[24].dblVal;
            depthMarketData_ex.AskVolume1 = vValue[25].intVal;            
            depthMarketData_ex.AveragePrice = vValue[42].dblVal;

            vMarketData.push_back(depthMarketData_ex);
        } 
    }   
    
    return true;          
}
CF_ERROR CSvrQuotServerImpl::GetReinitTime(sReinitTime& sTime)
{
    g_mutex.lock();
    memcpy(&sTime,&m_sReinitTime,sizeof(sReinitTime));
    g_mutex.unlock();
    return CF_ERROR_SUCCESS;
}

CF_ERROR CSvrQuotServerImpl::SetReinitTime(const sReinitTime& sTime)
{
    g_mutex.lock();
    memcpy(&m_sReinitTime,&sTime,sizeof(sReinitTime));

    std::string lsNightTime = sTime.mNightTime;
    std::string lsDayTime = sTime.mDayTime;
    char lsPath[MAX_PATH] = {0};
    GetModuleFileName(NULL,lsPath,MAX_PATH);
    *(strrchr(lsPath,'\\') + 1)= NULL;
    strcat(lsPath,CONFIG_FILE);

    WritePrivateProfileString("ServerReinitTime","NightTime",lsNightTime.c_str(),lsPath);
    WritePrivateProfileString("ServerReinitTime","DayTime",lsDayTime.c_str(),lsPath);
    g_mutex.unlock();

    return CF_ERROR_SUCCESS;
}

bool CSvrQuotServerImpl::LoadConnectCtpParamFromDB(BrokerInfo& brokerInfo,std::map<std::string,std::vector<std::string>>& mapBrokerCode2AddrParam)
{
    int nErrorCode = CF_ERROR_SUCCESS;

    std::string szFrontAddr;
    char szBuffer[MAX_SQL_LENGTH];

    //Broker
    memset(szBuffer, 0, sizeof(szBuffer));
    sprintf(szBuffer, "select * from BASE_BROKER t where t.type = %d", BROKER_TYPE_QUOT);
    std::vector<std::vector<_variant_t>> vecBrokerInfo;
    if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vecBrokerInfo, nErrorCode))
    {
        return false;
    }
    else
    {        
        if(vecBrokerInfo.size()==0)
            return false;

        std::vector<_variant_t> vValue = vecBrokerInfo[0];

        memset(&brokerInfo, 0, sizeof(BrokerInfo));
        brokerInfo.nBrokerID = vValue[0].intVal;
        strcpy(brokerInfo.szBrokerCode, vValue[1].operator _bstr_t());  
        strcpy(brokerInfo.szCompany, vValue[2].operator _bstr_t());  
        brokerInfo.nType = vValue[3].intVal;
        strcpy(brokerInfo.szAccount, vValue[4].operator _bstr_t());  
        strcpy(brokerInfo.szPassword, vValue[5].operator _bstr_t());

		LOG_INFO("Broker info code=%s company=%s account=%s",brokerInfo.szBrokerCode,brokerInfo.szCompany,brokerInfo.szAccount);
    }

    memset(szBuffer, 0, sizeof(szBuffer));
    sprintf(szBuffer, "select * from BASE_SERVERADDRQUOT t");
    std::vector<std::vector<_variant_t>> vecServerAddress;
    if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vecServerAddress, nErrorCode))
    {
        return false;
    }
    else
    {
        for ( UINT i = 0; i < vecServerAddress.size(); i++ )
        {
            ServerAddressQuot info;
            memset(&info, 0, sizeof(info));

            std::vector<_variant_t> vValue = vecServerAddress[i];

            int nIndex = 0;             
            info.nAddrID = vValue[nIndex++].intVal;
            strcpy(info.szBrokerCode, vValue[nIndex++].operator _bstr_t());
            strcpy(info.szIP, vValue[nIndex++].operator _bstr_t());
            info.nPort = vValue[nIndex++].intVal;

            memset(szBuffer, 0, sizeof(szBuffer));
            sprintf(szBuffer, "%s:%d", info.szIP, info.nPort);

            szFrontAddr = szBuffer; 

            if(0==strcmp(brokerInfo.szBrokerCode,info.szBrokerCode))
            {
                mapBrokerCode2AddrParam[info.szBrokerCode].push_back(szFrontAddr);
				LOG_INFO("Quot Front Address %s",szFrontAddr.c_str());
            }
        }        
    }

    return mapBrokerCode2AddrParam.empty()?false:true;
}
bool CSvrQuotServerImpl::LoadInstrumentsFromDB(const TThostFtdcDateType pTradeDay,std::set<std::string>& setInstrument)
{
    int nErrorCode = CF_ERROR_SUCCESS;

    char szYear[5];
    memset(szYear,0,sizeof(szYear));
    memcpy(szYear,pTradeDay,4);

    pTradeDay = pTradeDay+4;
    char szMonth[3];
    memset(szMonth,0,sizeof(szMonth));
    memcpy(szMonth,pTradeDay,2);

    pTradeDay = pTradeDay+2;
    char szDay[3];
    memset(szDay,0,sizeof(szDay));
    memcpy(szDay,pTradeDay,2);

    char szFormatTime[11];
    memset(szFormatTime,0,sizeof(szFormatTime));
    sprintf(szFormatTime,"%s-%s-%s",szYear,szMonth,szDay);

    char szBuffer[MAX_SQL_LENGTH];
    memset(szBuffer, 0, sizeof(szBuffer));
    sprintf(szBuffer, "select * from \"TRADEDATA_INSTRUEMENTS\" t where t.ValidateDate = \'%s\'",szFormatTime);

    std::vector<std::vector<_variant_t>> vNode;
    if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode) || vNode.size() == 0)
    {
        return false;
    }
    else
    {
        for ( UINT j = 0; j < vNode.size(); j++ )
        {
            PlatformStru_InstrumentInfo lTempInfp;
            memset(&lTempInfp, 0, sizeof(lTempInfp));

            std::vector<_variant_t> vValue = vNode[j];

            int i = 0;
            strcpy(lTempInfp.InstrumentID, vValue[i++].operator _bstr_t());	

            setInstrument.insert(lTempInfp.InstrumentID);
        }
    }

    return true;
}
//bool CSvrQuotServerImpl::DeleteQuotTriggerData_ex()
//{
//	int nErrorCode = CF_ERROR_SUCCESS;
//
//	char szBuffer[MAX_SQL_LENGTH];
//	memset(szBuffer, 0, sizeof(szBuffer));
//	sprintf(szBuffer, "delete from	AQUOT_TRIGGER t "); 
//	return CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode);
//}

bool CSvrQuotServerImpl::DeleteTradingDayData_ex()
{
    int nErrorCode = CF_ERROR_SUCCESS;

    char szBuffer[MAX_SQL_LENGTH];
    memset(szBuffer, 0, sizeof(szBuffer));
    sprintf(szBuffer, "select table_name from tabs where table_name like 'AQUOTINFO%%'"); 

    std::vector<std::vector<_variant_t>> vNode;
    if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode) || vNode.size() == 0)
    {
        return false;
    }
    else
    {
        int nSaveYear,nSaveMon,nSaveDay,nYear,nMon,nDay;
        sscanf_s(m_HistoryDataSaveTime,"%04d%02d%02d",&nSaveYear,&nSaveMon,&nSaveDay);
     

        for ( UINT j = 0; j < vNode.size(); j++ )
        {
            char szTableName[128];
            memset(szTableName, 0, sizeof(szTableName));

            std::vector<_variant_t> vValue = vNode[j];
            strcpy(szTableName, vValue[0].operator _bstr_t());	

            string temp(szTableName);
            temp = temp.substr(temp.find('_')+1,temp.length()-temp.find('_'));
            sscanf_s(temp.c_str(),"%04d%02d%02d",&nYear,&nMon,&nDay);
    
            if((nYear<nSaveYear) || ((nYear==nSaveYear)&&(nMon<nSaveMon)) || ((nYear==nSaveYear)&&(nMon==nSaveMon)&&(nDay<nSaveDay)))
            {
                //清除交易日前的数据
                memset(szBuffer, 0, sizeof(szBuffer));
                sprintf(szBuffer, "DROP TABLE %s",szTableName);
                LOG_INFO("%s",szBuffer);
                CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode);
            }
        }
    }

    return true;
}

void CSvrQuotServerImpl::LoadServerReinitTime()
{
    char lsPath[MAX_PATH] = {0};
    GetModuleFileName(NULL,lsPath,MAX_PATH);
    *(strrchr(lsPath,'\\') + 1)= NULL;
    strcat(lsPath,SIMULATE_FILE);

    memset(&m_sReinitTime,0,sizeof(sReinitTime));
    GetPrivateProfileString("ServerReinitTime","NightTime","20:30:30",m_sReinitTime.mNightTime,sizeof(m_sReinitTime.mNightTime),lsPath);
    GetPrivateProfileString("ServerReinitTime","DayTime","08:30:30",m_sReinitTime.mDayTime,sizeof(m_sReinitTime.mNightTime),lsPath);
}
void  CSvrQuotServerImpl::LoadQuotDataSaveTime()
{
    char lsPath[MAX_PATH] = {0};
    GetModuleFileName(NULL,lsPath,MAX_PATH);
    *(strrchr(lsPath,'\\') + 1)= NULL;
    strcat(lsPath,SIMULATE_FILE);

    memset(&m_HistoryDataSaveTime,0,sizeof(_ExchangDateType));
    GetPrivateProfileString("HistoryDataSaveTime","Time","20151212",m_HistoryDataSaveTime,sizeof(_ExchangDateType),lsPath);
}
bool  CSvrQuotServerImpl::GetKLineData(KLineQuery* pData, vector<KLineBase>& outData, vector<TLineBase>& outTLData)
{
	if(pData->nDataType==1)
	{
		switch(pData->nCycle)
		{
		case PHRASE_1MIN:
			CPlatformCTPQuotImpl::GetInstance().GetKLineMin1(pData,outData,outTLData);
			break;
		case PHRASE_5MIN:
			CPlatformCTPQuotImpl::GetInstance().GetKLineMin5(pData,outData,outTLData);
			break;	
		case PHRASE_15MIN:
			CPlatformCTPQuotImpl::GetInstance().GetKLineMin15(pData,outData,outTLData);
			break;
		case PHRASE_30MIN:
			CPlatformCTPQuotImpl::GetInstance().GetKLineMin30(pData,outData,outTLData);
			break;
		case PHRASE_60MIN:
			CPlatformCTPQuotImpl::GetInstance().GetKLineMin60(pData,outData,outTLData);
			break;
		case PHRASE_DAY:
			CPlatformCTPQuotImpl::GetInstance().GetKLineDay(pData,outData,outTLData);
			break;
		case PHRASE_WEEK:
			CPlatformCTPQuotImpl::GetInstance().GetKLineWeek(pData,outData,outTLData);
			break;
		case PHRASE_MONTH:
			CPlatformCTPQuotImpl::GetInstance().GetKLineMonth(pData,outData,outTLData);
			break;
		}
	}else if(pData->nDataType==2)
	{
		CPlatformCTPQuotImpl::GetInstance().GetKLineMin1(pData,outData,outTLData);
	}


	return outData.size()>0;
}

bool  CSvrQuotServerImpl::QueryKlineData(KLineQuery* pData, int&lError,int nsocket,int nSeq)
{
	vector<KLineBase> outData;	
	vector<TLineBase> outTLData;
	GetKLineData(pData,outData,outTLData);

	if(pData->nDataType==1)
	{
		RespKlineData(nsocket,nSeq,outData);	
		LOG_INFO("------>>>K线请求返回 %s [%I64d---%I64d] CONUT(%d)",pData->InstrumentID, pData->szStartTime, pData->szEndTime, (int)outData.size());
	}
	else if(pData->nDataType==2)
	{
		RespTlineData(nsocket,nSeq,outTLData);	
		LOG_INFO("------>>>分时线请求返回 %s [%I64d---%I64d] CONUT(%d)",pData->InstrumentID, pData->szStartTime, pData->szEndTime,(int)outTLData.size());
	}	

	return true;	
}
bool CSvrQuotServerImpl::QuerySettlementPrice(KLineQuery* pData ,int& lError,vector<SettlementPrice>& outData)
{
	if(pData && pData->nDataType==3)
	{		
		CEasyTime tStartTime(pData->szStartTime);
		CEasyTime tEndTime(pData->szEndTime);

		char szSql[MAX_SQL_LENGTH];
		char szBuf[9];
		memset(szSql, 0, sizeof(szSql));

		sprintf(szSql,"select t.SETTLEMENTDATE,t.SETTLEMENTPRICE from SETTLEMENT_SETTLEMENTPRICE t where t.SETTLEMENTDATE  between '%s' and '%s' and t.INSTRUMENTID = '%s' order by t.SETTLEMENTDATE",tStartTime.getstryyyymmdd().c_str(),tEndTime.getstryyyymmdd().c_str(),pData->InstrumentID);
		std::vector<std::vector<_variant_t>> vNode;
		if ( !CInterface_SvrDBOpr::getObj().QueryData(szSql, vNode, lError) || vNode.size() == 0)
		{
			return lError;
			LOG_INFO("------>>>结算价请求NODATA");
		}
		else
		{
			LOG_INFO("%s CONUT(%d)",szSql,(int)(vNode.size()));

			for ( UINT j = 0; j < vNode.size(); j++ )
			{

				SettlementPrice data;

				std::vector<_variant_t> vValue = vNode[j];
				int i = 0;

				memset(szBuf,0,sizeof(szBuf));
				strcpy(szBuf, vValue[i++].operator _bstr_t());	

				data.TradingDay = atoi(szBuf);
				data.Price= vValue[i++].dblVal;

				outData.push_back(data);
			}

			return lError;
		} 
	}

	return false;
}
 int CSvrQuotServerImpl::GetParamFromString(std::string Str, std::vector<std::string>& StringVec,char Delim)
{
	char* p = strtok((char*)Str.c_str(), &Delim); 
	while (p)
	{
		std::string buffer = p;
		StringVec.push_back(buffer);
		p = strtok(NULL, &Delim); 
	}
	return StringVec.size();
}
 void CSvrQuotServerImpl::RespKlineData(int nsocket,int nSeq,std::vector<KLineBase>& vecKlineData)
 {
	 int maxQueryCount =CMyXML("Account.xml").GetNodeContentInt2(1000,"root","maxQueryCount");
	 if(vecKlineData.size()>0 && vecKlineData.size()<maxQueryCount) 
	 {
		 CInterface_SvrTcp::getObj().SendPkgData(nsocket,
			 Cmd_QueryQuotKLineData_Rsp,
			 &vecKlineData[0] ,
			 vecKlineData.size()*sizeof(KLineBase),
			 nSeq,
			 0,
			 CF_ERROR_SUCCESS);
	 }
	 else if(vecKlineData.size()>maxQueryCount)
	 {
		 CInterface_SvrTcp::getObj().SendPkgData(nsocket,
			 Cmd_QueryQuotKLineData_Rsp,
			 &vecKlineData[0] ,
			 maxQueryCount*sizeof(KLineBase),
			 nSeq,
			 0,
			 CF_ERROR_SUCCESS);
	 }
	 else
	 {
		 CInterface_SvrTcp::getObj().SendPkgData(nsocket,
			 Cmd_QueryQuotKLineData_Rsp,
			 NULL, 0,
			 nSeq,
			 0,
			 CF_ERROR_SUCCESS);
	 }
 }

 void CSvrQuotServerImpl::RespTlineData(int nsocket,int nSeq,std::vector<TLineBase>& vecTlineData)
 {
	 int maxQueryCount =CMyXML("Account.xml").GetNodeContentInt2(1000,"root","maxQueryCount");
	 if(vecTlineData.size()>0 && vecTlineData.size()<maxQueryCount) 
	 {
		 CInterface_SvrTcp::getObj().SendPkgData(nsocket,
			 Cmd_QueryQuotTimeLineData_Rsp,
			 &vecTlineData[0] ,
			 vecTlineData.size()*sizeof(TLineBase),
			 nSeq,
			 0,
			 CF_ERROR_SUCCESS);
	 }
	 else if(vecTlineData.size()>maxQueryCount)
	 {
		 CInterface_SvrTcp::getObj().SendPkgData(nsocket,
			 Cmd_QueryQuotTimeLineData_Rsp,
			 &vecTlineData[0] ,
			 maxQueryCount*sizeof(TLineBase),
			 nSeq,
			 0,
			 CF_ERROR_SUCCESS);
	 }
	 else
	 {
		 CInterface_SvrTcp::getObj().SendPkgData(nsocket,
			 Cmd_QueryQuotTimeLineData_Rsp,
			 NULL, 0,
			 nSeq,
			 0,
			 CF_ERROR_SUCCESS);
	 }
 }

//回调函数
DWORD CSvrQuotServerImpl::QueryKlineDBData(LPVOID lp)
{
	if (!lp)
	{
		LOG_INFO("NUll");
	}
	std::vector<KLineBase>    vKlineData;
	int nErrorCode =CF_ERROR_SUCCESS;
	QueryPara *pData= (QueryPara*)lp;
	int maxQueryCount =CMyXML("Account.xml").GetNodeContentInt2(1000,"root","maxQueryCount");
	std::vector<std::string> vecSql;
	int nSqlCount =GetParamFromString(pData->szBuffer,vecSql);
	for (int i=0;i< nSqlCount;i++)
	{
		std::vector<std::vector<_variant_t>> vNode;
		vNode.clear();
		LOG_INFO("Execute Query KLine SQL\n%s\n", vecSql[i].c_str());
		if ( !CInterface_SvrDBOpr::getObj().QueryData(vecSql[i].c_str(), vNode, nErrorCode) || vNode.size() == 0)
		{
			LOG_INFO("Query KLine NO DATA\n");
		}
		else
		{
			LOG_INFO("取K线数据\n%s\nCONUT(%d)\n",vecSql[i].c_str(),(int)(vNode.size()));

			for ( UINT j = 0; j < vNode.size(); j++ )
			{
				std::vector<_variant_t> vValue = vNode[j];

				KLineBase kLineData;

    //            strcpy(kLineData.InstrumentID, vValue[0].operator _bstr_t());
				//kLineData.eCycle = (EnumPhrase)vValue[1].intVal;
				//sscanf(vValue[2].operator _bstr_t(), "%4d%2d%2d %2d:%2d:%2d",     
				//	&kLineData.dwTime.tm_year, 
				//	&kLineData.dwTime.tm_mon, 
				//	&kLineData.dwTime.tm_mday, 
				//	&kLineData.dwTime.tm_hour, 
				//	&kLineData.dwTime.tm_min,
				//	&kLineData.dwTime.tm_sec);
			
				kLineData.dwTime = vValue[0].intVal;
				kLineData.fOpenPrice = vValue[1].dblVal;
				kLineData.fHighPrice = vValue[2].dblVal;
				kLineData.fLowPrice = vValue[3].dblVal;
				kLineData.fClosePrice = vValue[4].dblVal;
				kLineData.fTurnover = vValue[5].dblVal;
				kLineData.dwVolume = vValue[6].intVal;				
				kLineData.dwOpenInterest = vValue[7].intVal;
				//kLineData.dJieSuan = vValue[10].dblVal;

				vKlineData.push_back(kLineData);

			} 
		} 

		if(vKlineData.size()>maxQueryCount) 
			break;
		
	}
	
	
    RespKlineData(pData->nSocket,pData->nSeq,vKlineData);

	return 0;
	
}
//回调函数
DWORD CSvrQuotServerImpl::QueryTimelineDBData(LPVOID lp)
{
	//if (!lp)
	//{
	//	LOG_INFO("NUll");
	//}
	//std::vector<TLineBase>    vTimeLineData;
	//int nErrorCode =CF_ERROR_SUCCESS;
	//std::vector<std::vector<_variant_t>> vNode;
	//QueryPara *pData= (QueryPara*)lp;
	//int maxQueryCount =CMyXML("Account.xml").GetNodeContentInt2(1000,"root","maxQueryCount");
	//std::vector<std::string> vecSql;
	//int nSqlCount =GetParamFromString(pData->szBuffer,vecSql);
	//for (int i=0;i<nSqlCount;i++)
	//{
	//	vNode.clear();
	//	if ( !CInterface_SvrDBOpr::getObj().QueryData(vecSql[i].c_str(), vNode, nErrorCode) || vNode.size() == 0)
	//	{
	//		LOG_INFO("Query TimeLine NO DATA");
	//	}
	//	else
	//	{
	//		LOG_INFO("取T线数据 %s CONUT(%d)",vecSql[i].c_str(),(int)(vNode.size()));

	//		for ( UINT j = 0; j < vNode.size(); j++ )
	//		{
	//			std::vector<_variant_t> vValue = vNode[j];

	//			TLineBase timeLineData;

	//			strcpy(timeLineData.InstrumentID, vValue[0].operator _bstr_t());
	//			sscanf(vValue[1].operator _bstr_t(), "%4d%2d%2d %2d:%2d:%2d",     
	//				&timeLineData.dwTime.tm_year, 
	//				&timeLineData.dwTime.tm_mon, 
	//				&timeLineData.dwTime.tm_mday, 
	//				&timeLineData.dwTime.tm_hour, 
	//				&timeLineData.dwTime.tm_min,
	//				&timeLineData.dwTime.tm_sec);
	//			
	//			timeLineData.fLastPrice = vValue[2].dblVal;
	//			timeLineData.dwVolume = vValue[3].intVal;
	//			timeLineData.dwVolumeMoney = vValue[4].dblVal;
	//			timeLineData.dwHoldVolume = vValue[5].intVal;


	//			vTimeLineData.push_back(timeLineData);

	//		} 
	//	} 
	//	if(vTimeLineData.size()>1000) 
	//		break;
	//}
	//RespTlineData(pData->nSocket,pData->nSeq,vTimeLineData);

	return 0;

}

CF_ERROR CSvrQuotServerImpl::QueryVolatilityData(Volatility& pData)
{
	LOG_INFO("------>>>波动率请求,Instrument===%s TradingDay=%s",pData.InstrumentID,pData.TradingDay);

	int nErrorCode =CF_ERROR_SUCCESS;
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));

	sprintf(szBuffer, "select * from (select * from VOLATILITY_HISTORY t where t.instrumentid = '%s' order by t.TRADINGDAY DESC) where rownum <= 1",pData.InstrumentID );
	std::vector<std::vector<_variant_t>> vNode;
	if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode) || vNode.size() == 0)
	{
		return nErrorCode;
		LOG_INFO("------>>>波动率请求NODATA");
	}
	else
	{
		for ( UINT j = 0; j < vNode.size(); j++ )
		{

			std::vector<_variant_t> vValue = vNode[j];
			int i = 0;
			strcpy(pData.TradingDay, vValue[i++].operator _bstr_t());	
			strcpy(pData.InstrumentID, vValue[i++].operator _bstr_t());
			pData.VolatilityTwenty = vValue[i++].dblVal;
			pData.VolatilitySixty= vValue[i++].dblVal;


		}
		LOG_INFO("------>>>波动率请求,Instrument===%s TradingDay=%s, VolatilityTwenty=%f",pData.InstrumentID,pData.TradingDay,pData.VolatilityTwenty);
		return nErrorCode;
	} 
}

int CSvrQuotServerImpl::GetRestLoadkLineDataThread()
{
	return m_bKLineDataThreadLoading;
}