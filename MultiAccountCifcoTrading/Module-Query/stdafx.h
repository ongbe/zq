// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"
// wxWidgets
#include <wx/wxprec.h>
#ifndef WX_PRECOMP    
    #include <wx/wx.h>
#endif
//#ifdef _DEBUG
//#define WXLOG_INFO wxLogDebug
//#else
//#define WXLOG_INFO wxLogTrace
//#endif
#include "../inc/Module-Misc/globalDefines.h"

// TODO: reference additional headers your program requires here
#include "../inc/Module-Misc/constants.h"
#include "../inc/Module-Misc/EventSet.h"
#include "../inc/Module-Misc/GlobalFunc.h"
#include "../inc/ISvr/PlatformSvrMgr.h"



#include "wx/odcombo.h"
#include "../inc/Module-Misc/ExtListCtrl.h"
#include "../inc/Module-Misc/FieldValueTool.h"
#include <vector>
#include <string>
#include "wx/statline.h"
#include <map>
#include "cfgMgr/CfgMgr.h"
#include <wx/listctrl.h>
#include "wx/file.h"

using namespace std;
#define AUTO_SETSIZE()	SetSize(GetSize().x,GetSize().y+wxSystemSettings::GetMetric(wxSYS_CAPTION_Y)-26)
#define INSERT_QUERYLIST_HEADER(classname,max_col,p)\
do{\
    std::string prefix=#p;\
    m_pWxExtListCtrl=new VirtualList<classname>(this, ID_LISTCTRL,wxDefaultPosition ,GetClientSize(),wxLC_REPORT|wxLC_VRULES|wxLC_VIRTUAL);\
    wxListItem col;\
    col.SetWidth(100);\
	for(int i=0; i<max_col;++i)\
	{  \
		col.SetAlign(i==0?wxLIST_FORMAT_LEFT:wxLIST_FORMAT_RIGHT);\
        col.SetText(LOADSTRING_TEXT(prefix+wxString::Format("%d",i+1).c_str()));\
        m_pWxExtListCtrl->InsertColumn(i,col);\
    }\
}while(0)
template<class T>
class VirtualList:public wxListCtrl
{
public:
	VirtualList(T *parent, wxWindowID id, 
		const wxPoint &pos=wxDefaultPosition, 
		const wxSize &size=wxDefaultSize, 
		long style=wxLC_ICON, 
		const wxValidator &validator=wxDefaultValidator, 
		const wxString &name=wxListCtrlNameStr):
	wxListCtrl(parent,id,pos,size,style,validator,name),
		m_parent(parent)
	{
	}
	wxString OnGetItemText (long item, long column) const
	{
		if(m_parent) 
			return m_parent->OnGetItemText(item,column);
		return  wxEmptyString;
	}
private:
	T *m_parent;
};


// TODO: reference additional headers your program requires here
