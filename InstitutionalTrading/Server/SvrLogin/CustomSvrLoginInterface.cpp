
#include "stdafx.h"
#include "CustomSvrLoginInterface.h"
//#include "SvrLogin.h"
#include "easymutex.h"
#include "CommonPkg.h"
#include <sstream>
#include "CommonDef.h"
#include "..\\SvrTcp\\Interface_SvrTcp.h"
#include "..\\OpenSource\\tinyxml\\tinyxml.h"
#include "MyXML.h"
CCunstomSvrLoginInterface* CCunstomSvrLoginInterface::m_pObj=NULL;

extern Ceasymutex			g_mutex;

CCunstomSvrLoginInterface::CCunstomSvrLoginInterface()
{
	//如果是日内第一次使用，直接使用初始值
	mnCurrentAvailableFrontID = 0x7F000001;
	mnCurrentAvailableSessionID = 0x7E000001;
	memset(&mExchangeTimeInfo,0,sizeof(mExchangeTimeInfo));
	//如果不是日内唯一，
	time_t lTime;
	struct tm * lTimeinfo; 
	time(&lTime);
	lTimeinfo = localtime(&lTime);
	char lcBuf[256] = {0};
	sprintf_s(lcBuf,"%04d-%02d-%02d",1900+lTimeinfo->tm_year,1+lTimeinfo->tm_mon,lTimeinfo->tm_mday);
	msTodayString = lcBuf;

	int nLastFrontID = GetPrivateProfileInt(msTodayString.c_str(),"FrontID",0,"FrontAndSessionID.txt");
	int nLastSessionID = GetPrivateProfileInt(msTodayString.c_str(),"SessionID",0,"FrontAndSessionID.txt");
	if(nLastFrontID != 0)
		mnCurrentAvailableFrontID = nLastFrontID;
	if(nLastSessionID != 0)
		mnCurrentAvailableSessionID = nLastSessionID;


	CMyXML myxml("InstitutionalTrading.xml");

	//主从服务器的配置
	mbSlave = myxml.GetNodeContentbool2(false,"root","SlaveServer");
	//从服务器登录用户名/密码，所有从服务器用同样的用户名和密码
	string lUser=myxml.GetNodeContent2("","root","Login","SlaveUserName");
	string lPass=myxml.GetNodeContent2("","root","Login","SlavePassword");
	mSlaver.SetUserAnsPass(lUser,lPass);
	//从服务器IP列表
	vector<string> SlaveIPs=myxml.GetNodeContent3("root","Login","SlaveIP");
	for(int i=0;i<(int)SlaveIPs.size();i++)
		mSlaver.AddIP(SlaveIPs[i]);
	//允许登录的socket用户类型
	vector<string>AllowUserTypeLogins=myxml.GetNodeContent3("root","Login","AllowUserTypeLogin");
	for(int i=0;i<(int)AllowUserTypeLogins.size();i++)
	{
		//从服务不允许管理终端登录
		if(AllowUserTypeLogins[i] == "USER_TYPE_MANAGE" && !mbSlave )	mAllowLoginUserType.insert(USER_TYPE_MANAGE);
		else if(AllowUserTypeLogins[i] == "USER_TYPE_RISK_CONTROL")		mAllowLoginUserType.insert(USER_TYPE_RISK_CONTROL);
		else if(AllowUserTypeLogins[i] == "USER_TYPE_TRADER")			mAllowLoginUserType.insert(USER_TYPE_TRADER);
		else if(AllowUserTypeLogins[i] == "USER_TYPE_VIEWER")			mAllowLoginUserType.insert(USER_TYPE_VIEWER);
		else if(AllowUserTypeLogins[i] == "USER_TYPE_SLAVESERVER")		mAllowLoginUserType.insert(USER_TYPE_SLAVESERVER);
	}

	if(mbSlave)
	{
		mnCurrentAvailableFrontID = 0x6F000001;
		mnCurrentAvailableSessionID = 0x6E000001;
	}
}

CCunstomSvrLoginInterface::~CCunstomSvrLoginInterface()
{
}

CCunstomSvrLoginInterface& CCunstomSvrLoginInterface::getObj(void)
{
	g_mutex.lock();
	if(!m_pObj)
		m_pObj=new CCunstomSvrLoginInterface();
	g_mutex.unlock();
	return *m_pObj;
}

////---------------------------------------------------------------------------------
////  设置交易所时间
////---------------------------------------------------------------------------------
void  CCunstomSvrLoginInterface::SetTimeInfo(const sExchangeTimeInfo& nsTimeInfo) 
{
	g_mutex.lock();
	memcpy(&mExchangeTimeInfo,&nsTimeInfo,sizeof(sExchangeTimeInfo));
	g_mutex.unlock();

}

void CCunstomSvrLoginInterface::GenerateUniqueFrontAndSessionID(int & nFrontID,int & nSessionID)
{
	nFrontID = ++mnCurrentAvailableFrontID;
	nSessionID = ++mnCurrentAvailableSessionID;
	std::stringstream lsStream;
	std::stringstream lsStream2;
	lsStream << nFrontID;
	lsStream2 << nSessionID;
	WritePrivateProfileString(msTodayString.c_str(),"FrontID",lsStream.str().c_str(),"FrontAndSessionID.txt");
	WritePrivateProfileString(msTodayString.c_str(),"SessionID",lsStream2.str().c_str(),"FrontAndSessionID.txt");
}

bool CCunstomSvrLoginInterface::InsertSocketInfo(const sSocketInfo & nSocketInfo)
{
	bool lbRet = false;

	int nUserID;
	RemoveSocket(nSocketInfo.mhSocket,nUserID);

	g_mutex.lock();
	mmapScoketAndSocketInfo.insert(std::make_pair(nSocketInfo.mhSocket,nSocketInfo));
	mmapUserIdAndSocketInfo.insert(std::make_pair(nSocketInfo.mnUserID,nSocketInfo));
	mmapUserNameAndSocketInfo.insert(std::make_pair(nSocketInfo.msUserName,nSocketInfo));

	FrontAndSessionIdKey lkey(nSocketInfo.mnFrontID,nSocketInfo.mnSessionID);
	mmapFrontIdAndSocketInfo.insert(std::make_pair(lkey,nSocketInfo));
	g_mutex.unlock();

	lbRet = true;
	return lbRet;
}
bool CCunstomSvrLoginInterface::InsertSocketInfo2(SOCKET nSocket,int nTraderID)
{
    bool lbRet = false;

    g_mutex.lock();

    if(mmapSocketAndTraderIDs.find(nSocket) == mmapSocketAndTraderIDs.end())
    {  
        std::vector<int> vTraderID;
        vTraderID.push_back(nTraderID);
        mmapSocketAndTraderIDs.insert(std::make_pair(nSocket,vTraderID));        
    }
    else
    {
        std::vector<int>& vTraderID = mmapSocketAndTraderIDs[nSocket];
        vTraderID.push_back(nTraderID);            
    }

    g_mutex.unlock();
    lbRet = true;
    return lbRet;
}
bool CCunstomSvrLoginInterface::RemoveSocket2(const SOCKET & nSocketHandle,const int & nTraderID)
{
	bool lbRet = false;
	g_mutex.lock();

    if(mmapSocketAndTraderIDs.find(nSocketHandle) != mmapSocketAndTraderIDs.end())
    {
        std::vector<int>& vTraderID = mmapSocketAndTraderIDs[nSocketHandle];
        std::vector<int>::iterator it = vTraderID.begin();
        while (it!=vTraderID.end())
        {            
            if(nTraderID == *it)
            {
               vTraderID.erase(it);

				//mmapSocketAndTraderIDs<socket,count>,一个socket对应的TraderID是主从双份的，所以此处只操作一次
				//否则会出现主从交易员在线不同步问题
				break;
            }

            it++;
        }
    }

	g_mutex.unlock();
	return lbRet;
}
bool CCunstomSvrLoginInterface::CloseUserSocket(int  nUserId)
{

	std::vector<SOCKET> lNeedClostSocket;
	_MapSocketAndSocketInfo::iterator lIter = mmapScoketAndSocketInfo.begin();
	for(;lIter != mmapScoketAndSocketInfo.end();lIter++)
	{
		if(	lIter->second.mnUserID = nUserId)
			lNeedClostSocket.push_back(lIter->first);	

	}

	std::vector<SOCKET>::iterator lIterSocket = lNeedClostSocket.begin();
	for(;lIterSocket != lNeedClostSocket.end();lIterSocket++)
	{
		int nUserId;
		RemoveSocket(*lIterSocket,nUserId);
		CInterface_SvrTcp::getObj().Disconnect(*lIterSocket);
	}
	lNeedClostSocket.clear();
	
	return true;
}
bool CCunstomSvrLoginInterface::CloseAllTraderSocket()
{

	std::vector<SOCKET> lNeedClostSocket;
	_MapSocketAndSocketInfo::iterator lIter = mmapScoketAndSocketInfo.begin();
	for(;lIter != mmapScoketAndSocketInfo.end();lIter++)
	{
		if(lIter->second.meUserType == USER_TYPE_TRADER || 
			lIter->second.meUserType == USER_TYPE_VIEWER ||
			lIter->second.meUserType == USER_TYPE_RISK_CONTROL ||
			lIter->second.meUserType == USER_TYPE_SLAVESERVER)
			lNeedClostSocket.push_back(lIter->first);	
		
	}
	
	LOGIN_LOG("CloseAllTraderSocket");

	std::vector<SOCKET>::iterator lIterSocket = lNeedClostSocket.begin();
	for(;lIterSocket != lNeedClostSocket.end();lIterSocket++)
	{
		int nUserId;
		//Disconnect后也会响应CMDID_SvrTcpDisconn调用RemoveSocket，导致重复调用，此处不调用
		//RemoveSocket(*lIterSocket,nUserId);

		//LOGIN_LOG("nSocket = %d		nUserID = %d",*lIterSocket,nUserId);

		CInterface_SvrTcp::getObj().Disconnect(*lIterSocket);
	}
	lNeedClostSocket.clear();
	
	return true;
}
bool CCunstomSvrLoginInterface::CloseAllSocket()
{
	std::vector<SOCKET> lNeedClostSocket;
	_MapSocketAndSocketInfo::iterator lIter = mmapScoketAndSocketInfo.begin();
	for(;lIter != mmapScoketAndSocketInfo.end();lIter++)
	{	
		lNeedClostSocket.push_back(lIter->first);	
	}

	std::vector<SOCKET>::iterator lIterSocket = lNeedClostSocket.begin();
	for(;lIterSocket != lNeedClostSocket.end();lIterSocket++)
	{
		int nUserId;
		RemoveSocket(*lIterSocket,nUserId);
		CInterface_SvrTcp::getObj().Disconnect(*lIterSocket);
	}
	lNeedClostSocket.clear();
	return true;
}
bool CCunstomSvrLoginInterface::RemoveSocket(const SOCKET & nSocketHandle,int & nUserId)
{
	bool lbRet = false;
	g_mutex.lock();

	_MapSocketAndSocketInfo::iterator lIter = mmapScoketAndSocketInfo.find(nSocketHandle);
	if(lIter != mmapScoketAndSocketInfo.end())
	{
		nUserId = lIter->second.mnUserID;
		mmapScoketAndSocketInfo.erase(lIter);
		lbRet = true;
	}

	_MapFrontIDAndSessionAndSocketInfo::iterator lIterFrontID = mmapFrontIdAndSocketInfo.begin();
	while(lIterFrontID != mmapFrontIdAndSocketInfo.end())
	{
		if(lIterFrontID->second.mhSocket == nSocketHandle )
		{
			mmapFrontIdAndSocketInfo.erase(lIterFrontID++);	
			break;
		}
		else
			lIterFrontID++;
	}

	_MultiMapUserIDAndSocketInfo::iterator lIterUserId = mmapUserIdAndSocketInfo.begin();
	while(lIterUserId != mmapUserIdAndSocketInfo.end())
	{
		if(lIterUserId->second.mhSocket == nSocketHandle )
		{
			mmapUserIdAndSocketInfo.erase(lIterUserId++);	
			break;
		}
		else
			lIterUserId++;
	}

	_MultiMapUserNameAndSocketInfo::iterator lIterUserName = mmapUserNameAndSocketInfo.begin();
	while(lIterUserName != mmapUserNameAndSocketInfo.end())
	{
		if(lIterUserName->second.mhSocket == nSocketHandle )
		{
			mmapUserNameAndSocketInfo.erase(lIterUserName++);	
			break;
		}
		else
			lIterUserName++;
	}

    if(mmapSocketAndTraderIDs.find(nSocketHandle) != mmapSocketAndTraderIDs.end())
    {
        std::vector<int> vTraderID = mmapSocketAndTraderIDs[nSocketHandle];
        std::vector<int>::iterator it = vTraderID.begin();
        while (it!=vTraderID.end())
        {
            std::map<int,int>::iterator itTraderID = mmapOnlineUser.find(*it);
            if(itTraderID != mmapOnlineUser.end())
            {
                int n = mmapOnlineUser[*it] ;
                if(n > 1)
                    mmapOnlineUser[*it] = --n;
                else
                    mmapOnlineUser.erase(itTraderID);

				//mmapSocketAndTraderIDs<socket,count>,一个socket对应的TraderID是主从双份的，所以此处只操作一次
				//否则会出现主从交易员在线不同步问题
				break;
            }

            it++;
        }
		mmapSocketAndTraderIDs.erase(nSocketHandle);
    }

	g_mutex.unlock();

	return lbRet;

}

//---------------------------------------------------------------------------------
//	获取socket的对应的UserID,FrontID,SessionID
//  成功返回CF_ERROR_SUCCESS
//  失败返回CF_ERROR_LOGIN_SOCKET_NOT_EXIST
//---------------------------------------------------------------------------------
CF_ERROR CCunstomSvrLoginInterface::GetSocketUserIDAndFrontSessionID(const SOCKET& nhSocketHandle,int & nUsrID,int & nFrontID,int& nSessionID) 
{

	CF_ERROR lRetError = CF_ERROR_LOGIN_SOCKET_NOT_EXIST;
	g_mutex.lock();
	if(mmapScoketAndSocketInfo.find(nhSocketHandle) \
		!= mmapScoketAndSocketInfo.end())
	{
		sSocketInfo ls = mmapScoketAndSocketInfo[nhSocketHandle];
		nFrontID = ls.mnFrontID;
		nSessionID = ls.mnSessionID;
		nUsrID = ls.mnUserID;
		lRetError = CF_ERROR_SUCCESS;
	}
	g_mutex.unlock();

	return lRetError;
}

CF_ERROR CCunstomSvrLoginInterface::GetSocketHandleWithFrontAndSessionID(const int & nFrontID,const int& nSessionID,SOCKET & nhShocketHandle) 
{
	CF_ERROR lRetError = CF_ERROR_LOGIN_SOCKET_NOT_EXIST;
	g_mutex.lock();
	FrontAndSessionIdKey lk(nFrontID,nSessionID);
	_MapFrontIDAndSessionAndSocketInfo::iterator liter = mmapFrontIdAndSocketInfo.find(lk);
	if( liter!= mmapFrontIdAndSocketInfo.end())
	{		
		nhShocketHandle = liter->second.mhSocket;		
		lRetError = CF_ERROR_SUCCESS;
	}
	g_mutex.unlock();

	return lRetError;

}

//---------------------------------------------------------------------------------
//	获取UserID的对应的socket列表
//  成功返回CF_ERROR_SUCCESS
//  成功返回CF_ERROR_SUCCESS
//  失败返回CF_ERROR_LOGIN_USER_NOT_EXIST
//---------------------------------------------------------------------------------
CF_ERROR CCunstomSvrLoginInterface::GetUserSockets(const int & nUserID, std::vector<SOCKET> & nVecSocket)
{
	int nRet = CF_ERROR_LOGIN_USER_NOT_EXIST;
	g_mutex.lock();
	_MultiMapUserIDAndSocketInfo::iterator lf = mmapUserIdAndSocketInfo.lower_bound(nUserID);
	_MultiMapUserIDAndSocketInfo::iterator le = mmapUserIdAndSocketInfo.upper_bound(nUserID);
	while(lf != le)
	{
		nVecSocket.push_back(lf->second.mhSocket);
		lf++;
		nRet = CF_ERROR_SUCCESS;
	}
	g_mutex.unlock();
	return nRet;
}
//---------------------------------------------------------------------------------
//	获取UserID的对应的socket列表
//  成功返回CF_ERROR_SUCCESS
//  成功返回CF_ERROR_SUCCESS
//  失败返回CF_ERROR_LOGIN_USER_NOT_EXIST
//---------------------------------------------------------------------------------
CF_ERROR CCunstomSvrLoginInterface::GetUserSockets(const std::string & nUserName, std::vector<SOCKET> & nVecSocket, int & nUserID)
{
	g_mutex.lock();
	_MultiMapUserNameAndSocketInfo::iterator lf = mmapUserNameAndSocketInfo.lower_bound(nUserName);
	_MultiMapUserNameAndSocketInfo::iterator le = mmapUserNameAndSocketInfo.upper_bound(nUserName);
	while(lf != le)
	{
		nUserID = lf->second.mnUserID;
		nVecSocket.push_back(lf->second.mhSocket);
		lf++;
	}
	g_mutex.unlock();
	return CF_ERROR_LOGIN_USER_NOT_EXIST;
}


//获取时间
bool CCunstomSvrLoginInterface::GetTimeInfo(sExchangeTimeInfo & nTimeInfo)
{
	g_mutex.lock();
	memcpy(&nTimeInfo,&mExchangeTimeInfo,sizeof(sExchangeTimeInfo));
	g_mutex.unlock();
	return true;

}

//---------------------------------------------------------------------------------
//	设置用户的交易数据是否准备好了	
//---------------------------------------------------------------------------------
void CCunstomSvrLoginInterface::SetUserDataReady(const std::string & nUserName,bool nbReady)
{
	g_mutex.lock();
	std::map<std::string,bool>::iterator lf = mmapUserDataReady.find(nUserName);
	if(lf != mmapUserDataReady.end())
	{
		lf->second = nbReady;
	}
	else
	{
		mmapUserDataReady.insert(std::make_pair(nUserName,nbReady));
	}
	g_mutex.unlock();
}
bool CCunstomSvrLoginInterface::GetUserDataReady(const std::string &nUserName)
{
	bool lbRet = false;
	g_mutex.lock();
	std::map<std::string,bool>::iterator lf = mmapUserDataReady.find(nUserName);
	if(lf != mmapUserDataReady.end())
	{
		lbRet = lf->second ;
	}	
	g_mutex.unlock();
	return lbRet;

}


bool CCunstomSvrLoginInterface::GetOnlineUser(std::set<int> & nVecUsreID)
{
	g_mutex.lock();
	_MultiMapUserIDAndSocketInfo::iterator lf = mmapUserIdAndSocketInfo.begin();
	_MultiMapUserIDAndSocketInfo::iterator le = mmapUserIdAndSocketInfo.end();
	while(lf != le)
	{
		nVecUsreID.insert(lf->first);
		lf++;
	}
	g_mutex.unlock();
	return true;
}

bool CCunstomSvrLoginInterface::GetUserTypeSocket(UserType  nUserType,std::vector<SOCKET> & nVecSocket)
{
	g_mutex.lock();
	_MapSocketAndSocketInfo::iterator lIter = mmapScoketAndSocketInfo.begin();
	for(;lIter != mmapScoketAndSocketInfo.end();lIter++)
	{
		if(lIter->second.meUserType == nUserType)
			nVecSocket.push_back(lIter->first);	

	}
	g_mutex.unlock();
	return true;
}



//---------------------------------------------------------------------------------
//	判断主从
//---------------------------------------------------------------------------------
bool CCunstomSvrLoginInterface::IsSlaver() 
{
	return mbSlave;
}

//---------------------------------------------------------------------------------
//	判断从服务用户数据的有效性
//---------------------------------------------------------------------------------
CF_ERROR CCunstomSvrLoginInterface::IsValidSlaveUser(const std::string& strUser,
													 const std::string& strPwd,
													 const std::string& strIP, 
													 int& nUserID)
{
	return mSlaver.ValidateSlaverUser(strUser,strPwd,strIP,nUserID);
}


void CCunstomSvrLoginInterface::PushSubscribeSocketAllUserStatus()
{
	std::set<SOCKET>::iterator lIter = msetSubsribleSocket.begin();
	for(; lIter != msetSubsribleSocket.end(); lIter++)
	{
		std::vector<sUserOnlineInfo> lvec;
		GetAllOnlineUserStatus(lvec);
		CInterface_SvrTcp::getObj().SendPkgData(*lIter,
			CMDID_AllUserLoginStatus_Push,
			lvec.size() == 0 ? NULL :&lvec[0],
			sizeof(sUserOnlineInfo)*lvec.size(),
			0,
			0,
			0,
			0,
			CF_ERROR_SUCCESS,0);
	}
}