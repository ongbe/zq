#pragma once

#include "PlatformDataMgr_Instruments.h"
#include "PlatformDataMgr_Quots.h"
#include "PlatformDataMgr_Rates.h"
#include "Interface_AccountOp.h"
class IFactoryAccountOp
{
private:
	static IFactoryAccountOp * mInstance;
public:
	static IFactoryAccountOp * GetFactory();
public:
	virtual IInterfaceAccountOp * CreateAccountOp() = 0;
	/*virtual IInterfaceAccountOp * CreateAccountOp(CPlatformDataMgr_Instruments * npInstrument,\
		CPlatformDataMgr_Quots * npQutos,\
		CPlatformDataMgr_Rates * npRates) = 0;*/
};

class CSubAccountFactoryAccountOp :public IFactoryAccountOp
{
public:
	virtual IInterfaceAccountOp * CreateAccountOp();
	//virtual IInterfaceAccountOp * CreateAccountOp(CPlatformDataMgr_Instruments * npInstrument,\
	//	CPlatformDataMgr_Quots * npQutos,\
	//	CPlatformDataMgr_Rates * npRates) ;
};