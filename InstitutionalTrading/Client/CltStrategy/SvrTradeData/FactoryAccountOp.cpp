#include "stdafx.h"
#include "FactoryAccountOp.h"
#include "Real_SubAccountOp.h"

IFactoryAccountOp*  IFactoryAccountOp::mInstance = NULL;

IFactoryAccountOp* IFactoryAccountOp::GetFactory()
{
	if(mInstance == NULL)
	{
		mInstance = new CSubAccountFactoryAccountOp();
	}
	return mInstance;
}
IInterfaceAccountOp * CSubAccountFactoryAccountOp::CreateAccountOp()
{
	return new CSubAccountOp();
}

//IInterfaceAccountOp * CSubAccountFactoryAccountOp::CreateAccountOp(CPlatformDataMgr_Instruments * npInstrument,
//														  CPlatformDataMgr_Quots * npQutos,
//														  CPlatformDataMgr_Rates * npRates)
//{
//	return new CSubAccountOp(npInstrument,npQutos,npRates);
//}