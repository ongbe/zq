/*≤È—Ø∫œ‘º*/
#ifndef _QUERYCONTRACT_H_
#define _QUERYCONTRACT_H_
#include "stdafx.h"
#include <wx/textctrl.h>
#include <wx/combobox.h>
#include <vector>
#include "../inc/Module-Misc/ExtListCtrl.h"
#include "../inc/ISvr/PlatformSvrMgr.h"
//#include "../Module-Misc2/PlatformStruct.h"
class CQueryContractDlg:public wxDialog
{
public:
	CQueryContractDlg(wxWindow *parent);
	~CQueryContractDlg();
	wxString OnGetItemText (long item, long column) const;

private:	
	wxListCtrl *m_pWxExtListCtrl;

   
	std::vector<PlatformStru_Contract> m_vectorContract;


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
