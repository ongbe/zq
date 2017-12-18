#pragma once

#include "BookOrderOperations.h"
#include "afxcmn.h"
#include "ExtTabCtrl.h"
#include "GeneralParamDlg.h"
#include "FeedbackDlg.h"
#include "afxwin.h"
#include "AlgoAutoData.h"

// CAlgoAutoOrderDlg dialog
#define OPT_BACKTIMER   65000
#define WAITCANCLETIMER  1000    
#define COM_IDLETIMER   2000  
#define COM_EXECUTETIMER  3000  

#define MAXVALUE 8000

class CAlgoAutoOrderDlg : public CBookOrderOperations
{
	DECLARE_DYNAMIC(CAlgoAutoOrderDlg)

public:
	CAlgoAutoOrderDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAlgoAutoOrderDlg();

// Dialog Data
	enum { IDD = IDD_ALGOAUTOORDER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
 
    afx_msg void OnClose();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnTcnSelchangeShowtab(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()

protected:        
    HICON m_hIcon;

public:
    static DWORD WINAPI ThreadProc(LPVOID lpParameter);
    void ParseCmd(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum);   

    CString GetInstrumentID();
    void    SetInstrumentID(CString& strInstrumentID);
  
    void ReadIni(void);

    void StartTrading(void);
    void StopTrading(void);

    BOOL IsTradeTime(){return m_bInTradeTimeSpan;}
    
    double GetUpperLimitPrice(){return m_tDepthMarketData.UpperLimitPrice;}
    double GetLowerLimitPrice(){return m_tDepthMarketData.LowerLimitPrice;};
    
public:  
    CAlgoAutoData *m_pAlgoAutoData; 
    CString m_strInstrumentName;

protected:
    void ParseCmd_Base(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum);
    void ParseCmd_Business(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum);
    void ParseCmd_Config(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum);

private:
    void AdjustChildWindowSize(void);//调整子窗口大小
    int  TradeRun(void);
    BOOL CancleWaitOrder();//撤挂单
    int GetWaitOrderCount();// 重置数量到达后的挂单量。
    BOOL ChangeGeneralParam(SINGLEOPT* pSingle);
    void EndTrading(void);//取消并重下交易 
    void SaveIni(void);
    void ThreadQuitClear(BOOL bSuccess);
    

    CGeneralParamDlg m_PageGeneralParam;
    CFeedbackDlg  m_PageFeedback;

    BOOL m_bOptTrigger;// 优化条件触发器    

    CExtTabCtrl m_TabCtrl;
    CStatusBarCtrl m_wndStatusBar;

    HANDLE m_hThread;
	HANDLE m_hCloseEvent;
    
    BOOL m_bThreadExit;
    BOOL m_bTrading;

    time_t m_StartTradingUTC;//开始执行的时间
    // 记录每秒的行情次数
    int m_SecondCount;
    // 保存每秒的行情价格
    map<int,double> m_mapSnapShot;
    // 要优化计算的实例
    SINGLEOPT* m_singleOptInstance;
    // 记录等待本次下单报单状态回报的时间（等待3秒才进行下次下单）
    int m_nWaitStatusTime;

    // 是否正在撤单
    BOOL m_bCancleOrdering;
    // 是否报单全部状态更新，只有全部更新了才能撤单
    BOOL m_bAllOrderStatusUpdata;
    // 本次是否已经下单
    BOOL m_bOrderYet;
    // 主动停止交易
    BOOL m_bStopTrade;
    // 记录出现异常的下单档数，只有当下单档数等于异常档数时，才可进行处理
    int m_nExceptCount;
    // 0:中金所合约时间段 1:商品期货时间段
    int m_nTradeType;
    // 是否是交易时间段
    BOOL m_bTradeTime;
    BOOL m_bInTradeTimeSpan;
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};
