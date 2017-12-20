#include "StdAfx.h"
#include "CBaseInputPanel.h"


BEGIN_EVENT_TABLE(CBaseInputPanel, wxPanel)
END_EVENT_TABLE()
//////////////////////////////////////////////////////////////////////////////////////


CBaseInputPanel::CBaseInputPanel(wxWindow *parent,
			wxWindowID winid,
			const wxPoint& pos,
			const wxSize& size,
			long style,
			const wxString& name):
	wxPanel(parent,winid,pos,size,style,name)
{
}

CBaseInputPanel::~CBaseInputPanel()
{
}

int CBaseInputPanel::ConvertOwnerDrawnComboBoxID(int nID, wxWindow* pPanel)
{
	wxWindow* pWin = FindWindowById(nID, pPanel);
	if(pWin==NULL) return nID;
	pWin = pWin->GetParent();
	if(pWin==NULL) return nID;
	if(pWin->IsKindOf(CLASSINFO(wxOwnerDrawnComboBox)))
		nID = pWin->GetId();
	return nID;
}

int CBaseInputPanel::ConvertOwnerDrawnComboBoxID(int nID)
{
	return ConvertOwnerDrawnComboBoxID(nID, this);
}
