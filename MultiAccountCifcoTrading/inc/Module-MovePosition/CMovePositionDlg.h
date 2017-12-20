#pragma once
#include <wx/button.h>
#include <wx/textctrl.h>
#include "CMovePositionServiceThread.h"
#include "CModifyOrderDlg.h"
#include "CCustomSpinCtrl.h"

class CRadioButtonGroup;
class CMovePositionDlg : public wxDialog
{

	enum
	{
		RADIO_SHIJIA = 0,
		RADIO_ZUIXINJIA,
		RADIO_BUSINESS,
		RADIO_ASSIGN
	};

	struct ORDER_OPTION
	{
		int nSourcePriceType;
		int nDestPriceType;

		long lSourceHuaDong;
		double fSourceAssign;
		long lDestHuaDong;
		double fDestAssign;
		
		int nTimeout;
		int nRetryTimes;
		int nSourceHandle;
		long lSourceTimeoutHuaDong;
		int nDestHandle;
		long lDestTimeoutHuaDong;
	};

	struct MOVE_DIALOG_STATE
	{
		int nDestContract;
		int nOperOrder;
		
		int nSourcePriceType;
		int nDestPriceType;

		wxString SourceHuaDong;
		wxString SourceAssign;
		wxString DestHuaDong;
		wxString DestAssign;

		wxString Timeout;
		wxString RetryTimes;
		int nSourceHandle;
		wxString SourceTimeoutHuaDong;
		int nDestHandle;
		wxString DestTimeoutHuaDong;
	};

public:
	CMovePositionDlg(wxWindow *parent);
	virtual ~CMovePositionDlg();

	virtual bool Show(bool show = true);

	void SetPosition(PlatformStru_Position& tPositionData) {
		m_tPositionData = tPositionData;
	};
	BOOL Init(CMovePositionServiceThread* poMovePositionServiceThread);

	void ReSendRequest();
	void RollBack();
	void DoCancel();
	void ShowTradeInfoDlg(const string& Title, const string& errormessage, BOOL bIsShow);

public:
	void OnHookKeyPress(wxKeyEvent& evt); 

protected:
	void OnStart(wxCommandEvent& event);
	void OnEdit(wxCommandEvent& event);
	void OnCancel(wxCommandEvent& event);
	void OnBtnClose(wxCommandEvent& event);
	void OnComboboxSelectChange(wxCommandEvent& event);
	void OnChildFocus(wxChildFocusEvent& event);
	void OnSpinUpPress(wxSpinEvent& event);
	void OnSpinDownPress(wxSpinEvent& event);
	void OnRadioButton(wxCommandEvent& event);
	void OnSetDefault(wxCommandEvent& event);

private:
	void OnClose(wxCloseEvent& event);
	void OnRcvTimer(wxCommandEvent& event);
	void OnRcvOrderLog(wxCommandEvent& event);
	void OnRcvTradeLog(wxCommandEvent& event);
	void OnRcvAction(wxCommandEvent& event);
	void CreateGUIControls();

	BOOL NeedSimulateMarketPrice();

	void SetAble(bool bPart1, bool bPart2 = false, bool bAll = true);
	void SendRequest(bool bIsRedo=false);
	void SetStaticTitle(wxString & strID);
	void SetDefault();
	void LoadConfig();
	void SaveConfig();
	void PrintChange();
	void SaveLastState();
	void ReadLastState();

	bool InitGrid();
	void UpdateAllRecord();
	int UpdateOrderList( const PlatformStru_OrderInfo& rawData, wxListCtrl* pwxListCtrl, std::map<int, long>& fieldID2ColumnID);
	int LoadOneOrderList( int iRowNo, const PlatformStru_OrderInfo &rawData, wxListCtrl* pwxListCtrl, std::map<int, long>& fieldID2ColumnID);

private:

	//Note: if you receive any error with these enum IDs, then you need to
	//change your old form code that are based on the #define control IDs.
	//#defines may replace a numeric value for the enum names.
	//Try copy and pasting the below block in your old form header files.
	enum
	{
		////GUI Enum Control ID Start
		ID_WXEDIT1 = 10200,
		ID_WXEDIT2 ,//=1026,
		ID_WXEDIT3 ,//=1033,
		ID_WXEDIT4 ,//=1036,
		ID_WXEDIT5 ,//=1041,
		ID_WXEDIT6 ,//=1045,
		ID_WXEDIT7 ,
		ID_WXEDIT8 ,
		ID_WXSPINCTRL1 ,//=1016,
		ID_WXSPINCTRL2 ,//=1028,
		ID_WXRADIOBUTTON1 ,//=1004,
		ID_WXRADIOBUTTON2 ,//=1007,
		ID_WXRADIOBUTTON3 ,//=1008,
		ID_WXRADIOBUTTON4 ,//=1011,
		ID_WXRADIOBUTTON5 ,//=1005,
		ID_WXRADIOBUTTON6 ,//=1023,
		ID_WXRADIOBUTTON7 ,//=1024,
		ID_WXRADIOBUTTON8 ,//=1027,
		ID_WXCOMBOBOX3 ,//=1039,
		ID_WXCOMBOBOX4 ,//=1043,
		ID_WXCOMBOBOX1 ,//=1018,
		ID_WXCOMBOBOX2 ,//=1030,
		ID_WXBUTTON1 ,//=1074,
		ID_WXBUTTON2 ,//=1075,
		ID_WXBUTTON3 ,//=1076,
		ID_WXBUTTONCLOSE ,//=1076,
		ID_WXBUTTONEDIT ,//=1076,
		ID_WXLISTCTRL1 ,//=1082,
		ID_WXLISTCTRL2 ,//=1083,
		ID_WXLISTCTRL3 ,
		ID_WXLISTCTRL4 ,
		ID_WXSTATICBOX1 ,//=1006,
		ID_WXSTATICBOX2 ,
		ID_WXSTATICBOX3 ,
		ID_WXSTATICBOX4 ,//=1046,
		ID_WXSTATICBOX5 ,//=1057,
		ID_WXSTATICBOX6 ,//=1069,
		ID_WXSTATICTEXT1 ,//=1001,
		ID_WXSTATICTEXT2 ,//=1002,
		ID_WXSTATICTEXT3 ,//=1003,
		ID_WXSTATICTEXT4 ,//=1009,
		ID_WXSTATICTEXT5 ,//=1017,
		ID_WXSTATICTEXT6 ,//=1019,
		ID_WXSTATICTEXT7 ,//=1025,
		ID_WXSTATICTEXT8 ,//=1029,
		ID_WXSTATICTEXT9 ,//=1032,
		ID_WXSTATICTEXT10 ,//=1034,
		ID_WXSTATICTEXT11 ,//=1035,
		ID_WXSTATICTEXT12 ,//=1037,
		ID_WXSTATICTEXT13 ,//=1038,
		ID_WXSTATICTEXT14 ,//=1040,
		ID_WXSTATICTEXT15 ,//=1042,
		ID_WXSTATICTEXT16 ,//=1044,
		ID_WXSTATICTEXT17 ,//=1047,
		ID_WXSTATICTEXT18 ,//=1081,
		ID_WXSTATICTEXT19 ,//=1080,
		ID_WXSTATICTEXT20 ,//=1079,
		ID_WXNOTEBOOKPAGE5 ,
		ID_WXNOTEBOOKPAGE4 ,
		ID_WXNOTEBOOK3 ,
		ID_WXNOTEBOOKPAGE3 ,
		ID_WXNOTEBOOKPAGE1 ,
		ID_WXNOTEBOOK1 ,
		ID_WXNOTEBOOKPAGE2 ,
        ID_MARKETTYPE_CONFIRMMSG_STATIC,
		////GUI Enum Control ID End
		ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
	};
private:

	BOOL	m_bRunning;

	CMovePositionServiceThread*		m_poMovePositionServiceThread;
	PlatformStru_Position	m_tPositionData;
	double			m_fPriceTick;
	CMovePositionServiceThread::MOVEPOSITION	m_tMove;

	std::vector<wxString> m_vecStaticData;

	std::map<int, long> m_FieldID2ColumnIDs[2];			//记录字段在哪一列显示 
	std::map<int, LIST_COLUMN_CFG> m_FieldAttrs[2];		//记录字段的显示属性
	std::map<int, DataRtnOrder*> m_RowDatas[2];			//记录字段的显示属性

	wxString m_strInstrumentIDBackup;

	CModifyOrderDlg * m_pModifyOrderDlg;
	ORDER_OPTION m_curOrderOption;
	MOVE_DIALOG_STATE m_lastState;

	//Dialog control 
	CCustomSpinCtrl * m_pCustomSpinCtrl1, *m_pCustomSpinCtrl2;
	CNumericTextCtrl * m_pCNumericTextCtrl1, * m_pCNumericTextCtrl2, * m_pCNumericTextCtrl3, * m_pCNumericTextCtrl4, * m_pCNumericTextCtrl5, * m_pCNumericTextCtrl6; 
	wxComboBox * m_pComboBox1, * m_pComboBox2, * m_pComboBox3, * m_pComboBox4;

	CRadioButtonGroup * m_pRadioGroupOriginal, * m_pRadioGroupDest;
	DECLARE_EVENT_TABLE();
};
