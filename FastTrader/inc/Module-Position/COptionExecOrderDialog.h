#pragma once
#include "stdafx.h"
#include "../inc/Module-Misc2/KernelStruct.h"

// 执行类型
///TFtdcActionTypeType是一个执行类型类型
//ActionType    	执行还是放弃

//Volume		数量

//OffsetFlag	开平

/////净
//#define THOST_FTDC_PD_Net '1'
/////多头
//#define THOST_FTDC_PD_Long '2'
/////空头
//#define THOST_FTDC_PD_Short '3'
///保留头寸申请的持仓方向
//PosiDirection

/////TFtdcExecOrderPositionFlagType是一个期权行权后是否保留期货头寸的标记类型
//保留或不保留
//ReservePositionFlag

/////期权行权后生成的头寸是否自动平仓
///TFtdcExecOrderCloseFlagType是一个期权行权后生成的头寸是否自动平仓类型
///自动平仓
///免于自动平仓
//CloseFlag



class COptionExecOrderDialog : public wxDialog
{
public:

	enum
	{
		ID_ACTIONTYPE_COMB=2001,
		ID_OFFSETFLAG_COMB,
		ID_POSIDIRECTION_COMB,
		ID_RESERVEPOSITIONFLAG_COMB,
		ID_CLOSEFLAG_COMB,
		ID_VOLUME_TEXT,
		ID_OK,
		ID_CANCEL,
		ID_INSTRUMENTID_STATIC,
		ID_STATIC,
		SUMNUM_ID	// 标志ID号的数量
	};


public:
	COptionExecOrderDialog(wxWindow *parent, wxWindowID id,
             const wxString& title,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = wxDEFAULT_DIALOG_STYLE,
             const wxString& name = wxDialogNameStr);
	~COptionExecOrderDialog();
    
	static COptionExecOrderDialog* CreateDailog(wxWindow* parent);
	void SetOptionPosition(PlatformStru_Position& position) {
		m_Position = position;
		wxStaticText* pStc = (wxStaticText*)FindWindowById(ID_INSTRUMENTID_STATIC, this);
		if(pStc != NULL) {
			pStc->SetLabel(wxString("期权合约: ")+position.InstrumentID);
		}
		wxTextCtrl* pTxt = (wxTextCtrl*)FindWindowById(ID_VOLUME_TEXT, this);
		if(pTxt != NULL) {
			wxString strText;
			strText.Printf("%d", position.CanCloseVolume);
			pTxt->SetValue(strText);
		}
	};

	void OnBtnOrder(wxCommandEvent& evt);
	void OnBtnClose(wxCommandEvent& evt);

	DECLARE_EVENT_TABLE()

public:
	void ResetLanguage();

private:
	PlatformStru_Position m_Position;
};