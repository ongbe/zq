#pragma once
#include "afxcmn.h"
#include "common-AlgoTradingNo1Dlg.h"
#include "Algo_No1.hpp"

#include <string>
#include <vector>
using namespace std;



// CTabPage1_ListCtrl dialog

class CTabPage1_ListCtrl : public CDialog
{
	DECLARE_DYNAMIC(CTabPage1_ListCtrl)

public:
	CTabPage1_ListCtrl(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTabPage1_ListCtrl();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// 放结果列表
	CListCtrl m_ListCtrl;
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void setcontent(const CAlgo_No1& algo);
	//刷新已成交量。只刷新指定节。
	void RefreshTradedVolume(int TradingSession,int TradedVolume);
};
