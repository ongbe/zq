// APIMainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "APIMainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAPIMainDlg dialog


CAPIMainDlg::CAPIMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAPIMainDlg::IDD, pParent)
{
	m_pSaveMsgCallBack = NULL;
	//{{AFX_DATA_INIT(CAPIMainDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAPIMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAPIMainDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAPIMainDlg, CDialog)
	//{{AFX_MSG_MAP(CAPIMainDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_COPYDATA, OnRecvCopyData)
END_MESSAGE_MAP()

LRESULT CAPIMainDlg::OnRecvCopyData( WPARAM wParam, LPARAM lParam )
{
	COPYDATASTRUCT* pCopyData = (COPYDATASTRUCT*)lParam;
	if ( NULL == pCopyData )
	{
		return FALSE;
	}

	//非控制消息都上抛给应用层
	if ( NULL != m_pSaveMsgCallBack )
	{
		int nCmdId = LOWORD(pCopyData->dwData);
		int nErrorCode = HIWORD(pCopyData->dwData);
		m_pSaveMsgCallBack(nCmdId, nErrorCode, pCopyData->lpData, pCopyData->cbData);
	}

	return TRUE;
}

void CAPIMainDlg::SetSaveMsgCallBackFn( MsgCallBackFuncInner msgCallBackFn )
{
	m_pSaveMsgCallBack = msgCallBackFn;
}

/////////////////////////////////////////////////////////////////////////////
// CAPIMainDlg message handlers
