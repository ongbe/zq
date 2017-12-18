//*************************************************************************************************
//	定义一种包结构
//	Stru_UniPkgHead2+PkgBody+CRC32(4)+Tail(4)
//	CRC32为(Stru_UniPkgHead2+PkgBody)内容的4字节(unsigned int)算术和。PkgBody长度不是4字节边界的，用0填充
//*************************************************************************************************

#ifndef _CommonPkg2_H_
#define _CommonPkg2_H_



#include <string.h>

#define HeadID_UniPkg2		0x7cab5cab
#define TailID_UniPkg2		0xba55ca82


enum EnumCompressFlag2
{
	CompressFlag2_NULL=0                         //don't compress
};

#pragma pack(push)
#pragma pack(1)

struct Stru_UniPkgHead2
{
	Stru_UniPkgHead2()
	{
		memset(this,0,sizeof(*this));
		headid=HeadID_UniPkg2;
		compressflag=CompressFlag2_NULL;
	}
	Stru_UniPkgHead2(int BodyLen,unsigned int Cmdid,unsigned int Seq,unsigned char Subpkgseq=0,unsigned char Subpkgcnt=1,
		unsigned int Userdata1=0,unsigned int Userdata2=0,unsigned int Userdata3=0,unsigned int Userdata4=0)
	{
		Set(BodyLen,Cmdid,Seq,Subpkgseq,Subpkgcnt,Userdata1,Userdata2,Userdata3,Userdata4);
	}
	void Set(int BodyLen,unsigned int Cmdid,unsigned int Seq=0,unsigned char Subpkgno=0,unsigned char Subpkgcnt=1,
		unsigned int Userdata1=0,unsigned int Userdata2=0,unsigned int Userdata3=0,unsigned int Userdata4=0)
	{
		headid=HeadID_UniPkg2;
		compressflag=CompressFlag2_NULL;
		bodylen=BodyLen;
		cmdid=Cmdid;
		seq=Seq;
		subpkgno=Subpkgno;
		subpkgcnt=Subpkgcnt;
		userdata1=Userdata1;
		userdata2=Userdata2;
		userdata3=Userdata3;
		userdata4=Userdata4;
	}

	//设置包内容
	static bool SetPkg(void* poutpkg,int outpkgsize,
		const void* pbody,
		int BodyLen,unsigned int Cmdid,unsigned int Seq=0,unsigned char Subpkgno=0,unsigned char Subpkgcnt=1,
		unsigned int Userdata1=0,unsigned int Userdata2=0,unsigned int Userdata3=0,unsigned int Userdata4=0)
	{
		if(outpkgsize<(int)sizeof(Stru_UniPkgHead2)+BodyLen+8) return false;

		Stru_UniPkgHead2 *pOutPkgHead=(Stru_UniPkgHead2*)poutpkg;
		char* pOutPkgBody=(char*)poutpkg+sizeof(Stru_UniPkgHead2);
		unsigned int* pOutCRC32=(unsigned int*)(pOutPkgBody+BodyLen);
		unsigned int* pOutTail=pOutCRC32+1;

		pOutPkgHead->Set(BodyLen,Cmdid,Seq,Subpkgno,Subpkgcnt,Userdata1,Userdata2,Userdata3,Userdata4);

		if(BodyLen>0&&pbody) memcpy(pOutPkgBody,pbody,BodyLen);

		(*pOutCRC32)=0;
		(*pOutTail)=0xba55ca82;
		return true;
	}
	//tell you whether a PkgStream is a valid package, if successful, *pUsedLengthByPkg return length used by current package
	static bool IsValidPkg(const void* pPkgStream,int StreamLen,int* pLengthUsedByPkg=NULL)
	{
		if(!pPkgStream||StreamLen<(int)sizeof(Stru_UniPkgHead2)+8) return false;

		const Stru_UniPkgHead2 *pPkgHead=(const Stru_UniPkgHead2*)pPkgStream;
		if(pPkgHead->headid!=HeadID_UniPkg2) return false;
		if(StreamLen<(int)sizeof(Stru_UniPkgHead2)+pPkgHead->bodylen+8) return false;
		const unsigned int* pCRC32=(const unsigned int*)((const char*)pPkgStream+sizeof(Stru_UniPkgHead2)+pPkgHead->bodylen);
		const unsigned int* pTail=pCRC32+1;
		if(*pTail!=TailID_UniPkg2) return false;

	    if(pLengthUsedByPkg) (*pLengthUsedByPkg)=(int)sizeof(Stru_UniPkgHead2)+pPkgHead->bodylen+8;
		return true;
	}

	//判断是否是非法包头。包长够的情况下才判断。
	//tell you whether a PkgStream is an invalid package. Typically, you can skip a character if the stream was started with an invalid package head
	static bool IsInvalidPkgHead(const void* pPkgStream,int StreamLen)
	{
		if(!pPkgStream) return true;

		const Stru_UniPkgHead2 *pPkgHead=(const Stru_UniPkgHead2*)pPkgStream;
	    if(StreamLen>=(int)sizeof(pPkgHead->headid)&&pPkgHead->headid!=HeadID_UniPkg2) return true;

		if(StreamLen<(int)sizeof(Stru_UniPkgHead2)) return false;
		const unsigned int* pCRC32=(const unsigned int*)((const char*)pPkgStream+sizeof(Stru_UniPkgHead2)+pPkgHead->bodylen);
		const unsigned int* pTail=pCRC32+1;
		if(StreamLen<(int)sizeof(Stru_UniPkgHead2)+pPkgHead->bodylen+8) return false;
		if(*pTail!=TailID_UniPkg2) return true;

		return false;
	}

	unsigned int	headid;				//id of UniPkgHead, always be  HeadID_UniPkg2
	int	bodylen;						//length of package body content, in bytes
	unsigned int	cmdid;				//cmdid defined by business module
	unsigned int	seq;				//defined by business module
	unsigned char	subpkgno;			//defined by business module, seq no of sub pkg, commonly be 0
	unsigned char	subpkgcnt;			//defined by business module, count of sub pkg, commonly be 1 if not split package
	unsigned short	compressflag;		//compress method that the package body content used. the value was define in EnumCompressFlag2
	unsigned int	userdata1;			//use it as require
	unsigned int	userdata2;
	unsigned int	userdata3;
	unsigned int	userdata4;
};

#pragma pack(pop)

#endif
