#pragma once

using namespace System;
using namespace System::Data;



namespace ReportDataProvider {

	template <typename _T>
	public ref class Singleton
	{
	protected:
		static _T^ mrInstance;		
	public:
		Singleton()
		{
			mrInstance = nullptr;
		}
		static _T ^ Instance()
		{
            if(mrInstance == nullptr)
			{
				mrInstance =  gcnew _T();
			}
			return mrInstance;
			
		}
	};
}