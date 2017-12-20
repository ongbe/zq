#include "StdAfx.h"
#include "CEditOrderChangeDailog.h"
#include "../inc/Module-Misc/tools_util.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"
using namespace util;
extern PlatformSvrMgr* g_pPlatformMgr;

BEGIN_EVENT_TABLE(CEditOrderChangeDailog, wxDialog)
    EVT_BUTTON(ID_EDITORDERCHANGE_OK,OnOk)
    EVT_BUTTON(ID_EDITORDERCHANGE_CANCEL,OnCancel)
	EVT_CHAR_HOOK(OnDialogCharHook)
	EVT_CHAR_EX(OnDialogCharHook)
	EVT_WINDOW_CREATE(OnCreateWindow)
	EVT_WINDOW_DESTROY(OnDestroyWindow)
	EVT_SPIN_UP(ID_EDITORDERCHANGE_NEWPRICE_SPINCTRL, OnPriceSpinUp)
	EVT_SPIN_DOWN(ID_EDITORDERCHANGE_NEWPRICE_SPINCTRL, OnPriceSpinDown)
	EVT_SPIN_UP(ID_EDITORDERCHANGE_NEWVOLUME_SPINCTRL, OnVolumeSpinUp)
	EVT_SPIN_DOWN(ID_EDITORDERCHANGE_NEWVOLUME_SPINCTRL, OnVolumeSpinDown)
END_EVENT_TABLE()
//////////////////////////////////////////////////////////////////////////////////////

CEditOrderChangeDailog::CEditOrderChangeDailog(wxWindow *parent, wxWindowID id,
             const wxString& title,
             const wxPoint& pos,
             const wxSize& size,
             long style,
             const wxString& name):
	wxDialog(parent,id,title,pos,size,style,name),
	m_fPriceTick(1.0f),
	m_nNewVolume(0),
	m_bNewIsMarketPrice(FALSE),
	m_fNewPrice(0.0f)
{
	DIALOG_CREATE_LOG();
	m_UpperLimitPrice = 0.0;
	m_LowerLimitPrice = 0.0;

	ZeroMemory(&m_tftdcOrderField, sizeof(m_tftdcOrderField));

	SetBackgroundColour(DEFAULT_COLOUR);
//	wxFont oldfont=GetFont();
//	oldfont.SetWeight(wxBOLD);
//	SetFont(oldfont);

	wxPoint posInit(5, 5);
	int nFW = 8, nFH = 16;
	int nVW = nFW+2, nVH = nFH+2+4;
	wxPoint ctlPos(0, 0);
	wxSize ctlSz(0, 0);

	wxWindow* itemTemp = NULL;

	ctlPos = wxPoint(posInit.x, posInit.y);
	ctlSz.Set(nFW*10, nFH);
	itemTemp = new wxStaticText(this, ID_EDITORDERCHANGE_STATIC1, wxEmptyString, ctlPos, ctlSz, wxALIGN_CENTER_HORIZONTAL);//单号: 1

	ctlPos = wxPoint(posInit.x, posInit.y+nVH);
	ctlSz.Set(nFW*10, nFH);
	itemTemp = new wxStaticText(this, ID_EDITORDERCHANGE_STATIC2, wxEmptyString, ctlPos, ctlSz);//合约: 2

	ctlPos = wxPoint(posInit.x, posInit.y+nVH*2);
	ctlSz.Set(nFW*10, nFH);
	itemTemp = new wxStaticText(this, ID_EDITORDERCHANGE_STATIC3, wxEmptyString, ctlPos, ctlSz, wxALIGN_CENTER_HORIZONTAL);//买卖: 3

	ctlPos = wxPoint(posInit.x+nFW*11, posInit.y+nVH*2);
	ctlSz.Set(nFW*10, nFH);
	itemTemp = new wxStaticText(this, ID_EDITORDERCHANGE_STATIC4, wxEmptyString, ctlPos, ctlSz, wxALIGN_CENTER_HORIZONTAL);//投保: 4
	if ( DEFAULT_SVR()->GetPlatformType() == PTYPE_PATSAPI )
	{
		itemTemp->Show(false);
	}

	ctlPos = wxPoint(posInit.x, posInit.y+nVH*3);
	ctlSz.Set(nFW*10, nFH);
	itemTemp = new wxStaticText(this, ID_EDITORDERCHANGE_STATIC5, wxEmptyString, ctlPos, ctlSz, wxALIGN_CENTER_HORIZONTAL);//开平: 6
	if ( DEFAULT_SVR()->GetPlatformType() == PTYPE_PATSAPI )
	{
		itemTemp->Show(false);
	}

	ctlPos = wxPoint(posInit.x, posInit.y+nVH*4);
	ctlSz.Set(nFW*4+nFW/2, nFH);
	itemTemp = new wxStaticText(this, ID_EDITORDERCHANGE_STATIC6, LOADSTRING(TITLE_PRICE), ctlPos, ctlSz, wxALIGN_LEFT);//价格: 

	ctlPos.x += (nFW*4)+nFW/2;
//	ctlPos.y += nVH;
	CreateFloatSpinCtrl_Price(ctlPos);

	ctlPos = wxPoint(posInit.x, posInit.y+nVH*5);
	ctlSz.Set(nFW*4+nFW/2, nFH);
	itemTemp = new wxStaticText(this, ID_EDITORDERCHANGE_STATIC7, LOADSTRING(TITLE_QTY), ctlPos, ctlSz, wxALIGN_LEFT);//数量: 

	ctlPos.x += (nFW*4)+nFW/2;
//	ctlPos.y += nVH;
	CreateFloatSpinCtrl_Volume(ctlPos);


	posInit.x += nFW*2;
	posInit.y += 24*6;

	ctlPos = wxPoint(posInit.x, posInit.y-8);
	ctlSz.Set(60, 26);
	itemTemp = new wxButton( this, ID_EDITORDERCHANGE_OK, LOADSTRING(ASKNAMEDLG_OK), ctlPos, ctlSz, 0 );//确定

	ctlPos = wxPoint(posInit.x+nFW*8, posInit.y-8);
	ctlSz.Set(60, 26);
	itemTemp = new wxButton( this, ID_EDITORDERCHANGE_CANCEL, LOADSTRING(ASKNAMEDLG_CANCEL), ctlPos, ctlSz, 0 );//取消

	SetSize(wxSize(180, 24*8+10));

	m_TabOrderVector.clear();
	//tab order
	m_TabOrderVector.push_back(ID_EDITORDERCHANGE_NEWPRICE_TEXT);
	m_TabOrderVector.push_back(ID_EDITORDERCHANGE_NEWVOLUME_TEXT);

	m_TabOrderVector.push_back(ID_EDITORDERCHANGE_OK);
    m_TabOrderVector.push_back(ID_EDITORDERCHANGE_CANCEL);

}

CEditOrderChangeDailog::~CEditOrderChangeDailog()
{
}

wxTextCtrl* CEditOrderChangeDailog::CreateFloatSpinCtrl_Price(wxPoint& ctrPosInit)
{
	wxPoint ctlPos(0, 0);
	wxSize ctlSz(0, 0);

	ctlPos=ctrPosInit;

	ctlSz.Set(100, 22);
	wxTextCtrl* itemTextCtrl3 = new wxTextCtrl( this, ID_EDITORDERCHANGE_NEWPRICE_TEXT, wxEmptyString, ctlPos, ctlSz, 0 );

//	itemTextCtrl3->Connect(ID_OIJSD_PRICE_TEXT, wxEVT_LEFT_UP, 
//			wxMouseEventHandler(COrderInputPanel_jsd::OnPriceClicked), NULL, this);

	ctlPos.x += 100;
	ctlSz.Set(16, 22);
    wxSpinButton* itemSpinButton4 = new wxSpinButton( this, ID_EDITORDERCHANGE_NEWPRICE_SPINCTRL, ctlPos, ctlSz, wxSP_VERTICAL );
    itemSpinButton4->SetRange(0, 1000000);
    itemSpinButton4->SetValue(0);
    return itemTextCtrl3;
}

wxTextCtrl* CEditOrderChangeDailog::CreateFloatSpinCtrl_Volume(wxPoint& ctrPosInit)
{
	wxPoint ctlPos(0, 0);
	wxSize ctlSz(0, 0);

	ctlPos=ctrPosInit;

	ctlSz.Set(100, 22);
    wxTextCtrl* itemTextCtrl3 = 
			new wxTextCtrl(this, ID_EDITORDERCHANGE_NEWVOLUME_TEXT, wxEmptyString, ctlPos, ctlSz, 0);

//	itemTextCtrl3->Connect(ID_OIJSD_VOLUME_TEXT, wxEVT_LEFT_UP, 
//			wxMouseEventHandler(COrderInputPanel_jsd::OnVolumeClicked), NULL, this);

	ctlPos.x += 100;
	ctlSz.Set(16, 22);
    wxSpinButton* itemSpinButton4 = 
			new wxSpinButton( this, ID_EDITORDERCHANGE_NEWVOLUME_SPINCTRL, ctlPos, ctlSz, wxSP_VERTICAL );
    itemSpinButton4->SetRange(0, 1000000);
    itemSpinButton4->SetValue(0);
    return itemTextCtrl3;
}

void CEditOrderChangeDailog::ShowOrderReq()
{

	PlatformStru_DepthMarketData field;
	ZeroMemory(&field, sizeof(field));
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetQuotInfo(m_tftdcOrderField.InstrumentID,field)!=0) {
    }
	m_UpperLimitPrice = field.UpperLimitPrice;
	m_LowerLimitPrice = field.LowerLimitPrice;

	wxWindow* pwinItem=NULL;
	wxString strValue;

	pwinItem=FindWindowById(ID_EDITORDERCHANGE_STATIC1, this);
	strValue.Printf(LOADFORMATSTRING(EOCD_ORDERNO,"%s"), m_tftdcOrderField.OrderSysID);//文本必须包含%s
	if(pwinItem) pwinItem->SetLabel(strValue);

	pwinItem=FindWindowById(ID_EDITORDERCHANGE_STATIC2, this);
	strValue.Printf(LOADFORMATSTRING(EOCD_CONTACT,"%s"), m_tftdcOrderField.InstrumentID);//文本必须包含%s
	if(pwinItem) pwinItem->SetLabel(strValue);

	pwinItem=FindWindowById(ID_EDITORDERCHANGE_STATIC3, this);

	if(pwinItem) pwinItem->SetLabel(
		m_tftdcOrderField.Direction==THOST_FTDC_D_Buy ? LOADSTRING(EOCD_BUYSELL_B) :LOADSTRING(EOCD_BUYSELL_S)
		);
	pwinItem=FindWindowById(ID_EDITORDERCHANGE_STATIC4, this);
	if(pwinItem) pwinItem->SetLabel(
		m_tftdcOrderField.CombHedgeFlag[0]==THOST_FTDC_BHF_Speculation ?LOADSTRING(EOCD_HEDG_S) :LOADSTRING(EOCD_HEDG_H)
		);


	pwinItem=FindWindowById(ID_EDITORDERCHANGE_STATIC5, this);
	if(pwinItem) 
		pwinItem->SetLabel(
		m_tftdcOrderField.CombOffsetFlag[0]==THOST_FTDC_OF_Open ? LOADSTRING(EOCD_OCT_O)  : 
				(m_tftdcOrderField.CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday ?LOADSTRING(EOCD_OCT_T) : LOADSTRING(EOCD_OCT_C))
				);



	if(m_tftdcOrderField.OrderPriceType==THOST_FTDC_OPT_AnyPrice) {
		strValue=LOADSTRING(OLS_MARKET);
	}
	else
		strValue = Price2String(m_tftdcOrderField.LimitPrice, m_fPriceTick).c_str();
	pwinItem=FindWindowById(ID_EDITORDERCHANGE_NEWPRICE_TEXT, this);
	if(pwinItem) pwinItem->SetLabel(strValue);

	pwinItem=FindWindowById(ID_EDITORDERCHANGE_NEWVOLUME_TEXT, this);
	strValue.Printf("%d", m_tftdcOrderField.VolumeTotal);
	if(pwinItem) pwinItem->SetLabel(strValue);

}

void CEditOrderChangeDailog::SetPrice(double fPrice)
{
	wxWindow* window=FindWindowById(ID_EDITORDERCHANGE_NEWPRICE_TEXT,this);
	wxString str;
	if(fPrice==-1||util::isInvalidValue(fPrice)) 
		str="-";
	window->SetLabel(Price2String(fPrice, m_fPriceTick).c_str());
}

double CEditOrderChangeDailog::GetPrice()
{
	wxWindow* window=FindWindowById(ID_EDITORDERCHANGE_NEWPRICE_TEXT,this);
	wxString str=window->GetLabel();
	double t=0;
	str.ToDouble(&t);
	return t;
}

// 价格是否为市价
BOOL CEditOrderChangeDailog::GetIsPriceMode()
{
	wxWindow* window=FindWindowById(ID_EDITORDERCHANGE_NEWPRICE_TEXT,this);
	wxString str=window->GetLabel();
	return str.compare(LOADSTRING(OLS_MARKET))==0;
}

void CEditOrderChangeDailog::SetQty(int qty)
{
	wxString strText;
	strText.Printf("%d", qty);
    wxWindow* window=FindWindowById(ID_EDITORDERCHANGE_NEWVOLUME_TEXT,this);
	window->SetLabel(strText);
}

int CEditOrderChangeDailog::GetQty()
{
	wxString strText;
    wxWindow* window=FindWindowById(ID_EDITORDERCHANGE_NEWVOLUME_TEXT,this);
	strText = window->GetLabel();
	return atoi(strText.c_str());
}

bool CEditOrderChangeDailog::Show( bool show )
{
	if(show) {
		ShowOrderReq();
		wxString strText;
		strText.Printf(LOADFORMATSTRING(EOCD_FCOSID,"%s"), m_tftdcOrderField.OrderSysID);//文本必须包含%s
		this->SetLabel(strText);
	}
    return wxDialog::Show(show);
}

wxString CEditOrderChangeDailog::GetStaticText(UINT nUIID)
{
	wxString strText;
    wxWindow* pWin=FindWindowById(nUIID, this);
	if(pWin!=NULL)
		strText = pWin->GetLabel();
	return strText;
}

void CEditOrderChangeDailog::OnOk(wxCommandEvent& event)
{
	m_bNewIsMarketPrice = GetIsPriceMode();
	m_fNewPrice = GetPrice();
	if(CheckSumPrice(m_fNewPrice, m_fPriceTick)) {
		wxMessageDialog dialog(NULL,LOADSTRING(CHECKSUM_PRICE_NORMAL),
				LOADSTRING(OIP_ERROR),wxNO_DEFAULT|wxOK|wxICON_ERROR);
		//SetInputPrice(fPrice);
		dialog.ShowModal();
		return;
	}
	m_nNewVolume = GetQty();

	if(m_nNewVolume<=0)
	{
		wxMessageDialog dialog(NULL,LOADSTRING(OIP_QTYBIGZERO),
				LOADSTRING(OIP_ERROR),wxNO_DEFAULT|wxOK|wxICON_ERROR);
		dialog.ShowModal();
		return;
	}

	USERLOG_INFO("快速改单，用户在对话框中单击按钮[确认]确认\n\"\n%s\n%s\n%s\n%s\n%s\n%s\n%s=[%s]\n%s=[%s]\n\"", 
			GetTitle(), 
			GetStaticText(ID_EDITORDERCHANGE_STATIC1).c_str(), 
			GetStaticText(ID_EDITORDERCHANGE_STATIC2).c_str(), 
			GetStaticText(ID_EDITORDERCHANGE_STATIC3).c_str(), 
			GetStaticText(ID_EDITORDERCHANGE_STATIC4).c_str(), 
			GetStaticText(ID_EDITORDERCHANGE_STATIC5).c_str(),
			GetStaticText(ID_EDITORDERCHANGE_STATIC6).c_str(),
			GetStaticText(ID_EDITORDERCHANGE_NEWPRICE_TEXT).c_str(),
			GetStaticText(ID_EDITORDERCHANGE_STATIC7).c_str(),
			GetStaticText(ID_EDITORDERCHANGE_NEWVOLUME_TEXT).c_str());

	EndModal(wxID_OK);
}

void CEditOrderChangeDailog::OnCancel(wxCommandEvent& event)
{
	EndModal(wxID_CANCEL);
}

void CEditOrderChangeDailog::OnCreateWindow(wxWindowCreateEvent& evt)
{
}

void CEditOrderChangeDailog::OnDestroyWindow(wxWindowDestroyEvent& evt)
{
}

void CEditOrderChangeDailog::OnDialogCharHook(wxKeyEvent& evt)
{
    
	wxSpinEvent evtSpin;
	wxKeyEvent* pEvent=&evt;//(wxKeyEvent*)evt.GetClientData();
    if(!pEvent) return;
    wxWindow *win = FindFocus();
    if(win == NULL) {
		evt.Skip();
        return;
    }
	pEvent->SetId(win->GetId());
    if (pEvent->GetKeyCode()==WXK_TAB || pEvent->GetKeyCode()==VK_TAB)
    {
        bool backward = pEvent->ShiftDown();
        int id,thisID=win->GetId(),count=m_TabOrderVector.size();
        for(id=0;id<count;id++)
        {
            if(m_TabOrderVector[id]==thisID)
            {
                if(backward)
                    id=id>0?id-1:count-1;
                else
                    id=id<count-1?id+1:0;
                FindWindowById(m_TabOrderVector[id],this)->SetFocus();
                return;
            }
        }
    }
    else if(pEvent->GetKeyCode()==WXK_UP || pEvent->GetKeyCode()==VK_UP)
    {
        int id,thisID=win->GetId(),count=m_TabOrderVector.size();
        for(id=0;id<count;id++)
        {
            if(m_TabOrderVector[id]==thisID)
            {
                if(id>0)
                {
                    FindWindowById(m_TabOrderVector[id-1],this)->SetFocus();
                   return;
                }
            }
        }
    }
    else if(pEvent->GetKeyCode()==WXK_DOWN || pEvent->GetKeyCode()==VK_DOWN)
    {
        int id,thisID=win->GetId(),count=m_TabOrderVector.size();
        for(id=0;id<count;id++)
        {
            if(m_TabOrderVector[id]==thisID)
            {
                if(id<count-1)
                {
                    FindWindowById(m_TabOrderVector[id+1],this)->SetFocus();
                    return;
                }
            }
        }
    }
	else if(evt.GetKeyCode()==WXK_LEFT || evt.GetKeyCode()==VK_LEFT) {
		switch(evt.GetId()) {
		case ID_EDITORDERCHANGE_NEWPRICE_TEXT:
			evtSpin.SetId(ID_EDITORDERCHANGE_NEWPRICE_SPINCTRL);
			OnPriceSpinDown(evtSpin);
			return;
			break;
		case ID_EDITORDERCHANGE_NEWVOLUME_TEXT:
			evtSpin.SetId(ID_EDITORDERCHANGE_NEWVOLUME_SPINCTRL);
			OnVolumeSpinDown(evtSpin);
			return;
			break;
		default:
			evt.Skip();
			return;
			break;
		}
	}
	else if(evt.GetKeyCode()==WXK_RIGHT || evt.GetKeyCode()==VK_RIGHT) {
		switch(evt.GetId()) {
		case ID_EDITORDERCHANGE_NEWPRICE_TEXT:
			evtSpin.SetId(ID_EDITORDERCHANGE_NEWPRICE_SPINCTRL);
			OnPriceSpinUp(evtSpin);
			return;
			break;
		case ID_EDITORDERCHANGE_NEWVOLUME_TEXT:
			evtSpin.SetId(ID_EDITORDERCHANGE_NEWVOLUME_SPINCTRL);
			OnVolumeSpinUp(evtSpin);
			return;
			break;
		default:
			evt.Skip();
			return;
			break;
		}
	}
	else {
		evt.Skip();
	}
}

void CEditOrderChangeDailog::OnPriceSpinUp(wxSpinEvent& event)
{
    double fPrice=GetPrice();
	CheckSumPrice(fPrice, m_fPriceTick, 1);
	fPrice = AnalysePrice(fPrice, m_UpperLimitPrice, m_LowerLimitPrice);
    SetPrice(fPrice);
}

void CEditOrderChangeDailog::OnPriceSpinDown(wxSpinEvent& event)
{
    double fPrice=GetPrice();
	CheckSumPrice(fPrice, m_fPriceTick, -1);
	fPrice = AnalysePrice(fPrice, m_UpperLimitPrice, m_LowerLimitPrice);
    SetPrice(fPrice);
}

void CEditOrderChangeDailog::OnVolumeSpinUp(wxSpinEvent& event)
{
	int nVolume = GetQty();
	if(nVolume<9999)
		nVolume++;
	SetQty(nVolume);
}

void CEditOrderChangeDailog::OnVolumeSpinDown(wxSpinEvent& event)
{
	int nVolume = GetQty();
	if(nVolume>0)
		nVolume--;
	SetQty(nVolume);
}
