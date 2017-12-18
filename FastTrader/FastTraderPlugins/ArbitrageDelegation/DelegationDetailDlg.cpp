// DelegationDetail.cpp : implementation file
//

#include "stdafx.h"
#include "Tools.h"
#include "PlusinAPI.h"
#include "ArbitrageDelegation.h"
#include "DelegationDetailDlg.h"


// CDelegationDetailDlg dialog

IMPLEMENT_DYNAMIC(CDelegationDetailDlg, CDialog)

CDelegationDetailDlg::CDelegationDetailDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDelegationDetailDlg::IDD, pParent)
{
}

CDelegationDetailDlg::CDelegationDetailDlg(set<long>& setFTID,CWnd* pParent /*=NULL*/)
: CDialog(CDelegationDetailDlg::IDD, pParent)
{
    m_setFTID = setFTID;
}

CDelegationDetailDlg::CDelegationDetailDlg(set<OrderKey>& setOrderKey,CWnd* pParent /*=NULL*/)
: CDialog(CDelegationDetailDlg::IDD, pParent)
{   
    m_setOrderKey = setOrderKey;
}

CDelegationDetailDlg::~CDelegationDetailDlg()
{
    m_mapOrderInfo.clear();
    m_setFTID.clear();
}

void CDelegationDetailDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_DETAIL, m_listDetailCtrl);
}


BEGIN_MESSAGE_MAP(CDelegationDetailDlg, CDialog)
    ON_BN_CLICKED(IDC_BUTTON_REVOKE, &CDelegationDetailDlg::OnBnClickedButtonRevoke)
    ON_BN_CLICKED(IDOK, &CDelegationDetailDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CDelegationDetailDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDelegationDetailDlg message handlers

BOOL CDelegationDetailDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    m_listDetailCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);

    //插入列 
    int i=0;
    m_listDetailCtrl.InsertColumn( i++, "报单编号",     LVCFMT_CENTER,    60 ); 
    m_listDetailCtrl.InsertColumn( i++, "合约",         LVCFMT_CENTER,    60 ); 
    m_listDetailCtrl.InsertColumn( i++, "买卖",         LVCFMT_CENTER,    60 ); 
    m_listDetailCtrl.InsertColumn( i++, "开平",         LVCFMT_CENTER,    60 ); 
    m_listDetailCtrl.InsertColumn( i++, "挂单状态",     LVCFMT_CENTER,    120 ); 
    m_listDetailCtrl.InsertColumn( i++, "报单价格",     LVCFMT_CENTER,    60 ); 
    m_listDetailCtrl.InsertColumn( i++, "报单手数",     LVCFMT_CENTER,    60 ); 
    m_listDetailCtrl.InsertColumn( i++, "未成交手数",   LVCFMT_CENTER,    80 );
    m_listDetailCtrl.InsertColumn( i++, "成交手数",     LVCFMT_CENTER,    60 ); 
    m_listDetailCtrl.InsertColumn( i++, "详细状态",     LVCFMT_CENTER,    120 ); 
    m_listDetailCtrl.InsertColumn( i++, "报单时间",     LVCFMT_CENTER,    60 ); 
    m_listDetailCtrl.InsertColumn( i++, "冻结保证金",   LVCFMT_CENTER,    80 ); 
    m_listDetailCtrl.InsertColumn( i++, "冻结手续费",   LVCFMT_CENTER,    80 ); 
    m_listDetailCtrl.InsertColumn( i++, "投保",         LVCFMT_CENTER,    60 ); 
    m_listDetailCtrl.InsertColumn( i++, "交易所",       LVCFMT_CENTER,    60 ); 
    m_listDetailCtrl.InsertColumn( i++, "报单引用",     LVCFMT_CENTER,    60 ); 
    m_listDetailCtrl.InsertColumn( i++, "报单类型",     LVCFMT_CENTER,    60 ); 
    m_listDetailCtrl.InsertColumn( i++, "是否强平",     LVCFMT_CENTER,    60 ); 
    m_listDetailCtrl.InsertColumn( i++, "强平信息",     LVCFMT_CENTER,    60 ); 

    if(!m_mapOrderInfo.empty())
        SetDataToListCtrl();
    if(!m_setOrderKey.empty())
    {
        if(g_hWnd_FT!=NULL)
        {
            PlusinAPI* pAPI=PlusinAPI::GetInstance();
            if(pAPI)
            {
                for(set<OrderKey>::iterator itset=m_setOrderKey.begin();itset!=m_setOrderKey.end();itset++)
                    pAPI->Send(CMDID_ALL_ORDER_INFO_QUERY2,&(*itset),sizeof(OrderKey));
            }
        }        
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDelegationDetailDlg::SetDataToListCtrl()
{    
    int index=0;
    for(map<string,PlatformStru_OrderInfo>::iterator it=m_mapOrderInfo.begin();it!=m_mapOrderInfo.end();it++,index++)
    {
        PlatformStru_OrderInfo *pOrderData = &(it->second);

        std::string strCombOffsetFlag,strHedgeFlag,strOrderStatus,strForceCloseReason;
        switch(pOrderData->CombOffsetFlag[0])
        {
        case THOST_FTDC_OF_Open:
            strCombOffsetFlag = "开仓";
            break;
        case THOST_FTDC_OF_Close:
            strCombOffsetFlag = "平仓";
            break;
        case THOST_FTDC_OF_ForceClose:
            strCombOffsetFlag = "强平";
            break;
        case THOST_FTDC_OF_CloseToday:
            strCombOffsetFlag = "平今";
            break;
        case THOST_FTDC_OF_CloseYesterday:
            strCombOffsetFlag = "平昨";
            break;
        case THOST_FTDC_OF_ForceOff:
            strCombOffsetFlag = "强减";
            break;
        case THOST_FTDC_OF_LocalForceClose:
            strCombOffsetFlag = "本地强平";
            break;
        }
        switch(pOrderData->CombHedgeFlag[0])
        {
        case THOST_FTDC_HF_Speculation: 
            strHedgeFlag = "投机";
            break;
        case THOST_FTDC_HF_Arbitrage:
            strHedgeFlag = "套利";
            break;
        case THOST_FTDC_HF_Hedge:
            strHedgeFlag = "套保";
            break;
        }

        switch(pOrderData->OrderStatus)
        {
        case THOST_FTDC_OST_AllTraded:
            strOrderStatus = "全部成交";
            break;
        case THOST_FTDC_OST_PartTradedQueueing:
            strOrderStatus = "部分成交还在队列中";
            break;
        case THOST_FTDC_OST_PartTradedNotQueueing:
            strOrderStatus = "部分成交不在队列中";
            break;
        case THOST_FTDC_OST_NoTradeQueueing:
            strOrderStatus = "未成交还在队列中";
            break;
        case THOST_FTDC_OST_NoTradeNotQueueing:
            strOrderStatus = "未成交不在队列中";
            break;
        case THOST_FTDC_OST_Canceled:
            strOrderStatus = "撤单";
            break;
        case THOST_FTDC_OST_Unknown:
            strOrderStatus = "未知";
            break;
        case THOST_FTDC_OST_NotTouched:
            strOrderStatus = "尚未触发";
            break;
        case THOST_FTDC_OST_Touched:
            strOrderStatus = "已触发";
            break;
        }

        switch(pOrderData->ForceCloseReason)
        {
        case THOST_FTDC_FCC_NotForceClose:
            strForceCloseReason = "非强平";
            break;
        case THOST_FTDC_FCC_LackDeposit:
            strForceCloseReason = "资金不足";
            break;
        case THOST_FTDC_OST_PartTradedNotQueueing:
            strForceCloseReason = "客户超仓";
            break;
        case THOST_FTDC_FCC_MemberOverPositionLimit:
            strForceCloseReason = "会员超仓";
            break;
        case THOST_FTDC_FCC_NotMultiple:
            strForceCloseReason = "持仓非整数倍";
            break;
        case THOST_FTDC_FCC_Violation:
            strForceCloseReason = "违规";
            break;
        case THOST_FTDC_FCC_Other:
            strForceCloseReason = "其它";
            break;
        case THOST_FTDC_FCC_PersonDeliv:
            strForceCloseReason = "自然人临近交割";
            break;
        case THOST_FTDC_FCC_ForceClose:
            strForceCloseReason = "强平";
            break;                
        }

        int i=1;
        m_listDetailCtrl.InsertItem(index,pOrderData->OrderSysID);                                                                   //报单编号
        m_listDetailCtrl.SetItemText(index,i++,pOrderData->InstrumentID);                                                            //合约
        m_listDetailCtrl.SetItemText(index,i++,pOrderData->Direction=='1'?"卖":"买");                                                     //买卖
        m_listDetailCtrl.SetItemText(index,i++,strCombOffsetFlag.c_str());                                                      //开平
        m_listDetailCtrl.SetItemText(index,i++,strOrderStatus.c_str());                                                         //挂单状态
        m_listDetailCtrl.SetItemText(index,i++,CTools::StringFromDouble(pOrderData->LimitPrice).c_str());                            //报单价格
        m_listDetailCtrl.SetItemText(index,i++,CTools::StringFromInt(pOrderData->VolumeTotalOriginal).c_str());                      //报单手数
        m_listDetailCtrl.SetItemText(index,i++,CTools::StringFromInt(pOrderData->VolumeTotal).c_str());                              //未成交手数
        m_listDetailCtrl.SetItemText(index,i++,CTools::StringFromInt(pOrderData->VolumeTotalOriginal-pOrderData->VolumeTotal).c_str());   //成交手数
        m_listDetailCtrl.SetItemText(index,i++,pOrderData->StatusMsg);                                                               //详细状态
        m_listDetailCtrl.SetItemText(index,i++,pOrderData->InsertTime);                                                              //报单时间
        m_listDetailCtrl.SetItemText(index,i++,CTools::StringFromDouble(pOrderData->freezeMargin).c_str());                          //冻结保证金
        m_listDetailCtrl.SetItemText(index,i++,CTools::StringFromDouble(pOrderData->troubleMoney).c_str());                          //冻结手续费
        m_listDetailCtrl.SetItemText(index,i++,strHedgeFlag.c_str());                                                           //投保
        m_listDetailCtrl.SetItemText(index,i++,pOrderData->ExchangeID);                                                              //交易所
        m_listDetailCtrl.SetItemText(index,i++,pOrderData->OrderRef);                                                                //报单引用
        m_listDetailCtrl.SetItemText(index,i++,pOrderData->UserForceClose?"是":"否");                                                //是否强平
        m_listDetailCtrl.SetItemText(index,i++,strForceCloseReason.c_str());                                                    //强平信息

        //绑定
        m_listDetailCtrl.SetItemData(index,(DWORD_PTR)pOrderData);
    }
}

int CDelegationDetailDlg::GetListSelectedIndex()
{
    POSITION pos = m_listDetailCtrl.GetFirstSelectedItemPosition(); 
    return m_listDetailCtrl.GetNextSelectedItem(pos);
}

void CDelegationDetailDlg::OnBnClickedButtonRevoke()
{
    // TODO: Add your control notification handler code here
    int nSelectedIndex = GetListSelectedIndex();
    if(nSelectedIndex<0)
        return;

    if(IDCANCEL == ::MessageBox(NULL, "是否确认撤单?", "警告", MB_OKCANCEL|MB_ICONWARNING|MB_SYSTEMMODAL))
        return;

    PlatformStru_OrderInfo* pOrderData = (PlatformStru_OrderInfo*)m_listDetailCtrl.GetItemData(nSelectedIndex);
    if(pOrderData!=NULL)
        RevokeOrder(pOrderData);
}

void CDelegationDetailDlg::RevokeOrder(PlatformStru_OrderInfo* pOrderData)
{
    TRACE("RevokeOrder OrderSysID = %s\n",pOrderData->OrderSysID);
    PlatformStru_InputOrderAction tAction;
    ZeroMemory(&tAction, sizeof(tAction));

    tAction.Thost.FrontID=pOrderData->FrontID;
    tAction.Thost.SessionID=pOrderData->SessionID;
    memcpy(tAction.Thost.OrderRef, pOrderData->OrderRef, sizeof(pOrderData->OrderRef));
    memcpy(tAction.Thost.OrderSysID, pOrderData->OrderSysID, sizeof(pOrderData->OrderSysID));
    memcpy(tAction.Thost.ExchangeID, pOrderData->ExchangeID, sizeof(pOrderData->ExchangeID));
    memcpy(tAction.Thost.InstrumentID, pOrderData->InstrumentID, sizeof(pOrderData->InstrumentID));


    PlusinAPI* pAPI=PlusinAPI::GetInstance();
    if(pAPI) 
        pAPI->Send(CMDID_ORDER_CANCEL_ORDER, &tAction, sizeof(tAction),1000);
}
void CDelegationDetailDlg::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    ::SendMessage(AfxGetApp()->m_pMainWnd->m_hWnd,WM_USER_DETAILDLG_CLOSE,0,0);

    OnOK();
}

void CDelegationDetailDlg::OnBnClickedCancel()
{
    // TODO: Add your control notification handler code here
    ::SendMessage(AfxGetApp()->m_pMainWnd->m_hWnd,WM_USER_DETAILDLG_CLOSE,0,0);

    OnCancel();
}

void CDelegationDetailDlg::Update(const PlatformStru_OrderInfo* pOrderData)
{
    if(pOrderData == NULL)
        return;    

    m_mapOrderInfo[pOrderData->InstrumentID] = *pOrderData;

    std::string strCombOffsetFlag,strHedgeFlag,strOrderStatus,strForceCloseReason,strOrderType;
    switch(pOrderData->CombOffsetFlag[0])
    {
    case THOST_FTDC_OF_Open:
        strCombOffsetFlag = "开仓";
        break;
    case THOST_FTDC_OF_Close:
        strCombOffsetFlag = "平仓";
        break;
    case THOST_FTDC_OF_ForceClose:
        strCombOffsetFlag = "强平";
        break;
    case THOST_FTDC_OF_CloseToday:
        strCombOffsetFlag = "平今";
        break;
    case THOST_FTDC_OF_CloseYesterday:
        strCombOffsetFlag = "平昨";
        break;
    case THOST_FTDC_OF_ForceOff:
        strCombOffsetFlag = "强减";
        break;
    case THOST_FTDC_OF_LocalForceClose:
        strCombOffsetFlag = "本地强平";
        break;
    }
    switch(pOrderData->CombHedgeFlag[0])
    {
    case THOST_FTDC_HF_Speculation: 
        strHedgeFlag = "投机";
        break;
    case THOST_FTDC_HF_Arbitrage:
        strHedgeFlag = "套利";
        break;
    case THOST_FTDC_HF_Hedge:
        strHedgeFlag = "套保";
        break;
    }

    switch(pOrderData->OrderStatus)
    {
    case THOST_FTDC_OST_AllTraded:
        strOrderStatus = "全部成交";
        break;
    case THOST_FTDC_OST_PartTradedQueueing:
        strOrderStatus = "部分成交还在队列中";
        break;
    case THOST_FTDC_OST_PartTradedNotQueueing:
        strOrderStatus = "部分成交不在队列中";
        break;
    case THOST_FTDC_OST_NoTradeQueueing:
        strOrderStatus = "未成交还在队列中";
        break;
    case THOST_FTDC_OST_NoTradeNotQueueing:
        strOrderStatus = "未成交不在队列中";
        break;
    case THOST_FTDC_OST_Canceled:
        strOrderStatus = "撤单";
        break;
    case THOST_FTDC_OST_Unknown:
        strOrderStatus = "未知";
        break;
    case THOST_FTDC_OST_NotTouched:
        strOrderStatus = "尚未触发";
        break;
    case THOST_FTDC_OST_Touched:
        strOrderStatus = "已触发";
        break;
    }

    switch(pOrderData->ForceCloseReason)
    {
    case THOST_FTDC_FCC_NotForceClose:
        strForceCloseReason = "非强平";
        break;
    case THOST_FTDC_FCC_LackDeposit:
        strForceCloseReason = "资金不足";
        break;
    case THOST_FTDC_OST_PartTradedNotQueueing:
        strForceCloseReason = "客户超仓";
        break;
    case THOST_FTDC_FCC_MemberOverPositionLimit:
        strForceCloseReason = "会员超仓";
        break;
    case THOST_FTDC_FCC_NotMultiple:
        strForceCloseReason = "持仓非整数倍";
        break;
    case THOST_FTDC_FCC_Violation:
        strForceCloseReason = "违规";
        break;
    case THOST_FTDC_FCC_Other:
        strForceCloseReason = "其它";
        break;
    case THOST_FTDC_FCC_PersonDeliv:
        strForceCloseReason = "自然人临近交割";
        break;
    case THOST_FTDC_FCC_ForceClose:
        strForceCloseReason = "强平";
        break;                
    }

    switch(pOrderData->OrderType)
    {
    case THOST_FTDC_ORDT_Normal:
        strOrderType = "正常";
        break;
    case THOST_FTDC_ORDT_DeriveFromQuote:
        strOrderType = "报价衍生";
        break;
    case THOST_FTDC_ORDT_DeriveFromCombination:
        strOrderType = "组合衍生";
        break;
    case THOST_FTDC_ORDT_Combination:
        strOrderType = "组合报单";
        break;
    case THOST_FTDC_ORDT_ConditionalOrder:
        strOrderType = "条件单";
        break;
    case THOST_FTDC_ORDT_Swap:
        strOrderType = "互换单";
        break;
    }


    bool bMatch = false;
    for (int index=0;index<m_listDetailCtrl.GetItemCount();index++)
    {
        //更新
        CString szInst = m_listDetailCtrl.GetItemText(index,1);
        if(szInst.CompareNoCase(pOrderData->InstrumentID)==0)
        {
            bMatch = true;

            int i=0;
            m_listDetailCtrl.SetItemText(index,i++,pOrderData->OrderSysID);                                                                   //报单编号
            m_listDetailCtrl.SetItemText(index,i++,pOrderData->InstrumentID);                                                            //合约
            m_listDetailCtrl.SetItemText(index,i++,pOrderData->Direction=='1'?"卖":"买");                                                     //买卖
            m_listDetailCtrl.SetItemText(index,i++,strCombOffsetFlag.c_str());                                                      //开平
            m_listDetailCtrl.SetItemText(index,i++,strOrderStatus.c_str());                                                         //挂单状态
            m_listDetailCtrl.SetItemText(index,i++,CTools::StringFromDouble(pOrderData->LimitPrice).c_str());                            //报单价格
            m_listDetailCtrl.SetItemText(index,i++,CTools::StringFromInt(pOrderData->VolumeTotalOriginal).c_str());                      //报单手数
            m_listDetailCtrl.SetItemText(index,i++,CTools::StringFromInt(pOrderData->VolumeTotal).c_str());                              //未成交手数
            m_listDetailCtrl.SetItemText(index,i++,CTools::StringFromInt(pOrderData->VolumeTotalOriginal-pOrderData->VolumeTotal).c_str());   //成交手数
            m_listDetailCtrl.SetItemText(index,i++,pOrderData->StatusMsg);                                                               //详细状态
            m_listDetailCtrl.SetItemText(index,i++,pOrderData->InsertTime);                                                              //报单时间
            m_listDetailCtrl.SetItemText(index,i++,CTools::StringFromDouble(pOrderData->freezeMargin).c_str());                          //冻结保证金
            m_listDetailCtrl.SetItemText(index,i++,CTools::StringFromDouble(pOrderData->troubleMoney).c_str());                          //冻结手续费
            m_listDetailCtrl.SetItemText(index,i++,strHedgeFlag.c_str());                                                           //投保
            m_listDetailCtrl.SetItemText(index,i++,pOrderData->ExchangeID);                                                              //交易所
            m_listDetailCtrl.SetItemText(index,i++,pOrderData->OrderRef);                                                                //报单引用
            m_listDetailCtrl.SetItemText(index,i++,strOrderType.c_str());                                                                //报单类型            
            m_listDetailCtrl.SetItemText(index,i++,pOrderData->UserForceClose?"是":"否");                                                //是否强平
            m_listDetailCtrl.SetItemText(index,i++,strForceCloseReason.c_str());                                                    //强平信息
        }
    }  

    if(!bMatch)//添加
    {
        int nCount = m_listDetailCtrl.GetItemCount();

        int i=1;
        m_listDetailCtrl.InsertItem(nCount,pOrderData->OrderSysID);                                                                   //报单编号
        m_listDetailCtrl.SetItemText(nCount,i++,pOrderData->InstrumentID);                                                            //合约
        m_listDetailCtrl.SetItemText(nCount,i++,pOrderData->Direction=='1'?"卖":"买");                                                     //买卖
        m_listDetailCtrl.SetItemText(nCount,i++,strCombOffsetFlag.c_str());                                                      //开平
        m_listDetailCtrl.SetItemText(nCount,i++,strOrderStatus.c_str());                                                         //挂单状态
        m_listDetailCtrl.SetItemText(nCount,i++,CTools::StringFromDouble(pOrderData->LimitPrice).c_str());                            //报单价格
        m_listDetailCtrl.SetItemText(nCount,i++,CTools::StringFromInt(pOrderData->VolumeTotalOriginal).c_str());                      //报单手数
        m_listDetailCtrl.SetItemText(nCount,i++,CTools::StringFromInt(pOrderData->VolumeTotal).c_str());                              //未成交手数
        m_listDetailCtrl.SetItemText(nCount,i++,CTools::StringFromInt(pOrderData->VolumeTotalOriginal-pOrderData->VolumeTotal).c_str());   //成交手数
        m_listDetailCtrl.SetItemText(nCount,i++,pOrderData->StatusMsg);                                                               //详细状态
        m_listDetailCtrl.SetItemText(nCount,i++,pOrderData->InsertTime);                                                              //报单时间
        m_listDetailCtrl.SetItemText(nCount,i++,CTools::StringFromDouble(pOrderData->freezeMargin).c_str());                          //冻结保证金
        m_listDetailCtrl.SetItemText(nCount,i++,CTools::StringFromDouble(pOrderData->troubleMoney).c_str());                          //冻结手续费
        m_listDetailCtrl.SetItemText(nCount,i++,strHedgeFlag.c_str());                                                           //投保
        m_listDetailCtrl.SetItemText(nCount,i++,pOrderData->ExchangeID);                                                              //交易所
        m_listDetailCtrl.SetItemText(nCount,i++,pOrderData->OrderRef);                                                                //报单引用
        m_listDetailCtrl.SetItemText(nCount,i++,strOrderType.c_str());                                                                //报单类型  
        m_listDetailCtrl.SetItemText(nCount,i++,pOrderData->UserForceClose?"是":"否");                                                //是否强平
        m_listDetailCtrl.SetItemText(nCount,i++,strForceCloseReason.c_str());                                                    //强平信息

        //绑定
        m_listDetailCtrl.SetItemData(nCount,(DWORD_PTR)&m_mapOrderInfo[pOrderData->InstrumentID]);
    }
}
