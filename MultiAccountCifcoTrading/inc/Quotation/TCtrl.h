#pragma once
#include "stdafx.h"
#include "wx/defs.h"

#include "wx/window.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
//#include "KItemData.h"

#include <time.h>
#include "wx\msw\font.h"
#include "../inc/Quotation/FData.h"

DECLARE_EVENT_TYPE(wxEVT_TCtrl_KeyDown, 3500)
DECLARE_EVENT_TYPE(wxEVT_TCtrl_LBUTTONDOWN, 3501)
DECLARE_EVENT_TYPE(wxEVT_TCtrl_MOUSEMOVE, 3502)
DECLARE_EVENT_TYPE(wxEVT_TCtrl_RBUTTONUP, 3503)
DECLARE_EVENT_TYPE(wxEVT_TCtrl_MOUSELEAVE, 3504)
DECLARE_EVENT_TYPE(wxEVT_TCtrl_LEFTDBCLICK, 3505)
DECLARE_EVENT_TYPE(wxEVT_TCtrl_MOUSEENTER, 3506)


class   CTCtrl : public wxPanel
{
public:
	 CTCtrl(void);
	 ~CTCtrl(void);

bool Create(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER|wxWANTS_CHARS|wxCLIP_CHILDREN,
		const wxString& name = wxPanelNameStr);


protected:
	wxColour			m_clBackGround; //背景色
	wxColour			m_clBorder;               //边框色
	wxColour			m_cldashLine;
	wxColour			m_clPrice;
	wxColour			m_clAveragePrice;
	wxColour			m_clCurSelLine;
	wxColour			m_clTipsBk;

	wxColour			m_clName; 
	wxColour			m_clTime; //时间
	wxColour			m_zhang;  //涨
	wxColour			m_die;    //跌
	wxColour			m_hengpan;//横盘
	wxColour			m_clLineName;//线的性质

	int					m_nBorderWidth;           //边框宽度
	vector<STLine *>	m_vecTLine;
	wxRect				m_RectOffSet;


	string			 m_ExchangeName; //交易所名称
	string			 m_ContractName; //合约名称
	string			 m_ContractDate; //合约到期
	//struct tm		 m_tmLastDay;

	int				 m_nBegin; //开始数值要大
	int				 m_nEnd;   //结束数值小
	
	

	double			 m_fLastClosePrice;//昨日收盘价
	double			 m_fMaxValue;
	double			 m_fMinValue;
	int				 m_nDayCount;
	int				  m_nOneDayScalesCount;
	vector<struct tm> m_vecTimeScales;//小到大排序
	vector<DWORD>     m_vecTMWidthSpan;
	DWORD			  m_TMWidthCount;  //交易时间刻度总共大小

	wxPoint		m_ptMouseMoveLast;
	bool		m_bIsMouse;    //鼠标消息
	bool		m_bIsKeyBoard; //键盘消息

	bool		m_bShowHeight;
	bool		m_bShowTime;
	int			m_nCurSel;
	STItem		m_sItem;
	int			m_nLeftPanelHeight;

	bool		m_bShowCross;			//显示十字光标
	bool		m_bMouseIn;			//鼠标是不是在里面
protected:
	virtual void		CalcMaxMin();
	virtual void		GetBeginEnd();
	virtual int			Time2X(struct tm tmTime);
	virtual struct tm	X2Time(int nx);
	virtual int			WeiZhi2XuHao(int nx);
	virtual int			XuHao2WeiZhi(int nItem);
	virtual int			JiaGe2GaoDu(float fJiaGe);
	virtual float		GaoDu2JiaGe(int nHeight);
	void DateTime2Tm(wxDateTime dt ,struct tm& tmDt);
	

	void OnLeftDown(wxMouseEvent& event);
	void OnLeftDBClick(wxMouseEvent& event);	
	void OnLeftUp(wxMouseEvent& event);
	void OnMouseMove(wxMouseEvent &event);
	void OnMouseLeave(wxMouseEvent &event);
	void OnMouseEnter(wxMouseEvent &event);
	void OnRightUp(wxMouseEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnChar( int nkeyCode );
public:	
	void DoMouseLeave();
	void DoMouseMove(wxPoint mousePos);

public:
	void SetFuture(string ExchangeName,string ContractName,string ContractDate);
	virtual BOOL SetVectorData(vector<STLine *>& veCTCtrl);
	void SetScales(vector<struct tm>& vecScales);
	void SetLastClosePrice(double dbPrice);
	virtual void SetCursel(STItem& sItem);
	BOOL SetVectorData_InTime(vector<STLine *>& veCTCtrl);
	void SetVolumeMultiple(double dbVolumeMultiple);//量和成交金额之间的乘数，比如股票1手是100
	void SetShowOneItemData(bool bShowOneItemData);
	bool InitCfg(TiXmlElement *root);
	void SetMaxMinValue(double& dbMaxValue, double& dbMinValue);
	void SetFData(CFData *pFData);
	void SetShowCross(bool bShowCross);
	bool GetShowCross(){ return m_bShowCross; };
	void SetMouseIn(bool bMouseIn);

public://config专用
	void SetBackGround(unsigned long lValue);
	void SetBorder(unsigned long lValue);
	void SetdashLine(unsigned long lValue);
	void SetPriceColor(unsigned long lValue);
	void SetAveragePriceColor(unsigned long lValue);
	void SetCurSelLineColor(unsigned long lValue);
	void SetTipsBkColor(unsigned long lValue);
	void SetNameColor(unsigned long lValue);
	void SetTimeColor(unsigned long lValue);
	void SetzhangColor(unsigned long lValue);
	void SetdieColor(unsigned long lValue);
	void SethengpanColor(unsigned long lValue);
	void SetLineNameColor(unsigned long lValue);
	void SetSpanLine(int nLineType, int	nLineSize);
	void SetHourLineLine(int nLineType, int	nLineSize);
	void SetHalfHourLine(int nLineType, int	nLineSize);
	void SetLeftSpanWidth(int nWidth);//设置左右刻度区的宽度
 protected:
    DECLARE_DYNAMIC_CLASS (CTCtrl)
    DECLARE_EVENT_TABLE()
	void DrawDC(wxDC& dc);
	void OnPaint (wxPaintEvent & PaintEvent);

	void DrawBorder(wxDC& dc, wxRect& rt);
	void DrawScales(wxDC& dc);
	struct tm GetDrawTime(vector<struct tm>& vecTimeScales, vector<DWORD>& vecTMWidthSpan, DWORD dwSpan);
	void DrawHead(wxDC& dc);
	void DrawPriceLines(wxDC& dc);
	bool DrawPriceLines_NoData(wxDC& dc, STLine *&pTLineEnd1, STLine *&pTLineBegin1);
	void DrawAveragePriceLines(wxDC& dc);
	void SetTextColor(wxDC& dc, double dbClosePrice);
	void DrawCurSelLine(wxDC& dc);
	void DrawTips(wxDC& dc);

	void DrawTips_Time(wxDC& dc, wxRect rtClient, int nX, bool bIsMouse);
	void DrawOneItemData(wxDC& dc, wxRect rtClient);//画左边价格提示框，不得已在k线模块里面画，其实这个应该单独一个模块
//	bool m_bShowOneItemData;
	void SItemGetValue(bool	bIsMouse, bool bIsKeyBoard);
	
protected:
	WXLRESULT MSWWindowProc(WXUINT   message,   WXWPARAM   wParam,   WXLPARAM   lParam) ;
	
	double		m_dbVolumeMultiple;
	int			GetDrawScalesCount(wxDC& dc);
	
	CFData		*m_pFData;//单只期货行情数据
	int			m_nNumDigits;


	int         m_nSpanLineType;		//大时间间隔线类型 0：点划线；1：实体线
	int			m_nSpanLineSize;		//大时间间隔线宽度
	int         m_nHourLineType;		//小时间间隔线类型 0：点划线；1：实体线
	int			m_nHourLineSize;		//小时间间隔线宽度
	int         m_nHalfHourLineLineType;//半小时间间隔线类型 0：点划线；1：实体线
	int			m_nHalfHourLineSize;	//半小时间间隔线宽度
};
