#pragma once
//#include "wx/wxprec.h"
//#ifndef WX_PRECOMP
//    #include "wx/wx.h"
//#endif
//#include "wx/popupwin.h"
//#include "wx/spinctrl.h"
//#include "wx/listctrl.h"
//
//
////----------------------------------------------------------------------------
//// SimpleTransientPopup
////----------------------------------------------------------------------------
//class MouseHandyInputPane : public wxPopupTransientWindow
//{
//private:
//	enum { conMaxButtonNum = 30 };
//
//// IDs for the controls and the menu commands
//enum
//{
//    ID_PopupButton = 1,
//    ID_PopupList = ID_PopupButton+100
//};
//
//
////#define CODEHEADS wxT("cu,Í­,t,al,ÂÁ,l,zn,Ð¿,x,ru,Ïð½º,xj,fu,È¼ÓÍ,ry,au,»Æ½ð,hj,rb,ÂÝÎÆ¸Ö,lwg,wr,Ïß²Ä,xc,IF,¹ÉÖ¸,gz,a,¶¹Ò»,dy,b,¶¹¶þ,de,m,¶¹ÆÉ,dp,y,¶¹ÓÍ,dy,c,»ÆÓñÃ×,hym,l,¾ÛÒÒÏ©,jyx,p,×Øéµ,zl,v,¾ÛÂÈÒÒÏ©,jlyx,SR,°×ÌÇ,bt,CF,ÃÞ»¨,mh,GN,ÂÌ¶¹,ld,RO,²ËÓÍ,cy,TA,PTA,pta,WS,Ç¿Âó,qm,WT,Ó²Âó,ym,ER,ÔçôÌµ¾,zxd")
//
//typedef struct {
//	char strCodeHead[8];	// ºÏÔ¼´úÂëÍ·
//	char strCodeName[16];	// ºÏÔ¼¼ò³Æ
//	char strCodeNameJP[8];	// ºÏÔ¼¼ò³ÆµÄÆ´ÒôÊ××ÖÄ¸
//} CODEHEADDATA, *PCODEHEADDATA;
//
//
//public:
//    MouseHandyInputPane( wxWindow *parent );
//    virtual ~MouseHandyInputPane();
//
//	void ShowKeyInput(wxString& strKeyInput) {
//		m_bKeyInput = TRUE;
//		m_strKeyInput = strKeyInput;
//	};
//
//    // wxPopupTransientWindow virtual methods are all overridden to log them
//    virtual void Popup(wxWindow *focus = NULL);
//    virtual void OnDismiss();
//    virtual bool ProcessLeftDown(wxMouseEvent& event);
//    virtual bool Show( bool show = true );
//    wxScrolledWindow* GetChild() { return m_panel; }
//
//private:
//    wxScrolledWindow *m_panel;
//    wxButton*		m_button[conMaxButtonNum];
//	wxListCtrl*		m_listBox;
//	BOOL			m_bKeyInput;
//	wxString		m_strKeyInput;
//	int				m_nCodeHeadsCount;
//	CODEHEADDATA*	m_ptCodeHeads;
//
//private:
//	void OnMouse(wxMouseEvent &event);
//    void OnSize( wxSizeEvent &event );
//    void OnSetFocus( wxFocusEvent &event );
//    void OnKillFocus( wxFocusEvent &event );
//    void OnButton( wxCommandEvent& event );
//    void OnListBoxSelected(wxListEvent& event);
//
//private:
//    DECLARE_CLASS(MouseHandyInputPane)
//    DECLARE_EVENT_TABLE()
//};
