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

	class CDlgRealTimePositionInfoData {
	public:
		static CDlgRealTimePositionInfoData* NewInstance() {
			return new CDlgRealTimePositionInfoData();
		}
		static void ReleaseInstance(CDlgRealTimePositionInfoData* instance) {
			if(instance!=NULL) 
				delete instance;
		}

	private:
		CDlgRealTimePositionInfoData(void) {
			nRowID = 0;
			bIsGroupQuery = false;
		}
		virtual ~CDlgRealTimePositionInfoData(void) {
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
		void CheckNewData(CDataInfo* pdataInfo, std::queue<SequencialPositionField>& queueRet);
		void CheckNewData_Quot(CDataInfo* pdataInfo, std::queue<SequencialPositionField>& queueRet);
		void RecalcPosition(CDataInfo* pdataInfo, DepthMarketDataField& quot, 
										std::queue<SequencialPositionField>& queueRet);

		std::set<int> setQueryAccountID;
		std::map<int, int> mapQueryReqID;
		std::set<int> setFilterOrganID;
		std::set<int> setFilterFinanProductID;
		std::set<std::string> setFilterInstrumentID;

		std::queue<SequencialPositionField> queuePositionInfo;
		std::map<std::string, int> mapKey2Index;
		std::map<int, std::string> mapIndex2Key;
		std::map<std::string, SequencialPositionField> mapPositionInfo;
		bool bIsGroupQuery;
		bool bShowZeroPosition;

		std::set<std::string> setHasQuotInstrumentID;
		std::queue<DepthMarketDataField> queueQuot;
		std::map<std::string, DepthMarketDataField> mapQuot;
	
	private:
		Ceasymutex oMutex;
		int nRowID;
	};
}
