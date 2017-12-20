///-----------------------------------------------------------------
///
/// @file      CProgressDlg.h
/// @author    Administrator
/// Created:   2012-2-27 15:56:18
/// @section   DESCRIPTION
///            CProgressDlg class declaration
///
///------------------------------------------------------------------

#ifndef __CProgressDlg_H__
#define __CProgressDlg_H__

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
#include <wx/button.h>
#include <wx/gauge.h>
#include <wx/stattext.h>
#include <wx/statbmp.h>
#include <wx/listbox.h>
////Header Include End
#include "checkversion.h"
////Dialog Style Start
#undef CProgressDlg_STYLE
#define CProgressDlg_STYLE wxCAPTION | wxSYSTEM_MENU | wxDIALOG_NO_PARENT | wxMINIMIZE_BOX | wxCLOSE_BOX
////Dialog Style End

class CProgressDlg : public wxDialog
{
public:
	CProgressDlg(wxWindow *parent);
	~CProgressDlg();
private:
	wxButton *WxButtonCancel;
	wxButton *WxButtonOK;
	wxStaticText *WxStaticText2;
	wxGauge *WxGauge1;
	wxStaticText *WxStaticText1;
	wxListBox *WxListBox1;
	DECLARE_EVENT_TABLE()

	enum
	{
		////GUI Enum Control ID Start
		ID_DUMMY_START = 1000,
		ID_WXBUTTON3,
		ID_WXBUTTON1,
		ID_PROGRESS_STATIC,
		ID_WXGAUGE1,
		ID_WXSTATICTEXT1,
		ID_WXSTATICBITMAP1,
		ID_WXLISTBOX1,
		////GUI Enum Control ID End
		ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
	};

public:


	void SetMainProcExe(wxString str){ m_strMainProc = str; };
	CheckVersion*   m_pCheckVersion;

	bool CreateUpdateThread();
	HANDLE m_hThreadHandle;

	void OnUpdatProgress(wxCommandEvent& event);
	void OnClose(wxCloseEvent& event);	
    void OnPaint(wxPaintEvent& event);
	void OnOK(wxCommandEvent& event);
	void OnCancel(wxCommandEvent& event);
	void GetJPGHandle(wxBitmap& bmp);	
private:
    wxBitmap m_BackBitmap;
    wxString m_strMainProc;

};

#endif
