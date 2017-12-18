#include "StdAfx.h"
#include "CBaseInputPanel.h"
#include "wx\odcombo.h"


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
	m_fUpperLimitPrice=0.0;
	m_fLowerLimitPrice=0.0;
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
