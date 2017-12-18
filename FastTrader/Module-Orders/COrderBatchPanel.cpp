#include "StdAfx.h"
#include "COrderBatchPanel.h"
#include "COrderInsertView.h"



BEGIN_EVENT_TABLE(COrderBatchPanel, wxPanel)
	EVT_RADIOBUTTON(ID_OBATCH_RADIOINPUTPRICE,OnRadioButton)
	EVT_RADIOBUTTON(ID_OBATCH_RADIONEWPRICE,OnRadioButton)
	EVT_RADIOBUTTON(ID_OBATCH_RADIOBSPRICE,OnRadioButton)
    EVT_PANEL_CHAR_HOOK(OnPanelCharHook)
END_EVENT_TABLE()
//////////////////////////////////////////////////////////////////////////////////////


COrderBatchPanel::COrderBatchPanel(wxWindow *parent,
            wxWindowID winid ,
            const wxPoint& pos ,
            const wxSize& size ,
            long style,
            const wxString& name ):
wxPanel(parent,winid,pos,size,style,name)
{

	wxPoint posInit(5, 5);
	int nFW = 8, nFH = 16;
	int nVW = nFW+2, nVH = nFH+6;
	wxPoint ctlPos(0, 0);
	wxSize ctlSz(0, 0);

	ctlPos = wxPoint(posInit.x, posInit.y+4);
	ctlSz.Set(nFW*7, nFH);
	wxRadioButton* itemRadioBox1 = new wxRadioButton( this, ID_OBATCH_RADIOINPUTPRICE, wxEmptyString, ctlPos, ctlSz);//输入价//, wxRB_GROUP);
	itemRadioBox1->SetValue(true);
	ctlPos = wxPoint(posInit.x+nFW*7+nFW/2, posInit.y+4);
	ctlSz.Set(nFW*7, nFH);
	wxRadioButton* itemRadioBox2 = new wxRadioButton( this, ID_OBATCH_RADIONEWPRICE, wxEmptyString, ctlPos, ctlSz);//最新//, wxRB_GROUP);
	ctlPos = wxPoint(posInit.x+nFW*7+nFW*7+nFW/2, posInit.y+4);
	ctlSz.Set(nFW*7, nFH);
	wxRadioButton* itemRadioBox3 = new wxRadioButton( this, ID_OBATCH_RADIOBSPRICE, wxEmptyString, ctlPos, ctlSz);//买卖//, wxRB_GROUP);

	ctlPos = wxPoint(posInit.x, posInit.y+nVH+2);
	ctlSz.Set(nFW*10, nFH);
	wxStaticText* itemStaticText1 = new wxStaticText( this, ID_OBATCH_STATICE1, wxEmptyString, ctlPos, ctlSz, wxALIGN_RIGHT);//每批手数
	ctlPos = wxPoint(posInit.x, posInit.y+nVH*2+2);
	ctlSz.Set(nFW*10, nFH);
	wxStaticText* itemStaticText2 = new wxStaticText( this, ID_OBATCH_STATICE2, wxEmptyString, ctlPos, ctlSz, wxALIGN_RIGHT);//批次间隔[秒]
	
	ctlPos = wxPoint(posInit.x+nFW*10+2, posInit.y+nVH);
	ctlSz.Set(nFW*10, 20);
	wxSpinCtrl* itemSpinCtrl1 = new wxSpinCtrl( this, ID_OBATCH_SPINCTRLVOLUME, wxEmptyString, ctlPos, ctlSz);
	ctlPos = wxPoint(posInit.x+nFW*10+2, posInit.y+nVH*2);
	ctlSz.Set(nFW*10, 20);
	wxSpinCtrl* itemSpinCtrl2 = new wxSpinCtrl( this, ID_OBATCH_SPINCTRLTIMER, wxEmptyString, ctlPos, ctlSz);

	m_TabOrderVector.clear();
	//tab order
	m_TabOrderVector.push_back(ID_OBATCH_RADIOINPUTPRICE);
	m_TabOrderVector.push_back(ID_OBATCH_SPINCTRLVOLUME);
	m_TabOrderVector.push_back(ID_OBATCH_SPINCTRLTIMER);

	ResetLanguage();
}

COrderBatchPanel::~COrderBatchPanel()
{
}
COrderBatchPanel* COrderBatchPanel::CreatePane(wxWindow* parent)
{
    return  new COrderBatchPanel(parent, GID_ORDER_INSERT_PANEL);
}
void COrderBatchPanel::OnPanelCharHook(wxCommandEvent& evt)
{
    wxKeyEvent* pEvent=(wxKeyEvent*)evt.GetClientData();
    if(!pEvent) return;
    wxWindow *win = FindFocus();
    if(win == NULL) {
        evt.Skip();
        return;
    }
    if(pEvent->GetKeyCode() == WXK_RETURN)
    {
        //int id,thisID=win->GetId(),count=m_TabOrderVector.size();
        //for(id=0;id<count;id++)
        //{
        //    if(m_TabOrderVector[id]==thisID)
        //    {
        //        if(id<count-1)
        //        {
        //            FindWindowById(m_TabOrderVector[id+1],this)->SetFocus();
        //            return;
        //        }
        //    }
        //}
    }
    else if(pEvent->GetKeyCode() == WXK_LEFT)
    {
		//wxSpinEvent evtSpin;
		//wxComboBox* itemCombo = NULL;
		//int thisID=win->GetId();
		//switch(thisID) {
		//case ID_OBATCH_SPINCTRLVOLUME:
		//	if(sm_nViewStyle == 3) {	// 如果是金仕达风格
		//		itemCombo = (wxComboBox*)FindWindowById(ID_OBATCH_SPINCTRLVOLUME, this);
		//		if(itemCombo != NULL) {
		//			int CurSel=0;
		//			if((CurSel=itemCombo->GetSelection())>0)
		//				itemCombo->SetSelection(itemCombo->GetSelection()-1);
		//			else
		//			{
		//				wxString strValue=itemCombo->GetValue();
		//				for(CurSel=0;CurSel<(int)itemCombo->GetCount();CurSel++)
		//				{
		//					if(itemCombo->GetString(CurSel).Find(strValue)==0) {
		//						break;
		//					}
		//				}
		//				if(CurSel<(int)itemCombo->GetCount())
		//					itemCombo->SetSelection(CurSel);
		//				else 
		//					itemCombo->SetSelection(0);
		//			}
		//			SetPanelItemText(ID_CMBLONGCODE, itemCombo->GetValue());
		//			m_DynamicItem.IsValid=true;
		//			m_NeedUpdatePrice=true;
		//			FindWindowById(ID_CTLWDSHOW,this)->Refresh();
		//		}
		//	}
		//	break;
		//case ID_OBATCH_SPINCTRLTIMER:
		//	evtSpin.SetId(ID_SPINCTRL_VOLUME_BUTTON);
		//	OnPriceSpinDown(evtSpin);
		//	break;
		//case ID_SPINCTRL_VOLUME_TEXT:
		//	evtSpin.SetId(ID_SPINCTRL_BUTTON);
		//	OnVolumeSpinDown(evtSpin);
		//	break;
		//};
    }
    else if(pEvent->GetKeyCode() == WXK_RIGHT)
    {
		//wxSpinEvent evtSpin;
		//wxComboBox* itemCombo = NULL;
		//int thisID=win->GetId();
		//switch(thisID) {
		//case ID_CONTRANCTNAME:
		//	if(sm_nViewStyle == 3) {	// 如果是金仕达风格
		//		itemCombo = (wxComboBox*)FindWindowById(ID_CONTRANCTNAME, this);
		//		if(itemCombo != NULL) {
		//			int CurSel=0;
		//			if((CurSel=itemCombo->GetSelection())<(int)itemCombo->GetCount())
		//				itemCombo->SetSelection(itemCombo->GetSelection()+1);
		//			else
		//			{
		//				wxString strValue=itemCombo->GetValue();
		//				for(CurSel=0;CurSel<(int)itemCombo->GetCount();CurSel++)
		//				{
		//					if(itemCombo->GetString(CurSel).Find(strValue)==0) {
		//						break;
		//					}
		//				}
		//				if(CurSel<(int)itemCombo->GetCount())
		//					itemCombo->SetSelection(CurSel);
		//				else 
		//					itemCombo->SetSelection(0);
		//			}
		//			SetPanelItemText(ID_CMBLONGCODE, itemCombo->GetValue());
		//			m_DynamicItem.IsValid=true;
		//			m_NeedUpdatePrice=true;
		//			FindWindowById(ID_CTLWDSHOW,this)->Refresh();
		//		}
		//	}
		//	break;
		//case ID_SPINCTRL_TEXT:
		//	evtSpin.SetId(ID_SPINCTRL_VOLUME_BUTTON);
		//	OnPriceSpinUp(evtSpin);
		//	break;
		//case ID_SPINCTRL_VOLUME_TEXT:
		//	evtSpin.SetId(ID_SPINCTRL_BUTTON);
		//	OnVolumeSpinUp(evtSpin);
		//	break;
		//};
    }
    else if(pEvent->GetKeyCode() == WXK_UP)
    {
        int id,thisID=win->GetId(),count=m_TabOrderVector.size();
        for(id=0;id<count;id++)
        {
            if(m_TabOrderVector[id]==thisID)
            {
                if(id>0)
                {
                    FindWindowById(m_TabOrderVector[id-1],this)->SetFocus();
                    return;
                }
            }
        }
    }
    else if(pEvent->GetKeyCode() == WXK_DOWN)
    {
        int id,thisID=win->GetId(),count=m_TabOrderVector.size();
        for(id=0;id<count;id++)
        {
            if(m_TabOrderVector[id]==thisID)
            {
                if(id<count-1)
                {
                    FindWindowById(m_TabOrderVector[id+1],this)->SetFocus();
                    return;
                }
            }
        }
    }
    //evt.Skip();
}

void COrderBatchPanel::ResetLanguage()
{
	wxWindow* pWin = NULL;
	pWin = FindWindowById(ID_OBATCH_RADIOINPUTPRICE, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_INPUTPRICE));
	pWin = FindWindowById(ID_OBATCH_RADIONEWPRICE, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_LASTPRICE2));
	pWin = FindWindowById(ID_OBATCH_RADIOBSPRICE, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_B_S_PRICE2));
	pWin = FindWindowById(ID_OBATCH_STATICE1, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_QTY_EACH));
	pWin = FindWindowById(ID_OBATCH_STATICE2, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_INTERVAL_SEC));
}

// 仅此用于日志文件输出
wxString COrderBatchPanel::GetUserInputText()
{
	wxString strRet;

	strRet = LOADSTRING(OBP_INPUT_BEGIN);
	{
		wxRadioButton* window=NULL;
		window=(wxRadioButton*)FindWindowById(ID_OBATCH_RADIOINPUTPRICE,this);
		if(window->GetValue()) {
			strRet+=LOADSTRING(OBP_INPUT_MIDDLE);
			strRet+=window->GetLabel();
			strRet+=LOADSTRING(OBP_INPUT_END);
		}
		window=(wxRadioButton*)FindWindowById(ID_OBATCH_RADIONEWPRICE,this);
		if(window->GetValue()) {
			strRet+=LOADSTRING(OBP_INPUT_MIDDLE);
			strRet+=window->GetLabel();
			strRet+=LOADSTRING(OBP_INPUT_END);
		}
		window=(wxRadioButton*)FindWindowById(ID_OBATCH_RADIOBSPRICE,this);
		if(window->GetValue()) {
			strRet+=LOADSTRING(OBP_INPUT_MIDDLE);
			strRet+=window->GetLabel();
			strRet+=LOADSTRING(OBP_INPUT_END);
		}
	}

	{
		wxSpinCtrl* window=NULL;
		window=(wxSpinCtrl*)FindWindowById(ID_OBATCH_SPINCTRLVOLUME,this);
		if(window) { 
			strRet+=LOADSTRING(OBP_INPUT_MIDDLE1);
			strRet+=Number2String(window->GetValue());
			strRet+=LOADSTRING(OBP_INPUT_END);
		}
	}
	
	{
		wxSpinCtrl* window=NULL;
		window=(wxSpinCtrl*)FindWindowById(ID_OBATCH_SPINCTRLTIMER,this);
		if(window) { 
			strRet+=LOADSTRING(OBP_INPUT_MIDDLE2);
			strRet+=Number2String(window->GetValue());
			strRet+=LOADSTRING(OBP_INPUT_END);
		}
	}
	strRet+="\n";
	return strRet;

}

void  COrderBatchPanel::OnRadioButton(wxCommandEvent& event)
{
     int id=event.GetId();
     bool bEnabled;

     if(id==ID_OBATCH_RADIOINPUTPRICE) bEnabled=true;
     else bEnabled=false;

     ((COrderInsertView*)(this->GetParent()))->EnablePriceInputCtl(bEnabled);
}
