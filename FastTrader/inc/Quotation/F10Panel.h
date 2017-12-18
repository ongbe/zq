#pragma once

#include "wx\msw\dialog.h"
#include "wx/richtext/richtextctrl.h"
#include "../inc/Quotation/wxRichTextCtrlEx.h"
#include "../inc/Quotation/F10HeadCtrl.h"
class CF10Panel :public wxPanel
{
public:
	CF10Panel();

	wxWindow* CreateDlg(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER|wxCLIP_CHILDREN,
		const wxString& name = wxPanelNameStr);


	~CF10Panel(void);

	DECLARE_DYNAMIC_CLASS (CF10Panel)
	 
	DECLARE_EVENT_TABLE()
	void OnFuncLButtonDown(wxCommandEvent& event);	
	void OnPaint (wxPaintEvent & PaintEvent);
	void DrawDC(wxDC& dc);
	void OnSize(wxSizeEvent& event);
	void OnKeyDown(wxCommandEvent& event);
	void OnEraseBackground(wxEraseEvent& event)
	{
		event.Skip();
	}
protected:
		wxRichTextCtrlEx*         m_richTextCtrl;
		CF10HeadCtrl			m_F10HeadCtrl;
		CFData*					m_pFData;
		wxColour				m_clBackGround;           //±³¾°É«
		wxColour				m_clBackText;           //±³¾°É«
public:
	  bool InitCfg(TiXmlElement *root);
	  void SetFData(CFData *pFData);

	  void LoadFile(SHeadCtrlItem& sItemSed);

	  vector<struct SHeadCtrlItem>  m_aryHeadCtrlItem;
};
