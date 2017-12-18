/*查询可用交易所*/
#ifndef _QUERYEXCHANGE_H_
#define _QUERYEXCHANGE_H_
#include "stdafx.h"
#include <wx/textctrl.h>
#include <wx/combobox.h>
#include <vector>
#include "../inc/Module-Misc/ExtListCtrl.h"
#include "../inc/ISvr/PlatformSvrMgr.h"
//#include "../Module-Misc2/PlatformStruct.h"
class CQueryExchangeDlg:public wxDialog
{
public:
	CQueryExchangeDlg(wxWindow *parent);
	~CQueryExchangeDlg();
	wxString OnGetItemText (long item, long column) const;

private:	
	wxListCtrl *m_pWxExtListCtrl;

   
	std::vector<PlatformStru_Exchange> m_vectorExchange;


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
