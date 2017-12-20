#pragma once

#include "PlatformDataMgr_Instruments.h"
#include "PlatformDataMgr_Quots.h"
#include "PlatformDataMgr_Rates.h"
#include "Interface_UserOp.h"
class IFactoryUserOp
{
private:
	static IFactoryUserOp * mInstance;
public:
	static IFactoryUserOp * GetFactory();
public:
	virtual IInterfaceUserOp * CreateUserOp() = 0;
	/*virtual IInterfaceAccountOp * CreateAccountOp(CPlatformDataMgr_Instruments * npInstrument,\
		CPlatformDataMgr_Quots * npQutos,\
		CPlatformDataMgr_Rates * npRates) = 0;*/
};

class CSubUserOpFactory:public IFactoryUserOp
{
public:
	virtual IInterfaceUserOp * CreateUserOp();
	//virtual IInterfaceAccountOp * CreateAccountOp(CPlatformDataMgr_Instruments * npInstrument,\
	//	CPlatformDataMgr_Quots * npQutos,\
	//	CPlatformDataMgr_Rates * npRates) ;
};