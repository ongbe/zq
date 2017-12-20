#pragma once

#include <Windows.h>
#include "wx\msw\dialog.h"
#include <vector>
#include <wx/listctrl.h>
#include "../inc/Module-Misc/ExtListCtrl.h"
#include "../inc/PlusinCommon/CommonDef.h"
#include "Plusin.h"

class CPlusin;

class PlusInMgrDlg :
	public wxDialog
{
	enum
	{
		ID_LIST,
		ID_OPEN,
		ID_DEL,
		ID_START,
	};
public:
	PlusInMgrDlg(wxWindow *parent, std::map<wxString, PLUGWIN2>& mapPlusIn, CPlusin& rPlusin);
	~PlusInMgrDlg(void);
	//响应添加按钮
	void OnAdd( wxCommandEvent& event );
	//响应删除按钮
	void OnDel( wxCommandEvent& event );
	//响应关闭按钮
	void OnClose( wxCommandEvent& event );
	//列表项激活
	void OnItemSelected(wxExtListEvent& event);
public:
	//向列表添加一个插件项，可能出现多线程操作
	void UpdatePlusIn( const wxString& strAppID, const PLUGWIN2& plusin );
	//向列表删除一个插件项，可能出现多线程操作
	void DelPlusIn( const wxString& strAppID );

	DECLARE_EVENT_TABLE()

private:
	wxExtListCtrl* m_pListCtrl;
	CPlusin& m_rPlusin;
	wxStaticText* m_pStaticText;
};
