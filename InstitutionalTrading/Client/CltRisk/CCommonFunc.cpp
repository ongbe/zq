#include "StdAfx.h"
#include "CCommonFunc.h"

#include <windows.h>

void CltRisk::ResetComboBoxTextReadOnly(System::Windows::Forms::ComboBox^ comboRelate) 
{
	HWND comboHandle = (HWND)((__int64)comboRelate->Handle);
	HWND editHandle = GetWindow(comboHandle, GW_CHILD);
	SendMessage(editHandle, EM_SETREADONLY, 1, 0);
}

unsigned long CltRisk::GetTickCount()
{
	return ::GetTickCount();
}

int CltRisk::GetLocalDate()
{
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	return sysTime.wYear*10000+sysTime.wMonth*100+sysTime.wDay;
}
