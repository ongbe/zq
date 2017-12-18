// NationalInstPromptDlg.h : header file
//

#pragma once

#include "OptSetupDlg.h"

#include <map>
#include <set>
#include <string>
using namespace std;


// CNationalInstPromptDlg dialog
class CNationalInstPromptDlg : public CDialog
{
// Construction
public:
	CNationalInstPromptDlg(CWnd* pParent = NULL);	// standard constructor
    virtual ~CNationalInstPromptDlg();

// Dialog Data
	enum { IDD = IDD_NATIONALINSTPROMPT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();  
    afx_msg void OnBnClickedButtonAdd();
    afx_msg void OnBnClickedButtonModify();
    afx_msg void OnBnClickedButtonDelete();
    afx_msg void OnBnClickedButtonApply();
    afx_msg void OnClose();   
    afx_msg void OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult);    
    afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg LRESULT OnChildClose(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

    CListCtrl m_listCtrl;
    CButton m_btnApplyCtrl;

public:
    // 是否登录上CTP
    bool m_bLogon;  
    string m_szAccount;                         //当前的账号
    set<std::string> m_setExchangeID;           //交易所
    set<string> m_setInstrumentID;              //合约

    COptSetupDlg* m_pOptDlg;                    //当前的OptSetupDlg
    static CNationalInstPromptDlg* m_pMainDlg;
    static int PluginsAPICallBack(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId);


    //更新列表内容
    void EnterAlgorithm(const PlatformStru_DepthMarketData* pMarketData);
    void UpdataListCtrl(const PlatformStru_DepthMarketData* pMarketData);

private:

    //读取ini文件的配置信息
    bool ReadData(void);
    //保存配置信息到ini文件
    void SaveData(void);

    void SetDataToListCtrl();
    void SubscribeInstrument();
    //void StartThread();
    //下单
    void SendInsertOrder(CParamData* pData);

    //得到当前选中listctrl项索引
    int GetListSelectedIndex();

private:  
						
    // 子窗口是否关闭
    bool m_bChildOpened;
    //保存所有的数据
    map<int, CParamData*> m_mapIndex2Data;


};
