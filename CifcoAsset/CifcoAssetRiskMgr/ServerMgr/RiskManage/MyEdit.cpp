// MyEdit.cpp : implementation file
//

#include "stdafx.h"
#include "RiskManage.h"
#include "MyEdit.h"


// CMyEdit

IMPLEMENT_DYNAMIC(CMyEdit, CEdit)

CMyEdit::CMyEdit()
{

}

CMyEdit::~CMyEdit()
{
}


BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
	ON_WM_CHAR()
END_MESSAGE_MAP()

void CMyEdit::OnChar( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	if ( ( nChar >= _T( 'a' ) && nChar <= _T( 'z' ) ) ||
		( nChar >= _T( 'A' ) && nChar <= _T( 'Z' ) ) ||
		( nChar >= _T( '0' ) && nChar <= _T( '9' ) ) ||
		nChar == VK_BACK)
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
}



// CMyEdit message handlers


