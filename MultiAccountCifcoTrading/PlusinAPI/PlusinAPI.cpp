#include "stdafx.h"
#include "PlusinImpl.h"
#include "../inc/PlusinCommon/PlusinAPI.h"

PlusinAPI* PlusinAPI::GetInstance()
{
	return PlusinImpl::GetInstance();
}

void PlusinAPI::DestroyInstance()
{
	PlusinImpl::DestroyInstance();
}
