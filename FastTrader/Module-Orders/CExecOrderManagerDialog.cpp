#include "StdAfx.h"

#include "CExecOrderManagerDialog.h"
#include "wx\odcombo.h"
//#include "../inc/Module-Misc/FieldValueTool.h"
#include "../inc/Module-Misc/zqControl.h"

extern PlatformSvrMgr* g_pPlatformMgr;
CExecOrderManagerDialog *s_pExecOrderManagerDialog = NULL;

// 查询执行宣告应答，也可以是执行宣告返回
int CExecOrderManagerDialog::RspExecOrderCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data)
{
	if(data.BID!=BID_RspQryExecOrder || data.Size!=sizeof(DataRspQryExecOrder))
		return 0;	

    wxCommandEvent evt(wxEVT_ORDER_INSERT_RspQryExecOrder, wxID_ANY);
    DWORD EvtParamID;
    CFTEventParam::CreateEventParam(EvtParamID,NULL,NULL,&data,sizeof(DataRspQryExecOrder),0);
    evt.SetInt((int)EvtParamID);
    s_pExecOrderManagerDialog->AddPendingEvent(evt);
    return 0;
}

BEGIN_EVENT_TABLE(CExecOrderManagerDialog, wxDialog)
//    EVT_SIZE( CAllOrdersPanel::OnSize )
    EVT_COMMAND(wxID_ANY, wxEVT_ORDER_INSERT_RspQryExecOrder,  CExecOrderManagerDialog::OnRspQryExecOrder)
//
//    EVT_BUTTON(ID_BUTTON_REMOVE,CAllOrdersPanel::OnRemove)
//    EVT_BUTTON(ID_BUTTON_REMOVEALL,CAllOrdersPanel::OnRemoveAll)
    EVT_LIST_EXT_ITEM_ACTIVATED(wxID_ANY, CExecOrderManagerDialog::OnActivated)
    EVT_LIST_EXT_CONTEXT_MENU(CExecOrderManagerDialog::OnContext)
    EVT_COMMAND(wxID_ANY, wxEVT_REMOVE,  CExecOrderManagerDialog::OnRemove)
//    EVT_COMMAND(wxID_ANY, wxEVT_REMOVEALL,  CAllOrdersPanel::OnRemoveAll)
//    EVT_LIST_EXT_COL_END_DRAG(wxID_ANY, OnColumnResize)
//	EVT_COMMAND(wxID_ANY, wxEVT_MA_PLAT_ADD,OnMAPlatformAdd)
//	EVT_COMMAND(wxID_ANY, wxEVT_MA_PLAT_DELETE, OnMAPlatformDelete)
END_EVENT_TABLE()



#define UpdateCol_String(FieldName)  \
            if(!bnewline&&memcmp(pOld->FieldName,pNew->FieldName,sizeof(pOld->FieldName))==0) \
                return false;\
            outItem.SetText(wxString(pNew->FieldName));
#define UpdateCol_String3(FieldName,DispName)  \
            if(!bnewline&&memcmp(pOld->FieldName,pNew->FieldName,sizeof(pOld->FieldName))==0) \
                return false;\
            outItem.SetText(wxString(DispName));
#define UpdateCol_Number(FieldName,DispName)  \
            if(!bnewline&&pOld->FieldName==pNew->FieldName)\
                return false;\
            outItem.SetText(wxString(DispName));\
            outItem.SetiColData((int)(pNew->FieldName));
#define UpdateCol_Price(FieldName)  \
            if(!bnewline&&pOld->FieldName==pNew->FieldName) \
                return false;\
            if(pNew->FieldName==util::GetIntInvalidValue()) outItem.SetText("-");\
            else outItem.SetText(Price2String(pNew->FieldName,pNew->PriceTick));\
            outItem.SetdColData(pNew->FieldName);
#define UpdateCol_Int(FieldName)  \
            if(!bnewline&&pOld->FieldName==pNew->FieldName) \
                return false;\
            if(pNew->FieldName==util::GetIntInvalidValue()) tmpstr.Printf("-");\
            else tmpstr.Printf("%d",pNew->FieldName);\
            outItem.SetText(tmpstr);\
            outItem.SetiColData(pNew->FieldName);
#define UpdateCol_Money(FieldName)  \
            if(!bnewline&&pOld->FieldName==pNew->FieldName) \
                return false;\
            if(pNew->FieldName==util::GetDoubleInvalidValue()) tmpstr.Printf("-");\
            else tmpstr.Printf("%.02lf", pNew->FieldName ); \
            outItem.SetText(tmpstr);\
            outItem.SetdColData(pNew->FieldName);
#define UpdateCol_Fund(FieldName)  \
			if(!bnewline&&pOld->FieldName==pNew->FieldName) \
				return false;\
			if(pNew->FieldName==util::GetDoubleInvalidValue()) tmpstr.Printf("-");\
			else tmpstr = GlobalFunc::GetAccountFormatString(pNew->FieldName, 2); \
			outItem.SetText(tmpstr);\
            outItem.SetdColData(pNew->FieldName);

bool CExecOrderManagerDialog::UpdateListItemCallBack(const void* pNewData,void* pOldData,unsigned long textcol,int FieldID,int ColID,int RowID,bool bnewline,wxExtListItem& outItem)
{
    const PlatformStru_ExecOrderField* pNew=(const PlatformStru_ExecOrderField*)pNewData;
	PlatformStru_ExecOrderField* pOld=(PlatformStru_ExecOrderField*)pOldData;
    bool brlt=false;
    wxString tmpstr;

    outItem.SetColumn(ColID);
    outItem.SetId(RowID);
    //outItem.SetTextColour(wxColor(textcol));
    outItem.SetTextColour(RGB(0,0,0));

    switch(FieldID)
    {
        case EXECORDER_BrokerID:
            if(!bnewline) return false;
            UpdateCol_String(BrokerID);
            return true;
        case EXECORDER_InvestorID:
            if(!bnewline) return false;
            UpdateCol_String(InvestorID);
            return true;
        case EXECORDER_InstrumentID:
            if(!bnewline) return false;
            UpdateCol_String(InstrumentID);
            return true;
        case EXECORDER_ExecOrderRef:
            if(!bnewline) return false;
            UpdateCol_String(ExecOrderRef);
            return true;
        case EXECORDER_UserID:
            if(!bnewline) return false;
            UpdateCol_String(UserID);
            return true;
        case EXECORDER_Volume:
            if(!bnewline) return false;
            UpdateCol_Int(Volume);
            return true;
        case EXECORDER_RequestID:
            if(!bnewline) return false;
            UpdateCol_Int(RequestID);
            return true;
        case EXECORDER_BusinessUnit:
            if(!bnewline) return false;
            UpdateCol_String(BusinessUnit);
            return true;
        case EXECORDER_OffsetFlag:
            if(!bnewline) return false;
            UpdateCol_Number(OffsetFlag,CFieldValueTool::OffsetFlag2String(pNew->OffsetFlag,SVR_LANGUAGE));
            return true;
        case EXECORDER_HedgeFlag:
            if(!bnewline) return false;
            UpdateCol_Number(HedgeFlag,CFieldValueTool::HedgeFlag2String(pNew->HedgeFlag,SVR_LANGUAGE));
            return true;
        case EXECORDER_ActionType:
            if(!bnewline) return false;
            UpdateCol_Number(ActionType,CFieldValueTool::ActionType2String(pNew->ActionType,SVR_LANGUAGE));
            return true;
        case EXECORDER_PosiDirection:
            if(!bnewline) return false;
            UpdateCol_Number(PosiDirection,CFieldValueTool::PosiDirection2String(pNew->PosiDirection,SVR_LANGUAGE));
            if ( pNew->PosiDirection == THOST_FTDC_PD_Long )
                outItem.SetTextColour(wxColor(RGB(255,0,0)));//红色
            else if ( pNew->PosiDirection == THOST_FTDC_PD_Short )
                outItem.SetTextColour(wxColor(RGB(0,128,0)));//绿色
            return true;
        case EXECORDER_ReservePositionFlag:
            if(!bnewline) return false;
            UpdateCol_Number(ReservePositionFlag,CFieldValueTool::ReservePositionFlag2String(pNew->ReservePositionFlag,SVR_LANGUAGE));
            return true;
        case EXECORDER_CloseFlag:
            if(!bnewline) return false;
            UpdateCol_Number(CloseFlag,CFieldValueTool::CloseFlag2String(pNew->CloseFlag,SVR_LANGUAGE));
            return true;
        case EXECORDER_ExecOrderLocalID:
            UpdateCol_String(ExecOrderLocalID);
            return true;
        case EXECORDER_ExchangeID:
            if(!bnewline) return false;
            UpdateCol_String(ExchangeID);
            return true;
        case EXECORDER_ParticipantID:
            if(!bnewline) return false;
            UpdateCol_String(ParticipantID);
            return true;
        case EXECORDER_ClientID:
            if(!bnewline) return false;
            UpdateCol_String(ClientID);
            return true;
        case EXECORDER_ExchangeInstID:
            if(!bnewline) return false;
            UpdateCol_String(ExchangeInstID);
            return true;
        case EXECORDER_TraderID:
            if(!bnewline) return false;
            UpdateCol_String(TraderID);
            return true;
        case EXECORDER_InstallID:
            if(!bnewline) return false;
            UpdateCol_Int(InstallID);
            return true;
        case EXECORDER_OrderSubmitStatus:
            UpdateCol_Number(OrderSubmitStatus,CFieldValueTool::OrderSubmitStatus2String(pNew->OrderSubmitStatus,SVR_LANGUAGE));
            return true;
        case EXECORDER_NotifySequence:
            UpdateCol_Int(NotifySequence);
            return true;
        case EXECORDER_TradingDay:
            if(!bnewline) return false;
            UpdateCol_String(TradingDay);
            return true;
        case EXECORDER_SettlementID:
            UpdateCol_Int(SettlementID);
            return true;
        case EXECORDER_ExecOrderSysID:
            UpdateCol_String(ExecOrderSysID);
            return true;
        case EXECORDER_InsertDate:
            UpdateCol_String(InsertDate);
            return true;
        case EXECORDER_InsertTime:
            UpdateCol_String(InsertTime);
            return true;
        case EXECORDER_CancelTime:
            UpdateCol_String(CancelTime);
            return true;
        case EXECORDER_ExecResult:
            UpdateCol_Number(ExecResult,CFieldValueTool::ExecResult2String(pNew->ExecResult,SVR_LANGUAGE));
            return true;
        case EXECORDER_ClearingPartID:
            UpdateCol_String(ClearingPartID);
            return true;
        case EXECORDER_SequenceNo:
            UpdateCol_Int(SequenceNo);
            return true;
        case EXECORDER_FrontID:
            UpdateCol_Int(FrontID);
            return true;
        case EXECORDER_SessionID:
            UpdateCol_Int(SessionID);
            return true;
        case EXECORDER_UserProductInfo:
            UpdateCol_String(UserProductInfo);
            return true;
        case EXECORDER_StatusMsg:
            UpdateCol_String(StatusMsg);
            return true;
        case EXECORDER_ActiveUserID:
            UpdateCol_String(ActiveUserID);
            return true;
        case EXECORDER_BrokerExecOrderSeq:
            UpdateCol_Int(BrokerExecOrderSeq);
            return true;
	};
	return false;
}

CExecOrderManagerDialog* CExecOrderManagerDialog::CreateDailog(wxWindow* parent)
{
    return new CExecOrderManagerDialog(parent, wxID_ANY, wxT("期权行权管理"));
}

CExecOrderManagerDialog::CExecOrderManagerDialog(wxWindow *parent, wxWindowID id,
             const wxString& title,
             const wxPoint& pos,
             const wxSize& size,
             long style,
             const wxString& name)
:wxDialog(parent,id,title,pos,size,style,name),
m_pMapExecOrder(NULL)
{
	const int nFW = 8, nFH = 20;
	const int nVW = nFW+2, nVH = nFH+8;

	m_baseFTID = 1;
	m_baseUpdateSeq = 1;
	m_pMapExecOrder = new CPlatformDataSetList<ExecOrderKey,PlatformStru_ExecOrderField>(m_baseFTID, m_baseUpdateSeq);

	SetBackgroundColour(DEFAULT_COLOUR);
	wxFont oldfont=GetFont();
	//oldfont.SetPointSize(9);
	oldfont.SetWeight(wxBOLD);
	SetFont(oldfont);
	
	wxPoint ctlPos(5, 5);
	wxSize ctlSz(0,0);

	wxWindow* pWin = NULL;

	ctlPos.y-=nVH/2;

	{		// 交易所
	ctlPos.y += nVH;
	ctlPos.x = 25;

	pWin = new wxStaticText( this, ID_STATIC, LOADSTRING(EXCHANGE), ctlPos, wxSize(nFW*6, nFH));

	wxArrayString strArrList;
	vector<pair<string,vector<PlatformStru_ProductInfo>>> outData;
	// 获取产品列表
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetExchangeID_ProductInfos(outData);
	for(int i=0; i<(int)outData.size(); ++i) {
		strArrList.Add(wxString(outData[i].first.c_str()));
	}
	
	ctlPos.x += nFW*8;
		
	wxOwnerDrawnComboBox* pCmbExchange = new wxOwnerDrawnComboBox(this, ID_EXCHANGE_COMB, wxT(""), 
			wxPoint(ctlPos.x, ctlPos.y), wxSize(nFW*8, nFH), strArrList, wxCB_READONLY);//wxCB_DROPDOWN);//|wxCB_SORT);
	pCmbExchange->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(CExecOrderManagerDialog::OnCmbExchange), NULL, this);
	}

	{			//标的物品种
	ctlPos.x += nFW*8 + 5;

	pWin = new wxStaticText( this, ID_STATIC, LOADSTRING(标的物品种), ctlPos, wxSize(nFW*10, nFH));

	ctlPos.x += nFW*10;

	wxArrayString strArrList;

	wxOwnerDrawnComboBox* pCmbTargetID = new wxOwnerDrawnComboBox(this, ID_TARGETPRODUCT_COMB, wxT(""), 
			ctlPos, wxSize(nFW*8, nFH), strArrList, wxCB_READONLY);
	pCmbTargetID->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(CExecOrderManagerDialog::OnCmbTargetID), NULL, this);
	}

	{			//标的物合约
	ctlPos.x += nFW*8 + 5;

	pWin = new wxStaticText( this, ID_STATIC, LOADSTRING(标的物合约), ctlPos, wxSize(nFW*10, nFH));

	ctlPos.x += nFW*10;

	wxArrayString strArrList;

	wxOwnerDrawnComboBox* pCmbExpireDate = new wxOwnerDrawnComboBox(this, ID_TARGETEXPAIREDATE_COMB, wxT(""), 
			ctlPos, wxSize(nFW*12, nFH), strArrList, wxCB_READONLY);
	pCmbExpireDate->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(CExecOrderManagerDialog::OnCmbExpareDate), NULL, this);
	}

	{			//期权方向
	ctlPos.x += nFW*12 + 5;

	pWin = new wxStaticText( this, ID_STATIC, LOADSTRING(方向), ctlPos, wxSize(nFW*4, nFH));

	ctlPos.x += nFW*4;

	wxArrayString strArrList;
	strArrList.Add(wxT("看涨"));
	strArrList.Add(wxT("看跌"));

	wxOwnerDrawnComboBox* pCmbTargetID = new wxOwnerDrawnComboBox(this, ID_OPTIONDIRECTION_COMB, wxT(""), 
			ctlPos, wxSize(nFW*8, nFH), strArrList, wxCB_READONLY);
	pCmbTargetID->SetSelection(0);
	pCmbTargetID->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(CExecOrderManagerDialog::OnCmbOptionDirection), NULL, this);
	}

	{			//期权合约
	ctlPos.y += nVH;
	ctlPos.x = 25;

	pWin = new wxStaticText( this, ID_STATIC, LOADSTRING(期权合约), ctlPos, wxSize(nFW*8, nFH));

	ctlPos.x += nFW*8;

	wxArrayString strArrList;
	strArrList.Add(wxT(""));

	wxOwnerDrawnComboBox* pCmbTargetID = new wxOwnerDrawnComboBox(this, ID_OPTIONINSTRUMENT_COMB, wxT(""), 
			ctlPos, wxSize(nFW*35, nFH), strArrList, wxCB_READONLY);
	}

	{
	//ctlPos = wxPoint(650 - 150, 90);
	ctlPos.x += nFW*35+2*nFW;
	ctlSz.Set(60, 26);
	wxButton* pBtn = new wxButton( this, ID_QUERY, LOADSTRING("查询"), wxPoint(ctlPos.x, ctlPos.y-5), ctlSz, 0 );		//询价
	pBtn->Connect(wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(CExecOrderManagerDialog::OnBtnQuery), NULL, this);

	//ctlPos = wxPoint(650 - 150 + 70, 90);
	//ctlSz.Set(60, 26);
	//pBtn = new wxButton( this, ID_CANCEL, LOADSTRING(BTN_CLOSE_TEXT), ctlPos, ctlSz, 0 );	//取消
	//pBtn->Connect(wxEVT_COMMAND_BUTTON_CLICKED, 
	//		wxCommandEventHandler(CForOrderInsertDialog::OnBtnClose), NULL, this);
	}


	{			//期权合约
	ctlPos.y += nVH;
	ctlPos.x = 5;
	m_pwxExtListCtrl= new CDataListCtrl<ExecOrderKey,PlatformStru_ExecOrderField>(UpdateListItemCallBack, 
								this, wxID_ANY, ctlPos, wxSize(1024-10-6, 650-100-5),
								wxLCEXT_TOGGLE_COLOUR|wxLCEXT_REPORT|wxLCEXT_VRULES|wxLCEXT_MASK_SORT,
								wxDefaultValidator, wxEmptyString);
	}

	//ResetLanguage();

	SetSize(wxSize(1024, 650));

	this->CenterOnScreen();

	s_pExecOrderManagerDialog = this;

    Init();
	if(DEFAULT_SVR() != NULL) {
        //DEFAULT_SVR()->SubscribeBusinessData(BID_RtnExecOrder, GID_ANY, RspExecOrderCallBackFunc);
        DEFAULT_SVR()->SubscribeBusinessData(BID_RspQryExecOrder, GID_ANY, RspExecOrderCallBackFunc);
    }

}

CExecOrderManagerDialog::~CExecOrderManagerDialog(void)
{
	if(DEFAULT_SVR() != NULL) {
        //DEFAULT_SVR()->UnSubscribeBusinessData(BID_RtnExecOrder, GID_ANY);
        DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspQryExecOrder, GID_ANY);
    }
	if(m_pMapExecOrder != NULL) {
		delete m_pMapExecOrder;
		m_pMapExecOrder = NULL;
	}
}

void CExecOrderManagerDialog::Init()
{
	m_pwxExtListCtrl->Clear();
	if(m_pwxExtListCtrl->GetColumnCount()>0) m_pwxExtListCtrl->DeleteAllColumns();
    //m_FieldID2ColumnID.clear();

    //CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
    //if(!pCfgMgr) return;
    //m_pListCfg = pCfgMgr->GetListCfg(m_nGID);
    //if(!m_pListCfg) return;

    ////设置LIST界面属性
    //m_HeadBackgroundColor = wxColor(m_pListCfg->HeadBackgroundColor);
    //m_HeadColor = wxColor(m_pListCfg->HeadColor);
    //m_BackgroundColor = wxColor(m_pListCfg->BackgroundColor);
    //m_TextColor = wxColor(m_pListCfg->TextColor);
    //m_TextColor2=m_pListCfg->TextColor;
    //m_Font.SetNativeFontInfoUserDesc(m_pListCfg->szFont); 
    //m_EvenLineBgColor=wxColor(m_pListCfg->EvenLineBackgroundColor);
    //m_OddLineBgColor= wxColor(m_pListCfg->OddLineBackgroundColor);

    //m_pwxExtListCtrl->SetHeaderBackgroundColour(m_HeadBackgroundColor);
    //m_pwxExtListCtrl->SetHeaderForegroundColour(m_HeadColor);
    m_pwxExtListCtrl->SetBackgroundColour(RGB(255,255,255));//m_BackgroundColor);
    m_pwxExtListCtrl->SetForegroundColour(RGB(0,0,0));//m_TextColor);
    //m_pwxExtListCtrl->SetFont(m_Font);
    m_pwxExtListCtrl->SetOddRowColour(RGB(221,221,221));//m_OddLineBgColor);
    m_pwxExtListCtrl->SetEvenRowColour(RGB(255,255,255));//m_EvenLineBgColor);

  //  wxExtListItem col;	
  //  int nColCount = pCfgMgr->GetListVisibleColCount(m_nGID);
  //  for(int i = 0; i < nColCount; i++)
  //  {
  //      LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListVisibleColCfg(m_nGID, i);
  //      if(!pColCfg) continue;

  //      //"合约" "报单编号"栏，左对齐
  //      if(pColCfg->id == OPENORDER_OrderLocalID || pColCfg->id == OPENORDER_InstrumentID || 
		//		(pColCfg->id == ALLORDER_InstrumentID) || (pColCfg->id == ALLORDER_OrderLocalID) )
  //          col.SetAlign(wxLISTEXT_FORMAT_LEFT);
  //      else//其余的，右对齐
  //          col.SetAlign(wxLISTEXT_FORMAT_RIGHT);
	
  //      col.SetText(pCfgMgr->GetColCfgCaption(m_nGID,pColCfg->id));
  //      col.SetWidth(pColCfg->Width);
  //      col.SetTextColour(pColCfg->TextColor);
		//col.SetColData(pColCfg->id);

  //      m_pwxExtListCtrl->InsertColumn(i, col); 

  //      m_FieldID2ColumnID[pColCfg->id] = i;
  //  }

    //m_pwxExtListCtrl->SetCfg(m_TextColor2,m_FieldID2ColumnID);



    wxExtListItem col;	
	int i=0;

	col.SetAlign(wxLISTEXT_FORMAT_LEFT);
    //col.SetAlign(wxLISTEXT_FORMAT_RIGHT);
	
    col.SetWidth(80);
    col.SetTextColour(RGB(0,0,0));

	col.SetText(wxT("经纪公司代码"));
	col.SetColData(EXECORDER_BrokerID);
    m_pwxExtListCtrl->InsertColumn(i, col); 
    //m_FieldID2ColumnID[pColCfg->id] = i;
    m_FieldID2ColumnID[EXECORDER_BrokerID] = i;

	i++;
	col.SetText(wxT("投资者代码"));
	col.SetColData(EXECORDER_InvestorID);
    m_pwxExtListCtrl->InsertColumn(i, col); 
    m_FieldID2ColumnID[EXECORDER_InvestorID] = i;

	i++;
	col.SetText(wxT("合约代码"));
	col.SetColData(EXECORDER_InstrumentID);
    m_pwxExtListCtrl->InsertColumn(i, col); 
    m_FieldID2ColumnID[EXECORDER_InstrumentID] = i;

	i++;
	col.SetText(wxT("执行宣告引用"));
	col.SetColData(EXECORDER_ExecOrderRef);
    m_pwxExtListCtrl->InsertColumn(i, col); 
    m_FieldID2ColumnID[EXECORDER_ExecOrderRef] = i;

	i++;
	col.SetText(wxT("用户代码"));
	col.SetColData(EXECORDER_UserID);
    m_pwxExtListCtrl->InsertColumn(i, col); 
    m_FieldID2ColumnID[EXECORDER_UserID] = i;

	i++;
	col.SetText(wxT("数量"));
	col.SetColData(EXECORDER_Volume);
    m_pwxExtListCtrl->InsertColumn(i, col); 
    m_FieldID2ColumnID[EXECORDER_Volume] = i;

	i++;
	col.SetText(wxT("请求编号"));
	col.SetColData(EXECORDER_RequestID);
    m_pwxExtListCtrl->InsertColumn(i, col); 
    m_FieldID2ColumnID[EXECORDER_RequestID] = i;

	i++;
	col.SetText(wxT("业务单元"));
	col.SetColData(EXECORDER_BusinessUnit);
    m_pwxExtListCtrl->InsertColumn(i, col); 
    m_FieldID2ColumnID[EXECORDER_BusinessUnit] = i;

	i++;
	col.SetText(wxT("开平标志"));
	col.SetColData(EXECORDER_OffsetFlag);
    m_pwxExtListCtrl->InsertColumn(i, col); 
    m_FieldID2ColumnID[EXECORDER_OffsetFlag] = i;

	i++;
	col.SetText(wxT("投机套保标志"));
	col.SetColData(EXECORDER_HedgeFlag);
    m_pwxExtListCtrl->InsertColumn(i, col); 
    m_FieldID2ColumnID[EXECORDER_HedgeFlag] = i;

	i++;
	col.SetText(wxT("执行类型"));
	col.SetColData(EXECORDER_ActionType);
    m_pwxExtListCtrl->InsertColumn(i, col); 
    m_FieldID2ColumnID[EXECORDER_ActionType] = i;

	i++;
	col.SetText(wxT("保留头寸申请的持仓方向"));
	col.SetColData(EXECORDER_PosiDirection);
    m_pwxExtListCtrl->InsertColumn(i, col); 
    m_FieldID2ColumnID[EXECORDER_PosiDirection] = i;

	i++;
	col.SetText(wxT("期权行权后是否保留期货头寸的标记"));
	col.SetColData(EXECORDER_ReservePositionFlag);
    m_pwxExtListCtrl->InsertColumn(i, col); 
    m_FieldID2ColumnID[EXECORDER_ReservePositionFlag] = i;

	i++;
	col.SetText(wxT("期权行权后生成的头寸是否自动平仓"));
	col.SetColData(EXECORDER_CloseFlag);
    m_pwxExtListCtrl->InsertColumn(i, col); 
    m_FieldID2ColumnID[EXECORDER_CloseFlag] = i;

	i++;
	col.SetText(wxT("本地执行宣告编号"));
	col.SetColData(EXECORDER_ExecOrderLocalID);
    m_pwxExtListCtrl->InsertColumn(i, col); 
    m_FieldID2ColumnID[EXECORDER_ExecOrderLocalID] = i;

	i++;
	col.SetText(wxT("交易所代码"));
	col.SetColData(EXECORDER_ExchangeID);
    m_pwxExtListCtrl->InsertColumn(i, col); 
    m_FieldID2ColumnID[EXECORDER_ExchangeID] = i;

	i++;
	col.SetText(wxT("会员代码"));
	col.SetColData(EXECORDER_ParticipantID);
    m_pwxExtListCtrl->InsertColumn(i, col); 
    m_FieldID2ColumnID[EXECORDER_ParticipantID] = i;

	i++;
	col.SetText(wxT("客户代码"));
	col.SetColData(EXECORDER_ClientID);
    m_pwxExtListCtrl->InsertColumn(i, col); 
    m_FieldID2ColumnID[EXECORDER_ClientID] = i;

	i++;
	col.SetText(wxT("合约在交易所的代码"));
	col.SetColData(EXECORDER_ExchangeInstID);
    m_pwxExtListCtrl->InsertColumn(i, col); 
    m_FieldID2ColumnID[EXECORDER_ExchangeInstID] = i;

	i++;
	col.SetText(wxT("交易所交易员代码"));
	col.SetColData(EXECORDER_TraderID);
    m_pwxExtListCtrl->InsertColumn(i, col); 
    m_FieldID2ColumnID[EXECORDER_TraderID] = i;

	i++;
	col.SetText(wxT("安装编号"));
	col.SetColData(EXECORDER_InstallID);
    m_pwxExtListCtrl->InsertColumn(i, col); 
    m_FieldID2ColumnID[EXECORDER_InstallID] = i;

	i++;
	col.SetText(wxT("执行宣告提交状态"));
	col.SetColData(EXECORDER_OrderSubmitStatus);
    m_pwxExtListCtrl->InsertColumn(i, col); 
    m_FieldID2ColumnID[EXECORDER_OrderSubmitStatus] = i;

	i++;
	col.SetText(wxT("报单提示序号"));
	col.SetColData(EXECORDER_NotifySequence);
    m_pwxExtListCtrl->InsertColumn(i, col); 
    m_FieldID2ColumnID[EXECORDER_NotifySequence] = i;

	i++;
	col.SetText(wxT("交易日"));
	col.SetColData(EXECORDER_TradingDay);
    m_pwxExtListCtrl->InsertColumn(i, col); 
    m_FieldID2ColumnID[EXECORDER_TradingDay] = i;

	i++;
	col.SetText(wxT("结算编号"));
	col.SetColData(EXECORDER_SettlementID);
    m_pwxExtListCtrl->InsertColumn(i, col); 
    m_FieldID2ColumnID[EXECORDER_SettlementID] = i;

	i++;
	col.SetText(wxT("执行宣告编号"));
	col.SetColData(EXECORDER_ExecOrderSysID);
    m_pwxExtListCtrl->InsertColumn(i, col); 
    m_FieldID2ColumnID[EXECORDER_ExecOrderSysID] = i;

	i++;
	col.SetText(wxT("报单日期"));
	col.SetColData(EXECORDER_InsertDate);
    m_pwxExtListCtrl->InsertColumn(i, col); 
    m_FieldID2ColumnID[EXECORDER_InsertDate] = i;

	i++;
	col.SetText(wxT("插入时间"));
	col.SetColData(EXECORDER_InsertTime);
    m_pwxExtListCtrl->InsertColumn(i, col); 
    m_FieldID2ColumnID[EXECORDER_InsertTime] = i;

	i++;
	col.SetText(wxT("撤销时间"));
	col.SetColData(EXECORDER_CancelTime);
    m_pwxExtListCtrl->InsertColumn(i, col); 
    m_FieldID2ColumnID[EXECORDER_CancelTime] = i;

	i++;
	col.SetText(wxT("执行结果"));
	col.SetColData(EXECORDER_ExecResult);
    m_pwxExtListCtrl->InsertColumn(i, col); 
    m_FieldID2ColumnID[EXECORDER_ExecResult] = i;

	i++;
	col.SetText(wxT("结算会员编号"));
	col.SetColData(EXECORDER_ClearingPartID);
    m_pwxExtListCtrl->InsertColumn(i, col); 
    m_FieldID2ColumnID[EXECORDER_ClearingPartID] = i;

	i++;
	col.SetText(wxT("序号"));
	col.SetColData(EXECORDER_SequenceNo);
    m_pwxExtListCtrl->InsertColumn(i, col); 
    m_FieldID2ColumnID[EXECORDER_SequenceNo] = i;

	i++;
	col.SetText(wxT("前置编号"));
	col.SetColData(EXECORDER_FrontID);
    m_pwxExtListCtrl->InsertColumn(i, col); 
    m_FieldID2ColumnID[EXECORDER_FrontID] = i;

	i++;
	col.SetText(wxT("会话编号"));
	col.SetColData(EXECORDER_SessionID);
    m_pwxExtListCtrl->InsertColumn(i, col); 
    m_FieldID2ColumnID[EXECORDER_SessionID] = i;

	i++;
	col.SetText(wxT("用户端产品信息"));
	col.SetColData(EXECORDER_UserProductInfo);
    m_pwxExtListCtrl->InsertColumn(i, col); 
    m_FieldID2ColumnID[EXECORDER_UserProductInfo] = i;

	i++;
	col.SetText(wxT("状态信息"));
	col.SetColData(EXECORDER_StatusMsg);
    m_pwxExtListCtrl->InsertColumn(i, col); 
    m_FieldID2ColumnID[EXECORDER_StatusMsg] = i;

	i++;
	col.SetText(wxT("操作用户代码"));
	col.SetColData(EXECORDER_ActiveUserID);
    m_pwxExtListCtrl->InsertColumn(i, col); 
    m_FieldID2ColumnID[EXECORDER_ActiveUserID] = i;

	i++;
	col.SetText(wxT("经纪公司报单编号"));
	col.SetColData(EXECORDER_BrokerExecOrderSeq);
    m_pwxExtListCtrl->InsertColumn(i, col); 
    m_FieldID2ColumnID[EXECORDER_BrokerExecOrderSeq] = i;

    m_pwxExtListCtrl->SetCfg(RGB(255,255,255),m_FieldID2ColumnID);

    return ;
}


//----------下面是内部事件响应处理函数，在主线程中执行----------


void CExecOrderManagerDialog::OnRspQryExecOrder(wxCommandEvent& evt)
{
    DataRspQryExecOrder rtnExecOrder;
    DWORD EvtParamID=(DWORD)evt.GetInt();
    if(CFTEventParam::GetEventParam(EvtParamID,NULL,NULL,&rtnExecOrder,NULL,sizeof(DataRspQryExecOrder),NULL))
        CFTEventParam::DeleteEventParam(EvtParamID);
    else return;

	if(rtnExecOrder.RspInfoField.ErrorID==CustomErrorID && rtnExecOrder.bIsLast) {
		//提示查询结果为空
		return;
	}

	if(rtnExecOrder.RspInfoField.ErrorID != 0)
		return;

	ExecOrderKey orderKey(rtnExecOrder.ExecOrderField);

	m_pMapExecOrder->NewReach_Internal(rtnExecOrder.ExecOrderField);

	if(rtnExecOrder.bIsLast) {
		vector<PlatformStru_ExecOrderField> vecAll;
		m_pMapExecOrder->GetAll2_Internal(vecAll);
		m_pwxExtListCtrl->UpdateAllItems(vecAll);
	}
}

void CExecOrderManagerDialog::OnCmbExchange(wxCommandEvent& evt)
{
	//vector<pair<string,vector<PlatformStru_ProductInfo>>> outData;
	//// 获取产品列表
	//if(DEFAULT_SVR()) DEFAULT_SVR()->GetExchangeID_ProductInfos(outData);
	//for(int i=0; i<(int)outData.size(); ++i) {
	//	strArrList.Add(wxString(outData[i].first.c_str()));
	//}
	wxOwnerDrawnComboBox* pCmbExchange = (wxOwnerDrawnComboBox*)FindWindowById(ID_EXCHANGE_COMB, this);
	wxString strExchange = pCmbExchange->GetString(pCmbExchange->GetSelection());
	wxOwnerDrawnComboBox* pCmbTargetProduct = (wxOwnerDrawnComboBox*)FindWindowById(ID_TARGETPRODUCT_COMB, this);
	wxArrayString strArrList;
	vector<string> outProductIDs;

	if(DEFAULT_SVR()) DEFAULT_SVR()->GetOptionProductIDsByExchangeID(string(strExchange.c_str()), outProductIDs);
	for(int i=0; i<(int)outProductIDs.size(); ++i) {
		strArrList.Add(wxString(outProductIDs[i].c_str()));
	}
	pCmbTargetProduct->Clear();
	pCmbTargetProduct->Append(strArrList);

}

void CExecOrderManagerDialog::OnCmbTargetID(wxCommandEvent& evt)
{
	wxOwnerDrawnComboBox* pCmbTargetProduct = (wxOwnerDrawnComboBox*)FindWindowById(ID_TARGETPRODUCT_COMB, this);
	wxString strProduct = pCmbTargetProduct->GetString(pCmbTargetProduct->GetSelection());
	wxOwnerDrawnComboBox* pCmbTargetExpireDate = (wxOwnerDrawnComboBox*)FindWindowById(ID_TARGETEXPAIREDATE_COMB, this);
	wxArrayString strArrList;
	vector<string> outInstrumentIDs;

	if(DEFAULT_SVR()) DEFAULT_SVR()->GetInstrumentListByProductID(string(strProduct.c_str()), outInstrumentIDs);
	for(int i=0; i<(int)outInstrumentIDs.size(); ++i) {
		strArrList.Add(wxString(outInstrumentIDs[i].c_str()));
	}
	pCmbTargetExpireDate->Clear();
	pCmbTargetExpireDate->Append(strArrList);

}

void CExecOrderManagerDialog::OnCmbExpareDate(wxCommandEvent& evt)
{
	wxOwnerDrawnComboBox* pCmbTargetExpireDate = (wxOwnerDrawnComboBox*)FindWindowById(ID_TARGETEXPAIREDATE_COMB, this);
	if(pCmbTargetExpireDate->GetSelection()==-1)
		return;
	wxString strInstrumentID = pCmbTargetExpireDate->GetString(pCmbTargetExpireDate->GetSelection());
	wxOwnerDrawnComboBox* pCmbDirection = (wxOwnerDrawnComboBox*)FindWindowById(ID_OPTIONDIRECTION_COMB, this);
	int nDirection = pCmbDirection->GetSelection();
	wxOwnerDrawnComboBox* pCmbOptionInstrument = (wxOwnerDrawnComboBox*)FindWindowById(ID_OPTIONINSTRUMENT_COMB, this);
	wxArrayString strArrList;
	vector<string> outCallInstrumentIDs;
	vector<string> outPutInstrumentIDs;

	strArrList.Add(wxT(""));
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetOptionInstrumentIDsByUnderlyingInstrumentID(
			strInstrumentID.c_str(), outCallInstrumentIDs, outPutInstrumentIDs);
	if(nDirection==0) {
		for(int i=0; i<(int)outCallInstrumentIDs.size(); ++i) {
			strArrList.Add(wxString(outCallInstrumentIDs[i].c_str()));
		}
	}
	else {
		for(int i=0; i<(int)outPutInstrumentIDs.size(); ++i) {
			strArrList.Add(wxString(outPutInstrumentIDs[i].c_str()));
		}
	}
	pCmbOptionInstrument->Clear();
	pCmbOptionInstrument->Append(strArrList);

}

void CExecOrderManagerDialog::OnCmbOptionDirection(wxCommandEvent& evt)
{
	OnCmbExpareDate(evt);
}

void CExecOrderManagerDialog::OnBtnQuery(wxCommandEvent& evt)
{
	PlatformStru_QryExecOrderField qryExecOrder;

	wxOwnerDrawnComboBox* pCmbInstrument = (wxOwnerDrawnComboBox*)FindWindowById(ID_OPTIONINSTRUMENT_COMB, this);
	if(pCmbInstrument == NULL)
		return;

	if(pCmbInstrument->GetSelection() > 0) {
		wxString strInstrumentID = pCmbInstrument->GetString(pCmbInstrument->GetSelection());
		strncpy(qryExecOrder.InstrumentID, strInstrumentID.c_str(), sizeof(qryExecOrder.InstrumentID)-1);
	}

	if(DEFAULT_SVR() != NULL) {
		if(DEFAULT_SVR()->ReqQryExecOrder(qryExecOrder, 0)==0) {
			if(m_pwxExtListCtrl != NULL)
				m_pwxExtListCtrl->Clear();
			if(m_pMapExecOrder != NULL) {
				m_pMapExecOrder->Clear();
				m_pMapExecOrder->Clear2();
			}
		}
		else {
			// 应提示查询接口有错误发生
		}
	}
}
//
////CommonBtnHeight
//
//void CAllOrdersPanel::OnSize( wxSizeEvent& event )
//{
//	wxRect rt=GetRect();
//    int Pox,Poy;
//
//	if(m_pwxExtListCtrl)
//		m_pwxExtListCtrl->SetSize(0,0,rt.width,rt.height-CommonBtnHeight-2);
//
//    Pox=8;
//    Poy=rt.height-2-CommonBtnHeight;
//
//    if(m_WxRadioButton1)
//    {
//        m_WxRadioButton1->SetPosition(wxPoint(Pox,Poy));
//        Pox+=m_WxRadioButton1->GetRect().GetWidth()+8;
//    }
//    if(m_WxRadioButton2)
//    {
//        m_WxRadioButton2->SetPosition(wxPoint(Pox,Poy));
//        Pox+=m_WxRadioButton2->GetRect().GetWidth()+8;
//    }
//    if(m_WxRadioButton3)
//    {
//        m_WxRadioButton3->SetPosition(wxPoint(Pox,Poy));
//        Pox+=m_WxRadioButton3->GetRect().GetWidth()+8;
//    }
//    if(m_WxRadioButton4)
//    {
//        m_WxRadioButton4->SetPosition(wxPoint(Pox,Poy));
//        Pox+=m_WxRadioButton4->GetRect().GetWidth()+8;
//    }
//
//    Pox+=8;
//
//    if(m_pButtonReQry)
//    {
//        m_pButtonReQry->SetPosition(wxPoint(Pox,Poy));
//        Pox+=m_pButtonReQry->GetRect().GetWidth()+8;
//    }
//
//    Pox+=8;
//
//    if(m_pButtonRemove)
//    {
//        m_pButtonRemove->SetPosition(wxPoint(Pox,Poy));
//        Pox+=m_pButtonRemove->GetRect().GetWidth()+8;
//    }
//    if(m_pButtonRemoveAll)
//    {
//        m_pButtonRemoveAll->SetPosition(wxPoint(Pox,Poy));
//        Pox+=m_pButtonRemoveAll->GetRect().GetWidth()+8;
//    }
//}

/*撤单按钮事件响应*/
void CExecOrderManagerDialog::OnRemove(wxCommandEvent& evt)
{
    int SelectItemId = m_pwxExtListCtrl->GetNextItem(-1, wxLISTEXT_NEXT_ALL, wxLISTEXT_STATE_SELECTED);
    if(SelectItemId<0)
    {
        wxMessageBox(LOADSTRING(SELECT_NOTHING),LOADSTRING(SELECT_TITLE),wxOK|wxICON_ERROR);
        return;
    }

    wxExtListItem  item;
    wxString strMsg,strInstrumentID;
    item.SetId(SelectItemId);
    item.SetMask(wxLISTEXT_MASK_DATA);
    m_pwxExtListCtrl ->GetItem( item );

    PlatformStru_ExecOrderField execOrder;

    if(!m_pwxExtListCtrl->GetValueByRow(SelectItemId, execOrder))
        return;

	PlatformStru_InputExecOrderActionField execOrderAction;
	
	//execOrderAction.ExecOrderActionRef = atoi(execOrder.ExecOrderRef);
	execOrderAction.RequestID = 0;

	strncpy(execOrderAction.ExecOrderRef, execOrder.ExecOrderRef, sizeof(execOrderAction.ExecOrderRef)-1);
	execOrderAction.FrontID = execOrder.FrontID;
	execOrderAction.SessionID = execOrder.SessionID;
	strncpy(execOrderAction.ExchangeID, execOrder.ExchangeID, sizeof(execOrderAction.ExchangeID)-1);
	strncpy(execOrderAction.ExecOrderSysID, execOrder.ExecOrderSysID, sizeof(execOrderAction.ExecOrderSysID)-1);
	strncpy(execOrderAction.InstrumentID, execOrder.InstrumentID, sizeof(execOrderAction.InstrumentID)-1);

	execOrderAction.ActionFlag = THOST_FTDC_AF_Delete;

	if(DEFAULT_SVR() != NULL) {
		if(DEFAULT_SVR()->ReqExecOrderAction(execOrderAction, 0)==0) {
		}
		else {
			// 应提示查询接口有错误发生
		}
	}
}

//
///*全撤按钮事件响应*/
//void CAllOrdersPanel::OnRemoveAll(wxCommandEvent& evt)
//{
//    int SelectItemId = 0, iItemCount = 0;
//    wxExtListItem  item;
//    wxString strMsg,strInstrumentID;
//
//    iItemCount = m_pwxExtListCtrl->GetItemCount();
//    if (iItemCount <= 0) 
//    {
//        wxMessageBox(LOADSTRING(SELECT_NOTHING),LOADSTRING(SELECT_TITLE),wxOK|wxICON_ERROR);
//        return;
//    }
//
//    wxString strmsg,strmsgall,strcaption;
//	strcaption=LOADSTRING(TITLE_CANCELORDER_CONFIRM);
//    vector<PlatformStru_OrderInfo> selectedOrder;//选中的列表项，撤单或全撤
//
//    for ( SelectItemId = 0; SelectItemId < iItemCount; SelectItemId++ )	
//    {
//        item.SetId(SelectItemId);
//        item.SetMask(wxLISTEXT_MASK_DATA);
//        m_pwxExtListCtrl ->GetItem( item );
//
//        PlatformStru_OrderInfo OrderInfo;
//        if(!m_pwxExtListCtrl->GetValueByRow(SelectItemId,OrderInfo))
//            continue;
//
//        IPlatformSingleSvr* pSvr = IS_MULTIACCOUNT_VERSION ? g_pPlatformMgr->GetPlatformSvr(string(OrderInfo.Account)) : DEFAULT_SVR();
//        if(!pSvr) continue;
//
//        switch(OrderInfo.OrderStatus)
//        {
//			case THOST_FTDC_OST_Unknown:
//            case THOST_FTDC_OST_PartTradedQueueing:
//            case THOST_FTDC_OST_NoTradeQueueing:		
//            {
//				PlatformStru_InstrumentInfo SimpleInstrumentInfo;
//	            pSvr->GetInstrumentInfo(OrderInfo.InstrumentID,SimpleInstrumentInfo);
//				std::string stdstrprice=Price2String(OrderInfo.LimitPrice,SimpleInstrumentInfo.PriceTick);
//				strmsg.Printf(LOADFORMATSTRING(CANCELQTYPRC_FORMAT,"%s%s%s%s%s%s%s"),//文本必须包含%s,%s,%s,%s,%s,%s,%s
//						wxString(OrderInfo.OrderSysID).Trim(true).Trim(false).c_str(),
//						CFieldValueTool::OffsetFlag2String(OrderInfo.CombOffsetFlag[0],SVR_LANGUAGE).c_str(),
//						CFieldValueTool::Direction2String(OrderInfo.Direction,SVR_LANGUAGE).c_str(),
//						OrderInfo.InstrumentID,
//						Number2String(OrderInfo.VolumeTotalOriginal).c_str(),
//						stdstrprice.c_str(),
//						CFieldValueTool::HedgeFlag2String(OrderInfo.CombHedgeFlag[0],SVR_LANGUAGE).c_str()
//						); 
//				strmsg = REPLACEFMTSTRING(strmsg.c_str());
//                strmsgall+=strmsg;
//                selectedOrder.push_back(OrderInfo);
//                break;
//            }
//            default:
//                break;
//        }
//    }
//    if(selectedOrder.size()==0) 
//    {
//        wxMessageBox(LOADSTRING(SELECT_NOTHING),LOADSTRING(SELECT_TITLE),wxOK|wxICON_ERROR);
//        return;
//    }
//	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
//	CANCEL_ORDER_CFG cancelOrderCfg;
//	if( pCfgMgr) 
//	{
//		cancelOrderCfg = pCfgMgr->GetCancelOrderCfg(); 
//	}
//	int ret = wxYES;
//	if( cancelOrderCfg.bCancelConfirm )
//	{ 
//		COrderMessageDlg dlg(NULL);
//		dlg.SetTitle(strcaption);
//		strmsgall.Replace("\n", "\r\n");
//		dlg.SetOrderInfo(strmsgall);
//		ret=dlg.ShowModal();
//		USERLOG_INFO("用户单击%s按钮[全撤]并且选择[%s]确认\n\"\n%s\n%s\n\"", 
//				(m_nViewStyle==conOpenOrderStyle?"委托模块":"挂单模块"), 
//				(ret==wxYES?"是":"否"), strcaption, strmsgall);
//	}
//	else
//		USERLOG_INFO("用户单击%s按钮[全撤]并且不需要确认\n\"\n%s\n%s\n\"", 
//				(m_nViewStyle==conOpenOrderStyle?"委托模块":"挂单模块"), 
//				(ret==wxYES?"是":"否"), strcaption, strmsgall);
//
//    if(ret==wxYES)
//    {
//        PlatformStru_InputOrderAction ReqData;
//        vector<PlatformStru_OrderInfo>::iterator it=selectedOrder.begin();
//		if(it!=selectedOrder.end()) 
//        {
//			for(; it!=selectedOrder.end(); ++it)
//			{
//                IPlatformSingleSvr* pSvr = IS_MULTIACCOUNT_VERSION ? g_pPlatformMgr->GetPlatformSvr(string(it->Account)) : DEFAULT_SVR();
//                if(!pSvr) continue;
//
//				ReqData.Thost.FrontID=it->FrontID;
//				ReqData.Thost.SessionID=it->SessionID;
//				memcpy(ReqData.Thost.OrderRef, it->OrderRef, sizeof(it->OrderRef));
//				memcpy(ReqData.Thost.OrderSysID, it->OrderSysID, sizeof(it->OrderSysID));
//				memcpy(ReqData.Thost.ExchangeID, it->ExchangeID, sizeof(it->ExchangeID));
//				memcpy(ReqData.Thost.InstrumentID, it->InstrumentID, sizeof(it->InstrumentID));
//
//				// 把要撤单的保存到Map中
//				string strKey;
//				strKey = ReqData.Thost.ExchangeID;
//				strKey += ",";
//				strKey += ReqData.Thost.OrderSysID;
//				m_mapCancelWait[strKey] = GetTickCount();
//
//				pSvr->ReqOrderAction(ReqData);
//			}
//			selectedOrder.clear();
//		}
//		else 
//        {
//			ShowTradeInfoDlg("OIP_ERROR", "CANCELORDER_ERROE_MULTI", NeedCancelFailConfirm());
//		}
//    }
//}

//双击撤单
void CExecOrderManagerDialog::OnActivated(wxExtListEvent& event)
{
//    CfgMgr *pCfgMgr = CFG_MGR_DEFAULT(); 
//    if( pCfgMgr == NULL) return;
//    CANCEL_ORDER_CFG cancelOrderCfg = pCfgMgr->GetCancelOrderCfg();
//    if(cancelOrderCfg.bDoubleClickMouseCancel)
//    {
//        wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED,ID_BUTTON_REMOVE);
//        ProcessEvent(evt);
//    }
//    event.Skip();
}

void CExecOrderManagerDialog::OnContext(wxContextMenuEvent& evt)
{
    wxMenu menu;
	BOOL bIsRowCLick = evt.GetId();
    int colcount=m_pwxExtListCtrl->GetColumnCount();
    int rowcount=m_pwxExtListCtrl->GetItemCount();
    if(bIsRowCLick)
    {
		int SelectItemId = m_pwxExtListCtrl->GetNextItem(-1, wxLISTEXT_NEXT_ALL, wxLISTEXT_STATE_SELECTED);
	    if(SelectItemId>=0)
		{
			wxExtListItem  item;
			item.SetId(SelectItemId);
			item.SetMask(wxLISTEXT_MASK_DATA);
			m_pwxExtListCtrl ->GetItem( item );

			PlatformStru_ExecOrderField execOrder;
			if(m_pwxExtListCtrl->GetValueByRow(SelectItemId, execOrder))
			{
				switch(execOrder.ExecResult)
				{
					case THOST_FTDC_OER_NoExec:
					case THOST_FTDC_OER_Unknown:
						{    
							menu.Append(ID_MENU_REMOVE,LOADSTRING(CONTEXTMENU_REMOVE));
							//menu.Append(ID_MENU_REMOVEALL,LOADSTRING(CONTEXTMENU_REMOVEALL));
							//menu.Append(ID_MENU_CHANGEORDER,LOADSTRING(CONTEXTMENU_CHANGEORDER));
							//menu.AppendSeparator();
						}
						break;
					default:
						break;
				}
			}
        }
    }

	//menu.Append(ID_MENU_AUTO_ADJUST_COL_WIDTH, LOADSTRING(CONTEXTMENU_AUTO_ADJUST_COL_WIDTH));
	//menu.Append(ID_MENU_EXPORT,LOADSTRING(CONTEXTMENU_EXPORT));
	//menu.Append(ID_MENU_COLUMNCONFIG,LOADSTRING(CONTEXTMENU_COLUMNCONFIG));
	//if ( bIsRowCLick )
	//{
	//	menu.AppendSeparator();
	//	INSERT_ADDTOMYSELF_ITEM(menu);
	//}

    if(menu.GetMenuItemCount()>0&&DEFAULT_SVR())
    {
        POINT pt;
        ::GetCursorPos(&pt);
        BOOL retcmd=zqPopupMenu(&menu,pt.x,pt.y,this);

		////加入自选
		//CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
		//if( retcmd - ID_MENU_ADDTOMYSELF >= 0 )
		//{
		//	bool bAdded = false;
		//	for ( int i = 0; i < m_pwxExtListCtrl->GetItemCount(); i++)
		//	{
		//		if ( m_pwxExtListCtrl->IsSelected( i ))
		//		{			
  //                  PlatformStru_OrderInfo OrderInfo;
  //                  if(!m_pwxExtListCtrl->GetValueByRow(i,OrderInfo))
  //                      continue;

		//			string InstrumentId = OrderInfo.InstrumentID;
		//			if(pCfgMgr->GetInstrumentGroupMemberCount(retcmd - ID_MENU_ADDTOMYSELF)>=DEFAULT_SVR()->GetGroupMaxContractNum())
		//			{
		//				wxMessageBox(LOADSTRING(OVER_LIMIT_OF_GROUP),LOADSTRING(USERLOGINDLG_ERROR),wxOK|wxICON_QUESTION);
		//				break;
		//			}
		//			else
		//			{
		//				pCfgMgr->AddInstrument( retcmd - ID_MENU_ADDTOMYSELF, InstrumentId.c_str()); 
		//				bAdded = true;					
		//			}
		//		}
		//	}

		//	if ( bAdded ){SEND_CONFIG_CHANGED_EVENT(MSG_INSTRUMENT_GROUP_CHANGE);}
		//	return;
		//}

        switch(retcmd)
        {
		//case ID_MENU_AUTO_ADJUST_COL_WIDTH:
		//	m_pwxExtListCtrl->AutoAdjustColumnWidth();
		//	SaveColWidth();
		//	break;
        case ID_MENU_REMOVE://撤单
            {
                wxCommandEvent evt(wxEVT_REMOVE);	
                this->AddPendingEvent(evt);
            }
            break;
   //     case ID_MENU_REMOVEALL://全撤
   //         {
   //             wxCommandEvent evt(wxEVT_REMOVEALL);	
   //             this->AddPendingEvent(evt);
   //         }
   //         break;
   //     case ID_MENU_CHANGEORDER://快速改单
			//{
			//	int SelectItemId = m_pwxExtListCtrl->GetNextItem(-1, wxLISTEXT_NEXT_ALL, wxLISTEXT_STATE_SELECTED);
			//	if(SelectItemId<0)
			//		return;

			//	PlatformStru_OrderInfo OrderInfo;

			//	if(m_pwxExtListCtrl->GetValueByRow(SelectItemId,OrderInfo)) 
			//	{
			//		wxCommandEvent request_event;
			//		if(DEFAULT_SVR()->HaveOrderType(UIOT_CMD_REPLACEORDER))
			//			request_event.SetEventType(wxEVT_ORDERINSERT_ORDERREPLACE);
			//		else
			//			request_event.SetEventType(wxEVT_ORDERINSERT_ORDERCHANGE);
			//		wxString strCmsStream;
			//		strCmsStream.Printf("%s,%s,%s",OrderInfo.InvestorID, OrderInfo.ExchangeID, wxString(OrderInfo.OrderSysID));
			//		if(1)
			//		{
			//			DWORD EvtParamID;
			//			std::string EvtParamStr(strCmsStream.c_str());
			//			if(CFTEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,2))
			//			{
			//				request_event.SetInt((int)EvtParamID);
			//				GETTOPWINDOW()->AddPendingEvent(request_event);
			//			}
			//		}
			//	}
   //         }
   //         break;  
   //     case ID_MENU_EXPORT://导出列表
   //         {
   //             wxString cap;
   //             switch(m_ShowType)
   //             {
   //                 case ALLORDER:
   //                     cap=LOADSTRING(ALL_ORDER);
   //                     break;
   //                 case OPENORDER:
   //                     cap=LOADSTRING(OPEN_ORDER);
   //                     break;
   //                 case TRADEDORDER:
   //                     cap=LOADSTRING(OIP_FILLED);
   //                     break;
   //                 case CANCELORDER:
   //                     cap=LOADSTRING(CANCEL_FAIL_ORDER);
   //                     break;
   //             }
   //				LIST_EXPORT_CSV_FILE(cap, m_pwxExtListCtrl);
   //         }
   //         break;
   //     case ID_MENU_COLUMNCONFIG://表格列设置
   //         switch(m_nViewStyle) 
   //         { 
   //         case conAllOrderStyle:
   //             SEND_LOAD_CONFIG_APP_EVENT(ID_ITEM_ALLORDER);
   //             break;
   //         case conOpenOrderStyle:
   //             SEND_LOAD_CONFIG_APP_EVENT(ID_ITEM_WAITTRADE);
   //             break;
   //         }
   //         break;
        default:
            break;
        }
    }
    evt.Skip();
}

//void CAllOrdersPanel::OnColumnResize( wxExtListEvent& event )
//{
//	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
//	if ( NULL == pCfgMgr )
//	{
//		return;
//	}
//
//	int nCol = event.GetColumn();
//	if ( nCol < 0 || nCol > pCfgMgr->GetListVisibleColCount( m_nGID ))
//	{
//		return;
//	}
//
//	LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListVisibleColCfg(m_nGID, nCol);
//	pColCfg->Width = m_pwxExtListCtrl->GetColumnWidth( nCol );
//}
//
//
//void CAllOrdersPanel::SaveColWidth()
//{
//	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
//	if ( NULL == pCfgMgr || NULL == m_pwxExtListCtrl )
//	{
//		return;
//	}
//
//	int nColCount = m_pwxExtListCtrl->GetColumnCount();
//	if ( nColCount != pCfgMgr->GetListVisibleColCount( m_nGID ))
//	{
//		return;
//	}
//
//	for ( int i = 0; i < nColCount; i++)
//	{
//		LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListVisibleColCfg(m_nGID, i);
//		pColCfg->Width = m_pwxExtListCtrl->GetColumnWidth( i );
//	}
//}
//
//void CAllOrdersPanel::OnMAPlatformAdd(wxCommandEvent& event)
//{
//	wxCommandEvent myEvent(wxEVT_SUBSCRIBE);
//	myEvent.SetInt(1);
//	ProcessEvent(myEvent);
//	m_pwxExtListCtrl->Clear();
//	ShowAll();
//
//}
//void CAllOrdersPanel::OnMAPlatformDelete(wxCommandEvent& evt)
//{
//	wxCommandEvent myEvent(wxEVT_SUBSCRIBE);
//	myEvent.SetInt(0);
//	ProcessEvent(myEvent);
//	myEvent.SetInt(1);
//	ProcessEvent(myEvent);
//	m_pwxExtListCtrl->Clear();
//	ShowAll();
//}
