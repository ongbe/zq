
#pragma once
#include "stdafx.h"
#include "../Module-Misc/LogDefine.h"

struct LOG_DATA;

class LogDlg :
    public wxDialog
{
public:
    enum
    {
        ID_GAUGE=100,
        ID_CHOICE,
        ID_BUTTON,
        ID_USERID,
        ID_CHECK1,
        ID_PASSWORD,
        ID_BUTTON2,
        TIMER_ID
    };

	struct LogInfoMulti
	{
		LogInfoMulti()
		{
			memset(this, 0, sizeof(*this));
		}
		char strType[64];	
		LOG_DATA* pData;
	};

public:
    static LogDlg* GetInstance();
    static LogDlg* CreateLogDlg(wxWindow*);
	static void DestroyInstance();

	void WriteLog(std::string strType, LOG_DATA* pData, wxString strTime=wxEmptyString);
    void InitListCtrl(bool );
    void OnExit(wxCommandEvent& event);
    //void PrintSortSystemTime();
    wxString GetLastMsg();
	void OnSize(wxSizeEvent& evt);
	void OnLanguageChanged(wxCommandEvent& event);
    DECLARE_EVENT_TABLE()

private:
	LogDlg(wxWindow *parent);
	~LogDlg();
	std::string GetLogString(LOG_DATA* pData);
	std::string GetLogParam( LOG_PARAM logParam );
	
private:
	static LogDlg* m_pLogDlg;
	wxExtListCtrl* m_pList;
};
