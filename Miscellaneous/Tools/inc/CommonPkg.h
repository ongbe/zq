#ifndef _CommonPkg_H_
#define _CommonPkg_H_

#include <string.h>

#define HeadID_UniPkg		0x7cab5caa
#define TailID_UniPkg		0xba55ca81

typedef unsigned int TAIL;

enum EnumCompressFlag
{
	CompressFlag_NULL=0                         //don't compress
};

#pragma pack(push)
#pragma pack(1)

struct Stru_UniPkgHead
{
	Stru_UniPkgHead()
	{
		memset(&(headid),0,sizeof(Stru_UniPkgHead));
		headid=HeadID_UniPkg;
		compressflag=CompressFlag_NULL;
	}
	Stru_UniPkgHead(int Len,unsigned int Cmdid,unsigned int Moduleid,unsigned int Seq,unsigned int Subseq=0,
		unsigned int Userdata1=0,unsigned int Userdata2=0,unsigned int Userdata3=0,unsigned int Userdata4=0)
	{
		Set(Len,Cmdid,Moduleid,Seq,Subseq,Userdata1,Userdata2,Userdata3,Userdata4);
	}
	void Set(int Len,unsigned int Cmdid,unsigned int Moduleid,unsigned int Seq,unsigned int Subseq=0,
		unsigned int Userdata1=0,unsigned int Userdata2=0,unsigned int Userdata3=0,unsigned int Userdata4=0)
	{
		headid=HeadID_UniPkg;
		compressflag=CompressFlag_NULL;
		len=Len;
		cmdid=Cmdid;
		moduleid=(unsigned int)Moduleid;
		seq=Seq;
		subseq=Subseq;
		userdata1=Userdata1;
		userdata2=Userdata2;
		userdata3=Userdata3;
		userdata4=Userdata4;
	};
	//tell you whether a PkgStream is a valid package, if successful, *pUsedLengthByPkg return length used by current package
	static bool IsValidPkg(const void* pPkgStream,int StreamLen,int* pLengthUsedByPkg=0)
	{
		if(!pPkgStream||StreamLen<(int)sizeof(Stru_UniPkgHead)+4)
			return false;

		const Stru_UniPkgHead *pPkgHead=(const Stru_UniPkgHead*)pPkgStream;
		if(pPkgHead->headid!=HeadID_UniPkg||
				StreamLen<(int)sizeof(Stru_UniPkgHead)+pPkgHead->len+4||
				(*(const unsigned int*)((const char*)pPkgStream+sizeof(Stru_UniPkgHead)+pPkgHead->len))!=TailID_UniPkg)
			return false;

	    if(pLengthUsedByPkg) (*pLengthUsedByPkg)=(int)sizeof(Stru_UniPkgHead)+pPkgHead->len+4;
		return true;
	}

	//tell you whether a PkgStream is an invalid package. Typically, you can skip a character if the stream was started with an invalid package head
	static bool IsInvalidPkgHead(const void* pPkgStream,int StreamLen)
	{
		if(!pPkgStream)
			return true;

		const Stru_UniPkgHead *pPkgHead=(const Stru_UniPkgHead*)pPkgStream;
	    if(StreamLen>=(int)sizeof(pPkgHead->headid)&&pPkgHead->headid!=HeadID_UniPkg)
	        return true;

		if(StreamLen>=(int)sizeof(Stru_UniPkgHead)&&
            StreamLen>=(int)sizeof(Stru_UniPkgHead)+pPkgHead->len+4&&
			(*(const unsigned int*)((const char*)pPkgStream+sizeof(Stru_UniPkgHead)+pPkgHead->len))!=TailID_UniPkg)
	        return true;

	    return false;
	}

	unsigned int headid;			//id of UniPkgHead, always be  HeadID_UniPkg
	int len;						//length of package body content, in bytes
	int compressflag;               //compress method that the package body content used. the value was define in EnumCompressFlag
	int prioritylevel;              //level of priority. the higher the value, the higher priority
	unsigned int cmdid;				//cmdid defined by business module
	unsigned int moduleid;			//moduleid that create the package, must be value of EnumModuleID
	unsigned int seq;				//defined by business module
	unsigned int subseq;			//defined by business module, commonly be 0 if you don't use it.
	unsigned int userdata1;			//use it as require
	unsigned int userdata2;
	unsigned int userdata3;
	unsigned int userdata4;
	unsigned int crc32;
};




struct Stru_Heartbeat
{
	Stru_UniPkgHead head;
	TAIL tail;
};

struct Stru_Licence
{
	char szAppID[33];
	char szLicence[34];
};

struct Stru_ServerAddr
{
	unsigned int	TCPIP;
	unsigned short	TCPPort;
	unsigned int	UDPIP;
	unsigned short	UDPPort;
};

//下面是通用命令字，高位为0x7fff。应用程序自行定义命令字时，避开高位0x7fff的值
#ifndef CMDID_HEARTBEAT
#define CMDID_HEARTBEAT		    0x7fff0001	    //心跳命令
#endif

#ifndef CMDID_HEARTBEATRSP
#define CMDID_HEARTBEATRSP	    0x7fff0002	    //心跳答复
#endif

#ifndef CMDID_LICENCE_LIST_REQ
#define CMDID_LICENCE_LIST_REQ	0x7fff0003	    //请求licence列表
#endif

#ifndef CMDID_LICENCE_LIST_RSP
#define CMDID_LICENCE_LIST_RSP	0x7fff0004	    //licence列表返回
#endif

#ifndef CMDID_SERVER_ADDR_REQ
#define CMDID_SERVER_ADDR_REQ	0x7fff0005	    //请求licence服务IP，端口号
#endif

#ifndef CMDID_SERVER_ADDR_RSP
#define CMDID_SERVER_ADDR_RSP	0x7fff0006	    //返回licence服务IP，端口号
#endif

#ifndef CMDID_QryFile
#define CMDID_QryFile           0x7fff0101      //向服务器请求文件，包体为文件名，可以带相对路径
#endif

#ifndef CMDID_RspQryFile
#define CMDID_RspQryFile        0x7fff0102      //服务器返回文件，包体为文件名(\0结尾) + 文件内容
#endif

#ifndef CMDID_RspQryFileFail
#define CMDID_RspQryFileFail    0x7fff0103      //服务器返回请求文件失败，包体为文件名(\0结尾) + 文件内容
#endif

#pragma pack(pop)

#endif
