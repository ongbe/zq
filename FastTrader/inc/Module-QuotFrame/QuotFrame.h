#pragma once
class CTimeChartContainer;
class CQuotFrame :
	public wxPanel
{
public:
	CQuotFrame(wxWindow *parent,
            wxWindowID winid =GID_QUOT_TABLE,//wxID_ANY,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxTAB_TRAVERSAL | wxNO_BORDER,
            const wxString& name = wxPanelNameStr);
	virtual ~CQuotFrame(void);
	void OnSize( wxSizeEvent& event );
    wxPanel* GetActivePanel();
	void MoveToPluginView();
	void MoveToQuotView();
	void MoveNextView();
	void TranslateCommandEvent(wxCommandEvent& event);
	void OnPluginNewQuote(wxCommandEvent& event);
	void OnGridDblClk(wxCommandEvent& evt);
    void OnEraseBackground(wxEraseEvent& event);

    bool GetCurInstrumentGrpInfo(std::string& GrpName,int& GrpID);
    void GetCurInstrumentID(std::string& strInstrument);
    //插件要求新建或删除面板，type==1新建；2删除
    wxWindow* Plugin_New_Del_Panel(int type);

private:
	bool m_bCanMoveNext;
	wxPanel*         m_QuotPanel;
	CTimeChartContainer*  m_PluginPanel;
	DECLARE_EVENT_TABLE()
};
