#pragma once

class AboutDlg :
	public wxDialog
{
public:
	AboutDlg(wxWindow *parent);
	~AboutDlg(void);

private:
	wxString GetValue(const wxString& sKey);
	void Unload();
	BOOL Load(LPCTSTR szFileName);
	struct TRANSLATION
	{
		WORD m_wLangID;   //e.g. 0x0409 LANG_ENGLISH, SUBLANG_ENGLISH_USA
		WORD m_wCodePage; //e.g. 1252 Codepage for Windows:Multilingual
	};
	WORD                m_wLangID;       //The current language ID of the resource
	WORD                m_wCharset;      //The current Character set ID of the resource
	char* m_VerData;       //Pointer to the version info blob
	TRANSLATION*        m_pTranslations; //Pointer to the "\\VarFileInfo\\Translation" version info
	int                 m_nTranslations; //The number of translated version infos in the resource
	VS_FIXEDFILEINFO*   m_pffi;          //Pointer to the fixed size version info data
};
