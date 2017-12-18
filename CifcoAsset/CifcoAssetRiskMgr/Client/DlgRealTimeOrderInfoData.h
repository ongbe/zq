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
#include "CDataInfo.h"
#include "UserApiStruct.h"
#include "CommonPkg.h"
#include "easymutex.h"

//using namespace std;
namespace Client {

	class DlgRealTimeOrderInfoData {
	public:
		static DlgRealTimeOrderInfoData* NewInstance() {
			return new DlgRealTimeOrderInfoData();
		}
		static void ReleaseInstance(DlgRealTimeOrderInfoData* instance) {
			if(instance!=NULL) 
				delete instance;
		}

	private:
		DlgRealTimeOrderInfoData(void) {
			nRowID = 0;
			bIsGroupQuery = false;
		}
		virtual ~DlgRealTimeOrderInfoData(void) {
		}

	public:
		int GetNextRowID() {
			return ++nRowID;
		};

		// LockObject()与UnloadObject()这两个函数必须成对调用
		void LockObject() {
			oMutex.lock();
		}
		void UnlockObject() {
			oMutex.unlock();
		}
		bool NewQuery();
		void ResetQuery();
		void CheckNewData(CDataInfo* pdataInfo, 
				std::queue<SequencialOrderField>& queueRet);

		std::set<int> setQueryAccountID;
		std::map<int, int> mapQueryReqID;
		std::set<int> setFilterOrganID;
		std::set<int> setFilterFinanProductID;
		std::set<string> setFilterInstrumentID;

		std::queue<SequencialOrderField> queueOrderInfo;
		std::map<string, int> mapKey2Index;
		std::map<int, string> mapIndex2Key;
		std::map<string, SequencialOrderField> mapOrderInfo;
		bool bIsGroupQuery;
	
	private:
		Ceasymutex oMutex;
		int nRowID;
	};
}
