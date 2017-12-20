class CFloatPanelDlg:public wxDialog
{
public:
    CFloatPanelDlg(wxWindow* parent);
    void SetPanel(wxWindow* pPanel);
    void OnSize(wxSizeEvent& evt);
    void DoSize();
	DECLARE_EVENT_TABLE()
private:
    wxWindow* m_panel;
};