#pragma once

#include <vector>
#include <wx/datectrl.h>
#include "../inc/Module-Misc/FieldValueTool.h"
using namespace std;

typedef struct
{
	string date;
	string jiaoyisuo;
	string heyue;
	string maimai;
	string openclose;
	string volume;
	string price;
	string shouxufei;
	string jiaoyifei;
	string jiaoyibianma;
	string xiweihao;
    string xitonghao;
	string chengjiaohao;
	string chengjiaoshijian;
	string toubao;
	string jiaogeqi;
}RecordInfo;

class CJ_TradeRecordDlg :public wxPanel
{
public:

	enum
	{
      ID_Bt1,
	  ID_Bt2,
	  ID_Com
	};

    CJ_TradeRecordDlg(wxWindow *parent,
            wxWindowID winid = wxID_ANY,
            const wxPoint& pos = wxPoint(0,0),
            const wxSize& size = wxSize(800,280),
            long style = wxTAB_TRAVERSAL | wxNO_BORDER,
            const wxString& name = wxPanelNameStr);

   ~CJ_TradeRecordDlg();

  // void OnSize( wxSizeEvent& event );

   wxButton* Bt1;
   wxButton* Bt2;
   wxComboBox* Com;
   wxDatePickerCtrl* date1;
   wxDatePickerCtrl* date2;
   wxListCtrl* List;

   DECLARE_EVENT_TABLE()

   vector<RecordInfo> m_MingXiVec;
   vector<RecordInfo> m_HuiZongVec;
   
public:
  
	 
public:

	void OnSize( wxSizeEvent& event );
	void OnQuery(wxCommandEvent& evt);

    void OnCancel(wxCommandEvent& evt);

	void OnRcvTrade(wxCommandEvent& evt);
	void OnRcvTradeRecord(wxCommandEvent& evt);

	void  InitionInteface();

	int FindRowToInsert( PlatformStru_TradeInfo& rawData );
 //  	void SetTotalData(CThostFtdcTradeField& rawData);
	//void SetDeTailData(CThostFtdcTradeField& rawData);

    //void OnCfgChanged(wxCommandEvent& evt);
    void SetDetailListAfterCfg();
	void SetTotalListAfterCfg();

	double GetShouXuFei( PlatformStru_TradeInfo & raw );
    wxString GetTotalCellText(const wxString& strColText,int row);
    wxString GetDetailCellText(const wxString& strColText,int row);
public:

    std::map<TradeKey,  PlatformStru_TradeInfo> m_outData;
	vector< DatailInfo> m_DatailData;
	vector< TotalInfo >  m_TotalData;

};
