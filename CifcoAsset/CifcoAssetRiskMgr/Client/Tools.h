#pragma once

#include <string>
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;



public ref class Tools
{
public:
	static std::string String2string(String^ mstr)
	{
		//String^ istrt=textBox1->Text;
		const char* pchars = (const char*)(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(mstr)).ToPointer();
		std::string rltstr(pchars);
		Runtime::InteropServices::Marshal::FreeHGlobal(IntPtr((void*)pchars));
		return rltstr;
	}
	static String^ string2String(const char* pchars)
	{
		String^ tmp= gcnew String(pchars);
		return tmp;
	}
};
