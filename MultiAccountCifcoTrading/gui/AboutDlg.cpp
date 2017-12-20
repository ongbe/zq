#include "StdAfx.h"
#include "AboutDlg.h"
#include <wx/hyperlink.h>
#include "../inc/Module-Misc/SimpleWriteLog.h"
#pragma comment(lib, "version.lib")
AboutDlg::AboutDlg(wxWindow *parent):
wxDialog(parent, wxID_ANY,LOADSTRING(ABOUTDLG_TITLE),wxDefaultPosition, wxSize(350,185))
{
	DIALOG_CREATE_LOG();
    AUTO_SETSIZE();
    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
 
    wxStaticBitmap* itemStaticBitmap4 = new wxStaticBitmap( this, wxID_STATIC, wxIcon(_T("zqICON_MAIN")), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemStaticBitmap4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer3->Add(itemBoxSizer5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText6 = new wxStaticText( this, wxID_STATIC, LOADSTRING(OIP_FASTTRADER), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer5->Add(itemStaticText6, 0, wxALIGN_LEFT|wxALL, 5);
    char exepath[_MAX_PATH];
	GetModuleFileName(NULL,exepath,_MAX_PATH);
    if(Load(exepath))
	{
		wxString strVersion=GetValue(_T("FileVersion")); 
		strVersion.Replace(wxString(","),wxString("."));
		strVersion=LOADSTRING(FILEVERSION)+strVersion;
		wxStaticText* itemVer = new wxStaticText( this, wxID_STATIC,strVersion, wxDefaultPosition, wxDefaultSize, 0 );
		itemBoxSizer5->Add(itemVer, 0, wxALIGN_LEFT|wxALL, 5);
	}
    wxHyperlinkCtrl* itemStaticText7 = new wxHyperlinkCtrl( this, wxID_ANY, LOADSTRING(COPYRIGHT),"http://www.cifco.net/");
    itemBoxSizer5->Add(itemStaticText7, 0, wxALIGN_LEFT|wxALL, 5);
	wxStaticText* itemStaticPhone = new wxStaticText( this, wxID_STATIC, LOADSTRING(TELEPHONE), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer5->Add(itemStaticPhone, 0, wxALIGN_LEFT|wxALL, 5);

    wxButton* itemButton8 = new wxButton( this, wxID_OK, LOADSTRING(OK), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemButton8, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);


}
wxString AboutDlg::GetValue(const wxString& sKey)
{
  //Validate our parameters
  if (m_VerData != NULL)
  {

  //What will be the return value from this function
  wxString sVal;

  //Form the string to query with
  wxString sQueryValue;
  sQueryValue=sQueryValue.Format(_T("\\StringFileInfo\\%04x%04x\\%s"), m_wLangID, m_wCharset, sKey.operator LPCTSTR());

  //Note that the definition for VerQueryValue in the VC 2005 Winver.h header file is incorrectly
  //defined as taking a non-const 2nd parameter, so to avoid this issue, lets get a non const pointer
  //to the "sQueryValue" buffer
  /*LPTSTR pszQueryValue = sQueryValue.GetBuffer(sQueryValue.GetLength());*/
  LPTSTR pszQueryValue =(LPTSTR) sQueryValue.c_str();

  //Do the query
  LPTSTR pVal = NULL;
  UINT nLen = 0;
  if (VerQueryValue(m_VerData, pszQueryValue, reinterpret_cast<LPVOID*>(&pVal), &nLen)) 
    sVal = pVal;
    
  //Release the non-const buffer now that we are finished with it    
  //sQueryValue.ReleaseBuffer();

  return sVal;
  }
  return wxEmptyString;
}
void AboutDlg::Unload()
{
  m_pffi = NULL;
  if (m_VerData != NULL)
  {
     m_VerData= NULL;
    /*delete m_VerData;*/
     free(m_VerData);
  }
  m_wLangID = 0;
  m_wCharset = 1252; //Use the ANSI code page as a default
  m_pTranslations = NULL;
  m_nTranslations = 0;
}
BOOL AboutDlg::Load(LPCTSTR szFileName)
{
  //Free up any previous memory lying around
  Unload();
  BOOL bSuccess = FALSE;
  DWORD dwHandle = 0;
  DWORD dwSize = GetFileVersionInfoSize(szFileName, &dwHandle);
  if (dwSize)
  {
    //Allocate some memory to hold the version info data
      //m_VerData=new char[dwSize+1];
     m_VerData=(char*)malloc(dwSize+1);
     if (m_VerData==NULL)
    {
      SetLastError(ERROR_OUTOFMEMORY);
      return FALSE;
    }
    if (GetFileVersionInfo(szFileName, dwHandle, dwSize, m_VerData))
    {
      //Get the fixed size version info data
      UINT nLen = 0;
      if (VerQueryValue(m_VerData, _T("\\"), reinterpret_cast<LPVOID*>(&m_pffi), &nLen))
      {
        //Retrieve the Lang ID and Character set ID
        if (VerQueryValue(m_VerData, _T("\\VarFileInfo\\Translation"), reinterpret_cast<LPVOID*>(&m_pTranslations), &nLen) && (nLen >= sizeof(TRANSLATION))) 
        {
          m_nTranslations = nLen / sizeof(TRANSLATION);
          m_wLangID = m_pTranslations[0].m_wLangID;
          m_wCharset = m_pTranslations[0].m_wCodePage;
        }
        bSuccess = TRUE;
      }
      else
        //TRACE(_T("CVersionInfo::Load, Failed to query file size version info for file %s, LastError:%d\n"), szFileName, ::GetLastError());
        ;
    }
    else
      //TRACE(_T("CVersionInfo::Load, Failed to read in version info for file %s, LastError:%d\n"), szFileName, ::GetLastError());
      ;
  }
  else
    //TRACE(_T("CVersionInfo::Load, Failed to get version info for file %s, LastError:%d\n"), szFileName, ::GetLastError());
    ;

  return bSuccess;
}
AboutDlg::~AboutDlg(void)
{
  Unload();
}
