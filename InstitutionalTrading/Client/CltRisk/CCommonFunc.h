#pragma once

namespace CltRisk {

	void ResetComboBoxTextReadOnly(System::Windows::Forms::ComboBox^ comboRelate);
	unsigned long GetTickCount();
	int GetLocalDate();
	
	#define MACRO2STRING(p) MACRO2STRING_TEXT(#p)
	#define MACRO2STRING_TEXT(p) p
}
