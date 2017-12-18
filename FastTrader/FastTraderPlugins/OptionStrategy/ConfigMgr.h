#ifndef _CONFIGERMANAGER_H_
#define _CONFIGERMANAGER_H_

class ConfigManager
{
public:
	static ConfigManager* GetInstance();

	static void DestroyConfigerManager();

	bool LoadConfig();
    bool LoadXMlConfig();
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

    int GetResolutionWidth();
    int GetResolutionHight();

    float GetSplitterH();
    float GetSplitterV();

    int GetColorMode();
    bool GetMultiInstance();

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
    int m_nColorMode;
	CString m_csPwd;
	int m_nAccountID;
	int m_nPermissionID;
	int m_nAssetMgmtOrgID;
    int m_nResolutionWidth;
    int m_nResolutionHight;
    float m_fSplitterH;
    float m_fSplitterV;
    bool m_bMultiInstance;//是否可以打开多个实例，1可以，0不可以，默认为0
};

#endif


