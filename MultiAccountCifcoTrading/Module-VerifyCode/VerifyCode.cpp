#include "StdAfx.h"
#include "VerifyCode.h"
#include <wx/dcmemory.h>
#include "../cfgMgr/CfgMgr.h"

const int VERIFY_CODE_LENTH = 4;

// char * rand_str(int in_len)
// {
// 	char *__r = (char *)malloc(in_len + 1);
// 
// 	int i;
// 
// 	if (__r == 0)
// 	{
// 		return 0;
// 	}
// 
// 	for (i = 0; i < in_len; i++)
// 	{
// 		__r[i] = (char)rand();
// 	}
// 
// 	__r[i] = 0;
// 
// 	return __r;
// }

//const char CCH[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char CCH[] = "0123456789abcdefghjkmnpqrstuvwxy";
bool VerifyCode::rand_str(int len)
{
	int nDivider = strlen(CCH);
	CfgMgr* pMgr = CFG_MGR_DEFAULT();
	if(NULL != pMgr)
	{
		if ( pMgr->GetMainWindowCfg()->nLogonVerify == 2)
		{
			nDivider = 10;
		}
	}

	char * ch = new char[len+1];
	memset(ch, 0, len+1);
	srand(time(NULL) + rand());
	for (int i = 0; i < len; ++i)
	{
		int x = rand() % nDivider;
		ch[i] = CCH[x];
	}

	m_VerifyCodeText = ch;

	delete[] ch;
	return true;
}

VerifyCode::VerifyCode(void)
{
	srand( (unsigned)time( NULL ) );
}

VerifyCode::~VerifyCode(void)
{

}

bool VerifyCode::Verify(wxString & code)
{
	
	return (m_VerifyCodeText.CmpNoCase(code) == 0)? true:false;
}

const wxBitmap & VerifyCode::GetVerifyCodeBitmap()
{
	RefreshCode();
	return m_VerifyCodeBitmap;
}

void VerifyCode::RefreshCode()
{
	rand_str(VERIFY_CODE_LENTH);
	CreateBmp();
}


bool VerifyCode::CreateBmp()
{

	wxBitmap bmp;

	bmp.Create(45, 20);

	wxMemoryDC dc(bmp);

	if (!dc.IsOk())
		return false;
	dc.SetFont(wxFont(12, wxDEFAULT ,wxNORMAL, wxNORMAL, FALSE,"Fixedsys"));
	dc.SetBackground( *wxTRANSPARENT_BRUSH );
	dc.Clear();
	dc.SetTextForeground( *wxRED );
	dc.DrawText( m_VerifyCodeText, 3, 3 );


	m_VerifyCodeBitmap = bmp;
	return true;
}
