// MainFrm.h : CMainFrame 类的接口
//


#pragma once

#include "ChildView.h"

class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// 属性
public:

protected:
	void ParseCmd(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum);
	void ParseCmd_Console(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum);
	void ParseCmd_BaseBusiness(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum);

	void QueryCurrInstrumentID();
	void QueryProductShortNames();
	void QueryExchangeIDs();
	void QueryProductInfos();
	void QueryInstrumentIDs(CString& strExchangeID);
	void ShowNewBookOrderDialog(CString& strInstrumentID);
	void ActiveDialog(HWND hWnd);

// 操作
public:
	static int CallbackRecvApiMsg(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId);

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	CChildView    m_wndView;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	DECLARE_MESSAGE_MAP()


protected:
	BOOL	m_bExitApp;
	BOOL	m_bWaitForNewBookOrder;
	std::queue<std::string> m_queueExchangeForQuery;

public:
	afx_msg void OnClose();
};


