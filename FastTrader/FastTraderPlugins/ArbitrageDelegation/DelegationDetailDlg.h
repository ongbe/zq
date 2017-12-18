#pragma once
#include "afxcmn.h"
#include "KernelStruct.h"

#define WM_USER_DETAILDLG_CLOSE WM_USER+200

// CDelegationDetailDlg dialog

class CDelegationDetailDlg : public CDialog
{
	DECLARE_DYNAMIC(CDelegationDetailDlg)

public:
	CDelegationDetailDlg(CWnd* pParent = NULL);   // standard constructor
    CDelegationDetailDlg(set<long>& setFTID,CWnd* pParent = NULL); 
    CDelegationDetailDlg(set<OrderKey>& setOrderKey,CWnd* pParent = NULL); 
	virtual ~CDelegationDetailDlg();

// Dialog Data
	enum { IDD = IDD_DELEGATIONDETAIL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedButtonRevoke();
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
	DECLARE_MESSAGE_MAP()

    CListCtrl m_listDetailCtrl;

public:
    void Update(const PlatformStru_OrderInfo* pOrderData);

private:
    void SetDataToListCtrl();

private:
    //得到当前选中listctrl项索引
    int GetListSelectedIndex();

    //撤单
    void RevokeOrder(PlatformStru_OrderInfo* pOrderData);

    //要显示的数据
    map<string,PlatformStru_OrderInfo> m_mapOrderInfo;
    //要请求的委托
    set<long> m_setFTID;
    set<OrderKey> m_setOrderKey;
};
