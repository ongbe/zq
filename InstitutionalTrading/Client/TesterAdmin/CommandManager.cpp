#include "stdafx.h"
#include "CommandManager.h"
using namespace System;
int CCommandManager::JustifySingleUserCanOrder(int i,std::vector<sOrderAction> & nAction)
{
	//随机决定在剩余的时间里面怎么发送剩余的报单

	//先决定发送几个
	int nOrder = 0;

	if(mnRemindSecond == 0)
		return mCommand.msDestionTrader[i].mnLastOrderNum;
	//先计算平均要发送几个
	int nAvgOrder = mCommand.msDestionTrader[i].mnLastOrderNum / mnRemindSecond;
	if(mnRemindSecond== 1)
		nOrder = mCommand.msDestionTrader[i].mnLastOrderNum;
	else
	{
		Int32 lRandom = System::Random::Random(System::Guid::NewGuid().GetHashCode()).Next(-1,1);

		////如果是0 降低概率
		if(nAvgOrder == 0)
		{		
			lRandom = System::Random::Random(System::Guid::NewGuid().GetHashCode()).Next(-mnRemindSecond,mCommand.msDestionTrader[i].mnLastOrderNum);
			if(lRandom > 0) 
				lRandom = 1;
			else
				lRandom = 0;
		}
	

		nOrder = nAvgOrder+lRandom;
		if(nOrder < 0)
			nOrder = 0;
	
		if(nOrder > mCommand.msDestionTrader[i].mnLastOrderNum)
			nOrder = mCommand.msDestionTrader[i].mnLastOrderNum;
	}

	for(int j = 0 ; j < nOrder; j++)
	{
		sOrderAction lsAction;
		strcpy(lsAction.mProduct,mCommand.mOrderControl.mProduct);
		strcpy(lsAction.mUserName,mCommand.msDestionTrader[i].msTraderName.c_str());
		lsAction.mnIndex = mCommand.msDestionTrader[i].mnIndex;
		if(mCommand.mOrderControl.mnOrder == OrderType_Random)
		{
			Int32 lRandom = System::Random::Random(System::Guid::NewGuid().GetHashCode()).Next(-2,2);
			lsAction.mnOrder = lRandom < 0 ? OrderType_OrderInput : OrderType_OrderAction;
		}
		else
		{
			lsAction.mnOrder = mCommand.mOrderControl.mnOrder;
		}

		if(mCommand.mOrderControl.mnOpenClose == OpenCloseType_Random)
		{
			Int32 lRandom2 = System::Random::Random(System::Guid::NewGuid().GetHashCode()).Next(-2,2);
			lsAction.mnOpenClose = lRandom2 < 0 ? OpenCloseType_Open : OpenCloseType_Close;
		}
		else
		{
			lsAction.mnOpenClose = mCommand.mOrderControl.mnOpenClose;
		}

		lsAction.mnSocket = mCommand.msDestionTrader[i].mnSocket;

		nAction.push_back(lsAction);
	}


	return nOrder;
}

bool CCommandManager::FetchCustomOrderAction(std::vector<sOrderAction> & nAction)
{
	//根据当前任务类型 
	//随机生成当前具体的任务
	if(mbRuning == false)
		return false;

	//剩余秒数减一
	mnRemindSecond --;
	for(int i = 0; i < mCommand.msDestionTrader.size();i++)
	{
		//决定这个用户生成几笔
		int nOrder = JustifySingleUserCanOrder(i,nAction);
		mCommand.msDestionTrader[i].mnLastOrderNum -= nOrder;
		mnHasSend += nOrder;
	}

	if(mnHasSend == mnTotal)
	{
		End();
	}

	return true;
}