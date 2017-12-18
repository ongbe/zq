#pragma once

#include <vector>

class CAccountListData
{
public:
	CAccountListData(void);
	~CAccountListData(void);

	void SetKey( int nKey ) { m_nKey = nKey; }
	int GetKey() { return m_nKey; }

	CString GetValue( int nCol ) const;
	void AddValue( const CString& csValue ) { m_vCol.push_back(csValue); }

private:
	int m_nKey;
	std::vector<CString> m_vCol;
};
