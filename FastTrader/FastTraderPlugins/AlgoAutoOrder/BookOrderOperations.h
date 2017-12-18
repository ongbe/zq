#pragma once

//#include "GridCtrl\GridCtrl.h"
#include "afxcmn.h"
#include "afxwin.h"

// CBookOrderOperations dialog

class CBookOrderOperations : public CDialog
{
public:
	enum {	conMaxPositionCount = 2 };
	enum {	conColBuyVolumeWait, conColBidVolume, conColPrice, 
			conColAskVolume, conColSellVolumeWait };
	enum {	conActionNothing, conActionOpen, conActionCloseToday, conActionClose };
	enum {	conColorGradientNum = 6 };

	typedef struct {
		int nBuy;
		int nSell;
	} BIDASKVOLUME, *PBIDASKVOLUME;

public:
	CBookOrderOperations(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	virtual ~CBookOrderOperations();

	bool IsExiting()
	{
		return m_bExiting;
	};

//protected:
	void QueryCurrUserID();
	void QueryCurrAccountID();
	void QueryCurrAccountInfo(CString& strCurrAccount);
	void QueryInstrumentInfo(CString& strInstrumentID);
	void QueryPositionInfo(CString& strInstrumentID);
	int QueryOrderWaitVolume(CString& strInstrumentID, double fLimitPrice, char cDirection);
	void QueryOrderWaitVolumeList(CString& strInstrumentID, char cDirection, BOOL bClearWaitMap);
	void QueryOrderWaitMaxMinPrice(CString& strInstrumentID);
	int QueryOrderInfo(int nFTID);
	void QueryOrderWaits();
	void QueryQuotSnapshot(CString& strInstrumentID);
	void SubscribeQuot(CString& strInstrumentID);
	void UnsubscribeQuot(CString& strInstrumentID);
	void QueryConfig_InstrumentIDInputPanelShow();
	void QueryConfig_QtyInputPanelShow();
	void QueryConfig_QtyInputPanelQtyDatas();

	void RecomputePositionProfit();
public:
	void NewOrder(CString& strInstrumentID, BOOL bDirection, int nOCMode, double fLimitPrice, int nVolume);
protected:
	void CancelOrder(CString& strInstrumentID, BOOL bDirection, double fLimitPrice);
	void CancelAllOrder(CString& strInstrumentID);
    void CancelOrder(PlatformStru_InputOrderAction& tAction);
    void CancelOrder(PlatformStru_OrderInfo& tInfo);
	void ChangeOrder(CString& strInstrumentID, BOOL bDirection, int nOCMode, double fLimitPrice, int nVolume);

    
	void ResetColorGradient(DWORD dwColorFirst, DWORD dwColorEnd) 
	{
		//m_nColorGradientIndex = bStart ? 0 : -1;
		m_dwColorFirst = dwColorFirst;
		m_dwColorEnd = dwColorEnd;
	};
	// nIndex从conColorGradientNum-1开始，然后每次nIndex--
	DWORD GetGradientColor(int nIndex)
	{
		if(nIndex>=0 && nIndex<=conColorGradientNum)
		{
			int nRed = (conColorGradientNum-nIndex)*GetRValue(m_dwColorEnd)/conColorGradientNum
				+ nIndex*GetRValue(m_dwColorFirst)/conColorGradientNum;
			int nGreen = (conColorGradientNum-nIndex)*GetGValue(m_dwColorEnd)/conColorGradientNum
				+ nIndex*GetGValue(m_dwColorFirst)/conColorGradientNum;
			int nBlue = (conColorGradientNum-nIndex)*GetBValue(m_dwColorEnd)/conColorGradientNum
				+ nIndex*GetBValue(m_dwColorFirst)/conColorGradientNum;
			return RGB(nRed, nGreen, nBlue);
		}
		else
			return 0;
	}

protected:
	// nSeqNum, strPrice
	std::map<int, std::string>		m_mapWaitRspOrderWait;
	// strPrice, nVolume
	std::map<std::string, BIDASKVOLUME>		m_mapOrderWaitVolume;

	CString							m_strCurrAccount;
	CString							m_strCurrSubAccount;
	CString							m_strOldInstrumentID;
	int								m_nDlgMaxWidth;
	int								m_ndlgMaxHeight;
	bool							m_bExiting;
	bool							m_bNeedInstrumentIDInputPanel;
	bool							m_bNeedQtyInputPanel;
	bool							m_bUserChangeInstrumentID;
	int								m_nQueryOrderLastSeqNum;
	std::map<int, PlatformStru_OrderInfo>	m_mapOrdersForCancel;
	std::map<std::string, DWORD>	m_mapPriceCancel;
	int								m_nColorGradientIndex;	// -1表示没有梯度颜色处理
	DWORD							m_dwColorFirst;
	DWORD							m_dwColorEnd;

	PRICEGRIDINDEX					m_tPriceGridIndex;
	PRICEGRIDINDEX					m_tOldPriceGridIndex;
	PlatformStru_InstrumentInfo		m_tInstrumentInfo;
	PlatformStru_TradingAccountInfo m_tAccountInfo;
	int								m_nPositionCount;
	PlatformStru_Position			m_tPositions[conMaxPositionCount];
	PlatformStru_DepthMarketData	m_tDepthMarketData;
	WaitOrderPrice					m_tWaitOrderPrice;
	WaitOrderPrice					m_tOldWaitOrderPrice;

	BOOKORDERDLGCOLOR				m_bookOrderDlgColor;

public:
    PlatformStru_DepthMarketData* GetDepthMarketData()
    {
        return &m_tDepthMarketData;
    }

    PlatformStru_InstrumentInfo* GetInstrumentInfo()
    {
        return &m_tInstrumentInfo;
    }

	void SetOldInstrumentID(CString strInstrumentID)
	{
		m_strOldInstrumentID = strInstrumentID;
	};

	CString GetOldInstrumentID()
	{
		return m_strOldInstrumentID;
	};

};
