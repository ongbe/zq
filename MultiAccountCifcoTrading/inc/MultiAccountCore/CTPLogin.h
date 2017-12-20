#pragma once
#define  TIMER_MILLISECONDS    100
#define  TIMER_OVERTIME        1200  
#define TIMER_ERROR                             -1
#define TIMER_READY                             100
enum _tagInitQry
{
	TIMER_QRYSETTLEMENTINFO,
	TIMER_QRYINSTRUMENT, 
	TIMER_QRYTRADINGACCOUNT, 
	TIMER_QRYTRADE, 
	TIMER_QRYORDER,
	TIMER_QRYINVESTORPOSITIONDETAIL,
	TIMER_QRYINVESTORPOSITION,
	TIMER_QRYINVESTORPOSITIONCOMBINEDETAIL,
	INITQRY_QRYCONTRACTBANK,
	TIMER_FINISH,
};
class LoginCallback
{
public:
	virtual void SetProgressPosition(IPlatformSingleSvr* pSvr,int TotalStep,int CurStep=-1,int pos=-1)=0;
	virtual void DoLoginFail(IPlatformSingleSvr* pSvr,wxString strSrc=wxEmptyString)=0;
	virtual void SetPromptText(IPlatformSingleSvr* pSvr,wxString strText=wxEmptyString)=0;
	virtual void OnQryFinish(IPlatformSingleSvr* pSvr)=0;
};
#pragma pack(push, 1)
struct Thunk
{
	BYTE  m_pushESP[3];
	DWORD op_movecx;
	DWORD_PTR val_ecx;
	BYTE op_call;
	DWORD_PTR val_address;
};
#pragma pack(pop)
class CTPLogin
{
public:
	enum
	{
		ID_TIMER=50,
	};
	void OnTimer();
	CTPLogin(IPlatformSingleSvr* pSvr,LoginCallback* cback);
	~CTPLogin();
	int SetParticipantInfo(const std::string& BrokerID, const std::string& UserID, 
		const std::string& UserPW, const std::string& OneTimePassword);
	int SetTradeFrontAddrs(const std::vector<std::string>& TradeAddr);
	int SetQuotFrontAddrs(const std::vector<std::string>& QuotAddr);
	int	SetProxyConnectStr(const std::string& ProxyType,const std::string& ProxyUser,
		const std::string& ProxyPwd,const std::string& ProxyAddr,const std::string& ProxyPort);
	int	SetProxyConnectStr(const std::string& str);
	int Login(const std::string& InvestorID, const std::string& InvestorPW, const std::string& InvestorNewPWD="");
	void DoLoginSuccess();
	void SplitwxString(wxString& strLine,std::vector<std::string>& vec);
	void SetOneInstrument(wxString& strLine);
	bool LoadTodayInstruments();
	void SaveTodayInstruments();
	bool LoadCommissionRate(TiXmlElement* pRoot);
	bool LoadMarginRate(TiXmlElement* pRoot);
	bool LoadCommissionRateAndMarginRate(wxString strFileName);
	BOOL MakeConstractIDVarietyMap(map<string, string>& mapConstractIDVariety);
	bool IsQryEnd(){return m_bQryEnd; }
public:
	static CTPLogin* GetCTPLogin(IPlatformSingleSvr* pSvr);
	static bool LoadSaveServerParam(bool bLoad);
	static int QrySettlementInfoThunk(const GUIModuleID GID,const AbstractBusinessData& data);
	static int QryInstrumentListThunk(const GUIModuleID GID,const AbstractBusinessData&);
	static int QryTradingAccountThunk(const GUIModuleID GID,const AbstractBusinessData&);
	static int QryTradeThunk(const GUIModuleID GID,const AbstractBusinessData& data);
	static int QryInvestorPositionDetailThunk(const GUIModuleID GID,const AbstractBusinessData&);
	static int QryInvestorPositionThunk(const GUIModuleID GID,const AbstractBusinessData&);
	static int QryInvestorPositionCombineDetailThunk(const GUIModuleID GID,const AbstractBusinessData&);
	static int QryOrderThunk(const GUIModuleID GID,const AbstractBusinessData& data);
private:
	int QrySettlementInfoCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data);
	int QryInstrumentListCallBackFunc(const GUIModuleID GID,const AbstractBusinessData&);
	int QryTradingAccountCallBackFunc(const GUIModuleID GID,const AbstractBusinessData&);
	int QryTradeCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data);
	int QryInvestorPositionDetailCallBackFunc(const GUIModuleID GID,const AbstractBusinessData&);
	int QryInvestorPositionCallBackFunc(const GUIModuleID GID,const AbstractBusinessData&);
	int QryInvestorPositionCombineDetailCallBackFunc(const GUIModuleID GID,const AbstractBusinessData&);
	int QryOrderCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data);
private:
	IPlatformSingleSvr* m_pSvr;
	LoginCallback* m_LoginCallback;
	int m_TickCount;
	int m_sReady,m_Step;
	MutexLock m_MutexLock;
	bool m_bQryEnd;
	//
	bool m_bfirstLTI;
	bool m_bResultLTI;
	bool m_bfirstCRM;
	//
	UINT_PTR m_TimerID;
};