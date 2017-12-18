// FinancialEdit.cpp : implementation file
//

#include "stdafx.h"
#include "RiskManage.h"
#include "FinancialEdit.h"


// CFinancialEdit

IMPLEMENT_DYNAMIC(CFinancialEdit, CMyEdit)

CFinancialEdit::CFinancialEdit()
{

}

CFinancialEdit::~CFinancialEdit()
{
}


BEGIN_MESSAGE_MAP(CFinancialEdit, CMyEdit)
	ON_CONTROL_REFLECT(EN_KILLFOCUS, &CFinancialEdit::OnEnKillfocus)
	ON_WM_CHAR()
END_MESSAGE_MAP()


// CFinancialEdit message handlers



void CFinancialEdit::OnEnKillfocus()
{
	// TODO: Add your control notification handler code here
	CString csTemp;
	this->GetWindowText(csTemp);

	csTemp.Remove(',');
	double d = atof(csTemp.GetBuffer(0));
	csTemp.ReleaseBuffer();
	csTemp = GetAccountFormatString(d, 2);

	this->SetWindowText(csTemp);
}

double CFinancialEdit::GetData()
{
	CString csTemp;
	this->GetWindowText(csTemp);

	csTemp.Remove(',');
	double d = atof(csTemp.GetBuffer(0));
	return d;
}

void CFinancialEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (( nChar >= _T( '0' ) && nChar <= _T( '9' ) ) ||
		nChar == _T('.') ||
		nChar == VK_BACK)
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
}

void CFinancialEdit::SetData( double dData, int nPrecision )
{
	CString csTemp = GetAccountFormatString(dData, 2);
	this->SetWindowText(csTemp);
}
