
#pragma once

#include <set>

class COwnerManager
{
public:

	typedef std::set<__int64> SETOWNER;

	typedef struct {
		char	csTradingDay[16];
	} FILEHEAD, *PFILEHEAD;		// sizeof()=64

public:

	COwnerManager();
	~COwnerManager();

	SETOWNER& GetOwner(SETOWNER& setOwner) {
		return m_setOwner;
	};
	bool Init(std::string& strAccount, std::string& strTradingDay);
	bool AppendOwner(int nFrontID, int nSessionID);
	bool HasOwner(int nFrontID, int nSessionID);

	// LockObject()与UnloadObject()这两个函数必须成对调用
	void LockObject() {
		m_oMutex.Lock();
	};
	void UnlockObject() {
		m_oMutex.Unlock();
	};

protected:
	bool LoadDataFromFile();
	bool SaveDataToFile();

private:
	SETOWNER			m_setOwner;
	wxMutex				m_oMutex;
	std::string			m_strAccount;
	std::string			m_strTradingDay;

};
