// QView.cpp : implementation file
//

#include "stdafx.h"
#include "OptionQuation.h"
#include "QView.h"


// CQView dialog

IMPLEMENT_DYNAMIC(CQView, CDialog)

CQView::CQView(CWnd* pParent /*=NULL*/)
	: CDialog(CQView::IDD, pParent)
{

}

CQView::~CQView()
{
}

void CQView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CQView, CDialog)
	ON_BN_CLICKED(IDOK, &CQView::OnBnClickedOk)
	ON_WM_SIZE()
	ON_MESSAGE(MESSAGE_LEFTDBCLICK, OnMessageLDBClick)
	ON_MESSAGE(MESSAGE_RETURN, OnMessageReturn)
	ON_WM_ERASEBKGND()
	ON_MESSAGE(MESSAGE_CHANGEVIEW, OnMessageChangeView)
	ON_MESSAGE(MESSAGE_CHANGETimeSPAN, OnMessageChangeTimeSpan)
	ON_MESSAGE(MESSAGE_INDEX_EXPRESS, OnMessageIndexExpress)	
	ON_MESSAGE(MESSAGE_INDEX_CONFIG, OnMessageIndexConfig)	
END_MESSAGE_MAP()

BOOL CQView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;//CHeaderCtrl::OnEraseBkgnd(pDC);
}
// CQView message handlers
BOOL CQView::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CRect rtClient;
	GetClientRect(&rtClient);

	m_TlineView.Create(rtClient, this, 11112);
	m_TlineView.ShowWindow(SW_HIDE);

	m_KlineView.Create(rtClient, this, 11111);
	m_KlineView.ShowWindow(SW_SHOW);
	

	return TRUE; 
}
void CQView::OnBnClickedOk()
{
	

}

void CQView::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	
	CRect rtClient;
	GetClientRect(&rtClient);
	
	if(::IsWindow(m_TlineView.GetSafeHwnd()))
		m_TlineView.MoveWindow(rtClient);
	
	if(::IsWindow(m_KlineView.GetSafeHwnd()))
		m_KlineView.MoveWindow(rtClient);
}
LRESULT CQView::OnMessageLDBClick(WPARAM wP,LPARAM lP)
{
	int& n = *(int *)wP;
	if(n == 0)
	{//来自TLineView
		m_TlineView.ShowWindow(SW_HIDE);
		m_KlineView.ShowWindow(SW_SHOW);

		m_KlineView.SetFocus();
	}
	else if(n == 1)
	{
		m_TlineView.ShowWindow(SW_SHOW);
		m_KlineView.ShowWindow(SW_HIDE);
		m_TlineView.SetFocus();
	}
	return 1L;
}
LRESULT CQView::OnMessageReturn(WPARAM wP,LPARAM lP)
{
	int& n = *(int *)wP;
	if(n == 0)
	{//来自TLineView
		m_TlineView.ShowWindow(SW_HIDE);
		m_KlineView.ShowWindow(SW_SHOW);
		m_KlineView.SetFocus();
	}
	else if(n == 1)
	{
		m_TlineView.ShowWindow(SW_SHOW);
		m_KlineView.ShowWindow(SW_HIDE);
		m_TlineView.SetFocus();
	}
	return 1L;
}
LRESULT CQView::OnMessageChangeView(WPARAM wP,LPARAM lP)
{
	int& n = *(int *)wP;
	if(n == 0)
	{//来自TLineView
		m_TlineView.ShowWindow(SW_HIDE);
		m_KlineView.ShowWindow(SW_SHOW);

		m_KlineView.SetFocus();
	}
	else if(n == 1)
	{
		m_TlineView.ShowWindow(SW_SHOW);
		m_KlineView.ShowWindow(SW_HIDE);
		m_TlineView.SetFocus();
	}
	return 1L;
}
LRESULT CQView::OnMessageChangeTimeSpan(WPARAM wP,LPARAM lP)
{
	GetParent()->SendMessage(MESSAGE_CHANGETimeSPAN, wP, lP);
	return 1L;
}
LRESULT CQView::OnMessageIndexExpress(WPARAM wP,LPARAM lP)
{
	GetParent()->SendMessage(MESSAGE_INDEX_EXPRESS,wP,lP);
	return 1L;
}
LRESULT CQView::OnMessageIndexConfig(WPARAM wP,LPARAM lP)
{
	GetParent()->SendMessage(MESSAGE_INDEX_CONFIG,wP,lP);
	return 1L;
}