/*≤È—Ø…Ã∆∑*/
#ifndef _QUERYCOMMODITY_H_
#define _QUERYCOMMODITY_H_
#include "stdafx.h"
#include <wx/textctrl.h>
#include <wx/combobox.h>
#include <vector>
#include "../inc/Module-Misc/ExtListCtrl.h"
#include "../inc/ISvr/PlatformSvrMgr.h"
#include "../inc/ISvr/PlatformStruct.h"
class CQueryCommodityDlg:public wxDialog
{
public:
	CQueryCommodityDlg(wxWindow *parent);
	~CQueryCommodityDlg();

	wxString OnGetItemText (long item, long column) const;
private:	
	wxListCtrl *m_pWxExtListCtrl;

   
	std::vector<PlatformStru_Commodity> m_vectorCommodity;


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
