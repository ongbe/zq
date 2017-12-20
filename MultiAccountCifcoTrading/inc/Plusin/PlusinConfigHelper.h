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
	std::string GetMenuName() const;
	std::string GetSummary() const;
	std::string GetDesc() const;

private:
	void ParseMenuPos( const std::string strMenuPos );

private:
	std::string m_strAppID;
	UINT m_unMenuPos;
	bool m_bStartWhenRegist;
	std::string m_strMenuName;
	std::string m_strSummary;
	std::string m_strDesc;
};
