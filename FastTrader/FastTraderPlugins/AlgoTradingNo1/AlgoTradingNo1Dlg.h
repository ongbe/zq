// AlgoTradingNo1Dlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "tabpage2_chart.h"
#include "tabpage1_listctrl.h"
#include "ExtTabCtrl.h"
#include "afxwin.h"
#include "common-AlgoTradingNo1Dlg.h"
#include "Algo_No1.hpp"
#include "KernelStruct.h"

#include <string>
#include <sstream>
#include <vector>
using namespace std;



// CAlgoTradingNo1Dlg 对话框
class CAlgoTradingNo1Dlg : public CDialog
{
// 构造
public:
	CAlgoTradingNo1Dlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CAlgoTradingNo1Dlg();

// 对话框数据
	enum { IDD = IDD_ALGOTRADINGNO1_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CBrush m_brush;


	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTcnSelchangingTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButton3();
	afx_msg void OnClose();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	// 计算结果标签页
	CExtTabCtrl m_tab_result;
	CTabpage2_Chart m_PageChart;
	CTabPage1_ListCtrl m_PageListCtrl;



	// 合约名称
	CEdit m_InstrumentName;
	// 方向
	CComboBox m_Direction;
	// 开仓/平仓
	CComboBox m_Offset;
	//	交易依据，无趋势/有趋势
	CComboBox m_TradeTrend;

	// 初始价格
	CEdit m_S0;
	// 手数
	CEdit m_X0;
	// 完成时间
	CEdit m_T;
	// 时间分段数
	CEdit m_N;
	// 风险偏好λ
	CEdit m_lambda;
	// 日波动率σ
	CEdit m_Sigma;
	// 日收益α
	CEdit m_Alpha;
	// 短期冲击成本η
	CEdit m_Eta;
	// 短期冲击成本ε
	CEdit m_Epsilon;
	// 长期冲击成本γ
	CEdit m_Gamma;
	// τ
	CEdit m_tau;
	// xbar
	CEdit m_xbar;
	// ηbar
	CEdit m_EtaBar;
	// k^2
	CEdit m_k2;
	// k
	CEdit m_k;
	// kT
	CEdit m_kT;
	// kτ
	CEdit m_ktau;
	// E(X)
	CEdit m_EX;
	// V(X)
	CEdit m_VX;

	//计算
	CButton m_btnCalculate;
	//执行
	CButton m_btnTrade;
	//取消
	CButton m_btnCancelTrading;

	CStatusBarCtrl m_wndStatusBar;

public:
	//读取ini文件的配置信息
	void ReadIni(void);
	//保存配置信息到ini文件
	void SaveIni(void);

	//计算结果,xk(无趋势),nk(无趋势),xk2(有趋势),nk2(有趋势)
	CAlgo_No1 m_algo;

	//是否登录成功
	bool m_bLogon;
	//当前的账号，从FastTrader中获取
	string m_Account;
	//上一次订阅行情的合约
	string m_SubscribedInstrumentName;

	//是否正在执行交易
	bool m_bTrading;
	//开始执行的时间
	time_t m_StartTradingUTC;
	//当前执行的节，0 ... N-1
	int m_TradingSession;


	static int PluginsAPICallBack(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId);
	static CAlgoTradingNo1Dlg* m_pDlg;


	//使能或禁用所有控件
	void EnableAllControls(BOOL bEnable);
	//进行一次下单,根据m_algo和m_TradingSession。Volume为-1时，自动计算下单量
	void SendInsertOrder(int Volume=-1);
	//处理成交回报
	void TradeInfoReach(PlatformStru_TradeInfo& TradeInfo);
	//处理报单回报。
	void OnOrderRtn(PlatformStru_OrderInfo& OrderInfo);
	//停止交易
	void StopTrading(void);

	//调整子窗口大小
	void AdjustChildWindowSize(void);
	//将控件中的值放到m_algo中去
	void GetAlgoParamFromWnd(void);
	//将m_algo中的值设置到控件中去
	void SetAlgoParamToWnd(void);

//	afx_msg void OnEnUpdateEdit1();
};

