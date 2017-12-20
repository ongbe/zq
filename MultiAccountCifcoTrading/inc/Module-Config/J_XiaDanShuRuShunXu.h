#pragma once


//class J_SheZhiHeYueZu;

class J_XiaDanShuRuShunXuDlg :
	public wxDialog
{
public:
	enum
	{
		ID_Bt1,
		ID_Bt2,
		ID_Bt3,
		ID_Bt4,
	};


	void OnBt1(wxCommandEvent& event);
	void OnBt2(wxCommandEvent& event);
	void OnBt3(wxCommandEvent& event);
	void OnBt4(wxCommandEvent& event);

	void  Init();
	void  ReadCfg();
	void  WriteCfg();

	J_XiaDanShuRuShunXuDlg(wxWindow *parent);


public:

	//WXLRESULT MSWWindowProc(WXUINT   message,   WXWPARAM   wParam,   WXLPARAM   lParam); 
	wxExtListCtrl* List;


	wxButton*  Bt1;
	wxButton*  Bt2;

	wxButton*  Bt3;
	wxButton*  Bt4;

	DECLARE_EVENT_TABLE()

	//自选合约


};


