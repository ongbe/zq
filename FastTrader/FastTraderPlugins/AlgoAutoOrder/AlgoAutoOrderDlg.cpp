// AlgoAutoOrderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AlgoAutoOrder.h"
#include "AlgoAutoOrderDlg.h"
#include "SequenceManager.h"
#include "Module-Misc2/tools_util.h"
#include "MessageBoxDialog.h"



#define QUERYMETIMER			0x7f998876
#define HIDEMETIMER				0x7f998877
#define TIMEOUTTIMER			0x7f998878
#define COLORGRADIENTTIMER		0x7f998879

// CAlgoAutoOrderDlg dialog

IMPLEMENT_DYNAMIC(CAlgoAutoOrderDlg, CDialog)

CAlgoAutoOrderDlg::CAlgoAutoOrderDlg(CWnd* pParent /*=NULL*/)
	: CBookOrderOperations(CAlgoAutoOrderDlg::IDD, pParent)
	, m_bThreadExit(FALSE)
    , m_bTrading(FALSE)
    , m_bOptTrigger(FALSE)
    , m_hThread(NULL)
    , m_strInstrumentName(_T(""))
    , m_SecondCount(0)
    , m_singleOptInstance(NULL)
    , m_nWaitStatusTime(0)
    , m_bCancleOrdering(FALSE)
    , m_bAllOrderStatusUpdata(FALSE)
    , m_bOrderYet(FALSE)
    , m_bStopTrade(FALSE)
    , m_nExceptCount(0)
    , m_nTradeType(0)
    , m_bTradeTime(FALSE)
    , m_bInTradeTimeSpan(FALSE)
{
    ZeroMemory(&m_tPriceGridIndex, sizeof(m_tPriceGridIndex));
    ZeroMemory(&m_tOldPriceGridIndex, sizeof(m_tOldPriceGridIndex));
    ZeroMemory(&m_tAccountInfo, sizeof(m_tAccountInfo));
    ZeroMemory(&m_tInstrumentInfo, sizeof(m_tInstrumentInfo));
    m_nPositionCount = 0;

    ZeroMemory(&m_tDepthMarketData, sizeof(m_tDepthMarketData));

    ZeroMemory(&m_bookOrderDlgColor, sizeof(m_bookOrderDlgColor));
    m_nDlgMaxWidth = 40;
    m_ndlgMaxHeight = 20;
    m_bNeedInstrumentIDInputPanel = true;
    m_bNeedQtyInputPanel = true;
    m_nQueryOrderLastSeqNum = -1;
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CAlgoAutoOrderDlg::~CAlgoAutoOrderDlg()
{
    LockObjectOverTime();    
    if (m_pAlgoAutoData)
    {
        delete m_pAlgoAutoData;
        m_pAlgoAutoData = NULL;
    }
    UnLockObjectOverTime();
}

void CAlgoAutoOrderDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TAB, m_TabCtrl);
}

BEGIN_MESSAGE_MAP(CAlgoAutoOrderDlg, CDialog)
    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CAlgoAutoOrderDlg::OnTcnSelchangeShowtab)
    ON_WM_TIMER()
    ON_WM_CLOSE()
END_MESSAGE_MAP()


// CAlgoAutoOrderDlg message handlers
void CAlgoAutoOrderDlg::ParseCmd(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum)
{
    if(m_bExiting)
        return;
    switch(nCmdId) {
        case CMDID_ACCOUNT_CURRENT_ACCOUNT_RSP:
        case CMDID_ACCOUNT_CURRENT_SUB_ACCOUNT_RSP:
        case CMDID_ACCOUNT_SUB_ACCOUNT_INFO_RSP:
        case CMDID_INSTRUMENT_INFO_RSP:
            ParseCmd_Base(nCmdId, nErrorCode, pData, nDataLen, nSeqNum);
            break;
        case CMDID_ALL_ORDER_LIST_RSP:
        case CMDID_ALL_ORDER_INFO_RSP:
        case CMDID_POSITION_INFO_RSP:
        case CMDID_POSITION_INFO_LIST_RSP:
        case CMDID_QUOTE_SNAPSHOT_RSP:
        case CMDID_QUOTE_SEND_QUOTE:
        case CMDID_ORDER_ORDER_STATUS_CHANGE:
        case CMDID_ORDER_TRADE_RTN:
        case CMDID_ORDER_INSERT_ORDER_RSP:
        case CMDID_ORDER_RSP_ORDER_INSERT1:
        case CMDID_ORDER_RSP_ORDER_INSERT2:
        case CMDID_ORDER_CANCEL_ORDER_RSP:
        case CMDID_ORDER_RSP_ORDER_ACTION1:				
        case CMDID_ORDER_RSP_ORDER_ACTION2:				

        case CMDID_ALL_ORDER_WAIT_ORDER_NUM_RSP:
        case CMDID_ALL_ORDER_WAIT_ORDER_PRICE_RSP:
            ParseCmd_Business(nCmdId, nErrorCode, pData, nDataLen, nSeqNum);
            break;
        case CMDID_CONFIG_CHANGE:
        case CMDID_CONFIG_ENABLE_MOUSE_INPUT_INST_RSP:
        case CMDID_CONFIG_ENABLE_MOUSE_INPUT_VOLUME_RSP:
        case CMDID_CONFIG_INPUT_VOLUMES_RSP:
            ParseCmd_Config(nCmdId, nErrorCode, pData, nDataLen, nSeqNum);
            break;
    };
}

void CAlgoAutoOrderDlg::ParseCmd_Base(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum)
{
    switch(nCmdId) {
        case CMDID_ACCOUNT_CURRENT_ACCOUNT_RSP:
            {
                if(nErrorCode == SUCCESS && nDataLen>0 && pData!=NULL) 
                {
                    char strUserID[64];
                    ZeroMemory(strUserID, sizeof(strUserID));
                    strncpy_s(strUserID, (char*)pData, nDataLen>63 ? 63 : nDataLen);
                    m_strCurrAccount = strUserID;

                    if(m_wndStatusBar.GetSafeHwnd()!=NULL)
                    {
                        char buf[256];
                        memset(buf,0,sizeof(buf));
                        sprintf_s(buf,sizeof(buf)-1,"登录交易终端成功，交易账号为:%s",m_strCurrAccount);
                        m_wndStatusBar.SetText(buf,0,0);
                    }

                }
                else 
                {
                    //AfxMessageBox("一键下单功能初始化失败，无法获得当前用户名", MB_ICONERROR|MB_OK);
                }
            }
            break;
        case CMDID_ACCOUNT_CURRENT_SUB_ACCOUNT_RSP:
            {/*
                if(nErrorCode == SUCCESS && nDataLen>0 && pData!=NULL)
                {
                    char strSubAccount[64];
                    ZeroMemory(strSubAccount, sizeof(strSubAccount));
                    strncpy_s(strSubAccount, (char*)pData, nDataLen>63 ? 63 : nDataLen);
                    m_strCurrSubAccount = strSubAccount;
                    QueryCurrAccountInfo(CString(strSubAccount));
                }
                else 
                {
                    //AfxMessageBox("一键下单功能初始化失败，无法获得当前子帐号", MB_ICONERROR|MB_OK);
                }*/
            }
            break;
        case CMDID_ACCOUNT_SUB_ACCOUNT_INFO_RSP:
            {/*
                if(nErrorCode==SUCCESS && pData!=NULL && nDataLen==sizeof(PlatformStru_TradingAccountInfo))
                {
                    memcpy(&m_tAccountInfo, pData, sizeof(PlatformStru_TradingAccountInfo));
                    ShowCurrAccountInfo();
                }
                else 
                {
                    //AfxMessageBox("一键下单功能初始化失败，无法获得当前帐号信息", MB_ICONERROR|MB_OK);
                }*/
            }
            break;
        case CMDID_INSTRUMENT_INFO_RSP:
            {
                if(nErrorCode==SUCCESS && pData!=NULL && nDataLen==sizeof(PlatformStru_InstrumentInfo))
                {
                    PlatformStru_InstrumentInfo* pInfo = (PlatformStru_InstrumentInfo*)pData;
                    char strInstrmentID[sizeof(InstrumentIDType)+4];
                    ZeroMemory(strInstrmentID, sizeof(strInstrmentID));
                    strncpy_s(strInstrmentID, pInfo->InstrumentID, sizeof(InstrumentIDType));
                    if(GetInstrumentID().CompareNoCase(strInstrmentID)==0) {
                        memcpy(&m_tInstrumentInfo, pData, sizeof(PlatformStru_InstrumentInfo));
                        QueryPositionInfo(CString(strInstrmentID));
                    }
                }
                else 
                {
                    //AfxMessageBox("一键下单功能初始化失败，无法指定合约信息", MB_ICONERROR|MB_OK);
                }
            }
            break;
        default:
            break;
    }
}

void CAlgoAutoOrderDlg::ParseCmd_Business(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum)
{
    CMessageBoxDialog dlg(this);
    vector<ORDERRSP> ::iterator iter;
    switch(nCmdId) {
        case CMDID_ORDER_INSERT_ORDER_RSP://下单请求响应
            if(nErrorCode == SUCCESS && pData!=NULL && nDataLen%sizeof(PlatformStru_InputOrder)==0 && nDataLen>0 && m_pAlgoAutoData!=NULL)
            {                
                PlatformStru_InputOrder *pInputOrder = (PlatformStru_InputOrder*)pData;
                ORDERRSP orderRsp;                
                orderRsp.orderRef = pInputOrder->OrderRef;
                orderRsp.price = pInputOrder->LimitPrice;
                orderRsp.count = pInputOrder->VolumeTotalOriginal;
                orderRsp.realCount = 0;
                orderRsp.bRspArrive = true;

                //LockObjectOverTime();
                SYSTEMTIME sys;
                GetLocalTime(&sys);  
                TRACE("下单回应 %d:%d:%d.%d  OrderRef=%s\n",sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds,pInputOrder->OrderRef);
                m_pAlgoAutoData->vecOrderRSP.push_back(orderRsp);
                //UnLockObjectOverTime(); 
            }
            break;
        case  CMDID_ORDER_RSP_ORDER_INSERT1:
        case  CMDID_ORDER_RSP_ORDER_INSERT2: 
            if(!m_bThreadExit)
            {
                StopTrading();
                dlg.m_Text = "下单错误";
                dlg.DoModal();
            } 
            break;
        case CMDID_ORDER_CANCEL_ORDER_RSP://撤单请求响应
            if(nErrorCode == SUCCESS && pData!=NULL && nDataLen%sizeof(PlatformStru_InputOrderAction)==0 && nDataLen>0 && m_pAlgoAutoData!=NULL)
            {  
                PlatformStru_InputOrderAction* pInputOrderAction = (PlatformStru_InputOrderAction*)pData;                 

                SYSTEMTIME sys;
                GetLocalTime(&sys);
                TRACE("撤单回应 %d:%d:%d.%d  OrderRef=%d\n",sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds,atoi(pInputOrderAction->Thost.OrderRef));
            }
            break;           
        case CMDID_ORDER_RSP_ORDER_ACTION1:		//撤单失败返回
            if(nErrorCode == SUCCESS && pData!=NULL && nDataLen%sizeof(PlatformStru_RspOrderAction1)==0 && nDataLen>0 && m_pAlgoAutoData!=NULL && m_bInTradeTimeSpan)
            {  
                Sleep(800);
                LockObjectOverTime();                
                PlatformStru_InputOrderAction Action;
                PlatformStru_RspOrderAction1* pOrderAction1 = (PlatformStru_RspOrderAction1*)pData;
                memcpy(&(Action.Thost),&(pOrderAction1->InputOrderActionField),sizeof(CCifcoInputOrderActionField));

                vector<ORDERRSP>::iterator iterdRsp = m_pAlgoAutoData->vecOrderRSP.begin();
                for (;iterdRsp!=m_pAlgoAutoData->vecOrderRSP.end();iterdRsp++)
                {
                    if (strcmp(iterdRsp->orderRef.c_str(),pOrderAction1->InputOrderActionField.OrderRef) == 0 &&
                        (iterdRsp->status.orderInfo.OrderStatus==THOST_FTDC_OST_PartTradedQueueing || iterdRsp->status.orderInfo.OrderStatus==THOST_FTDC_OST_NoTradeQueueing))////部分成交还在队列中.未成交还在队列中
                    {
                        TRACE("撤单1失败！\n");
                        CancelOrder(Action);    
                    }
                }
                UnLockObjectOverTime();
            }
            break;
        case CMDID_ORDER_RSP_ORDER_ACTION2:		//撤单失败返回
            if(nErrorCode == SUCCESS && pData!=NULL && nDataLen%sizeof(PlatformStru_RspOrderAction2)==0 && nDataLen>0 && m_pAlgoAutoData!=NULL && m_bInTradeTimeSpan)
            {  
                Sleep(800);
                LockObjectOverTime();                
                PlatformStru_InputOrderAction Action;
                PlatformStru_RspOrderAction2* pOrderAction2 = (PlatformStru_RspOrderAction2*)pData;                
                memcpy(&(Action.Thost),&(pOrderAction2->OrderActionField),sizeof(CCifcoInputOrderActionField));
                vector<ORDERRSP>::iterator iterdRsp = m_pAlgoAutoData->vecOrderRSP.begin();
                for (;iterdRsp!=m_pAlgoAutoData->vecOrderRSP.end();iterdRsp++)
                {
                    if (strcmp(iterdRsp->orderRef.c_str(),pOrderAction2->OrderActionField.OrderRef) == 0 &&
                        (iterdRsp->status.orderInfo.OrderStatus==THOST_FTDC_OST_PartTradedQueueing || iterdRsp->status.orderInfo.OrderStatus==THOST_FTDC_OST_NoTradeQueueing))////部分成交还在队列中.未成交还在队列中
                    {
                        TRACE("撤单2失败！\n");
                        CancelOrder(Action);    
                    }
                }             
                UnLockObjectOverTime();
            }
            break;             
        case CMDID_ALL_ORDER_LIST_RSP:
            break;
        case CMDID_ALL_ORDER_INFO_RSP: 
            break;
        case CMDID_POSITION_INFO_LIST_RSP:
            {
                if(nErrorCode == SUCCESS && nDataLen%sizeof(PlatformStru_Position)==0 && m_pAlgoAutoData!=NULL) 
                {
                    m_nPositionCount = 0;
                    for(int i=0; i<conMaxPositionCount; i++)
                    {
                        //m_tPositions[i].Clear();
                    }
                    
                    int nCount = nDataLen/sizeof(PlatformStru_Position);
                    PlatformStru_Position Position;
                    for(int i=0; i<nCount && i<conMaxPositionCount; i++)
                    {
                        memcpy(&Position, (PlatformStru_Position*)pData+i,sizeof(PlatformStru_Position));
                        if (Position.PosiDirection == THOST_FTDC_PD_Long)//买持仓
                        {
                            memcpy(&m_pAlgoAutoData->m_tBuyPosition, (PlatformStru_Position*)&Position,sizeof(PlatformStru_Position));
                        }else if(Position.PosiDirection == THOST_FTDC_PD_Short)//卖持仓
                        {
                            memcpy(&m_pAlgoAutoData->m_tSellPosition, (PlatformStru_Position*)&Position,sizeof(PlatformStru_Position));
                        }                        
                    }
                    m_nPositionCount = nCount>conMaxPositionCount? conMaxPositionCount : nCount;

                    if(m_PageGeneralParam.m_bOkClicked)
                        StartTrading(); 
                    //ShowPositionInfos();
                }
                else 
                {
                    //AfxMessageBox("无法获得持仓信息", MB_ICONERROR|MB_OK);
                }                
            }
            break;
        case CMDID_QUOTE_SNAPSHOT_RSP://返回行情快照
            {
                if(nErrorCode == SUCCESS && nDataLen%sizeof(PlatformStru_DepthMarketData)==0)
                {
                    if(nDataLen>0)
                    {
                        memcpy(&m_tDepthMarketData, pData, sizeof(PlatformStru_DepthMarketData));
                        m_tPriceGridIndex.bHasData = TRUE;
						if (m_PageGeneralParam.GetSafeHwnd()!=NULL)
						{
							m_PageGeneralParam.UpdateDepthMarketContent(m_tDepthMarketData);
						}                        
                    }
                }
            }
            break;
        case CMDID_QUOTE_SEND_QUOTE://推送合约行情
            {
                if(nErrorCode == SUCCESS && pData!=NULL && nDataLen%sizeof(PlatformStru_DepthMarketData)==0 && nDataLen>0)
                {
                    memcpy(&m_tDepthMarketData, pData, sizeof(PlatformStru_DepthMarketData));
                    if (m_PageGeneralParam.GetSafeHwnd()!=NULL)
                    {                       
                        m_PageGeneralParam.UpdateDepthMarketContent(m_tDepthMarketData);
                        if(m_PageGeneralParam.m_bOkClicked)
                            m_PageFeedback.UpdateDepthMarketContent();
                    }
                }
            }
            break;
        case CMDID_ORDER_ORDER_STATUS_CHANGE://报单状态回报
            if(nErrorCode == SUCCESS && pData!=NULL && nDataLen%sizeof(PlatformStru_OrderInfo)==0 && nDataLen>0 && m_pAlgoAutoData!=NULL)
            {                
                PlatformStru_OrderInfo* pOrderData = (PlatformStru_OrderInfo*)pData;

                string strSubTemp = "";
                string strStatus = "";
                if(pOrderData->OrderSubmitStatus == THOST_FTDC_OSS_InsertSubmitted)
                {
                    strSubTemp = "已经提交";
                }else if (pOrderData->OrderSubmitStatus == THOST_FTDC_OSS_CancelSubmitted)
                {
                    strSubTemp = "撤单已经提交";
                }else if (pOrderData->OrderSubmitStatus == THOST_FTDC_OSS_ModifySubmitted)
                {
                    strSubTemp = "修改已经提交";
                }else if (pOrderData->OrderSubmitStatus == THOST_FTDC_OSS_Accepted)
                {
                    strSubTemp = "已经接受";
                }else if (pOrderData->OrderSubmitStatus == THOST_FTDC_OSS_InsertRejected)
                {
                    strSubTemp = "报单已经被拒绝";
                }else if (pOrderData->OrderSubmitStatus == THOST_FTDC_OSS_CancelRejected)
                {
                    strSubTemp = "撤单已经被拒绝";
                }else if (pOrderData->OrderSubmitStatus == THOST_FTDC_OSS_ModifyRejected)
                {
                    strSubTemp = "改单已经被拒绝";
                }

                if(pOrderData->OrderStatus == THOST_FTDC_OST_AllTraded)
                {
                    strStatus = "全部成交";
                }else if (pOrderData->OrderStatus == THOST_FTDC_OST_PartTradedQueueing)
                {
                    strStatus = "部分成交还在队列中";
                }else if (pOrderData->OrderStatus == THOST_FTDC_OST_PartTradedNotQueueing)
                {
                    strStatus = "部分成交不在队列中";
                }else if (pOrderData->OrderStatus == THOST_FTDC_OST_NoTradeQueueing)
                {
                    strStatus = "未成交还在队列中";
                }else if (pOrderData->OrderStatus == THOST_FTDC_OST_NoTradeNotQueueing)
                {
                    strStatus = "未成交不在队列中";
                }else if (pOrderData->OrderStatus == THOST_FTDC_OST_Canceled)
                {
                    strStatus = "撤单";
                }else if (pOrderData->OrderStatus == THOST_FTDC_OST_Unknown)
                {
                    strStatus = "未知";
                }else if (pOrderData->OrderStatus == THOST_FTDC_OST_NotTouched)
                {
                    strStatus = "尚未触发";
                }else if (pOrderData->OrderStatus == THOST_FTDC_OST_Touched)
                {
                    strStatus = "已触发";
                }

                SYSTEMTIME sys;
                GetLocalTime(&sys);  

                TRACE("%d:%d:%d.%d OrderRef = %s,OrderSubmitStatus : %s,  OrderStatus = %s\n",sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds,pOrderData->OrderRef,strSubTemp.c_str(),strStatus.c_str());

                if (pOrderData->OrderStatus==THOST_FTDC_OST_Canceled)//已撤单
                {           
                    if(pOrderData->OrderSubmitStatus == THOST_FTDC_OSS_InsertRejected)
                    {
                        //下单错误(下单被拒绝，状态被撤单)
                        m_nExceptCount++;
                        if (m_pAlgoAutoData->m_nOrderCount == m_nExceptCount && m_bInTradeTimeSpan)
                        {
                            if(m_pAlgoAutoData->bIdleTime)
                                KillTimer(COM_IDLETIMER);
                            if(m_pAlgoAutoData->bExecuteTime)
                                KillTimer(COM_EXECUTETIMER);

                            TRACE("下单错误(下单被拒绝，状态被撤单),重下\n");
                            m_nExceptCount = 0;
                            m_pAlgoAutoData->TradingTime--;
                            m_pAlgoAutoData->vecTradedVolumePerTime.pop_back();
//                             m_pAlgoAutoData->bPatchOrder=false;
                            EndTrading();
                        }
                        break;
                    }else
                    {
                        BOOL bCancleFinish = TRUE;
                        BOOL bCountFinish = TRUE;
                        vector<ORDERRSP>::iterator iterdRsp = m_pAlgoAutoData->vecOrderRSP.begin();
                        for (;iterdRsp!=m_pAlgoAutoData->vecOrderRSP.end();++iterdRsp)
                        {
                            if (iterdRsp->cancle)//主动撤单
                            {                           
                                if(strcmp(iterdRsp->orderRef.c_str(),pOrderData->OrderRef) == 0)
                                {
                                    SYSTEMTIME sys;
                                    GetLocalTime(&sys);  
                                    TRACE("主动撤单 %d:%d:%d.%d  OrderRef=%s\n",sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds,pOrderData->OrderRef);

                                    iterdRsp->cancle = false;   
                                    iterdRsp->finish = true;
                                    iterdRsp->status.orderInfo.OrderStatus = THOST_FTDC_OST_Canceled;//全部撤单
                                    //iterdRsp = m_pAlgoAutoData->vecOrderRSP.erase(iterdRsp);
                                }
                            }

                            if (!iterdRsp->finish)
                                bCountFinish = FALSE;     
                            if(iterdRsp->cancle)
                                bCancleFinish = FALSE;
                        }

                        if (bCancleFinish && bCountFinish)
                        {
                            if (!m_bStopTrade)
                            {                           
                                TRACE("主动撤单全部完成，撤单成功，重新下单\n");
//                                 m_pAlgoAutoData->bPatchOrder=false;
                                EndTrading();
                            }
                            else
                                TRACE("主动撤单全部完成，撤单成功，停止交易\n");
                            break;
                        }
                    }
                }else
                {
                    //更新
                    BOOL bFlag = FALSE;
                    vector<ORDERRSP>::iterator iterdRsp = m_pAlgoAutoData->vecOrderRSP.begin();
                    for (;iterdRsp!=m_pAlgoAutoData->vecOrderRSP.end();iterdRsp++)
                    {
                        if (strcmp(iterdRsp->orderRef.c_str(),pOrderData->OrderRef) == 0)//更新状态
                        {
                            if(pOrderData->OrderStatus!=THOST_FTDC_OST_Unknown) 
                            {
                                SYSTEMTIME sys;
                                GetLocalTime(&sys);
                                TRACE("更新状态 %d:%d:%d.%d orderRef=%s\n",sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds,pOrderData->OrderRef);

                                iterdRsp->bStatus = TRUE;
                                iterdRsp->status.orderInfo = (*pOrderData);
                                iterdRsp->status.time = time(NULL);
                            }
                        }else
                        {
                            if(!iterdRsp->bStatus)
                                bFlag = TRUE;
                        }
                    }

                    if (m_pAlgoAutoData->m_nOrderCount == m_pAlgoAutoData->vecOrderRSP.size())
                    {
                        if (bFlag)
                            m_bAllOrderStatusUpdata = FALSE; 
                        else
                            m_bAllOrderStatusUpdata = TRUE; 
                    } 
                }
            }
            break;
        case CMDID_ORDER_TRADE_RTN://成交回报
            if(nErrorCode == SUCCESS && pData!=NULL && nDataLen%sizeof(PlatformStru_TradeInfo)==0 && nDataLen>0 && m_pAlgoAutoData!=NULL)
            {
                LockObjectOverTime();
                if(m_pAlgoAutoData->vecOrderRSP.size()==0)
                {
                    UnLockObjectOverTime();
                    break;
                }

                BOOL bCountFinish = TRUE;
                BOOL bCancleFinish = TRUE;
                PlatformStru_TradeInfo *pTrade = (PlatformStru_TradeInfo*)pData;
                vector<ORDERRSP>::iterator iterdRsp = m_pAlgoAutoData->vecOrderRSP.begin();
                for (;iterdRsp!=m_pAlgoAutoData->vecOrderRSP.end();++iterdRsp)
                {
                    if (iterdRsp->orderRef == pTrade->OrderRef)
                    {
                        iterdRsp->realCount += (pTrade->Volume);
                        if(iterdRsp->realCount == iterdRsp->count)
                        {
                            iterdRsp->finish = TRUE;
                            iterdRsp->cancle = FALSE;
                            iterdRsp->status.orderInfo.OrderStatus = THOST_FTDC_OST_AllTraded;//全部成交
                            //iterdRsp = m_pAlgoAutoData->vecOrderRSP.erase(iterdRsp);
                        }
                            

						m_pAlgoAutoData->AverageQuan += (pTrade->Price)*(pTrade->Volume);   

                        SYSTEMTIME sys;
                        GetLocalTime(&sys);
                        TRACE("成交回报 %d:%d:%d.%d orderRef=%s\n",sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds,pTrade->OrderRef);
                       
                        time_t curTime = time(NULL);
                        double fTime = (curTime - m_StartTradingUTC)/60.0f;
                        CheckSumPrice(fTime,1.0f,1);

                        m_pAlgoAutoData->vecTradedVolumePerMinute[(int)(fTime-1)] += pTrade->Volume;
                        m_pAlgoAutoData->vecTradedVolumePerTime[m_pAlgoAutoData->TradingTime-1] += pTrade->Volume;				

                        m_pAlgoAutoData->TotalVolume += pTrade->Volume;//所有下单成交的量
                        m_pAlgoAutoData->Volume += pTrade->Volume;//本次下单成交的量   
                        m_PageFeedback.UpdataFeedbackData(TRUE);
                        m_PageFeedback.UpdataHistogram();
                    }

                    if (!iterdRsp->finish)
                        bCountFinish = FALSE;
                    if(iterdRsp->cancle)
                        bCancleFinish = FALSE;
                }      

                if (bCountFinish && bCancleFinish && m_pAlgoAutoData->m_nOrderCount == m_pAlgoAutoData->vecOrderRSP.size())
                {
                    TRACE("成交回报后，所有报单成交成功,重新下单\n");
//                     m_pAlgoAutoData->bPatchOrder=false;
                    if(atoi(m_pAlgoAutoData->InstrumentCount.c_str()) == m_pAlgoAutoData->TotalVolume)
                    {
                        UnLockObjectOverTime();   
                        StopTrading();
                        CMessageBoxDialog dlg(this);
                        dlg.m_Text = "所有下单已全部成交，达到下单总数量，交易结束,交易成功！";
                        dlg.DoModal();
                        break;
                    }else 
                    {
                        UnLockObjectOverTime();         
                        EndTrading();
                        break;
                    }

                }
//                 else if(!m_bCancleOrdering && 
//                          m_pAlgoAutoData->Volume >= m_pAlgoAutoData->ResetCountValue && 
//                          !m_pAlgoAutoData->ResetCount.value.empty() &&
//                          m_pAlgoAutoData->ResetCountValue != 0)
//                 {
//                     TRACE("本次下单报单的成交量达到重置数量，补充下单量，重新下单\n"); 
//                     m_pAlgoAutoData->WaitVolume=GetWaitOrderCount();
//                     m_pAlgoAutoData->bPatchOrder=true;
//                     m_bThreadExit=false;
//                 }
// 
                UnLockObjectOverTime();
			}
			break;
		case CMDID_ALL_ORDER_WAIT_ORDER_NUM_RSP:
			break;
        case CMDID_ALL_ORDER_WAIT_ORDER_PRICE_RSP:
            break;
        default:
            break;
    };    
}

void CAlgoAutoOrderDlg::ParseCmd_Config(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum)
{
    switch(nCmdId) {
        case CMDID_CONFIG_CHANGE:
            {
                if(nErrorCode == SUCCESS) 
                {
//                     QueryConfig_InstrumentIDInputPanelShow();
//                     QueryConfig_QtyInputPanelShow();
//                     QueryConfig_QtyInputPanelQtyDatas();
                }
                else 
                {
                    //AfxMessageBox("无法获得持仓信息", MB_ICONERROR|MB_OK);
                }
            }
            break;
        case CMDID_CONFIG_ENABLE_MOUSE_INPUT_INST_RSP:
            {
                if(nErrorCode == SUCCESS && nDataLen==sizeof(bool) && pData!=NULL)
                {
                    memcpy(&m_bNeedInstrumentIDInputPanel, pData, sizeof(bool));
                }
                else 
                {
                    //AfxMessageBox("无法获得行情快照", MB_ICONERROR|MB_OK);
                }
            }
            break;
        case CMDID_CONFIG_ENABLE_MOUSE_INPUT_VOLUME_RSP:
            {
                if(nErrorCode == SUCCESS && nDataLen==sizeof(bool) && pData!=NULL)
                {
                    memcpy(&m_bNeedQtyInputPanel, pData, sizeof(bool));
                }
                else 
                {
                    //AfxMessageBox("无法获得行情快照", MB_ICONERROR|MB_OK);
                }
            }
            break;
        case CMDID_CONFIG_INPUT_VOLUMES_RSP:
            {
                if(nErrorCode == SUCCESS && pData!=NULL && nDataLen==9*sizeof(int))
                {
   /*                 if(m_pdlgQtyList!=NULL)
                    {
                        m_pdlgQtyList->ResetQtyList((int*)pData, nDataLen/sizeof(int));
                    }*/
                }
            }
            break;
        default:
            break;
    };
}

BOOL CAlgoAutoOrderDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon
    

    if(m_wndStatusBar.Create(WS_CHILD|WS_VISIBLE|CCS_BOTTOM, CRect(0,0,0,0), this, 0))
    {
        int strPartDim[2]={300,-1};
        m_wndStatusBar.SetParts(2,strPartDim);
        m_wndStatusBar.SetText("登录交易终端成功",0,0);
    }

    SetWindowText("盯盘建仓平仓");
    //初始化TAB
    TCITEM tc1,tc2;
    tc1.mask = TCIF_TEXT;
    tc1.pszText = "报单常规参数";
    tc2.mask = TCIF_TEXT;
    tc2.pszText = "下单监控";

    m_TabCtrl.InsertItem(0, &tc1);
    m_TabCtrl.InsertItem(1, &tc2);

    //创建子页面
    m_PageGeneralParam.Create(IDD_GENERALOPTIMIZE_DIALOG, &m_TabCtrl);
    m_PageFeedback.Create(IDD_FEEDBACK_DIALOG, &m_TabCtrl);

    m_PageGeneralParam.m_pAlgoAutoOrderDlg = this;
    m_PageFeedback.m_pAlgoAutoOrderDlg = this;    

    AdjustChildWindowSize();

    m_pAlgoAutoData = new CAlgoAutoData;

    //从ini加载配置
    ReadIni();

    //显示子页面
    m_PageGeneralParam.ShowWindow(SW_SHOW);
    m_PageFeedback.ShowWindow(SW_HIDE);
    
    m_TabCtrl.SetCurSel(0);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CAlgoAutoOrderDlg::AdjustChildWindowSize(void)
{
    RECT ClientRect;
    GetClientRect(&ClientRect);
    int cx=ClientRect.right-ClientRect.left;
    int cy=ClientRect.bottom-ClientRect.top;

    int StatusHei=0;
    if(m_wndStatusBar.GetSafeHwnd()!=NULL)
    {
        RECT Rect;
        m_wndStatusBar.GetWindowRect(&Rect);
        int wid=cx;
        int hei=Rect.bottom-Rect.top;
        StatusHei=hei;
        m_wndStatusBar.MoveWindow(0,cy-hei,wid,hei);
    }

    if(m_TabCtrl.GetSafeHwnd()!=NULL)
    {
        RECT Rect;
        m_TabCtrl.GetWindowRect(&Rect);
        ScreenToClient(&Rect);
        int wid=cx-Rect.left*2;
        int hei=cy-StatusHei;
        m_TabCtrl.MoveWindow(Rect.left,Rect.top,wid,hei);

        //获得TAB控件的坐标
        CRect rect=m_TabCtrl.GetPageRect();
        //将子页面移动到指定的位置
        if(m_PageGeneralParam.GetSafeHwnd())
            m_PageGeneralParam.MoveWindow(&rect);
        if(m_PageFeedback.GetSafeHwnd())
            m_PageFeedback.MoveWindow(&rect);
    }
}

void CAlgoAutoOrderDlg::OnTcnSelchangeShowtab(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: Add your control notification handler code here
    switch (m_TabCtrl.GetCurSel())
    {
    case 0:
        m_PageGeneralParam.ShowWindow(SW_SHOW);
        m_PageFeedback.ShowWindow(SW_HIDE);
        break;
    case 1:
        m_PageGeneralParam.ShowWindow(SW_HIDE);
        m_PageFeedback.ShowWindow(SW_SHOW);         
        break;
    default:
        break;
    }
    *pResult = 0;
}

CString CAlgoAutoOrderDlg::GetInstrumentID() 
{
	if(m_PageGeneralParam.GetSafeHwnd()!=NULL)
		return m_PageGeneralParam.GetInstrumentID();
	else
		return NULL;
};
void CAlgoAutoOrderDlg::SetInstrumentID(CString& strInstrumentID) 
{
    m_strInstrumentName = strInstrumentID;
	m_bUserChangeInstrumentID = false;
	m_bUserChangeInstrumentID = true;
	
	QueryInstrumentInfo(strInstrumentID);
	SubscribeQuot(strInstrumentID);
	//QueryQuotSnapshot(strInstrumentID);
    //QueryPositionInfo(strInstrumentID);
	m_strOldInstrumentID = strInstrumentID;

	if(m_PageGeneralParam.GetSafeHwnd()!=NULL)
		m_PageGeneralParam.SetInstrumentID(strInstrumentID);
};
DWORD WINAPI CAlgoAutoOrderDlg::ThreadProc(LPVOID lpParameter)
{
    CAlgoAutoOrderDlg* pAlgoAutoOrderDlg = (CAlgoAutoOrderDlg*)lpParameter;

    if(pAlgoAutoOrderDlg!=NULL)
        return pAlgoAutoOrderDlg->TradeRun();
    else
        return -1;
}
int CAlgoAutoOrderDlg::TradeRun(void)
{  
    //根据合约名称，设置当日交易时段
    CEasyTimespan<int> 		Timespan_IntraDay;

    int nStart = m_pAlgoAutoData->StartTime.GetHour()*3600 + 
                m_pAlgoAutoData->StartTime.GetMinute()*60 + 
                m_pAlgoAutoData->StartTime.GetSecond();

    int nEnd = m_pAlgoAutoData->EndTime.GetHour()*3600 + 
                m_pAlgoAutoData->EndTime.GetMinute()*60 + 
                m_pAlgoAutoData->EndTime.GetSecond();

    Timespan_IntraDay.insert(nStart,nEnd);
    m_pAlgoAutoData->TradingMinute = (int)((nEnd-nStart)/60.0);

    m_pAlgoAutoData->vecTradedVolumePerMinute.clear();
	m_pAlgoAutoData->vecTradedVolumePerTime.clear();    

    for(int j=1;j<=m_pAlgoAutoData->TradingMinute;j++)    
        m_pAlgoAutoData->vecTradedVolumePerMinute.push_back(0);
    

	//根据合约名称，设置每日交易时段
    CEasyTimespan<int> 		Timespan_IntraDay2;

	//正式交易系统
	if(m_pAlgoAutoData->InstrumentName.size()==6&&
		isupper(m_pAlgoAutoData->InstrumentName[0])&&
		isupper(m_pAlgoAutoData->InstrumentName[1])&&
		isdigit(m_pAlgoAutoData->InstrumentName[2]))
	{
		//中金所合约 9:15-11:30 13:00-15:15, total 4.5 hours (2h15'+2h15')
        Timespan_IntraDay2.insert(9*3600+15*60,11*3600+30*60);
        Timespan_IntraDay2.insert(13*3600,15*3600+15*60);
        m_nTradeType = 0;
	}
	else 
	{
		//商品期货 9:00-10:15 10:30-11:30 13:00-15:00, total 4h15'(1h15'+1h+2h)
        Timespan_IntraDay2.insert(9*3600,10*3600+15*60);
        Timespan_IntraDay2.insert(10*3600+30*60,11*3600+30*60);
        Timespan_IntraDay2.insert(13*3600+30*60,15*3600);
        m_nTradeType = 1;
	}

	if(1)
	{
		//模拟交易系统 15:15 - 20:00继续交易
        Timespan_IntraDay2.insert(15*3600+15*60,20*3600);
	}

    CEasyTradingTimespan TradingTimespans(false/*true*/,
                                            Timespan_IntraDay2,
                                            CEasyTimespan<time_t>(),
                                            true,
                                            Stru_TimezoneDst());

    m_PageFeedback.m_customTime.setTradingTimespan(TradingTimespans);
    m_PageFeedback.m_customTime.refreshTimeLineWin();
    m_PageFeedback.m_customCount.setTradingTimespan(TradingTimespans);
    m_PageFeedback.m_customCount.refreshTimeLineWin();

    time(&m_StartTradingUTC);    
    m_PageFeedback.m_customTime.setcontent(m_pAlgoAutoData,m_StartTradingUTC);
    m_PageFeedback.m_customCount.setcontent(m_pAlgoAutoData,m_StartTradingUTC);

   
	OPTPARAM* pOptPARAM = NULL;
	int       nRestOrderCount = 0;
    CString strName;

    SetTimer(WAITCANCLETIMER,200,NULL);
    if(m_pAlgoAutoData->bActiceOpt)
        SetTimer(OPT_BACKTIMER,1000,NULL);

	//业务逻辑算法  
	while(WaitForSingleObject(m_hCloseEvent,0)!=WAIT_OBJECT_0)
	{        
        m_bTrading = TRUE;

        if(m_wndStatusBar.GetSafeHwnd()!=NULL)
            m_wndStatusBar.SetText("交易进行中.....",0,0);


        CString strTip;
		time_t curtime;
		time(&curtime);
		CTime time = CTime(curtime);
        tm* newtime = localtime(&curtime);
        int nTime = (newtime->tm_hour)*3600+(newtime->tm_min)*60+(newtime->tm_sec);

        if(time > m_pAlgoAutoData->EndTime)
        {     
            strTip.Format("交易时间结束，下单已终止，挂单已撤销。");

            if(m_pAlgoAutoData->bIdleTime)
                KillTimer(COM_IDLETIMER);
            if(m_pAlgoAutoData->bExecuteTime)
                KillTimer(COM_EXECUTETIMER);

            m_PageGeneralParam.EnableCtrls(TRUE);            
            ThreadQuitClear(FALSE);

            CMessageBoxDialog dlg(this);
            dlg.m_Text = strTip;
            dlg.DoModal();
            break;
        }

        if(0 == m_nTradeType)//中金所合约 9:15-11:30 13:00-15:15, total 4.5 hours (2h15'+2h15')
        {
            if (nTime>=9*3600+15*60  && nTime<=11*3600+30*60 || 
                nTime>=13*3600       && nTime<=15*3600+15*60 )
            {
                m_bInTradeTimeSpan = TRUE;

                if(m_bTradeTime)
                {
                    if(!CancleWaitOrder())
                        m_bThreadExit = FALSE;
                    m_bTradeTime = FALSE;
                }

                if(!m_bThreadExit)
                {
                    TRACE("<<<<<<<<<<<<<<<<<<进入下单>>>>>>>>>>>>>>\n"); 
                    //
//                     if(!m_pAlgoAutoData->bPatchOrder)
//                     {
                        m_pAlgoAutoData->m_nOrderCount = 0;
                        m_bOrderYet = FALSE;
                        m_bCancleOrdering = FALSE;
                        m_bAllOrderStatusUpdata = FALSE; 
                        m_pAlgoAutoData->OrderCount.value = CTools_AlgoTrading::GetStringFromCEdit(m_PageGeneralParam.m_editOrderCount);
//                         m_pAlgoAutoData->ResetCount.value = CTools_AlgoTrading::GetStringFromCEdit(m_PageGeneralParam.m_editResetOrderCount);
//                     }
 
                    m_pAlgoAutoData->TradingTime++;
                    m_pAlgoAutoData->vecTradedVolumePerTime.push_back(0);             
                    //               

                    if(m_bOptTrigger)//优化的参数条件触发 
                    {                
                        while(!ChangeGeneralParam(m_singleOptInstance))
                        {
                            if (WaitForSingleObject(m_hCloseEvent,0)==WAIT_OBJECT_0)
                            {
                                m_bTrading = FALSE;
                                return 0;           
                            }

                            Sleep(10);
                            continue;                    
                        }

         
                        vector<ORDERINFO>::iterator  iter;
                        for(iter = m_pAlgoAutoData->vecOrderActiveInfo.begin(); iter != m_pAlgoAutoData->vecOrderActiveInfo.end(); iter++)
                        {
                            if(iter->count <= 0)
                                continue;

                            SYSTEMTIME sys;
                            GetLocalTime(&sys);  
                            TRACE("优化下单 %d:%d:%d.%d count%d =%d price = %.2f\n",sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds,iter->index,iter->count,iter->price);

                            m_PageFeedback.UpdataTitleData();
                            m_PageFeedback.UpdataWuDangData(FALSE);

                            strName.Format("%s",(iter->name).c_str());
                            NewOrder(strName,iter->direction,iter->offsetID,iter->price,iter->count);
                        } 
        

                        m_bOptTrigger = FALSE;
                    }else
                    {       
                        while(!m_pAlgoAutoData->AutoOrderCalculate())
                        {
                            if (WaitForSingleObject(m_hCloseEvent,0)==WAIT_OBJECT_0)
                            {
                                m_bTrading = FALSE;
                                return 0;           
                            }

                            Sleep(10);
                            continue;                    
                        }

              
                        vector<ORDERINFO>::iterator  iter;
                        for(iter = m_pAlgoAutoData->vecOrderInfo.begin(); iter != m_pAlgoAutoData->vecOrderInfo.end(); iter++)
                        {
                            if(iter->count <= 0)
                                continue;

                            SYSTEMTIME sys;
                            GetLocalTime(&sys);  //获取当前系统时间
//                             if(m_pAlgoAutoData->bPatchOrder)
//                                 TRACE("常规补单 %d:%d:%d.%d count%d =%d price = %.2f\n",sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds,iter->index,iter->count,iter->price);
//                             else
                                TRACE("常规下单 %d:%d:%d.%d count%d =%d price = %.2f\n",sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds,iter->index,iter->count,iter->price);

                            m_PageFeedback.UpdataTitleData();
                            m_PageFeedback.UpdataWuDangData(TRUE);

                            strName.Format("%s",(iter->name).c_str());
                            NewOrder(strName,iter->direction,iter->offsetID,iter->price,iter->count);
                        } 
           
                    }

                    if(m_pAlgoAutoData->m_nOrderCount != 0)
                        m_bOrderYet = TRUE;

                    if(m_pAlgoAutoData->bIdleTime)
                    {
                        switch(m_pAlgoAutoData->IdleTime.type)
                        {
                        case 0://秒
                            SetTimer(COM_IDLETIMER,atoi(m_pAlgoAutoData->IdleTime.value.c_str())*1000,NULL);
                            m_pAlgoAutoData->IdleTimeValue = atoi(m_pAlgoAutoData->IdleTime.value.c_str());
                            break;
                        case 1:
                            SetTimer(COM_IDLETIMER,atoi(m_pAlgoAutoData->IdleTime.value.c_str())*60*1000,NULL);
                            m_pAlgoAutoData->IdleTimeValue = atoi(m_pAlgoAutoData->IdleTime.value.c_str())*60;
                            break;
                        case 2:
                            SetTimer(COM_IDLETIMER,atoi(m_pAlgoAutoData->IdleTime.value.c_str())*60*60*1000,NULL);
                            m_pAlgoAutoData->IdleTimeValue = atoi(m_pAlgoAutoData->IdleTime.value.c_str())*60*60;
                            break;
                        }
                    }
                    if(m_pAlgoAutoData->bExecuteTime)
                    {
                        switch(m_pAlgoAutoData->IdleTime.type)
                        {
                        case 0:
                            SetTimer(COM_EXECUTETIMER,atoi(m_pAlgoAutoData->IdleTime.value.c_str())*1000,NULL);
                            m_pAlgoAutoData->ExecuteTimeValue = atoi(m_pAlgoAutoData->IdleTime.value.c_str());
                            break;
                        case 1:
                            SetTimer(COM_EXECUTETIMER,atoi(m_pAlgoAutoData->IdleTime.value.c_str())*60*1000,NULL);
                            m_pAlgoAutoData->ExecuteTimeValue = atoi(m_pAlgoAutoData->IdleTime.value.c_str())*60;
                            break;
                        case 2:
                            SetTimer(COM_EXECUTETIMER,atoi(m_pAlgoAutoData->IdleTime.value.c_str())*60*60*1000,NULL);
                            m_pAlgoAutoData->ExecuteTimeValue = atoi(m_pAlgoAutoData->IdleTime.value.c_str())*60*60;
                            break;
                        }
                    }

                    m_bThreadExit=TRUE;            
                    TRACE("<<<<<<<<<<<<<<<<<<下单结束>>>>>>>>>>>>>>>>>>>\n");
                }   
            }else
            {
                m_bInTradeTimeSpan = FALSE;

                if(!m_bTradeTime)
                {
                    if(m_pAlgoAutoData->bIdleTime)
                        KillTimer(COM_IDLETIMER);
                    if(m_pAlgoAutoData->bExecuteTime)
                        KillTimer(COM_EXECUTETIMER);

                    m_bTradeTime = TRUE;
                    m_bThreadExit=TRUE; 
                }

            }
        }else if(1 == m_nTradeType)//商品期货 9:00-10:15 10:30-11:30 13:00-15:00, total 4h15'(1h15'+1h+2h)
        {            
            if (nTime>=9*3600        && nTime<=10*3600+15*60 || 
                nTime>=10*3600+30*60 && nTime<=11*3600+30*60 || 
                nTime>=13*3600       && nTime<=15*3600)
            {
                m_bInTradeTimeSpan = TRUE;

                if(m_bTradeTime)
                {
                    if(!CancleWaitOrder())
                        m_bThreadExit = FALSE;
                    m_bTradeTime = FALSE;
                }

                if(!m_bThreadExit)
                {
                    TRACE("<<<<<<<<<<<<<<<<<<进入下单>>>>>>>>>>>>>>\n"); 
                    //
//                     if(!m_pAlgoAutoData->bPatchOrder)
//                     {
                        m_pAlgoAutoData->m_nOrderCount = 0;
                        m_bOrderYet = FALSE;
                        m_bCancleOrdering = FALSE;
//                         if(!m_pAlgoAutoData->bPatchOrder) m_bAllOrderStatusUpdata = FALSE; 
                        m_bAllOrderStatusUpdata = FALSE;
                        m_pAlgoAutoData->OrderCount.value = CTools_AlgoTrading::GetStringFromCEdit(m_PageGeneralParam.m_editOrderCount);
//                         m_pAlgoAutoData->ResetCount.value = CTools_AlgoTrading::GetStringFromCEdit(m_PageGeneralParam.m_editResetOrderCount);
//                     }
                    m_pAlgoAutoData->TradingTime++;
                    m_pAlgoAutoData->vecTradedVolumePerTime.push_back(0);             
                    //
                    

                    if(m_bOptTrigger)//优化的参数条件触发 
                    {                
                        while(!ChangeGeneralParam(m_singleOptInstance))
                        {                            
                            if (WaitForSingleObject(m_hCloseEvent,0)==WAIT_OBJECT_0)
                            {
                                m_bTrading = FALSE;
                                return 0;           
                            }

                            Sleep(10);
                            continue;                    
                        }

                        
                        vector<ORDERINFO>::iterator  iter;
                        for(iter = m_pAlgoAutoData->vecOrderActiveInfo.begin(); iter != m_pAlgoAutoData->vecOrderActiveInfo.end(); iter++)
                        {
                            if(iter->count <= 0)
                                continue;

                            SYSTEMTIME sys;
                            GetLocalTime(&sys);  
                            TRACE("优化下单 %d:%d:%d.%d count%d =%d price = %.2f\n",sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds,iter->index,iter->count,iter->price);

                            m_PageFeedback.UpdataTitleData();
                            m_PageFeedback.UpdataWuDangData(FALSE);

                            strName.Format("%s",(iter->name).c_str());
                            NewOrder(strName,iter->direction,iter->offsetID,iter->price,iter->count);
                        } 
                        

                        m_bOptTrigger = FALSE;
                    }else
                    {       
                        while(!m_pAlgoAutoData->AutoOrderCalculate())
                        {
                            if (WaitForSingleObject(m_hCloseEvent,0)==WAIT_OBJECT_0)
                            {
                                m_bTrading = FALSE;
                                return 0;           
                            }

                            Sleep(10);
                            continue;                    
                        }

                        
                        vector<ORDERINFO>::iterator  iter;
                        for(iter = m_pAlgoAutoData->vecOrderInfo.begin(); iter != m_pAlgoAutoData->vecOrderInfo.end(); iter++)
                        {
                            if(iter->count <= 0)
                                continue;

                            SYSTEMTIME sys;
                            GetLocalTime(&sys);  //获取当前系统时间
                            TRACE("常规下单 %d:%d:%d.%d count%d =%d price = %.2f\n",sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds,iter->index,iter->count,iter->price);

                            m_PageFeedback.UpdataTitleData();
                            m_PageFeedback.UpdataWuDangData(TRUE);

                            strName.Format("%s",(iter->name).c_str());
                            NewOrder(strName,iter->direction,iter->offsetID,iter->price,iter->count);
                        } 
                        
                    }

                    if(m_pAlgoAutoData->m_nOrderCount != 0)
                        m_bOrderYet = TRUE;

                    if(m_pAlgoAutoData->bIdleTime)
                    {
                        switch(m_pAlgoAutoData->IdleTime.type)
                        {
                        case 0://秒
                            SetTimer(COM_IDLETIMER,atoi(m_pAlgoAutoData->IdleTime.value.c_str())*1000,NULL);
                            m_pAlgoAutoData->IdleTimeValue = atoi(m_pAlgoAutoData->IdleTime.value.c_str());
                            break;
                        case 1:
                            SetTimer(COM_IDLETIMER,atoi(m_pAlgoAutoData->IdleTime.value.c_str())*60*1000,NULL);
                            m_pAlgoAutoData->IdleTimeValue = atoi(m_pAlgoAutoData->IdleTime.value.c_str())*60;
                            break;
                        case 2:
                            SetTimer(COM_IDLETIMER,atoi(m_pAlgoAutoData->IdleTime.value.c_str())*60*60*1000,NULL);
                            m_pAlgoAutoData->IdleTimeValue = atoi(m_pAlgoAutoData->IdleTime.value.c_str())*60*60;
                            break;
                        }
                    }
                    if(m_pAlgoAutoData->bExecuteTime)
                    {
                        switch(m_pAlgoAutoData->IdleTime.type)
                        {
                        case 0:
                            SetTimer(COM_EXECUTETIMER,atoi(m_pAlgoAutoData->IdleTime.value.c_str())*1000,NULL);
                            m_pAlgoAutoData->ExecuteTimeValue = atoi(m_pAlgoAutoData->IdleTime.value.c_str());
                            break;
                        case 1:
                            SetTimer(COM_EXECUTETIMER,atoi(m_pAlgoAutoData->IdleTime.value.c_str())*60*1000,NULL);
                            m_pAlgoAutoData->ExecuteTimeValue = atoi(m_pAlgoAutoData->IdleTime.value.c_str())*60;
                            break;
                        case 2:
                            SetTimer(COM_EXECUTETIMER,atoi(m_pAlgoAutoData->IdleTime.value.c_str())*60*60*1000,NULL);
                            m_pAlgoAutoData->ExecuteTimeValue = atoi(m_pAlgoAutoData->IdleTime.value.c_str())*60*60;
                            break;
                        }
                    }

                    m_bThreadExit=TRUE;            
                    TRACE("<<<<<<<<<<<<<<<<<<下单结束>>>>>>>>>>>>>>>>>>>\n");
                }   
            }else
            {
                m_bInTradeTimeSpan = FALSE;

                if(!m_bTradeTime)
                {                
                    if(m_pAlgoAutoData->bIdleTime)
                        KillTimer(COM_IDLETIMER);
                    if(m_pAlgoAutoData->bExecuteTime)
                        KillTimer(COM_EXECUTETIMER);

                    m_bTradeTime = TRUE;
                    m_bThreadExit=TRUE; 
                 }     
            }
        } 

    	Sleep(500);
    };

    m_bTrading = FALSE;
    return 0;
}
void CAlgoAutoOrderDlg::OnTimer(UINT_PTR nIDEvent)
{   
    CString strTip;
    time_t curtime;
    time(&curtime);
    CTime time = CTime(curtime);
    
    int nShang = 0;
    int nRemainder = 0;
    int nPreSnapShotPriceIndex = 0;
    double fPreSnapShotPrice = 0.0f;
    double fPrice = 0.0f;
    OPTPARAM *pOptPARAM = NULL;   

    if(m_pAlgoAutoData->bActiceTotalTime)
    {
        switch(m_pAlgoAutoData->RetryTotalTime.type)
        {
        case 0://时
            m_pAlgoAutoData->RetryTotalTimeValue = atoi(m_pAlgoAutoData->RetryTotalTime.value.c_str())*60*60;
            break;
        case 1:
            m_pAlgoAutoData->RetryTotalTimeValue = atoi(m_pAlgoAutoData->RetryTotalTime.value.c_str())*60;
            break;
        case 2:
            m_pAlgoAutoData->RetryTotalTimeValue = atoi(m_pAlgoAutoData->RetryTotalTime.value.c_str());
            break;
        }
    }

    SYSTEMTIME sys;
    GetLocalTime(&sys); 

    switch(nIDEvent)
    {
    case WAITCANCLETIMER:
        if (m_bStopTrade && m_bAllOrderStatusUpdata)
        {
            CancleWaitOrder();
            KillTimer(WAITCANCLETIMER);
            m_bStopTrade = FALSE;
            m_bAllOrderStatusUpdata = FALSE;
        } 
        break;
    case OPT_BACKTIMER:
        if (m_pAlgoAutoData->bActiceOpt && m_bOrderYet)//是否启动优化设置
        {
            m_SecondCount++;
            m_mapSnapShot.insert(std::pair<int, double>(m_SecondCount,atof(m_pAlgoAutoData->BasicPrice.value.c_str())));

            pOptPARAM = m_pAlgoAutoData->vecOptInfo.at(m_pAlgoAutoData->OptimizeIndex); 
            if(pOptPARAM != NULL)
            {
                vector<SINGLEOPT*>::iterator iterSingle = pOptPARAM->vecSingle.begin();
                for (;iterSingle!=pOptPARAM->vecSingle.end();iterSingle++)
                {
                    if (m_SecondCount>(*iterSingle)->BackTime)
                    {
                        nShang = m_SecondCount/(*iterSingle)->BackTime;
                        nRemainder = m_SecondCount%(*iterSingle)->BackTime;
                        nPreSnapShotPriceIndex = (*iterSingle)->BackTime*(nShang-1)+nRemainder+1;

                        std::map<int, double>::iterator it = m_mapSnapShot.find(nPreSnapShotPriceIndex);
                        if(it!=m_mapSnapShot.end())
                        {
                            fPreSnapShotPrice = it->second;
                            switch((*iterSingle)->PriceChange.type)
                            {
                            case 0://价格
                                fPrice = (atof(m_pAlgoAutoData->BasicPrice.value.c_str()) - fPreSnapShotPrice);
                                break;
                            case 1://百分比  
                                fPrice = (atof(m_pAlgoAutoData->BasicPrice.value.c_str()) - fPreSnapShotPrice)/fPreSnapShotPrice;  
                                break;
                            }

                            if (fPrice > atof((*iterSingle)->PriceChange.valueFrom.c_str()) && fPrice < atof((*iterSingle)->PriceChange.valueTo.c_str()))
                            {            
                                m_singleOptInstance = *iterSingle;
                                m_bOptTrigger = TRUE;
                            }        
                        }
                    }
                }
            }
        }

        break;
    case COM_IDLETIMER://idleTime arrive
        LockObjectOverTime();
        if (m_pAlgoAutoData->bActiceOverTime)                        
        {   
            m_pAlgoAutoData->RetryCountSave += 1;
            if (m_pAlgoAutoData->RetryCountSave >= atoi(m_pAlgoAutoData->RetryCount.c_str()))
            {
                //已经到达超时重试次数
                KillTimer(COM_IDLETIMER);
                UnLockObjectOverTime();
                CancleWaitOrder();
                StopTrading(); 

                strTip.Format("满足交易结束时间，下单已终止，挂单已撤销。本次下单共成交%d笔，剩余%d笔。累计成交量加权平均价%.2f。",
                    m_pAlgoAutoData->Volume,atoi(m_pAlgoAutoData->InstrumentCount.c_str())-m_pAlgoAutoData->TotalVolume,
                    (m_pAlgoAutoData->TotalVolume>0)?(m_pAlgoAutoData->AverageQuan/m_pAlgoAutoData->TotalVolume):0);
                MessageBox(strTip);
                break;
            }
        }

        if (m_pAlgoAutoData->bActiceTotalTime)
        {
            m_pAlgoAutoData->RetryTotalTimeSave += m_pAlgoAutoData->IdleTimeValue;
            if (m_pAlgoAutoData->RetryTotalTimeSave >= m_pAlgoAutoData->RetryTotalTimeValue)
            {
                //已经到达超时重试总时长
                KillTimer(COM_IDLETIMER);
                UnLockObjectOverTime();
                CancleWaitOrder();
                StopTrading();

                strTip.Format("满足交易结束时间，下单已终止，挂单已撤销。本次下单共成交%d笔，剩余%d笔。累计成交量加权平均价%.2f。",
                    m_pAlgoAutoData->Volume,atoi(m_pAlgoAutoData->InstrumentCount.c_str())-m_pAlgoAutoData->TotalVolume,
                    (m_pAlgoAutoData->TotalVolume>0)?(m_pAlgoAutoData->AverageQuan/m_pAlgoAutoData->TotalVolume):0);
                MessageBox(strTip);
                break;
            }
        } 

        //撤单重下                    
               
        if(m_bCancleOrdering)
        {
            KillTimer(COM_IDLETIMER);
            if(m_pAlgoAutoData->bExecuteTime)
                KillTimer(COM_EXECUTETIMER);
            
            UnLockObjectOverTime(); 
        }else
        {
            TRACE("挂单时间超过空闲时间，撤掉所有报单，重新下单\n"); 
            UnLockObjectOverTime(); 
            CancleWaitOrder();
        }               
        break;
    case COM_EXECUTETIMER:
        LockObjectOverTime();
        if(m_pAlgoAutoData->bIdleTime)
        {
            UnLockObjectOverTime();
            break;
        }

        if (m_pAlgoAutoData->bActiceOverTime)                        
        {   
            m_pAlgoAutoData->RetryCountSave += 1;
            if (m_pAlgoAutoData->RetryCountSave >= atoi(m_pAlgoAutoData->RetryCount.c_str()))
            {
                //已经到达超时重试次数
                KillTimer(COM_EXECUTETIMER);
                UnLockObjectOverTime();
                CancleWaitOrder();
                StopTrading(); 

                strTip.Format("满足交易结束时间，下单已终止，挂单已撤销。本次下单共成交%d笔，剩余%d笔。累计成交量加权平均价%.2f。",
                    m_pAlgoAutoData->Volume,atoi(m_pAlgoAutoData->InstrumentCount.c_str())-m_pAlgoAutoData->TotalVolume,
                    (m_pAlgoAutoData->TotalVolume>0)?(m_pAlgoAutoData->AverageQuan/m_pAlgoAutoData->TotalVolume):0);
                MessageBox(strTip);
                break;
            }
        }

        if (m_pAlgoAutoData->bActiceTotalTime)
        {
            m_pAlgoAutoData->RetryTotalTimeSave += m_pAlgoAutoData->ExecuteTimeValue;
            if (m_pAlgoAutoData->RetryTotalTimeSave >= m_pAlgoAutoData->RetryTotalTimeValue)
            {
                //已经到达超时重试总时长	
                KillTimer(COM_EXECUTETIMER);
                UnLockObjectOverTime();
                CancleWaitOrder();
                StopTrading();

                strTip.Format("满足交易结束时间，下单已终止，挂单已撤销。本次下单共成交%d笔，剩余%d笔。累计成交量加权平均价%.2f。",
                    m_pAlgoAutoData->Volume,atoi(m_pAlgoAutoData->InstrumentCount.c_str())-m_pAlgoAutoData->TotalVolume,
                    (m_pAlgoAutoData->TotalVolume>0)?(m_pAlgoAutoData->AverageQuan/m_pAlgoAutoData->TotalVolume):0);
                MessageBox(strTip);
                break;
            }
        }

        //撤单重下                   
        if(m_bCancleOrdering)
        {
            KillTimer(COM_EXECUTETIMER);
            UnLockObjectOverTime();
        }else
        {
            TRACE("挂单时间超过执行时间，撤掉所有报单，重新下单\n");
            UnLockObjectOverTime();
            CancleWaitOrder();
        }        
        break;                        
    }

    CBookOrderOperations::OnTimer(nIDEvent);
}
BOOL CAlgoAutoOrderDlg::CancleWaitOrder()
{    
    if(0 == m_pAlgoAutoData->m_nOrderCount)
    {
        EndTrading();
        return FALSE;
    }else
    {
        int nCount = 0;
        if (m_bAllOrderStatusUpdata)
        {
            vector<ORDERRSP> ::iterator iter = m_pAlgoAutoData->vecOrderRSP.begin();
            for (iter = m_pAlgoAutoData->vecOrderRSP.begin();iter!=m_pAlgoAutoData->vecOrderRSP.end();iter++)
            {
                if(iter->status.orderInfo.OrderStatus==THOST_FTDC_OST_PartTradedQueueing //部分成交还在队列中
                    || iter->status.orderInfo.OrderStatus==THOST_FTDC_OST_NoTradeQueueing//未成交还在队列中
                    )
                {    
                    m_bCancleOrdering = TRUE;

                    SYSTEMTIME sys;
                    GetLocalTime(&sys);  //获取当前系统时间
                    TRACE("撤单 %d:%d:%d.%d  OrderStatus = %d OrderRef = %s\n",sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds,iter->status.orderInfo.OrderStatus,iter->status.orderInfo.OrderRef);

                    iter->cancle = TRUE;
                    CancelOrder(iter->status.orderInfo);                  
                }else if(iter->status.orderInfo.OrderStatus==THOST_FTDC_OST_Canceled ||
                    iter->status.orderInfo.OrderStatus==THOST_FTDC_OST_AllTraded)
                {
                    nCount++;
                }
            }

            if(nCount == m_pAlgoAutoData->m_nOrderCount)
                EndTrading();

            return TRUE;
        }else
            return FALSE;
    }
}

// int CAlgoAutoOrderDlg::GetWaitOrderCount()
// {
//     int nCount = 0;
//     vector<ORDERRSP> ::iterator iter = m_pAlgoAutoData->vecOrderRSP.begin();
//     for (iter = m_pAlgoAutoData->vecOrderRSP.begin();iter!=m_pAlgoAutoData->vecOrderRSP.end();iter++)
//     {
//         if(iter->status.orderInfo.OrderStatus==THOST_FTDC_OST_PartTradedQueueing //部分成交还在队列中
//             || iter->status.orderInfo.OrderStatus==THOST_FTDC_OST_NoTradeQueueing//未成交还在队列中
//             )
//         {    
//             nCount++;
//         }   
//     }
// 
//     return nCount;
// }

void CAlgoAutoOrderDlg::StartTrading(void)
{
    m_PageGeneralParam.GetAlgoParamFromWnd();

    int nOrderAllCount = atoi(m_pAlgoAutoData->InstrumentCount.c_str());
    switch(m_pAlgoAutoData->InstrumentOffsetID)
    {
    case 0://开仓    
        break;
    case 1://平今
        if(1==m_pAlgoAutoData->InstrumentDirection)//卖
        {
            if(m_pAlgoAutoData->m_tBuyPosition.TodayPosition < nOrderAllCount)
            {
                ::MessageBox(NULL, "平今数量不足，请重新设置", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
                return ;
            }  
        }
        else
        {
            if(m_pAlgoAutoData->m_tSellPosition.TodayPosition < nOrderAllCount)
            {
                ::MessageBox(NULL, "平今数量不足，请重新设置", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
                return ;
            }
        }
        break;
    case 2://平仓
        if(1==m_pAlgoAutoData->InstrumentDirection)//卖
        {
            if(m_pAlgoAutoData->m_tBuyPosition.YdPositionRemain < nOrderAllCount)
            {
                ::MessageBox(NULL, "平仓数量不足，请重新设置", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
                return ;
            }  
        }
        else
        {
            if(m_pAlgoAutoData->m_tSellPosition.YdPositionRemain < nOrderAllCount)
            {
                ::MessageBox(NULL, "平仓数量不足，请重新设置", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
                return ;
            }
        }
        break;        
    }

    TRACE("启动一次交易\n");
    LockObjectOverTime();
    m_bStopTrade = FALSE;    
    m_pAlgoAutoData->RetryCountSave = 0;
    m_pAlgoAutoData->RetryTotalTimeSave = 0;
	m_pAlgoAutoData->AverageQuan = 0.0f;
    m_pAlgoAutoData->Volume = 0; 
    m_pAlgoAutoData->TotalVolume = 0;
    m_pAlgoAutoData->TradingTime = 0;
    m_pAlgoAutoData->vecOrderInfo.clear();
    m_pAlgoAutoData->vecOrderRSP.clear();    
    UnLockObjectOverTime();

    m_PageFeedback.UpdataFeedbackData(FALSE);    
    m_PageGeneralParam.EnableCtrls(FALSE);
    m_PageGeneralParam.m_bOkClicked = TRUE;

	DWORD code;
	if(!GetExitCodeThread(m_hThread,&code)||(code!=STILL_ACTIVE))
	{
 		m_hCloseEvent = CreateEvent(NULL, FALSE, FALSE , NULL);
		m_hThread=CreateThread(NULL,0,ThreadProc,this,0,NULL);//创建线程
	}
}

void CAlgoAutoOrderDlg::StopTrading(void)
{
    TRACE("停止一次交易\n");    
    m_bStopTrade = TRUE;

    if(m_wndStatusBar.GetSafeHwnd()!=NULL)
    {
        if (m_bTrading)
            m_wndStatusBar.SetText("交易中断",0,0);
        else
            m_wndStatusBar.SetText("交易完成",0,0);
    }

    if(m_hCloseEvent == NULL || m_hThread == NULL)
        return;    

    if(m_pAlgoAutoData->bIdleTime)
        KillTimer(COM_IDLETIMER);
    if(m_pAlgoAutoData->bExecuteTime)
        KillTimer(COM_EXECUTETIMER);

    if (m_pAlgoAutoData->bActiceOpt)
    {
        KillTimer(OPT_BACKTIMER);
    }

    DWORD dwExitCode;
    SetEvent(m_hCloseEvent);
    GetExitCodeThread(m_hThread, &dwExitCode);
    if (dwExitCode == STILL_ACTIVE)
    {
        WaitForSingleObject( m_hThread, 6000);
        if( GetExitCodeThread(m_hThread, &dwExitCode) !=0 && dwExitCode == STILL_ACTIVE )
        {
            TerminateThread( m_hThread, 0 );
        }
    }

    TRACE("...........\n");
    m_bTradeTime = FALSE;
    m_bTrading = FALSE;
    m_bThreadExit = FALSE;
    m_bOptTrigger = FALSE;
    m_PageGeneralParam.EnableCtrls(TRUE);
    m_PageGeneralParam.m_bOkClicked = FALSE;
    CloseHandle(m_hCloseEvent);
    CloseHandle(m_hThread);
    m_hCloseEvent = NULL;
    m_hThread = NULL;
}
void CAlgoAutoOrderDlg::ThreadQuitClear(BOOL bSuccess)
{
    if(m_wndStatusBar.GetSafeHwnd()!=NULL)
    {
        if (!bSuccess)
            m_wndStatusBar.SetText("交易中断",0,0);
        else
            m_wndStatusBar.SetText("交易完成",0,0);
    }

    if (m_pAlgoAutoData->bActiceOpt)
    {
        KillTimer(OPT_BACKTIMER);
    }

    CancleWaitOrder();
    
    m_bTrading = FALSE;
    m_bThreadExit = FALSE;
    m_bOptTrigger = FALSE;
    m_PageGeneralParam.EnableCtrls(TRUE);
    CloseHandle(m_hCloseEvent);
    CloseHandle(m_hThread);
    m_hCloseEvent = NULL;
    m_hThread = NULL;
}
void CAlgoAutoOrderDlg::EndTrading(void)
{   
    LockObjectOverTime();  
    if(m_pAlgoAutoData->bIdleTime)
        KillTimer(COM_IDLETIMER);
    if(m_pAlgoAutoData->bExecuteTime)
        KillTimer(COM_EXECUTETIMER);
    m_pAlgoAutoData->Volume = 0; 
    m_pAlgoAutoData->m_nOrderCount = 0;
    m_pAlgoAutoData->vecOrderRSP.clear();     
    m_bThreadExit = FALSE;
    m_nWaitStatusTime = 0;    
    UnLockObjectOverTime();
}
BOOL CAlgoAutoOrderDlg::ChangeGeneralParam(SINGLEOPT* pSingle)
{    
    //TRACE("进入优化计算\n");
    if (m_pAlgoAutoData->vecOrderInfo.size() == 0/* || !(m_pAlgoAutoData->bHoldArrived)*/)
        return FALSE;

    m_pAlgoAutoData->vecOrderActiveInfo.clear();    

    //计算多档下单量
    vector<ORDERINFO>::iterator iter = m_pAlgoAutoData->vecOrderInfo.begin();
    vector<string> vecOptPriceSpread;
    vector<string> vecPriceSpread = m_pAlgoAutoData->WDDATA_PriceSpread.data;

    ORDERINFO orderInfo;
    orderInfo.name = m_pAlgoAutoData->InstrumentName;
    orderInfo.direction = (m_pAlgoAutoData->InstrumentDirection == 0)?1:0;
    if(m_pAlgoAutoData->InstrumentOffsetID == 0) orderInfo.offsetID = 1;
    else if(m_pAlgoAutoData->InstrumentOffsetID == 1) orderInfo.offsetID = 2;
    else orderInfo.offsetID = 3;

    int nTotalCount = 0;
    int nRestOrderCount = 0;

    switch(m_pAlgoAutoData->InstrumentOffsetID)
    {
    case 0://开仓    
        nRestOrderCount = atoi(m_pAlgoAutoData->InstrumentCount.c_str()) - m_pAlgoAutoData->TotalVolume;//总剩余数量
        break;
    case 1://平今
        if(0==orderInfo.direction)//卖
            nRestOrderCount = ((atoi(m_pAlgoAutoData->InstrumentCount.c_str()) - m_pAlgoAutoData->TotalVolume)>=m_pAlgoAutoData->m_tBuyPosition.TodayPosition)?m_pAlgoAutoData->m_tBuyPosition.TodayPosition:(atoi(m_pAlgoAutoData->InstrumentCount.c_str()) - m_pAlgoAutoData->TotalVolume);
        else
            nRestOrderCount = ((atoi(m_pAlgoAutoData->InstrumentCount.c_str()) - m_pAlgoAutoData->TotalVolume)>=m_pAlgoAutoData->m_tSellPosition.TodayPosition)?m_pAlgoAutoData->m_tSellPosition.TodayPosition:(atoi(m_pAlgoAutoData->InstrumentCount.c_str()) - m_pAlgoAutoData->TotalVolume);
        break;
    case 2://平仓
        if(0==orderInfo.direction)//卖
            nRestOrderCount = ((atoi(m_pAlgoAutoData->InstrumentCount.c_str()) - m_pAlgoAutoData->TotalVolume)>=m_pAlgoAutoData->m_tBuyPosition.YdPositionRemain)?m_pAlgoAutoData->m_tBuyPosition.YdPositionRemain:(atoi(m_pAlgoAutoData->InstrumentCount.c_str()) - m_pAlgoAutoData->TotalVolume);
        else
            nRestOrderCount = ((atoi(m_pAlgoAutoData->InstrumentCount.c_str()) - m_pAlgoAutoData->TotalVolume)>=m_pAlgoAutoData->m_tSellPosition.YdPositionRemain)?m_pAlgoAutoData->m_tSellPosition.YdPositionRemain:(atoi(m_pAlgoAutoData->InstrumentCount.c_str()) - m_pAlgoAutoData->TotalVolume);
        break;        
    }

    if(0==nRestOrderCount)
        return FALSE;

    for (int i=0;i<5;i++)
    {
        vecOptPriceSpread = pSingle->OptSlidePriceSpread.data;

        //数量
        int nCount = ((int)((iter->count)*atof(pSingle->Multiple.c_str())) <= nRestOrderCount)?(int)((iter->count)*atof(pSingle->Multiple.c_str())):nRestOrderCount;
        if (pSingle->OptMarketDegreed != 0 && nCount > m_pAlgoAutoData->NewVolume * (pSingle->OptMarketDegreed/100.0f))
            nCount = (int)(m_pAlgoAutoData->NewVolume * (pSingle->OptMarketDegreed/100.0f));            

        orderInfo.count = nCount;

        //价格
        vecOptPriceSpread[i].erase(remove_if(vecOptPriceSpread[i].begin(),vecOptPriceSpread[i].end(),bind2nd(equal_to<char>(), ' ')),vecOptPriceSpread[i].end());
        if (0 == strlen(vecOptPriceSpread[i].c_str()))
        {
//             if (m_pAlgoAutoData->WDDATA_OrderCount.index == 0 && m_pAlgoAutoData->WDDATA_PriceSpread.index == 0)//百分比 百分比
//             {
//                 orderInfo.price = atof(m_pAlgoAutoData->BasicPrice.value.c_str())*(1+atof(vecPriceSpread[i].c_str())/100.0f);
// 
//             }else 
//             if(m_pAlgoAutoData->WDDATA_OrderCount.index == 0 && m_pAlgoAutoData->WDDATA_PriceSpread.index == 1)//百分比 点位 
//             {
//                 orderInfo.price = atof(m_pAlgoAutoData->BasicPrice.value.c_str())+(atof(vecPriceSpread[i].c_str()))*m_pAlgoAutoData->PriceTick;
// 
//             }
//             else if (m_pAlgoAutoData->WDDATA_OrderCount.index == 1 && m_pAlgoAutoData->WDDATA_PriceSpread.index == 0)//手数 百分比
//             {
//                 orderInfo.price = atof(m_pAlgoAutoData->BasicPrice.value.c_str())*(1+atof(vecPriceSpread[i].c_str())/100.0f);
// 
//             }else if (m_pAlgoAutoData->WDDATA_OrderCount.index == 1 && m_pAlgoAutoData->WDDATA_PriceSpread.index == 1)//手数 点位
//             {
//                 orderInfo.price = atof(m_pAlgoAutoData->BasicPrice.value.c_str())+atof(vecPriceSpread[i].c_str())*m_pAlgoAutoData->PriceTick;
//             }
        }
        else
        {
//             if( pSingle->OptSlidePriceSpread.index == 0)
//                 orderInfo.price = atof(m_pAlgoAutoData->BasicPrice.value.c_str())*(1+atof(vecOptPriceSpread[i].c_str())/100.0f);
//             else
                orderInfo.price = atof(m_pAlgoAutoData->BasicPrice.value.c_str())+(atof(vecOptPriceSpread[i].c_str()))*m_pAlgoAutoData->PriceTick;
        }

        orderInfo.index = i;            

        if(0 == m_pAlgoAutoData->InstrumentDirection)//买入
        {
            if (orderInfo.price > atof(m_pAlgoAutoData->LimitPrice.c_str()))
            {                                                    
                orderInfo.price = atof(m_pAlgoAutoData->LimitPrice.c_str());
            }
        }
        else if (1 == m_pAlgoAutoData->InstrumentDirection)
        {
            if (orderInfo.price < atof(m_pAlgoAutoData->LimitPrice.c_str()))
            {                               
                orderInfo.price = atof(m_pAlgoAutoData->LimitPrice.c_str());
            }
        }

        nTotalCount += orderInfo.count;
        nRestOrderCount -= orderInfo.count;

        if (orderInfo.count != 0)
        {
            m_pAlgoAutoData->m_nOrderCount++;
        }

        m_pAlgoAutoData->vecOrderActiveInfo.push_back(orderInfo);  

        iter++;
    }

    char p[32]={0};
    itoa(nTotalCount,p,10);
    //m_pAlgoAutoData->OrderCount.value = p;

    m_pAlgoAutoData->OrderCountValue = atoi(p);

//     switch(m_pAlgoAutoData->ResetCount.type)
//     { 
//     case 0:
//         itoa((int)((atof(m_pAlgoAutoData->ResetCount.value.c_str())*nTotalCount)/100.0f),p,10);
//         break;
//     case 1:
//         itoa(atoi(m_pAlgoAutoData->ResetCount.value.c_str()),p,10);
//         break;
//     }

//     m_pAlgoAutoData->ResetCount.value = p; 
//     m_pAlgoAutoData->ResetCountValue = atoi(p);

    if (0==m_pAlgoAutoData->OrderCountValue/*||0==m_pAlgoAutoData->ResetCountValue*/)
        return FALSE;

    CString strHistoryPrice;
    strHistoryPrice.Format("%s",m_pAlgoAutoData->BasicPrice.value.c_str());
    pSingle->History = strHistoryPrice;
    return TRUE;
    //TRACE("结束优化计算\n");    
}       

void CAlgoAutoOrderDlg::OnClose()
{
    // TODO: Add your message handler code here and/or call default
   
    int irlt;
    if(!m_bTrading)
    {
        irlt=MessageBox("是否保存当前设置？","AlgoTrading",MB_YESNO);
        if(irlt==IDYES)
            SaveIni();

        ::PostMessage(::AfxGetMainWnd()->GetSafeHwnd(),	GetBookOrderCloseMSGID(), 0, (LPARAM)this);
        CBookOrderOperations::OnClose();
    }
    else
    {
        MessageBox("正在执行交易，如果想退出窗口，请点击停止交易按钮，然后再关闭窗口。");
    }

}

void CAlgoAutoOrderDlg::SaveIni(void)
{
    char PathFileName[512];
    char InstrumentName[512];

    //获取程序路径
    memset(PathFileName,0,sizeof(PathFileName));
    memset(InstrumentName,0,sizeof(InstrumentName));
    CTools_Win32::GetMainPath(PathFileName,sizeof(PathFileName)-1-strlen(IniFileName_AlgoAutoData));
    sprintf_s(InstrumentName,"%s_%s",m_PageGeneralParam.GetInstrumentID(),IniFileName_AlgoAutoData);
    strcat(PathFileName,InstrumentName);
    CIniFile *pIniFile=NULL;
    pIniFile=new CIniFile(PathFileName);
    if(!pIniFile) return;

    //保存交易设置
    //是否正在执行交易
    pIniFile->UpdateKeyVal("Trading","m_bTrading",(int)m_bTrading);
    //开始执行的时间
//     pIniFile->UpdatedwordKeyVal("Trading","m_StartTradingUTC",(unsigned int)m_StartTradingUTC);
    //当前执行的节，0 ... N-1
//     pIniFile->UpdateKeyVal("Trading","m_TradingSession",m_TradingSession);


    if(pIniFile)
    {
        delete pIniFile;
        pIniFile=NULL;
    }

    if(!m_bTrading)
        m_PageGeneralParam.GetAlgoParamFromWnd();
    m_pAlgoAutoData->SaveIni();

}

void CAlgoAutoOrderDlg::ReadIni(void)
{
    char PathFileName[512];
    char InstrumentName[512];

    //获取程序路径
    memset(PathFileName,0,sizeof(PathFileName));
    memset(InstrumentName,0,sizeof(InstrumentName));
    CTools_Win32::GetMainPath(PathFileName,sizeof(PathFileName)-1-strlen(IniFileName_AlgoAutoData));
    sprintf_s(InstrumentName,"%s_%s",m_strInstrumentName,IniFileName_AlgoAutoData);
    strcat(PathFileName,InstrumentName);
    HFILE hF=_lopen(PathFileName,OF_READ);
    if(hF==-1)
    {
        _lclose(hF);
        return;
    }

    string str=m_strInstrumentName.GetBuffer(0);
    if(m_pAlgoAutoData->ReadIni(str))
        m_PageGeneralParam.SetAlgoParamToWnd();

    m_strInstrumentName.ReleaseBuffer();
}



BOOL CAlgoAutoOrderDlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE)  
        return TRUE;
    if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
        return TRUE;

    return CBookOrderOperations::PreTranslateMessage(pMsg);
}
