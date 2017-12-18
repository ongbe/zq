#pragma once

class PlusinConfigHelper
{
public:
	PlusinConfigHelper(void);
	~PlusinConfigHelper(void);

	bool LoadConfig( const wxString& strPath );
	
	std::string GetAppID() const;
	UINT GetMenuPos() const;
	bool NeedStartWhenRegist() const;
	bool CanMultiInstance() const;
    bool NeedCurrentInstrumentID() const;
	std::string GetMenuName() const;
	std::string GetSummary() const;
	std::string GetDesc() const;

private:
	void ParseMenuPos( const std::string strMenuPos );

private:
	std::string m_strAppID;
	UINT m_unMenuPos;
	bool m_bStartWhenRegist;
    bool m_bMultiInstance;      //是否可以打开多个实例
    bool m_bNeedCurrentInstrumentID;
	std::string m_strMenuName;
	std::string m_strSummary;
	std::string m_strDesc;
};
