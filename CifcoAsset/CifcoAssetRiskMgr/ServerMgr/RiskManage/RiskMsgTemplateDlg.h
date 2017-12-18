#pragma once
#include "CGridListCtrlEx.h"
#include "RiskManageStruct.h"
#include "afxcmn.h"
#include "afxwin.h"

// CRiskMsgTemplateDlg dialog

class CRiskMsgTemplateDlg : public CDialog
{
	DECLARE_DYNAMIC(CRiskMsgTemplateDlg)

public:
	CRiskMsgTemplateDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRiskMsgTemplateDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_MSG_TEMPLATE };

	virtual BOOL OnInitDialog();

	void SetIndicator( const std::vector<RiskIndicator>& vIndicator);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CGridListCtrlEx m_listInd;
	CGridListCtrlEx m_listMsg;
	CButton m_btnSave;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg LRESULT OnRecvMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLvnItemchangedListMsgInd(NMHDR *pNMHDR, LRESULT *pResult);

private:
	bool FindEvtMsgTemplate(int nIndicatorID, int nLevelID, EventMessageTemplate& msg);
	bool IsDataChanged();
	void EnableControls(BOOL bFlag);
	CString ConventErrorMsg( const CString& csMsg );

private:
	std::vector<RiskIndicator> m_vIndicator;
	std::map<int, int> m_mapMaxRiskLevel;
	std::map<int, std::map<int, EventMessageTemplate>> m_mapEvtMsgTemplate;
	int m_nSelIndID;
	std::vector<EventMessageTemplate> m_vCurEvtMsg;
	
public:
	afx_msg void OnBnClickedButtonMsgSave();
	afx_msg void OnDestroy();
};
