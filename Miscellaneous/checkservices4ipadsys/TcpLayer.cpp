// ConfigMgr.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <windows.h>
#include "TcpLayer.h"
#include "FileOpr.h"
#include "Tools_Ansi.h"
#include "CommonPkg.h"
#include "TcpClientWorker.h"
#include <windows.h>
#include "psapi.h"

#include <string>
using namespace std;



#ifdef _DEBUG
	#pragma comment(lib, "ToolsD.lib")
#else
	#pragma comment(lib, "Tools.lib")
#endif

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")


#pragma unmanaged


//内部变量
bool CTcpLayer::m_bStop=false;
Ceasymutex CTcpLayer::m_mutex;
void* CTcpLayer::m_pTcpClient=NULL;
char* CTcpLayer::m_pRcvBuf=NULL;
int CTcpLayer::m_RcvBufLen=0;
char* CTcpLayer::m_pSndBuf=NULL;
int CTcpLayer::m_SndBufLen=0;
time_t CTcpLayer::m_ReceivePkgTime=0;


void CTcpLayer::Init()
{
	m_mutex.lock();

	createtcpclient();

	m_mutex.unlock();
}

void CTcpLayer::Release()
{
	m_mutex.lock();
	CFileOpr::writelocallog("TcpLayer.log","Release，m_pTcpClient=%x ",
							m_pTcpClient);

	if(m_pTcpClient)
	{
		delete m_pTcpClient;
		m_pTcpClient=NULL;
	}
	m_mutex.unlock();
}

//系统退出前，先Stop，停止分发，再关闭各模块窗口。
void CTcpLayer::Stop()
{
	m_mutex.lock();
	m_bStop=true;
	m_mutex.unlock();
}


//从tcp得到数据后，调用此回调函数进行处理
void CTcpLayer::RecvDataCBFunc(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
	if(m_bStop) return;

	m_mutex.lock();
	if(!m_pRcvBuf) m_pRcvBuf=(char*)malloc(TcpRecvBufSize);
	if(!m_pRcvBuf||m_RcvBufLen+DataLen>TcpRecvBufSize)
	{
		CFileOpr::writelocallog("TcpLayer.log","RecvDataCBFunc缓冲区不足，m_pRcvBuf=%x m_RcvBufLen=%d DataLen=%d TcpRecvBufSize=%d localaddr=(%u):%u svraddr=(%u):%u, hSocket=%x",
								m_pRcvBuf,m_RcvBufLen,DataLen,TcpRecvBufSize,LocalAddr.IP,LocalAddr.Port,SvrAddr.IP,SvrAddr.Port,hSocket);
	}
	else
	{
		memcpy(m_pRcvBuf+m_RcvBufLen,pData,DataLen);
		m_RcvBufLen+=DataLen;
	}
	m_mutex.unlock();


    //解析并处理接收到的数据
    ParseRecvPackageInWindowThread();
}

//断开tcp连接后，调用此回调函数进行处理
void CTcpLayer::DisconnCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
	CFileOpr::writelocallog("TcpLayer.log","DisconnCBFunc，localaddr=(%u):%u svraddr=(%u):%u, hSocket=%x",
							LocalAddr.IP,LocalAddr.Port,SvrAddr.IP,SvrAddr.Port,hSocket);

	m_mutex.lock();
	m_mutex.unlock();
}

//开始调用connect()后，调用此函数通知应用层
void CTcpLayer::StartConnCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
	CFileOpr::writelocallog("TcpLayer.log","StartConnCBFunc，localaddr=(%u):%u svraddr=(%u):%u, hSocket=%x",
							LocalAddr.IP,LocalAddr.Port,SvrAddr.IP,SvrAddr.Port,hSocket);
	m_mutex.lock();
	m_mutex.unlock();
}

//连接成功后，调用此回调函数进行处理
void CTcpLayer::ConnSuccCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
	CFileOpr::writelocallog("TcpLayer.log","StartConnCBFunc，localaddr=(%u):%u svraddr=(%u):%u, hSocket=%x",
							LocalAddr.IP,LocalAddr.Port,SvrAddr.IP,SvrAddr.Port,hSocket);
	
	char sendData[256]=""; /*= "01580101 00000001 2af90000 00280000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00006164 6d696e20 20202020 20202020 20202020 20203132 33343520 20202020 20202020 20202020 2020";
	int dataLen = strlen(loginReq);*/
	m_mutex.lock();
    int dataLen = getloginReqData(sendData);
	Senddata( sendData, dataLen );
	memset(sendData,0,sizeof(sendData));
	dataLen = getHQReqData(sendData);
    Senddata( sendData, dataLen );
	m_mutex.unlock();
}

//连接失败后，调用此回调函数进行处理
void CTcpLayer::ConnFailCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
	CFileOpr::writelocallog("TcpLayer.log","ConnFailCBFunc，localaddr=(%u):%u svraddr=(%u):%u, hSocket=%x",
							LocalAddr.IP,LocalAddr.Port,SvrAddr.IP,SvrAddr.Port,hSocket);

	m_mutex.lock();
	m_mutex.unlock();
}

//定期调用的回调函数，一般每秒会调用一次，可以用于检查心跳超时等
void CTcpLayer::OnTimeCBFunc(void)
{
	m_mutex.lock();
	time_t cursecs=time(NULL);
	if(cursecs-m_ReceivePkgTime>=10)
	{}

	//发送尚未发送的数据
	if(!m_bStop)
		senddata_internal();


	m_mutex.unlock();
}

//创建tcpclient
void CTcpLayer::createtcpclient(void)
{
	m_mutex.lock();
	if(!m_bStop&&!m_pTcpClient)
	{
		string svrIP("114.255.138.141"),localIP("0");
		DWORD dwsvrIP,dwlocalIP;
		unsigned short svrPort=18611,localPort=0;

		dwsvrIP=CTools_Ansi::ConvertStrIP(svrIP.c_str());
		if(dwsvrIP!=0&&svrPort!=0)
		{
			dwlocalIP=CTools_Ansi::ConvertStrIP(localIP.c_str());

			m_pTcpClient=new CTcpClientWorker(Stru_IPAddress(dwlocalIP,localPort),Stru_IPAddress(dwsvrIP,svrPort),
				 RecvDataCBFunc,
				 DisconnCBFunc,
				 StartConnCBFunc,
				 ConnSuccCBFunc,
				 ConnFailCBFunc,
				 OnTimeCBFunc,
				 SocketBufSize,SocketBufSize);

			if(Debug_TcpLayer||m_pTcpClient)
				CFileOpr::writelocallog("TcpLayer.log","创建TcpClient，localaddr=%s(%u):%u svraddr=%s(%u):%u, m_pTcpClient=%x",
                localIP.c_str(),dwlocalIP,localPort,svrIP.c_str(),dwsvrIP,svrPort,m_pTcpClient);
		}
	}
	m_mutex.unlock();
}


//发送数据
bool CTcpLayer::Senddata(const char* pData,int DataLen)
{
	if(m_bStop) return false;

	bool brlt=false;
	m_mutex.lock();
    brlt=Senddata_withoutlock(pData,DataLen);
	m_mutex.unlock();
	return brlt;
}


//发送数据，不加锁
bool CTcpLayer::Senddata_withoutlock(const char* pData,int DataLen)
{
    bool brlt=false;
	if(!m_pSndBuf) m_pSndBuf=(char*)malloc(TcpSendBufSize);
	if(!m_pSndBuf||m_SndBufLen+DataLen>TcpSendBufSize)
	{
		CFileOpr::writelocallog("TcpLayer.log","senddata缓冲区不足，m_pSndBuf=%x m_SndBufLen=%d DataLen=%d TcpSendBufSize=%d m_pTcpClient=%x hSocket=%x ConnStatus=%d",
			m_pSndBuf,m_SndBufLen,DataLen,TcpSendBufSize,m_pTcpClient,
			m_pTcpClient?((CTcpClientWorker*)m_pTcpClient)->getSocket():-1,
			m_pTcpClient?((CTcpClientWorker*)m_pTcpClient)->getConnStatus():0);
		brlt=false;
	}
	else
	{
		memcpy(m_pSndBuf+m_SndBufLen,pData,DataLen);
		m_SndBufLen+=DataLen;
		brlt=true;
		senddata_internal();
	}
    return brlt;
}


//发送数据
void CTcpLayer::senddata_internal(void)
{
	if(m_pSndBuf&&
		!m_bStop&&m_SndBufLen>0&&
		m_pTcpClient&&
		((CTcpClientWorker*)m_pTcpClient)->getSocket()>0&&
		((CTcpClientWorker*)m_pTcpClient)->getConnStatus()==TcpClientConnStatus_Connected)
	{
		int sentlen=((CTcpClientWorker*)m_pTcpClient)->SendData(m_pSndBuf,m_SndBufLen);
		if(sentlen>0&&sentlen<=m_SndBufLen)
		{
			memmove(m_pSndBuf,m_pSndBuf+sentlen,m_SndBufLen-sentlen);
			m_SndBufLen-=sentlen;
		}
	}
}



//在主线程中调用，用于解析并分发接收数据包。
//接收到数据后，会向主线程发送WM_COMMAND命令，主线程再调用此函数进行解析和分发。
//这样在窗口线程中处理数据，会比较安全
void CTcpLayer::ParseRecvPackageInWindowThread(void)
{
	if(m_bStop) return;

	m_mutex.lock();

	if(!m_pRcvBuf) m_pRcvBuf=(char*)malloc(TcpRecvBufSize);

	if(m_pRcvBuf)
	{
        //m_pRcvBuf是接收到的数据缓冲区指针
        //m_RcvBufLen是接收到的数据长度
        int ProcessedLen=0;

		m_ReceivePkgTime=time(NULL);
		//初始化包头
		DataHead	rHead;
		memset(&rHead,0,sizeof(rHead));
		g_Mem2Head(&rHead, m_pRcvBuf);
		CFileOpr::writelocallog("TcpLayer.log","ParseRecvPackageInWindowThread，m_usDataType=%u,m_ulDataLength=%u",
							rHead.m_usDataType,rHead.m_ulDataLength);
		if(rHead.m_usDataType == 11001 && rHead.m_rStatus.s.m_cError == 0 && rHead.m_pcReserve[0] == 1)//登录返回
		{
			
			char data[1000];
			memset(&data,0,sizeof(data));
			memcpy(&data, m_pRcvBuf+50, 64);
			CFileOpr::writelocallog("TcpLayer.log","登陆成功：%s",data);
		}


		ProcessedLen = _DataHeadLength_ + rHead.m_ulDataLength;
		//ProcessedLen是已经处理过的数据长度，需要从m_pRcvBuf中删掉
		if(ProcessedLen>0&&ProcessedLen<=m_RcvBufLen)
		{
			memmove(m_pRcvBuf,m_pRcvBuf+ProcessedLen,m_RcvBufLen-ProcessedLen);
			m_RcvBufLen-=ProcessedLen;
		}
	}

	m_mutex.unlock();
}
int CTcpLayer::getloginReqData(char* loginReq)
 {
	    long		lCount = 0;
       //复制头信息
		DataHead rHead;
		//初始化包头信息
	    g_InitHead(&rHead);
	//	memcpy(&rHead, &m_rHead, _DataHeadLength_);
        //初始写入偏移量为包头之后的位置
		lCount = _DataHeadLength_;

      //用户名密码
		lCount += g_String2Mem(loginReq+lCount, "admin", 20);
		lCount += g_String2Mem(loginReq+lCount, "12345", 20);
  
        //计算包体长度记入包头
		rHead.m_ulDataLength = lCount - _DataHeadLength_;
		rHead.m_usDataType = 11001;
        //写入包头
		g_Head2Mem(loginReq, &rHead);//复制头信息
		return lCount;
		
 }

int CTcpLayer::getHQReqData(char* pData)
 {
	    long		lCount = 0;
       //复制头信息
		DataHead rHead;
		//初始化包头信息
	    g_InitHead(&rHead);
	//	memcpy(&rHead, &m_rHead, _DataHeadLength_);
        //初始写入偏移量为包头之后的位置
		lCount = _DataHeadLength_;
		int index=1;
		int count=12;

		////开始位置
		//lCount += g_Int2Mem(pData+lCount,&index , 2);
		////结束位置
		//lCount += g_Int2Mem(pData+lCount,&index , 2);
		////获取数量
		//lCount += g_Int2Mem(pData+lCount,&count , 2);
		//推送
		lCount += g_String2Mem(pData+lCount, "0", 1);		
        
        lCount += g_Int2Mem(pData+lCount,&count , 2);
		long data1=3413060;
		long data2=2213051;
		//行情索引
		lCount += g_Int2Mem(pData+lCount,&data1, 4);
		lCount += g_Int2Mem(pData+lCount,&data2, 4);
	

		//计算包体长度记入包头
		rHead.m_ulDataLength = lCount - _DataHeadLength_;
		rHead.m_usDataType = 30002;
        //写入包头
		g_Head2Mem(pData, &rHead);//复制头信息
		return lCount;
		
 }

long CTcpLayer::g_Head2Mem(char* pcData, DataHead* pHead)
{
    //按照类型依次将DataHead的各个成员写入内存
    //每次写入后记录写入内存的偏移量用于后续写入
	long lCount = 0;
	lCount += g_Int2Mem(pcData+lCount, &(pHead->m_usHeadID), 2);
	lCount += g_Int2Mem(pcData+lCount, &(pHead->m_usVersion), 2);
	lCount += g_Int2Mem(pcData+lCount, &(pHead->m_ulDataID), 4);
	lCount += g_Int2Mem(pcData+lCount, &(pHead->m_usDataType), 2);
	lCount += g_Int2Mem(pcData+lCount, &(pHead->m_ulDataLength), 4);
	lCount += g_Int2Mem(pcData+lCount, &(pHead->m_usEncryptType), 2);
	lCount += g_Int2Mem(pcData+lCount, &(pHead->m_ulEncryptLength), 4);
	lCount += g_Int2Mem(pcData+lCount, &(pHead->m_usSignatureType), 2);
	lCount += g_Int2Mem(pcData+lCount, &(pHead->m_ulSignatureLength), 4);
	lCount += g_Int2Mem(pcData+lCount, &(pHead->m_ulSrcCode), 4);
	lCount += g_Int2Mem(pcData+lCount, &(pHead->m_ulDestCode), 4);
    //char型数据无字节序，直接内存复制即可
	memcpy(pcData+lCount, &(pHead->m_rStatus.S.m_cStatus), 1);
	lCount += 1;
    //char型数据无字节序，直接内存复制即可
	memcpy(pcData+lCount, &(pHead->m_pcReserve), 15);
	lCount += 15;
	return lCount;
}
long  g_lDataID = 0;
void CTcpLayer::g_InitHead(DataHead* pHead)
{
    //依次初始化DataHead各个成员
	pHead->m_usHeadID = 0x0158;
	pHead->m_usVersion = 0x0201;
    //m_ulDataID为数据包的唯一标识，不允许重复
	pHead->m_ulDataID = g_lDataID++;
    if(LONG_MAX == g_lDataID)
    {
        g_lDataID = 0;
    }
	pHead->m_usDataType = 0;
	pHead->m_ulDataLength = 0;
	pHead->m_usEncryptType = 0;
	pHead->m_ulEncryptLength = 0;
	pHead->m_usSignatureType = 0;
	pHead->m_ulSignatureLength = 0;
	pHead->m_ulSrcCode = 0;
	pHead->m_ulDestCode = 0;
	pHead->m_rStatus.S.m_cStatus = 0;
	memset(pHead->m_pcReserve, 0, 15);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// 将字符串写入内存
/// @param pcBuffer[out]：要写入的内存地址
/// @param str[in]：用于写的字符串
/// @param nBuffer[in]：写入长度
/// @retval 写入长度
///
//------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////
long CTcpLayer::g_String2Mem(char* pcBuffer, std::string str, int nBuffer)
{
	int nStr = str.length();
    //按照指定长度写入，字符串超长自动截取
	if (nBuffer<nStr)
    {
		nStr = nBuffer;
    }
    //写入字符串
    int i=0;
	for (i=0; i<nStr; i++)
    {
		pcBuffer[i] = str[i];
    }
    //字符串如果不够指定长度则后边追加足够的空格
	for (; i<nBuffer; i++)
    {
		pcBuffer[i] = ' ';
    }
	return long(nBuffer);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// 将1个整数写入内存
/// @param pcMem[out]：要写入的内存
/// @param pValue[in]：要写的整数指针
/// @param s[in]：写入长度
/// @retval 写入长度

//--------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////
long CTcpLayer::g_Int2Mem(char* pcMem, void* pValue, int s)
{
    //4是long的长度
	if (s==4)
	{
        //取得要写的整数
		long* plValue = (long*)pValue;
        //整数网络字节序转换
		long lTrans = (long)htonl(*plValue);
        //将整数写入内存
		memcpy(pcMem, &lTrans, 4);
	}
    //只要不是long型就认为是short型
    //2是short的长度
	else
	{
        //取得要写的整数
		short* psValue = (short*)pValue;
        //整数网络字节序转换
		short sTrans = (short)htons(*psValue);
        //将整数写入内存
		memcpy(pcMem, &sTrans, 2);
	}
	return s;
}

long CTcpLayer::g_Mem2Head(DataHead* pHead, char* pcData)
{
    //按照类型依次读取DataHead的各个成员
    //每次读取后记录读取内存的偏移量用于后续读取
	long lCount = 0;
	lCount += g_Mem2Int(&(pHead->m_usHeadID), pcData+lCount, 2);
	lCount += g_Mem2Int(&(pHead->m_usVersion), pcData+lCount, 2);
	lCount += g_Mem2Int(&(pHead->m_ulDataID), pcData+lCount, 4);
	lCount += g_Mem2Int(&(pHead->m_usDataType), pcData+lCount, 2);
	lCount += g_Mem2Int(&(pHead->m_ulDataLength), pcData+lCount, 4);
	lCount += g_Mem2Int(&(pHead->m_usEncryptType), pcData+lCount, 2);
	lCount += g_Mem2Int(&(pHead->m_ulEncryptLength), pcData+lCount, 4);
	lCount += g_Mem2Int(&(pHead->m_usSignatureType), pcData+lCount, 2);
	lCount += g_Mem2Int(&(pHead->m_ulSignatureLength), pcData+lCount, 4);
	lCount += g_Mem2Int(&(pHead->m_ulSrcCode), pcData+lCount, 4);
	lCount += g_Mem2Int(&(pHead->m_ulDestCode), pcData+lCount, 4);
    //char型数据无字节序，直接内存复制即可
	memcpy(&(pHead->m_rStatus.S.m_cStatus), pcData+lCount, 1);
	lCount += 1;
    //char型数据无字节序，直接内存复制即可
	memcpy(&(pHead->m_pcReserve), pcData+lCount, 15);
	lCount += 15;
	return lCount;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// 从内存读取1个整数
/// @param pValue[out]：读取的结果
/// @param pcMem[in]：要读取的内存地址
/// @param s[in]：读取长度
/// @retval 读取长度
//--------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////
long CTcpLayer::g_Mem2Int(void* pValue, char* pcMem, int s)
{
    //4是long的长度
	if (s==4)
	{
		long* plValue = (long*)pValue;		
		long lTrans;
        //按long型读取一段long长度的内存
		memcpy(&lTrans, pcMem, 4);
        //将读取结果放入传出的指针对象
		*plValue = (long)(htonl(lTrans));
	}
    //只要不是long型就认为是short型
    //2是short的长度
	else
	{
		short* psValue = (short*)pValue;		
		short sTrans;
        //按short型读取一段short长度的内存
		memcpy(&sTrans, pcMem, 2);
        //将读取结果放入传出的指针对象
		*psValue = (short)(htons(sTrans));
	}
	return s;
}

typedef BOOL	(__stdcall *EnumProcessesFunc)(DWORD *lpidProcess,DWORD cb,DWORD *cbNeeded);
typedef DWORD	(__stdcall *GetModuleBaseNameFunc)(HANDLE hProcess,HMODULE hModule,LPTSTR lpBaseName,DWORD nSize);
typedef BOOL	(__stdcall *EnumProcessModulesFunc)(HANDLE hProcess,HMODULE *lphModule,DWORD cb,LPDWORD lpcbNeeded);
HANDLE	m_hPsapi=NULL;													//LoadLibrary("psapi",)的句柄
HANDLE  m_pEnumProcesses=NULL;
HANDLE			m_pEnumProcessModules=NULL;
HANDLE  m_pGetModuleBaseName=NULL;
 // 找进程ID：
//	判断是否存在指定模块名称的进程
//	iModuleName：模块名称
//	pProcessID：保存模块对应的进程的ID
//	返回值：TRUE 进程存在；FALSE 进程不存在
BOOL CTcpLayer::IsProcessExist(const char *iModuleName,DWORD *pProcessID)
{
	char			ModuleName[256];
	char			iBaseName[256];
	char			BaseName[256];
	int				i,iLen;
	DWORD			ProcessID[256];
	BOOL			bRlt,bExist;
	DWORD			cbNeeded,cbReturned;
	HANDLE			hProcess;
	HMODULE			hModule;
																				//先分离出单纯的模块文件名称
	bRlt=CTools_Ansi::GetPureFileName(iModuleName,ModuleName,sizeof(ModuleName));
	if(!bRlt) return FALSE;
	if(!ModuleName[0]) return FALSE;

	m_hPsapi=(HMODULE)LoadLibrary("psapi");
	if(m_hPsapi)
	{
		m_pEnumProcesses=(HANDLE)GetProcAddress((HMODULE)m_hPsapi,"EnumProcesses");   
		m_pEnumProcessModules=(HANDLE)GetProcAddress((HMODULE)m_hPsapi,"EnumProcessModules");   

		m_pGetModuleBaseName=(HANDLE)GetProcAddress((HMODULE)m_hPsapi,"GetModuleBaseNameA");   

	}


	//if(!m_pEnumProcesses) return 0;
	//if(!m_pGetModuleBaseName) return 0;
	//if(!m_pEnumProcessModules) return FALSE;
																				//枚举进程
	memset((char*)ProcessID,0,sizeof(ProcessID));
	bRlt=((EnumProcessesFunc)m_pEnumProcesses)(ProcessID,sizeof(ProcessID),&cbNeeded);
	if(!bRlt) return FALSE;

	bExist=FALSE;
	for(i=0;!bExist&&i<(int)(cbNeeded/sizeof(DWORD));i++)
	{																			//获取进程句柄
		hProcess=OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,ProcessID[i]);
		if(hProcess)
		{																		
			bRlt=((EnumProcessModulesFunc)m_pEnumProcessModules)(hProcess,&hModule,sizeof(hModule),&cbReturned);
			if(bRlt)
			{
				iLen=((GetModuleBaseNameFunc)m_pGetModuleBaseName)(hProcess,hModule,iBaseName,sizeof(iBaseName)-1);
				if(iLen>0)
				{
					memset(BaseName,0,sizeof(BaseName));
					bRlt=CTools_Ansi::GetPureFileName(iBaseName,BaseName,sizeof(BaseName)-1);
					if(stricmp(ModuleName,BaseName)==0) 
					{
						if(pProcessID) *pProcessID=ProcessID[i];
						bExist=TRUE;
					}
				}
			}
			CloseHandle(hProcess);
		}
	}

	return bExist;
}
