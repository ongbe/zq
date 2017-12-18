#pragma once
#include "stdafx.h"

#include <wx/datectrl.h>
#include "wx/odcombo.h"
#include "ConfigPanelBase.h"

class CJ_JieSuanDlg : public ConfigPanelBase
{
	enum
	{
		ID_BTN_QUERY,
		ID_BTN_PREVIEW,
		ID_BTN_PRINT,
		ID_BTN_SAVE_AS_FILE,
		ID_BTN_CENTER_SETTLEMENT,
		ID_RADIO_DATE,
		ID_MyEdit,
		ID_DATE
	};

public:
	CJ_JieSuanDlg(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxPanelNameStr);

	~ CJ_JieSuanDlg();

	void InitionInterface();
	void OnQuery(wxCommandEvent& evt);
	void OnPreview(wxCommandEvent& evt);
	void OnPrint(wxCommandEvent& evt);
	void OnSaveFile(wxCommandEvent& event);
	void OnSettle(wxCommandEvent& event);
	void OnSize(wxSizeEvent& evt);
	void OnHisCal(wxCommandEvent& evt);
	void OnQryDayInfo(wxCommandEvent& event);
	void OnLanguageChanged(wxCommandEvent& event);

	DECLARE_EVENT_TABLE()

private:
	wxString m_timestring;

	wxButton* m_pBtnQuery;
	wxButton* m_pBtnPreview;
	wxButton* m_pBtnPrint;
	wxButton* m_pBtnSaveAs;
	wxButton* m_pBtnSettlement;
	wxRadioButton* m_pRadioDate;

	wxDatePickerCtrl* date;
	wxString m_AllContent;
};
