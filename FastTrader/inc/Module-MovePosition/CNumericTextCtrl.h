#pragma once
#include "wx/textctrl.h"

static const char g_slide[] = {'0','1','2','3','4','5','6','7','8','9','0','-','\0'};
static const char g_assign[] = {'0','1','2','3','4','5','6','7','8','9','0','.','\0'};

class CNumericTextCtrl : public wxTextCtrl
{
public:
	CNumericTextCtrl();
	CNumericTextCtrl(wxWindow *parent,
					wxWindowID id, 
					const wxString &value=wxEmptyString, 
					const wxPoint &pos=wxDefaultPosition, 
					const wxSize &size=wxDefaultSize, 
					long style=0, 
					const wxValidator &validator=wxDefaultValidator, 
					const wxString &name=wxTextCtrlNameStr);
	~CNumericTextCtrl();

	//如果没有调用这个函数设置允许的字符集合，默认仅允许输入0~9的数字
	void SetFilter(const char * pChar)
	{
		m_pCharSet = pChar;
	}

protected:
	void OnChar(wxKeyEvent& event);

private:
	const char * m_pCharSet;

};