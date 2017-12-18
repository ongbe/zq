
#include "stdafx.h"
#include "../inc/Quotation/commonBNCMFunc.h"

int GetPackageHeadLen()
{
	return (int)sizeof(Stru_UniPkgHead);
}

int GetPackageHeadTailLen()
{
	return (int)sizeof(Stru_UniPkgHead)+(int)sizeof(DWORD);
}

bool PackageData(UINT nCMDID, UINT nModuleID, int nContentLen, char* pBuff, int nBuffLen, 
				 UINT seq, UINT subseq, UINT nData1, UINT nData2, UINT nData3, UINT nData4)
{
	if(pBuff==NULL)
		return false;
	if(nContentLen+GetPackageHeadTailLen()>nBuffLen)
		return false;

	DWORD dwTail = TailID_UniPkg;
	int nPkgLen = GetPackageHeadTailLen()+nContentLen;

	Stru_UniPkgHead* pHead = (Stru_UniPkgHead*)pBuff;
	pHead->headid = HeadID_UniPkg;
	pHead->cmdid = nCMDID;
	pHead->moduleid = nModuleID;
	pHead->len = nContentLen;
	pHead->seq = seq;
	pHead->subseq = subseq;
	pHead->userdata1 = nData1;
	pHead->userdata2 = nData2;
	pHead->userdata3 = nData3;
	pHead->userdata4 = nData4;
	*((DWORD*)(pBuff+(nPkgLen-4)))=dwTail;
	return true;
}