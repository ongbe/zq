/*查询可用账户*/
#ifndef _QUERYTRADER_H_
#define _QUERYTRADER_H_
#include "stdafx.h"
#include <wx/textctrl.h>
#include <wx/combobox.h>
#include <vector>
#include "../inc/Module-Misc/ExtListCtrl.h"
#include "../inc/ISvr/PlatformSvrMgr.h"
#include "../inc/ISvr/PlatformStruct.h"
class CQueryTraderDlg:public wxDialog
{
public:
	CQueryTraderDlg(wxWindow *parent);
	~CQueryTraderDlg();
	wxString OnGetItemText (long item, long column) const;

private:	
	wxListCtrl *m_pWxExtListCtrl;

   
	std::vector<PlatformStru_Trader> m_vectorTrader;


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

