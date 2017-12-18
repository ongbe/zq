#pragma once

#include "MyEdit.h"
// CFinancialEdit

class CFinancialEdit : public CMyEdit
{
	DECLARE_DYNAMIC(CFinancialEdit)

public:
	CFinancialEdit();
	virtual ~CFinancialEdit();

	double GetData();
	void SetData(double dData, int nPrecision = 2);

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnEnKillfocus();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};


