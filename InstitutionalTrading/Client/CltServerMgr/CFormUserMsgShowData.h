#pragma once

#include <vector>
#include "CommonDef.h"
#include "CommonStruct.h"
#include "CommonPkg.h"
#include "CommonErrorCode.h"

namespace CltServerMgr {

	class CFormUserMsgShowData {
	public:
		static CFormUserMsgShowData* NewInstance() {
			return new CFormUserMsgShowData();
		}
		static void ReleaseInstance(CFormUserMsgShowData* instance) {
			if(instance!=NULL) 
				delete instance;
		}

	private:
		CFormUserMsgShowData(void) {
			nRowIndex = 0;
		}
		virtual ~CFormUserMsgShowData(void) {
		}

	public:

		void AppendUserMsg(UserMsgStru& msg) {
			vecUserMsg.push_back(msg);
		};

		bool GetPrevUserMsg(UserMsgStru& msg) {
			if(nRowIndex<=0) {
				nRowIndex = 0;
				return false;
			}
			else {
				nRowIndex--;
				msg = vecUserMsg[nRowIndex];
				return true;
			}
		};

		bool GetNextUserMsg(UserMsgStru& msg) {
			if(nRowIndex>=GetMsgCount()-1) {
				nRowIndex = GetMsgCount()-1;
				return false;
			}
			else {
				nRowIndex++;
				msg = vecUserMsg[nRowIndex];
				return true;
			}
		};

		int GetMsgCount() {
			return vecUserMsg.size();
		};

		void MoveToLast() {
			nRowIndex = vecUserMsg.size()-1;
		};

		void ResetData() {
			vecUserMsg.clear();
		};

		int nRowIndex;
		std::vector<UserMsgStru> vecUserMsg;

	};
}
