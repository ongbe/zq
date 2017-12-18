//*******************************************************************************************
//	作用：	定义不定长包的缓存类
//	描述：	1. 此类用于将不定长数据块缓存
//			2. 数据结构为：Package = PackageHead + Data + Additional(1-3) + Tail
//			3. Data占用的空间按4字节用零补整，就是Additional部分。每个package都是4字节对齐的
//			4. 遵循先进先出的原则
//			5. 主要外部接口：
//				SaveDataTo()	：将一块数据存入缓存
//				GetDataFrom()	：从缓存中取出当前数据块
//				GetCurDataLen()	：取当前数据块的长度
//				IsEmpty()		：缓存是否为空
//			6. 杂项接口：
//				SetCacheParam()	：设置缓存参数，对于文件缓存，设置文件名和最大尺寸；对于内存缓存，设置最大尺寸
//			7. m_bEmptyID
//				很重要，因为缓存按环形缓冲区处理。需要通过m_bEmptyID判断是否为空
//			8. 非线程安全：
//				保存数据和读取数据的操作可以分别放在两个线程里。但不能多个线程同时读取或同时保存。
//			9. 保存两个指针：GetOff和SetOff，GetOff等于SetOff表示为空，GetOff可以追SetOff，但不能超过它，SetOff不能追上GetOff
//
//	history 
//	20100119	lzq		创建该文件
//
//*******************************************************************************************
#include <stdafx.h>
#include <time.h>
#include <stdlib.h>

//using namespace std;
#include "packagecache.h"


#define GuardRead()  MutexLockGuradEx lock(m_pReadMutexLock)											//读保护
#define GuardWrite() MutexLockGuradEx lock(m_pWriteMutexLock)											//写保护

//	类的构造函数
CPackageCache::CPackageCache(int CacheSize, int Mode)
{
	m_pCache=NULL;
	m_CacheSize=0;
	m_GetOff=0;
	m_SetOff=0;

	m_Mode = Mode;
	m_pReadMutexLock=NULL;
	m_pWriteMutexLock=NULL;

	switch(m_Mode)
	{
		case 0://单线程读,单线程写
			break;
		case 1://单线程读,多线程写
			m_pWriteMutexLock = new MutexLock();
			break;
		case 2://多线程读,单线程写
			m_pReadMutexLock = new MutexLock();
			break;
		case 3://多线程读,多线程写
		default:
            //next gragment fixed by liao, 20101011
			m_pReadMutexLock = new MutexLock();
            m_pWriteMutexLock = m_pReadMutexLock;
//			m_pReadMutexLock = new MutexLock();
//			m_pWriteMutexLock = new MutexLock();
			break;
	}

	SetCacheParam(CacheSize);
}


//	类的析构函数
CPackageCache::~CPackageCache()
{
	if(m_pCache) 
	{
		free(m_pCache);
		m_pCache=NULL;
	}

    //next fragment added by liao, 20101011
    if(m_pReadMutexLock&&m_pReadMutexLock==m_pWriteMutexLock)
    {
		delete m_pReadMutexLock;
		m_pReadMutexLock=m_pWriteMutexLock=NULL;
    }

	if(m_pReadMutexLock)
	{
		delete m_pReadMutexLock;
		m_pReadMutexLock=NULL;
	}

	if(m_pWriteMutexLock)
	{
		delete m_pWriteMutexLock;
		m_pWriteMutexLock=NULL;
	}
}

//	如果BlockSize和BlockCount为零，表示要初始化内存区
void CPackageCache::SetCacheParam(int CacheSize)		
{
	if(CacheSize>0)
		m_CacheSize=(CacheSize/4+(CacheSize%4==0?0:1))*4;

	m_GetOff=0;
	m_SetOff=0;

	if(m_pCache) 
	{
		free(m_pCache);
		m_pCache=NULL;
	}

	if(m_CacheSize>0)
		m_pCache=(char*)malloc(m_CacheSize);
}




bool CPackageCache::IsEmpty(void)
{

	GuardRead();

	return m_GetOff==m_SetOff?true:false;
}



//	从指定位置读一块数据，考虑数据环形折返，不校验数据长度
//	Off是相对位置
//	返回读出字节个数
int CPackageCache::ReadData(int Off,char* pData,int ReadLen) const
{
	int iLen;

	if(!m_pCache||Off<0||Off>=m_CacheSize||ReadLen>m_CacheSize)
		return 0;

	if(Off+ReadLen>m_CacheSize) 
	{
		iLen=m_CacheSize-Off;
		memcpy(pData,m_pCache+Off,iLen);
		memcpy(pData+iLen,m_pCache,ReadLen-iLen);
	}
	else 
	{
		memcpy(pData,m_pCache+Off,ReadLen);
	}

	return ReadLen;
}


//	向指定位置写一块数据，考虑数据环形折返，不校验数据长度
//	Off是相对位置
//	返回写入数据块的个数
int	CPackageCache::WriteData(int Off, const char* pData,int WriteLen)
{
	int iLen;

	if(!m_pCache||Off<0||Off>=m_CacheSize||WriteLen>m_CacheSize)
		return 0;

	if(Off+WriteLen>m_CacheSize) 
	{
		iLen=m_CacheSize-Off;
		memcpy(m_pCache+Off,pData,iLen);
		memcpy(m_pCache,pData+iLen,WriteLen-iLen);
	}
	else 
	{
		memcpy(m_pCache+Off,pData,WriteLen);
	}

	return WriteLen;
}





//	将一包数据存入本地缓存文件，pPackage指向的包头后面跟数据和包围
//	如果数据块大于缓存最大尺寸，返回失败
//	不允许覆盖原来的数据
//	注意：DataLen需要按4字节取整
bool CPackageCache::SaveDataTo(PackageHeadStru* pPackageHead, const void* pData,int* pOriSetOff,int* pNewSetOff)
{
	int PackageLen,i,AdditionalLen;
	DWORD dwTail=TAIL_PACKAGECACHE;

	if(!pPackageHead) return false;
	if(pPackageHead->DataLen>0&&!pData) return false;

    pPackageHead->Prefix=PREFIX_PACKAGECACHE;
	AdditionalLen=4-(pPackageHead->DataLen)%4;
	if(AdditionalLen==4) AdditionalLen=0;

	PackageLen=sizeof(PackageHeadStru) + pPackageHead->DataLen+AdditionalLen + 4;


	GuardWrite();
	if(m_SetOff>=m_GetOff&&m_CacheSize-(m_SetOff-m_GetOff)<=PackageLen) return false;
	if(m_SetOff<m_GetOff&&m_GetOff-m_SetOff<=PackageLen) return false;

    if(pOriSetOff) (*pOriSetOff)=m_SetOff;

	WriteData(m_SetOff,(char*)pPackageHead,sizeof(PackageHeadStru));
	if(pPackageHead->DataLen>0)
	{
		WriteData((m_SetOff+sizeof(PackageHeadStru))%m_CacheSize,(char*)pData,pPackageHead->DataLen);
		if(AdditionalLen>0)
		{													//按4字节取整
			i=0;
			WriteData((m_SetOff+sizeof(PackageHeadStru)+pPackageHead->DataLen)%m_CacheSize,(char*)&i,AdditionalLen);
		}
	}

	WriteData((m_SetOff+sizeof(PackageHeadStru)+pPackageHead->DataLen+AdditionalLen)%m_CacheSize,(char*)&dwTail,sizeof(dwTail));
	m_SetOff=(m_SetOff+PackageLen)%m_CacheSize;

    if(pNewSetOff) (*pNewSetOff)=m_SetOff;

	return true;
}

//将一块数据存入缓存
bool CPackageCache::SaveDataTo(int CmdID,int PlatformID,int ExchangeID,DWORD dwSeq,int DataLen, const void* pData,int* pOriSetOff,int* pNewSetOff)
{
    PackageHeadStru PackageHead;

    memset(&PackageHead,0,sizeof(PackageHead));
    PackageHead.CmdID=CmdID;
    PackageHead.PlatformID=PlatformID;
    PackageHead.ExchangeID=ExchangeID;
    PackageHead.DataLen=DataLen;
    PackageHead.dwSeq=dwSeq;

    return SaveDataTo(&PackageHead,pData,pOriSetOff,pNewSetOff);
}

//	跳过无效的数据
//	执行完后要么m_GetOff==m_SetOff,要么有一个合法包
void CPackageCache::IgnoreInvalidPackage(void)
{
	PackageHeadStru	PackageHead;
	int AdditionalLen,PackageLen,ReadLen;
	DWORD dwTail;

	if(!m_pCache) return ;
	if(m_GetOff==m_SetOff) return ;

	while(1)
	{
		ReadLen=ReadData(m_GetOff,(char*)&PackageHead,sizeof(PackageHead));

		if(ReadLen==sizeof(PackageHead)&&PackageHead.Prefix==PREFIX_PACKAGECACHE)
		{
			AdditionalLen=4-(PackageHead.DataLen%4);
			if(AdditionalLen==4) AdditionalLen=0;
			PackageLen=sizeof(PackageHeadStru)+PackageHead.DataLen+AdditionalLen+4;
			dwTail=0;
			ReadData((m_GetOff+PackageLen-4)%m_CacheSize,(char*)&dwTail,4);
		
			if( dwTail==TAIL_PACKAGECACHE&&
				(m_GetOff<m_SetOff && m_GetOff+PackageLen<=m_SetOff ||
				 m_GetOff>m_SetOff && (m_GetOff+PackageLen<m_CacheSize||m_GetOff+PackageLen-m_CacheSize<=m_SetOff))  )
			{
				break;
			}
		}


		m_GetOff+=4;
		if(m_GetOff>=m_CacheSize) m_GetOff=0;

		if(m_GetOff==m_SetOff) break;
	}

	return ;
}

//取当前包里数据长度
int CPackageCache::GetCurPackageDataLen(void)                                 
{
	GuardRead();

	PackageHeadStru	PackageHead;

	if(!m_pCache) return 0;

	IgnoreInvalidPackage();

	if(m_GetOff==m_SetOff) return 0;

															//这时保证有一个合法包
    PackageHead.DataLen=0;
	ReadData(m_GetOff,(char*)&PackageHead,sizeof(PackageHead));
    return PackageHead.DataLen;
}

//从字符缓存中取出当前数据块，不删除。如果DataSize不够，返回失败.如果pData==NULL则只取包头
bool CPackageCache::PeekFrom(int&CmdID,int&PlatformID,int& ExchangeID,DWORD& dwSeq,int& DataLen,void* pData,int DataSize,int* pGetOff)
{
    PackageHeadStru PackageHead;
    bool bRlt;

    bRlt=PeekFrom(&PackageHead,pData,DataSize,pGetOff);
    if(!bRlt) return false;

    CmdID=PackageHead.CmdID;
    PlatformID=PackageHead.PlatformID;
    ExchangeID=PackageHead.ExchangeID;
    dwSeq=PackageHead.dwSeq;

    return true;
}

//	从字符缓存中取出当前数据块，不删除。如果DataSize不够，返回失败
//	如果pData==NULL则只取包头
bool CPackageCache::PeekFrom(PackageHeadStru* pPackageHead,void* pData,int DataSize,int* pGetOff)
{
	GuardRead();

	PackageHeadStru	PackageHead;

	if(!m_pCache) return false;

	IgnoreInvalidPackage();

	if(m_GetOff==m_SetOff) return false;

															//这时保证有一个合法包
	ReadData(m_GetOff,(char*)&PackageHead,sizeof(PackageHead));
	if(pPackageHead)
		memcpy((char*)pPackageHead,(char*)&PackageHead,sizeof(PackageHeadStru));

	if(PackageHead.DataLen>0&&pData&&PackageHead.DataLen>DataSize)
	{
		return false;										//缓冲区尺寸不足，直接返回false
	}


	if(PackageHead.DataLen>0&&pData) 
		ReadData((m_GetOff+sizeof(PackageHeadStru))%m_CacheSize,(char*)pData,PackageHead.DataLen);

    if(pGetOff) (*pGetOff)=m_GetOff;

	return true;
}


//	从缓存中取出当前数据块，然后删除之。如果DataSize不够，返回失败
bool CPackageCache::GetFrom(PackageHeadStru* pPackageHead,void* pData,int DataSize,int* pOriGetOff,int* pNewGetOff)
{
	bool bRlt;
	int AdditionalLen,PackageLen;
    PackageHeadStru PackageHead;

    if(!pPackageHead) pPackageHead=&PackageHead;

	GuardRead();

    if(pOriGetOff) (*pOriGetOff)=m_GetOff;

#if 0//不能打开，否则死锁
	bRlt=PeekFrom(pPackageHead,pData,DataSize);
#else
	do
	{//copy form CPackageCache::PeekFrom(PackageHeadStru* pPackageHead,void* pData,int DataSize)
		PackageHeadStru	PackageHead;


		if(!m_pCache)
		{
			bRlt=false;
			break;
		}

		IgnoreInvalidPackage();

		if(m_GetOff==m_SetOff)
		{
			bRlt=false;
			break;
		}

																//这时保证有一个合法包
		ReadData(m_GetOff,(char*)&PackageHead,sizeof(PackageHead));
		if(pPackageHead)
			memcpy((char*)pPackageHead,(char*)&PackageHead,sizeof(PackageHeadStru));

		if(PackageHead.DataLen>0&&pData&&PackageHead.DataLen>DataSize)
		{
			bRlt=false;										//缓冲区尺寸不足，直接返回false
			break;
		}

		if(PackageHead.DataLen>0&&pData) 
			ReadData((m_GetOff+sizeof(PackageHeadStru))%m_CacheSize,(char*)pData,PackageHead.DataLen);

		bRlt=true;
	}while(0);
#endif

	if(bRlt)
	{
		AdditionalLen=4-(pPackageHead->DataLen%4);
		if(AdditionalLen==4) AdditionalLen=0;
		PackageLen=sizeof(PackageHeadStru)+pPackageHead->DataLen+AdditionalLen+4;
		m_GetOff=(m_GetOff+PackageLen)%m_CacheSize;
	}

    if(pNewGetOff) (*pNewGetOff)=m_GetOff;
	return bRlt;
}


//	删除数据
void CPackageCache::DeleteCurPackage(int* pOriGetOff,int* pNewGetOff)
{
	GetFrom(NULL,NULL,0,pOriGetOff,pNewGetOff);
}



//清空缓冲区
void CPackageCache::Empty(void)
{
	GuardRead();

	m_GetOff=m_SetOff;
}


void CPackageCache::GetPosition(int& GetOff,int& SetOff)
{

	GuardRead();

    GetOff=m_GetOff;
    SetOff=m_SetOff;
}

//在缓冲区中，将所有CmdID等于指定值的包的CmdID置为零(目的是使之无效)
void CPackageCache::ClearCmdIDInPackageHead(int CmdID)
{
    PackageHeadStru PackageHead;

    if(CmdID==0) return;

	GuardWrite();

    int Off=0;
    int Len=(m_SetOff+m_CacheSize-m_GetOff)%m_CacheSize;
    int AdditionalLen,PackageLen;
    while(Off+(int)sizeof(PackageHead)<=Len)
    {
	    if(ReadData((Off+m_GetOff)%m_CacheSize,(char*)&PackageHead,sizeof(PackageHead))!=(int)sizeof(PackageHead))
            break;
        if(PackageHead.CmdID==CmdID)
        {
            PackageHead.CmdID=0;
            WriteData((Off+m_GetOff)%m_CacheSize,(char*)&PackageHead,sizeof(PackageHead));
        }

        AdditionalLen=4-(PackageHead.DataLen%4);
		if(AdditionalLen==4) AdditionalLen=0;
		PackageLen=sizeof(PackageHeadStru)+PackageHead.DataLen+AdditionalLen+4;

        Off+=PackageLen;
    }
}
