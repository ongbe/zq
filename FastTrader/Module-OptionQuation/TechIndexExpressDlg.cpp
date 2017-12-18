// TechIndexExpressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OptionQuation.h"
#include "TechIndexExpressDlg.h"


// CTechIndexExpressDlg dialog

IMPLEMENT_DYNAMIC(CTechIndexExpressDlg, CDialog)

CTechIndexExpressDlg::CTechIndexExpressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTechIndexExpressDlg::IDD, pParent)
{
	m_strTechIndex = "";
}

CTechIndexExpressDlg::~CTechIndexExpressDlg()
{
}

void CTechIndexExpressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Edit);
}


BEGIN_MESSAGE_MAP(CTechIndexExpressDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CTechIndexExpressDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTechIndexExpressDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

void CTechIndexExpressDlg::SetTechIndex(CString strTechIndex)
{
	m_strTechIndex = strTechIndex;	
}
// CTechIndexExpressDlg message handlers
BOOL CTechIndexExpressDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString strTitle;
	strTitle.Format(_T("%s指标用法"), m_strTechIndex);
	SetWindowText(strTitle);

	Init();
	return TRUE;
}
void CTechIndexExpressDlg::Init()
{
	m_strExpress = "";
	CString strPath;
	char localPath[256];
	memset(localPath, 0, 256);
	GetModuleFileName( NULL, localPath, 256);
	string filename=localPath;
	size_t splitpos=filename.find_last_of('\\');
	strPath = filename.substr(0, splitpos+1).c_str();

	CString strDir;
	strDir.Format(_T("%shqData\\tech.xml"), strPath);

	TiXmlDocument* pXmlDocment = new TiXmlDocument( strDir.GetBuffer(strDir.GetLength()));
	if( NULL == pXmlDocment)
	{
		return ;
	}

	std::auto_ptr<TiXmlDocument> ptr( pXmlDocment );
	if( !pXmlDocment->LoadFile() )
	{
		return ;
	}

	TiXmlElement *root = pXmlDocment->RootElement();
	if ( NULL == root )
	{
		return ;
	}

	if( std::string(root->Value()) != "tech")
	{
		return ;
	}

	TiXmlElement *tList2 = root->FirstChildElement(m_strTechIndex.GetBuffer(m_strTechIndex.GetLength()));
	if(tList2 == NULL)
		return ;
	const char *strValue = tList2->ToElement()->GetText();
	m_Edit.SetWindowText(strValue);

	m_strExpress = strValue;

}
void CTechIndexExpressDlg::OnBnClickedOk()
{
	HANDLE hClip;
	char *pBuf;
	char* str = (char *)m_strExpress.GetBuffer(m_strExpress.GetLength());
	if(!OpenClipboard())
		return;
	EmptyClipboard();
	hClip=GlobalAlloc(GMEM_MOVEABLE,strlen(str)+1);
	pBuf=(char *)GlobalLock(hClip);
	strcpy(pBuf,str);
	GlobalUnlock(hClip);
	SetClipboardData(CF_TEXT,hClip);
	CloseClipboard();
}

void CTechIndexExpressDlg::OnBnClickedCancel()
{
	OnOK();
}
