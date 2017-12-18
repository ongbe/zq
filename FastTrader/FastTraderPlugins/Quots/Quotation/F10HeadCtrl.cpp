#include "stdafx.h"
#include "../../../inc/Quotation/F10HeadCtrl.h"
#include <time.h>



DEFINE_EVENT_TYPE(wxEVT_F10HeadCtrl_LBUTTONDOWN)
DEFINE_EVENT_TYPE(wxEVT_F10HeadCtrl_KeyDown)
IMPLEMENT_CLASS(CF10HeadCtrl, wxPanel)

CF10HeadCtrl::CF10HeadCtrl(void)
{
	m_clBackGround	= wxColour(RGB(0,0,0));
	m_clBorder		= wxColour(RGB(192,0,0));

	m_clSelect		=  RGB(82,255,255);				  //选中
	m_clNormal		=  RGB(255,255,82);				  //正常色
	m_nBorderWidth	= 3;
	m_nColCount		= 8;
	m_nRowCount		= 2;
	m_pFData		= NULL;

	m_nSelIndex = 0;
}
bool CF10HeadCtrl::Create(wxWindow *parent,
			wxWindowID winid ,
			const wxPoint& pos ,
			const wxSize& size,
			long style,
			const wxString& name )
{
	style |= wxWANTS_CHARS|wxTE_PROCESS_ENTER|wxCLIP_CHILDREN|wxCLIP_SIBLINGS;
	bool bReturn = wxPanel::Create(parent,winid,pos, size,	style, name);		
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);


	return bReturn;
}
void CF10HeadCtrl::InsertItem(int nRow, int nCol, wxString strItem, bool bSendMess)
{
	SHeadCtrlItem sItem;
	sItem.nCol		= nCol;
	sItem.nRow		= nRow;
	sItem.strName	= strItem;
	sItem.bSendMess = bSendMess;
	sItem.bSelect	= false;
	if(nRow == 0 && nCol == 0)
		sItem.bSelect	= true;
	m_aryHeadCtrlItem.push_back(sItem);
}
void CF10HeadCtrl::SetConfigStr(const char* str)
{
	string strName = str;
	vector<string> vec;
	split(strName, ' ', vec);
	if(vec.size() == 0)
		return;

	for(int i=0; i< (int)vec.size(); i++)
	{
		int nRow =0, nCol =i;
		if(i >= m_nColCount)
		{
			nRow = i/m_nColCount;
			nCol = i%m_nColCount;
		}
		wxString str = vec[i];
		InsertItem(nRow, nCol, str, true);
	}
}
void CF10HeadCtrl::GetArray(vector<struct SHeadCtrlItem>&  aryHeadCtrlItem)
{
	aryHeadCtrlItem.clear();
	aryHeadCtrlItem =m_aryHeadCtrlItem;
}
void CF10HeadCtrl::SetRowColCount(int nRow, int nCol)
{
	m_nRowCount = nRow;
	m_nColCount = nCol;
}
void CF10HeadCtrl::SetFData(CFData *pFData)
{
	m_pFData = pFData;	
	m_nSelIndex = 0;
}
CF10HeadCtrl::~CF10HeadCtrl(void)
{
	
}
BEGIN_EVENT_TABLE(CF10HeadCtrl, wxPanel)
	EVT_PAINT(CF10HeadCtrl::OnPaint)
	EVT_LEFT_DOWN(OnLeftDown)
//	EVT_LEFT_DOWN(CF10HeadCtrl::OnLeftDown)
//	EVT_SIZE(CF10HeadCtrl::OnSize)	
END_EVENT_TABLE()

void CF10HeadCtrl::OnPaint (wxPaintEvent & PaintEvent)
{
	wxPaintDC dc(this);
	wxMemoryDC memdc;  
	memdc.SetLayoutDirection(dc.GetLayoutDirection());
    wxBitmap bmp;
	wxSize size = GetClientSize();
    bmp.Create(size.GetWidth(), size.GetHeight());
    memdc.SelectObject(bmp);

	DrawDC(memdc);
	dc.Blit(0, 0, size.GetWidth(),size.GetHeight(),&memdc, 0, 0);
  
}
void CF10HeadCtrl::DrawDC(wxDC& dc)
{
	wxSize size = GetClientSize();
	wxBrush brush(m_clBackGround);
	dc.SetBrush(brush);
	dc.DrawRectangle(0, 0, size.x, size.y);
	
	wxRect rt(0, 0, size.x, size.y);
	DrawBorder(dc, rt);
	DrawText(dc);
}
void CF10HeadCtrl::DrawBorder(wxDC& dc, wxRect& rt)
{
	wxPen penBorder(wxColour(m_clBorder), m_nBorderWidth, wxSOLID);
	dc.SetPen(penBorder);
	dc.DrawLine(rt.GetLeft(), rt.GetTop(), rt.GetRight(), rt.GetTop());
	dc.DrawLine(rt.GetLeft(), rt.GetTop(), rt.GetLeft(), rt.GetBottom());
	dc.DrawLine(rt.GetLeft(), rt.GetBottom(), rt.GetRight(), rt.GetBottom());
	dc.DrawLine(rt.GetRight(), rt.GetBottom(), rt.GetRight(), rt.GetTop());

	
	penBorder.SetWidth(1);
	dc.SetPen(penBorder);
	dc.DrawLine(rt.GetLeft(), (rt.GetTop()+rt.GetBottom())/2, rt.GetRight(), (rt.GetTop()+rt.GetBottom())/2);

	for(int i=0; i< m_nColCount+1; i++)
	{
		
		dc.DrawLine(rt.GetLeft() + (rt.GetRight() - rt.GetLeft())*1.0/(m_nColCount+1) * i , rt.GetTop(),
			(rt.GetRight() - rt.GetLeft())*1.0/(m_nColCount+1) * i , rt.GetBottom());
	}

}
void CF10HeadCtrl::DrawText(wxDC& dc)
{
	if(m_pFData == NULL)
		return;
	wxFont wxfont(13, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT, false); 	
	dc.SetFont(wxfont);
	wxRect rt = GetItemRect(0,  -1);
	dc.SetClippingRegion(rt);
	wxString  strText = m_pFData->m_ContractName;
	wxCoord w,h;
	wxPoint pt;
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rt.GetLeft() + (rt.GetWidth() - w)/2;
	pt.y = rt.GetTop() + (rt.GetHeight() - h)/2;
	dc.SetTextForeground(m_clNormal); 
	dc.DrawText(strText, pt);

	rt = GetItemRect(1,  -1);
	dc.SetClippingRegion(rt);
	strText = m_pFData->m_ContractName;
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rt.GetLeft() + (rt.GetWidth() - w)/2;
	pt.y = rt.GetTop() + (rt.GetHeight() - h)/2;
	dc.SetTextForeground(m_clNormal); 
	dc.DrawText(strText, pt);

	dc.DestroyClippingRegion();

	for(int i =0; i<(int)m_aryHeadCtrlItem.size(); i++)
	{
		SHeadCtrlItem& sItem = m_aryHeadCtrlItem[i];		
		wxRect rt = GetItemRect(sItem.nRow,  sItem.nCol);
		sItem.rtItem = rt;
		dc.SetClippingRegion(rt);

		wxString  strText = sItem.strName;
		wxCoord w,h;
		wxPoint pt;
		 
		dc.GetTextExtent(strText, &w, &h);	
		pt.x = rt.GetLeft() + (rt.GetWidth() - w)/2;
		pt.y = rt.GetTop() + (rt.GetHeight() - h)/2;
		
		if(m_nSelIndex ==i)
			dc.SetTextForeground(m_clSelect); 
		else		
			dc.SetTextForeground(m_clNormal); 
		
		dc.DrawText(strText, pt);
		dc.DestroyClippingRegion();
	}


}
wxRect CF10HeadCtrl::GetItemRect(int nRow, int nCol)
{
	wxRect rtClient = GetClientRect();
	int nWidth  = rtClient.GetWidth()/(m_nColCount +1);
	int nHeight = rtClient.GetHeight()/m_nRowCount;

	int nLeft = rtClient.GetLeft() +nWidth+ nCol*nWidth;
	int nTop  = rtClient.GetTop() + nRow*nHeight;
	
	wxRect rt;
	rt.SetLeft(nLeft);
	rt.SetRight(nLeft + nWidth);
	rt.SetTop(nTop);
	rt.SetBottom(nTop + nHeight);

	return rt;
}
void CF10HeadCtrl::OnLeftDown(wxMouseEvent& event)
{
	SetFocus();
	wxPoint pt = wxGetMousePosition(); 
	ScreenToClient(&pt.x, &pt.y);
	SHeadCtrlItem sItemSed;
	int nIndex = m_nSelIndex;
	for(int i =0; i< (int)m_aryHeadCtrlItem.size(); i++)
	{
		SHeadCtrlItem& sItem = m_aryHeadCtrlItem[i];		
		wxRect rt = GetItemRect(sItem.nRow,  sItem.nCol);
		if(rt.Contains(pt))
		{
			if(m_nSelIndex == i)
				return;		
			m_nSelIndex = i;
			sItemSed = sItem;
			break;
		}
	}
	if(nIndex == m_nSelIndex)
		return;

	Refresh();
	wxWindow* window= GetParent();
	wxCommandEvent myEvent(wxEVT_F10HeadCtrl_LBUTTONDOWN);
	myEvent.SetClientData(&sItemSed);
	window->ProcessEvent(myEvent);
}
WXLRESULT CF10HeadCtrl::MSWWindowProc(WXUINT   message,   WXWPARAM   wParam,   WXLPARAM   lParam) 
{//wxpanel不支持return escape，所以在这里处理onchar	
	switch (message) 
	{ 
	case WM_KEYDOWN: 			
		{
			OnChar((int)wParam);
		}
		break;
	default:
		break;
	} 
	return wxPanel::MSWWindowProc(message,wParam,lParam); 
}
void CF10HeadCtrl::OnChar(int nkeyCode)
{	
	switch ( nkeyCode )
	{
	case VK_RETURN:
		{

		}
		break;
	case VK_ESCAPE:
		{

		}
		break;
	default:
		break;
	}	
	wxWindow *winSend = NULL;
	wxWindow* window= GetParent();//CHILDFrame
	wxCommandEvent myEvent(wxEVT_F10HeadCtrl_KeyDown);
	myEvent.SetInt(nkeyCode);
	window->ProcessEvent(myEvent);
}