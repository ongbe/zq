// TabPage1_ListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "AlgoTradingNo1.h"
#include "TabPage1_ListCtrl.h"



// CTabPage1_ListCtrl dialog

IMPLEMENT_DYNAMIC(CTabPage1_ListCtrl, CDialog)

CTabPage1_ListCtrl::CTabPage1_ListCtrl(CWnd* pParent /*=NULL*/)
	: CDialog(CTabPage1_ListCtrl::IDD, pParent)
{

}

CTabPage1_ListCtrl::~CTabPage1_ListCtrl()
{
}

void CTabPage1_ListCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CTabPage1_ListCtrl, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CTabPage1_ListCtrl message handlers

// CAlgoTradingNo1Dlg 消息处理程序

BOOL CTabPage1_ListCtrl::OnInitDialog()
{
	CDialog::OnInitDialog();

	DWORD dwStyle=m_ListCtrl.GetExtendedStyle();
	dwStyle|=LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT;
	m_ListCtrl.SetExtendedStyle(dwStyle);

	m_ListCtrl.InsertColumn(0,"k",LVCFMT_LEFT,40);
	m_ListCtrl.InsertColumn(1,"tk=k*t",LVCFMT_LEFT,120);
	m_ListCtrl.InsertColumn(2,"xk",LVCFMT_LEFT,120);
	m_ListCtrl.InsertColumn(3,"nk",LVCFMT_LEFT,120);
	m_ListCtrl.InsertColumn(4,"xk有趋势",LVCFMT_LEFT,120);
	m_ListCtrl.InsertColumn(5,"nk有趋势",LVCFMT_LEFT,120);
	m_ListCtrl.InsertColumn(6,"已成交量",LVCFMT_LEFT,120);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


void CTabPage1_ListCtrl::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(m_ListCtrl.GetSafeHwnd()!=NULL)
		m_ListCtrl.MoveWindow(0,0,cx,cy);
}


void CTabPage1_ListCtrl::setcontent(const CAlgo_No1& algo)
{
	m_ListCtrl.DeleteAllItems();
	for(int j=0;j<(int)algo.vecxj.size();j++)
	{
		m_ListCtrl.InsertItem(j,CTools_AlgoTrading::StringFromInt(j).c_str());
		m_ListCtrl.SetItemText(j,1,CTools_AlgoTrading::StringFromDouble(j*algo.tau).c_str());
		m_ListCtrl.SetItemText(j,2,CTools_AlgoTrading::StringFromDouble(algo.vecxj[j]).c_str());
		m_ListCtrl.SetItemText(j,3,j==0?"":CTools_AlgoTrading::StringFromDouble(algo.vecnj[j-1]).c_str());
		m_ListCtrl.SetItemText(j,4,CTools_AlgoTrading::StringFromDouble(algo.vecxj2[j]).c_str());
		m_ListCtrl.SetItemText(j,5,j==0?"":CTools_AlgoTrading::StringFromDouble(algo.vecnj2[j-1]).c_str());
	}
}

//刷新已成交量。只刷新指定节。
void CTabPage1_ListCtrl::RefreshTradedVolume(int TradingSession,int TradedVolume)
{
	int ItemCnt=m_ListCtrl.GetItemCount();
	if(TradingSession>=0&&TradingSession+1<ItemCnt)
		m_ListCtrl.SetItemText(TradingSession+1,6,CTools_AlgoTrading::StringFromInt(TradedVolume).c_str());
}

