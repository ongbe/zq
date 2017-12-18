struct StatusBarField
{
public:
	StatusBarField():
	  m_TimeDiff(0),m_static(NULL)
   {

   }
   time_t		m_TimeDiff;//与本地时间的差
   wxWindow*    m_static;//静态控件ID
};
class LinkStatusDlg;
class MyStatusBar : public wxStatusBar
{
public:
    MyStatusBar(wxWindow *parent);
    virtual ~MyStatusBar();
	void  OnLinkStatus( wxMouseEvent& event );   
    void  LBtnLeftDown( wxMouseEvent& event );   
	void OnTimer( wxTimerEvent& event);
    void OnSize(wxSizeEvent& evt);
	bool GetFieldRect(int i, wxRect& rect) const;
    wxString GetStatusString(int status,bool bdetail=false);
    wxDateTime GetDateTimeFromString(const char* pzStr);
	void OnLanguageChanged(wxCommandEvent& event);
    int GetValidExchangeIDs(vector<string>& outData);
private:
  enum
    {
        Field_LocalTime,
        Field_Msg,
        Field_LayoutStyle,
        Field_ReConnect,
        Field_QuotTrader,
        Field_Exchange
    }; 
	LinkStatusDlg* m_LinkStatusDlg;
	int m_nStyle;
	vector<StatusBarField> m_ExchangeFields;
    wxTimer m_timer;
    DECLARE_EVENT_TABLE()

};
