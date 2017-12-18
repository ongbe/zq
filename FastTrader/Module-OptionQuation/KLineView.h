#pragma once
#include "afxwin.h"
#define  TECHINDEX_MAX 2

// CKLineView
#include "FData.h"
#include "TechIndex.h"
enum enumKType
{	
	enumKLine=2,   //k线图
	enumKVolume, //k线量能
	enumKScales,  //k线刻度
	enumKLeftBar, //k线左边信息框

	enumKSplitBar,//分隔条
};
struct SHitTestKView
{
	enumKType	eType;	
	int			nIndex; //如果分隔条，则是第几个分隔条
};
class __declspec(dllexport)  CKLineView : public CWnd
{
	DECLARE_DYNAMIC(CKLineView)

public:
	CKLineView();
	virtual ~CKLineView();
	BOOL Create(CRect rtClient, CWnd* pParentWnd, UINT nID);

	std::string    m_ExchangeName;
	std::string    m_ContractName;
	
	vector<SKLine> m_vecKLine;        //由最近到远的数据

	CBitmap			m_Bmp;
	
	COLORREF		m_clBackGround;           //背景色
	COLORREF		m_clBorder;               //边框色

	COLORREF		m_clRedLine;
	COLORREF		m_clGreenLine;
	COLORREF		m_cldashLine;
	COLORREF		m_clCurSelLine;

	COLORREF		m_clTipsBk;
	COLORREF		m_clSAR;//SAR技术指标的颜色
	COLORREF		m_clTriangle;
	COLORREF		m_clText;

	HFONT				m_hSysFontHead;//头部字体
	CFont				m_FontHead;

	HFONT				m_hSysFontTip;//提示框字体
	CFont				m_FontTip;


	HFONT				m_hSysFontLeft;//左边信息框字体
	CFont				m_FontLeft;    //左边文字的字体

	HFONT				m_hSysFont1;//左右侧刻度字体
	CFont				m_FontScales;

	HFONT				m_hSysFontDownScalesLeft;//下刻度尺左侧字体
	CFont				m_FontDownScalesLeft;    //下刻度尺左侧字体

	int				m_nBorderWidth;           //边框宽度

	int				m_nShowCount;//一共显示多少根k线
	double			m_dbDayWidth;//日线宽度
	int				m_nDaySpan;//日线间距
	int				m_nBegin;//开始日线
	int				m_nEnd;//结束日线在数组中序号
	int				m_nCurSel;

	double			m_fMaxValue;
	double			m_fMinValue;
	CRect			m_RectOffSet;

	int				m_nMaxPos;
	int				m_nMinPos;//用来画最高最低价


	UINT			m_uTimerarr[2];
	BOOL			m_bMaxMinChange;


	int				m_nLeftPos;
	int				m_nRightPos;

	bool			m_bIsMouse;    //鼠标消息
	bool			m_bIsKeyBoard; //键盘消息

	bool			m_bShowHeight;
	bool			m_bShowTime;

	EnumPhrase		 m_enumPhrase;   //时间周期


	//KLine中用到
	COLORREF		m_clLineName;//线的性质

	CPoint			m_ptMouseMoveLast;

	bool			m_bShowCross;			//显示十字光标
	bool			m_bMouseIn;				//鼠标是不是在里面
	int				m_nNumDigits;


	double			m_fMaxValue_KVolume;
	double			m_fMinValue_KVolume;

protected://字体颜色，左侧上角提示框专用
	COLORREF		m_LeftpanelName; 
	COLORREF		m_LeftpanelTime;	//时间
	COLORREF		m_Leftpanelzhang;	//涨
	COLORREF		m_Leftpaneldie;		//跌
	COLORREF		m_Leftpanelhengpan;	//横盘

	void SetTextColor(CDC* pDC, double dbClosePrice);
	void GetTimeText(EnumPhrase	 enumPhrase, CString& strText, CString& strText2);

	int				m_nLeftPanelHeight;

private:
	int		JiaGe2GaoDu(double fJiaGe);
	double	GaoDu2JiaGe(int nHeight);
	int		WeiZhi2XuHao(int nWidthPos);
	float   XuHao2WeiZhi(int nItem, int& nLeft, int& nRight);

	int  	time2WeiZhi(struct tm tmTime, int& nLeft, int& nRight);
	virtual void	GetBeginEnd();
	virtual void    CalcMaxMin();
	void OnChar( int nkeyCode );
	bool GetShowCount(int  nMax);//nMax 1:放大; -1:缩小;0保持不变
	void GetLineName(CString& strLineName);

	void	CalcMaxMin_Volume();
	int		JiaGe2GaoDu_Volume(double fJiaGe);
	double	GaoDu2JiaGe_Volume(int nHeight);

	CString GetWeekName(int week);
protected:
	void OnPaint();
	void DrawBitmap(CDC *pDC);
	void DrawScales(CDC *pDC);
	void DrawBorder(CDC *pDC);
	void DrawHead(CDC *pDC);
	void DrawItems(CDC *pDC);
	void DrawOneItem(CDC *pDC, int nLeft, int nRight, int nTop1, int nTop2, int nTop3, int nTop4);
	void DrawZuiGaoZuiDiJia(CDC *pDC);
	void DrawCommonLast(CDC* pDC);
	void DrawCurSelLine(CDC *pDC);
	void DrawTips(CDC *pDC);
	void DrawTips_Time(CDC* pDC, CRect rtClient, int nX);
	void DrawOneItemData(CDC* pDC,  CRect rtClient);

	//画量能
	void DrawBitmap_Volume(CDC *pDC);
	void DrawHead_Volume(CDC *pDC);
	void DrawScales_Volume(CDC *pDC);
	void DrawItems_Volume(CDC* pDC);
	void DrawOneItem_Volume(CDC* pDC, int nLeft, int nRight, int nTop1, bool IsZhang);
	//画刻度尺
	void DrawBitmap_Scales(CDC *pDC);
	void DrawLeftText(CDC *pDC);
	void DrawItems_Scales(CDC *pDC);
	void DrawItems_Day(CDC *pDC);
	void DrawItems_MINS(CDC *pDC);
	void ProcessMins(CDC *pDC, EnumPhrase enumPhrase, int i, int& nItemSpan, struct tm tm33, struct tm& tmPrevious, int& nFirstEnd);


	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:	
	void SetFuture(string ExchangeName,string ContractName);
	BOOL SetVectorData(vector<SKLine>& vecKLine);
	void SetPhrase(EnumPhrase enumPhrase); 
	EnumPhrase  GetPhrase(){ return m_enumPhrase;};
	void GetFuture(string& ContractName);	
	void SetMarks();	

	void SetShowCross(bool bShowCross);
	bool GetShowCross(){ return m_bShowCross; };
	void SetMouseIn(bool bMouseIn);

	
	int					m_nKViewCount;		//几个视图
	CRect				m_rtKLine;			//K线矩形区域

	CRect				m_rtKVolume;		//K量能矩形区域
	CRect				m_rtScales;			//K刻度尺矩形区域
	//////////////////////////////////////////////////////////////////////////
	bool					HitTest(CPoint pt, SHitTestKView& hitTest);
	eMouseModes				m_MouseMode;
	bool					m_bLbuttonDown;
	CPoint					m_LbuttonDownPoint; 
	int						m_nResizeRange; //2个像素之内开始变鼠标形状
	int						m_nMaxSpan;     //窗口最小为5个像素
	double					m_dbKLine;
	double					m_dbKVolume;

	void					OnView1();
	void					OnView2();

public:
	BOOL					SetInTimeData(CFData& fData);

	void OnRButtonUp(UINT nFlags, CPoint point);
	void OnToKLine();
	void OnToK1();
	void OnToK5();
	void OnToK15();
	void OnToK30();
	void OnToK60();
	void OnToKDay();
	void OnToKWeek();
	void OnToKMonth();
	void OnIndexExpress();
	void OnIndexConfig();

public:
	CTechIndex  m_pTechIndex[TECHINDEX_MAX];
	CString    m_strTechIndexName[TECHINDEX_MAX];

public:
	bool GetTechIndexNames(std::vector<std::string>& vecTechNames,  EnumPhrase& enumPhrase);
public:
	void DrawTechIndex(CDC *pDC);//画技术指标
	COLORREF m_clLineColor[COLOR_MAX];
	void SetTechIndex(CString strTechIndex);
	
	void DrawTechIndex_Volume(CDC *pDC);//画技术指标

	SHitTestKView  m_hitTestLast;
};


