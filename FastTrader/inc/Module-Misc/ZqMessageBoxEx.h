/***************************************************************************//**
*   @file       ZqMessageBox.h
*   @brief      可以保存配置的显示提示信息的对话框（模式的对话框）
*
*   @author     杨桂明 
*   @version    0.0.1
*   @date       created on 2012/06/05
********************************************************************************/

#pragma once

#include "../Module-Misc2/BusinessData.hpp"

//#ifndef _ZQMESSAGEBOXEX_H_
//#define _ZQMESSAGEBOXEX_H_

class CLocalOrderService;

class CZqMessageBoxEx : public wxDialog
{
public:
	enum {	nothing, newOrder, cancelOrder, placeOrder, deleteInstrumentID, condServOverBound };
	enum {	noButton, 
			btnOK = 0x01,
			btnCancel = 0x02,
			btnYes = 0x04,
			btnNo = 0x08 };
	enum {	ID_TIMETIMER = 8090,
			ID_TIMELABEL,		
			ID_AUTOCLOSE		
	};

public:
	CZqMessageBoxEx(wxWindow *parent=NULL);
	~CZqMessageBoxEx();

	BOOL NeedServCondConvertSuccessConfirm();
	BOOL NeedServCondConvertSuccessSound();
	BOOL NeedServCondConvertFailConfirm();
	BOOL NeedServCondConvertFailSound();

	void ResetMessage(wxString& strTitle, wxString& strMessage, int nBtnType=btnOK, int nConfigType=nothing);
	// nMode, 0 条件单 pInputOrder有效，1 止盈, 2 止损 nNum有效 是PLDetail的编号
	void SetCondInfo(CThostFtdcInputOrderField* pInputOrder, long nNum, int nMode) 
	{
		if(pInputOrder!=NULL)
			m_tInputOrder = *pInputOrder;
		m_nPLDetailNum = nNum;
		m_nMode = nMode;
	};
	BOOL DoCond(BOOL bIsAuto=TRUE);

    virtual bool Show(bool show = true);
	void OnBtnClick(wxCommandEvent& event);
	void OnConfigCheck_Click(wxCommandEvent& event);

	void OnOK(wxCommandEvent& event);
	void OnClose(wxCloseEvent& event);
	void OnTimer(wxTimerEvent& event);
	void CreateGUIControls();

private:
	enum
		{
			ID_MESSAGE_TEXT = 1001,
			ID_CHECK_CONFIG,
			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};

private:
    wxTimer		m_timer;//行情更新定时器
	UINT		m_nRemianTime;

	CThostFtdcInputOrderField	m_tInputOrder;
	int			m_nPLDetailNum;
	int			m_nMode;
	
	int			m_nBtnType;
	int			m_nConfigType;
	wxString	m_strTitle;
	wxString	m_strMessage;

	DECLARE_EVENT_TABLE();

};

//#endif