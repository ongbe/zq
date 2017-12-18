#pragma once

#include <wx/dialog.h>
#include <wx/textctrl.h>
#include <wx/bitmap.h>
#include <vector>
#include "VirtualKeyButton.h"


class VirtualKeyboard : public wxDialog
{
enum
{
	ID_CHAR_1=100,
	ID_CHAR_2,
	ID_CHAR_3,
	ID_CHAR_4,
	ID_CHAR_5,
	ID_CHAR_6,
	ID_CHAR_7,
	ID_CHAR_8,
	ID_CHAR_9,
	ID_CHAR_10,
	ID_CHAR_11,
	ID_CHAR_12,
	ID_CHAR_13,
	ID_CHAR_14,
	ID_CHAR_15,
	ID_CHAR_16,
	ID_CHAR_17,
	ID_CHAR_18,
	ID_CHAR_19,
	ID_CHAR_20,
	ID_CHAR_21,
	ID_CHAR_22,
	ID_CHAR_23,
	ID_CHAR_24,
	ID_CHAR_25,
	ID_CHAR_26,
	ID_CHAR_27,
	ID_CHAR_28,
	ID_CHAR_29,
	ID_CHAR_30,
	ID_CHAR_31,
	ID_CHAR_32,
	ID_CHAR_33,
	ID_CHAR_34,
	ID_CHAR_35,
	ID_CHAR_36,
	ID_CHAR_37,
	ID_CHAR_38,
	ID_CHAR_39,
	ID_CHAR_40,
	ID_CHAR_41,
	ID_CHAR_42,
	ID_CHAR_43,
	ID_CHAR_44,
	ID_CHAR_45,
	ID_CHAR_46,
	ID_CHAR_47,
	ID_CHAR_48,
	ID_CHAR_49,
	ID_CHAR_50,
	ID_CHAR_51,
	ID_CHAR_52,
	ID_CHAR_53,
	ID_CHAR_54,
	ID_CHAR_55,
	ID_CHAR_56,
	ID_CHAR_57,
	ID_CHAR_58,
	ID_CHAR_59,
	ID_CHAR_60,
	ID_CHAR_61,
	ID_CHAR_62,
	ID_CHAR_63,
	ID_CHAR_64,
	ID_CHAR_65,
	ID_CHAR_66,
	ID_CHAR_67,
	ID_CHAR_68,
	ID_BACKSPACE,
	ID_VIRTUAL_KEYBOARD_CLOSE,
	ID_BLANK,
	ID_CAPSLOCK

};
public:
	VirtualKeyboard(wxWindow *parent, wxTextCtrl* textctrl);
	virtual ~VirtualKeyboard(void);

	DECLARE_EVENT_TABLE()

protected:
	void OnVirtualButtonDown(wxCommandEvent& event);
	void OnBackspace(wxCommandEvent& event);
	void OnClose(wxCommandEvent& event);
	void OnBlank(wxCommandEvent& event);
	void OnCapsLock(wxCommandEvent& event);

	void OnMotion(wxMouseEvent& evt);
	void OnLeftDown(wxMouseEvent& evt);
	void OnLeftUp(wxMouseEvent& evt);

	void OnPaint(wxPaintEvent& event);
	void OnEraseBackground(wxEraseEvent& event);

private:
	void RandomList (wxChar * pList);
// 	void ChangeButtonBackground(int nID, int flag);

private:
	wxTextCtrl* m_pPasswordTextCtrl;
	wxWindow * m_pParent;
	wxPoint m_MouseMoveDelta;
 	bool m_bIsCapslockButtonDown;

	wxBitmap m_BackBitmap;

	std::vector<VirtualKeyButton*> m_vecVirtualKeyButton;
};
