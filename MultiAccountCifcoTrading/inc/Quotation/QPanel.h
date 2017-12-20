#pragma once
#include "wx\msw\dialog.h"
#include "KSplitterWindow.h"
#include <string>
#include "../inc/Quotation/CritSec.h"
#include "../inc/Quotation/AutoLock.h"
#include "..\\inc\\ISvr\\BusinessData.hpp"
#include "../inc/Quotation/FData.h"
#include "../inc/Quotation/GridEx.h"
#define HQDANGSHU  10  //这个值不要随便改，如果改动则需要相应代码同步更改，防止数组越界
DECLARE_EVENT_TYPE(wxEVT_QPanelData_InTime, 3800)
class CQPanel :public wxPanel
{
public:
	CQPanel();

	wxWindow* CreateDlg(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER|wxCLIP_CHILDREN,
		const wxString& name = wxPanelNameStr);


	~CQPanel(void);

	DECLARE_DYNAMIC_CLASS (CQPanel)
	 
	DECLARE_EVENT_TABLE()

	
	void OnPaint (wxPaintEvent & PaintEvent);
	void OnSize(wxSizeEvent& event);
	void DrawBorder(wxDC& dc);
	int  DrawBaoJia(wxDC& dc);
	void DrawDetail(wxDC& dc, int YBegin);
	WXLRESULT MSWWindowProc(WXUINT   message,   WXWPARAM   wParam,   WXLPARAM   lParam); 
	void OnChar( int nkeyCode );
	void OnLeftDBClick(wxMouseEvent& event);
	void OnFuncKeyDown(wxCommandEvent& event);
public:
	

private:
	wxColour			 m_clBackGround; //背景色
	wxColour			 m_clBorder;

	wxColour			 m_Leftpanelzhang;  //涨
	wxColour			 m_Leftpaneldie;    //跌
	wxColour			 m_Leftpanelhengpan;//横盘

	wxColour			 m_clText;//
	wxColour			 m_clVolume;//量的颜色
	double				 m_dbLastdbPrice;//昨结算价
	int					 m_nXianShou;//现手
	wxString		     m_strKaiPing;
public:
	void Init();
	void SetFuture(string ExchangeName,string ContractName,string ContractDate);	//某只期货的名称
	void SetTextColor(wxDC& dc, double dbClosePrice);
	void SetInTimeData(PlatformStru_DepthMarketData& MarketDataInfo);

	void SetFData(CFData* pFData);
private:
	void GetTextDefaultValue(wxString& strText, double dbValue, double dbDefault);
	void GetTextDefaultValue_Volume(wxString& strText, double dbValue, double dbDefault);
	double GetAveragePrice();

private:
	string				m_ExchangeName; //交易所名称
	string				m_ContractName; //合约名称
	string				m_ContractDate; //合约到期

	wxColour			m_clContractName;//合约名称颜色


	
	PlatformStru_DepthMarketData m_MarketDataInfo;//行情信息

	
	
	CFData*				m_pFData;
	

//////////////////////////////////////////////////////////////////////////
//以上是TLine相关，下面是KLine相关
//////////////////////////////////////////////////////////////////////////


public://grid相关
	wxGridEx*					m_grid;
	BigGridTable*				m_table;

	vector<SRow>				m_vecRow;
	//map<int,vector<string>>*	m_pTextData;
	wxColour					m_clGridLabelText;//
	wxColour					m_clGridLabelBK;//

	int							m_nHeadFontSize;
	int							m_nDetailFontSize;

	int							m_nGridHeadFontSize;

	SQIntime *m_pItemLastShow;
	void SetGridLastItem();

	void InitGrid();

public:
	bool InitCfg(TiXmlElement *root);
	void SetHQCount(int nHQCount);
	void SetBackGround(unsigned long lValue);
	void SetBorder(unsigned long lValue);
	void SetLeftpanelzhang(unsigned long lValue);
	void SetLeftpaneldie(unsigned long lValue);
	void SetLeftpanelhengpan(unsigned long lValue);
	void SetContractName(unsigned long lValue);
	void SetclText(unsigned long lValue);
	void SetclVolume(unsigned long lValue);
	void SetclGridLabelText(unsigned long lValue);
	void SetHeadFontSize(int nHeadFontSize);
	void SetDetailFontSize(int nDetailFontSize);
	void SetGridHeadFontSize(unsigned long lValue);
private:
	void GetGridSize(wxPoint& pt, wxSize& rt);
	
	void SetOneItem(SQIntime *pItemLast, SQIntime *pItemLast2,int nRow, bool bRefresh = true);
	void OnFuncQPanelData_InTime(wxCommandEvent& event);

	bool m_bShowVisible;

	int				   m_nNumDigits;//有效数字
	void GetDescName(int nDesc, wxString& strDesc);

	int		m_nHQCount;					//五档行情，最多五档,最少一档
	double	m_BidPrice[HQDANGSHU];		//买入的十档行情
	int		m_BidVolume[HQDANGSHU];	//买入的十档量能
	double	m_AskPrice[HQDANGSHU];		//卖出的十档行情
	int		m_AskVolume[HQDANGSHU];	//卖出的十档量能




};
