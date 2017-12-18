#include "StdAfx.h"

#include "../../../inc/Quotation/KLineView.h"
#include "../../../inc/Quotation/TechIndexConfigDlg.h"
#include "../../../inc/Quotation/TechIndexExpressDlg.h"
#include "../../../inc/Quotation/MyFrame.h"
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
#define  ID_TECHINDEX_USEEXPRESS	23140

#define  ID_TECHINDEX_MA			23142
#define  ID_TECHINDEX_KDJ			23143
#define  ID_TECHINDEX_MACD			23144
#define  ID_TECHINDEX_BOLL			23145
#define  ID_TECHINDEX_UOS			23146
#define  ID_TECHINDEX_BIAS			23147
#define  ID_TECHINDEX_DMI			23148
#define  ID_TECHINDEX_PSY			23149
#define  ID_TECHINDEX_ROC			23150
#define  ID_TECHINDEX_VOL			23151
#define  ID_TECHINDEX_BBI			23152
#define  ID_TECHINDEX_EXPMA			23153
#define  ID_TECHINDEX_DMA			23154
#define  ID_TECHINDEX_SAR			23155
#define  ID_TECHINDEX_TRIX			23156
#define  ID_TECHINDEX_MTM			23157
#define  ID_TECHINDEX_CR			23158
#define  ID_TECHINDEX_VR			23159
#define  ID_TECHINDEX_OBV			23160
#define  ID_TECHINDEX_ASI			23161
#define  ID_TECHINDEX_EMV			23162
#define  ID_TECHINDEX_CCI			23163
#define  ID_TECHINDEX_RSI			23164
#define  ID_TECHINDEX_CDP			23165

#define  ID_VIEW_ONE				24000
#define  ID_VIEW_TWO				24001
#define  ID_VIEW_THREE				24002
#define  ID_VIEW_INFO				24003

#define  ID_KTLINE_ADD				24157
#define  ID_KTLINE_MAXSIZE			24158

#define  ID_RETURN_TLINE			24161
#define  ID_RETURN_BAOJIABIAO		24162

#define MAXSPAN 10

DEFINE_EVENT_TYPE(wxEVT_KLINEVIEW_MOUSEMOVEHOOK)
HHOOK g_hMouse=NULL; //鼠标的钩子过程
wxPanel *g_pWnd = NULL;

DEFINE_EVENT_TYPE(wxEVT_RETURN_TLINE)
DEFINE_EVENT_TYPE(wxEVT_RETURN_BAOJIABIAO)
DEFINE_EVENT_TYPE(wxEVT_KLINEVIW_INFO)

IMPLEMENT_CLASS(CKLineView, wxPanel)
CKLineView::CKLineView(MyFrame* myFrame):m_myFrame(myFrame)
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

	m_nViewCount = 3;
	m_bShowRightPanel = true; 
}

CKLineView::~CKLineView(void)
{
	UnhookWindowsHookEx(g_hMouse);
}
void CKLineView::ClearMemory_Mark()
{
	if(m_pFData == NULL)
		return;
//	if(m_pKLineCtrl)
//		m_pKLineCtrl->SetMarks(m_pFData->m_mapMarks);
//	if(m_pKVolumeCtrl)
//		m_pKVolumeCtrl->SetMarks(m_pFData->m_mapMarks);
//	if(m_pKTechIndex)
//		m_pKTechIndex->SetMarks(m_pFData->m_mapMarks);
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
EVT_MENU(ID_TECHINDEX_USEEXPRESS,	OnTechIndexUseExpress)
EVT_MENU(ID_TECHINDEX_MA,	CKLineView::OnMA)
EVT_MENU(ID_TECHINDEX_KDJ,	CKLineView::OnKDJ)
EVT_MENU(ID_TECHINDEX_MACD,	CKLineView::OnMACD)
EVT_MENU(ID_TECHINDEX_BOLL,	CKLineView::OnBOLL)
EVT_MENU(ID_TECHINDEX_UOS,	CKLineView::OnUOS)
EVT_MENU(ID_TECHINDEX_BIAS,	CKLineView::OnBIAS)
EVT_MENU(ID_TECHINDEX_DMI,	CKLineView::OnDMI)
EVT_MENU(ID_TECHINDEX_PSY,	CKLineView::OnPSY)
EVT_MENU(ID_TECHINDEX_ROC,	CKLineView::OnROC)
EVT_MENU(ID_TECHINDEX_VOL,	CKLineView::OnVOL)
EVT_MENU(ID_TECHINDEX_BBI,	CKLineView::OnBBI)
EVT_MENU(ID_TECHINDEX_EXPMA,	CKLineView::OnEXPMA)
EVT_MENU(ID_TECHINDEX_DMA,	CKLineView::OnDMA)
EVT_MENU(ID_TECHINDEX_SAR,	CKLineView::OnSAR)
EVT_MENU(ID_TECHINDEX_TRIX,	CKLineView::OnTRIX)
EVT_MENU(ID_TECHINDEX_MTM,	CKLineView::OnMTM)
EVT_MENU(ID_TECHINDEX_CR,	CKLineView::OnCR)
EVT_MENU(ID_TECHINDEX_VR,	CKLineView::OnVR)
EVT_MENU(ID_TECHINDEX_OBV,	CKLineView::OnOBV)
EVT_MENU(ID_TECHINDEX_ASI,	CKLineView::OnASI)
EVT_MENU(ID_TECHINDEX_EMV,	CKLineView::OnEMV)
EVT_MENU(ID_TECHINDEX_CCI,	CKLineView::OnCCI)
EVT_MENU(ID_TECHINDEX_RSI,	CKLineView::OnRSI)
EVT_MENU(ID_TECHINDEX_CDP,	CKLineView::OnCDP)

EVT_MENU(ID_DEL_MARK,	CKLineView::DelMark)

EVT_MENU(ID_KTLINE_ADD,	OnKTLineAdd)
EVT_MENU(ID_KTLINE_MAXSIZE,	OnKTLineMaxSize)

EVT_MENU(ID_RETURN_TLINE,	OnReturnTLine)
EVT_MENU(ID_RETURN_BAOJIABIAO,	OnReturnBaoJiaBiao)


EVT_COMMAND(wxID_ANY, wxEVT_KLINEVIEW_MOUSEMOVEHOOK, CKLineView::OnFuncMouseMoveHook)

EVT_COMMAND(wxID_ANY, wxEVT_KCtrl_MARKSCHANGE, CKLineView::OnMarksChange)


EVT_MENU(ID_VIEW_ONE,	OnViewOne)
EVT_MENU(ID_VIEW_TWO,	OnViewTwo)
EVT_MENU(ID_VIEW_THREE,	OnViewThree)
EVT_MENU(ID_VIEW_INFO,	OnViewInfo)

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
	//MAPsmark *mapMarks =  (MAPsmark *)event.GetClientData();
	wxPanel *pPanel = reinterpret_cast<wxPanel *>(event.GetExtraLong());
	//if(m_pFData == NULL || mapMarks == NULL)
	//	return;

//	m_pFData->m_mapMarks.clear();
//	m_pFData->m_mapMarks = *mapMarks;

	if(m_pKLineCtrl && pPanel!= m_pKLineCtrl)
	{
		m_pKLineCtrl->SetMarks();
	}
	if(m_pKVolumeCtrl && pPanel!= m_pKVolumeCtrl)
	{
		m_pKVolumeCtrl->SetMarks();
	}
	if(m_pKTechIndex && pPanel!= m_pKTechIndex)
	{
		m_pKTechIndex->SetMarks();
	}
}

void CKLineView::OnSize(wxSizeEvent& event)
{
	if(m_splitter == NULL)
		return;

	switch(m_nViewCount)
	{
	case 1:
		{
			OnView1();
			break;
		}
	case 2:
		{
			OnView2();
			break;
		}
	case 3:
		{
			OnView3();
			break;
		}
	}
/*
	wxRect rtClient = GetClientRect();
	m_splitter->SetSize(rtClient.width, rtClient.height);		
	m_splitter->UpdateSize();
	m_splitterBottom->SetSashPosition(-20);
	m_splitterBottom->UpdateSize();
	m_splitterTop->UpdateSize();	*/
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
	if(m_myFrame)	
		m_myFrame->ProcessEvent(event);
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
CFData* CKLineView::GetFData()
{
	return m_pFData;
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
		m_pKLineCtrl->SetPhrase(enumPhrase);
		m_pKLineCtrl->SetVectorData(*vecKLineCommon);	
	//	m_pKLineCtrl->SetMarks(m_pFData->m_mapMarks);
		m_pKLineCtrl->Refresh(false);
	}
	if(m_pKVolumeCtrl)
	{
		m_pKVolumeCtrl->SetPhrase(enumPhrase);
		m_pKVolumeCtrl->SetVectorData(*vecKLineCommon);
//		m_pKVolumeCtrl->SetMarks(m_pFData->m_mapMarks);
		m_pKVolumeCtrl->Refresh(false);
	}
	if(m_pKScalesCtrl)
	{		
		m_pKScalesCtrl->SetPhrase(enumPhrase);
		m_pKScalesCtrl->SetVectorData(*vecKLineCommon);	
		m_pKScalesCtrl->Refresh(false);
	}
	if(m_pKTechIndex)
	{		
		m_pKTechIndex->SetPhrase(enumPhrase);
		m_pKTechIndex->SetVectorData(*vecKLineCommon);	
	//	m_pKTechIndex->SetMarks(m_pFData->m_mapMarks);
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
//	if(m_pFData)
//		m_pFData->m_enumMark = enumMark;
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
	m_pRightUpWnd =reinterpret_cast<CKCtrl*>(event.GetExtraLong());
	wxSize sz(200,200);
	wxPoint pos =  wxGetMousePosition();
	pos = ScreenToClient(pos);
	wxString strTechIndexName = m_pRightUpWnd->GetTechIndexName();

	wxMenu *menuPopUp = new wxMenu(wxEmptyString, wxMENU_TEAROFF);

	wxMenu *pMenuSpan = new wxMenu;
	menuPopUp->AppendSubMenu(pMenuSpan, wxT("变换周期"));
	pMenuSpan->AppendCheckItem(ID_PHRASE_1MIN,_T("1分钟图"),_T(""));
	pMenuSpan->AppendCheckItem(ID_PHRASE_5MIN,_T("5分钟图"),_T(""));
	pMenuSpan->AppendCheckItem(ID_PHRASE_15MIN,_T("15分钟图"),_T(""));
	pMenuSpan->AppendCheckItem(ID_PHRASE_30MIN,_T("30分钟图"),_T(""));
	pMenuSpan->AppendCheckItem(ID_PHRASE_60MIN,_T("60分钟图"),_T(""));
	pMenuSpan->AppendSeparator();

	pMenuSpan->AppendCheckItem(ID_PHRASE_DAY,_T("日线图"),_T(""));
	pMenuSpan->AppendCheckItem(ID_PHRASE_WEEK,_T("周线图"),	_T(""));
	pMenuSpan->AppendCheckItem(ID_PHRASE_MONTH,_T("月线图"),_T(""));
	CheckPhrase(menuPopUp);//选中的勾
	menuPopUp->AppendSeparator();

	menuPopUp->AppendCheckItem(ID_CROSS_SHOW,_T("十字光标"),_T(""));

	bool bShowCross = false;
	if(m_pKLineCtrl)
		bShowCross = m_pKLineCtrl->GetShowCross();
	menuPopUp->Check(ID_CROSS_SHOW, bShowCross);

	wxMenu *pMenuDraw = new wxMenu;
	pMenuDraw->AppendCheckItem(ID_DRAW_TRIANGLE_UP,	_T("向上三角形"),_T(""));
	pMenuDraw->AppendCheckItem(ID_DRAW_TRIANGLE_DOWN,_T("向下三角形"),_T(""));
	menuPopUp->AppendSubMenu(pMenuDraw, wxT("标注"));


	wxMenu *pMenuTech = new wxMenu;
	menuPopUp->AppendSubMenu(pMenuTech, wxT("当前指标"));
	pMenuTech->AppendCheckItem(ID_TECHINDEX_USEEXPRESS, _T("指标用法注释"), _T(""));	
	if(strTechIndexName != OBV 
		&& strTechIndexName != ASI
		&& strTechIndexName != CDP)//obv不需要参数设置
		pMenuTech->AppendCheckItem(ID_TECHINDEX_ADJUSTPARA, _T("调整指标参数"), _T(""));	
	
	

	wxMenu *pTech = new wxMenu;
	menuPopUp->AppendSubMenu(pTech, wxT("所有指标"));

//	wxString strTechIndexName;
	//if(m_pKLineCtrl)
	//	strTechIndexName = m_pKLineCtrl->GetTechIndexName();

	wxMenu *pTechMain = new wxMenu;
	pTech->AppendSubMenu(pTechMain, wxT("趋向指标"));

	wxString strFormat;
	wxString strShow;
	wxString strName = MA;
	wxString strDes  = wxT("均价");	
	strFormat.Printf(wxT("%%s%%%ds"), MAXSPAN+strDes.length()-strName.length());
	strShow.Printf(strFormat, strName, strDes);
	pTechMain->AppendCheckItem(ID_TECHINDEX_MA,	  strShow,	_T(""));	

	strName = MACD;
	strDes  = wxT("指数平滑移动平均线");	
	strFormat.Printf(wxT("%%s%%%ds"), MAXSPAN+strDes.length()-strName.length());
	strShow.Printf(strFormat, strName, strDes);
	pTechMain->AppendCheckItem(ID_TECHINDEX_MACD, strShow,	_T(""));	

	strName = DMI;
	strDes  = wxT("趋向指标");	
	strFormat.Printf(wxT("%%s%%%ds"), MAXSPAN+strDes.length()-strName.length());
	strShow.Printf(strFormat, strName, strDes);
	pTechMain->AppendCheckItem(ID_TECHINDEX_DMI, strShow,	_T(""));

	strName = DMA;
	strDes  = wxT("平均线差");	
	strFormat.Printf(wxT("%%s%%%ds"), MAXSPAN+strDes.length()-strName.length());
	strShow.Printf(strFormat, strName, strDes);
	pTechMain->AppendCheckItem(ID_TECHINDEX_DMA, strShow,	_T(""));

	strName = BBI;
	strDes  = wxT("多空指数");	
	strFormat.Printf(wxT("%%s%%%ds"), MAXSPAN+strDes.length()-strName.length());
	strShow.Printf(strFormat, strName, strDes);
	pTechMain->AppendCheckItem(ID_TECHINDEX_BBI, strShow,	_T(""));

	strName = EXPMA;
	strDes  = wxT("指数平滑移动平均线");	
	strFormat.Printf(wxT("%%s%%%ds"), MAXSPAN+strDes.length()-strName.length());
	strShow.Printf(strFormat, strName, strDes);
	pTechMain->AppendCheckItem(ID_TECHINDEX_EXPMA, strShow,	_T(""));


	strName = SAR;
	strDes  = wxT("指数转向（stop and reverse）");	
	strFormat.Printf(wxT("%%s%%%ds"), MAXSPAN+strDes.length()-strName.length());
	strShow.Printf(strFormat, strName, strDes);
	pTechMain->AppendCheckItem(ID_TECHINDEX_SAR, strShow,	_T(""));

	strName = TRIX;
	strDes  = wxT("三重指数平滑移动平均线");	
	strFormat.Printf(wxT("%%s%%%ds"), MAXSPAN+strDes.length()-strName.length());
	strShow.Printf(strFormat, strName, strDes);
	pTechMain->AppendCheckItem(ID_TECHINDEX_TRIX, strShow,	_T(""));

	strName = MTM;
	strDes  = wxT("MTM动力指标");	
	strFormat.Printf(wxT("%%s%%%ds"), MAXSPAN+strDes.length()-strName.length());
	strShow.Printf(strFormat, strName, strDes);
	pTechMain->AppendCheckItem(ID_TECHINDEX_MTM, strShow,	_T(""));

    //趋向指标结束
	//////////////////////////////////////////////////////////////////////////

	wxMenu *pTechVol = new wxMenu;
	pTech->AppendSubMenu(pTechVol, wxT("成交量指标"));

	strName = VOL;
	strDes  = wxT("成交量");	
	strFormat.Printf(wxT("%%s%%%ds"), MAXSPAN+strDes.length()-strName.length());
	strShow.Printf(strFormat, strName, strDes);
	pTechVol->AppendCheckItem(ID_TECHINDEX_VOL,	  strShow,	_T(""));	
//	pTechVol->Check(ID_TECHINDEX_VOL, strTechIndexName == VOL);


	wxMenu *pTechNL = new wxMenu;
	pTech->AppendSubMenu(pTechNL, wxT("能量指标"));

	strName = PSY;
	strDes  = wxT("心理线");	
	strFormat.Printf(wxT("%%s%%%ds"), MAXSPAN+strDes.length()-strName.length());
	strShow.Printf(strFormat, strName, strDes);
	pTechNL->AppendCheckItem(ID_TECHINDEX_PSY, strShow,	_T(""));

	strName = CRTECH;
	strDes  = wxT("CR能量指标");	
	strFormat.Printf(wxT("%%s%%%ds"), MAXSPAN+strDes.length()-strName.length());
	strShow.Printf(strFormat, strName, strDes);
	pTechNL->AppendCheckItem(ID_TECHINDEX_CR, strShow,	_T(""));

	strName = VR;
	strDes  = wxT("VR容量比率");	
	strFormat.Printf(wxT("%%s%%%ds"), MAXSPAN+strDes.length()-strName.length());
	strShow.Printf(strFormat, strName, strDes);
	pTechNL->AppendCheckItem(ID_TECHINDEX_VR, strShow,	_T(""));


	wxMenu *pTechLJ = new wxMenu;
	pTech->AppendSubMenu(pTechLJ, wxT("量价指标"));

	strName = ASI;
	strDes  = wxT("振动升降指标");	
	strFormat.Printf(wxT("%%s%%%ds"), MAXSPAN+strDes.length()-strName.length());
	strShow.Printf(strFormat, strName, strDes);
	pTechLJ->AppendCheckItem(ID_TECHINDEX_ASI, strShow,	_T(""));

	strName = OBV;
	strDes  = wxT("能量潮");	
	strFormat.Printf(wxT("%%s%%%ds"), MAXSPAN+strDes.length()-strName.length());
	strShow.Printf(strFormat, strName, strDes);
	pTechLJ->AppendCheckItem(ID_TECHINDEX_OBV, strShow,	_T(""));

	strName = EMV;
	strDes  = wxT("简易波动指标");	
	strFormat.Printf(wxT("%%s%%%ds"), MAXSPAN+strDes.length()-strName.length());
	strShow.Printf(strFormat, strName, strDes);
	pTechLJ->AppendCheckItem(ID_TECHINDEX_EMV, strShow,	_T(""));


	wxMenu *pTechFQX = new wxMenu;
	pTech->AppendSubMenu(pTechFQX, wxT("反趋向指标"));

	strName = BIAS;
	strDes  = wxT("乖离率");	
	strFormat.Printf(wxT("%%s%%%ds"), MAXSPAN+strDes.length()-strName.length());
	strShow.Printf(strFormat, strName, strDes);
	pTechFQX->AppendCheckItem(ID_TECHINDEX_BIAS, strShow,	_T(""));

	strName = KDJ;
	strDes  = wxT("随机指标");	
	strFormat.Printf(wxT("%%s%%%ds"), MAXSPAN+strDes.length()-strName.length());
	strShow.Printf(strFormat, strName, strDes);
	pTechFQX->AppendCheckItem(ID_TECHINDEX_KDJ,  strShow,	_T(""));

	strName = ROC;
	strDes  = wxT("变动速率");	
	strFormat.Printf(wxT("%%s%%%ds"), MAXSPAN+strDes.length()-strName.length());
	strShow.Printf(strFormat, strName, strDes);
	pTechFQX->AppendCheckItem(ID_TECHINDEX_ROC, strShow,	_T(""));

	strName = CCI;
	strDes  = wxT("顺势指标");	
	strFormat.Printf(wxT("%%s%%%ds"), MAXSPAN+strDes.length()-strName.length());
	strShow.Printf(strFormat, strName, strDes);
	pTechFQX->AppendCheckItem(ID_TECHINDEX_CCI, strShow,	_T(""));

	strName = RSI;
	strDes  = wxT("相对强弱指标（Relative Strength Index）");	
	strFormat.Printf(wxT("%%s%%%ds"), MAXSPAN+strDes.length()-strName.length());
	strShow.Printf(strFormat, strName, strDes);
	pTechFQX->AppendCheckItem(ID_TECHINDEX_RSI, strShow,	_T(""));
	
//////////////////////////////////////////////////////////////////////////
	wxMenu *pTechYLZC = new wxMenu;
	pTech->AppendSubMenu(pTechYLZC, wxT("压力支撑指标"));

	strName = BOLL;
	strDes  = wxT("布林带");	
	strFormat.Printf(wxT("%%s%%%ds"), MAXSPAN+strDes.length()-strName.length());
	strShow.Printf(strFormat, strName, strDes);
	pTechYLZC->AppendCheckItem(ID_TECHINDEX_BOLL, strShow,	_T(""));
	
	wxMenu *pTechBD = new wxMenu;
	pTech->AppendSubMenu(pTechBD, wxT("摆动指标"));

	strName = CDP;
	strDes  = wxT("逆市操作");	
	strFormat.Printf(wxT("%%s%%%ds"), MAXSPAN+strDes.length()-strName.length());
	strShow.Printf(strFormat, strName, strDes);
	pTechBD->AppendCheckItem(ID_TECHINDEX_CDP,  strShow,	_T(""));
	
	strName = UOS;
	strDes  = wxT("终极指标");	
	strFormat.Printf(wxT("%%s%%%ds"), MAXSPAN+strDes.length()-strName.length());
	strShow.Printf(strFormat, strName, strDes);
	pTechBD->AppendCheckItem(ID_TECHINDEX_UOS,  strShow,	_T(""));

	menuPopUp->AppendSeparator();
	wxMenu *pView = new wxMenu;
	menuPopUp->AppendSubMenu(pView, wxT("视图组合"));
	pView->AppendCheckItem(ID_VIEW_ONE,  wxT("一图组合"),	_T(""));
	pView->Check(ID_VIEW_ONE, m_nViewCount == 1);
	pView->AppendCheckItem(ID_VIEW_TWO,  wxT("二图组合"),	_T(""));
	pView->Check(ID_VIEW_TWO, m_nViewCount == 2);
	pView->AppendCheckItem(ID_VIEW_THREE,  wxT("三图组合"),	_T(""));
	pView->Check(ID_VIEW_THREE, m_nViewCount == 3);
	
	
	menuPopUp->AppendCheckItem(ID_VIEW_INFO, wxT("信息窗口"));
	menuPopUp->Check(ID_VIEW_INFO, m_bShowRightPanel);

	menuPopUp->AppendCheckItem(ID_KTLINE_ADD,	_T("加入我的合约"), _T(""));
	int bMax = 0;
	if(m_myFrame)
		bMax = m_myFrame->GetMaxSize();

	if(bMax == 1)
		menuPopUp->AppendCheckItem(ID_KTLINE_MAXSIZE,	_T("还原窗口大小"), _T(""));
	else
		menuPopUp->AppendCheckItem(ID_KTLINE_MAXSIZE,	_T("最大化"), _T(""));


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
	MAPsmark*  pMarks = m_pFData->GetMarksCommon(m_enumPhrase);
	if(pMarks == NULL)
		return;
	MAPsmark::iterator it = pMarks->begin();
	while(it != pMarks->end())
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
					m_pKLineCtrl->Refresh();
				}
				if(m_pKVolumeCtrl)
				{
					m_pKVolumeCtrl->Refresh();
				}
				if(m_pKTechIndex)
				{
					m_pKTechIndex->Refresh();
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
void CKLineView::OnTechIndexUseExpress(wxCommandEvent& WXUNUSED(event))
{	
	CTechIndexExpressDlg dlg(this);
	dlg.SetTechIndex(m_strTechName_ByMessage);
	dlg.ShowModal();
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

void CKLineView::OnVOL(wxCommandEvent& WXUNUSED(event))
{
	if(m_pKVolumeCtrl)
		m_pKVolumeCtrl->SetTechIndex(VOL);
}
void CKLineView::OnBBI(wxCommandEvent& WXUNUSED(event))
{
	if(m_pKLineCtrl)
		m_pKLineCtrl->SetTechIndex(BBI);
}
void CKLineView::OnEXPMA(wxCommandEvent& WXUNUSED(event))
{
	if(m_pKLineCtrl)
		m_pKLineCtrl->SetTechIndex(EXPMA);
}
void CKLineView::OnDMA(wxCommandEvent& WXUNUSED(event))
{
	if(m_pKTechIndex)
		m_pKTechIndex->SetTechIndex(DMA);
}
void CKLineView::OnSAR(wxCommandEvent& WXUNUSED(event))
{
	if(m_pKLineCtrl)
		m_pKLineCtrl->SetTechIndex(SAR);
}
void CKLineView::OnTRIX(wxCommandEvent& WXUNUSED(event))
{
	if(m_pKTechIndex)
		m_pKTechIndex->SetTechIndex(TRIX);
}
void CKLineView::OnMTM(wxCommandEvent& WXUNUSED(event))
{
	if(m_pKTechIndex)
		m_pKTechIndex->SetTechIndex(MTM);
}
void CKLineView::OnCR(wxCommandEvent& WXUNUSED(event))
{
	if(m_pKTechIndex)
		m_pKTechIndex->SetTechIndex(CRTECH);
}
void CKLineView::OnVR(wxCommandEvent& WXUNUSED(event))
{
	if(m_pKTechIndex)
		m_pKTechIndex->SetTechIndex(VR);
}
void CKLineView::OnOBV(wxCommandEvent& WXUNUSED(event))
{
	if(m_pKTechIndex)
		m_pKTechIndex->SetTechIndex(OBV);
}
void CKLineView::OnASI(wxCommandEvent& WXUNUSED(event))
{
	if(m_pKTechIndex)
		m_pKTechIndex->SetTechIndex(ASI);
}
void CKLineView::OnEMV(wxCommandEvent& WXUNUSED(event))
{
	if(m_pKTechIndex)
		m_pKTechIndex->SetTechIndex(EMV);
}
void CKLineView::OnCCI(wxCommandEvent& WXUNUSED(event))
{
	if(m_pKTechIndex)
		m_pKTechIndex->SetTechIndex(CCI);
}
void CKLineView::OnRSI(wxCommandEvent& WXUNUSED(event))
{
	if(m_pKTechIndex)
		m_pKTechIndex->SetTechIndex(RSI);
}
void CKLineView::OnCDP(wxCommandEvent& WXUNUSED(event))
{
	if(m_pKLineCtrl)
		m_pKLineCtrl->SetTechIndex(CDP);
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
void CKLineView::OnViewOne(wxCommandEvent& WXUNUSED(event))
{
	m_nViewCount = 1;
	OnView1();	
}
void CKLineView::OnView1()
{
	m_splitterTop->Unsplit(m_pKVolumeCtrl);
	m_splitterBottom->Unsplit(m_pKTechIndex);
	m_pKLineCtrl->Show(true);
	m_pKVolumeCtrl->Show(false);
	m_pKTechIndex->Show(false);
	m_splitter->SetSashGravity(1);	
	m_splitter->SetSashPosition(-20);
	m_splitter->SetWindowStyleFlag(GetWindowStyleFlag() | wxSP_NOSASH);
	wxRect rtClient = GetClientRect();
	m_splitter->SetSize(rtClient.width, rtClient.height);
	m_splitter->UpdateSize();	
}
void CKLineView::OnViewTwo(wxCommandEvent& WXUNUSED(event))
{
	m_nViewCount = 2;
	OnView2();
}
void CKLineView::OnView2()
{
	if(m_pKLineCtrl && m_pKVolumeCtrl)
		m_splitterTop->SplitHorizontally(m_pKLineCtrl, m_pKVolumeCtrl);		
	//	if(m_pKTechIndex && m_pKScalesCtrl)
	//		m_splitterBottom->SplitHorizontally(m_pKTechIndex, m_pKScalesCtrl,-20);	

	m_splitterBottom->Unsplit(m_pKTechIndex);
	m_pKLineCtrl->Show(true);
	m_pKVolumeCtrl->Show(true);
	m_pKTechIndex->Show(false);
	wxRect rtClient = GetClientRect();
	m_splitter->SetSize(rtClient.width, rtClient.height);
	m_splitter->SetSashGravity(0.75);	
	m_splitter->SetSashSize(1.0);

	m_splitter->SetSashPosition(-20);
	m_splitter->UpdateSize();

	m_splitter->SetWindowStyleFlag(GetWindowStyleFlag() | wxSP_NOSASH);

	int nKV = (rtClient.GetHeight()-20)*0.67;
	m_splitterTop->SetSashPosition(nKV);
	m_splitterTop->SetSashGravity(0.67);
	m_splitterTop->SetSashSize(1);
	m_splitterTop->UpdateSize();	

	
	m_splitterBottom->SetSashGravity(1.0);
	m_splitterBottom->SetSashSize(1);	
	m_splitterBottom->SetSashPosition(-20);	
	m_splitterBottom->UpdateSize();
}
void CKLineView::OnViewThree(wxCommandEvent& WXUNUSED(event))
{
	m_nViewCount = 3;
	OnView3();
}
void CKLineView::OnView3()
{
	m_pKLineCtrl->Show(true);
	m_pKVolumeCtrl->Show(true);
	m_pKTechIndex->Show(true);
	if(m_pKLineCtrl && m_pKVolumeCtrl)
		m_splitterTop->SplitHorizontally(m_pKLineCtrl, m_pKVolumeCtrl);		
	if(m_pKTechIndex && m_pKScalesCtrl)
		m_splitterBottom->SplitHorizontally(m_pKTechIndex, m_pKScalesCtrl,-20);	

	wxRect rtClient = GetClientRect();
	m_splitter->SetSize(rtClient.width, rtClient.height);
	m_splitter->SetSashGravity(0.75);	
	m_splitter->SetSashSize(1.0);

	long style = GetWindowStyleFlag();
	style&=~wxSP_NOSASH;
	m_splitter->SetWindowStyleFlag(style);

	int nKV = rtClient.GetHeight() *0.75;
	m_splitter->SetSashPosition(nKV);
	m_splitter->UpdateSize();

	int nK = rtClient.GetHeight() *0.75 *0.67;
	m_splitterTop->SetSashPosition(nK);
	m_splitterTop->SetSashGravity(0.67);	
	m_splitterTop->SetSashSize(1);
	m_splitterTop->UpdateSize();	

	m_splitterBottom->SetSashGravity(1.0);
	m_splitterBottom->SetSashSize(1);	
	m_splitterBottom->SetSashPosition(-20);	
	m_splitterBottom->UpdateSize();
}
wxString CKLineView::GetTechIndex(KTYPE KType)
{
	if(KType == KTYPE_KLINE)
	{
		if(m_pKLineCtrl)
			return m_pKLineCtrl->GetTechIndexName();
	}
	else if(KType == KTYPE_VOLUME)
	{
		if(m_pKVolumeCtrl)
			return m_pKVolumeCtrl->GetTechIndexName();
	}
	else if(KType == KTYPE_TECHINDEX)
	{
		if(m_pKTechIndex)
			return m_pKTechIndex->GetTechIndexName();
	}
	return "";
}
void CKLineView::OnViewInfo(wxCommandEvent& WXUNUSED(event))
{
	wxWindow* window= GetParent();
	wxCommandEvent myEvent(wxEVT_KLINEVIW_INFO);
	myEvent.SetClientData(this);
	window->ProcessEvent(myEvent);
}
void CKLineView::OnKTLineAdd(wxCommandEvent& event)
{
	if(m_myFrame)
		m_myFrame->SetAddMySelect();
}
void CKLineView::OnKTLineMaxSize(wxCommandEvent& event)
{
	if(m_myFrame)
		m_myFrame->SendMaxMize();
}
