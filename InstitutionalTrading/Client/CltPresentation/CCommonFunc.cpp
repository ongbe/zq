#include "StdAfx.h"
#include "CCommonFunc.h"

#include <windows.h>

void CltPresentation::ResetComboBoxTextReadOnly(System::Windows::Forms::ComboBox^ comboRelate) 
{
	HWND comboHandle = (HWND)((__int64)comboRelate->Handle);
	HWND editHandle = GetWindow(comboHandle, GW_CHILD);
	SendMessage(editHandle, EM_SETREADONLY, 1, 0);
}

unsigned long CltPresentation::GetTickCount()
{
	return ::GetTickCount();
}
