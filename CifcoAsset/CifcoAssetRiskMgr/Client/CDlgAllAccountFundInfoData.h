#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include <set>
#include <map>
#include <queue>
#include <string>
#include "CDataInfo.h"
#include "UserApiStruct.h"
#include "CommonPkg.h"
#include "easymutex.h"
//using namespace std;
namespace Client {

	class CDlgAllAccountFundInfoData {
	public:
		static CDlgAllAccountFundInfoData* NewInstance() {
			return new CDlgAllAccountFundInfoData();
		}
		static void ReleaseInstance(CDlgAllAccountFundInfoData* instance) {
			if(instance!=NULL) 
				delete instance;
		}

	private:
		CDlgAllAccountFundInfoData(void) {
			nRowID = 0;
			bIsGroupQuery = false;
		}
		virtual ~CDlgAllAccountFundInfoData(void) {
		}

	public:
		// LockObject()与UnloadObject()这两个函数必须成对调用
		void LockObject() {
			oMutex.lock();
		}
		void UnlockObject() {
			oMutex.unlock();
		}
		int GetNextRowID() {
			return ++nRowID;
		}
		bool NewQuery();
		void ResetQuery();
		void CheckNewData(CDataInfo* pdataInfo, std::queue<RiskAllAccountField>& queueRet);
		void CheckNewData(CDataInfo* pdataInfo, std::queue<RiskSyncAccountField>& queueRet);

		std::set<int> setQueryAccountID;
		std::map<int, int> mapQueryReqID;
		std::set<int> setFilterOrganID;
		std::set<int> setFilterFinanProductID;

		std::queue<RiskAllAccountField> queueAllFundInfo;
		std::queue<RiskSyncAccountField> queueLastFundInfo;
		std::map<std::string, int> mapKey2Index;
		std::map<int, std::string> mapIndex2Key;
		std::map<std::string, SeqPreRiskAccountField> mapPreFundInfo;
		std::map<std::string, RiskSyncAccountField> mapLastFundInfo;
		bool bIsGroupQuery;
	
	private:
		Ceasymutex oMutex;
		int nRowID;
	};
}
