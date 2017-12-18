#pragma once

#include <string>

class ConfigPanelBase : public wxPanel
{
public:
	ConfigPanelBase( wxWindow* pParent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL ,
		const wxString& name = wxPanelNameStr);

	~ConfigPanelBase(void);

	void CreateHeader( std::string strTitle );
	void OnLanguageChanged();
	void EnableSaveAndReturn( bool bEnable );

	void SetSungardStyle( bool bFlag );
	wxString GetWindowCaption();

	//virtual void OnReturn(wxCommandEvent& event);

	DECLARE_EVENT_TABLE()

private:
	std::string m_strTitle;
	wxStaticText* m_pStaticTitle;
	wxButton* m_pBtnReturn;
	wxButton* m_pBtnSaveAndReturn;

protected:
	bool m_bSungardStyle;
};
