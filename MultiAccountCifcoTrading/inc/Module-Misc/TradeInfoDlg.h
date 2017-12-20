/***************************************************************************//**
*   @file         TradeInfoDlg.h
*   @brief        交易信息提示对话框（非模式的对话框）
*
*   @author     jacky <panxun.li@cithinc.com>
*   @version    0.0.1
*   @date       created on 2010/10/11
********************************************************************************/
#ifndef _TRADEINFODLG_H_
#define _TRADEINFODLG_H_

#include <wx/button.h>
#include <wx/textctrl.h>
#include <set>

struct LOG_DATA;

class CTradeInfoDlg:public wxDialog
{
public:
	CTradeInfoDlg(wxWindow *parent);
	~CTradeInfoDlg();
    static CTradeInfoDlg *GetInstance(wxWindow *parent=NULL);
	static void Release();
    void WxButton1Click(wxCommandEvent& event);
    void OnBtnNext(wxCommandEvent& event);
    void OnBtnPrev(wxCommandEvent& event);
	void OnConfigCheck_Click(wxCommandEvent& event);
    void OnSize(wxSizeEvent& event);
    void Reload();
    virtual bool Show(bool show = true);
	void ResetLanguage();
	void ShowTradeMsg(const wxString& strInfo, int nInfoId, const wxString& strTitle, int nTitleId, BOOL bIsShowDlg);
	void ShowCheckConfig();
	void WriteLog( LOG_DATA* pData);

private:
    void SetInfo(const wxString& info,int id=-1);
    void SetTitle( const wxString& title,int id=-1);
	void SetMessage( const wxString& title, const wxString& info, int id);

	void InitConfig();

	void OnClose(wxCloseEvent& event);
	void CreateGUIControls();
private:
	wxButton *WxButton1;	
	wxStaticText *WxStaticText1;
    //static CTradeInfoDlg* m_thisDlg;
    wxArrayString m_TitleArray;
    wxArrayString m_InfoArray;
	enum
		{
			ID_ONLYLINE=1009,
			ID_WXBUTTON1,
            ID_BTN_PREV,
            ID_BTN_NEXT,
			ID_CHECK_CONFIG,
			ID_WXSTATICTEXT1 = 1003,
			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};
	std::set<int> m_setSysID;
    int m_current;
    //int m_LocalTitleId;
    //int m_LocalInfoId;
	bool m_bInitialized;

	bool m_bCancelSuccessDlg;
	bool m_bCancelFailDlg;
	bool m_bOrderFailDlg;
	bool m_bOrderSuccessDlg;
	bool m_bTradeDlg;

	bool m_bCancelSuccessDlgChg;
	bool m_bCancelFailDlgChg;
	bool m_bOrderFailDlgChg;
	bool m_bOrderSuccessDlgChg;
	bool m_bTradeDlgChg;

private:
	DECLARE_EVENT_TABLE();

};

#define TRADEINFODLG(parent)        (CTradeInfoDlg::GetInstance())
#define TRADEINFODLG_CREATE(parent) (CTradeInfoDlg::GetInstance(GETTOPWINDOW()))
#define TRADEINFODLG_RELEASE()      (CTradeInfoDlg::Release())
#endif