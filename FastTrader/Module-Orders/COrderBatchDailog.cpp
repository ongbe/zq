#include "StdAfx.h"
#include "COrderBatchDailog.h"
#include "../inc/Module-Misc2/SimpleWriteLog.h"
//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;


BEGIN_EVENT_TABLE(COrderBatchDailog, wxDialog)
    EVT_BUTTON(ID_OBATCH2_IDPAUSE,OnPause)
    EVT_BUTTON(ID_OBATCH2_IDCANCEL,OnCancel)
	EVT_CLOSE(OnClose)
    EVT_TIMER(ID_OBATCH2_TIMER, OnTimer)
END_EVENT_TABLE()
//////////////////////////////////////////////////////////////////////////////////////

//int COrderBatchDailog::sm_nGroupNum=0;

COrderBatchDailog::COrderBatchDailog(wxWindow *parent, wxWindowID id,
             const wxString& title,
             const wxPoint& pos,
             const wxSize& size,
             long style,
             const wxString& name):
	wxDialog(parent,id,title,pos,size,style,name),
	m_poLocalOrderService(NULL),
	//m_ReqID(0),
	m_timer(this,ID_OBATCH2_TIMER),
	m_nPriceType(0),
	m_nHedgeStatus(0),
	m_nBatchEachNum(0),
	m_nTimerInterval(0),
	m_nBatchCount(0),
	m_nBatchEndNum(0),
	m_nSendSum(0),
	m_nSuccessSum(0),
	m_nTimerOut(1),
	m_dwTickCountBak(0),
	m_bIsPause(FALSE)
{
	DIALOG_CREATE_LOG();
    AUTO_SETSIZE();
	CLocalOrderService::sm_nBatchOrderNum++;
	m_nGroupNum = CLocalOrderService::sm_nBatchOrderNum;
	
	ZeroMemory(&m_OrderReq, sizeof(CThostFtdcInputOrderField));

	wxPoint posInit(5, 5);
	int nFW = 8, nFH = 16;
	int nVW = nFW+2, nVH = nFH+2;
	wxPoint ctlPos(0, 0);
	wxSize ctlSz(0, 0);

	wxWindow* itemTemp = NULL;

	ctlPos = wxPoint(posInit.x, posInit.y);
	ctlSz.Set(nFW*10, nFH);
	itemTemp = new wxStaticText(this, ID_OBATCH2_STATIC1, wxEmptyString, ctlPos, ctlSz, wxALIGN_CENTER_HORIZONTAL);//组号: 1

	ctlPos = wxPoint(posInit.x, posInit.y+nVH);
	ctlSz.Set(nFW*10, nFH);
	itemTemp = new wxStaticText(this, ID_OBATCH2_STATIC2, wxEmptyString, ctlPos, ctlSz);//合约: 2

	ctlPos = wxPoint(posInit.x, posInit.y+nVH*2);
	ctlSz.Set(nFW*10, nFH);
	itemTemp = new wxStaticText(this, ID_OBATCH2_STATIC3, wxEmptyString, ctlPos, ctlSz, wxALIGN_CENTER_HORIZONTAL);//买卖: 3

	ctlPos = wxPoint(posInit.x, posInit.y+nVH*3);
	ctlSz.Set(nFW*10, nFH);
	itemTemp = new wxStaticText(this, ID_OBATCH2_STATIC4, wxEmptyString, ctlPos, ctlSz, wxALIGN_CENTER_HORIZONTAL);//开平: 4
	if ( DEFAULT_SVR()->GetPlatformType() == PTYPE_PATSAPI )
	{
		itemTemp->Show(false);
	}

	ctlPos = wxPoint(posInit.x, posInit.y+nVH*4);
	ctlSz.Set(nFW*10, nFH);
	itemTemp = new wxStaticText(this, ID_OBATCH2_STATIC5, wxEmptyString, ctlPos, ctlSz, wxALIGN_CENTER_HORIZONTAL);//投保: 5
	if ( DEFAULT_SVR()->GetPlatformType() == PTYPE_PATSAPI )
	{
		itemTemp->Show(false);
	}

	ctlPos = wxPoint(posInit.x, posInit.y+nVH*5);
	ctlSz.Set(nFW*10, nFH);
	itemTemp = new wxStaticText(this, ID_OBATCH2_STATIC6, wxEmptyString, ctlPos, ctlSz, wxALIGN_RIGHT);//数量: 6

	ctlPos = wxPoint(posInit.x, posInit.y+nVH*6);
	ctlSz.Set(nFW*10, nFH);
	itemTemp = new wxStaticText(this, ID_OBATCH2_STATIC7, wxEmptyString, ctlPos, ctlSz, wxALIGN_RIGHT);//价格: 7

	posInit.x += nFW*10;
	
	ctlPos = wxPoint(posInit.x, posInit.y);
	ctlSz.Set(nFW*4, nFH);
	itemTemp = new wxStaticText(this, wxID_ANY, LOADSTRING(BATCHDLG_SUM), ctlPos, ctlSz, wxALIGN_RIGHT);//共
	ctlPos = wxPoint(posInit.x+nFW*4, posInit.y);
	ctlSz.Set(nFW*4, nFH);
	itemTemp = new wxStaticText(this, ID_OBATCH2_STATICTOTAL, _("0"), ctlPos, ctlSz, wxALIGN_RIGHT);
	ctlPos = wxPoint(posInit.x+nFW*4+nFW*4, posInit.y);
	ctlSz.Set(nFW*2, nFH);
	itemTemp = new wxStaticText(this, wxID_ANY, LOADSTRING(BATCHDLG_BATCH), ctlPos, ctlSz, wxALIGN_RIGHT);//批

	ctlPos = wxPoint(posInit.x, posInit.y+nVH*2);
	ctlSz.Set(nFW*4, nFH);
	itemTemp = new wxStaticText(this, wxID_ANY, LOADSTRING(BATCHDLG_ATNUM), ctlPos, ctlSz, wxALIGN_RIGHT);//到第
	ctlPos = wxPoint(posInit.x+nFW*4, posInit.y+nVH*2);
	ctlSz.Set(nFW*4, nFH);
	itemTemp = new wxStaticText(this, ID_OBATCH2_STATICINDEX, _("1"), ctlPos, ctlSz, wxALIGN_RIGHT);
	ctlPos = wxPoint(posInit.x+nFW*4+nFW*4, posInit.y+nVH*2);
	ctlSz.Set(nFW*2, nFH);
	itemTemp = new wxStaticText(this, wxID_ANY, LOADSTRING(BATCHDLG_BATCH), ctlPos, ctlSz, wxALIGN_RIGHT);//批

	posInit.x += nFW*12;
	
	ctlPos = wxPoint(posInit.x, posInit.y);
	ctlSz.Set(nFW*8, nFH);
	itemTemp = new wxStaticText(this, wxID_ANY, LOADSTRING(OIP_SENDED), ctlPos, ctlSz, wxALIGN_RIGHT);//已发送
	ctlPos = wxPoint(posInit.x+nFW*8, posInit.y);
	ctlSz.Set(nFW*4, nFH);
	itemTemp = new wxStaticText(this, ID_OBATCH2_STATICSENDTOTAL, _("0"), ctlPos, ctlSz, wxALIGN_RIGHT);
	ctlPos = wxPoint(posInit.x+nFW*8+nFW*4, posInit.y);
	ctlSz.Set(nFW*2, nFH);
	itemTemp = new wxStaticText(this, wxID_ANY, LOADSTRING(BATCHDLG_BATCH), ctlPos, ctlSz, wxALIGN_RIGHT);//批

	ctlPos = wxPoint(posInit.x, posInit.y+nVH);
	ctlSz.Set(nFW*8, nFH);
	itemTemp = new wxStaticText(this, wxID_ANY, LOADSTRING(OIP_SNEDOK), ctlPos, ctlSz, wxALIGN_RIGHT);//发送成功
	ctlPos = wxPoint(posInit.x+nFW*8, posInit.y+nVH);
	ctlSz.Set(nFW*4, nFH);
	itemTemp = new wxStaticText(this, ID_OBATCH2_STATICSUCCESS, _("0"), ctlPos, ctlSz, wxALIGN_RIGHT);
	ctlPos = wxPoint(posInit.x+nFW*8+nFW*4, posInit.y+nVH);
	ctlSz.Set(nFW*2, nFH);
	itemTemp = new wxStaticText(this, wxID_ANY, LOADSTRING(BATCHDLG_BATCH), ctlPos, ctlSz, wxALIGN_RIGHT);//批

	ctlPos = wxPoint(posInit.x, posInit.y+nVH*2);
	ctlSz.Set(nFW*8, nFH);
	itemTemp = new wxStaticText(this, wxID_ANY, LOADSTRING(BATCHDLG_REMIAN), ctlPos, ctlSz, wxALIGN_RIGHT);//发送还有
	ctlPos = wxPoint(posInit.x+nFW*8, posInit.y+nVH*2);
	ctlSz.Set(nFW*4, nFH);
	itemTemp = new wxStaticText(this, ID_OBATCH2_STATICTIMER, _("0"), ctlPos, ctlSz, wxALIGN_RIGHT);
	ctlPos = wxPoint(posInit.x+nFW*8+nFW*4, posInit.y+nVH*2);
	ctlSz.Set(nFW*2, nFH);
	itemTemp = new wxStaticText(this, wxID_ANY, LOADSTRING(BATCHDLG_SECOND), ctlPos, ctlSz, wxALIGN_RIGHT);//秒


	posInit.x -= nFW*4;

	ctlPos = wxPoint(posInit.x, posInit.y+nVH*4);
	ctlSz.Set(88, 24);
	itemTemp = new wxButton( this, ID_OBATCH2_IDPAUSE, LOADSTRING(OIP_PAUSE), ctlPos, ctlSz, 0 );//暂停

	ctlPos = wxPoint(posInit.x, posInit.y+nVH*6);
	ctlSz.Set(88, 24);
	itemTemp = new wxButton( this, ID_OBATCH2_IDCANCEL, LOADSTRING(ASKNAMEDLG_CANCEL), ctlPos, ctlSz, 0 );//取消

	posInit.x = 5;
	posInit.y = 5+nVH*8;

}

COrderBatchDailog::~COrderBatchDailog()
{
}

void COrderBatchDailog::ShowOrderReq()
{
	wxWindow* pwinItem=NULL;
	wxString strValue;
	pwinItem=FindWindowById(ID_OBATCH2_STATIC1, this);
	strValue.Printf(LOADFORMATSTRING(OBD_GROUPNO,"%d"), m_nGroupNum);//文本必须包含%d
	if(pwinItem) pwinItem->SetLabel(strValue);
    //
	pwinItem=FindWindowById(ID_OBATCH2_STATIC2, this);
	strValue.Printf(LOADFORMATSTRING(EOCD_CONTACT,"%s"), m_OrderReq.InstrumentID);//文本必须包含%s
	if(pwinItem) pwinItem->SetLabel(strValue);
    //
	pwinItem=FindWindowById(ID_OBATCH2_STATIC3, this);
	if(pwinItem) pwinItem->SetLabel(
		m_OrderReq.Direction==THOST_FTDC_D_Buy ? LOADSTRING(EOCD_BUYSELL_B) :LOADSTRING(EOCD_BUYSELL_S)
		);
    //
	pwinItem=FindWindowById(ID_OBATCH2_STATIC4, this);
	if(pwinItem) 
		pwinItem->SetLabel(
		m_OrderReq.CombOffsetFlag[0]==THOST_FTDC_OF_Open ? LOADSTRING(EOCD_OCT_O)  : 
				(m_OrderReq.CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday ?LOADSTRING(EOCD_OCT_T) : LOADSTRING(EOCD_OCT_C))
				);
	//
	pwinItem=FindWindowById(ID_OBATCH2_STATIC5, this);
	if(pwinItem) pwinItem->SetLabel(
		m_OrderReq.CombHedgeFlag[0]==THOST_FTDC_BHF_Speculation ?LOADSTRING(EOCD_HEDG_S) :LOADSTRING(EOCD_HEDG_H)
		);
	//
	pwinItem=FindWindowById(ID_OBATCH2_STATIC6, this);		
	strValue.Printf(LOADFORMATSTRING(OBD_QTY,"%d"), m_OrderReq.VolumeTotalOriginal);//文本必须包含%d
	if(pwinItem) pwinItem->SetLabel(strValue);
	//
	pwinItem=FindWindowById(ID_OBATCH2_STATIC7, this);
	if(m_OrderReq.OrderPriceType==THOST_FTDC_OPT_AnyPrice) {
			strValue=LOADSTRING(OBD_MKTPRC);
	}
	else {
		strValue.Printf(LOADFORMATSTRING(OBD_LMTPRC,"%f"), m_OrderReq.LimitPrice);//文本必须包含%f
	}

	if(pwinItem) pwinItem->SetLabel(strValue);

}

bool COrderBatchDailog::Show( bool show )
{

    if(show) {
	    m_nBatchCount = (m_OrderReq.VolumeTotalOriginal+m_nBatchEachNum-1)/m_nBatchEachNum;
	    m_nBatchEndNum = m_OrderReq.VolumeTotalOriginal%m_nBatchEachNum;
	    if(m_nBatchEndNum == 0) m_nBatchEndNum = m_nBatchEachNum;


	    wxArrayString arrString;
	    for(int i=0; i<m_nBatchCount+1; i++)
		    arrString.Add(_(""));

	    wxPoint posInit(5, 5);
	    int nFW = 8, nFH = 16;
	    int nVW = nFW+2, nVH = nFH+2;
	    wxPoint ctlPos(0, 0);
	    wxSize ctlSz(0, 0);
	    ctlPos = wxPoint(posInit.x, posInit.y+nVH*8);
	    ctlSz.Set(nFW*36, nFH*4);
	    wxListBox* itemTemp = new wxListBox( this, ID_OBATCH2_LISTMSG, ctlPos, ctlSz, arrString);

	    wxWindow* pwinItem = NULL;
	    wxString strValue;

	    strValue.Printf("%d", m_nBatchCount);
	    pwinItem=FindWindowById(ID_OBATCH2_STATICTOTAL, this);
	    if(pwinItem) pwinItem->SetLabel(strValue);

	    ShowOrderReq();

	    //行情更新定时器
        m_timer.Start(100);
    }
    return wxDialog::Show(show);
}

void COrderBatchDailog::OnTimer(wxTimerEvent& event)
{
    if(m_nSendSum>=m_nBatchCount) {
        m_timer.Stop();
        return;
    }
	
    if(m_bIsPause)
		return;	
	PlatformStru_DepthMarketData field;
	PlatformStru_InputOrder req=m_OrderReq;

    wxString strName=req.InstrumentID;
	int ret=0;

	wxWindow* pwinItem = NULL;
	wxString strValue;

	DWORD dwTick = GetTickCount();
	if(m_dwTickCountBak/1000 != dwTick/1000) {
		m_nTimerOut--;
		m_dwTickCountBak = dwTick; 
		strValue.Printf("%d", m_nTimerOut);
		pwinItem=FindWindowById(ID_OBATCH2_STATICTIMER, this);
		if(pwinItem) pwinItem->SetLabel(strValue);
	}

	bool bPriceIsInvalid = false;

	if(m_nTimerOut<=0) {
		switch(m_nPriceType) {
		case 0:
			//if(req.OrderPriceType!=THOST_FTDC_OPT_AnyPrice) {
			//}
			break;
		case 1:
			memset(&field,0,sizeof(field));
			if(DEFAULT_SVR()&&DEFAULT_SVR()->GetQuotInfo(strName.c_str(), field)==0) {
				if(!(bPriceIsInvalid=util::isInvalidValue(field.LastPrice))) {
					//价格类型
					req.OrderPriceType=THOST_FTDC_OPT_LimitPrice;
					//有效期类型
					req.TimeCondition=THOST_FTDC_TC_GFD;
					req.LimitPrice = field.LastPrice;
				}
			}
			break;
		case 2:
			memset(&field,0,sizeof(field));
			if(DEFAULT_SVR()&&DEFAULT_SVR()->GetQuotInfo(strName.c_str(), field)==0) {
				if(req.Direction == THOST_FTDC_D_Buy) {
					if(!(bPriceIsInvalid=util::isInvalidValue(field.AskPrice1))) {
						//价格类型
						req.OrderPriceType=THOST_FTDC_OPT_LimitPrice;
						//有效期类型
						req.TimeCondition=THOST_FTDC_TC_GFD;
						req.LimitPrice = field.AskPrice1;
					}
				}
				else {
					if(!(bPriceIsInvalid=util::isInvalidValue(field.BidPrice1))) {
						//价格类型
						req.OrderPriceType=THOST_FTDC_OPT_LimitPrice;
						//有效期类型
						req.TimeCondition=THOST_FTDC_TC_GFD;
						req.LimitPrice = field.BidPrice1;
					}
				}
			}
			break;
		};
		if(!bPriceIsInvalid) {
			if(m_nSendSum<m_nBatchCount-1)
				req.VolumeTotalOriginal = m_nBatchEachNum;
			else
				req.VolumeTotalOriginal = m_nBatchEndNum;
			
			ret = VerifyOrder(req);

			CLocalOrderService::BATCHSUBORDER tOrder={0};

			tOrder.nGroupNum = m_nGroupNum;
			tOrder.nSubNum = m_nSendSum+1;
			tOrder.nRegNum = req.RequestID;	//COrderServiceThread::sm_nReqNum;
			tOrder.nRegState = ret==0 ? conReqSendOk : conReqSendFail;
			tOrder.req = req;
			if(m_poLocalOrderService) {
				m_poLocalOrderService->LockObject();
				m_poLocalOrderService->OrderBatchSubAdd(tOrder);
				m_poLocalOrderService->UnlockObject();

				if(GETTOPWINDOW()) {
					wxCommandEvent evtnew(wxEVT_QUERY_BATCH_REQUERY, GetId());
					evtnew.SetExtraLong(tOrder.nNum);
					GETTOPWINDOW()->AddPendingEvent(evtnew);
				}
			}

			m_OrderReq = req;
			m_nSendSum++;

			strValue.Printf("%d", m_nSendSum);
			pwinItem=FindWindowById(ID_OBATCH2_STATICSENDTOTAL, this);
			if(pwinItem) pwinItem->SetLabel(strValue);
			strValue.Printf("%d", m_nSendSum+1);
			pwinItem=FindWindowById(ID_OBATCH2_STATICINDEX, this);
			if(pwinItem) pwinItem->SetLabel(strValue);
			
			if(ret==0) {
				m_nSuccessSum++;
				strValue.Printf("%d", m_nSuccessSum);
				pwinItem=FindWindowById(ID_OBATCH2_STATICSUCCESS, this);
				if(pwinItem) pwinItem->SetLabel(strValue);
				wxListBox* pwinList=(wxListBox*)FindWindowById(ID_OBATCH2_LISTMSG, this);
				if(pwinList) {
					strValue.Printf(LOADFORMATSTRING(BATCH_NUM_OK_FORMAT,"%d"), m_nSendSum);//文本必须包含%d
					pwinList->SetString((unsigned int)m_nSendSum-1, strValue.c_str());
					pwinList->SetSelection(m_nSendSum-1);
				}
			}
			else {
				//COrderServiceThread::sm_nReqNum--;
				wxListBox* pwinList=(wxListBox*)FindWindowById(ID_OBATCH2_LISTMSG, this);
				if(pwinList) {
					strValue.Printf(LOADFORMATSTRING(BATCH_NUM_FAIL_FORMAT,"%d%s"), //文本必须包含%d,%s
						m_nSendSum, CFieldValueTool::ReturnCode2String(ret,SVR_LANGUAGE).c_str());
					pwinList->SetString((unsigned int)m_nSendSum-1, strValue.c_str());
					pwinList->SetSelection(m_nSendSum-1);
				}
			}
			m_nTimerOut = m_nTimerInterval;
		}
		else {
			m_nTimerOut++;
			wxListBox* pwinList=(wxListBox*)FindWindowById(ID_OBATCH2_LISTMSG, this);
			if(pwinList) {
				pwinList->SetString((unsigned int)m_nSendSum, LOADSTRING(BATCH_ORDER_INVALIDPRICE));//"买卖价或最新价无效，本次批量下单推迟，直到价格有效为止");
				pwinList->SetSelection(m_nSendSum);
			}
		}
	}

	if(req.OrderPriceType==THOST_FTDC_OPT_AnyPrice) {
		strValue =LOADSTRING(OBD_MKTPRC);
	}
	else {
		strValue.Printf(LOADFORMATSTRING(OBD_LMTPRC_STR,"%s"), Double2String(m_OrderReq.LimitPrice).c_str());//文本必须包含%s
	}
	pwinItem=FindWindowById(ID_OBATCH2_STATIC7, this);

	if(pwinItem) pwinItem->SetLabel(strValue);

	if(m_nSendSum>=m_nBatchCount) {
		m_timer.Stop();
		pwinItem=FindWindowById(ID_OBATCH2_STATICINDEX, this);
		if(pwinItem) pwinItem->SetLabel(_(""));
		pwinItem=FindWindowById(ID_OBATCH2_STATICTIMER, this);
		if(pwinItem) pwinItem->SetLabel(_(""));
		pwinItem=FindWindowById(ID_OBATCH2_IDPAUSE, this);
		if(pwinItem) pwinItem->Enable(false);
		pwinItem=FindWindowById(ID_OBATCH2_IDCANCEL, this);
		if(pwinItem) pwinItem->SetLabel(LOADSTRING(BTN_CLOSE_TEXT));

		wxListBox* pwinList=(wxListBox*)FindWindowById(ID_OBATCH2_LISTMSG, this);
		if(pwinList) {
			strValue=LOADSTRING(BATCH_ALL_SEND_FINISH);
			pwinList->SetString((unsigned int)m_nSendSum, strValue.c_str());
			pwinList->SetSelection(m_nSendSum);
		}

	}

}

// 仅此用于日志文件输出
wxString COrderBatchDailog::GetUserViewText()
{
	wxString strRet;
	wxWindow* pwinItem=NULL;

    pwinItem=FindWindowById(ID_OBATCH2_STATIC1, this);
    if(pwinItem) 
		strRet += pwinItem->GetLabel();
	strRet += ", \t";

    pwinItem=FindWindowById(ID_OBATCH2_STATIC2, this);
    if(pwinItem) 
		strRet += pwinItem->GetLabel();
	strRet += ", \t";

    pwinItem=FindWindowById(ID_OBATCH2_STATIC3, this);
    if(pwinItem) 
		strRet += pwinItem->GetLabel();
	strRet += ", \t";

    pwinItem=FindWindowById(ID_OBATCH2_STATIC4, this);
    if(pwinItem) 
		strRet += pwinItem->GetLabel();
	strRet += ", \t";

    pwinItem=FindWindowById(ID_OBATCH2_STATIC5, this);
    if(pwinItem) 
		strRet += pwinItem->GetLabel();
	strRet += ", \t";

    pwinItem=FindWindowById(ID_OBATCH2_STATIC6, this);
    if(pwinItem) 
		strRet += pwinItem->GetLabel();
	strRet += ", \t";

    pwinItem=FindWindowById(ID_OBATCH2_STATIC7, this);
    if(pwinItem) 
		strRet += pwinItem->GetLabel();
	strRet += ", \t";

    pwinItem=FindWindowById(ID_OBATCH2_STATICTOTAL, this);
	if(pwinItem) {
		strRet += "共";
		strRet += pwinItem->GetLabel();
		strRet += "批";
	}
	strRet += ", \t";

    pwinItem=FindWindowById(ID_OBATCH2_STATICSENDTOTAL, this);
	if(pwinItem) {
		strRet += "已发送";
		strRet += pwinItem->GetLabel();
		strRet += "批";
	}
	strRet += ", \t";

    pwinItem=FindWindowById(ID_OBATCH2_STATICSUCCESS, this);
	if(pwinItem) {
		strRet += "发送成功";
		strRet += pwinItem->GetLabel();
		strRet += "批";
	}
	strRet += ", \t";

    pwinItem=FindWindowById(ID_OBATCH2_STATICINDEX, this);
	if(pwinItem) {
		strRet += "到第";
		strRet += pwinItem->GetLabel();
		strRet += "批";
	}
	strRet += ", \t";

    pwinItem=FindWindowById(ID_OBATCH2_STATICTIMER, this);
	if(pwinItem) {
		strRet += "发送还有";
		strRet += pwinItem->GetLabel();
		strRet += "秒";
	}

	return strRet;
}

void COrderBatchDailog::OnPause(wxCommandEvent& event)
{

	USERLOG_INFO("用户单击批量下单窗口中[%s]按钮，\r\n“%s”\r\n", 
        (m_bIsPause ? LOADSTRING(OIP_CONTINUE) : LOADSTRING(OIP_PAUSE)), GetUserViewText().c_str());

	m_bIsPause = !m_bIsPause;
	wxWindow* pwinItem = NULL;
	pwinItem=FindWindowById(ID_OBATCH2_IDPAUSE, this);
	if(pwinItem) pwinItem->SetLabel(m_bIsPause ? LOADSTRING(OIP_CONTINUE) : LOADSTRING(OIP_PAUSE));

}

void COrderBatchDailog::OnCancel(wxCommandEvent& event)
{
	USERLOG_INFO("用户单击批量下单窗口中[取消]按钮，\r\n“%s”\r\n", 
        GetUserViewText().c_str());

	m_timer.Stop();
	Destroy();
}

void COrderBatchDailog::OnClose(wxCloseEvent& evt)
{
	USERLOG_INFO("用户单击批量下单窗口中[关闭]按钮，\r\n“%s”\r\n", 
        GetUserViewText().c_str());

	m_timer.Stop();
	Destroy();
	evt.Skip();
}
