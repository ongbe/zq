// NationalInstPromptDlg.h : header file
//

#pragma once

#include "easymutex.h"
#include "OptSetupDlg.h"
#include "DelegationDetailDlg.h"
#include "LogFile.h"

#include <map>
#include <set>
#include <string>
#include "afxwin.h"
using namespace std;


struct Stru_Inst2Price
{
    double BidPrice1;//买一价
    double AskPrice1;//卖一价
};

enum PRICE_TYPE
{
    PRICE_TYPE_BID,
    PRICE_TYPE_ASK
};

#define BASE_YEAR 2014
/*
       sign      Year        Month        Day                   id   
32bit  |-1-|------6------|-----5-----|-----5-----|--------------15---------------|  
*/
#define  MAKE_DATEBASE_ID(_year,_month,_day,_id) \
    (int)((int)( (int)(_year) << 25 ) | (int)( (int)(_month) << 20 ) | (int)( (int)(_day) << 15 ) | (int)(_id)  )



// CArbitrageDelegationDlg dialog
class CArbitrageDelegationDlg : public CDialog
{
// Construction
public:
	CArbitrageDelegationDlg(CWnd* pParent = NULL);	// standard constructor
    virtual ~CArbitrageDelegationDlg();

// Dialog Data
	enum { IDD = IDD_ARBITRAGEDELEGATION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnBnClickedButtonArbitrage();    
    afx_msg void OnBnClickedButtonModify();
    afx_msg void OnBnClickedButtonRevoke();
    afx_msg void OnBnClickedButtonDelegatedetail();
    afx_msg void OnClose();   
    afx_msg LRESULT OnSetupDlgClose(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnDetailDlgClose(WPARAM wParam, LPARAM lParam);
    afx_msg void OnBnClickedCheckDisplayrevoke();
    afx_msg void OnBnClickedCheckTriggered();    
    afx_msg void OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()

    CListCtrl m_listCtrl;


public:
    // 是否登录上CTP
    bool m_bLogon;  
    string m_szAccount;                         //当前的账号

    CLogFile*   m_pLog;                         //log
    COptSetupDlg* m_pOptDlg;                    //当前的OptSetupDlg
    CDelegationDetailDlg* m_pDetailDlg;         //当前的DelegationDetailDlg
    static CArbitrageDelegationDlg* m_pMainDlg;
    static int PluginsAPICallBack(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId);

private:

    //读取ini文件的配置信息
    bool ReadData(void);
    //保存配置信息到ini文件
    void SaveData(void);

    void SetDataToListCtrl(bool bDevoke = false,bool bTrigger = false);
    void SubscribeInstrument();

    //下单
    void SendInsertOrder(CParamData* pData);

    //得到当前选中listctrl项索引
    int GetListSelectedIndex();


    //行情到来后
    //bool bMatch(PlatformStru_DepthMarketData* pMarketData,CParamData* pDataParam);
    bool EnterAlgorithm(CParamData* pDataParam);
    void UpdateInst2Price(string InstrumentID,double BidPrice1, double AskPrice1);
    double GetPrice(string InstrumentID,PRICE_TYPE nType);
    int  GetListIndex(int nParamID);
    bool bInstsArrived(string szInst1,string szInst2);


    //内存中有
    bool GetOrderRefByParamID(int nParamID,string& orderRef1,string& orderRef2);
    //内存中无，向FastTrader请求
    bool GetOrderKeyByParamID(int nParamID,set<OrderKey>& setOrderKey);
    bool GetOrderKeyByOrderRef(string szOrderRef,OrderKey& key);
    
    void SetSetupDlgOpened(int nVal);
    bool GetSetupDlgOpened();

    void SetDetailDlgOpened(int nVal);
    bool GetDetailDlgOpened();

private:  
						
    // SetupDlg窗口是否打开
    volatile int m_bSetupDlgOpened;    
    
    // DetailDlg窗口是否打开
    volatile int m_bDetailDlgOpened;    
    

    //保存已触发的数据 key=Param_ID
    map<int, CParamData*> m_mapIndex2DataTriggered;

    //保存已撤单的数据 key=Param_ID
    map<int, CParamData*> m_mapIndex2DataRevoked;

    //保存未触发和未撤单的数据 key=Param_ID
    map<int, CParamData*> m_mapIndex2Data;
    
    //保存合约行情价
    map<string, Stru_Inst2Price> m_mapInst2Price;
    
    //所有交易所合约
    set<string> m_setInstrumentID;

    //流水号
    int m_nNextArbitrageID;

    Ceasymutex m_Inst2PriceMutex;
    //Ceasymutex m_Inst2SubedMutex;
    Ceasymutex m_OrderMutex;
    Ceasymutex m_DataMutex;

    //两个CheckButton
    static bool m_bRevoke;
    static bool m_bTrigger;

    //下单相关 key1:InstrumentID
    map<string,bool>       m_mapInstrument2Subscribed;        //记录订阅的合约

    //有下面两个关系 可以得到Param_ID和FTID的映射，用于重启后向FastTrader请求委托单
    //<Param_ID OrderRef>   
    map<int,set<string>>       m_mapParamID2OrderRef;        //Param_ID和OrderRef的绑定关系  用于程序过程中的查看明细

    //<OrderRef OrderKey>   
    map<string,OrderKey>       m_mapOrderRef2OrderKey;        //OrderRef和OrderKey明细单的绑定关系 



    //订阅的合约深度行情是否全部到来
    volatile bool m_bInstSubedAllReached;
    
};
