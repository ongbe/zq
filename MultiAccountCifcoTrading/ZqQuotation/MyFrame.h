#pragma once
#include "wx/frame.h"

DECLARE_EVENT_TYPE(wxEVT_Data_InTime, 3700)
DECLARE_EVENT_TYPE(wxEVT_GetTime, 3701)
DECLARE_EVENT_TYPE(wxEVT_SetLineViewTime, 3702)
DECLARE_EVENT_TYPE(wxEVT_ReSend_Message, 3703)
class MyFrame: public wxFrame
{
public:
	MyFrame(WXHWND hWnd );
	virtual ~MyFrame();
	// Menu commands
  void PhraseMin1(wxCommandEvent& event);
  void PhraseMin5(wxCommandEvent& event);
  void PhraseMin15(wxCommandEvent& event);
  void PhraseMin30(wxCommandEvent& event);
  void PhraseMin60(wxCommandEvent& event);

  void PhraseDay(wxCommandEvent& event);
  void PhraseWeek(wxCommandEvent& event);
  void PhraseMonth(wxCommandEvent& event);
  void Quit(wxCommandEvent& WXUNUSED(event) );

  void CheckPhraseUI(int nID);

  void MarkTriangleUp(wxCommandEvent& event);
  void MarkTriangleDown(wxCommandEvent& event);

  void KXIAN(wxCommandEvent& event);
  void FENSHI(wxCommandEvent& event);

  void OnUpdateUI(wxUpdateUIEvent& evt);

  void OnSize(wxSizeEvent& event);

  void OnDataInTime(PlatformStru_DepthMarketData* pData, CFData* pFData, bool bUpdate);//bupdate 主要用于区分是不是当前显示的期货行情，是则刷新，不是则不刷新
  void OnReSendMessage(wxCommandEvent& event);

  
private:
	DECLARE_EVENT_TABLE()
	DECLARE_NO_COPY_CLASS(MyFrame)

	CKLineView*		m_pKLineView;
	CTLineView*		m_pTLineView;
	CQPanel*		m_pQPanel;		
	KSplitterWindow* m_splitter;

	string			m_ExchangeName; //交易所名称
	string			m_ContractName; //合约名称
	string			m_ContractDate; //合约到期
public:
	wxTimer			m_timerClose;
	void			OnTimer1(wxTimerEvent& event);


	
private:
	WXHWND   m_parent;
public:


	WXLRESULT MSWWindowProc(WXUINT   message,   WXWPARAM   wParam,   WXLPARAM   lParam); 
	void OnClose(wxCloseEvent& evt);

	void SetTodayScales(struct tm tmDay, wxString ExchangeName, vector<struct tm>& vecTimeScales);
	void SetTodayScales(struct tm tmDay, wxString strTransSpan, wxString ExchangeName, vector<struct tm>& vecTimeScales);
	void SetTodayScales(string strTimes, vector<struct tm>& vecTimeScales);
private:
	void OnFuncLDBClickKLine(wxCommandEvent& event);	
	void OnFuncLDBClickTLine(wxCommandEvent& event);

	void OnFuncKeyDownTLine(wxCommandEvent& event);
	void OnFuncKeyDownKLine(wxCommandEvent& event);
	void OnFuncKeyQPanel(wxCommandEvent& event);



private:
	bool InitFromCFG();
	bool InitCfg(TiXmlElement *root);

	struct  SSpan
	{
		struct tm tmBegin;
		struct tm tmEnd;
	};
	map<string, vector<SSpan*>*> m_mapTransTime;//交易时间，现在已经用不上了，如果某天交易时间改到行情软件里面做，那么这个还是用的上的，先保留代码，以备将来可能的更改。


	
public:
	wxTimer			m_timerTrans;
	int				m_nTime;//timer的时间限制
	DWORD			m_dwBeginTickCount;
	struct tm		m_tmLastTrans;//最后交易时间
	void			SetLastTransTime(struct tm tmLastTrans);
	bool			m_bChangedTime;//时间更改标志
	string			m_strTradingDay; //交易日期
	bool			m_bInitTimerTrans;//第一次执行，用于初始化操作
private:
	//CTCPDataService*	m_poTCPDataService;
	

public:
	PlusinAPI*		m_pApi;
	int OnRecvApiMsg_Process( int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId  );
	static int OnRecvApiMsg(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId);

	void ProcessConsoleMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen , int nRequestId);
	void ProcessExchangeMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen , int nRequestId);
	void ProcessInstrumentMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId );
	void ProcessQuoteMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId );
	void ProcessAccountMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId );

	void ClearFData();
	map<string,  CFData *>  m_mapID2FData;
	map<DWORD,  string>     m_mapNum2ID;     //返回包的顺序，对应哪只期货的id
	int						m_nQueryNum;     //请求的序号

	CFData*		GetFDataByQueryID(int nQueryNum);
	int			GetNextRequestId();
	void        InitGUI(CFData *pData, PlatformStru_DepthMarketData *pQutoData, bool bRemain);
	void        InitGUIData(CFData *pFData, PlatformStru_DepthMarketData *pQutoData);

	string		m_strSelFData;//哪只是显示的

protected:
	vector<string> m_vecFData;    //报价表中的所有合约
	int			   m_nFDataIndex; //在报价表的序号
	void		   OnPrior();     //报价表的前一个合约行情
	void		   OnNext();      //报价表的下一个合约行情

	void		   DelFData(int nCount);//超过多少只合约后，删除内存中多余的
protected:
	int			   m_nTrace;	
	BOOL GetRegIntValue(HKEY hRoot,wxString strKeyName,wxString strValueName,int& nValue);


protected:
	void OnFuncReturnKLine(wxCommandEvent& event);
	void OnFuncReturnTLine(wxCommandEvent& event);
	void OnFuncReturnBaoJiaBiao(wxCommandEvent& event);
	void OnFuncReturnBaoJiaBiao2(wxCommandEvent& event);


	void OnReturnKLine();
	void OnReturnTLine();
	void OnReturnBaoJiaoBiao();

	int m_nRightSpanMiniWidth;
	void SetMiniWidth(unsigned long value);
};

struct SMessage
{
	int nCmdId;
	int nErrorCode;
	void* pData;
	int nDataLen;
	int nRequestId;

	SMessage(int nId, int nCode, void* p, int nLen, int nRId)
	{
		nCmdId			= nId;
		nErrorCode		= nCode;
		if(nLen >0)
		{
			pData       = new char[nLen];
			memcpy(pData, p, nLen);
		}
		else 
			pData  = NULL;
		
		nDataLen = nLen;
		nRequestId = nRId;
	}

};