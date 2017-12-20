#pragma once

class CChangeOrderManager
{
public:

	enum {	conMaxRemainTime = 3 };

	typedef struct {
		int nSeqNum;
		CWnd* pWnd;
		DWORD dwRemainTime;
		PlatformStru_InputOrder inputOrder;
	} CHANGEORDERDATA, *PCHANGEORDERDATA;

	class CChangeOrderKey
	{
	public:
		CChangeOrderKey(CString& strAccount, CString& strSubAccount, CString& strInstumentID, char cDirection) 
		{
			m_strAccount = strAccount.GetBuffer(0);
			m_strSubAccount = strSubAccount.GetBuffer(0);
			m_strInstrumentID = strInstumentID.GetBuffer(0);
			m_cDirection = cDirection;
		};

		CChangeOrderKey(const PlatformStru_InputOrder& inputOrder)
		{
			m_strAccount = inputOrder.InvestorID;
			m_strSubAccount = inputOrder.strAccount;
			m_strInstrumentID = inputOrder.InstrumentID;
			m_cDirection = inputOrder.Direction;
		};

		CChangeOrderKey& operator= (const CChangeOrderKey& other)
		{
			if (this == &other)
				return *this;
			this->m_strAccount = other.m_strAccount;
			this->m_strSubAccount = other.m_strSubAccount;
			this->m_strInstrumentID = other.m_strInstrumentID;
			this->m_cDirection = other.m_cDirection;
			return *this;
		};

		//注意，通过OpenDate和TradeID的比较顺序，保证set<PositionCombKey>对相同条件的记录，按时间排序
		bool operator< (const CChangeOrderKey& r) const
		{
			int irlt;

			irlt=m_strAccount.compare(r.m_strAccount);
			if(irlt<0) return true;
			if(irlt>0) return false;
			
			irlt=m_strSubAccount.compare(r.m_strSubAccount);
			if(irlt<0) return true;
			if(irlt>0) return false;

			irlt=m_strInstrumentID.compare(r.m_strInstrumentID);
			if(irlt<0) return true;
			if(irlt>0) return false;

			if(m_cDirection<r.m_cDirection)
				return true;
			else if(m_cDirection>r.m_cDirection)
				return false;

			return false;
		};

		bool operator== (const CChangeOrderKey& r) const
		{
			int irlt;
			
			irlt=m_strAccount.compare(r.m_strAccount);
			if(irlt!=0) return false;

			irlt=m_strSubAccount.compare(r.m_strSubAccount);
			if(irlt!=0) return false;

			irlt=m_strInstrumentID.compare(r.m_strInstrumentID);
			if(irlt!=0) return false;

	        if(m_cDirection!=r.m_cDirection)
		        return false;
			
			return true;
		};
    
		bool operator!= (const CChangeOrderKey& r) const
		{
			return !(*this == r);
		};
    
		bool operator> (const CChangeOrderKey& r) const
		{
			return !(*this < r || *this == r);
		};

	public:
		std::string m_strAccount;
		std::string m_strSubAccount;
		std::string m_strInstrumentID;
		char		m_cDirection;
	};

public:
	static DWORD WINAPI ThreadProc(LPVOID lpParameter);

public:
	CChangeOrderManager(void);
	~CChangeOrderManager(void);

	BOOL Init();
	int Run();
	void Release();

	BOOL LockObject() {
		::WaitForSingleObject(m_hMutex, INFINITE);
		return TRUE;
	};
	BOOL UnlockObject() {
		::ReleaseMutex(m_hMutex);
		return TRUE;
	};

	BOOL ResetChangeOrder(CString& strAccount, CString& strSubAccount, 
			CString& strInstrumentID, char cDirection, 
			PlatformStru_InputOrder& inputOrder, CWnd* pWnd);
	BOOL ResetChangeOrderSeqNum(CString& strAccount, CString& strSubAccount, 
			CString& strInstrumentID, char cDirection, int nSeqNum);
	BOOL ParseRsp(int nSeqNum, int nVolume);

protected:
	BOOL	m_bExit;
	HANDLE	m_hMutex;
	HANDLE	m_hThread;
	std::map<CChangeOrderKey, CHANGEORDERDATA>	m_mapWaitOrder;	// 等待查询相关未成交挂单的数量返回的记录集

};
