class CImportExport;
class AskNameDlg: public wxDialog
{
public:
	enum{ID_BTN_LOAD_WITHOUT_SAVE=100};
    AskNameDlg(wxWindow* parent,CImportExport* pImportExport);
    ~AskNameDlg();
    wxString GetCaption();
    void OnDel(wxCommandEvent& event);
    void OnOK(wxCommandEvent& event);
    DECLARE_EVENT_TABLE()
private:
	CImportExport* m_pImportExport;
    wxComboBox* m_cmbName; 
};