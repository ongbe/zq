#pragma once
//请在这个头文件中定义客户端请求服务器的command ID

//每个客户端发向服务器的请求包需要在包头的userdata1的地方填写userid，登陆请求除外

#define  MAKE_CMDID(_base,_offset) \
	((int)( (int)(_base) << 16 ) + (int)(_offset))



#define  CMDID_RM_BASE_HIWORD_SERVER_TCP              0x0000 //ServerTcp服务高位CMDID

//-----------------------------------------------------------------------------------
//	下面定义常量
//-----------------------------------------------------------------------------------
//通知订阅的线程，有消息到达，lParam=EventParamID，EventParam的pEventData是包数据，Stru_UniPkgHead + 数据 + 包尾, userint=hSocket
#ifndef WndCmd_YourPkgArrival2
#define WndCmd_YourPkgArrival2		0x3bbd
#endif

#ifndef WndCmd_YourEventArrival2
#define WndCmd_YourEventArrival2		0x3bbe
#endif

#ifndef WM_YourEventArrival2
#define WM_YourEventArrival2		0x8f01
#endif

#define MAX_SQL_LENGTH			4000	//SQL语句最大长度
#define MAX_USABLE_SQL_LENGTH	3999

//-----------------------------------------------------------------------------------
//	下面定义命令字
//-----------------------------------------------------------------------------------
#define CMDID_Heartbeat					MAKE_CMDID(CMDID_RM_BASE_HIWORD_SERVER_TCP,0x2000)				//客户端每隔20s向服务器发送心跳，内容为空
#define CMDID_HeartbeatRsp				MAKE_CMDID(CMDID_RM_BASE_HIWORD_SERVER_TCP,0x2001)				//服务器向客户端返回心跳响应，内容为空

#define CMDID_SvrTcpDisconn				MAKE_CMDID(CMDID_RM_BASE_HIWORD_SERVER_TCP,0x2100)				//有tcp连接断开 pkghead的userdata1为断开的hSocket
#define CMDID_SvrTcpCon				    MAKE_CMDID(CMDID_RM_BASE_HIWORD_SERVER_TCP,0x2111)				//有tcp连接 pkghead的userdata1为断开的hSocket


#define CMDID_SvrGet					MAKE_CMDID(CMDID_RM_BASE_HIWORD_SERVER_TCP,0x3100)				//获取文件
#define CMDID_SvrGetRsp				    MAKE_CMDID(CMDID_RM_BASE_HIWORD_SERVER_TCP,0x3111)				//获取文件返回


struct sGet
{
	char mPath[256];
};