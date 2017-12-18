// OptionStrategyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OptionStrategy.h"
#include "OptionStrategyDlg.h"
#include "TransparentHelper.h"
#include "TCPDataService.h"
#include "CommPublic.h"
#include "ConfigMgr.h"


#include "..\..\TPriceList\TPriceListDLL.h"

#pragma warning (disable:4244)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


COptionStrategyDlg* COptionStrategyDlg::m_pMainDlg = NULL;
int COptionStrategyDlg::PluginsAPICallBack(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId)
{
    switch(nCmdId)
    {
     case CMDID_UI_Change_ColorScheme:
     case CMDID_UI_ColorScheme_RSP:
         {
             int nColorScheme = *(int*)pData;
             m_pMainDlg->SetBKColorMode(nColorScheme?Color_Thick:Color_Light);
             m_pMainDlg->UpdataColorMode(nColorScheme?Color_Thick:Color_Light);
             break;
         }
    case CMDID_CONSOLE_LOGON_RSP:
        {
            //登录返回
            if(nErrorCode==SUCCESS)
            {
                CDataInfo::NewInstance()->m_bLogon=true;

                //查询账号//查询交易日
                PlusinAPI* pAPI=PlusinAPI::GetInstance();
                if(pAPI)
                {
                    pAPI->Send(CMDID_ACCOUNT_CURRENT_ACCOUNT_QUERY,NULL,0);

                    CDataInfo::NewInstance()->SetTradingDayFlag(false);
                    pAPI->Send(CMDID_TRADINGDAY_QUERY,NULL,0);
                }
            }
            else 
                CDataInfo::NewInstance()->m_bLogon=false;

#ifdef _LOG   
            if(CDataInfo::NewInstance()->m_pLog)
            {
                char buf[256];
                memset(buf,0,sizeof(buf));
                sprintf(buf,"PlusinAPI 登录%s\n",CDataInfo::NewInstance()->m_bLogon?"成功":"失败");

                CDataInfo::NewInstance()->m_pLog->Log(buf);
            }
#endif

            break;
        }
    case CMDID_TRADINGDAY_RSP:
        {
            if(nErrorCode==SUCCESS)
            {
                char buff[32];
                memset(buff,0,sizeof(buff));
                memcpy(buff,pData,sizeof(buff));

                CDataInfo::NewInstance()->m_TradingDay = string(buff);
                CDataInfo::NewInstance()->SetTradingDayFlag(true);
            }

            break;
        }
    case CMDID_CONSOLE_CLOSE:
        {
            ::PostMessage(m_pMainDlg->GetSafeHwnd(), WM_CLOSE, 0, NULL);
            break;
        }
    case CMDID_ACCOUNT_CURRENT_ACCOUNT_RSP:
        {
            //查询账号返回
            if(nErrorCode==SUCCESS&&nDataLen==(int)sizeof(CommonAccountIDType))
            {
                CommonAccountIDType& pAccount=*(CommonAccountIDType*)pData;
                if(pAccount[sizeof(CommonAccountIDType)-1]!=0)
                    pAccount[sizeof(CommonAccountIDType)-1]=0;
                CDataInfo::NewInstance()->m_szAccount=string(pAccount); 
#ifdef _LOG 
                if(CDataInfo::NewInstance()->m_pLog)
                {
                    char buf[256];
                    memset(buf,0,sizeof(buf));
                    sprintf(buf,"查询账号返回 Account=%s\n",CDataInfo::NewInstance()->m_szAccount.c_str());

                    CDataInfo::NewInstance()->m_pLog->Log(buf);
                }
#endif
                //查询交易所
                PlusinAPI* pAPI=PlusinAPI::GetInstance();
                if(pAPI)
                    pAPI->Send(CMDID_EXCHANGE_LIST_QUERY,NULL,0);
            }
            break;
        } 
    case CMDID_CONSOLE_FASTTRADER_TO_PLUSIN_SHOW:
        {
            //LoadXMlConfig();            
            //if(!bMultiInstance)
            //{
            //    HANDLE   hMutex=::CreateMutex(NULL,TRUE,"OnlyOneSingleApp");
            //    if (hMutex!=NULL)  
            //    {  
            //        if(GetLastError()==ERROR_ALREADY_EXISTS)  
            //        {  
            //            AfxMessageBox("该配置只能有一个应用程序运行.");  
            //            return   FALSE;  
            //        }  
            //    } 
            //}
            //else
            //{

            //}
            
            PlusinAPI* pAPI=PlusinAPI::GetInstance();
            if(pAPI)
            {
                pAPI->Send(CMDID_ORDER_CURRENT_INSTRUENT_QUERY, NULL, 0);
                pAPI->Send(CMDID_UI_ColorScheme_QUERY, NULL, 0);
            }

            break;
        }
    case CMDID_ORDER_CURRENT_INSTRUENT_RSP:
        {
            if(nErrorCode==SUCCESS && pData!=NULL && nDataLen==sizeof(InstrumentIDType))
            {
                 m_pMainDlg->m_szCurrInstrumentID = ((string)(char*)pData);

                //检查是否可以显示主窗口
                m_pMainDlg->SetTimer(3,800,NULL);                
            }
        }
    case CMDID_EXCHANGE_CURRENT_TIME_RSP:
        {
            if(nErrorCode==SUCCESS&&nDataLen==(int)sizeof(ExchangeCurTime))
            {
                char* pExchangeData = (char*)pData;
            }
            break;
        }
        //查询交易所返回
    case CMDID_EXCHANGE_LIST_RSP:
        {            
            if(nErrorCode==SUCCESS)
            {
                char* pExchangeData = (char*)pData;
                char ExchangeID[sizeof(CommonExchangeIDType)];
                set<string> setExchangeID;  

                int nCount = nDataLen/sizeof(CommonExchangeIDType);
                for(int i=0;i<nCount;i++)
                {
                    memset(ExchangeID,0,sizeof(CommonExchangeIDType));
                    memcpy(ExchangeID,pExchangeData+i*sizeof(CommonExchangeIDType),sizeof(CommonExchangeIDType));

                    setExchangeID.insert((string)ExchangeID);

#ifdef _LOG 
                    if(CDataInfo::NewInstance()->m_pLog)
                    {
                        char buf[64];
                        memset(buf,0,sizeof(buf));
                        sprintf(buf,"交易所 %s\n",ExchangeID);

                        CDataInfo::NewInstance()->m_pLog->Log(buf);
                    }
#endif
                } 

                CDataInfo::NewInstance()->SetExchangeFlag(true);
                //////////////////////////////////////////////////////////////////////////
                //查询指定交易所的期权标的物品种列表
                PlusinAPI* pAPI=PlusinAPI::GetInstance();
                if(pAPI)
                {
                    char ExchangeID[31];
                    for (set<string>::iterator it=setExchangeID.begin();it!=setExchangeID.end();it++)
                    {
                        //TRACE("ExchangeID = %s\n",(char*)(*it).c_str());
                        memset(ExchangeID,0,sizeof(ExchangeID));
                        memcpy(ExchangeID,(char*)(*it).c_str(),sizeof(ExchangeID));
                        pAPI->Send(CMDID_OPTIONUNDERLYINGPRODUCT_LIST_QUERY,ExchangeID,sizeof(ExchangeID));
                    }
                }
            }
            break;
        } 
        //查询指定交易所的期权标的物品种列表返回
    case CMDID_OPTIONUNDERLYINGPRODUCT_LIST_RSP:
        {            
            if(nErrorCode==SUCCESS)
            {
                char* pInstrumentData = (char*)pData;
                char ExchangeID[sizeof(CommonExchangeIDType)],ProductID[sizeof(CommonInstrumentIDType)];
                set<string> setProducID;
                setProducID.clear();

                int nCount = nDataLen/sizeof(CommonInstrumentIDType);
                for(int i=0;i<nCount;i++)
                {                    
                    memset(ProductID,0,sizeof(CommonInstrumentIDType));
                    if(i==0)
                    {
                        memset(ExchangeID,0,sizeof(CommonInstrumentIDType));
                        memcpy(ExchangeID,pInstrumentData,sizeof(CommonExchangeIDType));
                    }
                    else
                    {
                        memcpy(ProductID,pInstrumentData+i*sizeof(CommonInstrumentIDType),sizeof(CommonInstrumentIDType));
                        setProducID.insert((string)ProductID);
                    }
                } 

                CDataInfo::NewInstance()->SetProductFlag(true);
                CDataInfo::NewInstance()->m_mapExchangeID2ProductID.insert(make_pair((string)ExchangeID,setProducID));
                //////////////////////////////////////////////////////////////////////////
                ////查询指定品种对应的合约
                PlusinAPI* pAPI=PlusinAPI::GetInstance();
                if(pAPI)
                {
                    char ProducID[31];
                    for (set<string>::iterator it=setProducID.begin();it!=setProducID.end();it++)
                    {
                        memset(ProducID,0,sizeof(ProducID));
                        memcpy(ProducID,(char*)(*it).c_str(),sizeof(ProducID));

                        pAPI->Send(CMDID_INSTRUMENT_LIST_BYPRODUCTID_QUERY,ProducID,sizeof(ProducID));
                    }
                }
            }
            break;
        } 
        //查询指定品种对应的合约返回
    case CMDID_INSTRUMENT_LIST_BYPRODUCTID_RSP:
        {            
            if(nErrorCode==SUCCESS)
            {
                char* pInstrumentData = (char*)pData;
                char ProductID[sizeof(CommonInstrumentIDType)],InstID[sizeof(CommonInstrumentIDType)];
                set<string> setInstID;

                int nCount = nDataLen/sizeof(CommonInstrumentIDType);
                for(int i=0;i<nCount;i++)
                {
                    
                    memset(InstID,0,sizeof(CommonInstrumentIDType));

                    if(i==0)
                    {
                        memset(ProductID,0,sizeof(CommonInstrumentIDType));  
                        memcpy(ProductID,pInstrumentData,sizeof(CommonInstrumentIDType));
                    }
                    else
                    {
                        memcpy(InstID,pInstrumentData+i*sizeof(CommonInstrumentIDType),sizeof(CommonInstrumentIDType));
                        setInstID.insert((string)InstID);
                    }
                } 

                CDataInfo::NewInstance()->SetInstrumentFlag(true);
                CDataInfo::NewInstance()->m_mapProductID2InstID.insert(make_pair((string)ProductID,setInstID));
                //////////////////////////////////////////////////////////////////////////
                m_pMainDlg->UpdateCombBoxCtrl();
            }
            break;
        } 
        //查询标的物相关合约返回
    case CMDID_OPTIONINSTRUMENTSBYUNDERLYING_LIST_RSP:
        {
            if(nErrorCode==SUCCESS)
            {     
                string strLog("");


                char* pInstrumentData = (char*)pData;
                char underlyingInstrumentID[sizeof(CommonInstrumentIDType)],InstID[sizeof(CommonInstrumentIDType)];
                set<string> setRelaInstID;

                int nCount = nDataLen/sizeof(CommonInstrumentIDType);
                for(int i=0;i<nCount;i++)
                {                    
                    memset(InstID,0,sizeof(CommonInstrumentIDType));

                    if(i==0)
                    {
                        memset(underlyingInstrumentID,0,sizeof(CommonInstrumentIDType)); 
                        memcpy(underlyingInstrumentID,pInstrumentData,sizeof(CommonInstrumentIDType));
#ifdef _LOG              
                        char buf[512];
                        memset(buf,0,sizeof(buf));
                        sprintf(buf,"查询标的物相关合约列表返回\n标的物 %s\n相关合约\n",underlyingInstrumentID);
                        strLog += (string)buf; 
#endif
                    }
                    else
                    {
                        memcpy(InstID,pInstrumentData+i*sizeof(CommonInstrumentIDType),sizeof(CommonInstrumentIDType));
                        setRelaInstID.insert((string)InstID);
#ifdef _LOG 
                        memset(buf,0,sizeof(buf));
                        sprintf(buf,"%s\n",InstID);
                        strLog += (string)buf; 
#endif
                    }
                }

                //////////////////////////////////////////////////////////////////////////
                CeasymutexGuard guard(CDataInfo::NewInstance()->m_mutexRelaInst);
                CDataInfo::NewInstance()->m_setRelaInstrumentInfoReq.clear();
                CDataInfo::NewInstance()->m_vecRelaInstrumentInfoReq.clear();
                PlusinAPI* pAPI=PlusinAPI::GetInstance();
                if(pAPI)
                { 
                    //查询标的物快照、合约信息,并订阅行情
                    pAPI->Send(CMDID_QUOTE_SNAPSHOT_QUERY,underlyingInstrumentID,sizeof(InstrumentIDType));
                    pAPI->Send(CMDID_INSTRUMENT_INFO_QUERY,underlyingInstrumentID,sizeof(InstrumentIDType));
                    pAPI->Send(CMDID_QUOTE_SUBSCRIBE,underlyingInstrumentID,sizeof(InstrumentIDType));   

                    //保存当前订阅行情的合约名称
                    CDataInfo::NewInstance()->m_SubscribedInstrument=(string)underlyingInstrumentID;
#ifdef _LOG 
                    memset(buf,0,sizeof(buf));
                    sprintf(buf,"查询标的物合约信息,并订阅行情 %s\n查询标的物相关合约信息,并订阅行情\n",underlyingInstrumentID);
                    strLog += (string)buf; 
#endif

                    string strInstID;
                    char RelaInstID[sizeof(InstrumentIDType)],strTemp[sizeof(InstrumentIDType)];
                    set<string>::iterator it = setRelaInstID.begin();
                    while (it!=setRelaInstID.end())
                    {
                        strInstID = *it;
                        memset(strTemp,0,sizeof(InstrumentIDType));
                        memcpy(strTemp,strInstID.c_str(),sizeof(InstrumentIDType)-1);

                        memset(RelaInstID,0,sizeof(InstrumentIDType));
                        memcpy(RelaInstID,strTemp,sizeof(InstrumentIDType)-1);

                        //查询标的物相关合约信息
                        pAPI->Send(CMDID_INSTRUMENT_INFO_QUERY,RelaInstID,sizeof(InstrumentIDType));
                        CDataInfo::NewInstance()->m_setRelaInstrumentInfoReq.insert((string)RelaInstID);
                        
                        //查询标的物相关合约行情快照
                        pAPI->Send(CMDID_QUOTE_SNAPSHOT_QUERY,RelaInstID,sizeof(InstrumentIDType));
                        //订阅标的物相关合约行情
                        pAPI->Send(CMDID_QUOTE_SUBSCRIBE,RelaInstID,sizeof(InstrumentIDType));
                        ++it;
#ifdef _LOG 
                        memset(buf,0,sizeof(buf));
                        sprintf(buf,"%s\n",RelaInstID);
                        strLog += (string)buf; 
#endif
                    }

#ifdef _LOG 
                    if(CDataInfo::NewInstance()->m_pLog)
                    {
                        CDataInfo::NewInstance()->m_pLog->Log(strLog.c_str());
                    }
#endif
                }
            }

            break;
        }
    case CMDID_INSTRUMENT_INFO_RSP:
        {
            if(nErrorCode==SUCCESS && pData!=NULL && nDataLen==sizeof(PlatformStru_InstrumentInfo))
            {
                PlatformStru_InstrumentInfo* pInfo = (PlatformStru_InstrumentInfo*)pData;

                CeasymutexGuard guard(CDataInfo::NewInstance()->m_mutexRelaInst);

                //标的物相关合约信息
                if(strncmp(CDataInfo::NewInstance()->m_SubscribedInstrument.c_str(),pInfo->InstrumentID,sizeof(pInfo->InstrumentID))!=0)
                {
                    set<string>::iterator it = CDataInfo::NewInstance()->m_setRelaInstrumentInfoReq.begin();
                    while (it!=CDataInfo::NewInstance()->m_setRelaInstrumentInfoReq.end())
                    {
                        if (::strncmp(pInfo->InstrumentID,(*it).c_str(),sizeof(pInfo->InstrumentID))==0)
                        {
                            it = CDataInfo::NewInstance()->m_setRelaInstrumentInfoReq.erase(it);
                            break;
                        }

                        ++it;
                    }

                    CDataInfo::NewInstance()->m_vecRelaInstrumentInfoReq.push_back(*pInfo);
                    //要等全部信息返回后再更新TPriceListPanel
                    if(CDataInfo::NewInstance()->m_setRelaInstrumentInfoReq.size()==0)
                    {
                        SetOptionInstruments(CDataInfo::NewInstance()->m_vecRelaInstrumentInfoReq);
                        sort(CDataInfo::NewInstance()->m_vecRelaInstrumentInfoReq.begin(),CDataInfo::NewInstance()->m_vecRelaInstrumentInfoReq.end(),CDataInfo::upSort);
                    }
                }
                else
                {
                    //标的物合约信息
                    CDataInfo::NewInstance()->SetInstrumentInfo(*pInfo);
                    m_pMainDlg->SetSubscribedInstInfo(*pInfo);
                    //XListCtrl插入到期日
                    ::SendMessage(m_pMainDlg->m_T_PriceModuleDialog.GetSafeHwnd(),WM_USER_QUOT_UPDATE,0,0); 
                    ::SendMessage(m_pMainDlg->m_AnalyseModuleDialog.GetSafeHwnd(),WM_USER_EXPIREDATE_UPDATE,0,0);
                } 
            }

            break;
        }
        //订阅合约行情返回
    case CMDID_QUOTE_SUBSCRIBE_RSP:
        {
            if(nErrorCode == 1)
                TRACE("订阅的合约数量超过限制");
            if(nErrorCode == 999)
                TRACE("未知错误");
#ifdef _LOG 
            if(m_pMainDlg->m_pLog)
            {
                char buf[512];
                memset(buf,0,sizeof(buf));
                sprintf(buf,"订阅合约行情返回(%s nErrorCode=%d)\n",(nErrorCode==SUCCESS)?"成功":"失败",nErrorCode);

                m_pMainDlg->m_pLog->Log(buf);
            }
#endif
            break;
        }
    case CMDID_QUOTE_SNAPSHOT_RSP://返回行情快照
        {
            if(nErrorCode == SUCCESS && nDataLen%sizeof(PlatformStru_DepthMarketData)==0)
            {
                PlatformStru_DepthMarketData* pMarketData=(PlatformStru_DepthMarketData*)pData; 
                //标的物相关合约信息
                if(strncmp(CDataInfo::NewInstance()->m_SubscribedInstrument.c_str(),pMarketData->InstrumentID,sizeof(pMarketData->InstrumentID))==0)
                {
                    CDataInfo::NewInstance()->SetLastPriceFlag(true);
                    CDataInfo::NewInstance()->SetInstrumentDepthData(*pMarketData);
                    ::SendMessage(m_pMainDlg->m_T_PriceModuleDialog.GetSafeHwnd(),WM_USER_QUOT_UPDATE,(WPARAM)pMarketData,0);
                }
                else//更新TPriceListPanel
                    NewMarketData(*pMarketData); 

                //更新内存合约行情                
                CDataInfo::NewInstance()->SetLastQuotInfo(*pMarketData);
            }
        }
        break;
    case CMDID_QUOTE_SEND_QUOTE:
        {
            //接收到行情推送
            if(nErrorCode==SUCCESS&&nDataLen==sizeof(PlatformStru_DepthMarketData))
            {
                PlatformStru_DepthMarketData* pMarketData=(PlatformStru_DepthMarketData*)pData;  
#ifdef _LOG   
                if(m_pMainDlg->m_pLog)
                {
                    char buf[256];
                    memset(buf,0,sizeof(buf));
                    sprintf(buf,"接收到行情推送 %s\n",pMarketData->InstrumentID);

                    m_pMainDlg->m_pLog->Log(buf);
                }
#endif
                //标的物行情
                if(CDataInfo::NewInstance()->m_SubscribedInstrument.compare((string)pMarketData->InstrumentID)==0) 
                {
                    CDataInfo::NewInstance()->SetLastPriceFlag(true);
                    CDataInfo::NewInstance()->SetInstrumentDepthData(*pMarketData);
                    ::SendMessage(m_pMainDlg->m_T_PriceModuleDialog.GetSafeHwnd(),WM_USER_QUOT_UPDATE,(WPARAM)pMarketData,0);
                }
                else
                {
                    //更新TPriceListPanel
                    NewMarketData(*pMarketData);
                }
                
                //更新内存合约行情                
                CDataInfo::NewInstance()->SetLastQuotInfo(*pMarketData);         
            }
            break;
        }  
        //PositionKey 持仓返回
    case CMDID_POSITION_INFO_RSP:
        {
            if(nErrorCode == SUCCESS && nDataLen%sizeof(PlatformStru_Position)==0) 
            {
                //更新持仓数量
                ::SendMessage(m_pMainDlg->m_QuickOrderModuleDialog.GetSafeHwnd(),WM_USER_POSITION_UPDATE,(WPARAM)pData,0);
            }
            break;             
        }
        //////////////////////////////////////////////////////////////////////////
    case CMDID_ORDER_INSERT_ORDER_RSP://下单请求响应
        {
            if(nErrorCode == SUCCESS && pData!=NULL && nDataLen%sizeof(PlatformStru_InputOrder)==0 && nDataLen>0)
            {                
                PlatformStru_InputOrder *pInputOrder = (PlatformStru_InputOrder*)pData;
                if(pInputOrder==NULL)
                    break;
#ifdef _LOG 
                if(CDataInfo::NewInstance()->m_pLog)
                {
                    char buf[512];
                    memset(buf,0,sizeof(buf));
                    sprintf(buf,"下单成功返回 RequestId=%d,Instrument=%s,OrderRef=%s,CombOffsetFlag=%s,Direction=%s,LimitPrice=%.6f,VolumeTotalOriginal=%d\n",
                        nRequestId,pInputOrder->InstrumentID,pInputOrder->OrderRef,pInputOrder->CombOffsetFlag,
                        pInputOrder->Direction==THOST_FTDC_D_Buy?"买":"卖",pInputOrder->LimitPrice,pInputOrder->VolumeTotalOriginal);

                    CDataInfo::NewInstance()->m_pLog->Log(buf);
                }
#endif
            }
            break;
        }
    case  CMDID_ORDER_RSP_ORDER_INSERT1://下单失败返回
        {
            PlatformStru_RspOrderInsert1* pRspOrderInsert1 = (PlatformStru_RspOrderInsert1*)pData;
            if(pRspOrderInsert1==NULL)
                break;
#ifdef _LOG 
            if(CDataInfo::NewInstance()->m_pLog)
            {
                char buf[512];
                memset(buf,0,sizeof(buf));
                sprintf(buf,"下单失败1返回 ParamID=%d,Instrument=%s,OrderRef=%s,LimitPrice=%.6f\n",
                    nRequestId,pRspOrderInsert1->InputOrderInsertField.InstrumentID,pRspOrderInsert1->InputOrderInsertField.OrderRef,
                    pRspOrderInsert1->InputOrderInsertField.LimitPrice);

                CDataInfo::NewInstance()->m_pLog->Log(buf);
            }
#endif
            break;
        }
    case  CMDID_ORDER_RSP_ORDER_INSERT2://下单失败返回
        {
            PlatformStru_RspOrderInsert2* pRspOrderInsert2 = (PlatformStru_RspOrderInsert2*)pData;
            if(pRspOrderInsert2==NULL)
                break;
#ifdef _LOG 
            if(CDataInfo::NewInstance()->m_pLog)
            {
                char buf[512];
                memset(buf,0,sizeof(buf));
                sprintf(buf,"下单失败2返回 ParamID=%d,Instrument=%s,OrderRef=%s,LimitPrice=%.6f\n",
                    nRequestId,pRspOrderInsert2->OrderInsertField.InstrumentID,pRspOrderInsert2->OrderInsertField.OrderRef,
                    pRspOrderInsert2->OrderInsertField.LimitPrice);

                CDataInfo::NewInstance()->m_pLog->Log(buf);
            }
#endif
            break;
        }
    case CMDID_ORDER_TRADE_RTN:
        {
            //成交回报
            if(nDataLen==sizeof(PlatformStru_TradeInfo)&&m_pMainDlg!=NULL)
            {
                //m_pMainDlg->TradeInfoReach(*(PlatformStru_TradeInfo*)pData);
                PlatformStru_TradeInfo* pTradeInfo = (PlatformStru_TradeInfo*)pData;        
                char positionDirection;
                if(pTradeInfo->OffsetFlag == THOST_FTDC_OF_Close)
                    positionDirection = (pTradeInfo->Direction==THOST_FTDC_D_Buy)?THOST_FTDC_PD_Short:THOST_FTDC_PD_Long;
                else if(pTradeInfo->OffsetFlag == THOST_FTDC_OF_Open)
                    positionDirection = (pTradeInfo->Direction==THOST_FTDC_D_Buy)?THOST_FTDC_PD_Long:THOST_FTDC_PD_Short;

                PositionKey key(pTradeInfo->Account,
                                pTradeInfo->InstrumentID,
                                positionDirection,
                                pTradeInfo->HedgeFlag);
                PlusinAPI* pAPI=PlusinAPI::GetInstance();
                if(pAPI)
                {
                    pAPI->Send(CMDID_POSITION_INFO_QUERY2, &key,sizeof(PositionKey));
                }
            }
            break;
        }
    case CMDID_ORDER_ORDER_STATUS_CHANGE:
        {
            //报单状态回报
            //if(nDataLen==sizeof(PlatformStru_OrderInfo)&&m_pMainDlg!=NULL)
            //{
            //    m_pMainDlg->OnOrderRtn(*(PlatformStru_OrderInfo*)pData);
            //}
            break;
        }
    }
    return 0;
}

LRESULT COptionStrategyDlg::OnRecvMessage( WPARAM wParam, LPARAM lParam )
{
    if ( 0 == lParam )
    {
        return -1;
    }

    RecvData* pRecvData = (RecvData*)lParam;
    if ( pRecvData->head.cmdid == Cmd_QueryVolatilityData_Rsp )
    {
        if ( pRecvData->head.userdata1 == SUCCESS )
        {
            Volatility *pVolatility = (Volatility *)pRecvData->pData;
            CDataInfo::NewInstance()->SetVolatility(pVolatility->VolatilityTwenty);
            CDataInfo::NewInstance()->SetVolatilityFlag(true);

            //通知UI更新
            ::SendMessage(m_QuickOrderModuleDialog.GetSafeHwnd(),WM_USER_VOLATILITY_UPDATE,0,0);
            ::SendMessage(m_AnalyseModuleDialog.GetSafeHwnd(),WM_USER_VOLATILITY_UPDATE,0,0);
        }
    }

    return 0;
}


void COptionStrategyDlg::UpdateCombBoxCtrl()
{
    m_combExchange.ResetContent();

    map<string,set<string>> mapExchangeID2ProductID; 
    CDataInfo::NewInstance()->GetExchangeID2ProductID(mapExchangeID2ProductID);

    string exchangeID;
    map<string,set<string>>::iterator it = mapExchangeID2ProductID.begin();
    while (it!=mapExchangeID2ProductID.end())
    {
        m_combExchange.AddString((it->first).c_str());
        ++it;
    }
}

// COptionStrategyDlg dialog

COptionStrategyDlg::COptionStrategyDlg(int nWidth, int nHight, float fSplitterH, float fSplitterV, CWnd* pParent /*=NULL*/)
	: CBKDialog(COptionStrategyDlg::IDD, pParent)
    , m_nResolutionWidth(nWidth)
    , m_nResolutionHight(nHight) 
    , m_SplitterHRate(fSplitterH)
    , m_SplitterVRate(fSplitterV)
    , m_strNote(_T(""))
    , m_bVisible(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_pMemDC = NULL;
    m_pOldBitmap = NULL;
    m_pBitmapMem = NULL;
    m_pMainDlg = this;
}
COptionStrategyDlg::~COptionStrategyDlg()
{
    m_pMainDlg = NULL;

    if ( m_pMemDC )
    {
        delete m_pBitmapMem;
        m_pBitmapMem = NULL;
        delete m_pMemDC;
        m_pMemDC = NULL;
    }
}

LRESULT COptionStrategyDlg::QuotUpdateTimeSetMsgHandler(WPARAM wParam,LPARAM lParam)
{
    int nState = *(int*)wParam;
    int nValue = *(int*)lParam;

    KillTimer(1);

    if(nState)
        SetTimer(1,nValue*1000,NULL);

    return 1;
}
LRESULT COptionStrategyDlg::VolatilityReSetMsgHandler(WPARAM wParam,LPARAM lParam)
{
    UpdateLineChartCtrl();
    return 1;
}
void COptionStrategyDlg::DoDataExchange(CDataExchange* pDX)
{
    CBKDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_TITLE, m_staticTitle);
    DDX_Control(pDX, IDC_COMBO_EXCHANGE, m_combExchange);
    DDX_Control(pDX, IDC_COMBO_STYLE, m_combProduct);
    DDX_Control(pDX, IDC_COMBO_INSTRUMENT, m_combInstrument);
    DDX_Control(pDX, IDC_BUTTON_CLOSE, m_btnClose);
    DDX_Control(pDX, IDC_BUTTON_MIN, m_btnMin);
    DDX_Control(pDX, IDC_BUTTON_MAX, m_btnMax);
}

BEGIN_MESSAGE_MAP(COptionStrategyDlg, CBKDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_WM_SIZE()
    ON_WM_NCHITTEST()
    ON_WM_SETCURSOR()
    ON_WM_NCLBUTTONDOWN()
    ON_NOTIFY(SPN_MAXMINPOS, IDC_SPLITTERH, OnMaxMinInfo)
    ON_NOTIFY(SPN_MAXMINPOS, IDC_SPLITTERV, OnMaxMinInfo) 
    ON_NOTIFY(SPN_DELTA,     IDC_SPLITTERV, OnSplitterVDelta)
    ON_MESSAGE( WM_TRANSPARENT_BK, OnTransaprentBk)
    ON_WM_GETMINMAXINFO()
    ON_BN_CLICKED(IDC_BUTTON_CLOSE, &COptionStrategyDlg::OnBnClickedButtonClose)
    ON_CBN_SELCHANGE(IDC_COMBO_EXCHANGE, &COptionStrategyDlg::OnCbnSelchangeComboExchange)
    ON_CBN_SELCHANGE(IDC_COMBO_STYLE, &COptionStrategyDlg::OnCbnSelchangeComboStyle)
    ON_CBN_SELCHANGE(IDC_COMBO_INSTRUMENT, &COptionStrategyDlg::OnCbnSelchangeComboInstrument)
    ON_MESSAGE(RECV_DATA_MSG, &COptionStrategyDlg::OnRecvMessage)
    ON_MESSAGE(WM_USER_QUOT_UPDATE,QuotUpdateTimeSetMsgHandler)
    ON_MESSAGE(WM_VOLATILITY_RESET,VolatilityReSetMsgHandler)
    ON_WM_TIMER()
    ON_WM_WINDOWPOSCHANGING()
    ON_BN_CLICKED(IDC_BUTTON_MIN, &COptionStrategyDlg::OnBnClickedButtonMin)
    ON_BN_CLICKED(IDC_BUTTON_MAX, &COptionStrategyDlg::OnBnClickedButtonMax)
END_MESSAGE_MAP()


// COptionStrategyDlg message handlers

BOOL COptionStrategyDlg::OnInitDialog()
{
	CBKDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here   
    if(g_hWnd_FT!=NULL)
    {
        PlusinAPI* pAPI=PlusinAPI::GetInstance();
        if(pAPI)
        {
            pAPI->Init( "OptionStrategy",
                        "OptionStrategy",
                        &COptionStrategyDlg::PluginsAPICallBack,
                        g_hWnd_FT,
                        NEED_NO_POS_NO_BIND,
                        GetSafeHwnd());
            pAPI->Logon();
        }
    }

    CTCPDataService::GetInstance()->RegisteHWnd(m_hWnd);


    this->SetWindowText("期权策略");
    this->SetWindowPos(NULL,0,0,m_nResolutionWidth,m_nResolutionHight,SWP_FRAMECHANGED|SWP_NOMOVE);

    m_btnMin.SetBitmaps(IDB_BITMAP_BTNMIN, RGB(0, 255, 0), (int)BTNST_AUTO_DARKER);
    m_btnMax.SetBitmaps(IDB_BITMAP_BTNMAX, RGB(0, 255, 0), (int)BTNST_AUTO_DARKER);
    m_btnClose.SetBitmaps(IDB_BITMAP_BTNCLOSE, RGB(0, 255, 0), (int)BTNST_AUTO_DARKER);

//     m_btnMin.DrawTransparent();
//     m_btnMax.DrawTransparent();
//     m_btnClose.DrawTransparent();


    m_T_PriceModuleDialog.Create(IDD_T_PIRCE_DIALOG,this);  
    m_T_PriceModuleDialog.ShowWindow(SW_SHOW);

    m_QuickOrderModuleDialog.Create(IDD_QUICKORDER_DIALOG,this); 
    m_QuickOrderModuleDialog.SetParent(this);
    m_QuickOrderModuleDialog.ShowWindow(SW_SHOW);

    m_AnalyseModuleDialog.Create(IDD_ANALYSE_DIALOG,this);
    m_AnalyseModuleDialog.SetParent(this);
    m_AnalyseModuleDialog.ShowWindow(SW_SHOW);

    //create splitter widows
    CreateSplitterWnd();

    //move contorls to postion
    Resize();  

    CRect rcMain;
    GetWindowRect(&rcMain);

    CRect rcSplitterV;
    m_wndSplitterV.GetWindowRect(&rcSplitterV);
    ScreenToClient(&rcSplitterV);

    CRect rcSplitterH;
    m_wndSplitterH.GetWindowRect(&rcSplitterH);
    ScreenToClient(&rcSplitterH);


    m_SplitterVRate = rcSplitterV.left*1.0f/rcMain.Width();
    m_SplitterHRate = rcSplitterH.top*1.0f/rcMain.Height();
    m_staticTitle.SetFocus();


    SetTimer(2,1000,NULL);//更新注意信息
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR COptionStrategyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void COptionStrategyDlg::OnSize(UINT nType, int cx, int cy)
{
    CBKDialog::OnSize(nType, cx, cy);

    // TODO: Add your message handler code here
    BuildBkDC();
    Resize();

    if(m_btnMax.GetSafeHwnd())
        m_btnMax.RedrawWindow();
}

void COptionStrategyDlg::OnMaxMinInfo(NMHDR* pNMHDR, LRESULT* pResult)
{
    //  Get current pos of the child controls
    CRect rc;
    this->GetWindowRect(rc);
    this->ScreenToClient(rc);

    //  return the pos limit
    SPC_NM_MAXMINPOS* pNewMaxMinPos = (SPC_NM_MAXMINPOS*)pNMHDR;
    if (IDC_SPLITTERV == pNMHDR->idFrom)
    {
        pNewMaxMinPos->lMin = rc.left + 456;
        pNewMaxMinPos->lMax = rc.right - 248;
    }
    else
    {
        pNewMaxMinPos->lMin = rc.top + 200;
        pNewMaxMinPos->lMax = rc.bottom - 140;
    }
}
void COptionStrategyDlg::UpdataColorMode(BK_COLOR_MODE eMode)
{
    SetBKColorMode(eMode);
    m_T_PriceModuleDialog.UpdataColorMode(eMode);
    m_QuickOrderModuleDialog.UpdataColorMode(eMode);
    m_AnalyseModuleDialog.UpdataColorMode(eMode);
    Invalidate();
    UpdateWindow();
}
void COptionStrategyDlg::OnSplitterVDelta(NMHDR* pNMHDR, LRESULT* pResult)
{
    //  this function just want to show you how to use the delta event
    *pResult = 0;

    SPC_NM_DELTA* pDelta = (SPC_NM_DELTA*)pNMHDR;
    if (NULL == pDelta)
    {
        return;
    }
}
void COptionStrategyDlg::Resize()
{
    CRect rcDlg;
    this->GetClientRect(rcDlg);

    CRect rcTitle;
    rcTitle.left    = rcDlg.left    + STD_GAP;
    rcTitle.right   = rcDlg.right   - STD_GAP;
    rcTitle.top     = rcDlg.top     + STD_GAP;
    rcTitle.bottom  = rcTitle.top   + STD_TITLE_HEIGHT;

    if (FALSE == IsWindow(m_wndSplitterV.GetSafeHwnd()))
    {
        return;
    }

    CRect rcBtnClose;
    rcBtnClose.left     = rcTitle.right-20;
    rcBtnClose.right    = rcTitle.right;
    rcBtnClose.top      = rcTitle.top;
    rcBtnClose.bottom   = rcTitle.top+20;
    this->m_btnClose.MoveWindow(rcBtnClose, TRUE);

    CRect rcBtnMin;
    rcBtnMin.left     = rcTitle.right-62;
    rcBtnMin.right    = rcBtnMin.left+20;
    rcBtnMin.top      = rcTitle.top;
    rcBtnMin.bottom   = rcTitle.top+20;
    this->m_btnMin.MoveWindow(rcBtnMin, TRUE);

    CRect rcBtnMax;
    rcBtnMax.left     = rcTitle.right-41;
    rcBtnMax.right    = rcBtnMax.left+20;
    rcBtnMax.top      = rcTitle.top;
    rcBtnMax.bottom   = rcTitle.top+20;
    this->m_btnMax.MoveWindow(rcBtnMax, TRUE);

    CRect rcNote;
    rcNote.left     = rcTitle.left;
    rcNote.right    = rcNote.left+250;
    rcNote.top      = rcDlg.bottom-25;
    rcNote.bottom   = rcNote.top+20;
    GetDlgItem(IDC_STATIC_NOTE)->MoveWindow(rcNote, TRUE);

    CRect rcSplitV;
    m_wndSplitterV.GetWindowRect(rcSplitV);
    this->ScreenToClient(rcSplitV);
    rcSplitV.top = rcTitle.bottom + STD_GAP;
    rcSplitV.bottom = rcDlg.bottom - STD_GAP;
    rcSplitV.left = rcDlg.Width()*m_SplitterVRate;
    rcSplitV.right = rcSplitV.left + SPLIT_WIDTH;
    this->m_wndSplitterV.MoveWindow(rcSplitV, TRUE);

    CRect rcSplitH;
    m_wndSplitterH.GetWindowRect(rcSplitH);
    this->ScreenToClient(rcSplitH);
    rcSplitH.left = rcDlg.left + STD_GAP;
    rcSplitH.right = rcSplitV.left;
    rcSplitH.top = rcDlg.Height()*m_SplitterHRate;
    rcSplitH.bottom = rcSplitH.top + SPLIT_WIDTH;
    this->m_wndSplitterH.MoveWindow(rcSplitH, TRUE);

    CRect rcPrice;
    rcPrice.left     = rcTitle.left;
    rcPrice.right    = rcSplitV.left;
    rcPrice.top      = rcTitle.bottom + STD_GAP;
    rcPrice.bottom   = rcSplitH.top;
    this->m_T_PriceModuleDialog.MoveWindow(rcPrice, TRUE);

    CRect rcOrder;
    rcOrder.top      = rcSplitH.bottom;
    rcOrder.bottom   = rcDlg.bottom - STD_GAP;
    rcOrder.left     = rcTitle.left;
    rcOrder.right    = rcSplitV.left;
    this->m_QuickOrderModuleDialog.MoveWindow(rcOrder, TRUE);

    CRect rcAnalyse;
    rcAnalyse.left     = rcSplitV.right;
    rcAnalyse.right    = rcTitle.right;
    rcAnalyse.top      = rcTitle.bottom + STD_GAP;
    rcAnalyse.bottom   = rcDlg.bottom - STD_GAP;
    this->m_AnalyseModuleDialog.MoveWindow(rcAnalyse, TRUE);
}

void COptionStrategyDlg::MoveDlgItem(int nID, const CRect& rcPos, BOOL bRepaint)
{
    CWnd* pWnd = this->GetDlgItem(nID);
    if (NULL == pWnd)
    {
        return;
    }

    pWnd->MoveWindow(rcPos, bRepaint);

    CRect rcsp;
    m_wndSplitterH.GetWindowRect(rcsp);
    this->ScreenToClient(rcsp);
}

void COptionStrategyDlg::CreateSplitterWnd(void)
{
    //  create splittet windows
    CRect rc;
    CWnd *pWnd;

    pWnd = GetDlgItem(IDC_SPLITTERV);
    pWnd->GetWindowRect(rc);
    ScreenToClient(rc);
    BOOL bRet = m_wndSplitterV.Create(WS_CHILD | WS_VISIBLE, rc, this, IDC_SPLITTERV, SPS_VERTICAL|SPS_DELTA_NOTIFY, RGB(0, 255, 0),1);
    if (FALSE == bRet)
    {
        AfxMessageBox("m_wndSplitter1 create failed");
    }

    pWnd = GetDlgItem(IDC_SPLITTERH);
    pWnd->GetWindowRect(rc);
    ScreenToClient(rc);
    bRet = m_wndSplitterH.Create(WS_CHILD | WS_VISIBLE, rc, this, IDC_SPLITTERH, SPS_HORIZONTAL, RGB(0, 255, 0),1);
    if (FALSE == bRet)
    {
        AfxMessageBox("m_wndSplitter2 create failed");
    }

    //  register windows for splitter
    this->m_wndSplitterV.RegisterLinkedWindow(SPLS_LINKED_LEFT,     &m_T_PriceModuleDialog);
    this->m_wndSplitterV.RegisterLinkedWindow(SPLS_LINKED_LEFT,     &m_QuickOrderModuleDialog);
    this->m_wndSplitterV.RegisterLinkedWindow(SPLS_LINKED_LEFT,     &m_wndSplitterH);
    this->m_wndSplitterV.RegisterLinkedWindow(SPLS_LINKED_RIGHT,    &m_AnalyseModuleDialog);

    this->m_wndSplitterH.RegisterLinkedWindow(SPLS_LINKED_UP,       &m_T_PriceModuleDialog);
    this->m_wndSplitterH.RegisterLinkedWindow(SPLS_LINKED_DOWN,     &m_QuickOrderModuleDialog);

    //  relayout the splotter to make them good look
    this->m_wndSplitterV.Relayout();
    this->m_wndSplitterH.Relayout();
}
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void COptionStrategyDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        //CBKDialog::OnPaint();
        CPaintDC dc(this);
        CRect rtClient;
        GetClientRect(&rtClient);

        BuildBkDC();
        if ( m_pMemDC )
        {
            dc.BitBlt( 0,0,rtClient.Width(), rtClient.Height(), m_pMemDC, 0,0, SRCCOPY);
        }
    }
}

LRESULT COptionStrategyDlg::OnTransaprentBk( WPARAM wParam, LPARAM lParam)
{
    HDC hdc = ( HDC)wParam;
    HWND hwnd = ( HWND)lParam;
    CTransparentHelper::OnTranparentControl( m_pMemDC->GetSafeHdc(), (WPARAM)hdc, (LPARAM)hwnd);
    return TRUE;
}

void COptionStrategyDlg::BuildBkDC()
{  
    CDC* pWndDC = GetDC();

    CBitmap bmpbk;
    (Color_Light == m_ColorMode)?bmpbk.LoadBitmap(IDB_BITMAPOptionStrategyBK):bmpbk.LoadBitmap(IDB_BITMAPOptionStrategyBK_Deep);
    BITMAP bmpInfo;
    bmpbk.GetBitmap( &bmpInfo);

    CBitmap bmpbkTitle;
    (Color_Light == m_ColorMode)?bmpbkTitle.LoadBitmap(IDB_BITMAPOptionStrategyTitle):bmpbkTitle.LoadBitmap(IDB_BITMAPOptionStrategyTitle_Deep);
    BITMAP bmpInfoTitle;
    bmpbkTitle.GetBitmap( &bmpInfoTitle);

    CDC memdcBK;
    memdcBK.CreateCompatibleDC( pWndDC);
    CBitmap* pOld = memdcBK.SelectObject( &bmpbk);

    CDC memdcTitle;
    memdcTitle.CreateCompatibleDC( pWndDC);
    CBitmap* pOldTitle = memdcTitle.SelectObject( &bmpbkTitle);


    CRect rtClient;
    GetClientRect(&rtClient);

    if ( m_pMemDC )
    {
        m_pMemDC->SelectObject( m_pOldBitmap);
        delete m_pBitmapMem;
        m_pBitmapMem = NULL;
        delete m_pMemDC;
        m_pMemDC = NULL;
    }

    if ( m_pMemDC == NULL )
    {
        m_pMemDC = new CDC;
        m_pMemDC->CreateCompatibleDC( pWndDC);

        m_pBitmapMem = new CBitmap;
        m_pBitmapMem->CreateCompatibleBitmap( pWndDC, rtClient.Width(), rtClient.Height());
        m_pOldBitmap = (CBitmap*)m_pMemDC->SelectObject( m_pBitmapMem);
    }

    m_pMemDC->StretchBlt( 0,STD_GAP + STD_TITLE_HEIGHT, rtClient.Width(), rtClient.Height(), &memdcBK, 0,0,bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);
    m_pMemDC->StretchBlt( 0,0, rtClient.Width(), STD_GAP + STD_TITLE_HEIGHT, &memdcTitle, 0,0,bmpInfoTitle.bmWidth, bmpInfoTitle.bmHeight, SRCCOPY);

    memdcBK.SelectObject( pOld);
    memdcTitle.SelectObject( pOldTitle);

    ReleaseDC( pWndDC);


    //when the parent dialog's background is rebuild, notify the child which has an transparent tag.
    CTransparentHelper::NotifyTransparentChild( GetSafeHwnd());
}

//无标题栏操作相关
LRESULT mMouseStates = 0;
LRESULT COptionStrategyDlg::OnNcHitTest(CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    CRect rect;
    GetWindowRect(&rect);
    if(point.x <= rect.left+3)
        return (mMouseStates=HTLEFT);
    else if(point.x >= rect.right-3)
        return (mMouseStates=HTRIGHT);
    else if(point.y <= rect.top+3)
        return (mMouseStates=HTTOP);
    else if(point.y >= rect.bottom-3)
        return (mMouseStates=HTBOTTOM);
    else if(point.x <= rect.left+10 && point.y <= rect.top+10)
        return (mMouseStates=HTTOPLEFT);
    else if(point.x >= rect.right-10 && point.y <= rect.top+10)
        return (mMouseStates=HTTOPRIGHT);
    else if(point.x <= rect.left+10 && point.y >= rect.bottom-10)
        return (mMouseStates=HTBOTTOMLEFT);
    else if(point.x >= rect.right-10 && point.y >= rect.bottom-10)
        return (mMouseStates=HTBOTTOMRIGHT);
    else
        return rect.PtInRect(point) ? HTCAPTION : CBKDialog::OnNcHitTest(point);  

    return CBKDialog::OnNcHitTest(point);
}

BOOL COptionStrategyDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    // TODO: Add your message handler code here and/or call default
    if(nHitTest == HTCAPTION || nHitTest == HTSYSMENU ||
        nHitTest == HTMENU || nHitTest == HTCLIENT)
    {
        SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
    }
    else if(nHitTest == HTTOP || nHitTest == HTBOTTOM)
    {
        SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS)));
    }
    else if(nHitTest == HTLEFT || nHitTest == HTRIGHT)
    {
        SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));
    }
    else if(nHitTest == HTTOPLEFT || nHitTest == HTBOTTOMRIGHT)
    {
        SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENWSE)));
    }
    else if(nHitTest == HTTOPRIGHT || nHitTest == HTBOTTOMLEFT)
    {
        SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENESW)));
    }
    else
    {
        SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
    }

    return CBKDialog::OnSetCursor(pWnd, nHitTest, message);
}

void COptionStrategyDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if(nHitTest == HTTOP)
        SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_TOP, MAKELPARAM(point.x, point.y));
    else if(nHitTest == HTBOTTOM)
        SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOM, MAKELPARAM(point.x, point.y));
    else if(nHitTest == HTLEFT)
        SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_LEFT, MAKELPARAM(point.x, point.y));
    else if(nHitTest == HTRIGHT)
        SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_RIGHT, MAKELPARAM(point.x, point.y));
    else if(nHitTest == HTTOPLEFT)
        SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPLEFT, MAKELPARAM(point.x, point.y));
    else if(nHitTest == HTTOPRIGHT)
        SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPRIGHT, MAKELPARAM(point.x, point.y));
    else if(nHitTest == HTBOTTOMLEFT)
        SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMLEFT, MAKELPARAM(point.x, point.y));
    else if(nHitTest == HTBOTTOMRIGHT)
        SendMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMRIGHT, MAKELPARAM(point.x, point.y));

    CBKDialog::OnNcLButtonDown(nHitTest, point);
}

void COptionStrategyDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
    // TODO: Add your message handler code here and/or call default
    lpMMI->ptMinTrackSize.x = 800;
    lpMMI->ptMinTrackSize.y = 500; 
    CBKDialog::OnGetMinMaxInfo(lpMMI);
}
void COptionStrategyDlg::OnBnClickedButtonClose()
{
    CBKDialog::OnOK();
    // TODO: Add your control notification handler code here
    if(g_hWnd_FT!=NULL)
    {
        PlusinAPI* pAPI=PlusinAPI::GetInstance();
        if(pAPI)
            pAPI->Logout();

        PlusinAPI::DestroyInstance();
    }

    CTCPDataService::GetInstance()->UnRegisteHWnd(m_hWnd);

    OnOK();
}

void COptionStrategyDlg::OnCbnSelchangeComboExchange()
{
    // TODO: Add your control notification handler code here
    CString str;
    m_combExchange.GetLBText(m_combExchange.GetCurSel(),str);

    m_combProduct.ResetContent();

    map<string,set<string>> mapExchangeID2ProductID;
    CDataInfo::NewInstance()->GetExchangeID2ProductID(mapExchangeID2ProductID);

    set<string> setProduct;
    map<string,set<string>>::iterator it = mapExchangeID2ProductID.find((string)str.GetBuffer(0));
    if(it!=mapExchangeID2ProductID.end())
    {
        setProduct = it->second;
        set<string>::iterator it_ = setProduct.begin();
        while(it_!=setProduct.end())
        {
            m_combProduct.AddString((*it_).c_str());
            ++it_;
        }
    }
}

void COptionStrategyDlg::OnCbnSelchangeComboStyle()
{
    // TODO: Add your control notification handler code here
    CString str;
    m_combProduct.GetLBText(m_combProduct.GetCurSel(),str);

    m_combInstrument.ResetContent();

    map<string,set<string>> mapProductID2InstID;
    CDataInfo::NewInstance()->GetProductID2InstID(mapProductID2InstID);


    set<string> setInst;
    map<string,set<string>>::iterator it = mapProductID2InstID.find((string)str.GetBuffer(0));
    if(it!=mapProductID2InstID.end())
    {
        setInst = it->second;
        set<string>::iterator it_ = setInst.begin();
        while(it_!=setInst.end())
        {
            m_combInstrument.AddString((*it_).c_str());
            ++it_;
        }
    }
}

void COptionStrategyDlg::OnCbnSelchangeComboInstrument()
{
    // TODO: Add your control notification handler code here
    CString str;   
    char instID[sizeof(InstrumentIDType)];

    m_combInstrument.GetLBText(m_combInstrument.GetCurSel(),str);
    if(str.IsEmpty())
        return;

    string subScribedInstrument(str.GetBuffer(0)), strLog("");

    if(g_hWnd_FT!=NULL)
    {
        PlusinAPI* pAPI=PlusinAPI::GetInstance();
        if(pAPI)
        {
            if(CDataInfo::NewInstance()->m_SubscribedInstrument.compare(subScribedInstrument)!=0)
            {
                memset(instID,0,sizeof(InstrumentIDType));
                memcpy(instID,CDataInfo::NewInstance()->m_SubscribedInstrument.c_str(),sizeof(subScribedInstrument)-1);
                
                CeasymutexGuard guard(CDataInfo::NewInstance()->m_mutexRelaInst);

                //退订指定标的物合约行情
                if(strlen(instID)>0)
                    pAPI->Send(CMDID_QUOTE_UNSUBSCRIBE,instID,sizeof(InstrumentIDType));
#ifdef _LOG   
                char buf[256];
                memset(buf,0,sizeof(buf));
                sprintf(buf,"退订旧期权合约 %s\n",instID);
                strLog += (string)buf;
#endif
                //退订指定标的物相关合约行情
                vector<PlatformStru_InstrumentInfo>::iterator it = CDataInfo::NewInstance()->m_vecRelaInstrumentInfoReq.begin();
                while (it!=CDataInfo::NewInstance()->m_vecRelaInstrumentInfoReq.end())
                {
                    pAPI->Send(CMDID_QUOTE_UNSUBSCRIBE,it->InstrumentID,sizeof(InstrumentIDType));
                    
#ifdef _LOG   
                    memset(buf,0,sizeof(buf));
                    sprintf(buf,"退订旧标的物相关合约 %s\n",it->InstrumentID);

                    strLog += (string)buf;
#endif
                    ++it;
                }


                //查询标的物相关合约
                pAPI->Send(CMDID_OPTIONINSTRUMENTSBYUNDERLYING_LIST_QUERY,(char*)subScribedInstrument.c_str(),31);
#ifdef _LOG   
                memset(buf,0,sizeof(buf));
                sprintf(buf,"查询标的物相关合约列表 %s\n",subScribedInstrument.c_str());

                strLog += (string)buf;
#endif
                //查询标的物合约波动率
                CDataInfo::NewInstance()->SetVolatilityFlag(false);
                CDataInfo::NewInstance()->SetLastPriceFlag(false);

                Volatility sVolatility;
                if(CDataInfo::NewInstance()->GetTradingDayFlag())
                {
                    memcpy(sVolatility.TradingDay, CDataInfo::NewInstance()->m_TradingDay.c_str(),sizeof(TThostFtdcDateType));
                    memcpy(sVolatility.InstrumentID,subScribedInstrument.c_str(),sizeof(InstrumentIDType));
                    CTCPDataService::GetInstance()->SendData(Cmd_QueryVolatilityData_Req, &sVolatility, sizeof(Volatility));
                }
                else
                    CDataInfo::NewInstance()->SetNoteText("交易日没有查询到！");

#ifdef _LOG   
                memset(buf,0,sizeof(buf));
                sprintf(buf,"查询标的物合约波动率 %s\n",subScribedInstrument.c_str());

                CDataInfo::NewInstance()->m_pLog->Log(strLog.c_str());
#endif

                m_QuickOrderModuleDialog.ClearListCtrl();
            }
        }
    }

    m_T_PriceModuleDialog.SetFocus();
}

void COptionStrategyDlg::UpdateLineChartCtrl()
{
    m_AnalyseModuleDialog.UpdateLineChartCtrl();
}
void COptionStrategyDlg::UpdateXListCtrl(const PlatformStru_OrderInfo& outOrder)
{
    m_QuickOrderModuleDialog.UpdateXListCtrl(outOrder);
}
void COptionStrategyDlg::SetSubscribedInstInfo(const PlatformStru_InstrumentInfo& sInstInfo)
{
    m_QuickOrderModuleDialog.SetSubscribedInst(sInstInfo);
}
void COptionStrategyDlg::ReqPosition(const PositionKey& key)
{
    PositionKey sendKey(key);
    PlusinAPI* pAPI=PlusinAPI::GetInstance();
    if(pAPI)
    {
            pAPI->Send(CMDID_POSITION_INFO_QUERY2, &sendKey,sizeof(PositionKey));
    }
}
void COptionStrategyDlg::NewOrder(vector<PlatformStru_OrderInfo>& vecOrders)
{   
    PlusinAPI* pAPI=PlusinAPI::GetInstance();
    if(pAPI)
    {
        PlatformStru_InputOrder tInputOrder;
        PlatformStru_OrderInfo sOrderInfo;
        
        for (vector<PlatformStru_OrderInfo>::const_iterator it=vecOrders.begin(); it!=vecOrders.end(); ++it)
        {            
            sOrderInfo = *it;

            ZeroMemory(&tInputOrder, sizeof(tInputOrder));
            MakeReq_Order(tInputOrder, (string)sOrderInfo.InstrumentID, CDataInfo::NewInstance()->m_szAccount, OrderType_FT_Immediately, 
                          sOrderInfo.Direction, sOrderInfo.CombOffsetFlag[0], sOrderInfo.VolumeTotalOriginal, sOrderInfo.LimitPrice, 0, 
                          FALSE, FALSE, FALSE);

            pAPI->Send(CMDID_ORDER_INSERT_ORDER, &tInputOrder,sizeof(tInputOrder), 0);
        }
    }
}

BOOL COptionStrategyDlg::MakeReq_Order(PlatformStru_InputOrder& req, string& strInstrumentID, string& strAccount, int nOrderType, 
                                       char cDirection, char cOffsetFlag, int nVolume, double fPrice, double fTriggerPrice, 
                                       BOOL bIsHedge,BOOL bIsMarketPrice, BOOL bIsReplaceOrder)
                                       
{    
    // 设置为外盘改单类型
    if(bIsReplaceOrder)
        req.OrderClass=ORDERCLASS_ReplaceOrder;
    else
        req.OrderClass=ORDERCLASS_NewOrder;

    switch(nOrderType) {
    case OrderType_FT_Touch:
        req.ContingentCondition = THOST_FTDC_CC_Touch;				// 止损
        req.StopPrice = fTriggerPrice;								// 同时写止损触发价
        break;
    case OrderType_FT_TouchProfit:
        req.ContingentCondition = THOST_FTDC_CC_TouchProfit;		// 止赢
        req.StopPrice = fTriggerPrice;								// 同时写止盈触发价
        break;
    case OrderType_FT_ParkedOrder:
        req.ContingentCondition = THOST_FTDC_CC_ParkedOrder;		// 预埋单
        break;
    case OrderType_FT_LastPriceGreaterThanStopPrice:
        ///最新价大于条件价
        req.ContingentCondition = THOST_FTDC_CC_LastPriceGreaterThanStopPrice;
        req.StopPrice = fTriggerPrice;								// 同时写触发价
        break;
    case OrderType_FT_LastPriceGreaterEqualStopPrice:
        ///最新价大于等于条件价
        req.ContingentCondition = THOST_FTDC_CC_LastPriceGreaterEqualStopPrice;
        req.StopPrice = fTriggerPrice;								// 同时写触发价
        break;
    case OrderType_FT_LastPriceLesserThanStopPrice:
        ///最新价小于条件价
        req.ContingentCondition =  THOST_FTDC_CC_LastPriceLesserThanStopPrice;
        req.StopPrice = fTriggerPrice;								// 同时写触发价
        break;
    case OrderType_FT_LastPriceLesserEqualStopPrice:
        ///最新价小于等于条件价
        req.ContingentCondition = THOST_FTDC_CC_LastPriceLesserEqualStopPrice;
        req.StopPrice = fTriggerPrice;								// 同时写触发价
        break;
    case OrderType_FT_AskPriceGreaterThanStopPrice:
        ///卖一价大于条件价
        req.ContingentCondition = THOST_FTDC_CC_AskPriceGreaterThanStopPrice;
        req.StopPrice = fTriggerPrice;								// 同时写触发价
        break;
    case OrderType_FT_AskPriceGreaterEqualStopPrice:
        ///卖一价大于等于条件价
        req.ContingentCondition = THOST_FTDC_CC_AskPriceGreaterEqualStopPrice;
        req.StopPrice = fTriggerPrice;								// 同时写触发价
        break;
    case OrderType_FT_AskPriceLesserThanStopPrice:
        ///卖一价小于条件价
        req.ContingentCondition = THOST_FTDC_CC_AskPriceLesserThanStopPrice;
        req.StopPrice = fTriggerPrice;								// 同时写触发价
        break;
    case OrderType_FT_AskPriceLesserEqualStopPrice:
        ///卖一价小于等于条件价
        req.ContingentCondition = THOST_FTDC_CC_AskPriceLesserEqualStopPrice;
        req.StopPrice = fTriggerPrice;								// 同时写触发价
        break;
    case OrderType_FT_BidPriceGreaterThanStopPrice:
        ///买一价大于条件价
        req.ContingentCondition = THOST_FTDC_CC_BidPriceGreaterThanStopPrice;
        req.StopPrice = fTriggerPrice;								// 同时写触发价
        break;
    case OrderType_FT_BidPriceGreaterEqualStopPrice:
        ///买一价大于等于条件价
        req.ContingentCondition = THOST_FTDC_CC_BidPriceGreaterEqualStopPrice;
        req.StopPrice = fTriggerPrice;								// 同时写触发价
        break;
    case OrderType_FT_BidPriceLesserThanStopPrice:
        ///买一价小于条件价
        req.ContingentCondition = THOST_FTDC_CC_BidPriceLesserThanStopPrice;
        req.StopPrice = fTriggerPrice;								// 同时写触发价
        break;
    case OrderType_FT_BidPriceLesserEqualStopPrice:
        ///买一价小于等于条件价
        req.ContingentCondition = THOST_FTDC_CC_BidPriceLesserEqualStopPrice;
        req.StopPrice = fTriggerPrice;								// 同时写触发价
        break;
    case OrderType_FT_Immediately:
    default:
        req.ContingentCondition = THOST_FTDC_CC_Immediately;		// 立即模式
        break;
    };

    // 交易帐号
    strncpy_s(req.strAccount, strAccount.c_str(), 
        sizeof(req.strAccount)-1 < strAccount.length() ? 
        sizeof(req.strAccount)-1 : strAccount.length());

    //合约代码
    strncpy_s(req.InstrumentID, strInstrumentID.c_str(), 
        sizeof(req.InstrumentID)-1 < strInstrumentID.length() ? 
        sizeof(req.InstrumentID)-1 : strInstrumentID.length());

    //买卖方向
    req.Direction = cDirection;// ? THOST_FTDC_D_Buy : THOST_FTDC_D_Sell);

    if(bIsMarketPrice) {
        //价格类型
        req.OrderPriceType=THOST_FTDC_OPT_AnyPrice;
        //价格
        req.LimitPrice=0;
        //有效期类型
        req.TimeCondition=THOST_FTDC_TC_IOC;
    }
    else {
        //价格类型
        req.OrderPriceType=THOST_FTDC_OPT_LimitPrice;
        //限价价格
        req.LimitPrice=fPrice;
        //有效期类型
        req.TimeCondition=THOST_FTDC_TC_GFD;
    }

    //开平标志
    req.CombOffsetFlag[0]=cOffsetFlag;

    //组合投机套保标志，只有第一个字节有效
    if(!bIsHedge)
        req.CombHedgeFlag[0]=THOST_FTDC_HF_Speculation;	// 投机
    else
        req.CombHedgeFlag[0]=THOST_FTDC_HF_Hedge;		// 保值

    //数量
    req.VolumeTotalOriginal=nVolume;
    //成交量类型
    req.VolumeCondition = THOST_FTDC_VC_AV; 
    //最小成交量
    req.MinVolume=1;

    req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;

    return TRUE;
}
//处理成交回报。这里只做简单处理：只要合约名称/方向/开平/投保一致，就累计已成交数量。
void COptionStrategyDlg::TradeInfoReach(PlatformStru_TradeInfo& TradeInfo)
{
    m_QuickOrderModuleDialog.UpdatePositionVolume(TradeInfo);    
}

//处理报单回报。
void COptionStrategyDlg::OnOrderRtn(PlatformStru_OrderInfo& OrderInfo)
{
    //if(OrderInfo.InstrumentID[sizeof(OrderInfo.InstrumentID)-1]!=0)
    //    OrderInfo.InstrumentID[sizeof(OrderInfo.InstrumentID)-1]=0;
    //if(string(OrderInfo.InstrumentID)!=m_algo.strInstrumentName)
    //    return;
    //char Direction=m_algo.direction==0?'0':'1';
    //if(OrderInfo.Direction!=Direction)
    //    return;
    //if(m_algo.OffsetID==0&&OrderInfo.CombOffsetFlag[0]!='0'||
    //    m_algo.OffsetID!=0&&OrderInfo.CombOffsetFlag[0]=='0') 
    //    return;
    //char HedgeFlag='1';
    //if(OrderInfo.CombHedgeFlag[0]!=HedgeFlag)
    //    return;

    ////如果状态为已经撤单，对剩余数量重新下单
    //if(OrderInfo.OrderStatus=='5'&&
    //    OrderInfo.VolumeTotal>0)
    //{
    //    SendInsertOrder(OrderInfo.VolumeTotal);
    //}
}
void COptionStrategyDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: Add your message handler code here and/or call default
    if(nIDEvent==1)        
    {
        //更新LineChart
        ::SendMessage(m_AnalyseModuleDialog.GetSafeHwnd(),WM_USER_QUOT_UPDATE,0,0);
    }
    else if(nIDEvent==2)//static note update
    {
        m_strNote = CDataInfo::NewInstance()->GetNoteText();
        GetDlgItem(IDC_STATIC_NOTE)->SetWindowText(m_strNote);
    }
    else if(nIDEvent==3)
    {
        if(CDataInfo::NewInstance()->GetExchangeFlag() &&
            CDataInfo::NewInstance()->GetProductFlag() &&
            CDataInfo::NewInstance()->GetInstrumentFlag())
        {     
            KillTimer(3);

            map<string,set<string>> mapExchangeID2ProductID;
            map<string,set<string>> mapProductID2InstID;

            CDataInfo::NewInstance()->GetExchangeID2ProductID(mapExchangeID2ProductID);
            CDataInfo::NewInstance()->GetProductID2InstID(mapProductID2InstID);

            string sProductName = CDataInfo::NewInstance()->GetProductByInstID(m_szCurrInstrumentID);
            string sExchangeName = CDataInfo::NewInstance()->GetExchangeByProductID(sProductName);


            set<string> setProduct;
            map<string,set<string>>::iterator it = mapExchangeID2ProductID.find(sExchangeName);
            if(it!=mapExchangeID2ProductID.end())
            {
                setProduct = it->second;
                set<string>::iterator it_ = setProduct.begin();
                while(it_!=setProduct.end())
                {
                    m_combProduct.AddString((*it_).c_str());
                    ++it_;
                }
            }

            set<string> setInst;
            map<string,set<string>>::iterator it2 = mapProductID2InstID.find(sProductName);
            if(it2!=mapProductID2InstID.end())
            {
                setInst = it2->second;
                set<string>::iterator it_ = setInst.begin();
                while(it_!=setInst.end())
                {
                    m_combInstrument.AddString((*it_).c_str());
                    ++it_;
                }
            }

            int nIndex = 0; 
            nIndex = m_pMainDlg->m_combExchange.FindString(-1,(LPCTSTR)sExchangeName.c_str());
            m_pMainDlg->m_combExchange.SetCurSel(nIndex);

            nIndex = m_pMainDlg->m_combProduct.FindString(-1,(LPCTSTR)sProductName.c_str());
            m_pMainDlg->m_combProduct.SetCurSel(nIndex);

            nIndex = m_pMainDlg->m_combInstrument.FindString(-1,(LPCTSTR)m_szCurrInstrumentID.c_str());
            m_pMainDlg->m_combInstrument.SetCurSel(nIndex);

            m_pMainDlg->DisplayWindow(true);  

            OnCbnSelchangeComboInstrument();
        }
    }

    CBKDialog::OnTimer(nIDEvent);
}

void COptionStrategyDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
    CBKDialog::OnWindowPosChanging(lpwndpos);

    // TODO: Add your message handler code here
    if ( !m_bVisible )  
        lpwndpos->flags &= ~SWP_SHOWWINDOW ;  
}
void COptionStrategyDlg::DisplayWindow( BOOL bShow )  
{  
    if ( bShow )  
    {  
        m_bVisible = TRUE;  
        ShowWindow( SW_SHOWNORMAL );  
    }  
    else  
    {  
        m_bVisible = FALSE;  
        ShowWindow( SW_HIDE );  
    }  
}

void COptionStrategyDlg::OnBnClickedButtonMin()
{
    // TODO: Add your control notification handler code here
    ShowWindow(SW_MINIMIZE);
}

WINDOWPLACEMENT lwndpl;//保存当前窗口大小位置
void COptionStrategyDlg::OnBnClickedButtonMax()
{
    // TODO: Add your control notification handler code here
    //ShowWindow(SW_MAXIMIZE);

    static bool bMax = true;
    m_btnMax.SetBitmaps(bMax?IDB_BITMAP_BTNRESTORE:IDB_BITMAP_BTNMAX, RGB(0, 255, 0), (int)BTNST_AUTO_DARKER);

    if(bMax)
    {
        WINDOWPLACEMENT * lpwndpl;
        lpwndpl=&lwndpl;
        GetWindowPlacement(lpwndpl);

        int cx = GetSystemMetrics(SM_CXFULLSCREEN);
        int cy = GetSystemMetrics(SM_CYFULLSCREEN);
        CRect rt;
        SystemParametersInfo(SPI_GETWORKAREA,0,&rt,0);
        cy = rt.bottom;
        MoveWindow(0, 0, cx, cy); 
    }else
    {
        WINDOWPLACEMENT * lpwndpl;
        lpwndpl=&lwndpl;
        lpwndpl->showCmd=SW_RESTORE;
        SetWindowPlacement(lpwndpl);  
    }
    
    bMax = !bMax;
}
