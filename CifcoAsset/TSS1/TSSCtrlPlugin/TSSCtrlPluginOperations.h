#pragma once

#include "afxcmn.h"
#include "afxwin.h"

// CBookOrderOperations dialog

class CTSSCtrlPluginOperations : public CDialog
{

public:
	typedef std::map<std::string, PLOTCTRLITEMCFG> MAPINSTCFG;
	typedef std::map<std::string, INSTRUMENTCTRLDIRECT> MAPINSTCTRL;
	typedef std::map<std::string, std::string> MAPPLOTINST;
	typedef std::map<std::string, PlatformStru_InstrumentInfo> MAPINSTINFO;
	typedef std::map<std::string, PlatformStru_Position> MAPINSTPOSI;
	typedef std::map<std::string, int> MAPINSTVOL;
	typedef std::map<std::string, PlatformStru_PositionDetail> MAPPOSIDETAIL;
	typedef std::map<std::string, std::string> MAPPOSIDETAILTIME;
	typedef std::vector<PlatformStru_PositionDetail> VECPOSIDETAIL;
	typedef std::map<std::string, PlatformStru_DepthMarketData> MAPQUOT;

public:
    struct ORDERCLOSEINFO
    {
        std::string     szOrderRef;//挂单标识
        TThostFtdcInstrumentIDType     szInstrumentID;//
        TThostFtdcDirectionType szDirection;//
        DWORD           dTime;//撤单时间
        bool            bFlag;//是否已做撤单操作

        ORDERCLOSEINFO()
        {
            dTime = 0;
            bFlag = false;
        }
    };
    

public:
	enum {	conTimerNum = 123456 };
	enum {	conMaxPositionCount = 2 };
	enum {	conColBuyVolumeWait, conColBidVolume, conColPrice, 
			conColAskVolume, conColSellVolumeWait };
	enum {	conActionNothing, conActionOpen, conActionCloseToday, conActionClose };
	enum {	conColorGradientNum = 6 };

public:
	CTSSCtrlPluginOperations(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	virtual ~CTSSCtrlPluginOperations();

	bool IsExiting() {
		return m_bExiting;
	};
	int GetNextRowID_Plot() {
		return ++m_nRowID_Plot;
	};
	int GetNextRowID_Position() {
		return ++m_nRowID_Position;
	};
	int GetNextRowID_PosiDetail() {
		return ++m_nRowID_PosiDetail;
	};
	BOOL GetInstPlot(std::string& strInstrumentID, PLOTCTRLITEMCFG& item);
	BOOL GetLastPrice(std::string& strInstrumentID, double& fLastPrice);
	BOOL GetInstPriceTick(std::string& strInstrumentID, double& fPriceTick);
	int GetRemainTime(std::string& strOpenTime);
	int GetInstCanHandleTime(std::string& strInstrumentID);
	int GetRemainTime(std::string& strInstrumentID, std::string& strOpenTime);
	int GetPosiHoldingTime(std::string& strInstrumentID, int nTime);
	BOOL IsInRestTime(std::string& strInstrumentID, int nTime);

protected:
	void QueryCurrAccountID();
    void QueryCurrSubAccountID();
	void QueryExchangeIDs();
	void QueryInstrumentIDs(CString& strExchangeID);
	void QueryCurrAccountInfo(CString& strCurrAccount);
	void QueryPositionInfo(CString& strCurrAccount);
	void QueryPosiDetailInfo(CString& strCurrAccount);
	int QueryOpenOrderVol(CString& strCurrAccount, CString& strInstrumentID);
	void QueryOpenOrderVol(CString& strCurrAccount);
	void QueryPosiDetailOpenTime(CString& strCurrAccount);
	void QueryInstrumentInfo(CString& strInstrumentID);
	void QueryQuotSnapshot(CString& strInstrumentID);
    void QueryAllOrderFTIDList();
    void QueryOrderInfoByFTID(int& nID);
	void SubscribeQuot(CString& strInstrumentID);
	void UnsubscribeQuot(CString& strInstrumentID);
    
	//无审核下单
	void SendInputOrder(PlatformStru_InputOrder& inputOrder);
	void ForceOrderVerify();

	void RecomputePositionProfit();
	BOOL MayClose_Time();
	BOOL CheckPosiHasLose(PlatformStru_InputOrder& inputOrder);

	void NewOrder(CString& strInstrumentID, BOOL bDirection, int nOCMode, double fLimitPrice, int nVolume);
	void NewOrder_NotVerify(CString& strInstrumentID, BOOL bDirection, int nOCMode, double fLimitPrice, int nVolume);
	BOOL CloseAll();
    void CancelOrder(CString& strInstrumentID, BOOL bDirection, double fLimitPrice);//撤单
    void CancelOrder(PlatformStru_OrderInfo& tInfo);
	BOOL ProcPosiDetailMaxTime();
	BOOL ProcPosiMaxLose();

	BOOL CanCloseAll();
	BOOL CanClose(CString& strInstumentID, char cDirect, char cHedge);

	virtual void ShowMsgLog(CString& strMsg) = 0;

protected:

	int						m_nRowID_Plot;
	int						m_nRowID_Position;
	int						m_nRowID_PosiDetail;
	map<std::string, int>	mapKey2Index_Plot;
	map<int, std::string>	mapIndex2Key_Plot;
	map<std::string, int>	mapKey2Index_Position;
	map<int, std::string>	mapIndex2Key_Position;
	map<std::string, int>	mapKey2Index_PosiDetail;
	map<int, std::string>	mapIndex2Key_PosiDetail;

	std::queue<std::string>	m_queueExchangeForQuery;

	MAPINSTCFG				m_mapInstCfg;
	MAPINSTCTRL				m_mapInstCtrl; 
	MAPPLOTINST				m_mapPlotInst; 
	MAPINSTINFO				m_mapInstInfo;
	MAPINSTPOSI				m_mapInstPosi;
	MAPINSTVOL				m_mapInstPosiVol;
	MAPINSTVOL				m_mapInstOpenWaitVol;
	MAPPOSIDETAIL			m_mapPosiDetail;
	//MAPPOSIDETAILTIME		m_mapPosiDetailTime;
	MAPQUOT					m_mapQuot;

	bool					m_bExiting;
	int						m_nQueryOrderLastSeqNum;

	bool					m_bHasLoseOverLimit_AcctMax;
	bool					m_bHasLoseRegress_AcctMax;
	DWORD					m_dwLoseOLimitStart_AcctMax;
	DWORD					m_dwLoseRegressStart_AcctMax;

	DWORD					m_dwPreCloseTime;

	CString					m_strCurrAccount;
	CString					m_strCurrSubAccount;
	PlatformStru_TradingAccountInfo m_tAccountInfo;
//	int								m_nPositionCount;
//	PlatformStru_Position			m_tPositions[conMaxPositionCount];


	map<string, PlatformStru_InputOrder>	m_mapCloseOrder;
	set<string>				m_mapCloseWaitOrderRef;	
	set<int>				m_setCloseOrder;	

    map<string,ORDERCLOSEINFO>      m_mapOrderCloseInfo;//记录撤单信息
    bool					m_bHasDayLostMax;//是否是触发日亏
    map<string,PlatformStru_OrderInfo>      m_mapOtherWaitOrder;//记录其他撤单信息
	long					m_nLastFTID4Cancel;
	int						m_nLastSeqNum4OpenVal;
};
