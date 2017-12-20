#include "StdAfx.h"

#include "../inc/Quotation/KLineView.h"
#include "../inc/Quotation/TechIndexConfigDlg.h"
#include "wx/dcmirror.h"
#define  ID_PHRASE_1MIN				23114
#define  ID_PHRASE_5MIN				23115
#define  ID_PHRASE_15MIN			23116
#define  ID_PHRASE_30MIN			23117
#define  ID_PHRASE_60MIN			23118
#define  ID_PHRASE_DAY				23119
#define  ID_PHRASE_WEEK				23120
#define  ID_PHRASE_MONTH			23121

#define  ID_CROSS_SHOW				23134

#define  ID_DRAW_TRIANGLE_UP		23135
#define  ID_DRAW_TRIANGLE_DOWN		23136

#define  ID_DEL_MARK				23137
#define  ID_TECHINDEX_ADJUSTPARA	23139

#define  ID_TECHINDEX_MA			23142
#define  ID_TECHINDEX_KDJ			23143
#define  ID_TECHINDEX_MACD			23144
#define  ID_TECHINDEX_BOLL			23145
#define  ID_TECHINDEX_UOS			23146
#define  ID_TECHINDEX_BIAS			23147
#define  ID_TECHINDEX_DMI			23148
#define  ID_TECHINDEX_PSY			23149
#define  ID_TECHINDEX_ROC			23150


#define  ID_RETURN_TLINE			23161
#define  ID_RETURN_BAOJIABIAO		23162

DEFINE_EVENT_TYPE(wxEVT_KLINEVIEW_MOUSEMOVEHOOK)
HHOOK g_hMouse=NULL; //鼠标的钩子过程
wxPanel *g_pWnd = NULL;

DEFINE_EVENT_TYPE(wxEVT_RETURN_TLINE)
DEFINE_EVENT_TYPE(wxEVT_RETURN_BAOJIABIAO)
IMPLEMENT_CLASS(CKLineView, wxPanel)
CKLineView::CKLineView(void)
{
	m_enumPhrase    = PHRASE_DAY;
	m_pKLineCtrl	= NULL;
	m_pKVolumeCtrl	= NULL;
	m_pKTechIndex	= NULL;
	m_pKScalesCtrl  = NULL;
	m_splitter		= NULL;
	m_splitterTop   = NULL;
	m_splitterBottom= NULL;

	m_pFData = NULL;
	m_pRightUpWnd   = NULL;
	m_bInit = false;
}

CKLineView::~CKLineView(void)
{
	UnhookWindowsHookEx(g_hMouse);
}
void CKLineView::ClearMemory_Mark()
{
	if(m_pFData == NULL)
		return;
	if(m_pKLineCtrl)
		m_pKLineCtrl->SetMarks(m_pFData->m_mapMarks);
	if(m_pKVolumeCtrl)
		m_pKVolumeCtrl->SetMarks(m_pFData->m_mapMarks);
	if(m_pKTechIndex)
		m_pKTechIndex->SetMarks(m_pFData->m_mapMarks);
}
BEGIN_EVENT_TABLE(CKLineView, wxPanel)
EVT_PAINT(CKLineView::OnPaint)
EVT_SIZE(CKLineView::OnSize)
EVT_COMMAND(wxID_ANY, wxEVT_KCtrl_KeyDown, CKLineView::OnFuncKeyDown)
EVT_COMMAND(wxID_ANY, wxEVT_KCtrl_LBUTTONDOWN, CKLineView::OnFuncLButtonDown)
EVT_COMMAND(wxID_ANY, wxEVT_KCtrl_RBUTTONUP, CKLineView::OnFuncRButtonUp)
EVT_COMMAND(wxID_ANY, wxEVT_KCtrl_MOUSELEAVE, CKLineView::OnFuncMouseLeave)
EVT_COMMAND(wxID_ANY, wxEVT_KCtrl_MOUSEENTER, CKLineView::OnFuncMouseEnter)
EVT_COMMAND(wxID_ANY, wxEVT_KCtrl_LEFTDBCLICK, OnFuncLDBClick)

EVT_MENU(ID_PHRASE_1MIN,	CKLineView::PhraseMin1)
EVT_MENU(ID_PHRASE_5MIN,	CKLineView::PhraseMin5)
EVT_MENU(ID_PHRASE_15MIN,	CKLineView::PhraseMin15)
EVT_MENU(ID_PHRASE_30MIN,	CKLineView::PhraseMin30)
EVT_MENU(ID_PHRASE_60MIN,	CKLineView::PhraseMin60)
EVT_MENU(ID_PHRASE_DAY,		CKLineView::PhraseDay)
EVT_MENU(ID_PHRASE_WEEK,	CKLineView::PhraseWeek)
EVT_MENU(ID_PHRASE_MONTH,	CKLineView::PhraseMonth)


EVT_MENU(ID_CROSS_SHOW,	CKLineView::CrossShow)
EVT_MENU(ID_DRAW_TRIANGLE_UP,	CKLineView::MarkTriangleUp)
EVT_MENU(ID_DRAW_TRIANGLE_DOWN,	CKLineView::MarkTriangleDown)
EVT_MENU(ID_TECHINDEX_ADJUSTPARA,	CKLineView::OnTechIndexAdjustPara)

EVT_MENU(ID_TECHINDEX_MA,	CKLineView::OnMA)
EVT_MENU(ID_TECHINDEX_KDJ,	CKLineView::OnKDJ)
EVT_MENU(ID_TECHINDEX_MACD,	CKLineView::OnMACD)
EVT_MENU(ID_TECHINDEX_BOLL,	CKLineView::OnBOLL)
EVT_MENU(ID_TECHINDEX_UOS,	CKLineView::OnUOS)
EVT_MENU(ID_TECHINDEX_BIAS,	CKLineView::OnBIAS)
EVT_MENU(ID_TECHINDEX_DMI,	CKLineView::OnDMI)
EVT_MENU(ID_TECHINDEX_PSY,	CKLineView::OnPSY)
EVT_MENU(ID_TECHINDEX_ROC,	CKLineView::OnROC)
EVT_MENU(ID_DEL_MARK,	CKLineView::DelMark)

EVT_MENU(ID_RETURN_TLINE,	OnReturnTLine)
EVT_MENU(ID_RETURN_BAOJIABIAO,	OnReturnBaoJiaBiao)


EVT_COMMAND(wxID_ANY, wxEVT_KLINEVIEW_MOUSEMOVEHOOK, CKLineView::OnFuncMouseMoveHook)

EVT_COMMAND(wxID_ANY, wxEVT_KCtrl_MARKSCHANGE, CKLineView::OnMarksChange)
//EVT_COMMAND(wxID_ANY, wxEVT_KCtrl_MARKSCHANGE, CKLineView::OnMarksChange)
//EVT_COMMAND(wxID_ANY, wxEVT_KCtrl_MARKSCHANGE, CKLineView::OnMarksChange)
EVT_LEAVE_WINDOW(OnMouseLeave)
EVT_ENTER_WINDOW(OnMouseEnter)



END_EVENT_TABLE()

LRESULT CALLBACK MouseProc( int nCode,WPARAM wParam, LPARAM lParam)
{
	if(wParam == WM_MOUSEMOVE)
	{
		MOUSEHOOKSTRUCT *mhookstruct; //鼠标HOOK结构体 
		mhookstruct = (MOUSEHOOKSTRUCT*)lParam; 
		POINT pt = mhookstruct->pt; 
		if(g_pWnd != NULL)
		{		
			wxCommandEvent myEvent(wxEVT_KLINEVIEW_MOUSEMOVEHOOK);
			myEvent.SetClientData((void*)&pt);
			g_pWnd->ProcessEvent(myEvent);
		}	
	}
	return CallNextHookEx(g_hMouse,nCode,wParam,lParam);
}



wxPanel* CKLineView::GetControl(KTYPE kType)
{
	wxPanel* pWnd = NULL;	
	switch(kType)
	{
		case KTYPE_KLINE:
			pWnd = m_pKLineCtrl;
			break;
		case KTYPE_VOLUME:
			pWnd = m_pKVolumeCtrl;
			break;
		case KTYPE_TECHINDEX:
			pWnd = m_pKTechIndex;
			break;
		case KTYPE_SCALES:
			pWnd = m_pKScalesCtrl;
			break;

	}
	return pWnd;
}
void CKLineView::CfgShow(long lKLineType)
{
	m_lKLineType = lKLineType;

	m_splitter = new KSplitterWindow(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxSP_3D|wxNO_BORDER);
	m_splitter->SetSashGravity(0.75);	
	m_splitter->SetSashSize(1.0);

	m_splitterTop = new KSplitterWindow(m_splitter,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxSP_3D|wxNO_BORDER);
	m_splitterTop->SetSashGravity(0.67);
	m_splitterTop->SetSashSize(1);

	m_splitterBottom = new KSplitterWindow(m_splitter,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxSP_3D|wxNO_BORDER|wxSP_NOSASH);//wxSP_NOSASH这里主要为了刻度栏不可以拉大小，以后窗口不同的分隔形式的话，就需要改
	m_splitterBottom->SetSashGravity(1.0);
	m_splitterBottom->SetSashSize(1);	

	wxRect rtClient = GetClientRect();
	m_splitter->SetSize(rtClient.width, rtClient.height);
	if( (lKLineType & KTYPE_KLINE) == KTYPE_KLINE)
	{
		wxSize sz(0,0);
		m_pKLineCtrl = new CKLine;
		m_pKLineCtrl->Create(m_splitterTop, 1111, wxPoint(0,0), sz);
		m_pKLineCtrl->Show();	
	}
	if( (lKLineType & KTYPE_VOLUME) == KTYPE_VOLUME)
	{
		wxSize sz(0,0);
		m_pKVolumeCtrl = new CKVolume;
		m_pKVolumeCtrl->Create(m_splitterTop, 1112, wxPoint(0,0), sz);	
		m_pKVolumeCtrl->Show();
	}
	if( (lKLineType & KTYPE_TECHINDEX) ==  KTYPE_TECHINDEX)
	{
		wxSize sz(0,0);
		m_pKTechIndex = new CKTechIndex;
		m_pKTechIndex->Create(m_splitterBottom, 1113, wxPoint(0,0), sz);
		m_pKTechIndex->Show();	
	}
	if( (lKLineType & KTYPE_SCALES) ==  KTYPE_SCALES)
	{		
		wxSize sz(0,20);
		m_pKScalesCtrl = new CKScales;
		m_pKScalesCtrl->Create(m_splitterBottom, 1114,  wxPoint(0,0), sz);
		m_pKScalesCtrl->Show();		
	}
	if(m_pKLineCtrl && m_pKVolumeCtrl)
	{
		m_splitterTop->SplitHorizontally(m_pKLineCtrl, m_pKVolumeCtrl);		
	}
	else if(m_pKLineCtrl)
		m_splitterTop->Initialize(m_pKLineCtrl);
	else if(m_pKVolumeCtrl)
		m_splitterTop->Initialize(m_pKVolumeCtrl);	

	if(m_pKTechIndex && m_pKScalesCtrl)
	{
		m_splitterBottom->SplitHorizontally(m_pKTechIndex, m_pKScalesCtrl,-20);
	}
	else if(m_pKTechIndex)
		m_splitterBottom->Initialize(m_pKTechIndex);
	else if(m_pKScalesCtrl)
		m_splitterBottom->Initialize(m_pKScalesCtrl);
	m_splitter->SplitHorizontally(m_splitterTop, m_splitterBottom);	
	SetSize(100,100);//初始化大小很重要，太大太小都会使分隔条分隔的大小不对，wxWindgets真是一个破库，妈的！
	
}
wxWindow* CKLineView::CreateDlg(wxWindow *parent,
											wxWindowID winid ,
											const wxPoint& pos ,
											const wxSize& size,
											long style,
											const wxString& name )
{		
	style |= wxWANTS_CHARS|wxTE_PROCESS_ENTER|wxCLIP_CHILDREN;
	bool bReturn = wxPanel::Create(parent,winid,pos, size,	style|wxCLIP_CHILDREN|wxCLIP_SIBLINGS, name);			
	if(!bReturn)
		return NULL;
	g_pWnd = this;
	g_hMouse = SetWindowsHookEx(WH_MOUSE,MouseProc,NULL,GetCurrentThreadId());
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	return this;
}
void CKLineView::OnFuncMouseMoveHook(wxCommandEvent& event)
{
	POINT pt = *(POINT *)event.GetClientData();
	wxPoint ptWX(pt.x, pt.y);

	wxPoint wxpt = ptWX;
	wxpt = ScreenToClient(wxpt);
	wxSize size = GetClientSize();
	if(wxpt.x > size.x-1 || wxpt.y >size.y)
		return;

	if(m_pKLineCtrl)
	{
		m_pKLineCtrl->DoMouseMove(ptWX);
	}
	if(m_pKVolumeCtrl)
	{
		m_pKVolumeCtrl->DoMouseMove(ptWX);
	}
	if(m_pKTechIndex)
	{
		m_pKTechIndex->DoMouseMove(ptWX);
	}
	if(m_pKScalesCtrl)
	{
		m_pKScalesCtrl->DoMouseMove(ptWX);
	}
}
void CKLineView::OnMarksChange(wxCommandEvent& event)
{
	MAPsmark *mapMarks =  (MAPsmark *)event.GetClientData();
	wxPanel *pPanel = reinterpret_cast<wxPanel *>(event.GetExtraLong());
	if(m_pFData == NULL || mapMarks == NULL)
		return;

	m_pFData->m_mapMarks.clear();
	m_pFData->m_mapMarks = *mapMarks;

	if(m_pKLineCtrl && pPanel!= m_pKLineCtrl)
	{
		m_pKLineCtrl->SetMarks(m_pFData->m_mapMarks);
	}
	if(m_pKVolumeCtrl && pPanel!= m_pKVolumeCtrl)
	{
		m_pKVolumeCtrl->SetMarks(m_pFData->m_mapMarks);
	}
	if(m_pKTechIndex && pPanel!= m_pKTechIndex)
	{
		m_pKTechIndex->SetMarks(m_pFData->m_mapMarks);
	}
}

void CKLineView::OnSize(wxSizeEvent& event)
{
	if(m_splitter == NULL)
		return;
	wxRect rtClient = GetClientRect();
	m_splitter->SetSize(rtClient.width, rtClient.height);	
	m_splitter->UpdateSize();
	m_splitterBottom->UpdateSize();
	m_splitterTop->UpdateSize();	
}

void CKLineView::OnPaint (wxPaintEvent & PaintEvent)
{
	wxPaintDC dc(this);
	wxMemoryDC memdc;  
	memdc.SetLayoutDirection(dc.GetLayoutDirection());
	wxBitmap bmp;
	wxSize size = GetClientSize();
	bmp.Create(size.GetWidth(), size.GetHeight());
	memdc.SelectObject(bmp);

	size = GetClientSize();
	wxBrush brush(wxColour(RGB(192,0,110)));
	memdc.SetBrush(brush);
	memdc.DrawRectangle(0, 0, size.x, size.y);

	dc.Blit(0, 0, size.GetWidth(),size.GetHeight(),&memdc, 0, 0);

}

void CKLineView::OnFuncKeyDown(wxCommandEvent& event)
{
	int keyCode = event.GetInt();
	if(keyCode == VK_LEFT 
		|| keyCode == VK_RIGHT
		|| keyCode == VK_UP
		|| keyCode == VK_DOWN)
	{
		SItem&  sItem = *(SItem *)event.GetClientData();
		if(m_pKLineCtrl)
		{
			m_pKLineCtrl->SetCursel(sItem);
			m_pKLineCtrl->SetShowCross(true);
			m_pKLineCtrl->SetMouseIn(true);
			m_pKLineCtrl->Refresh(false);
			UpdateWindow((HWND)m_pKLineCtrl->GetHWND() );	 
		}
		if(m_pKVolumeCtrl)
		{
			m_pKVolumeCtrl->SetCursel(sItem);
			m_pKVolumeCtrl->SetShowCross(true);
			m_pKVolumeCtrl->SetMouseIn(true);
			m_pKVolumeCtrl->Refresh(false);
			UpdateWindow((HWND)m_pKVolumeCtrl->GetHWND() );	 
		}
		if(m_pKScalesCtrl)
		{
			m_pKScalesCtrl->SetCursel(sItem);	
			m_pKScalesCtrl->SetShowCross(true);
			m_pKScalesCtrl->SetMouseIn(true);
			m_pKScalesCtrl->Refresh(false);
			UpdateWindow((HWND)m_pKScalesCtrl->GetHWND() );	 
		}
		if(m_pKTechIndex)
		{
			m_pKTechIndex->SetCursel(sItem);
			m_pKTechIndex->SetShowCross(true);
			m_pKTechIndex->SetMouseIn(true);
			m_pKTechIndex->Refresh(false);
			UpdateWindow((HWND)m_pKTechIndex->GetHWND() );	 
		}		
	}	
	wxWindow* window= GetParent();	
	window->ProcessEvent(event);
}


void CKLineView::OnFuncLButtonDown(wxCommandEvent& event)
{	
	//OnMarksChange(event);
	if(m_pKLineCtrl)
	{
		m_pKLineCtrl->SetMouseIn(true);
	//	m_pKLineCtrl->SetShowCross(true);
		m_pKLineCtrl->Refresh(false);
		UpdateWindow((HWND)m_pKLineCtrl->GetHWND() );	 
	}
	if(m_pKVolumeCtrl)
	{
		m_pKVolumeCtrl->SetMouseIn(true);
	//	m_pKVolumeCtrl->SetShowCross(true);
		m_pKVolumeCtrl->Refresh(false);
		UpdateWindow((HWND)m_pKVolumeCtrl->GetHWND() );	 
	}
	if(m_pKScalesCtrl)
	{
		m_pKScalesCtrl->SetMouseIn(true);
	//	m_pKScalesCtrl->SetShowCross(true);
		m_pKScalesCtrl->Refresh(false);
		UpdateWindow((HWND)m_pKScalesCtrl->GetHWND() );	 
	}
	if(m_pKTechIndex)
	{
		m_pKTechIndex->SetMouseIn(true);
	//	m_pKTechIndex->SetShowCross(true);
		m_pKTechIndex->Refresh(false);
		UpdateWindow((HWND)m_pKTechIndex->GetHWND() );	 
	}		
	wxWindow* window= GetParent();	
	window->ProcessEvent(event);
}
void CKLineView::OnFuncMouseLeave(wxCommandEvent& event)
{
	void *pCtrl = event.GetClientData();
	
	if(m_pKLineCtrl && pCtrl != m_pKLineCtrl)
		m_pKLineCtrl->DoMouseLeave();
	if(m_pKVolumeCtrl && pCtrl != m_pKVolumeCtrl)
		m_pKVolumeCtrl->DoMouseLeave();
	if(m_pKScalesCtrl && pCtrl != m_pKScalesCtrl)
		m_pKScalesCtrl->DoMouseLeave();
	if(m_pKTechIndex && pCtrl != m_pKTechIndex)
		m_pKTechIndex->DoMouseLeave();

	wxRect rt = GetClientRect();
	wxPoint pt = wxGetMousePosition();
	pt =  ScreenToClient(pt);
	if(pt.x < rt.GetWidth() && pt.y <rt.GetHeight() && pt.x >=0 && pt.y >=0)
		return;
	else
	{
		if(m_pKLineCtrl)
		{
			m_pKLineCtrl->SetMouseIn(false);
			m_pKLineCtrl->Refresh(false);
			UpdateWindow((HWND)m_pKLineCtrl->GetHWND() );	 
		}
		if(m_pKVolumeCtrl)
		{
			m_pKVolumeCtrl->SetMouseIn(false);	
			m_pKVolumeCtrl->Refresh(false);
			UpdateWindow((HWND)m_pKVolumeCtrl->GetHWND() );	 
		}
		if(m_pKScalesCtrl)
		{		
			m_pKScalesCtrl->SetMouseIn(false);
			m_pKScalesCtrl->Refresh(false);
			UpdateWindow((HWND)m_pKScalesCtrl->GetHWND() );	 
		}
		if(m_pKTechIndex)
		{			
			m_pKTechIndex->SetMouseIn(false);
			m_pKTechIndex->Refresh(false);
			UpdateWindow((HWND)m_pKTechIndex->GetHWND() );	 
		}
	}

}
void CKLineView::OnFuncMouseEnter(wxCommandEvent& event)
{
	wxRect rt = GetClientRect();
	wxPoint pt = wxGetMousePosition();
	pt = ScreenToClient(pt);
	if(pt.x < rt.GetWidth() && pt.y <rt.GetHeight() && pt.x >=0 && pt.y >=0)
	{
		if(m_pKLineCtrl)
		{
			m_pKLineCtrl->SetMouseIn(true);
			m_pKLineCtrl->Refresh(false);
			UpdateWindow((HWND)m_pKLineCtrl->GetHWND() );	 
		}
		if(m_pKVolumeCtrl)
		{
			m_pKVolumeCtrl->SetMouseIn(true);
			m_pKVolumeCtrl->Refresh(false);
			UpdateWindow((HWND)m_pKVolumeCtrl->GetHWND() );	 
		}
		if(m_pKScalesCtrl)
		{		
			m_pKScalesCtrl->SetMouseIn(true);
			m_pKScalesCtrl->Refresh(false);
			UpdateWindow((HWND)m_pKScalesCtrl->GetHWND() );	 
		}
		if(m_pKTechIndex)
		{			
			m_pKTechIndex->SetMouseIn(true);
			m_pKTechIndex->Refresh(false);
			UpdateWindow((HWND)m_pKTechIndex->GetHWND() );	 
		}
	}	
}
void CKLineView::SetFData(CFData *pData)
{
	if(pData == NULL)
		return;
	m_pFData = pData;
	m_bInit = false;
	if(m_pKLineCtrl)
	{
		m_pKLineCtrl->SetFData(pData);	
	}
	if(m_pKVolumeCtrl)
	{
		m_pKVolumeCtrl->SetFData(pData);		
	}
	if(m_pKTechIndex)
	{
		m_pKTechIndex->SetFData(pData);		
	}
	if(m_pKScalesCtrl)
	{
		m_pKScalesCtrl->SetFData(pData);	
	}
}
EnumPhrase CKLineView::GetPhrase()
{
	return m_enumPhrase;
}
void CKLineView::SetPhrase(EnumPhrase enumPhrase)
{
	if(m_bInit && m_enumPhrase == enumPhrase)
		return;
	m_bInit =true;
	if(m_pFData == NULL)
		return;

	m_pFData->SetPhrase(enumPhrase);
	m_enumPhrase = enumPhrase;
	vector<SKLine *>* vecKLineCommon = m_pFData->GetVectorCommon(m_enumPhrase);
	if(m_pKLineCtrl)
	{
		//m_pKLineCtrl->SetPhrase(enumPhrase);
		m_pKLineCtrl->SetFuture(m_pFData->m_ExchangeName, m_pFData->m_ContractName, m_pFData->m_ContractDate, enumPhrase);
		m_pKLineCtrl->SetVectorData(*vecKLineCommon);	
		m_pKLineCtrl->SetMarks(m_pFData->m_mapMarks);
		m_pKLineCtrl->Refresh(false);
	}
	if(m_pKVolumeCtrl)
	{
		//m_pKVolumeCtrl->SetPhrase(enumPhrase);
		m_pKVolumeCtrl->SetFuture(m_pFData->m_ExchangeName, m_pFData->m_ContractName, m_pFData->m_ContractDate, enumPhrase);
		m_pKVolumeCtrl->SetVectorData(*vecKLineCommon);
		m_pKVolumeCtrl->SetMarks(m_pFData->m_mapMarks);
		m_pKVolumeCtrl->Refresh(false);
	}
	if(m_pKScalesCtrl)
	{
		//m_pKScalesCtrl->SetPhrase(enumPhrase);
		m_pKScalesCtrl->SetFuture(m_pFData->m_ExchangeName, m_pFData->m_ContractName, m_pFData->m_ContractDate, enumPhrase);
		m_pKScalesCtrl->SetVectorData(*vecKLineCommon);	
		m_pKScalesCtrl->Refresh(false);
	}
	if(m_pKTechIndex)
	{
		//m_pKTechIndex->SetPhrase(enumPhrase);
		m_pKTechIndex->SetFuture(m_pFData->m_ExchangeName, m_pFData->m_ContractName, m_pFData->m_ContractDate, enumPhrase);
		m_pKTechIndex->SetVectorData(*vecKLineCommon);	
		m_pKTechIndex->SetMarks(m_pFData->m_mapMarks);
		m_pKTechIndex->Refresh(false);
	}
}
void CKLineView::SetMarkType(EnumMark enumMark)
{
	if(m_pKLineCtrl)
	{
		m_pKLineCtrl->SetMarkType(enumMark);		
	}
	if(m_pKVolumeCtrl)
	{
		m_pKVolumeCtrl->SetMarkType(enumMark);		
	}
	if(m_pKTechIndex)
	{
		m_pKTechIndex->SetMarkType(enumMark);		
	}
	if(m_pFData)
		m_pFData->m_enumMark = enumMark;
}
void CKLineView::OnFuncRButtonUp(wxCommandEvent& event)
{
	if(m_pFData)
	{
		m_pFData->m_pDelMark = (SPoint *)event.GetClientData();
		if(m_pFData->m_pDelMark != NULL)
		{
			wxPoint pos =  wxGetMousePosition();
			pos = ScreenToClient(pos);
			wxMenu *menuPopUp = new wxMenu(wxEmptyString, wxMENU_TEAROFF);
			menuPopUp->AppendCheckItem(ID_DEL_MARK,
				_T("删除标注"),
				_T(""));

			PopupMenu( menuPopUp, pos );
			delete menuPopUp;

			return;
		}
	}	
	m_strTechName_ByMessage = event.GetString();
	m_pRightUpWnd =reinterpret_cast<wxWindow*>(event.GetExtraLong());
	wxSize sz(200,200);
	wxPoint pos =  wxGetMousePosition();
	pos = ScreenToClient(pos);
	

	wxMenu *menuPopUp = new wxMenu(wxEmptyString, wxMENU_TEAROFF);
	menuPopUp->AppendCheckItem(ID_PHRASE_1MIN,
		_T("1分钟图"),
		_T(""));
	menuPopUp->AppendCheckItem(ID_PHRASE_5MIN,
		_T("5分钟图"),
		_T(""));
	menuPopUp->AppendCheckItem(ID_PHRASE_15MIN,
		_T("15分钟图"),
		_T(""));
	menuPopUp->AppendCheckItem(ID_PHRASE_30MIN,
		_T("30分钟图"),
		_T(""));
	menuPopUp->AppendCheckItem(ID_PHRASE_60MIN,
		_T("60分钟图"),
		_T(""));
	menuPopUp->AppendSeparator();

	menuPopUp->AppendCheckItem(ID_PHRASE_DAY,
		_T("日线图"),
		_T(""));
	menuPopUp->AppendCheckItem(ID_PHRASE_WEEK,
		_T("周线图"),
		_T(""));
	menuPopUp->AppendCheckItem(ID_PHRASE_MONTH,
		_T("月线图"),
		_T(""));
	CheckPhrase(menuPopUp);//选中的勾
	menuPopUp->AppendSeparator();

	menuPopUp->AppendCheckItem(ID_CROSS_SHOW,
		_T("十字光标"),
		_T(""));

	bool bShowCross = false;
	if(m_pKLineCtrl)
		bShowCross = m_pKLineCtrl->GetShowCross();
	menuPopUp->Check(ID_CROSS_SHOW, bShowCross);

	wxMenu *pMenuDraw = new wxMenu;
	pMenuDraw->AppendCheckItem(ID_DRAW_TRIANGLE_UP,
		_T("向上三角形"),
		_T(""));
	pMenuDraw->AppendCheckItem(ID_DRAW_TRIANGLE_DOWN,
		_T("向下三角形"),
		_T(""));
	menuPopUp->AppendSubMenu(pMenuDraw, wxT("标注"));


	wxMenu *pMenuTech = new wxMenu;
	menuPopUp->AppendSubMenu(pMenuTech, wxT("当前指标"));
	pMenuTech->AppendCheckItem(ID_TECHINDEX_ADJUSTPARA,
		_T("调整指标参数"),
		_T(""));	
	

	wxMenu *pTech = new wxMenu;
	menuPopUp->AppendSubMenu(pTech, wxT("所有指标"));
	pTech->AppendCheckItem(ID_TECHINDEX_MA,	  MA,	_T(""));	
	pTech->AppendCheckItem(ID_TECHINDEX_KDJ,  KDJ,	_T(""));
	pTech->AppendCheckItem(ID_TECHINDEX_MACD, MACD,	_T(""));	
	pTech->AppendCheckItem(ID_TECHINDEX_BOLL, BOLL,	_T(""));	
	pTech->AppendCheckItem(ID_TECHINDEX_UOS,  UOS,	_T(""));
	pTech->AppendCheckItem(ID_TECHINDEX_BIAS, BIAS,	_T(""));
	pTech->AppendCheckItem(ID_TECHINDEX_DMI, DMI,	_T(""));
	pTech->AppendCheckItem(ID_TECHINDEX_PSY, PSY,	_T(""));
	pTech->AppendCheckItem(ID_TECHINDEX_ROC, ROC,	_T(""));



	menuPopUp->AppendSeparator();
	menuPopUp->AppendCheckItem(ID_RETURN_TLINE,	_T("分时走势图"), _T(""));
	menuPopUp->AppendCheckItem(ID_RETURN_BAOJIABIAO,	_T("报价表"), _T(""));

	PopupMenu( menuPopUp, pos );
	delete menuPopUp;
}
void CKLineView::PhraseMin1(wxCommandEvent& event)
{
	SetPhrase(PHRASE_1MIN);	
}
void CKLineView::PhraseMin5(wxCommandEvent& event)
{	
	SetPhrase(PHRASE_5MIN);	
}
void CKLineView::PhraseMin15(wxCommandEvent& event)
{
	SetPhrase(PHRASE_15MIN);
}
void CKLineView::PhraseMin30(wxCommandEvent& event)
{
	SetPhrase(PHRASE_30MIN);
}
void CKLineView::PhraseMin60(wxCommandEvent& event)
{
	SetPhrase(PHRASE_60MIN);	
}
void CKLineView::PhraseDay(wxCommandEvent& event)
{
	SetPhrase(PHRASE_DAY);	
}
void CKLineView::PhraseWeek(wxCommandEvent& event)
{
	SetPhrase(PHRASE_WEEK);
}
void CKLineView::PhraseMonth(wxCommandEvent& event)
{
	SetPhrase(PHRASE_MONTH);		
}
void CKLineView::CrossShow(wxCommandEvent& event)
{
	bool bShowCross = false;
	if(m_pKLineCtrl)
		bShowCross = m_pKLineCtrl->GetShowCross();

	 
	 if(m_pKLineCtrl)
	 {	
		  m_pKLineCtrl->SetShowCross(!bShowCross);
		  m_pKLineCtrl->Refresh(false);
		  UpdateWindow((HWND)m_pKLineCtrl->GetHWND() );	 
		
	 }
	 if(m_pKVolumeCtrl)
	 {		
		m_pKVolumeCtrl->SetShowCross(!bShowCross);
		m_pKVolumeCtrl->Refresh(false);
		UpdateWindow((HWND)m_pKVolumeCtrl->GetHWND() );	 
	 }
	 if(m_pKScalesCtrl)
	 {	
		 m_pKScalesCtrl->SetShowCross(!bShowCross);
		 m_pKScalesCtrl->Refresh(false);
		 UpdateWindow((HWND)m_pKScalesCtrl->GetHWND() );	 
	 }
	 if(m_pKTechIndex)
	 {	
		m_pKTechIndex->SetShowCross(!bShowCross);
		m_pKTechIndex->Refresh(false);
		UpdateWindow((HWND)m_pKTechIndex->GetHWND() );	 
	 }
}
void CKLineView::CheckPhrase(wxMenu *&menuPopUp)
{
	EnumPhrase phrase = PHRASE_USERDEFINE;
	if(m_pKLineCtrl)
		phrase = m_pKLineCtrl->GetPhrase();
	if(phrase == PHRASE_1MIN)
		menuPopUp->Check(ID_PHRASE_1MIN, true);
	else
		menuPopUp->Check(ID_PHRASE_1MIN, false);

	if(phrase == PHRASE_5MIN)
		menuPopUp->Check(ID_PHRASE_5MIN, true);
	else
		menuPopUp->Check(ID_PHRASE_5MIN, false);

	if(phrase == PHRASE_15MIN)
		menuPopUp->Check(ID_PHRASE_15MIN, true);
	else
		menuPopUp->Check(ID_PHRASE_15MIN, false);

	if(phrase == PHRASE_30MIN)
		menuPopUp->Check(ID_PHRASE_30MIN, true);
	else
		menuPopUp->Check(ID_PHRASE_30MIN, false);

	if(phrase == PHRASE_60MIN)
		menuPopUp->Check(ID_PHRASE_60MIN, true);
	else
		menuPopUp->Check(ID_PHRASE_60MIN, false);

	if(phrase == PHRASE_DAY)
		menuPopUp->Check(ID_PHRASE_DAY, true);
	else
		menuPopUp->Check(ID_PHRASE_DAY, false);

	if(phrase == PHRASE_WEEK)
		menuPopUp->Check(ID_PHRASE_WEEK, true);
	else
		menuPopUp->Check(ID_PHRASE_WEEK, false);

	if(phrase == PHRASE_MONTH)
		menuPopUp->Check(ID_PHRASE_MONTH, true);
	else
		menuPopUp->Check(ID_PHRASE_MONTH, false);
	
}
void CKLineView::MarkTriangleUp(wxCommandEvent& event)
{	
	SetMarkType(MARK_TRIANGLE_UP);
}
void CKLineView::MarkTriangleDown(wxCommandEvent& event)
{	
	SetMarkType(MARK_TRIANGLE_DOWN);
}
BOOL CKLineView::SetInTimeData()
{
	if(m_pFData == NULL)
		return true;
	vector<SKLine *>* vecKLineCommon = m_pFData->GetVectorCommon(m_enumPhrase);
	if(vecKLineCommon == NULL)
		return false;
	if(m_pKLineCtrl)
	{	
		m_pKLineCtrl->SetVectorData_InTime(*vecKLineCommon);
	}
	if(m_pKVolumeCtrl)
	{		
		m_pKVolumeCtrl->SetVectorData_InTime(*vecKLineCommon);
	}
	if(m_pKScalesCtrl)
	{	
		m_pKScalesCtrl->SetVectorData_InTime(*vecKLineCommon);	
	}
	if(m_pKTechIndex)
	{	
		m_pKTechIndex->SetVectorData_InTime(*vecKLineCommon);
	}
	return true;
}

void CKLineView::OnFuncLDBClick(wxCommandEvent& event)
{
	wxWindow* window= GetParent();
	window->ProcessEvent(event);
}
bool CKLineView::InitCfg(TiXmlElement *root)
{
	if(m_pKLineCtrl)
	{
		if(!m_pKLineCtrl->InitCfg(root))
		{
			return false;
		}
	}


	if(m_pKVolumeCtrl)
	{
		if(!m_pKVolumeCtrl->InitCfg(root))
		{
			return false;
		}
	}

	if(m_pKScalesCtrl)
	{
		if(!m_pKScalesCtrl->InitCfg(root))
		{
			return false;
		}
	}
	if(m_pKTechIndex)
	{
		if(!m_pKTechIndex->InitCfg(root))
		{
			return false;
		}
	}
	return true;
}
void CKLineView::OnMouseLeave(wxMouseEvent &event)
{
	

	event.Skip();
}
void CKLineView::OnMouseEnter(wxMouseEvent &event)
{
	
}
void CKLineView::SetLastTransTime(struct tm tmTime)
{
	if(m_pFData == NULL)
		return;
		
	vector<SKLine *>* vecKLineCommon = m_pFData->GetVectorCommon(m_enumPhrase);
	if(vecKLineCommon == NULL)
		return;
	if(m_pKLineCtrl)
	{		
		m_pKLineCtrl->SetVectorData_InTime(*vecKLineCommon);			
	}
	if(m_pKVolumeCtrl)
	{		
		m_pKVolumeCtrl->SetVectorData_InTime(*vecKLineCommon);				
	}
	if(m_pKScalesCtrl)
	{	
		m_pKScalesCtrl->SetVectorData_InTime(*vecKLineCommon);				
	}
	if(m_pKTechIndex)
	{		
		m_pKTechIndex->SetVectorData_InTime(*vecKLineCommon);			
	}
}

void CKLineView::DelMark(wxCommandEvent& event)
{
	if(m_pFData == NULL)
		return;

	MAPsmark::iterator it = m_pFData->m_mapMarks.begin();
	while(it != m_pFData->m_mapMarks.end())
	{
		MAPsmark::iterator itTemp = it;
		it++;

		EnumMark enumMark	= (EnumMark)itTemp->first;
		SMark	 *pTemp		= itTemp->second;
		for(vector<SPoint *>::iterator it = pTemp->vecPt.begin(); it != pTemp->vecPt.end(); )
		{
			SPoint *p = *it;
			if(p == m_pFData->m_pDelMark)
			{
				delete p;
				it = pTemp->vecPt.erase(it);
				if(m_pKLineCtrl)
				{
					m_pKLineCtrl->SetMarks(m_pFData->m_mapMarks);
				}
				if(m_pKVolumeCtrl)
				{
					m_pKVolumeCtrl->SetMarks(m_pFData->m_mapMarks);
				}
				if(m_pKTechIndex)
				{
					m_pKTechIndex->SetMarks(m_pFData->m_mapMarks);
				}
			}
			else 
				it++;
		}
	}	

}

void CKLineView::OnTechIndexAdjustPara(wxCommandEvent& WXUNUSED(event))
{
	vector<int> vecConfig;
	if(m_pFData)
		m_pFData->GetTechIndexConfig(m_strTechName_ByMessage, m_enumPhrase, vecConfig);
	
	TechIndexConfigDlg dlg(this);
	dlg.SetTechIndex(m_strTechName_ByMessage, m_enumPhrase, vecConfig);
	dlg.SetMsgWnd(m_pRightUpWnd);
	dlg.ShowModal();
//	if(m_pFData)
//		m_pFData->SetTechIndexConfig(m_strTechName_ByMessage, m_enumPhrase, dlg.m_vecConfig);
	
	m_strTechName_ByMessage = "";
	m_pRightUpWnd = NULL;
}
void CKLineView::OnMA(wxCommandEvent& WXUNUSED(event))
{
	if(m_pKLineCtrl)
		m_pKLineCtrl->SetTechIndex(MA);
}
void CKLineView::OnKDJ(wxCommandEvent& WXUNUSED(event))
{
	if(m_pKTechIndex)
		m_pKTechIndex->SetTechIndex(KDJ);
}
void CKLineView::OnMACD(wxCommandEvent& WXUNUSED(event))
{
	if(m_pKTechIndex)
		m_pKTechIndex->SetTechIndex(MACD);
}
void CKLineView::OnBOLL(wxCommandEvent& WXUNUSED(event))
{
	if(m_pKLineCtrl)
		m_pKLineCtrl->SetTechIndex(BOLL);
}
void CKLineView::OnUOS(wxCommandEvent& WXUNUSED(event))
{
	if(m_pKTechIndex)
		m_pKTechIndex->SetTechIndex(UOS);
}
void CKLineView::OnBIAS(wxCommandEvent& WXUNUSED(event))
{
	if(m_pKTechIndex)
		m_pKTechIndex->SetTechIndex(BIAS);
}
void CKLineView::OnDMI(wxCommandEvent& WXUNUSED(event))
{
	if(m_pKTechIndex)
		m_pKTechIndex->SetTechIndex(DMI);
}
void CKLineView::OnPSY(wxCommandEvent& WXUNUSED(event))
{
	if(m_pKTechIndex)
		m_pKTechIndex->SetTechIndex(PSY);
}
void CKLineView::OnROC(wxCommandEvent& WXUNUSED(event))
{
	if(m_pKTechIndex)
		m_pKTechIndex->SetTechIndex(ROC);
}
void CKLineView::OnReturnTLine(wxCommandEvent& event)
{
	wxWindow* window= GetParent();
	wxCommandEvent myEvent(wxEVT_RETURN_TLINE);
	window->ProcessEvent(myEvent);
}
void CKLineView::OnReturnBaoJiaBiao(wxCommandEvent& event)
{
	wxWindow* window= GetParent();
	wxCommandEvent myEvent(wxEVT_RETURN_BAOJIABIAO);
	window->ProcessEvent(myEvent);
}