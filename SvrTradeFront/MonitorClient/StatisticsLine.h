#pragma once

enum enumType
{
	enumHead =1, //分时图头部
	enumTLine,   //分时图
	enumTVolume, //分时图量能
	enumScales,  //分时图刻度
	enumLeftBar, //分时图左边信息框

	enumSplitBar,//分隔条
};
struct SHitTest
{
	enumType	eType;	
	int			nIndex; //如果分隔条，则是第几个分隔条
};
class CStatisticsLine : public CWnd
{
	DECLARE_DYNAMIC(CStatisticsLine)

public:
	CStatisticsLine();
	virtual ~CStatisticsLine();
	BOOL Create(CRect rtClient, CWnd* pParentWnd, UINT nID);

	CBitmap				m_Bmp;

	COLORREF			m_clBackGround;
	COLORREF			m_clBorder;					//边框色
	COLORREF			m_cldashLine;
	COLORREF			m_clPrice;
	COLORREF			m_clAveragePrice;
	COLORREF			m_clCurSelLine;
	COLORREF			m_clTipsBk;

	COLORREF			m_clName; 
	COLORREF			m_clTime;					//时间
	COLORREF			m_zhang;					//涨
	COLORREF			m_die;						//跌
	COLORREF			m_hengpan;					//横盘
	COLORREF			m_clLineName;				//线的性质

	int					m_nBorderWidth;				//边框宽度
	std::vector<StatData>		m_vecTLine;
	CRect				m_RectOffSet;


	std::string				m_ExchangeName;			//交易所名称
	std::string				m_ContractName;			//合约名称
	int					m_nShowCount;//一共显示多少根k线
	double				m_dbDayWidth;//日线宽度
	int					m_nDaySpan;//日线间距
	int					m_nBegin;				//开始数值要大
	int					m_nEnd;					//结束数值小

	double				m_fLastClosePrice;		//昨日收盘价
	double				m_fMaxValue;            //今日最大值
	double				m_fMinValue;            //今日最小值
	int					m_nDayCount;            
	int					m_nOneDayScalesCount;
	std::vector<struct tm>	m_vecTimeScales;		//小到大排序
	std::vector<DWORD>		m_vecTMWidthSpan;
	DWORD				m_TMWidthCount;			//交易时间刻度总共大小

	int					m_nLeftPanelHeight;    //左边信息框的高度
	double				m_dbVolumeMultiple;    
	int					m_nNumDigits;          //保留有效数字
 

	HFONT				m_hSysFontHead;//头部字体
	CFont				m_FontHead;

	HFONT				m_hSysFont1;//左右侧刻度字体
	CFont				m_FontScales;

	HFONT				m_hSysFontTip;//提示框字体
	CFont				m_FontTip;

	HFONT				m_hSysFontLeft;//左边信息框字体
	CFont				m_FontLeft;    //左边文字的字体

	HFONT				m_hSysFontBottomScales;//底部刻度字体
	CFont				m_FontBottomScales;

	CPoint				m_ptMouseMoveLast; //鼠标最后移动的位置
	bool				m_bIsMouse;    //鼠标消息
	bool				m_bIsKeyBoard; //键盘消息

	bool				m_bShowHeight;   //显示高度的tip
	bool				m_bShowTime;     //显示时间轴的tip
	int					m_nCurSel;		 //当前选中的条目号
	bool				m_bShowCross;			//显示十字光标
	bool				m_bMouseIn;			//鼠标是不是在里面


	STItem				m_sItem;
	int					m_nTViewCount;		//几个视图
	CRect				m_rtTLine;			//分时图线矩形区域
	CRect				m_rtTVolume;			//分时图量能矩形区域
	CRect				m_rtScales;			//分时图刻度尺矩形区域
public:	
	void SetMaxMinValue(double& dbMaxValue, double& dbMinValue);
	void SetScales(std::vector<struct tm>& vecScales);
	void SetLastClosePrice(double dbPrice);
	void SetVolumeMultiple(double dbVolumeMultiple);
	void SetFuture(std::string ExchangeName,std::string ContractName);
	BOOL SetVectorData(std::vector<StatData>& vec);
	void SetDelay(bool bDelay)
	{
		m_IsDelay = bDelay;
	}
protected:
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	void OnToKLine();

	//以下绘制分时图部分
	void	DrawBitmap_TLine(CDC *pDC);
	void    DrawBorder(CDC *pDC);
	void	DrawScales(CDC* pDC);
	void	DrawHead(CDC* pDC);
	void	DrawUpRecev(CDC* pDC);
	void	DrawUpSend(CDC* pDC);
	void	DrawDownRecev(CDC* pDC);
	void	DrawDownSend(CDC* pDC);
	void	DrawUpDelay(CDC* pDC);
	void	DrawDownDelay(CDC* pDC);

	bool	DrawPriceLines_NoData(CDC* pDC, StatData& pTLineEnd1, StatData& pTLineBegin1);
	//void	DrawAveragePriceLines(CDC* pDC);
	void	DrawCurSelLine(CDC* pDC);
	void	DrawTips(CDC* pDC);
	void	DrawTips_Time(CDC* pDC, CRect rtClient, int nX, bool bIsMouse);
	//void	DrawOneItemData(CDC *pDC,  CRect rtClient);
	void	SetTextColor(CDC* pDC, double dbClosePrice);
	void	DrawCommonLast(CDC* pDC);

	void		CalcMaxMin();
	void		GetBeginEnd();
	int			Time2X(struct tm tmTime);
	struct tm	X2Time(int nx);
	int			WeiZhi2XuHao(int nx);
	int			XuHao2WeiZhi(int nItem);
	int			JiaGe2GaoDu(double fJiaGe);
	double		GaoDu2JiaGe(int nHeight);
	void		DateTime2Tm(COleDateTime dt ,struct tm& tmDt);
	int			GetDrawScalesCount(CDC* pDC);
	//void		SItemGetValue(bool	bIsMouse, bool bIsKeyBoard);

	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

//以下TVolume相关函数和变量
//////////////////////////////////////////////////////////////////////////
	void		DrawBitmap_TVolume(CDC *pDC);		
	void		DrawScales_TVolume(CDC* pDC);
	void		DrawVolumeLine_TVolume(CDC* pDC);

	void		CalcMaxMin_TVolume();
	int			JiaGe2GaoDu_TVolume(double fJiaGe);
	double		GaoDu2JiaGe_TVolume(int nHeight);
	double		m_fMaxValue_TVolume;
	double		m_fMinValue_TVolume;

//////////////////////////////////////////////////////////////////////////
//刻度
	void		DrawBitmap_TScales(CDC* pDC);
	struct tm	GetDrawTime(std::vector<struct tm>& vecTimeScales, std::vector<DWORD>& vecTMWidthSpan, DWORD dwSpan);


 //////////////////////////////////////////////////////////////////////////
	bool					HitTest(CPoint pt, SHitTest& hitTest);
	eMouseModes				m_MouseMode;
	bool					m_bLbuttonDown;
	CPoint					m_LbuttonDownPoint; 
	int						m_nResizeRange; //2个像素之内开始变鼠标形状
	int						m_nMaxSpan;     //窗口最小为5个像素
	double					m_dbTLine;
	double					m_dbTVolume;
	void					OnView1();
	void					OnView2();

public:
//	BOOL					SetInTimeData(CFData& fData);

	COLORREF				m_clLineColor[7];
	bool					m_IsDelay;//延迟图
};


