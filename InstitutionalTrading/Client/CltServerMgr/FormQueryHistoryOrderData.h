#pragma once

#include <set>
#include <map>
#include <queue>
#include <string>
#include "CommonDef.h"
#include "CommonPkg.h"
#include "CommonErrorCode.h"
#include "CDataInfo.h"
#include "Tools.h"
#include "easymutex.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

//using namespace std;
namespace CltServerMgr {

	class FormQueryHistoryOrderData {
	public:
		static FormQueryHistoryOrderData* NewInstance() {
			return new FormQueryHistoryOrderData();
		}
		static void ReleaseInstance(FormQueryHistoryOrderData* instance) {
			if(instance!=NULL) 
				delete instance;
		}

	private:
		FormQueryHistoryOrderData(void) {
			//nRowID = 0;
		}
		virtual ~FormQueryHistoryOrderData(void) {
		}

	public:
		// LockObject()与UnloadObject()这两个函数必须成对调用
		void LockObject() {
			oMutex.lock();
		}
		void UnlockObject() {
			oMutex.unlock();
		}
		//int GetNextRowID() {
		//	return ++nRowID;
		//}
		bool NewQuery(std::string& strStartDate, std::string& strEndDate, int nUserID, int& nRetCount);
		bool NextQuery();
		bool StopQuery();
		void ResetQuery();
		//void CheckNewData(CDataInfo* pdataInfo, std::queue<PlatformStru_TradingAccountInfo>& queueRet);

		std::set<std::string> setQueryAccountID;
		//std::map<int, int> mapQueryReqID;

		std::queue<PlatformStru_OrderInfo> queueOrderInfo;
		//std::map<std::string, int> mapKey2Index;
		//std::map<int, std::string> mapIndex2Key;
		//std::map<std::string, PlatformStru_OrderInfo> mapOrderInfo;
	
	private:
		Ceasymutex	oMutex;
		std::queue<std::string> queueDate;
		int			nUserID;
		//int nRowID;
	};
}
