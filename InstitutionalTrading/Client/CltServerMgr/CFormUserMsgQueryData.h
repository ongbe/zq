#pragma once

#include <set>
#include <map>
#include <queue>
#include <string>
#include "CommonDef.h"
#include "CommonStruct.h"
#include "CommonPkg.h"
#include "CommonErrorCode.h"
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

	class CFormUserMsgQueryData {
	public:
		static CFormUserMsgQueryData* NewInstance() {
			return new CFormUserMsgQueryData();
		}
		static void ReleaseInstance(CFormUserMsgQueryData* instance) {
			if(instance!=NULL) 
				delete instance;
		}

	private:
		CFormUserMsgQueryData(void) {
			nRowID = 0;
		}
		virtual ~CFormUserMsgQueryData(void) {
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
		bool NewQuery(int nUserID, std::string& strDate, bool bIsSend);
		void ResetQuery_Send();
		void ResetQuery_Recv();

		std::queue<UserMsgStru> queueLastSendMsg;
		std::queue<UserMsgStru> queueLastRecvMsg;

		std::map<int, int> mapKey2Index_Send;
		std::map<int, int> mapIndex2Key_Send;
		//std::map<std::string, UserMsgStru> mapLastMsg_Send;
		std::map<int, int> mapKey2Index_Recv;
		std::map<int, int> mapIndex2Key_Recv;
		//std::map<std::string, UserMsgStru> mapLastMsg_Recv;
		std::map<int, std::string> mapSendMsg_HasRecv;
		std::map<int, std::string> mapSendMsg_NotRecv;
	
	private:
		Ceasymutex oMutex;
		int nRowID;
	};
}
