// StreamCache.cpp: implementation of the CMemoryCache class.
//
//	作用：	定义字符流缓存类
//	描述：	1. 此类用于将字符流缓存
//			2. 遵循先进先出的原则
//			3. 主要外部接口：
//				SaveDataTo()	：将一块数据存入缓存
//				GetDataFrom()	：从缓存中取出当前数据块
//				GetCurDataLen()	：取当前数据块的长度
//				IsEmpty()		：缓存是否为空
//			4. m_bEmptyID
//				很重要，因为缓存按环形缓冲区处理。需要通过m_bEmptyID判断是否为空
//			5. 线程安全：
//				保存数据和读取数据的操作线程安全，可以多个线程同时读取或同时保存。
//			6. 保存两个指针：GetOff和SetOff，GetOff等于SetOff表示为空，GetOff可以追SetOff，但不能超过它，SetOff不能追上GetOff
//			7. 注意，构造和析构对象时，是线程不安全的。父线程要注意保护 
//
//	history 
//	20100119	lzq		创建该文件
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include "MemoryCache.h"
#include "Tools_Ansi.h"
#include "Tools_Win32.h"


#pragma unmanaged

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/**************************************************************************************************//**
*   @brief				CMemoryCache类的构造函数        
*   @param[in] CacheSize	缓冲区大小
*   @param[in] Mode			运行模式:
*   - 0 单线程读,单线程写
*   - 3 多线程读,多线程写
*******************************************************************************************************/
CMemoryCache::CMemoryCache(int CacheSize,int Mode)
{
	m_pData=NULL;
	m_CacheSize=CacheSize;
	if(m_CacheSize<0) m_CacheSize=0;
	m_GetOff=0;
	m_SetOff=0;

	m_Mode = Mode;
	m_pSem = NULL;

	if(m_Mode==3)
		m_pSem=CTools_Win32::AllocWinSem();

	if(m_CacheSize>0)
	{
		m_pData=(char*)malloc(m_CacheSize*2+1);
		if(m_pData)
			memset(m_pData,0,m_CacheSize*2+1);
	}

}


CMemoryCache::~CMemoryCache()
{
	if(m_pData) 
	{
		free(m_pData);
		m_pData=NULL;
	}

	if(m_pSem)
	{
		CTools_Win32::ReleaseWinSem(m_pSem);
		m_pSem=NULL;
	}

}


//	将一块数据存入本地缓存文件
//	如果数据块大于缓存最大尺寸，返回失败
//	不允许覆盖原来的数据
bool CMemoryCache::SaveDataTo(void *pData,int DataLen)
{
	bool bRlt;
	int	CmpOff;														//用于比较的数据块写位置
															
	if(!pData) return false;
	if(DataLen>m_CacheSize) return false;															//数据太大了，放不下
	if(DataLen<0) return false;
	if(DataLen==0) return true;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	if(m_SetOff<m_GetOff) CmpOff=m_GetOff;
	else CmpOff=m_GetOff+m_CacheSize;
	if(m_SetOff+DataLen>=CmpOff) bRlt=false;
	else
	{
		WriteData_Internal(m_SetOff,pData,DataLen);
		m_SetOff=(m_SetOff+DataLen)%m_CacheSize;
		bRlt=true;
	}

	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

	return bRlt;
}

//取当前包里数据长度
int CMemoryCache::GetLen(void)
{
	int Len=0;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	Len=GetLen_Internal();

	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

    return Len;
}


//取子串的位置，距离当前位置的距离，子串以0结尾
//如果没有子串，返回-1
int	CMemoryCache::GetPos_SubStr(const char* pSubStr)
{
	int Pos=0,SubLen,ContentLen;
	char *pp;

	if(!m_pData) return -1;
	SubLen=strlen(pSubStr);
	if(SubLen>m_CacheSize) return -1;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	pp=strstr(m_pData+m_GetOff,pSubStr);
	if(!pp) Pos=-1;
	else Pos=pp-(m_pData+m_GetOff);

	ContentLen=GetLen_Internal();

	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

	if(Pos==-1) return -1;
	if(Pos+SubLen>ContentLen) return -1;
	return Pos;
}


//从本地缓存中取出指定长度的内容，不删除
bool CMemoryCache::PeekFrom(void *pData,int PeekLen)
{
	bool	bRlt=false;
	int		CurLen;

	if(!m_pData) return false;
	if(!pData) return false;
	if(PeekLen==0) return true;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	CurLen=GetLen_Internal();
	if(PeekLen>CurLen) bRlt=false;
	else 
	{
		ReadDate_Internal(m_GetOff,pData,PeekLen);
		bRlt=true;
	}


	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

	return bRlt;
}


//从本地缓存中取出指定长度的内容，并从缓存中删除
bool CMemoryCache::GetFrom(void *pData,int GetLen)
{
	bool	bRlt=false;
	int		CurLen;

	if(!m_pData) return false;
	if(!pData) return false;
	if(GetLen==0) return true;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	CurLen=GetLen_Internal();
	if(GetLen>CurLen) bRlt=false;
	else 
	{
		ReadDate_Internal(m_GetOff,pData,GetLen);
		m_GetOff=(m_GetOff+GetLen)%m_CacheSize;
		bRlt=true;
	}

	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

	return bRlt;
}

//删除指定长度的数据
void CMemoryCache::DeleteData(int DeleteLen)
{
	bool	bRlt=false;
	int		CurLen;

	if(DeleteLen<=0) return;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	CurLen=GetLen_Internal();
	if(DeleteLen<0) DeleteLen=0;
	else if(DeleteLen>CurLen) DeleteLen=CurLen;

	m_GetOff=(m_GetOff+DeleteLen)%m_CacheSize;

	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);
}


//清空缓冲区，由读取线程调用
void CMemoryCache::Empty(void)
{
	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);
	m_GetOff=m_SetOff;
	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);
}


bool CMemoryCache::IsEmpty(void)
{
	return m_GetOff==m_SetOff?true:false;
}


void CMemoryCache::GetPosition(int& GetOrd,int& SetOrd)
{
    GetOrd=m_GetOff;
    SetOrd=m_SetOff;
}



//取Cache的尺寸
int CMemoryCache::GetCacheSize(void)
{
	return m_CacheSize;
}


//取当前包里数据长度，内部调用
int CMemoryCache::GetLen_Internal(void)
{
	if(m_SetOff==m_GetOff) return 0;
	else if(m_GetOff<m_SetOff) return m_SetOff-m_GetOff;
	else return m_CacheSize-m_GetOff+m_SetOff;
}



//	从指定位置读一块数据
//	iOff是相对位置
//	返回读出的数据长度
//	由于写数据时写了两份数据，所以读数据时不用考虑环形
int	CMemoryCache::ReadDate_Internal(int iOff,void*pData,int ReadLen)
{
	if(!m_pData||iOff<0||iOff>=m_CacheSize||ReadLen<0||ReadLen>m_CacheSize)			//尺寸过大
		return 0;

	memcpy(pData,m_pData+iOff,ReadLen);
	return ReadLen;
}


//	向指定位置写一块数据
//	iOff是相对位置
//	返回写入数据的长度
//	需要写两份数据
int	CMemoryCache::WriteData_Internal(int iOff,void*pData,int WriteLen)
{
	if(iOff<0||iOff>=m_CacheSize||WriteLen<0||WriteLen>m_CacheSize)					
		return 0;			

	if(!m_pData) return 0;
	if(iOff+WriteLen<=m_CacheSize)												//没有达到缓存的最大尺寸
	{
		memcpy(m_pData+iOff,pData,WriteLen);
		memcpy(m_pData+m_CacheSize+iOff,pData,WriteLen);
	}
	else
	{																			//达到了缓存的最大边界，环形处理
		memcpy(m_pData+iOff,pData,m_CacheSize-iOff);
		memcpy(m_pData,(char*)pData+m_CacheSize-iOff,WriteLen-(m_CacheSize-iOff));

		memcpy(m_pData+m_CacheSize+iOff,pData,m_CacheSize-iOff);
		memcpy(m_pData+m_CacheSize,(char*)pData+m_CacheSize-iOff,WriteLen-(m_CacheSize-iOff));
	}
	return WriteLen;
}

