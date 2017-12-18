#pragma once
#include <wx/button.h>
#include <wx/textctrl.h>
#include "CMovePositionServiceThread.h"
#include "CModifyOrderDlg.h"
#include "CCustomSpinCtrl.h"

class CRadioButtonGroup;
class CSwapPositionDlg : public wxDialog
{

	enum
	{
		RADIO_SHIJIA = 0,
		RADIO_ZUIXINJIA,
		RADIO_BUSINESS,
		RADIO_ASSIGN
	};

	struct ORDER_OPTION_SWAP
	{
		int nSourcePriceType1;
		int nDestPriceType1;
		long lSourceHuaDong1;
		double fSourceAssign1;
		long lDestHuaDong1;
		double fDestAssign1;
		int nSourceHandle1;
		long lSourceTimeoutHuaDong1;
		int nDestHandle1;
		long lDestTimeoutHuaDong1;

		int nSourcePriceType2;
		int nDestPriceType2;
		long lSourceHuaDong2;
		double fSourceAssign2;
		long lDestHuaDong2;
		double fDestAssign2;
		int nSourceHandle2;
		long lSourceTimeoutHuaDong2;
		int nDestHandle2;
		long lDestTimeoutHuaDong2;

		int nTimeout;
		int nRetryTimes;
	};

	struct SWAP_DIALOG_STATE
	{
		bool bCheck;
		int nOperOrder;
		int nCabin1;
		int nCabin2;
		wxString Cabin1;
		wxString Cabin2;

		int nSourcePriceType1;
		int nDestPriceType1;
		int nSourceHandle1;
		int nDestHandle1;
		wxString SourceHuaDong1;
		wxString DestHuaDong1;
		wxString SourceAssign1;
		wxString DestAssign1;
		wxString SourceTimeoutHuaDong1;
		wxString DestTimeoutHuaDong1;

		int nSourcePriceType2;
		int nDestPriceType2;
		int nSourceHandle2;
		int nDestHandle2;
		wxString SourceHuaDong2;
		wxString DestHuaDong2;
		wxString SourceAssign2;
		wxString DestAssign2;
		wxString SourceTimeoutHuaDong2;
		wxString DestTimeoutHuaDong2;

		wxString Timeout;
		wxString RetryTimes;

	};

	struct STATIC_TITLE_DATA
	{
		string ID;
		string Direct;
		int nVol;
		PlatformStru_Position PositionStruct;

		STATIC_TITLE_DATA()
		{
			//memset(ID, 0, sizeof(ID));
			//memset(Direct, 0, sizeof(Direct));
			nVol = -1;

		}

	};

public:
	CSwapPositionDlg(wxWindow *parent);
	virtual ~CSwapPositionDlg();

	virtual bool Show(bool show = true);

	void SetPosition(PlatformStru_Position& tPositionData) {
		m_tPositionData1 = tPositionData;
	};
	void SetPosition(PlatformStru_Position& tPositionData1, PlatformStru_Position& tPositionData2) {
		m_tPositionData1 = tPositionData1;
		m_tPositionData2 = tPositionData2;
	};
	BOOL Init(CMovePositionServiceThread* poMovePositionServiceThread, vector<PlatformStru_Position> & vecPositionInfo);
	
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
	void OnChildFocus(wxChildFocusEvent& event);
	void OnComboboxSelectChange(wxCommandEvent& event);
	void OnSpinUpPress(wxSpinEvent& event);
	void OnSpinDownPress(wxSpinEvent& event);
	void OnCheckClick(wxCommandEvent& event);
	void OnRadioButton(wxCommandEvent& event);
	void OnSetDefault(wxCommandEvent& event);

private:
	void OnClose(wxCloseEvent& event);
	void OnRcvTimer(wxCommandEvent& event);
	void OnRcvOrderLog(wxCommandEvent& event);
	void OnRcvTradeLog(wxCommandEvent& event);
	void OnRcvAction(wxCommandEvent& event);
	void CreateGUIControls();
	void SaveLastState();
	void ReadLastState();

	BOOL NeedSimulateMarketPrice();

	void SetAble(bool bPart1, bool bPart2 = false, bool bAll = true);
	bool SendRequest(bool bIsRedo=false);
	void SetStaticTitle(STATIC_TITLE_DATA * pData1, STATIC_TITLE_DATA * pData2);
	void SetDefault();
	void LoadConfig();
	void SaveConfig();
	void PrintChange();
	bool InitGrid();
	void UpdateAllRecord();
	void InitAllCabin();
	void UpdateAllCabin(wxString & cabinlable1, wxString & cabinlable2);
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
		ID_WXEDIT1 = 10100,
		ID_WXEDIT2 ,
		ID_WXEDIT3 ,
		ID_WXEDIT4 ,
		ID_WXEDIT5 ,
		ID_WXEDIT6 ,
		ID_WXEDIT7 ,
		ID_WXEDIT8 ,
		ID_WXEDIT9 ,
		ID_WXEDIT10 ,
		ID_WXEDIT11 ,
		ID_WXEDIT12 ,
		ID_WXEDIT13 ,
		ID_WXEDIT14 ,
		ID_WXSPINCTRL1 ,
		ID_WXSPINCTRL2 ,
		ID_WXSPINCTRL3 ,
		ID_WXSPINCTRL4 ,
		ID_WXRADIOBUTTON1 ,
		ID_WXRADIOBUTTON2 ,
		ID_WXRADIOBUTTON3 ,
		ID_WXRADIOBUTTON4 ,
		ID_WXRADIOBUTTON5 ,
		ID_WXRADIOBUTTON6 ,
		ID_WXRADIOBUTTON7 ,
		ID_WXRADIOBUTTON8 ,
		ID_WXRADIOBUTTON9 ,
		ID_WXRADIOBUTTON10 ,
		ID_WXRADIOBUTTON11 ,
		ID_WXRADIOBUTTON12 ,
		ID_WXRADIOBUTTON13 ,
		ID_WXRADIOBUTTON14 ,
		ID_WXRADIOBUTTON15 ,
		ID_WXRADIOBUTTON16 ,
		ID_WXCOMBOBOX4 ,
		ID_WXCOMBOBOX5 ,
		ID_WXCOMBOBOX6 ,
		ID_WXCOMBOBOX7 ,
		ID_WXCOMBOBOX1 ,
		ID_WXCOMBOBOX2 ,
		ID_WXCOMBOBOX3 ,
		ID_WXCHECKBOX1 ,
		ID_WXBUTTON1 ,
		ID_WXBUTTON2 ,
		ID_WXBUTTON3 ,
		ID_WXBUTTONCLOSE ,
		ID_WXBUTTONEDIT ,
		ID_WXLISTCTRL1 ,
		ID_WXLISTCTRL2 ,
		ID_WXLISTCTRL3 ,
		ID_WXLISTCTRL4 ,
		ID_WXLISTCTRL5 ,
		ID_WXLISTCTRL6 ,
		ID_WXLISTCTRL7 ,
		ID_WXLISTCTRL8 ,
		ID_WXSTATICBOX1 ,
		ID_WXSTATICBOX2 ,
		ID_WXSTATICBOX3 ,
		ID_WXSTATICBOX4 ,
		ID_WXSTATICBOX5 ,
		ID_WXSTATICBOX6 ,
		ID_WXSTATICBOX7 ,
		ID_WXSTATICBOX8 ,
		ID_WXSTATICBOX13 ,
		ID_WXSTATICBOX14 ,
		ID_WXSTATICBOX15 ,
		ID_WXSTATICBOX16 ,
		ID_WXNOTEBOOK1 ,
		ID_WXNOTEBOOK2 ,
		ID_WXNOTEBOOKPAGE1 ,
		ID_WXNOTEBOOKPAGE2 ,
		ID_WXNOTEBOOKPAGE3 ,
		ID_WXNOTEBOOKPAGE4 ,
		ID_WXSTATICTEXT1 ,
		ID_WXSTATICTEXT2 ,
		ID_WXSTATICTEXT3 ,
		ID_WXSTATICTEXT4 ,
		ID_WXSTATICTEXT5 ,
		ID_WXSTATICTEXT6 ,
		ID_WXSTATICTEXT7 ,
		ID_WXSTATICTEXT8 ,
		ID_WXSTATICTEXT9 ,
		ID_WXSTATICTEXT10 ,
		ID_WXSTATICTEXT11 ,
		ID_WXSTATICTEXT12 ,
		ID_WXSTATICTEXT13 ,
		ID_WXSTATICTEXT14 ,
		ID_WXSTATICTEXT15 ,
		ID_WXSTATICTEXT16 ,
		ID_WXSTATICTEXT17 ,
		ID_WXSTATICTEXT18 ,
		ID_WXSTATICTEXT19 ,
		ID_WXSTATICTEXT20 ,
		ID_WXSTATICTEXT21 ,
		ID_WXSTATICTEXT22 ,
		ID_WXSTATICTEXT23 ,
		ID_WXSTATICTEXT24 ,
		ID_WXSTATICTEXT25 ,
		ID_WXSTATICTEXT26 ,
		ID_WXSTATICTEXT27 ,
		ID_WXSTATICTEXT28 ,
		ID_WXSTATICTEXT29 ,
		ID_WXSTATICTEXT30 ,
		ID_WXSTATICTEXT31 ,
		ID_WXSTATICTEXT32 ,
		ID_WXSTATICTEXT33 ,
		ID_WXSTATICTEXT34 ,
		ID_WXSTATICTEXT35 ,
		ID_WXSTATICTEXT36 ,
		ID_WXSTATICTEXT37 ,
		ID_WXNOTEBOOKPAGE12 ,
		ID_WXNOTEBOOKPAGE11 ,
		ID_WXNOTEBOOK6 ,
		ID_WXNOTEBOOKPAGE10 ,
		ID_WXNOTEBOOKPAGE9 ,
		ID_WXNOTEBOOK5 ,
		ID_WXNOTEBOOKPAGE8 ,
		ID_WXNOTEBOOKPAGE7 ,
		ID_WXNOTEBOOK3 ,
		ID_WXNOTEBOOK4 ,
		ID_WXNOTEBOOKPAGE6 ,
		ID_WXNOTEBOOKPAGE5 ,
        ID_MARKETTYPE_CONFIRMMSG_STATIC,
		////GUI Enum Control ID End
		ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
	};


private:
	CMovePositionServiceThread*		m_poMovePositionServiceThread;
	PlatformStru_Position	m_tPositionData1;
	PlatformStru_Position	m_tPositionData2;

	double m_fPriceTick1, m_fPriceTick2;

	CModifyOrderDlg * m_pModifyOrderDlg;

	std::map<wxString, STATIC_TITLE_DATA> m_mapStaticData;
	std::vector<PlatformStru_Position> m_vecPositionStruct;

	std::map<int, long> m_FieldID2ColumnIDs[4];			//记录字段在哪一列显示 
	std::map<int, LIST_COLUMN_CFG> m_FieldAttrs[4];		//记录字段的显示属性
	std::map<int, DataRtnOrder*> m_RowDatas[4];			//记录字段的显示属性

	CMovePositionServiceThread::SWAPPOSITION	m_tSwap;
	CMovePositionServiceThread::MOVEPOSITION	m_tMovePosition1, m_tMovePosition2;

	ORDER_OPTION_SWAP m_curOrderOption;
	SWAP_DIALOG_STATE m_lastState;

	//Dialog control 
	CCustomSpinCtrl * m_pCustomSpinCtrl1, *m_pCustomSpinCtrl2, *m_pCustomSpinCtrl3, *m_pCustomSpinCtrl4;
	CNumericTextCtrl * m_pCNumericTextCtrl1, * m_pCNumericTextCtrl2, * m_pCNumericTextCtrl3, * m_pCNumericTextCtrl4, * m_pCNumericTextCtrl5,
		* m_pCNumericTextCtrl6, * m_pCNumericTextCtrl7, * m_pCNumericTextCtrl8, * m_pCNumericTextCtrl9, * m_pCNumericTextCtrl10; 
	wxComboBox * m_pComboBox1, * m_pComboBox2, * m_pComboBox3, * m_pComboBox4, * m_pComboBox5, * m_pComboBox6, * m_pComboBox7;

	CRadioButtonGroup * m_pRadioGroupOriginal1, * m_pRadioGroupDest1, * m_pRadioGroupOriginal2, * m_pRadioGroupDest2;

	DECLARE_EVENT_TABLE();
};
