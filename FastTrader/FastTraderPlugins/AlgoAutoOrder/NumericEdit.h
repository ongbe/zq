#pragma once


// CNumericEdit

///
/// <ramarks></remarks>
///
class CNumericEdit : public CEdit
{
	DECLARE_DYNAMIC(CNumericEdit)

public:
	///
	/// <summary></summary>
	///
	CNumericEdit();
	///
	/// <summary></summary>
	///
	virtual ~CNumericEdit();

protected:
	DECLARE_MESSAGE_MAP()

protected:
	///
	/// <summary></summary>
	///
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

	///
	/// <summary></summary>
	///
	CString m_strDelim;
    CString m_strInfinity;// '-'
};


