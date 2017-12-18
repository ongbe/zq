#ifndef _CONFIGERMANAGER_H_
#define _CONFIGERMANAGER_H_

class ConfigManager
{
public:
	static ConfigManager* GetInstance();

	static void DestroyConfigerManager();

	bool LoadConfig();

	bool SaveConfig();

	CString GetAccount() const;
	void SetAccount( const CString& csAccount );

	bool GetRememberAccountFlag() const;
	void SetRememberAccountFlag( const bool bFlag );

	const CString GetServerIPAddress();
	int GetServerPort();

	const CString& GetSystemPath();

	void SetPassword(const CString& csPwd);
	CString GetPassword() const;

	void SetAccountID( int nAccountID);
	int GetAccountID();

	void SetPermissonID( int nAccountType);
	int GetPermissonID();

	void SetAssetMgmtOrgID( int nOrgID );
	int GetAssetMgmtOrgID();

private:
	~ConfigManager(void);

	ConfigManager(void);

	bool IsFileExsit( const CString& csFileName );

private:
	static ConfigManager* m_pInstance;
	CString m_csSystemPath;
	CString m_csIPAddr;
	int m_nPort;

	CString m_csAccount;
	bool m_bRememberAccount;
	CString m_csPwd;
	int m_nAccountID;
	int m_nPermissionID;
	int m_nAssetMgmtOrgID;
};

#endif


