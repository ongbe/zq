
#pragma once


const int s_nReplationPersonTypeCount = 4;
const char s_strRelationPersonType[][64] = {
	TEXT("1 指令下达人"), 
	TEXT("2 资金调拨人"), 
	TEXT("3 法人代表"), 
	TEXT("4 开户授权人") 
};


#define TRANSCURRFORM(pCurrForm, nModuleNum) 	if(pCurrForm!=NULL) {\
		if(pCurrForm->GetModuleNum() == nModuleNum) {\
			return;\
		}\
		if(pCurrForm->isChanged_Item()) {\
			if(AfxMessageBox("当前窗口数据改变未保存，是否离开当前窗体？", MB_YESNO|MB_ICONQUESTION)==IDNO) {\
				return;\
			}\
		}\
		delete pCurrForm;\
		pCurrForm = NULL;\
	}


int GetPackageHeadLen();
int GetPackageHeadTailLen();
bool PackageData(UINT nCMDID, UINT nModuleID, int nContentLen, char* pBuff, int nBuffLen, 
				 UINT seq=0, UINT subseq=0, UINT nData1=0, UINT nData2=0, UINT nData3=0, UINT nData4=0);