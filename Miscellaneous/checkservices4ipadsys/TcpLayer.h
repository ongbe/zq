#ifndef _TCPLAYER_H_
#define _TCPLAYER_H_

#include "SocketBase.h"

#include <map>
#include <string>
using namespace std;
//数据头信息状态
union DataHeadStatus
{
	struct
	{
		unsigned char m_cStatus;
	} S;
	struct
	{
		unsigned char m_cError	:1;
		unsigned char m_cLog	:1;
		unsigned char bOther	:6;
	} s;
};
//数据头信息结构体
typedef struct tagDataHead
{
	unsigned short m_usHeadID;
	unsigned short m_usVersion;
	unsigned long m_ulDataID;
    // 数据类型, 0:删除m_ulDataID对应的数据
	unsigned short m_usDataType;
    // 数据包长度(不算包头)
	unsigned long m_ulDataLength;
    // 加密方法, 0:无加密;
	unsigned short m_usEncryptType;
    // 加密长度.
	unsigned long m_ulEncryptLength;
    // 签名方法, 0:无压缩,加密;
	unsigned short m_usSignatureType;
    // 签名长度.
	unsigned long m_ulSignatureLength;
	unsigned long m_ulSrcCode;
	unsigned long m_ulDestCode;
    // 标志位.
	DataHeadStatus m_rStatus;
	char m_pcReserve[15];
}DataHead;

//数据头信息长度
#define _DataHeadLength_		50


//TcpClient 收发缓冲区的大小
#define SocketBufSize (10*1024*1024)

//Tcp接收后等待处理的数据缓冲区大小
#define TcpRecvBufSize (100*1024*1024)

//Tcp等待发送的数据缓冲区大小
#define TcpSendBufSize (10*1024*1024)




//通知主线程有数据到达的WM_COMMAND消息的wParam命令
#define WndCmd_DataArrival 0x3bbb
//通知模块窗口有消息到达，lParam是消息数据指针，Stru_UniPkgHead + 数据
#define WndCmd_YourPkgArrival 0x3bbc

#define Debug_TcpLayer 1

#ifdef WIN32
#pragma managed(push,off)
#endif 

class CTcpLayer
{
public:
	static void Init();
	static void Release();
	static void Stop();

	//发送数据
	static bool Senddata(const char* pData,int DataLen);
	//发送数据，不加锁
	static bool Senddata_withoutlock(const char* pData,int DataLen);



private:
	//从tcp得到数据后，调用此回调函数进行处理
	static void	RecvDataCBFunc(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);

	//断开tcp连接后，调用此回调函数进行处理
	static void	DisconnCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);

	//开始调用connect()后，调用此函数通知应用层
	static void	StartConnCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);

	//连接成功后，调用此回调函数进行处理
	static void	ConnSuccCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);

	//连接失败后，调用此回调函数进行处理
	static void	ConnFailCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);

	//定期调用的回调函数，一般每秒会调用一次，可以用于检查心跳超时等
	static void OnTimeCBFunc(void);

	//创建tcpclient
	static void createtcpclient(void);

	//发送数据
	static void senddata_internal(void);

	//用于解析并分发接收数据包。
	static void ParseRecvPackageInWindowThread(void);
	//得到登录数据请求包
	static  int getloginReqData(char* loginReq);
	//得到行情数据请求包
	static int getHQReqData(char* pData);

	////从内存获取DataHead
	static long g_Mem2Head(DataHead*, char*);
	//将DataHead写入内存
	static long g_Head2Mem(char*, DataHead*);
	////从内存读取1个整数
	static long g_Mem2Int(void*, char*, int);
	////将1个整数写入内存
	static long g_Int2Mem(char*, void*, int);
	//初始化DataHead
	static void g_InitHead(DataHead*);
	////从内存读取一段字符串
	//long g_Mem2String(CString&, char*, int);
	////从内存读取一段字符串
	//CString g_Mem2String(char*, int);
	////将字符串写入内存
	static long g_String2Mem(char*, std::string, int);
	//找进程ID
	BOOL IsProcessExist(const char *iModuleName,DWORD *pProcessID);

private:
	static Ceasymutex m_mutex;
	static void* m_pTcpClient;
	static bool m_bStop;

	static char* m_pRcvBuf;
	static int m_RcvBufLen;
	static char* m_pSndBuf;
	static int m_SndBufLen;
	static time_t m_ReceivePkgTime;
 
    //static DataHead m_rHead;// 
	//上次发送心跳时间
	static time_t m_HeartbeatTime;
};

#ifdef WIN32
#pragma managed(pop)
#endif 

#endif


