#include "StdAfx.h"

#include "../../../inc/Quotation/F10Panel.h"

#include "wx/dcmirror.h"





IMPLEMENT_CLASS(CF10Panel, wxPanel)
CF10Panel::CF10Panel()
{
	m_clBackGround	 = wxColour(RGB(0,0,0));
	m_clBackText	 = wxColour(RGB(255,255,82));
	m_richTextCtrl = NULL;
}

CF10Panel::~CF10Panel(void)
{
	
}

BEGIN_EVENT_TABLE(CF10Panel, wxPanel)
EVT_PAINT(CF10Panel::OnPaint)
EVT_SIZE(CF10Panel::OnSize)
EVT_COMMAND(wxID_ANY, wxEVT_F10HeadCtrl_LBUTTONDOWN, OnFuncLButtonDown)
EVT_COMMAND(wxID_ANY, wxEVT_RichTextCtrlEx_KeyDown,OnKeyDown)
EVT_COMMAND(wxID_ANY, wxEVT_F10HeadCtrl_KeyDown,OnKeyDown)
END_EVENT_TABLE()

wxWindow* CF10Panel::CreateDlg(wxWindow *parent,
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
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	
	m_F10HeadCtrl.Create(this, 32222, wxPoint(0, 0), wxSize(size.GetX(), 50) );
	m_F10HeadCtrl.Show();
	m_richTextCtrl = new wxRichTextCtrlEx(this, 32223, wxEmptyString, wxPoint(0, 50), wxSize(size.GetX(), size.GetY()-50), wxRE_READONLY|wxVSCROLL|wxNO_BORDER|wxWANTS_CHARS);
	wxFont wxfont(13, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT, false); 	
	m_richTextCtrl->SetFont(wxfont);
	m_richTextCtrl->SetBackgroundColour(m_clBackGround);	
	wxTextAttrEx attributes;
	attributes.SetTextColour(m_clBackText);
	m_richTextCtrl->SetBasicStyle(attributes);
	return this;
}
void CF10Panel::OnPaint (wxPaintEvent & PaintEvent)
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
void CF10Panel::DrawDC(wxDC& dc)
{
	wxSize size = GetClientSize();
	wxBrush brush(m_clBackGround);
	dc.SetBrush(brush);
	dc.DrawRectangle(0, 0, size.x, size.y);

}
void CF10Panel::OnSize(wxSizeEvent& event)
{
	if(!m_richTextCtrl)
		return;
	wxRect rtClient = GetClientRect();
	m_F10HeadCtrl.SetSize(rtClient.width, 50);
	m_F10HeadCtrl.Refresh();
	m_richTextCtrl->SetPosition(wxPoint(rtClient.GetLeft(), 50));
	m_richTextCtrl->SetSize(rtClient.width, rtClient.height -50);

}
void CF10Panel::SetFData(CFData *pFData)
{
	m_pFData = pFData;	
	m_F10HeadCtrl.SetFData(pFData);
	SHeadCtrlItem& sItemSed =  m_aryHeadCtrlItem[0];
	LoadFile(sItemSed);
}
bool CF10Panel::InitCfg(TiXmlElement *root)
{
	TiXmlElement *tList = root->FirstChildElement("F10Panel");
	if(tList == NULL)
		return false;

	TiXmlElement* colname = tList->FirstChildElement("colname");
	if(colname == NULL)
		return false;
	const char *ptype = colname->Attribute("typeid");
	unsigned long type = ColorStr2Long(ptype);
	
	const char *rowcount = colname->Attribute("rowcount");
	if(rowcount == NULL)
		return false;
	unsigned long row = ColorStr2Long(rowcount);

	const char *clocount = colname->Attribute("clocount");
	if(clocount == NULL)
		return false;
	unsigned long col = ColorStr2Long(clocount);
	
	m_F10HeadCtrl.SetRowColCount(row, col);


	const char *name = colname->Attribute("name");
	if(name == NULL)
		return false;
	
	m_F10HeadCtrl.SetConfigStr(name);
	m_F10HeadCtrl.GetArray(m_aryHeadCtrlItem);
	return true;
}

void CF10Panel::OnFuncLButtonDown(wxCommandEvent& event)
{
	if(m_pFData == NULL)
		return;

	SHeadCtrlItem& sItemSed = *(SHeadCtrlItem*)event.GetClientData();
	LoadFile(sItemSed);
	
}
void CF10Panel::LoadFile(SHeadCtrlItem& sItemSed)
{
	wxString strPath;
	m_pFData->GetFileSavePath(strPath);	
	wxString strDir;
	strDir.Printf(_T("%shqData\\"), strPath);
	wxString strSubDir;
	if(m_pFData->m_Simulate == 1)
		strSubDir.Printf(_T("%shqData_simi\\"), strDir);
	else
		strSubDir.Printf(_T("%shqData_Trans\\"), strDir);	

	strPath.Printf(_T("%s%s\\F10\\%s\\%s_%s.txt"), strSubDir, m_pFData->m_ExchangeName.c_str(), sItemSed.strName, m_pFData->m_ContractName.c_str(), sItemSed.strName );
	
	
	if(_access(strPath.c_str(), 0) != -1)
		m_richTextCtrl->LoadFile(strPath, 1);
	else
		m_richTextCtrl->Clear();
}
void CF10Panel::OnKeyDown(wxCommandEvent& event)
{
	int keyCode = event.GetInt();
	if(keyCode == WXK_ESCAPE)
	{
		wxWindow* window= GetParent();
		window->ProcessEvent(event);
	}
}