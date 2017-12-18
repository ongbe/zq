#pragma once
#include "OptionQuationInterface.h"
#include "resource.h"
#include "FData.h"
#include "QView.h"
#define  QUERY_MIN1_DAYCOUNT	20
#define  QUERY_MIN5_DAYCOUNT	100
#define  QUERY_MIN15_DAYCOUNT	100
#define  QUERY_MIN30_DAYCOUNT	100
#define  QUERY_MIN60_DAYCOUNT	100
#define  QUERY_DAY_DAYCOUNT		3650//10年的数据应该是够了
#define  QUERY_WEEK_DAYCOUNT	3650
#define  QUERY_MONTH_DAYCOUNT	3650

#define  QUERY_MAX_RETURN		1000
struct SGetInfo
{
	int nType;    //nType =0 的时候，str代表时间(格式: 10:15:50)，以后ntype可能还有别的扩展
	std::string str;
	SGetInfo()
	{
		nType =0;
		str = "";
	}
};
// CDlgQuation dialog
typedef bool (*CallBackGetTime)(SGetInfo& sGetInfo);

class  QUATION_API CDlgQuation : public CDialog
{
	DECLARE_DYNAMIC(CDlgQuation)

public:
	CDlgQuation(CWnd* pParent = NULL); 
	virtual ~CDlgQuation();
	HICON m_hIcon;
	EnumPhrase m_enumPhrase;

	CallBackGetTime m_callbackGet;
	void SetCallBackFunction(CallBackGetTime callbackGet);


	//设置交易日
	void SetTradingDay_Dlg(std::string strTradingDay);//20140828
	//要显示的合约名
	void ShowInstrument_Dlg(std::string strInstrument);	//IF1410
	//合约详情
	bool SetInstrumentInfo_Dlg(PlatformStru_InstrumentInfo& InstrumentData);
	//今日的交易刻度
	void SetTradingScales_Dlg(std::string strInstrument, const vector<pair<string,string>>& TradingTime);//时间的格式: 0915 1130   1300 1515
	//该合约的深度行情
	void SetDepthMarketData(std::string strInstrument, PlatformStru_DepthMarketData* QutoData);

	//等上面所有函数调用完，再调用这个函数
	void InitFinish_Dlg(std::string strInstrument);


// Dialog Data
	enum { IDD = IDD_DIALOG_QUATION };

	CQView		  m_QView;
//////////////////////////////////////////////////////////////////////////
//本地测试用
public:
/*	CFData		  m_FData;	
	void ReadTLineFileTest();	
	void ReadFileTest_KLineDay();
	void ReadFileTest_KLine60();*/
//////////////////////////////////////////////////////////////////////////
	
	void SetTodayScales(string strTimes, vector<struct tm>& vecTimeScales);
private:
	map<string,  CFData>		m_mapID2FData;
	CReadWriteLock				m_Mutex_ID2FData;

	std::queue<std::string>		m_queueInstrument;
	bool						m_bConnectHQServer;  //行情实时服务器连接状态
	bool						m_bConnectHQHistoryServer;//行情历史服务器连接状态
	std::string				    m_strInstrument; //订阅的行情
	std::string					m_strKInstrument;//历史行情服务器如果断开
	

	std::string					m_strTradingDay;//今天的交易日	
	std::string					m_strUpdateTime;//今天最新的时间

	string						m_strSelFData;  //哪只是显示的期货
	void						OnDataInTime(PlatformStru_DepthMarketData* pData, CFData* pFData, bool bUpdate);

	map<int, KLineQuery>		m_mapIndex2QueryInfo;
	int nQueryIndex;
	
protected:
	void SetPhraseVector(EnumPhrase enumPhrase, CFData& fdata);

	virtual void DoDataExchange(CDataExchange* pDX);   
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()

	LRESULT OnMessageReturn(WPARAM wP,LPARAM lP);
	LRESULT OnMessageQutoInTime(WPARAM wP,LPARAM lP);
	LRESULT OnMessageConnectSuccess(WPARAM wP,LPARAM lP);
	LRESULT OnMessageConnectfail(WPARAM wP,LPARAM lP);
	LRESULT OnMessageChangeTimeSpan(WPARAM wP,LPARAM lP);
	LRESULT OnMessageQutoTLineHistory(WPARAM wP,LPARAM lP);	
	LRESULT OnMessageQutoKLineHistory(WPARAM wP,LPARAM lP);	
	LRESULT OnMessageIndexExpress(WPARAM wP,LPARAM lP);	
	LRESULT OnMessageIndexConfig(WPARAM wP,LPARAM lP);	
	LRESULT OnMessageConfigChange(WPARAM wP,LPARAM lP);	
	

	void	DelFData(std::string strInstrument);
	void    SubscribleInstrument(std::string strInstrument);
	void    UnSubscribleInstrument(std::string strInstrument);

	void    QueryHistoryTData(CFData& fData);
	void    QueryHistoryKLine(CFData& fData, EnumPhrase enumPhrase);	
	void	QueryHistoryKLine_Mins(CFData& fData, EnumPhrase	enumPhrase, int nDayCount);
	
	void	QueryHistoryKLine_Day(CFData& fData);
	void	QueryHistoryKLine_Week(CFData& fData);
	void	QueryHistoryKLine_Month(CFData& fData);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void OnDestroy();
	void OnClose();
	//历史行情服务器地址
	std::string		m_HisSvrIP;
	unsigned short	m_HisSvrPort;
	//实时行情服务器地址
	std::string		m_IntimeQuotSvrIP;
	unsigned short  m_IntimeQuotSvrPort;
	void Load(std::string& HisSvrIP, unsigned short& HisSvrPort, std::string& IntimeQuotSvrIP, unsigned short& IntimeQuotSvrPort);
};
