/*查询报单类型*/
#ifndef _QUERYORDERTYPE_H_
#define _QUERYORDERTYPE_H_
#include "stdafx.h"
#include <wx/textctrl.h>
#include <wx/combobox.h>
#include <vector>
#include "../inc/Module-Misc/ExtListCtrl.h"
#include "../inc/ISvr/PlatformSvrMgr.h"
#include "../inc/ISvr/PlatformStruct.h"
class CQueryOrderTypeDlg:public wxDialog
{
public:
	CQueryOrderTypeDlg(wxWindow *parent);
	~CQueryOrderTypeDlg();
	wxString OnGetItemText (long item, long column) const;

private:	
	wxListCtrl *m_pWxExtListCtrl;

   
	std::vector<PlatformStru_OrderType> m_vectorOrderType;


	enum
		{			
            ID_LISTCTRL,

			////GUI Enum Control ID End
			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};

private:
	void CreateGUIControls();

};

#endif
