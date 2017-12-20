#pragma once

#include <map>
#include <string>
#include "CommonStruct.h"



namespace CltPresentation {
	class CFinancialProductIDListDlgData {
	public:
		class CData {
		public:
			CData() {
				nID = -1;
				bSelect = false;
				memset(&data, 0, sizeof(data));
			}
			~CData() {
			}
			int				nID;
			bool			bSelect;
			FinancialProduct	data;
		};

	public:
		static CFinancialProductIDListDlgData* NewInstance() {
			return new CFinancialProductIDListDlgData();
		}
		static void ReleaseInstance(CFinancialProductIDListDlgData* instance) {
			if(instance!=NULL) 
				delete instance;
		}

	private:
		CFinancialProductIDListDlgData() {
		}
		~CFinancialProductIDListDlgData() {
		}

	public:
		std::map<int, CData> mapFinaProduct;
	};
}
