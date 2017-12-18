#include "stdafx.h"
#include "cfgMgr/CfgMgr.h"
#include "J_XiaDanShuRuShunXu.h"
#include "../inc/Module-Misc2/SimpleWriteLog.h"

BEGIN_EVENT_TABLE(J_XiaDanShuRuShunXuDlg,wxDialog)

	EVT_BUTTON(ID_Bt1,OnBt1)
	EVT_BUTTON(ID_Bt2,OnBt2)
	EVT_BUTTON(ID_Bt3,OnBt3)
	EVT_BUTTON(ID_Bt4,OnBt4)

END_EVENT_TABLE()


J_XiaDanShuRuShunXuDlg::J_XiaDanShuRuShunXuDlg(wxWindow *parent):
wxDialog(parent, wxID_ANY,_T(""),wxDefaultPosition, wxSize(800,800),wxNO_3D|wxDEFAULT_DIALOG_STYLE|wxMINIMIZE_BOX|wxDIALOG_MODELESS )

{	
	DIALOG_CREATE_LOG();
	Bt2 = new wxBitmapButton(this, ID_Bt2, wxBitmap("zqBITMAP_DOWN"), wxPoint(124, 71));

	Bt4 = new wxButton(this, ID_Bt4, LOADSTRING(ASKNAMEDLG_CANCEL), wxPoint(89, 126), wxSize(75, 25), 0, wxDefaultValidator, wxT("WxButton4"));

	Bt3 = new wxButton(this, ID_Bt3, LOADSTRING(ASKNAMEDLG_OK), wxPoint(9, 125), wxSize(75, 25), 0, wxDefaultValidator, wxT("WxButton3"));

	List = new wxExtListCtrl(this, wxID_ANY, wxPoint(12, 11), wxSize(108, 109), wxLCEXT_REPORT, wxDefaultValidator, wxT("WxListCtrl1"));

	Bt1 = new wxBitmapButton(this, ID_Bt1, wxBitmap("zqBITMAP_UP"), wxPoint(123, 12));


	SetTitle(LOADSTRING(CONFIG_INPUT_ORDER));
	SetIcon(wxNullIcon);
	SetSize(8,8,177,187);
	Center();
	ReadCfg();

	//Init();

	//m_Dlg = NULL;
}



void J_XiaDanShuRuShunXuDlg::OnBt1(wxCommandEvent& event)
{
	int count = List->GetSelectedItemCount();
	int j = 0;
	if( count == 0 )
		return;

	int row;
	int rownum = List->GetItemCount();
	wxExtListItem item;
	item.SetMask(wxLISTEXT_MASK_TEXT|wxLISTEXT_MASK_STATE|wxLISTEXT_SET_ITEM);

	for(int i=0; i< rownum; i++)
	{

		if( j >count )
			break;

		if( List->IsSelectedItem( i ) )
		{
			row = i;
			if(row == 0) {
				j++;
				continue;
			}

			item.SetId(row);
			item.SetColumn(0);

			List->GetItem( item );

			List->DeleteItem( item );

			List->InsertItem( row-1,item.GetText() );

			item.SetId( row-1 );

			List->SetItemState( row-1,wxLISTEXT_STATE_FOCUSED|wxLISTEXT_STATE_SELECTED,wxLISTEXT_STATE_SELECTED);

		}

		if(j == count)
			break;
	}
}

void J_XiaDanShuRuShunXuDlg::OnBt2(wxCommandEvent& event)
{
	int count = List->GetSelectedItemCount();
	if( count == 0 )
		return;

	int row = 0;
	int rownum = List->GetItemCount();
	int j = 0;

	wxExtListItem item;
	item.SetMask(wxLISTEXT_MASK_TEXT);

	for(int i = rownum-1; i >= 0; i--)
	{

		if( List->IsSelectedItem( i ) )
		{
			row = i;
			if(row == rownum -1) {
				j++;
				continue;
			}

			item.SetId(row);
			item.SetColumn(0);

			List->GetItem( item );

			List->DeleteItem( item );
			List->InsertItem( row+1,item.GetText() );

			List->SetItemState( row+1,wxLISTEXT_STATE_FOCUSED|wxLISTEXT_STATE_SELECTED,wxLISTEXT_STATE_SELECTED);
		}
	}
}

void J_XiaDanShuRuShunXuDlg::OnBt3(wxCommandEvent& event)
{
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return ;

	LPORDER_CFG  p = pMgr->GetOrderCfg();
	string str;
	int count = List->GetItemCount();
	for(int i=0;i<count;i++)
	{
		str = List->GetSubItemText(i,0);
		strncpy(p->Name[i], str.c_str(), sizeof(p->Name[i])-1);
	}

	//pMgr->Save();
	this->EndDialog(1);
}

void J_XiaDanShuRuShunXuDlg::OnBt4(wxCommandEvent& event)
{
	this->EndDialog(1);
}


void J_XiaDanShuRuShunXuDlg::Init()
{


}

void  J_XiaDanShuRuShunXuDlg::ReadCfg()
{
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return ;

	LPORDER_CFG  p = pMgr->GetOrderCfg();

	List->InsertColumn(0, LOADSTRING(CONFIG_ITEM_NAME));
	List->InsertItem(0, p->Name[0] );
	List->InsertItem(1, p->Name[1] );
	List->InsertItem(2, p->Name[2] );
	List->InsertItem(3, p->Name[3] );
	List->InsertItem(4, p->Name[4] );
}

void  J_XiaDanShuRuShunXuDlg::WriteCfg()
{

}