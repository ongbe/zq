#pragma once 

#include <wx/clrpicker.h>

class CFontAndColorSetPanel : public wxPanel
{
public:
	enum
	{
		ID_BTN_QUOTEFONT,
		ID_BTN_OTHERLISTFONT,
		ID_CBX_COLORPLAN,
		ID_COLOR_QUOTE_TEXT,
		ID_COLOR_QUOTE_BACKGROUND,
		ID_COLOR_QUOTE_ODDBACK,
		ID_COLOR_QUOTE_EVENBACK,
		ID_COLOR_QUOTE_TITLETEXT,
		ID_COLOR_QUOTE_TITLEBACK,
		ID_COLOR_INSTRUMENT,
		ID_COLOR_OTHERLIST_TEXT,
		ID_COLOR_OTHERLIST_BACK,
		ID_COLOR_OTHERLIST_ODDBACK,
		ID_COLOR_OTHERLIST_EVENBACK,
		ID_COLOR_OTHERLIST_TITLETEXT,
		ID_COLOR_OTHERLIST_TITLEBACK,
	};

	CFontAndColorSetPanel(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxPanelNameStr);

	~CFontAndColorSetPanel();

	void OnBtnQuoteFont(wxCommandEvent& event);
	void OnBtnOtherListFont(wxCommandEvent& event);
	void OnColorChange( wxColourPickerEvent & event );
	void OnChangeSel(wxCommandEvent& event);

	void ReadCfg();
	void WriteCfg();
	void ApplyColorsToUI();
	void Recovery();

	DECLARE_EVENT_TABLE()

private:
	wxComboBox* m_cbxColorPlan;
	wxButton*  m_btnQuoteFont;
	wxButton*  m_btnOtherListFont;

	wxColourPickerCtrl* m_quoteTextColor;
	wxColourPickerCtrl* m_quoteBackgroungColor;
	wxColourPickerCtrl* m_quoteOddBackColor;
	wxColourPickerCtrl* m_quoteEvenBackColor;
	wxColourPickerCtrl* m_quoteTitleTextColor;
	wxColourPickerCtrl* m_quoteTitleBackgroundColor;
	wxColourPickerCtrl* m_instrumentColor;
	wxColourPickerCtrl* m_otherListTextColor;
	wxColourPickerCtrl* m_otherListBackColor;
	wxColourPickerCtrl* m_otherListOddBackColor;
	wxColourPickerCtrl* m_otherListEvenBackColor;
	wxColourPickerCtrl* m_otherListTitleTextColor;
	wxColourPickerCtrl* m_otherListTitleBackColor;

	wxColour  m_defaultQuoteTextColor;
	wxColour  m_defaultQuoteBackColor;
	wxColour  m_defaultQuoteEvenBackColor;
	wxColour  m_defaultQuoteOddBackColor;
	wxColour  m_defaultQuoteTitleTextColor;
	wxColour  m_defaultQuoteTitleBackColor;
	wxColour  m_defaultInstrumentColor;
	wxColour  m_defaultOtherListTextColor;
	wxColour  m_defaultOtherListBackColor;
	wxColour  m_defaultOtherListEvenBackColor;
	wxColour  m_defaultOtherListOddBackColor;
	wxColour  m_defaultOtherListTitleTextColor;
	wxColour  m_defaultOtherListTitleBackColor;
	wxFont m_defaultQuoteFont;
	wxFont m_defaultOtherListFont; 

	wxColour  m_curQuoteTextColor;
	wxColour  m_curQuoteBackColor;
	wxColour  m_curQuoteEvenBackColor;
	wxColour  m_curQuoteOddBackColor;
	wxColour  m_curQuoteTitleTextColor;
	wxColour  m_curQuoteTitleBackColor;
	wxColour  m_curInstrumentColor;
	wxColour  m_curOtherListTextColor;
	wxColour  m_curOtherListBackColor;
	wxColour  m_curOtherListEvenBackColor;
	wxColour  m_curOtherListOddBackColor;
	wxColour  m_curOtherListTitleTextColor;
	wxColour  m_curOtherListTitleBackColor;

	wxFont  m_curQuoteFont;
	wxFont  m_curOtherListFont;

	wxString m_strCurStyle;
};


