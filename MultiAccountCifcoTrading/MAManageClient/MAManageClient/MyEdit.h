#pragma once


// CMyEdit

class CMyEdit : public CEdit
{
	DECLARE_DYNAMIC(CMyEdit)

public:
	CMyEdit();
	virtual ~CMyEdit();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};

