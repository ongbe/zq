// RiskManageView.h : CRiskManageView 类的接口
//


#pragma once
#include "AccountManageDlg.h"
#include "RiskManageStruct.h"
#include <map>
#include <vector>


class CRiskManageView : public CFormView
{
protected: // 仅从序列化创建
	CRiskManageView();
	DECLARE_DYNCREATE(CRiskManageView)

public:
	enum{ IDD = IDD_MACLIENT_FORM };

// 属性
public:
	CRiskManageDoc* GetDocument() const;

// 操作
public:
	int GetCurPageIndex();
	AccountType GetAccountType();
	void GetSelectAccountID(std::vector<int>& vec);
	void ClearAllData();
	void ClearData( AccountType eType );
	void SetData( std::map<int, RiskMgmtUserInfoEx>& accountMap );
	void SetData( std::map<int, TrustTradeAccountEx>& trustTradeMap );
	void SetData( std::map<int, TraderInfoEx>& traderMap );

	//void SetData( AccountType eType, std::map<int, AccountInfoEx>& accountMap );
	void AddAccount( const RiskMgmtUserInfoEx& userInfoEx);
	void AddAccount( const TrustTradeAccountEx& trustTradeEx);
	void AddAccount( const TraderInfoEx& traderInfoEx);

	void ModifyAccount( const RiskMgmtUserInfoEx& userInfoEx);
	void ModifyAccount( const TrustTradeAccountEx& trustTradeEx);
	void ModifyAccount( const TraderInfoEx& traderInfoEx);

	void DelAccount( int nAccountID, AccountType eType );
	void ShowListData(AccountType eType, bool bShow);
	//void SetPermissionData(const PermissionData& foreData, const PermissionData& bkgData);

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

// 实现
public:
	virtual ~CRiskManageView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

private:
	void GenerateListData( const RiskMgmtUserInfoEx &userInfoEx, CAccountListData &listData );
	void GenerateListData( const TrustTradeAccountEx &trustTradeEx, CAccountListData &listData );
	void GenerateListData( const TraderInfoEx &traderInfoEx, CAccountListData &listData );

public:
	CTabCtrl m_mainTab;
	CAccountManageDlg m_manageAccountDlg;
	CAccountManageDlg m_trustTradeAccountDlg;
	CAccountManageDlg m_traderAccountDlg;

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpcs);
	afx_msg void OnTcnSelchangeTabView(NMHDR *pNMHDR, LRESULT *pResult);

private:
	bool m_bInited;
};

#ifndef _DEBUG  // RiskManageView.cpp 中的调试版本
inline CRiskManageDoc* CRiskManageView::GetDocument() const
   { return reinterpret_cast<CRiskManageDoc*>(m_pDocument); }
#endif

