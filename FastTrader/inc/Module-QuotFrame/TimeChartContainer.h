#pragma once
#include "Module-MainNoteBook/zqAuiNotebook.h"
class CTimeChartContainer :
	public zqPanel
{
public:
	CTimeChartContainer(wxWindow *parent,
            wxWindowID winid =wxID_ANY,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxTAB_TRAVERSAL | wxNO_BORDER,
            const wxString& name = wxPanelNameStr);
	~CTimeChartContainer(void);
	void SendInstrumentMsg(wxPanel* panel);
	void OnLeftDClick(wxMouseEvent& event);
	void OnPluginHide(wxCommandEvent& event);
	DECLARE_EVENT_TABLE()
};
