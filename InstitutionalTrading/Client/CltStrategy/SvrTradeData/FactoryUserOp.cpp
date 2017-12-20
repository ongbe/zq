#include "stdafx.h"
#include "FactoryUserOp.h"
#include "Real_SubUserOp.h"

IFactoryUserOp*  IFactoryUserOp::mInstance = NULL;

IFactoryUserOp* IFactoryUserOp::GetFactory()
{
	if(mInstance == NULL)
	{
		mInstance = new CSubUserOpFactory();
	}
	return mInstance;
}
IInterfaceUserOp * CSubUserOpFactory::CreateUserOp()
{
	return new CSubUserOp();
}
