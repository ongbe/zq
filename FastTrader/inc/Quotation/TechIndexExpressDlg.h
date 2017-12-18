///-----------------------------------------------------------------
///
/// @file      CTechIndexExpressDlg.h
/// @author    Administrator
/// Created:   2012-8-23 13:20:57
/// @section   DESCRIPTION
///            CTechIndexExpressDlg class declaration
///
///------------------------------------------------------------------

#ifndef __CTECHINDEXEXPRESSDLG_H__
#define __CTECHINDEXEXPRESSDLG_H__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
	#include <wx/dialog.h>
#else
	#include <wx/wxprec.h>
#endif

//Do not add custom headers between 
//Header Include Start and Header Include End.
//wxDev-C++ designer will remove them. Add custom headers after the block.
////Header Include Start
////Header Include End

////Dialog Style Start
#undef CTechIndexExpressDlg_STYLE
#define CTechIndexExpressDlg_STYLE wxCAPTION | wxSYSTEM_MENU | wxDIALOG_NO_PARENT | wxCLOSE_BOX
////Dialog Style End

class CTechIndexExpressDlg : public wxDialog
{
	private:
		DECLARE_EVENT_TABLE();
		
	public:
		CTechIndexExpressDlg(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("Untitled1"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = CTechIndexExpressDlg_STYLE);
		virtual ~CTechIndexExpressDlg();
		wxTextCtrl *WxEdit1;
		wxButton *WxButton3;
		wxButton *WxButton2;
		wxButton *WxButton1;
	private:
		//Do not add custom control declarations between 
		//GUI Control Declaration Start and GUI Control Declaration End.
		//wxDev-C++ will remove them. Add custom code after the block.
		////GUI Control Declaration Start
		////GUI Control Declaration End
		
	private:
		//Note: if you receive any error with these enum IDs, then you need to
		//change your old form code that are based on the #define control IDs.
		//#defines may replace a numeric value for the enum names.
		//Try copy and pasting the below block in your old form header files.
		
		enum
		{
			////GUI Enum Control ID Start
			ID_DUMMY_START = 1000,
			ID_WXEDIT1,
			ID_WXBUTTON3,
			ID_WXBUTTON2,
			ID_WXBUTTON1,
			////GUI Enum Control ID End
			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};
	private:
		void OnClose(wxCloseEvent& event);
		void CreateGUIControls();

public:
		void SetTechIndex(wxString strTechIndex);
		wxString	m_strTechIndex;
		void Init();
		void OnCloseEvent( wxCommandEvent &event );
		void OnCopy( wxCommandEvent &event );
		
		wxString m_strExpress;
};

#endif
