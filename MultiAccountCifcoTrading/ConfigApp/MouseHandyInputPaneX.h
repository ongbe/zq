#pragma once
//#include "stdafx.h"
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include "wx/popupwin.h"
#include "wx/spinctrl.h"
#include "wx/listctrl.h"
#include <set>
#include "..\inc\Module-Misc\orderCommonFunc.h"

//typedef struct {
//	char strCodeHead[64];	// 合约代码头
//	char strCodeName[64];	// 合约简称
//	char strCodeNameJP[64];	// 合约简称的拼音首字母
//} CODEHEADDATA, *PCODEHEADDATA;

BEGIN_DECLARE_EVENT_TYPES()
DECLARE_EVENT_TYPE(wxEVT_CODE_MOUSEINPUT, 2401)
END_DECLARE_EVENT_TYPES()


//----------------------------------------------------------------------------
// MouseHandyInputPaneX
//----------------------------------------------------------------------------
class MouseHandyInputPaneX : public wxPopupTransientWindow
{
private:
	enum { conMaxButtonNum = 1000 };

// IDs for the controls and the menu commands
enum
{
    ID_PopupButton = 10000,
    ID_PopupList = ID_PopupButton+conMaxButtonNum
};

public:
    MouseHandyInputPaneX( wxWindow *parent );
    virtual ~MouseHandyInputPaneX();

	void SetCodeGroup(vector<GroupInfo>* poGroup);

	void ShowKeyInput(wxString& strKeyInput) {
		m_bKeyInput = TRUE;
		m_strKeyInput = strKeyInput;
	};

    // wxPopupTransientWindow virtual methods are all overridden to log them
    virtual void Popup(wxWindow *focus = NULL);
    virtual void OnDismiss();
    virtual bool ProcessLeftDown(wxMouseEvent& event);
    virtual bool Show( bool show = true );

    wxScrolledWindow* GetChild() { return m_panel; }

private:
    wxScrolledWindow *m_panel;
	vector<GroupInfo>* m_poGroup;
    //wxButton*		m_button[conMaxButtonNum];
	wxListCtrl*		m_listBox;
	BOOL			m_bKeyInput;
	wxString		m_strKeyInput;
	int				m_nCodeHeadsCount;
	CODEHEADDATA*	m_ptCodeHeads;
	std::map<std::string, std::string> m_mapCodeToName;

private:
	void OnMouse(wxMouseEvent &event);
    void OnSize( wxSizeEvent &event );
    void OnSetFocus( wxFocusEvent &event );
    void OnKillFocus( wxFocusEvent &event );
    void OnButton( wxCommandEvent& event );
    void OnListBoxSelected(wxListEvent& event);

	void MatchInstrumentByCode( const std::string& strCode, bool bExactMatch, std::set<std::string>& setShowText );
	void MatchInstrumentBySimpleSpell( const std::string& strSimpleSpell, std::set<std::string>& setShowText);

private:
    DECLARE_CLASS(MouseHandyInputPaneX)
    DECLARE_EVENT_TABLE()
};