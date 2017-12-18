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

DECLARE_EVENT_TYPE(wxEVT_KCtrl_KeyDown, 3000)
DECLARE_EVENT_TYPE(wxEVT_KCtrl_LBUTTONDOWN, 3001)
DECLARE_EVENT_TYPE(wxEVT_KCtrl_MOUSEMOVE, 3002)
DECLARE_EVENT_TYPE(wxEVT_KCtrl_RBUTTONUP, 3003)
DECLARE_EVENT_TYPE(wxEVT_KCtrl_MOUSELEAVE, 3004)
DECLARE_EVENT_TYPE(wxEVT_KCtrl_LEFTDBCLICK, 3005)
DECLARE_EVENT_TYPE(wxEVT_KCtrl_MARKSCHANGE, 3009)
DECLARE_EVENT_TYPE(wxEVT_KCtrl_MOUSEENTER, 3010)
DECLARE_EVENT_TYPE(wxEVT_KCtrl_RBUTTONDOWN, 3011)
DECLARE_EVENT_TYPE(wxEVT_KCtrl_MOUSEMOVEHOOK, 3012)





class   CKCtrl : public wxPanel
{
public:
	 CKCtrl(void);
	 ~CKCtrl(void);

bool Create(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER|wxWANTS_CHARS|wxCLIP_CHILDREN,
		const wxString& name = wxPanelNameStr);


protected:
	vector<SKLine *> m_vecKLine;        //由最近到远的数据
	wxColour  m_clBackGround;           //背景色
	wxColour  m_clBorder;               //边框色

	wxColour	m_clRedLine;
	wxColour	m_clGreenLine;
	wxColour	m_cldashLine;
	wxColour	m_clCurSelLine;

	wxColour	m_clTipsBk;
	


	

	int       m_nBorderWidth;           //边框宽度
	
	int		  m_nShowCount;//一共显示多少根k线
	double	  m_dbDayWidth;//日线宽度
	int		  m_nDaySpan;//日线间距
	int       m_nBegin;//开始日线
	int		  m_nEnd;//结束日线在数组中序号
	int		  m_nCurSel;

	double	  m_fMaxValue;
	double	  m_fMinValue;
	wxRect	  m_RectOffSet;

	int		  m_nMaxPos;
	int		  m_nMinPos;//用来画最高最低价

	SItem	  m_sItem;
	UINT	  m_uTimerarr[2];
	BOOL	  m_bMaxMinChange;
	wxTimer	  m_timer1;
	wxTimer	  m_timer2;

	int		   m_nLeftPos;
	int		   m_nRightPos;

	wxWindow   *m_pMsgWnd;

	bool		m_bIsMouse;    //鼠标消息
	bool		m_bIsKeyBoard; //键盘消息

	bool		m_bShowHeight;
	bool		m_bShowTime;

	EnumPhrase		 m_enumPhrase;   //时间周期


//KLine中用到
	wxColour	m_clLineName;//线的性质

	wxPoint		m_ptMouseMoveLast;
public:
	void SetFData(CFData *pFData);
	BOOL SetVectorData(vector<SKLine *>& vecKLine);
	void SetPhrase(EnumPhrase enumPhrase); 
	EnumPhrase  GetPhrase();
	virtual void SetCursel(SItem& sItem);
	void SetMsgWnd(wxWindow *pMsgWnd);
	void SetMarkType(EnumMark enumMark);
	void SetMarks();	
	virtual BOOL SetVectorData_InTime(vector<SKLine *>& vecKLine); //最新的实时行情

	void SetShowCross(bool bShowCross);
	bool GetShowCross(){ return m_bShowCross; };
	void SetMouseIn(bool bMouseIn);
	virtual bool InitCfg(TiXmlElement *root);

	wxString    GetTechIndexName();
public://config专用
	void SetBackGround(unsigned long lValue);
	void SetBorder(unsigned long lValue);
	void SetRedLine(unsigned long lValue);
	void SetGreenLine(unsigned long lValue);
	void SetdashLine(unsigned long lValue);
	void SetCurSelLine(unsigned long lValue);
	void SetTipsBk(unsigned long lValue);
	void SetLineName(unsigned long lValue);
	void SetLeftpanelName(unsigned long lValue);
	void SetLeftpanelTime(unsigned long lValue);
	void SetLeftpanelzhang(unsigned long lValue);
	void SetLeftpaneldie(unsigned long lValue);
	void SetLeftpanelhengpan(unsigned long lValue);
	void SetTrangleUp(unsigned long lValue);
	void SetTrangleDown(unsigned long lValue);
	void SetLineColor(int nIndex, unsigned long lValue);
	void SetLeftSpanWidth(int nWidth);//设置左右刻度区的宽度

 protected:
    DECLARE_DYNAMIC_CLASS (CKCtrl)
    DECLARE_EVENT_TABLE()

	void	OnTimer1(wxTimerEvent& event);
	virtual int		JiaGe2GaoDu(float fJiaGe);
	virtual float	GaoDu2JiaGe(int nHeight);
	virtual int		WeiZhi2XuHao(int nWidthPos);
	virtual float   XuHao2WeiZhi(int nItem, int& nLeft, int& nRight);

	int  	time2WeiZhi(struct tm tmTime, int& nLeft, int& nRight);
	virtual void	GetBeginEnd();
	virtual void    CalcMaxMin();
	void OnChar( int nkeyCode );
	bool GetShowCount(int  nMax);//nMax 1:放大; -1:缩小;0保持不变
	void SItemGetValue(bool	bIsMouse, bool bIsKeyBoard);

	void OnPaint (wxPaintEvent & PaintEvent);
	void DrawDC(wxDC& dc);
	void DrawHead(wxDC& dc);
	void DrawBorder(wxDC& dc, wxRect& rt);
	void DrawItems(wxDC& dc);
	void DrawOneItem(wxDC& dc, int nLeft, int nRight, int nTop1, int nTop2, int nTop3, int nTop4);
	void DrawScales(wxDC& dc);
	void DrawZuiGaoZuiDiJia(wxDC& dc);
	void DrawCurSelLine(wxDC& dc);
	void DrawTips(wxDC& dc);
	
	virtual void DrawTips_Time(wxDC& dc, wxRect rtClient, int nX);
	void DrawOneItemData(wxDC& dc, wxRect rtClient);//画左边价格提示框，不得已在k线模块里面画，其实这个应该单独一个模块
	bool m_bShowCross;			//显示十字光标
	bool m_bMouseIn;			//鼠标是不是在里面
	
	
	void OnLeftDown(wxMouseEvent& event);
	void OnLeftDBClick(wxMouseEvent& event);	
	void OnRightDown(wxMouseEvent& event);	
	void OnLeftUp(wxMouseEvent& event);
	void OnMouseMove(wxMouseEvent &event);
	void OnMouseLeave(wxMouseEvent &event);
	void OnMouseEnter(wxMouseEvent &event);
public:	
	void DoMouseLeave();
protected:
	void OnSize(wxSizeEvent& event);
	void OnRightUp(wxMouseEvent& event);
	


	void timeToSting(time_t time1, char *szTime);

	void GetLineName(wxString& strLineName);
	

protected://字体颜色，左侧上角提示框专用
	wxColour m_LeftpanelName; 
	wxColour m_LeftpanelTime; //时间
	wxColour m_Leftpanelzhang;  //涨
	wxColour m_Leftpaneldie;    //跌
	wxColour m_Leftpanelhengpan;//横盘

	void SetTextColor(wxDC& dc, double dbClosePrice);
	void GetTimeText(EnumPhrase	 enumPhrase, wxString& strText, wxString& strText2);

	int  m_nLeftPanelHeight;

protected://图像画图用的
	EnumMark			m_enumMark;
//	MAPsmark			m_mapMarks;
	int					m_KType;//是在哪个模块里面标注的
	void				SetKType(int nKType);//这个函数主要用于解决相同类型模块出现不同标注的问题；比如两个技术指标模块，出现标注的定位

	virtual void DrawMarks(wxDC& dc);

	void DrawTriangleUp(wxDC& dc, struct tm tm1, double dbPrice, bool bSelected);
	void DrawTriangleDown(wxDC& dc, struct tm tm1, double dbPrice, bool bSelected);

	wxColour		m_clTrangleUp;			//画图用的，向上三角形的颜色
	wxColour		m_clTrangleUpBorder;    //边框颜色
	wxColour		m_clTrangleDown;		//画图用的，向下三角形的颜色
	wxColour		m_clTrangleDownBorder;  //边框颜色


	public:
		virtual void DoMouseMove(wxPoint mousePos);
		void RefreshTip();

	WXLRESULT MSWWindowProc(WXUINT   message,   WXWPARAM   wParam,   WXLPARAM   lParam);


	CFData		*m_pFData;//单只期货行情数据
	int			m_nNumDigits;

	CTechIndex* m_pTechIndex;
	wxString    m_strTechIndexName;


public:
	void DrawTechIndex(wxDC& dc);//画技术指标
	wxColour m_clLineColor[COLOR_MAX];

	void OnConfigChange(wxCommandEvent& event);
	void SetTechIndex(wxString strTechIndex);


};
