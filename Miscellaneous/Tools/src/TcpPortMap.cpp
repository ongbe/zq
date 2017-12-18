// TcpClientWorker.cpp: implementation of the CTcpClientWorker class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <assert.h>
#include "TcpPortMap.h"
#include "tools_ansi.h"
#include "PostMsgByEventParam.h"
#include "EventParam.h"
#include "FileOpr.h"


#ifdef WIN32
#pragma unmanaged
#endif 



#define DEBUGID


auto_ptr<CTcpPortMap>   CTcpPortMap::m_pObj;
HANDLE	                CTcpPortMap::m_hThread=0;
unsigned long           CTcpPortMap::m_idThread=0;


//获取类的唯一实例指针。进程生命周期内只会创建一次。
CTcpPortMap& CTcpPortMap::getObj(void)
{
    //用running和InterlockedCompareExchange保证互斥，volatile避免编译器优化
    static volatile long running=0;
    while(InterlockedCompareExchange(&running,1,0)==1) Sleep(0);
    if(m_pObj.get()==NULL)
        m_pObj.reset(new CTcpPortMap());
    InterlockedExchange(&running,0);

    return *m_pObj;
}

CTcpPortMap::CTcpPortMap()
{
#ifdef DEBUGID
    CFileOpr::getObj().writelocallog("TcpPortMap","");
    CFileOpr::getObj().writelocallog("TcpPortMap","CTcpPortMap::CTcpPortMap()");
#endif
	m_hThread=CreateThread(NULL,65536,(LPTHREAD_START_ROUTINE)ThreadWorker,this,0,&m_idThread);
    CFileOpr::getObj().writelocallog("threadid","TID:\t% 8u\tCTcpPortMap::ThreadWorker",m_idThread);
}

CTcpPortMap::~CTcpPortMap()
{
#ifdef DEBUGID
    CFileOpr::getObj().writelocallog("TcpPortMap","CTcpPortMap::~CTcpPortMap() Start");
#endif
    m_mutex.lock();

	//发送WM_QUIT通知线程结束
	PostThreadMessage(m_idThread,WM_QUIT,0,0);

    m_idThread=0;
	if(m_hThread)
	{
		DWORD ExitCode;
		WaitForSingleObject((HANDLE)m_hThread,8000);
		if(GetExitCodeThread((HANDLE)m_hThread,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
		{
			TerminateThread((HANDLE)m_hThread,0);
		}
		CloseHandle(m_hThread);
		m_hThread=NULL;
	}



    vector<Stru_IPAddress> bindaddrs;
    GetAllBindAddr_Internal(bindaddrs);
    m_mutex.unlock();

    for(int i=0;i<(int)bindaddrs.size();i++)
        DelPortMap(bindaddrs[i]);

    m_pObj.release();

#ifdef DEBUGID
    CFileOpr::getObj().writelocallog("TcpPortMap","CTcpPortMap::~CTcpPortMap() End");
#endif
}


bool CTcpPortMap::IsDestAddrExist(const Stru_IPAddress& DestAddr)
{
    CeasymutexGuard guard(m_mutex);
    for(map<Stru_IPAddress,Stru_PortMapSvr>::iterator it=m_mapPortMapSvr.begin();it!=m_mapPortMapSvr.end();it++)
    {
        if(it->second.Param.DestAddr==DestAddr) return true;
    }
    return false;
}

bool CTcpPortMap::NewPortMap(Stru_PortMapParam& param)
{
    CeasymutexGuard guard(m_mutex);

    //检查地址是否已经绑定
    if(m_mapPortMapSvr.find(param.BindAddr)!=m_mapPortMapSvr.end())
        return false;

    //构造新的TcpSvr
    CTcpServerWorker *pTcpSvr=new CTcpServerWorker(param.BindAddr,param.MaxListen,TcpSvr_RcvDataCB,TcpSvr_AcceptCB,TcpSvr_DisconnCB,NULL,param.SOSNDBUFSize,param.SORCVBUFSize);
    if(!pTcpSvr) 
        return false;

    if(param.BindAddr.Port==0) 
        param.BindAddr.Port=pTcpSvr->GetBindAddr().Port;

    m_mapPortMapSvr[param.BindAddr]=Stru_PortMapSvr(param,pTcpSvr);

    return true;
}

void CTcpPortMap::DelPortMap(const Stru_IPAddress& bindaddr)
{
    CeasymutexGuard guard(m_mutex);
    DelPortMap_Internal(bindaddr);
}
void CTcpPortMap::DelPortMap_Internal(const Stru_IPAddress& bindaddr)
{
    map<Stru_IPAddress,Stru_PortMapSvr>::iterator it=m_mapPortMapSvr.find(bindaddr);
    if(it!=m_mapPortMapSvr.end())
    {
        if(it->second.pTcpSvr!=NULL)
        {
            //删除对应的TcpClient
            vector<SOCKET> AllDataSocket;
            it->second.pTcpSvr->GetAllDataSocket(AllDataSocket);
            for(int i=0;i<(int)AllDataSocket.size();i++)
                DelPortMapClient_Internal(AllDataSocket[i]);

            //删除TcpSvr
            delete it->second.pTcpSvr;
        }
        m_mapPortMapSvr.erase(it);
    }
}
void CTcpPortMap::DelPortMap2(const Stru_IPAddress& destaddr)
{
    CeasymutexGuard guard(m_mutex);

    vector<Stru_IPAddress> bindaddrs;
    GetBindAddr_Internal(destaddr,bindaddrs);
    for(int i=0;i<(int)bindaddrs.size();i++)
        DelPortMap_Internal(bindaddrs[i]);
}

void CTcpPortMap::DelPortMapClient_Internal(SOCKET hDataSocket)
{
    map<SOCKET,Stru_PortMapClient>::iterator it1=m_MapDataSocket2PortMapClient.find(hDataSocket);
    if(it1!=m_MapDataSocket2PortMapClient.end()&&
        it1->second.pTcpClient!=NULL)
    {
        SOCKET hClientSocket=it1->second.pTcpClient->getSocket();
        if(hClientSocket>0)
        {
            map<SOCKET,Stru_PortMapClient>::iterator it2=m_MapClientSocket2PortMapClient.find(hClientSocket);
            if(it2!=m_MapClientSocket2PortMapClient.end()&&
                it2->second.hDataSocket==hDataSocket)
                m_MapClientSocket2PortMapClient.erase(it2);
        }

        delete it1->second.pTcpClient;
        m_MapDataSocket2PortMapClient.erase(it1);
    }
}
void CTcpPortMap::GetAllBindAddr_Internal(vector<Stru_IPAddress>& bindaddrs)
{
    bindaddrs.clear();
    for(map<Stru_IPAddress,Stru_PortMapSvr>::iterator it=m_mapPortMapSvr.begin();it!=m_mapPortMapSvr.end();it++)
        bindaddrs.push_back(it->first);
}
void CTcpPortMap::GetBindAddr(const Stru_IPAddress& destaddr,vector<Stru_IPAddress>& bindaddrs)
{
    CeasymutexGuard guard(m_mutex);
    GetBindAddr_Internal(destaddr,bindaddrs);
}
void CTcpPortMap::GetBindAddr_Internal(const Stru_IPAddress& destaddr,vector<Stru_IPAddress>& bindaddrs)
{
    bindaddrs.clear();
    for(map<Stru_IPAddress,Stru_PortMapSvr>::iterator it=m_mapPortMapSvr.begin();it!=m_mapPortMapSvr.end();it++)
    {
        if(it->second.Param.DestAddr==destaddr)
            bindaddrs.push_back(it->first);
    }
}

bool CTcpPortMap::GetPortMap_Internal(const Stru_IPAddress& bindaddr,Stru_PortMapSvr& PortMapSvr)
{
    map<Stru_IPAddress,Stru_PortMapSvr>::iterator it=m_mapPortMapSvr.find(bindaddr);
    if(it==m_mapPortMapSvr.end()) return false;

    PortMapSvr=it->second;
    return true;
}
bool CTcpPortMap::GetPortMap2_Internal(SOCKET hDataSocket,Stru_PortMapSvr& PortMapSvr)
{
    for(map<Stru_IPAddress,Stru_PortMapSvr>::iterator it=m_mapPortMapSvr.begin();it!=m_mapPortMapSvr.end();it++)
    {
        if(it->second.pTcpSvr!=NULL&&it->second.pTcpSvr->IsDataSocket(hDataSocket))
        {
            PortMapSvr=it->second;
            return true;
        }
    }
    return false;
}
bool CTcpPortMap::GetPortMapClient_Internal(SOCKET hDataSocket,Stru_PortMapClient& PortMapClient)
{
    map<SOCKET,Stru_PortMapClient>::const_iterator it=m_MapDataSocket2PortMapClient.find(hDataSocket);
    if(it!=m_MapDataSocket2PortMapClient.end())
    {
        PortMapClient=it->second;
        return true;
    }
    return false;
}
bool CTcpPortMap::SetPortMapClient_Internal(SOCKET hDataSocket,const Stru_PortMapClient& PortMapClient)
{
    map<SOCKET,Stru_PortMapClient>::iterator it=m_MapDataSocket2PortMapClient.find(hDataSocket);
    if(it!=m_MapDataSocket2PortMapClient.end())
    {
        it->second=PortMapClient;
        return true;
    }
    return false;
}






//下面是TcpServer的回调函数
void CTcpPortMap::TcpSvr_RcvDataCB(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hDataSocket)
{
#ifdef DEBUGID
    CFileOpr::getObj().writelocallog("TcpPortMap","TcpSvr_RcvDataCB() \t\tDataLen=%d hDataSocket=%d",DataLen,hDataSocket);
#endif
    Stru_SocketCallBackParam2 CBParam(DataLen,SvrAddr,RemoteAddr,hDataSocket,0);
    CPostMsgByEventParam::getObj().postMsgStrThread(m_idThread,(int)CBType_TcpSvr_RcvData,0,string(""),&CBParam,sizeof(CBParam),pData,DataLen);
}

void CTcpPortMap::TcpSvr_RcvData_Internal(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hDataSocket)
{
    Stru_PortMapSvr PortMapSvr;
    Stru_PortMapClient PortMapClient;
    if(GetPortMap2_Internal(hDataSocket,PortMapSvr)&&
        GetPortMapClient_Internal(hDataSocket,PortMapClient)&&
        PortMapClient.pTcpClient!=NULL)
    {
        if(PortMapSvr.Param.MapMode==Map_BypassHttpProxySvr&&
            PortMapClient.ProxySvrConnStatus!=Stru_PortMapClient::ProxySvr_ConnectSucc)
        {
#ifdef DEBUGID
            CFileOpr::getObj().writelocallog("TcpPortMap","TcpSvr_RcvDataCB()_Inter DataLen=%d hDataSocket=%d 暂存数据",DataLen,hDataSocket);
#endif
            PortMapClient.SaveData_WaitingForHttpProxyConnSucc(pData,DataLen);
            SetPortMapClient_Internal(hDataSocket,PortMapClient);
        }
        else
        {
            PortMapClient.pTcpClient->SendData(pData,DataLen);
#ifdef DEBUGID
            CFileOpr::getObj().writelocallog("TcpPortMap","TcpSvr_RcvDataCB()_Inter DataLen=%d hDataSocket=%d 转发数据",DataLen,hDataSocket);
#endif
        }
    }
}

//接收一个连接请求，创建一个TcpClient对象，连接转发目的地
void CTcpPortMap::TcpSvr_AcceptCB(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hDataSocket)
{
#ifdef DEBUGID
    CFileOpr::getObj().writelocallog("TcpPortMap","TcpSvr_AcceptCB() \t\thDataSocket=%d",hDataSocket);
#endif
    Stru_SocketCallBackParam2 CBParam(0,SvrAddr,RemoteAddr,hDataSocket,0);
    CPostMsgByEventParam::getObj().postMsgStrThread(m_idThread,(int)CBType_TcpSvr_Accept,0,string(""),&CBParam,sizeof(CBParam));
}
void CTcpPortMap::TcpSvr_Accept_Internal(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hDataSocket)
{
    Stru_PortMapSvr PortMapSvr;
    if(!CTcpPortMap::getObj().GetPortMap2_Internal(hDataSocket,PortMapSvr))
        return;

    Stru_IPAddress PeerAddr;
    if(PortMapSvr.Param.MapMode==Map_Direct) PeerAddr=PortMapSvr.Param.DestAddr;
    else if(PortMapSvr.Param.MapMode==Map_BypassHttpProxySvr) PeerAddr=PortMapSvr.Param.ProxyServerAddr;

    CTcpClientWorker *pTcpClient=new CTcpClientWorker(
        Stru_IPAddress(),
        PeerAddr,
        TcpClient_RcvDataCB,
        TcpClient_DisconnCB,
        TcpClient_StartConnectCB,
        TcpClient_ConnSuccessCB,
        TcpClient_ConnFailureCB,
        NULL,
        PortMapSvr.Param.SOSNDBUFSize,
        PortMapSvr.Param.SORCVBUFSize,
        0,
        (int)hDataSocket);

    if(pTcpClient!=NULL)
    {
        Stru_PortMapClient PortMapClient(hDataSocket,pTcpClient);

        //如果通过http proxy连接，先把http connect包发送出去
        if(PortMapSvr.Param.MapMode==Map_BypassHttpProxySvr)
        {
#ifdef DEBUGID
            CFileOpr::getObj().writelocallog("TcpPortMap","TcpSvr_Accept()_Inter\t创建TcpClient 将CONNECT请求发送给TcpClient hDataSocket=%d",hDataSocket);
#endif
            char buf[256];
            _snprintf(buf,sizeof(buf)-1,"CONNECT %s:%d HTTP/1.0\r\n\r\n",CTools_Ansi::ConvertdwIP(PortMapSvr.Param.DestAddr.IP),PortMapSvr.Param.DestAddr.Port);
            buf[sizeof(buf)-1]=0;
            pTcpClient->SendData(buf,strlen(buf));

            PortMapClient.ProxySvrConnStatus=Stru_PortMapClient::ProxySvr_Connecting;
        }
        else
        {
#ifdef DEBUGID
            CFileOpr::getObj().writelocallog("TcpPortMap","TcpSvr_Accept()_Inter\t创建TcpClient hDataSocket=%d",hDataSocket);
#endif
        }

        map<SOCKET,Stru_PortMapClient>& MapDataSocket2PortMapClient=CTcpPortMap::getObj().m_MapDataSocket2PortMapClient;
        MapDataSocket2PortMapClient[hDataSocket]=PortMapClient;
    }
}


void CTcpPortMap::TcpSvr_DisconnCB(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hDataSocket)
{
#ifdef DEBUGID
    CFileOpr::getObj().writelocallog("TcpPortMap","TcpSvr_DisconnCB() \t\thDataSocket=%d",hDataSocket);
#endif
    Stru_SocketCallBackParam2 CBParam(0,SvrAddr,RemoteAddr,hDataSocket,0);
    CPostMsgByEventParam::getObj().postMsgStrThread(m_idThread,(int)CBType_TcpSvr_Disconn,0,string(""),&CBParam,sizeof(CBParam));
}
void CTcpPortMap::TcpSvr_Disconn_Internal(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hDataSocket)
{
    CTcpPortMap::getObj().DelPortMapClient_Internal(hDataSocket);
}



//下面是TcpClient的回调函数
void CTcpPortMap::TcpClient_RcvDataCB(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
{
#ifdef DEBUGID
    CFileOpr::getObj().writelocallog("TcpPortMap","TcpClt_RcvDataCB() \t\thSocket=%d UserData=%d DataLen=%d",hSocket,UserData,DataLen);
#endif
    Stru_SocketCallBackParam2 CBParam(DataLen,SvrAddr,SvrAddr,hSocket,UserData);
    CPostMsgByEventParam::getObj().postMsgStrThread(m_idThread,(int)CBType_TcpClient_RcvData,0,string(""),&CBParam,sizeof(CBParam),pData,DataLen);
}
void CTcpPortMap::TcpClient_RcvData_Internal(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
{
    int off=0;

    Stru_PortMapSvr PortMapSvr;
    Stru_PortMapClient PortMapClient;
    SOCKET hDataSocket=(SOCKET)UserData;
    if(hDataSocket!=INVALID_SOCKET&&
        hDataSocket!=0&&
        CTcpPortMap::getObj().GetPortMap2_Internal(hDataSocket,PortMapSvr)&&
        PortMapSvr.pTcpSvr!=NULL&&
        CTcpPortMap::getObj().GetPortMapClient_Internal(hDataSocket,PortMapClient)&&
        PortMapClient.pTcpClient!=NULL)
    {
        if(PortMapSvr.Param.MapMode==Map_BypassHttpProxySvr&&
            PortMapClient.ProxySvrConnStatus==Stru_PortMapClient::ProxySvr_Connecting)
        {
            //http proxy server返回的连接结果 HTTP/1.0 200 OK\r\n\r\n表示成功
            if(DataLen>=19&&memcmp(pData,"HTTP",4)==0)
            {
                if(memcmp(pData+9,"200",3)==0)
                {
#ifdef DEBUGID
                    CFileOpr::getObj().writelocallog("TcpPortMap","TcpClt_RcvDataCB()_Inter hSocket=%d UserData=%d 得到HTTP 200 OK hDataSocket=%d",hSocket,UserData,hDataSocket);
#endif

                    //连接成功
                    PortMapClient.ProxySvrConnStatus=Stru_PortMapClient::ProxySvr_ConnectSucc;
                    CTcpPortMap::getObj().SetPortMapClient_Internal(hDataSocket,PortMapClient);
                    off=DataLen;

                    //如果有等待发送的数据，发送出去
                    if(PortMapClient.IsHaveData_WaitingForHttpProxyConnSucc())
                    {
                        int i=0,off2=0;
                        for(;i<PortMapClient.MaxPkgCount_WaitingForHttpProxyConnSucc;i++)
                        {
                            if(PortMapClient.DataBufLen_WaitingForHttpProxyConnSucc[i]>0)
                            {
                                PortMapClient.pTcpClient->SendData(
                                    PortMapClient.DataBuf_WaitingForHttpProxyConnSucc+off2,
                                    PortMapClient.DataBufLen_WaitingForHttpProxyConnSucc[i]);
                                off2+=PortMapClient.DataBufLen_WaitingForHttpProxyConnSucc[i];
                                PortMapClient.DataBufLen_WaitingForHttpProxyConnSucc[i]=0;
                            }
                        }
#ifdef DEBUGID
                        CFileOpr::getObj().writelocallog("TcpPortMap","TcpClt_RcvDataCB()_Inter hSocket=%d UserData=%d 发送等待缓冲区里的数据 dataLen=%d",hSocket,UserData,off2);
#endif
                    }
                }
                else 
                {
                    //连接失败
#ifdef DEBUGID
                    CFileOpr::getObj().writelocallog("TcpPortMap","TcpClt_RcvDataCB()_Inter hSocket=%d UserData=%d 连接失败",hSocket,UserData);
#endif
                    PortMapClient.ProxySvrConnStatus=Stru_PortMapClient::ProxySvr_ConnectFail;
                    CTcpPortMap::getObj().SetPortMapClient_Internal(hDataSocket,PortMapClient);
                    CTcpPortMap::getObj().DelPortMapClient_Internal(hDataSocket);
                    PortMapSvr.pTcpSvr->OffLineOneDataSocket(hDataSocket);
                    return;
                }
            }

        }

        //转发数据到DataSocket
        if(DataLen>off)
        {
            PortMapSvr.pTcpSvr->SendData(pData+off,DataLen-off,hDataSocket);
#ifdef DEBUGID
            CFileOpr::getObj().writelocallog("TcpPortMap","TcpClt_RcvDataCB() \t\thSocket=%d UserData=%d 转发数据 DataLen=%d hDataSocket=%d",hSocket,UserData,DataLen-off,hDataSocket);
#endif
        }
    }
}

void CTcpPortMap::TcpClient_DisconnCB(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
{
#ifdef DEBUGID
    CFileOpr::getObj().writelocallog("TcpPortMap","TcpClt_DisconnCB() \t\thSocket=%d UserData=%d",hSocket,UserData);
#endif
    Stru_SocketCallBackParam2 CBParam(0,LocalAddr,SvrAddr,hSocket,UserData);
    CPostMsgByEventParam::getObj().postMsgStrThread(m_idThread,(int)CBType_TcpClient_Disconn,0,string(""),&CBParam,sizeof(CBParam));
}
void CTcpPortMap::TcpClient_Disconn_Internal(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
{
    Stru_PortMapSvr PortMapSvr;
    Stru_PortMapClient PortMapClient;
    SOCKET hDataSocket=(SOCKET)UserData;
    if(hDataSocket!=INVALID_SOCKET&&
        hDataSocket!=0&&
        CTcpPortMap::getObj().GetPortMap2_Internal(hDataSocket,PortMapSvr)&&
        PortMapSvr.pTcpSvr!=NULL&&
        CTcpPortMap::getObj().GetPortMapClient_Internal(hDataSocket,PortMapClient)&&
        PortMapClient.pTcpClient!=NULL)
    {
        CTcpPortMap::getObj().DelPortMapClient_Internal(hDataSocket);
        PortMapSvr.pTcpSvr->OffLineOneDataSocket(hDataSocket);
    }
}

void CTcpPortMap::TcpClient_StartConnectCB(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
{
#ifdef DEBUGID
    CFileOpr::getObj().writelocallog("TcpPortMap","TcpClt_StartConnectCB() \thSocket=%d UserData=%d",hSocket,UserData);
#endif
    Stru_SocketCallBackParam2 CBParam(0,LocalAddr,SvrAddr,hSocket,UserData);
    CPostMsgByEventParam::getObj().postMsgStrThread(m_idThread,(int)CBType_TcpClient_StartConnect,0,string(""),&CBParam,sizeof(CBParam));
}
void CTcpPortMap::TcpClient_StartConnect_Internal(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
{
    map<SOCKET,Stru_PortMapClient>& MapClientSocket2PortMapClient=CTcpPortMap::getObj().m_MapClientSocket2PortMapClient;
    map<SOCKET,Stru_PortMapClient>::iterator it=MapClientSocket2PortMapClient.find(hSocket);
    SOCKET hDataSocket=(SOCKET)UserData;
    Stru_PortMapClient PortMapClient;
    if(it==MapClientSocket2PortMapClient.end()&&
        CTcpPortMap::getObj().GetPortMapClient_Internal(hDataSocket,PortMapClient))
    {
        MapClientSocket2PortMapClient[hSocket]=PortMapClient;
    }
}

void CTcpPortMap::TcpClient_ConnSuccessCB(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
{
#ifdef DEBUGID
    CFileOpr::getObj().writelocallog("TcpPortMap","TcpClt_ConnSuccessCB() \thSocket=%d UserData=%d",hSocket,UserData);
#endif
    Stru_SocketCallBackParam2 CBParam(0,LocalAddr,SvrAddr,hSocket,UserData);
    CPostMsgByEventParam::getObj().postMsgStrThread(m_idThread,(int)CBType_TcpClient_ConnSuccess,0,string(""),&CBParam,sizeof(CBParam));
}
void CTcpPortMap::TcpClient_ConnSuccess_Internal(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
{
}

void CTcpPortMap::TcpClient_ConnFailureCB(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
{
#ifdef DEBUGID
    CFileOpr::getObj().writelocallog("TcpPortMap","TcpClt_ConnFailureCB() hSocket=%d UserData=%d",hSocket,UserData);
#endif
    Stru_SocketCallBackParam2 CBParam(0,LocalAddr,SvrAddr,hSocket,UserData);
    CPostMsgByEventParam::getObj().postMsgStrThread(m_idThread,(int)CBType_TcpClient_ConnFailure,0,string(""),&CBParam,sizeof(CBParam));
}
void CTcpPortMap::TcpClient_ConnFailure_Internal(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
{
    Stru_PortMapSvr PortMapSvr;
    Stru_PortMapClient PortMapClient;
    SOCKET hDataSocket=(SOCKET)UserData;
    if(hDataSocket!=INVALID_SOCKET&&
        hDataSocket!=0&&
        CTcpPortMap::getObj().GetPortMap2_Internal(hDataSocket,PortMapSvr)&&
        PortMapSvr.pTcpSvr!=NULL&&
        CTcpPortMap::getObj().GetPortMapClient_Internal(hDataSocket,PortMapClient)&&
        PortMapClient.pTcpClient!=NULL)
    {
        CTcpPortMap::getObj().DelPortMapClient_Internal(hDataSocket);
        PortMapSvr.pTcpSvr->OffLineOneDataSocket(hDataSocket);
    }
}




unsigned long CTcpPortMap::ThreadWorker(void *arg)
{
	CTcpPortMap     *pObj;

	pObj=(CTcpPortMap*)arg;
	if(!pObj) return 0;

    //用于接收数据缓冲区
    int RecvDataBufSize=65536;
    char *pTmpRecvDataBuf=(char*)malloc(RecvDataBufSize);

	//**************************************************
	//开始消息处理循环
	//**************************************************
    MSG Msg;
    while(GetMessage(&Msg,NULL,0,0))
    {
        if(Msg.message==WM_COMMAND)
        {
            getObj().m_mutex.lock();

            eTcpCBType tcpCBType=(eTcpCBType)Msg.wParam;
            unsigned int EventParamID=(unsigned int)Msg.lParam;
            Stru_SocketCallBackParam2 CBParam;
            int RltEventDataLen,RltEventDataLen2;
            if(CEventParam::getObj().GetEventParam2(false,EventParamID,NULL,NULL,&CBParam,&RltEventDataLen,sizeof(CBParam),NULL,&RltEventDataLen2,0,NULL,NULL)&&
                RltEventDataLen==(int)sizeof(CBParam))
            {
                switch(tcpCBType)
                {
                    case CBType_TcpSvr_RcvData:
                    {
                        //TcpSvr收到数据
                        if(pTmpRecvDataBuf&&RltEventDataLen2>RecvDataBufSize)
                        {
                            RecvDataBufSize=RltEventDataLen2;
                            free(pTmpRecvDataBuf);
                            pTmpRecvDataBuf=(char*)malloc(RecvDataBufSize);
                        }
                        if(pTmpRecvDataBuf&&CEventParam::getObj().GetEventParam2(false,EventParamID,NULL,NULL,NULL,NULL,0,pTmpRecvDataBuf,&RltEventDataLen2,RecvDataBufSize,NULL,NULL))
                        {
                            getObj().TcpSvr_RcvData_Internal(pTmpRecvDataBuf,CBParam.DataLen,CBParam.LocalAddr,CBParam.RemoteAddr,CBParam.hSocket);
                        }
                        break;
                    }
                    case CBType_TcpSvr_Accept:
                    {
                        //TcpSvr接收到连接请求
                        getObj().TcpSvr_Accept_Internal(CBParam.LocalAddr,CBParam.RemoteAddr,CBParam.hSocket);
                        break;
                    }
                    case CBType_TcpSvr_Disconn:
                    {
                        //TcpSvr的数据连接断开
                        getObj().TcpSvr_Disconn_Internal(CBParam.LocalAddr,CBParam.RemoteAddr,CBParam.hSocket);
                        break;
                    }
                    case CBType_TcpClient_RcvData:
                    {
                        //TcpClient收到数据
                        if(pTmpRecvDataBuf&&RltEventDataLen2>RecvDataBufSize)
                        {
                            RecvDataBufSize=RltEventDataLen2;
                            free(pTmpRecvDataBuf);
                            pTmpRecvDataBuf=(char*)malloc(RecvDataBufSize);
                        }
                        if(pTmpRecvDataBuf&&CEventParam::getObj().GetEventParam2(false,EventParamID,NULL,NULL,NULL,NULL,0,pTmpRecvDataBuf,&RltEventDataLen2,RecvDataBufSize,NULL,NULL))
                        {
                            getObj().TcpClient_RcvData_Internal(pTmpRecvDataBuf,CBParam.DataLen,CBParam.LocalAddr,CBParam.RemoteAddr,CBParam.hSocket,CBParam.UserData);
                        }
                        break;
                    }
                    case CBType_TcpClient_Disconn:
                    {
                        //TcpClient连接断开
                        getObj().TcpClient_Disconn_Internal(CBParam.LocalAddr,CBParam.RemoteAddr,CBParam.hSocket,CBParam.UserData);
                        break;
                    }
                    case CBType_TcpClient_StartConnect:
                    {
                        //TcpClient开始连接
                        getObj().TcpClient_StartConnect_Internal(CBParam.LocalAddr,CBParam.RemoteAddr,CBParam.hSocket,CBParam.UserData);
                        break;
                    }
                    case CBType_TcpClient_ConnSuccess:
                    {
                        //TcpClient连接成功
                        getObj().TcpClient_ConnSuccess_Internal(CBParam.LocalAddr,CBParam.RemoteAddr,CBParam.hSocket,CBParam.UserData);
                        break;
                    }
                    case CBType_TcpClient_ConnFailure:
                    {
                        //TcpClient连接失败
                        getObj().TcpClient_ConnFailure_Internal(CBParam.LocalAddr,CBParam.RemoteAddr,CBParam.hSocket,CBParam.UserData);
                        break;
                    }
                }
            }

            getObj().m_mutex.unlock();

            CEventParam::getObj().DeleteEventParam(EventParamID);
        }
    }
	
    if(pTmpRecvDataBuf)
    {
        free(pTmpRecvDataBuf);
        RecvDataBufSize=0;
    }

	return 0;
}
