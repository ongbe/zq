#pragma once
#include "wx/defs.h"

#include "wx/window.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
//#include "KItemData.h"

#include <time.h>
#include "wx\msw\font.h"
#include "../inc/Quotation/FData.h"
DECLARE_EVENT_TYPE(wxEVT_F10HeadCtrl_KeyDown, 8001)
DECLARE_EVENT_TYPE(wxEVT_F10HeadCtrl_LBUTTONDOWN, 8000)
struct SHeadCtrlItem
{
	int			nCol;     //列
	int			nRow;     //行
	wxString	strName;  //名称
	DWORD		dwData;   //绑定的值
	wxRect		rtItem;	   //区域大小	
	bool		bSendMess; //是否发送消息
	bool		bSelect;// 是否选中
};
class   CF10HeadCtrl : public wxPanel
{
public:
	 CF10HeadCtrl(void);
	 ~CF10HeadCtrl(void);

bool Create(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER|wxWANTS_CHARS|wxCLIP_CHILDREN,
		const wxString& name = wxPanelNameStr);


protected:
	wxColour			m_clBackGround;           //背景色
	wxColour			m_clBorder;               //边框色

	wxColour			m_clSelect;				  //选中
	wxColour			m_clNormal;				  //正常色
	int					m_nBorderWidth;           //边框宽度

	int					m_nRowCount;
	int					m_nColCount;

	int					m_nSelIndex;
	CFData*				m_pFData;
public:
	void SetRowColCount(int nRow, int nCol);
	void SetConfigStr(const char* str);
	void InsertItem(int nRow, int nCol, wxString strItem, bool bSendMess);
	void SetFData(CFData *pFData);
	void GetArray(vector<struct SHeadCtrlItem>&  aryHeadCtrlItem);

	vector<struct SHeadCtrlItem>  m_aryHeadCtrlItem;

	
 protected:
    DECLARE_DYNAMIC_CLASS (CF10HeadCtrl)
    DECLARE_EVENT_TABLE()	

	void OnPaint (wxPaintEvent & PaintEvent);
	void DrawDC(wxDC& dc);
	void DrawBorder(wxDC& dc, wxRect& rt);
	void DrawText(wxDC& dc);
	WXLRESULT MSWWindowProc(WXUINT   message,   WXWPARAM   wParam,   WXLPARAM   lParam);
	void OnChar(int nkeyCode);

	wxRect GetItemRect(int nRow, int nCol);
	void OnLeftDown(wxMouseEvent& event);
};
