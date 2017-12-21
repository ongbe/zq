#pragma once

enum eFrontAddsType
{
    TRADE_TYPE,		//交易前置机服务地址
    QUOT_TYPE		//行情前置机服务地址
};
struct Stru_UniPkgUDPHead:public Stru_UniPkgHead
{
	Stru_UniPkgUDPHead()
	{
		memset(&(headid),0,sizeof(Stru_UniPkgUDPHead));
		headid=HeadID_UniPkg;
		compressflag=CompressFlag_NULL;
	}
	Stru_UniPkgUDPHead(int Len,unsigned int Cmdid,unsigned int Moduleid,unsigned int Seq,unsigned int Subseq=0,
		unsigned int Userdata1=0,unsigned int Userdata2=0,unsigned int Userdata3=0,unsigned int Userdata4=0, long  nUTC = 0, long  nMili = 0 )
	{
		Set(Len,Cmdid,Moduleid,Seq,Subseq,Userdata1,Userdata2,Userdata3,Userdata4, nUTC, nMili);
	}
	void Set(int Len,unsigned int Cmdid,unsigned int Moduleid,unsigned int Seq,unsigned int Subseq=0,
		unsigned int Userdata1=0,unsigned int Userdata2=0,unsigned int Userdata3=0,unsigned int Userdata4=0, long long  nTime = 0, long  nMili = 0  )
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
		nUTCTime = nTime;
		nMiliSecond = nMili;
	};
	//tell you whether a PkgStream is a valid package, if successful, *pUsedLengthByPkg return length used by current package
	static bool IsValidPkg(const void* pPkgStream,int StreamLen,int* pLengthUsedByPkg=0)
	{
		if(!pPkgStream||StreamLen<(int)sizeof(Stru_UniPkgUDPHead)+4)
			return false;

		const Stru_UniPkgUDPHead *pPkgHead=(const Stru_UniPkgUDPHead*)pPkgStream;
		if(pPkgHead->headid!=HeadID_UniPkg||
			StreamLen<(int)sizeof(Stru_UniPkgUDPHead)+pPkgHead->len+4||
			(*(const unsigned int*)((const char*)pPkgStream+sizeof(Stru_UniPkgUDPHead)+pPkgHead->len))!=TailID_UniPkg)
			return false;

		if(pLengthUsedByPkg) (*pLengthUsedByPkg)=(int)sizeof(Stru_UniPkgUDPHead)+pPkgHead->len+4;
		return true;
	}

	//tell you whether a PkgStream is an invalid package. Typically, you can skip a character if the stream was started with an invalid package head
	static bool IsInvalidPkgHead(const void* pPkgStream,int StreamLen)
	{
		if(!pPkgStream)
			return true;

		const Stru_UniPkgUDPHead *pPkgHead=(const Stru_UniPkgUDPHead*)pPkgStream;
		if(StreamLen>=(int)sizeof(pPkgHead->headid)&&pPkgHead->headid!=HeadID_UniPkg)
			return true;

		if(StreamLen>=(int)sizeof(Stru_UniPkgUDPHead)&&
			StreamLen>=(int)sizeof(Stru_UniPkgUDPHead)+pPkgHead->len+4&&
			(*(const unsigned int*)((const char*)pPkgStream+sizeof(Stru_UniPkgUDPHead)+pPkgHead->len))!=TailID_UniPkg)
			return true;

		return false;
	}	

	long  nUTCTime;		//utc秒数
	long  nMiliSecond;	//毫秒数
};
struct CErrorInfo
{
	///错误代码
	int		ErrorID;
	///错误信息
	char	ErrorMsg[81];
};