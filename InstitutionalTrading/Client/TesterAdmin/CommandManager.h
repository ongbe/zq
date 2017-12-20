#pragma once
//任务管理
#include <string>
#include <vector>

enum eCommandType
{
	CommandType_ReInit,   //参数为空
	CommandType_StartUser,//参数为UserName
	CommandType_EndUser, //参数为UserName
	CommandType_Order     //参数为sOrderAction
};
enum eOrderType
{
	OrderType_OrderInput,
	OrderType_OrderAction,
	OrderType_Random,
};
enum eOpenCloseType
{
	OpenCloseType_Open,
	OpenCloseType_Close,
	OpenCloseType_Random,
};

struct sOrderAction
{
	char mUserName[30];
	int  mnIndex;
	int  mnSocket;
	eOrderType mnOrder; 
	char mProduct[30];
	eOpenCloseType  mnOpenClose;
};

struct sOrderControl
{
	eOrderType mnOrder; 
	char mProduct[30];
	eOpenCloseType  mnOpenClose;
	int mnDuring;//下单总时常s
	int mnTotal; //下单总笔数
	bool mbUserSingle; //是否是分每个用户
};
struct sUserAndSocket
{
	sUserAndSocket()
	{
		mnSocket = mnIndex = mnNumOrder = mnLastOrderNum = 0;
	}
	sUserAndSocket(std::string nsName,
		int nSocket,
		int nIndex):msTraderName(nsName),mnSocket(nSocket),mnIndex(nIndex){mnNumOrder = 0;mnLastOrderNum = 0;}

	void SetCanOrderNum(int nNumOrder)
	{
		mnNumOrder = nNumOrder;
		mnLastOrderNum = nNumOrder;
	}
	std::string msTraderName;
	int mnSocket;
	int mnIndex;
	int mnNumOrder;
	int mnLastOrderNum;

};
struct sCommand
{
	eCommandType meCommandType;
	std::vector<sUserAndSocket> msDestionTrader;
	sOrderControl mOrderControl;

	void AddUserAndSocket(const sUserAndSocket & nUserAndSocket)
	{
		msDestionTrader.push_back(nUserAndSocket);
	}
	void Clear()
	{
		msDestionTrader.clear();
	}
};
class CCommandManager
{
public:
	CCommandManager()
	{
		mbRuning = false;
		mnTotal = 0;
		mnRemindSecond = 0;
		mnHasSend = 0;
	}
	void SetCurrentCommand(const sCommand & nCommand)
	{
		mCommand = nCommand;
		if(mCommand.mOrderControl.mbUserSingle)
		{
			mnTotal = mCommand.mOrderControl.mnTotal * mCommand.msDestionTrader.size();
			//
			for(int i = 0; i < mCommand.msDestionTrader.size();i++)
			{
				mCommand.msDestionTrader[i].SetCanOrderNum(mCommand.mOrderControl.mnTotal);
			}
		}
		else
		{
			mnTotal = mCommand.mOrderControl.mnTotal;
			//随机决定每个用户可以下多少笔
			int nLast = mnTotal;
			for(int i = 0; i < mCommand.msDestionTrader.size();i++)
			{
				if(i == mCommand.msDestionTrader.size()-1)
					mCommand.msDestionTrader[i].SetCanOrderNum(nLast);
				else
				{
					mCommand.msDestionTrader[i].SetCanOrderNum(System::Random::Random(System::Guid::NewGuid().GetHashCode()).Next(0,nLast/2));
					nLast -= mCommand.msDestionTrader[i].mnNumOrder;
				}			
			}
		}

		mnHasSend = 0;
		mnRemindSecond = mCommand.mOrderControl.mnDuring;
	}
	void GetCurrentCommand(sCommand & nCommand)
	{
		nCommand = mCommand;
	}
	void Start()
	{
		mbRuning = true;
	}
	void End()
	{
		mbRuning = false;	

	}
	bool IsRuning(){return mbRuning;};

	int JustifySingleUserCanOrder(int i,std::vector<sOrderAction> & nAction);
	//每s调用一次的获取具体任务的接口
	bool FetchCustomOrderAction(std::vector<sOrderAction> & nAction);

	void GetDesc(std::string & nsDesc)
	{
		nsDesc.clear();
		char lsTemp[1024] = {0};
		sprintf(lsTemp,"当前任务： 总笔数： %d,已发送笔数： %d ,总时长： %ds 剩余时长：%ds,目标用户数：%d",mnTotal,mnHasSend,mCommand.mOrderControl.mnDuring,mnRemindSecond,mCommand.msDestionTrader.size());
		nsDesc = lsTemp;
	}
private:
	bool mbRuning;
	sCommand mCommand;
	int mnTotal;        //总共多少笔
	int mnHasSend;      //已发出多少笔
	int mnRemindSecond; //剩余秒述

};