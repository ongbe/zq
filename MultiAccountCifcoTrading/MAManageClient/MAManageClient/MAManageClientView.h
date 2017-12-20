// MAManageClientView.h : CMAManageClientView 类的接口
//


#pragma once
#include "AccountManageDlg.h"
#include "MultiAccountStruct.h"
#include <map>
#include <vector>


class CMAManageClientView : public CFormView
{
protected: // 仅从序列化创建
	CMAManageClientView();
	DECLARE_DYNCREATE(CMAManageClientView)

public:
	enum{ IDD = IDD_MACLIENT_FORM };

// 属性
public:
	CMAManageClientDoc* GetDocument() const;

// 操作
public:
	int GetCurPageIndex();
	AccountType GetAccountType();
	void GetSelectAccountID(std::vector<int>& vec);
	void ClearAllData();
	void ClearData( AccountType eType );
	void SetData( std::map<int, AccountInfoEx>& accountMap );
	void SetData( AccountType eType, std::map<int, AccountInfoEx>& accountMap );
	void AddAccount( const AccountInfoEx& accountInfoEx);
	void AddAccount( AccountType eType, const AccountInfoEx& accountInfoEx);
	void ModifyAccount( const AccountInfoEx& accountInfoEx);
	void DelAccount( int nAccountID, AccountType eType );
	void ShowForeListData(bool bShow);
	void ShowBkgListData(bool bShow);
	void SetPermissionData(const PermissionData& foreData, const PermissionData& bkgData);

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
	virtual ~CMAManageClientView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

public:
	CTabCtrl m_mainTab;
	CAccountManageDlg m_bkgAccountDlg;
	CAccountManageDlg m_foreAccountDlg;

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpcs);
	afx_msg void OnTcnSelchangeTabView(NMHDR *pNMHDR, LRESULT *pResult);

private:
	bool m_bInited;
	bool m_bShowForeListData;
	bool m_bShowBkgListData;
};

#ifndef _DEBUG  // MAManageClientView.cpp 中的调试版本
inline CMAManageClientDoc* CMAManageClientView::GetDocument() const
   { return reinterpret_cast<CMAManageClientDoc*>(m_pDocument); }
#endif

