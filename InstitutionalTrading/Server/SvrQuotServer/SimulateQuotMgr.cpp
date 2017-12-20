#include "SimulateQuotMgr.h"

CSimulateQuotMgr* CSimulateQuotMgr::m_pObj=NULL;
Ceasymutex CSimulateQuotMgr::m_mutexSimulate;

//手动模拟回放读和发送线程
DWORD SimulateReadProcess(void* lpParam)
{    
    CSimulateQuotMgr *pInst = (CSimulateQuotMgr*)lpParam;
    if(!pInst)
    {
        LOG_INFO("模拟行情服务器回放读线程失败!");
        return 0;
    }

    eSimulateDayType nPlayType;
    vector<string> vDayTime;

    //搜索数据 
    do 
    {
        LOG_INFO("读线程正在搜索数据");
        Sleep(2000);
        if(WaitForSingleObject(pInst->m_hReadStopEvent,0)==WAIT_OBJECT_0)
        {
            LOG_INFO("手动停止模拟行情，退出读线程");
            return 1;
        }
    } while (!pInst->GetSimulateData(nPlayType,vDayTime));

    char szTableName[128]; 
    char szValiDatedate[9];
    char szFirstUpdateTime[9];
    char szFirstUpdateTimePre[9];
    char szNextUpdateTime[9];   

    int nErrorCode = CF_ERROR_SUCCESS;
    char szBuffer[MAX_SQL_LENGTH];    
    std::vector<std::vector<_variant_t>> vNode;

    if(SimulateDayType_Nearest == nPlayType)
    {      
        string szDayTime(vDayTime[0]);

        int pos = 0;
        string subs[2];         
        for (int i=0;i<2;++i)
        {         
            int index = szDayTime.find(" ");
            subs[i] = szDayTime.substr(pos,index);
            pos += subs[i].size() + 1;
        } 

        memset(szTableName, 0, sizeof(szTableName));
        memset(szValiDatedate,0,sizeof(szValiDatedate));
        memset(szFirstUpdateTime,0,sizeof(szFirstUpdateTime));
        memset(szFirstUpdateTimePre, 0, sizeof(szFirstUpdateTimePre));

        memcpy(szValiDatedate,subs[0].c_str(),sizeof(szValiDatedate));
        memcpy(szFirstUpdateTime,subs[1].c_str(),sizeof(szFirstUpdateTime));
        memcpy(szFirstUpdateTimePre,szFirstUpdateTime,sizeof(szFirstUpdateTime));
        sprintf(szTableName,"%s_%s",QUOTTABLENAME,szValiDatedate);

        int szTime[3];
        memset(szTime,0,sizeof(szTime));
        sscanf_s(szFirstUpdateTime,"%2d:%2d:%2d",&szTime[0],&szTime[1],&szTime[2]);
        int nTimeTick = szTime[0]*60*60+szTime[1]*60+szTime[2];  


        while(1)
        {
            LOG_INFO("读线程正常工作......");
            Sleep(100);
            if(WaitForSingleObject(pInst->m_hReadStopEvent,0)==WAIT_OBJECT_0)
            {
                LOG_INFO("手动停止模拟行情，退出读线程");
                return 1;
            }
 
            if(pInst->GetSimulateQuotSize()>10000) //容器中不低于10000条数据,多余10000条时，等待30s
            {
                Sleep(30*1000);
                LOG_INFO("GetSimulateQuotSize()>10000");
                continue;
            }
        
            //取数据
            memset(szNextUpdateTime,0,sizeof(szNextUpdateTime));
            nTimeTick+=1*60;//每次取60秒的行情数据
            int shi = (nTimeTick/3600)%24;    
            int fen = nTimeTick/60%60;    
            int miao = nTimeTick%60;
            sprintf(szNextUpdateTime,"%.2d:%.2d:%.2d",shi,fen,miao);

            if(0==strcmp(szNextUpdateTime,szFirstUpdateTimePre))
            {
                LOG_INFO("模拟行情服务器指定回放数据已经读取完成!从头开始读取");
                //return 1;
            }

            memset(szBuffer, 0, sizeof(szBuffer));
            sprintf(szBuffer, "select * from \"%s\" t where t.updatetime between \'%s\' and \'%s\' and t.validatedate = \'%s\' order by t.updatetime",szTableName,szFirstUpdateTime,szNextUpdateTime,szValiDatedate);

            memcpy(szFirstUpdateTime,szNextUpdateTime,sizeof(szFirstUpdateTime));

            if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode) || vNode.size() == 0)
                continue;
            else
            {
                LOG_INFO("=====%s CONUT(%d)",szBuffer,(int)(vNode.size()));
                
                for ( UINT j = 0; j < vNode.size(); j++ )
                {
                    std::vector<_variant_t> vValue = vNode[j];

                    PlatformStru_DepthMarketData depthMarketData;

                    int i = 0;
                    strcpy(depthMarketData.TradingDay, vValue[i++].operator _bstr_t());	
                    strcpy(depthMarketData.InstrumentID, vValue[i++].operator _bstr_t());
                    strcpy(depthMarketData.ExchangeID, vValue[i++].operator _bstr_t());
                    strcpy(depthMarketData.ExchangeInstID, vValue[i++].operator _bstr_t());
                    depthMarketData.LastPrice = vValue[i++].dblVal;
                    depthMarketData.PreSettlementPrice = vValue[i++].dblVal;
                    depthMarketData.PreClosePrice = vValue[i++].dblVal;
                    depthMarketData.PreOpenInterest = vValue[i++].dblVal;
                    depthMarketData.OpenPrice = vValue[i++].dblVal;
                    depthMarketData.HighestPrice = vValue[i++].dblVal;
                    depthMarketData.LowestPrice = vValue[i++].dblVal;
                    depthMarketData.Volume = vValue[i++].intVal;
                    depthMarketData.Turnover = vValue[i++].dblVal;
                    depthMarketData.OpenInterest = vValue[i++].dblVal;
                    depthMarketData.ClosePrice = vValue[i++].dblVal;
                    depthMarketData.SettlementPrice = vValue[i++].dblVal;
                    depthMarketData.UpperLimitPrice = vValue[i++].dblVal;
                    depthMarketData.LowerLimitPrice = vValue[i++].dblVal;
                    depthMarketData.PreDelta = vValue[i++].dblVal;
                    depthMarketData.CurrDelta = vValue[i++].dblVal;
                    strcpy(depthMarketData.UpdateTime, vValue[i++].operator _bstr_t());
                    depthMarketData.UpdateMillisec = vValue[i++].intVal;
                    depthMarketData.BidPrice1 = vValue[i++].dblVal;
                    depthMarketData.BidVolume1 = vValue[i++].intVal;
                    depthMarketData.AskPrice1 = vValue[i++].dblVal;
                    depthMarketData.AskVolume1 = vValue[i++].intVal;
                    depthMarketData.BidPrice2 = vValue[i++].dblVal;
                    depthMarketData.BidVolume2 = vValue[i++].intVal;
                    depthMarketData.AskPrice2 = vValue[i++].dblVal;
                    depthMarketData.AskVolume2 = vValue[i++].intVal;
                    depthMarketData.BidPrice3 = vValue[i++].dblVal;
                    depthMarketData.BidVolume3 = vValue[i++].intVal;
                    depthMarketData.AskPrice3 = vValue[i++].dblVal;
                    depthMarketData.AskVolume3 = vValue[i++].intVal;
                    depthMarketData.BidPrice4 = vValue[i++].dblVal;
                    depthMarketData.BidVolume4 = vValue[i++].intVal;
                    depthMarketData.AskPrice4 = vValue[i++].dblVal;
                    depthMarketData.AskVolume4 = vValue[i++].intVal;
                    depthMarketData.BidPrice5 = vValue[i++].dblVal;
                    depthMarketData.BidVolume5 = vValue[i++].intVal;
                    depthMarketData.AskPrice5 = vValue[i++].dblVal;
                    depthMarketData.AskVolume5 = vValue[i++].intVal;
                    depthMarketData.AveragePrice = vValue[i++].dblVal;
                    depthMarketData.BidPrice6 = vValue[i++].dblVal;
                    depthMarketData.BidVolume6 = vValue[i++].intVal;
                    depthMarketData.AskPrice6 = vValue[i++].dblVal;
                    depthMarketData.AskVolume6 = vValue[i++].intVal;
                    depthMarketData.BidPrice7 = vValue[i++].dblVal;
                    depthMarketData.BidVolume7 = vValue[i++].intVal;
                    depthMarketData.AskPrice7 = vValue[i++].dblVal;
                    depthMarketData.AskVolume7 = vValue[i++].intVal;
                    depthMarketData.BidPrice8 = vValue[i++].dblVal;
                    depthMarketData.BidVolume8 = vValue[i++].intVal;
                    depthMarketData.AskPrice8 = vValue[i++].dblVal;
                    depthMarketData.AskVolume8 = vValue[i++].intVal;
                    depthMarketData.BidPrice9 = vValue[i++].dblVal;
                    depthMarketData.BidVolume9 = vValue[i++].intVal;
                    depthMarketData.AskPrice9 = vValue[i++].dblVal;
                    depthMarketData.AskVolume9 = vValue[i++].intVal;
                    depthMarketData.BidPrice10 = vValue[i++].dblVal;
                    depthMarketData.BidVolume10 = vValue[i++].intVal;
                    depthMarketData.AskPrice10 = vValue[i++].dblVal;
                    depthMarketData.AskVolume10 = vValue[i++].intVal;
                    depthMarketData.NewVolume = vValue[i++].intVal;

                    pInst->SetSimulateQuot(depthMarketData);
                }  

                vNode.clear();
            }    
        }
    }else if(SimulateDayType_Special == nPlayType)
    {  
        while(1)
        {
            if(WaitForSingleObject(pInst->m_hReadStopEvent,0)==WAIT_OBJECT_0)
            {
                LOG_INFO("手动停止模拟行情，退出读线程");
                return 1;
            }

            int nIndex = 0;
            for(;nIndex<(int)vDayTime.size();nIndex++)
            {
                string szDayTime(vDayTime[nIndex]);

                int pos = 0;
                string subs[2];         
                for (int i=0;i<2;++i)
                {         
                    int index = szDayTime.find(" ");
                    subs[i] = szDayTime.substr(pos,index);
                    pos += subs[i].size() + 1;
                } 

                memset(szTableName, 0, sizeof(szTableName));
                memset(szValiDatedate,0,sizeof(szValiDatedate));
                memset(szFirstUpdateTime,0,sizeof(szFirstUpdateTime));
                memcpy(szValiDatedate,subs[0].c_str(),sizeof(szValiDatedate));
                memcpy(szFirstUpdateTime,subs[1].c_str(),sizeof(szFirstUpdateTime));
                sprintf(szTableName,"%s_%s",QUOTTABLENAME,szValiDatedate);

                int szTime[3];
                memset(szTime,0,sizeof(szTime));
                sscanf_s(szFirstUpdateTime,"%2d:%2d:%2d",&szTime[0],&szTime[1],&szTime[2]);
                int nTimeTick = szTime[0]*60*60+szTime[1]*60+szTime[2]; 


                memset(szFirstUpdateTimePre, 0, sizeof(szFirstUpdateTimePre));
                memcpy(szFirstUpdateTimePre,szFirstUpdateTime,sizeof(szFirstUpdateTime));

                bool bLoop=false;

                while(!bLoop)
                {
                    if(WaitForSingleObject(pInst->m_hReadStopEvent,0)==WAIT_OBJECT_0)
                    {
                        LOG_INFO("手动停止模拟行情，退出读线程");
                        return 1;
                    }

                    if(pInst->GetSimulateQuotSize()>10000) //容器中不低于10000条数据,多余10000条时，等待30s
                    {
                        Sleep(30*1000);
                        LOG_INFO("GetSimulateQuotSize()>10000");
                        continue;
                    }

                    //取数据
                    memset(szNextUpdateTime,0,sizeof(szNextUpdateTime));
                    nTimeTick+=1*60;//每次取60秒的行情数据
                    int shi = (nTimeTick/3600)%24;    
                    int fen = nTimeTick/60%60;    
                    int miao = nTimeTick%60;
                    sprintf(szNextUpdateTime,"%.2d:%.2d:%.2d",shi,fen,miao);

                    if(0==strcmp(szNextUpdateTime,szFirstUpdateTimePre))
                    {
                        bLoop = true;
                        continue;
                    }

                    memset(szBuffer, 0, sizeof(szBuffer));
                    sprintf(szBuffer, "select * from \"%s\" t where t.updatetime between \'%s\' and \'%s\' and t.validatedate = \'%s\' order by t.updatetime",szTableName,szFirstUpdateTime,szNextUpdateTime,szValiDatedate);

                    memcpy(szFirstUpdateTime,szNextUpdateTime,sizeof(szFirstUpdateTime));

                    if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode) || vNode.size() == 0)
                        continue;
                    else
                    {
                        LOG_INFO("=====%s CONUT(%d)",szBuffer,(int)(vNode.size()));

                        for ( UINT j = 0; j < vNode.size(); j++ )
                        {
                            std::vector<_variant_t> vValue = vNode[j];

                            PlatformStru_DepthMarketData depthMarketData;

                            int i = 0;
                            strcpy(depthMarketData.TradingDay, vValue[i++].operator _bstr_t());	
                            strcpy(depthMarketData.InstrumentID, vValue[i++].operator _bstr_t());
                            strcpy(depthMarketData.ExchangeID, vValue[i++].operator _bstr_t());
                            strcpy(depthMarketData.ExchangeInstID, vValue[i++].operator _bstr_t());
                            depthMarketData.LastPrice = vValue[i++].dblVal;
                            depthMarketData.PreSettlementPrice = vValue[i++].dblVal;
                            depthMarketData.PreClosePrice = vValue[i++].dblVal;
                            depthMarketData.PreOpenInterest = vValue[i++].dblVal;
                            depthMarketData.OpenPrice = vValue[i++].dblVal;
                            depthMarketData.HighestPrice = vValue[i++].dblVal;
                            depthMarketData.LowestPrice = vValue[i++].dblVal;
                            depthMarketData.Volume = vValue[i++].intVal;
                            depthMarketData.Turnover = vValue[i++].dblVal;
                            depthMarketData.OpenInterest = vValue[i++].dblVal;
                            depthMarketData.ClosePrice = vValue[i++].dblVal;
                            depthMarketData.SettlementPrice = vValue[i++].dblVal;
                            depthMarketData.UpperLimitPrice = vValue[i++].dblVal;
                            depthMarketData.LowerLimitPrice = vValue[i++].dblVal;
                            depthMarketData.PreDelta = vValue[i++].dblVal;
                            depthMarketData.CurrDelta = vValue[i++].dblVal;
                            strcpy(depthMarketData.UpdateTime, vValue[i++].operator _bstr_t());
                            depthMarketData.UpdateMillisec = vValue[i++].intVal;
                            depthMarketData.BidPrice1 = vValue[i++].dblVal;
                            depthMarketData.BidVolume1 = vValue[i++].intVal;
                            depthMarketData.AskPrice1 = vValue[i++].dblVal;
                            depthMarketData.AskVolume1 = vValue[i++].intVal;
                            depthMarketData.BidPrice2 = vValue[i++].dblVal;
                            depthMarketData.BidVolume2 = vValue[i++].intVal;
                            depthMarketData.AskPrice2 = vValue[i++].dblVal;
                            depthMarketData.AskVolume2 = vValue[i++].intVal;
                            depthMarketData.BidPrice3 = vValue[i++].dblVal;
                            depthMarketData.BidVolume3 = vValue[i++].intVal;
                            depthMarketData.AskPrice3 = vValue[i++].dblVal;
                            depthMarketData.AskVolume3 = vValue[i++].intVal;
                            depthMarketData.BidPrice4 = vValue[i++].dblVal;
                            depthMarketData.BidVolume4 = vValue[i++].intVal;
                            depthMarketData.AskPrice4 = vValue[i++].dblVal;
                            depthMarketData.AskVolume4 = vValue[i++].intVal;
                            depthMarketData.BidPrice5 = vValue[i++].dblVal;
                            depthMarketData.BidVolume5 = vValue[i++].intVal;
                            depthMarketData.AskPrice5 = vValue[i++].dblVal;
                            depthMarketData.AskVolume5 = vValue[i++].intVal;
                            depthMarketData.AveragePrice = vValue[i++].dblVal;
                            depthMarketData.BidPrice6 = vValue[i++].dblVal;
                            depthMarketData.BidVolume6 = vValue[i++].intVal;
                            depthMarketData.AskPrice6 = vValue[i++].dblVal;
                            depthMarketData.AskVolume6 = vValue[i++].intVal;
                            depthMarketData.BidPrice7 = vValue[i++].dblVal;
                            depthMarketData.BidVolume7 = vValue[i++].intVal;
                            depthMarketData.AskPrice7 = vValue[i++].dblVal;
                            depthMarketData.AskVolume7 = vValue[i++].intVal;
                            depthMarketData.BidPrice8 = vValue[i++].dblVal;
                            depthMarketData.BidVolume8 = vValue[i++].intVal;
                            depthMarketData.AskPrice8 = vValue[i++].dblVal;
                            depthMarketData.AskVolume8 = vValue[i++].intVal;
                            depthMarketData.BidPrice9 = vValue[i++].dblVal;
                            depthMarketData.BidVolume9 = vValue[i++].intVal;
                            depthMarketData.AskPrice9 = vValue[i++].dblVal;
                            depthMarketData.AskVolume9 = vValue[i++].intVal;
                            depthMarketData.BidPrice10 = vValue[i++].dblVal;
                            depthMarketData.BidVolume10 = vValue[i++].intVal;
                            depthMarketData.AskPrice10 = vValue[i++].dblVal;
                            depthMarketData.AskVolume10 = vValue[i++].intVal;
                            depthMarketData.NewVolume = vValue[i++].intVal;

                            pInst->SetSimulateQuot(depthMarketData);
                        }  

                        vNode.clear();
                    } 
                }   
            }
        }
    }

    return 1;
}
DWORD SimulateSendProcess(void* lpParam)
{
    CSimulateQuotMgr *pInst = (CSimulateQuotMgr*)lpParam;
    if(!pInst)
    {
        LOG_INFO("模拟行情服务器回放发送线程失败!");
        return 0;
    }

    eSimulatePlayMode eMode;
    pInst->GetSimulatePlayMode(eMode);

    int nTimeBase=0,nTimeSecond=0,nWaitTime;
    int szTime[3];
    char szUpdateTime[9];
    bool bSecond = false;
    vector<PlatformStru_DepthMarketData> sVec;
    while (1)
    {
        LOG_INFO("发送线程正常工作......");
        Sleep(100);

        if(WaitForSingleObject(pInst->m_hSendStopEvent,0)==WAIT_OBJECT_0)
        {
            LOG_INFO("手动停止模拟行情，退出发送线程");
            return 1;
        }

        pInst->GetSimulateQuot(sVec);
        for (vector<PlatformStru_DepthMarketData>::iterator it = sVec.begin();it!=sVec.end();it++)
        {            
            if(WaitForSingleObject(pInst->m_hSendStopEvent,0)==WAIT_OBJECT_0)
            {
                LOG_INFO("手动停止模拟行情，退出发送线程");
                return 1;
            }

            PlatformStru_DepthMarketData data = (*it);

            if(SimulatePlayMode_Normal == eMode)
            {
                memset(szUpdateTime,0,sizeof(szUpdateTime));
                memcpy(szUpdateTime,data.UpdateTime,sizeof(szUpdateTime));

                memset(szTime,0,sizeof(szTime));
                sscanf_s(szUpdateTime,"%2d:%2d:%2d",&szTime[0],&szTime[1],&szTime[2]);
                nTimeSecond = szTime[0]*60*60+szTime[1]*60+szTime[2]; 

                if(bSecond)
                {
                    nWaitTime = abs(nTimeSecond-nTimeBase);                
                    Sleep(nWaitTime*1000);
                }  

                nTimeBase = nTimeSecond;
                bSecond = true;
            }
            else if(SimulatePlayMode_NoDelay == eMode)
            {
                //无延迟，立即发送
            }

            LOG_INFO("模拟调用 Inst=%s",data.InstrumentID);
            CPlatformCTPQuotImpl::GetInstance().OnRtnDepthMarketDataSimulate(&data);

            //更新模拟内存中最后一条行情数据
            CPlatformCTPQuotImpl::GetInstance().SetLastQuot( data );
        }

        sVec.clear();

        Sleep(100);
    }

    return 1;
}


CSimulateQuotMgr& CSimulateQuotMgr::getObj(void)
{
    m_mutexSimulate.lock();
    if (NULL == m_pObj)    
        m_pObj = new CSimulateQuotMgr();
    m_mutexSimulate.unlock();
    return *m_pObj;
}

void CSimulateQuotMgr::deleteObj()
{
    m_mutexSimulate.lock();
    if(NULL != m_pObj)
    {
        delete m_pObj;
        m_pObj = NULL;
    }	
    m_mutexSimulate.unlock();
}
CSimulateQuotMgr::CSimulateQuotMgr(void)
{
    m_hReadStopEvent = NULL;
    m_hSendStopEvent = NULL;

    m_bSimulateQuit = 0;
    m_bSimulatePause= 0;

    m_hReadThread_manual = NULL;
    m_hSendThread_manual = NULL;    
    
    m_idReadThread_manual = 0;
    m_idSendThread_manual = 0;

    m_eSimulatePlayMode=SimulatePlayMode_Normal;

    LoadSimulatePlayMode();
    LoadSimulateDay();
    LoadSimulateTime();
}

CSimulateQuotMgr::~CSimulateQuotMgr(void)
{
    SetSimulateQuit(0);
    SetSimulatePause(0);

    SetEvent(m_hReadStopEvent);
    SetEvent(m_hSendStopEvent);

    Sleep(3000);

    DWORD ExitCode;
    //读线程
    WaitForSingleObject((HANDLE)m_hReadThread_manual,8000);
    if(GetExitCodeThread((HANDLE)m_hReadThread_manual,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
        TerminateThread((HANDLE)m_hReadThread_manual,0);
    CloseHandle(m_hReadThread_manual);

    m_hReadThread_manual=NULL;
    m_idReadThread_manual=0;

    //发送线程
    WaitForSingleObject((HANDLE)m_hSendThread_manual,8000);
    if(GetExitCodeThread((HANDLE)m_hSendThread_manual,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
        TerminateThread((HANDLE)m_hSendThread_manual,0);
    CloseHandle(m_hSendThread_manual);

    m_hSendThread_manual=NULL;
    m_idSendThread_manual=0;

    CloseHandle(m_hReadStopEvent);
    m_hReadStopEvent = NULL;

    CloseHandle(m_hSendStopEvent);
    m_hSendStopEvent = NULL;
}
void CSimulateQuotMgr::LoadSimulatePlayMode()
{
    char lsPath[MAX_PATH] = {0};
    GetModuleFileName(NULL,lsPath,MAX_PATH);
    *(strrchr(lsPath,'\\') + 1)= NULL;
    strcat(lsPath,SIMULATE_FILE);

    m_eSimulatePlayMode = (!GetPrivateProfileInt("SimulatePlayMode","PlayMode",SimulatePlayMode_Normal,lsPath))?SimulatePlayMode_Normal:SimulatePlayMode_NoDelay;
}
CF_ERROR CSimulateQuotMgr::GetSimulatePlayMode(eSimulatePlayMode& eMode)
{
    eMode = m_eSimulatePlayMode;
    return CF_ERROR_SUCCESS;
}
CF_ERROR CSimulateQuotMgr::SetSimulatePlayMode(const eSimulatePlayMode& sMode)
{
    m_mutexSimulate.lock();
    m_eSimulatePlayMode = sMode;

    char lsPath[MAX_PATH] = {0};
    GetModuleFileName(NULL,lsPath,MAX_PATH);
    *(strrchr(lsPath,'\\') + 1)= NULL;
    strcat(lsPath,SIMULATE_FILE);

    char lsPlayMode[4];
    memset(lsPlayMode,0,sizeof(lsPlayMode));
    itoa(m_eSimulatePlayMode,lsPlayMode,10);

    WritePrivateProfileString("SimulatePlayMode","PlayMode",lsPlayMode,lsPath);
    m_mutexSimulate.unlock();
    return CF_ERROR_SUCCESS;
}
void CSimulateQuotMgr::LoadSimulateDay()
{
    char lsPath[MAX_PATH] = {0};
    GetModuleFileName(NULL,lsPath,MAX_PATH);
    *(strrchr(lsPath,'\\') + 1)= NULL;
    strcat(lsPath,SIMULATE_FILE);

    memset(&m_sSimulateDay,0,sizeof(sSimulateDay));

    int bNearestDay = GetPrivateProfileInt("SimulateDay","NearestDay",1,lsPath);
    if(!bNearestDay)
    {
        GetPrivateProfileString("SimulateDay","StartDay","20140220",m_sSimulateDay.mStartDay,sizeof(m_sSimulateDay.mStartDay),lsPath);
        GetPrivateProfileString("SimulateDay","EndDay"  ,"20140221",m_sSimulateDay.mEndDay  ,sizeof(m_sSimulateDay.mEndDay)  ,lsPath);
    }
    else
        m_sSimulateDay.mType = bNearestDay?SimulateDayType_Nearest:SimulateDayType_Special;
}
CF_ERROR CSimulateQuotMgr::GetSimulateDay(sSimulateDay& sDay)
{
    memcpy(&sDay,&m_sSimulateDay,sizeof(m_sSimulateDay));
    return CF_ERROR_SUCCESS;
}
CF_ERROR CSimulateQuotMgr::SetSimulateDay(const sSimulateDay& sDay)
{
    m_mutexSimulate.lock();
    memcpy(&m_sSimulateDay,&sDay,sizeof(sSimulateDay));

    std::string lsStartDay = sDay.mStartDay;
    std::string lsEndDay = sDay.mEndDay;
    char lsPath[MAX_PATH] = {0};
    GetModuleFileName(NULL,lsPath,MAX_PATH);
    *(strrchr(lsPath,'\\') + 1)= NULL;
    strcat(lsPath,SIMULATE_FILE);

    char lsNearest[4];
    memset(lsNearest,0,sizeof(lsNearest));
    itoa(sDay.mType,lsNearest,10);

    WritePrivateProfileString("SimulateDay","NearestDay",lsNearest,lsPath);
    WritePrivateProfileString("SimulateDay","StartDay",lsStartDay.c_str(),lsPath);
    WritePrivateProfileString("SimulateDay","EndDay"  ,lsEndDay.c_str(),lsPath);
    m_mutexSimulate.unlock();

    return CF_ERROR_SUCCESS;
}
void CSimulateQuotMgr::LoadSimulateTime()
{
    char lsPath[MAX_PATH] = {0};
    GetModuleFileName(NULL,lsPath,MAX_PATH);
    *(strrchr(lsPath,'\\') + 1)= NULL;
    strcat(lsPath,SIMULATE_FILE);

    SYSTEMTIME sys;
    GetLocalTime( &sys );

    memset(&m_sSimulateTime,0,sizeof(sSimulateTime));
    switch(sys.wDayOfWeek)
    {
    case Monday:
        GetPrivateProfileString("Monday","StartTime","20:30:30",m_sSimulateTime.mStartTime,sizeof(m_sSimulateTime.mStartTime),lsPath);
        GetPrivateProfileString("Monday","EndTime","08:30:30",m_sSimulateTime.mEndTime,sizeof(m_sSimulateTime.mEndTime),lsPath);
        break;
    case Tuesday:
        GetPrivateProfileString("Tuesday","StartTime","20:30:30",m_sSimulateTime.mStartTime,sizeof(m_sSimulateTime.mStartTime),lsPath);
        GetPrivateProfileString("Tuesday","EndTime","08:30:30",m_sSimulateTime.mEndTime,sizeof(m_sSimulateTime.mEndTime),lsPath);
        break;
    case Wednesday:
        GetPrivateProfileString("Wednesday","StartTime","20:30:30",m_sSimulateTime.mStartTime,sizeof(m_sSimulateTime.mStartTime),lsPath);
        GetPrivateProfileString("Wednesday","EndTime","08:30:30",m_sSimulateTime.mEndTime,sizeof(m_sSimulateTime.mEndTime),lsPath);
        break;
    case Thursday:
        GetPrivateProfileString("Thursday","StartTime","20:30:30",m_sSimulateTime.mStartTime,sizeof(m_sSimulateTime.mStartTime),lsPath);
        GetPrivateProfileString("Thursday","EndTime","08:30:30",m_sSimulateTime.mEndTime,sizeof(m_sSimulateTime.mEndTime),lsPath);
        break;
    case Friday:
        GetPrivateProfileString("Friday","StartTime","20:30:30",m_sSimulateTime.mStartTime,sizeof(m_sSimulateTime.mStartTime),lsPath);
        GetPrivateProfileString("Friday","EndTime","08:30:30",m_sSimulateTime.mEndTime,sizeof(m_sSimulateTime.mEndTime),lsPath);
        break;
    case Saturday:
        GetPrivateProfileString("Saturday","StartTime","20:30:30",m_sSimulateTime.mStartTime,sizeof(m_sSimulateTime.mStartTime),lsPath);
        GetPrivateProfileString("Saturday","EndTime","08:30:30",m_sSimulateTime.mEndTime,sizeof(m_sSimulateTime.mEndTime),lsPath);
        break;
    case Sunday:
        GetPrivateProfileString("Sunday","StartTime","20:30:30",m_sSimulateTime.mStartTime,sizeof(m_sSimulateTime.mStartTime),lsPath);
        GetPrivateProfileString("Sunday","EndTime","08:30:30",m_sSimulateTime.mEndTime,sizeof(m_sSimulateTime.mEndTime),lsPath);
        break;
    }
}
CF_ERROR CSimulateQuotMgr::GetSimulateTime(sSimulateTime& sTime)
{
    memcpy(&sTime,&m_sSimulateTime,sizeof(m_sSimulateTime));
    return CF_ERROR_SUCCESS;
}

void CSimulateQuotMgr::SetSimulateQuot(const PlatformStru_DepthMarketData& pDepthMarketData)
{ 
    m_mutexSimulate.lock();
    m_queueSimulateQuot.push(pDepthMarketData);
    m_mutexSimulate.unlock();
}
void CSimulateQuotMgr::GetSimulateQuot(std::vector<PlatformStru_DepthMarketData>& vecdepthMarketData)
{
    m_mutexSimulate.lock();
    while (!m_queueSimulateQuot.empty())
    {                
        PlatformStru_DepthMarketData targetData = m_queueSimulateQuot.front();
        m_queueSimulateQuot.pop();

        vecdepthMarketData.push_back(targetData);
    } 
    m_mutexSimulate.unlock();
}
int CSimulateQuotMgr::GetSimulateQuotSize()
{
    return (int)(m_queueSimulateQuot.size());
}

bool CSimulateQuotMgr::GetSimulateData(eSimulateDayType& nType,vector<string>& vDayTime)
{     
    char szTableName[19]; //AQUOTINFO_20140225
    char szDataTime[18];
    char szValiDatedate[9];
    char szUpdateTime[9];

    int nErrorCode = CF_ERROR_SUCCESS;
    char szBuffer[MAX_SQL_LENGTH];    
    std::vector<std::vector<_variant_t>> vNode;
    
    nType = m_sSimulateDay.mType;

    if(SimulateDayType_Nearest == nType)
    {
        //取当前本地系统交易日  
        
        int N;//追溯前30天  
        time_t timer; 
        time(&timer);
        tm* t_tm = localtime(&timer); 
        switch(t_tm->tm_wday)
        {
        case Monday:
        case Tuesday:
        case Wednesday:
        case Thursday:
        case Friday:
            N=0;
            break;
        case Saturday:
            N=1;
            break;
        case Sunday:
            N=2;
            break;
        }

   
        while( N<30) 
        {  
            time_t cur_time = time(NULL) - N*24*60*60;
            t_tm = gmtime(&cur_time);

            if(t_tm->tm_wday == Saturday || t_tm->tm_wday == Sunday)
            {
                N++;
                continue;
            }

            memset(szTableName,0,sizeof(szTableName));
            sprintf(szTableName,"%s_%04d%02d%02d",QUOTTABLENAME,t_tm->tm_year+1900,t_tm->tm_mon+1,t_tm->tm_mday);

            memset(szBuffer, 0, sizeof(szBuffer));
            sprintf(szBuffer, "select  tt.validatedate, tt.updatetime from (select  validatedate, updatetime from %s order by updatetime) tt where rownum = 1",szTableName);
            LOG_INFO("%s",szBuffer);

            if (CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode) || vNode.size() == 0)          
            {
                memset(szDataTime,0,sizeof(szDataTime));
                memset(szValiDatedate,0,sizeof(szValiDatedate));
                memset(szUpdateTime,0,sizeof(szUpdateTime));

                std::vector<_variant_t> vValue = vNode[0]; 
                strcpy(szValiDatedate, vValue[0].operator _bstr_t());  //VALIDATEDATE 20140114
                strcpy(szUpdateTime, vValue[1].operator _bstr_t()); //UPDATETIME 09:00:00
                sprintf(szDataTime,"%s %s",szValiDatedate,szUpdateTime);
                 
                vDayTime.push_back((string)szDataTime);

                LOG_INFO("--------%s %s",szValiDatedate,szUpdateTime);
                return true;
            }

            N++;
        }
    }else if(SimulateDayType_Special == nType)
    {
        memset(szTableName,0,sizeof(szTableName));
        sprintf(szTableName,"%s_%s",QUOTTABLENAME,m_sSimulateDay.mStartDay);

        time_t lTime;
        struct tm * lTimeinfo = NULL; 
        time(&lTime);
        lTimeinfo = localtime(&lTime);

        struct tm  *pTime;
        struct tm  lStartDayinfo ;
        struct tm  lEndDayinfo ; 	
        memcpy(&lStartDayinfo,lTimeinfo,sizeof(tm));
        memcpy(&lEndDayinfo,lTimeinfo,sizeof(tm));

        sscanf_s(m_sSimulateDay.mStartDay,"%04d%02d%02d",&lStartDayinfo.tm_year,&lStartDayinfo.tm_mon,&lStartDayinfo.tm_mday);
        sscanf_s(m_sSimulateDay.mEndDay,  "%04d%02d%02d",&lEndDayinfo.tm_year,  &lEndDayinfo.tm_mon,  &lEndDayinfo.tm_mday);

        lStartDayinfo.tm_year -= 1900;
        lStartDayinfo.tm_mon -= 1;
        lEndDayinfo.tm_year -= 1900;
        lEndDayinfo.tm_mon -= 1;

        time_t lStartDay = mktime(&lStartDayinfo);
        time_t lEndDay   = mktime(&lEndDayinfo);                

        time_t lDifftime = lEndDay - lStartDay;
        while( lDifftime>=0) 
        {  
            memset(szBuffer, 0, sizeof(szBuffer));
            sprintf(szBuffer, "select  tt.validatedate, tt.updatetime from (select  validatedate, updatetime from %s order by updatetime) tt where rownum = 1",szTableName);
            LOG_INFO("%s",szBuffer);

            vNode.clear();
            if ( CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode) || vNode.size() == 0)
            {
                memset(szDataTime,0,sizeof(szDataTime));
                memset(szValiDatedate,0,sizeof(szValiDatedate));
                memset(szUpdateTime,0,sizeof(szUpdateTime));

                std::vector<_variant_t> vValue = vNode[0]; 
                strcpy(szValiDatedate, vValue[0].operator _bstr_t());  //VALIDATEDATE 20140114
                strcpy(szUpdateTime, vValue[1].operator _bstr_t()); //UPDATETIME 09:00:00
                sprintf(szDataTime,"%s %s",szValiDatedate,szUpdateTime);

                vDayTime.push_back((string)szDataTime);

                LOG_INFO("--------%s %s",szValiDatedate,szUpdateTime);
            }

            //从开始的一天开始找
            lStartDay += 24*60*60;//一天
            lDifftime = lEndDay - lStartDay;

            pTime = localtime(&lStartDay);
            memset(szTableName,0,sizeof(szTableName));
            sprintf(szTableName,"%s_%04d%02d%02d",QUOTTABLENAME,pTime->tm_year+1900,pTime->tm_mon+1,pTime->tm_mday);
        }   
        
        return ((int)vDayTime.size()>0);
    }
   
    return false;
}

void CSimulateQuotMgr::SetSimulateQuit(int nVal)
{
    int lnSimulate = nVal;
    InterlockedExchange((volatile long*)(&m_bSimulateQuit),lnSimulate);
}
bool CSimulateQuotMgr::GetSimulateQuit()
{
    int lnSimulate = 0;
    InterlockedExchange((volatile long*)(&lnSimulate),m_bSimulateQuit);
    return lnSimulate != 0;
}

void CSimulateQuotMgr::SetSimulatePause(int nVal)
{
    int lnSimulate = nVal;
    InterlockedExchange((volatile long*)(&m_bSimulatePause),lnSimulate);
}
bool CSimulateQuotMgr::GetSimulatePause()
{
    int lnSimulate = 0;
    InterlockedExchange((volatile long*)(&lnSimulate),m_bSimulatePause);
    return lnSimulate != 0;
}



//模拟控制
bool CSimulateQuotMgr::SimulateQuotPlay()
{
    if(!GetSimulatePause())
    {
        //一次手动过来，自动就失效。
        m_hReadStopEvent = CreateEvent(NULL, FALSE, FALSE , NULL);
        m_hSendStopEvent = CreateEvent(NULL, FALSE, FALSE , NULL);

        //读取数据线程
        m_hReadThread_manual=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)SimulateReadProcess,this,0,&m_idReadThread_manual);
        //发送数据线程
        m_hSendThread_manual=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)SimulateSendProcess,this,0,&m_idSendThread_manual);
    }
    else
    {
        ResumeThread(m_hReadThread_manual);
        ResumeThread(m_hSendThread_manual);
    }

    return true;
}

bool CSimulateQuotMgr::SimulateQuotPause()
{
    SetSimulatePause(1);
    SuspendThread(m_hReadThread_manual);
    SuspendThread(m_hSendThread_manual);
    return false;
}

bool CSimulateQuotMgr::SimulateQuotStop()
{
    if(m_bSimulatePause)
    {
        ResumeThread(m_hReadThread_manual);
        ResumeThread(m_hSendThread_manual);
    }

    SetSimulatePause(0);
    SetSimulateQuit(1);

    SetEvent(m_hReadStopEvent);
    SetEvent(m_hSendStopEvent);

    DWORD ExitCode;
    //读线程
    WaitForSingleObject((HANDLE)m_hReadThread_manual,8000);
    if(GetExitCodeThread((HANDLE)m_hReadThread_manual,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
        TerminateThread((HANDLE)m_hReadThread_manual,0);
    CloseHandle(m_hReadThread_manual);

    m_hReadThread_manual=NULL;
    m_idReadThread_manual=0;

    //发送线程
    WaitForSingleObject((HANDLE)m_hSendThread_manual,8000);
    if(GetExitCodeThread((HANDLE)m_hSendThread_manual,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
        TerminateThread((HANDLE)m_hSendThread_manual,0);
    CloseHandle(m_hSendThread_manual);

    m_hSendThread_manual=NULL;
    m_idSendThread_manual=0;

    CloseHandle(m_hReadStopEvent);
    m_hReadStopEvent = NULL;

    CloseHandle(m_hSendStopEvent);
    m_hSendStopEvent = NULL;

    return false;
}