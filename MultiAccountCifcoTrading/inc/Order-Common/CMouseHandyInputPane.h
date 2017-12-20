#pragma once
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include "wx/popupwin.h"
#include "wx/spinctrl.h"
#include "wx/listctrl.h"
#include "..\inc\Module-Misc\orderCommonFunc.h"


//----------------------------------------------------------------------------
// SimpleTransientPopup
//----------------------------------------------------------------------------
class CMouseHandyInputPane : public wxDialog
{
private:
	enum { conMaxButtonNum = 1000 };

	// IDs for the controls and the menu commands
	enum
	{
		ID_PopupButton = 1,
		ID_PopupList = ID_PopupButton+conMaxButtonNum, 
		ID_PopupListAll
	};

public:
    CMouseHandyInputPane(wxWindow *parent, wxWindowID id,
             const wxString& title,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = wxNO_BORDER,
             const wxString& name = wxDialogNameStr);
    virtual ~CMouseHandyInputPane();

	int GetContractListStyle();
	void ShowView(wxString& strKeyInput, BOOL bShowCmd=TRUE);
	void ResetContractList();
	void SetViewStyle(int nViewStyle) {
		m_nViewStyle = nViewStyle;
	};

    virtual bool Show(bool show = true);
    wxScrolledWindow* GetChild() { return m_panel; }
	void TransMouseHook(wxPoint mousePos, UINT nMouseMSG);

private:
    void OnSize(wxSizeEvent &event);
    void OnSetFocus(wxFocusEvent &event);
    void OnKillFocus(wxFocusEvent &event);
    void OnButton(wxCommandEvent& event);
    void OnListBoxSelected(wxListEvent& event);
	void OnListKeyDown(wxKeyEvent& event);
	void OnListItemMouseClick(wxMouseEvent &event);
	void OnListItemMouseClick_Up(wxMouseEvent &event);
	void OnListMouseMove(wxMouseEvent &event);
	void OnTimer(wxTimerEvent& event);
	void MatchInstrumentByCode( const std::string& strCode, bool bExactMatch, std::set<std::string>& setShowText );
	void MatchInstrumentBySimpleSpell( const std::string& strSimpleSpell, std::set<std::string>& setShowText);
	void InitCodeToNameMap();

private:
	int				m_nViewStyle;
    wxScrolledWindow *m_panel;
//    wxButton*		m_button[conMaxButtonNum];
	wxListCtrl*		m_listBox;
	wxListCtrl*		m_listBox_All;
	int				m_nCodeHeadsCount;
	CODEHEADDATA*	m_ptCodeHeads;
	BOOL			m_bHasUserChange;
	std::map<std::string, std::string> m_mapCodeToName;
	wxTimer m_timer;
private:
    DECLARE_CLASS(CMouseHandyInputPane)
    DECLARE_EVENT_TABLE()
};
