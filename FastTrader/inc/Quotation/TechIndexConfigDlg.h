///-----------------------------------------------------------------
///
/// @file      TechIndexConfigDlg.h
/// @author    Administrator
/// Created:   2012-3-16 9:11:45
/// @section   DESCRIPTION
///            TechIndexConfigDlg class declaration
///
///------------------------------------------------------------------

#ifndef __TechIndexConfigDlg_H__
#define __TechIndexConfigDlg_H__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
	#include <wx/dialog.h>
#else
	#include <wx/wxprec.h>
#endif
using namespace std;
#include <vector>
//Do not add custom headers between 
//Header Include Start and Header Include End.
//wxDev-C++ designer will remove them. Add custom headers after the block.
////Header Include Start
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/spinctrl.h>
////Header Include End

////Dialog Style Start
#undef TechIndexConfigDlg_STYLE
#define TechIndexConfigDlg_STYLE wxCAPTION | wxSYSTEM_MENU | wxDIALOG_NO_PARENT | wxCLOSE_BOX
////Dialog Style End
#define MAX_CONFIG_ITEMS  6

DECLARE_EVENT_TYPE(wxEVT_CONFIG_CHANGE, 3800)

class CInitConfig
{
public:
	wxString	strTechIndex;
	bool		bLeftShow[MAX_CONFIG_ITEMS];
	wxString	strLeft[MAX_CONFIG_ITEMS];
	bool		bConfigShow[MAX_CONFIG_ITEMS];
	int			nConfig[MAX_CONFIG_ITEMS];
	bool		bRightShow[MAX_CONFIG_ITEMS];
	wxString	strRight[MAX_CONFIG_ITEMS];
	vector<int> vecConfigMin;
	vector<int> vecConfigMax;
	CInitConfig()
	{
		strTechIndex = "";
		for(int i =0; i< MAX_CONFIG_ITEMS; i++)
		{
			bLeftShow[i]	= 0;
			strLeft[i]		= "";
			bConfigShow[i]	= 0;
			nConfig[i]		= 0;
			bRightShow[i]	= 0;
			strRight[i]		= "";
		}
	}
	CInitConfig& operator =(const CInitConfig& other)
	{
		strTechIndex = other.strTechIndex;
		for(int i =0; i< MAX_CONFIG_ITEMS; i++)
		{
			bLeftShow[i]	= other.bLeftShow[i];
			strLeft[i]		= other.strLeft[i];
			bConfigShow[i]	= other.bConfigShow[i];
			nConfig[i]		= other.nConfig[i];
			bRightShow[i]	= other.bRightShow[i];
			strRight[i]		= other.strRight[i];
			vecConfigMin	= other.vecConfigMin;
			vecConfigMax	= other.vecConfigMax;
		}
		return *this;
	}
};
class TechIndexConfigDlg : public wxDialog
{
	private:
		DECLARE_EVENT_TABLE();
		
	public:
		TechIndexConfigDlg(){};
		TechIndexConfigDlg(wxWindow *parent, wxWindowID id = 123234, const wxString &title = wxT("TechIndexConfigDlg"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = TechIndexConfigDlg_STYLE);
		virtual ~TechIndexConfigDlg();
	
	private:
		//Do not add custom control declarations between 
		//GUI Control Declaration Start and GUI Control Declaration End.
		//wxDev-C++ will remove them. Add custom code after the block.
		////GUI Control Declaration Start
		wxButton *WxButton1;
		wxButton *WxButton2;

	
		wxStaticText *WxStaticText1[MAX_CONFIG_ITEMS];
		wxSpinCtrl *WxSpinCtrl1[MAX_CONFIG_ITEMS];
		wxStaticText *WxStaticText11[MAX_CONFIG_ITEMS];
		////GUI Control Declaration End
		
	private:
		//Note: if you receive any error with these enum IDs, then you need to
		//change your old form code that are based on the #define control IDs.
		//#defines may replace a numeric value for the enum names.
		//Try copy and pasting the below block in your old form header files.
		enum
		{
			////GUI Enum Control ID Start
			ID_DUMMY_START = 1000,
			ID_WXBUTTON1,
			ID_WXBUTTON2,
			ID_WXSTATICTEXT1,
			ID_WXSTATICTEXT2,
			ID_WXSTATICTEXT3,
			ID_WXSTATICTEXT4,
			ID_WXSTATICTEXT5,
			ID_WXSTATICTEXT6,

			ID_WXSTATICTEXT11,
			ID_WXSTATICTEXT21,
			ID_WXSTATICTEXT31,
			ID_WXSTATICTEXT41,
			ID_WXSTATICTEXT51,
			ID_WXSTATICTEXT61,
			
			ID_WXSPINCTRL6,
			ID_WXSPINCTRL5,
			ID_WXSPINCTRL4,
			ID_WXSPINCTRL3,
			ID_WXSPINCTRL2,
			ID_WXSPINCTRL1,
			////GUI Enum Control ID End
			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};
	
	private:
		void OnClose(wxCloseEvent& event);
		void CreateGUIControls();

		 void OnRestore( wxCommandEvent &event );
		 void OnCloseEvent( wxCommandEvent &event );

		
		 CInitConfig m_cInitConfig;
		 void InitDialog( CInitConfig&  cInitConfig);

public:
		 void SetTechIndex(wxString strTechIndex, EnumPhrase enumPhrase, vector<int>& vecValue);
		 void SetMsgWnd(wxWindow *pMsgWnd){ m_pMsgWnd = pMsgWnd; }

protected:
	 void OnSpinUp6( wxCommandEvent& event);
	 void OnSpinUp5( wxCommandEvent& event);
	 void OnSpinUp4( wxCommandEvent& event);
	 void OnSpinUp3( wxCommandEvent& event);
	 void OnSpinUp2( wxCommandEvent& event);
	 void OnSpinUp1( wxCommandEvent& event );
	
	 void OnSpinValue(int nID, int nValue);
	 wxWindow *m_pMsgWnd;
	 bool m_bInit;

	 bool WriteConfigFrmFile(string strTechIndexName, EnumPhrase enumPhrase, vector<int>& vecCofig, bool bDel = false);
	 bool m_bDelConfig;   //恢复默认配置，需要从配置中删除
	 bool m_bWriteConfig; //改变了默认的参数，需要写配置文件
	EnumPhrase m_enumPhrase;
public:
	vector<int>  m_vecConfig;
};

#endif
