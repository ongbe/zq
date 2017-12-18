// FixedPackage.cpp: implementation of the CFixedPackage class.
//
//	作用：	定义定长包的缓存类
//	描述：	1. 此类用于将定长数据块缓存
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

#include "FixedPackage.h"
#include "Tools_Ansi.h"
#include "Tools_Win32.h"



/**************************************************************************************************//**
*   @brief				CBlockCache类的构造函数        
*   @param[in] BlockSize	每个数据块的字节数
*   @param[in] BlockCount	数据块的总个数
*   @param[in] Mode			运行模式:
*   - 0 单线程读,单线程写
*   - 3 多线程读,多线程写
*******************************************************************************************************/
CFixedPackage::CFixedPackage(int BlockSize, int BlockCount, int Mode)
{
	m_pData=NULL;
	m_BlockSize=0;
	m_BlockCount=0;
	m_GetOrd=0;
	m_SetOrd=0;

	m_Mode = Mode;
	m_pSem = NULL;

	if(m_Mode==3)
		m_pSem=CTools_Win32::AllocWinSem();

	m_BlockSize=BlockSize;
	if(m_BlockSize<0) m_BlockSize=0;
	m_BlockCount=BlockCount;
	if(m_BlockCount<0) m_BlockCount=0;

	if(m_BlockSize*m_BlockCount>0)
		m_pData=(char*)malloc(m_BlockSize*m_BlockCount);
}


//	类的析构函数
CFixedPackage::~CFixedPackage()
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
bool CFixedPackage::SaveDataTo(void *pBlockData)
{
	bool bRlt;

	if(!pBlockData) return false;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	if(m_SetOrd>=m_GetOrd&&m_BlockCount-(m_SetOrd-m_GetOrd)<=1||
	   m_SetOrd<m_GetOrd&&m_GetOrd-m_SetOrd<=1)
	   bRlt=false;
	else bRlt=true;

	if(bRlt)
	{
		WriteData(m_SetOrd,(char*)pBlockData);
		m_SetOrd=(m_SetOrd+1)%m_BlockCount;
	}

	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

	return bRlt;
}


//	取当前数据块的个数
int	CFixedPackage::GetBlockCount(void)
{
	int BlockCount;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	if(m_SetOrd==m_GetOrd) BlockCount=0;
	else if(m_GetOrd<m_SetOrd) BlockCount=m_SetOrd-m_GetOrd;
	else BlockCount=m_BlockCount-m_GetOrd+m_SetOrd;

	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

	return BlockCount;
}


//	从字符缓存中取出当前数据块，不删除
bool CFixedPackage::PeekFrom(void *pData)
{
	bool bRlt;

	if(!m_pData) return false;
	if(!pData) return false;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	if(m_GetOrd==m_SetOrd) bRlt=false;
	else
	{
		ReadDate(m_GetOrd,(char*)pData);
		bRlt=true;
	}

	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

	return bRlt;
}



//	从缓存中取出当前数据块，然后删除之
bool CFixedPackage::GetFrom(void *pData)
{
	bool bRlt;

	if(!m_pData) return false;
	if(!pData) return false;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	if(m_GetOrd==m_SetOrd) bRlt=false;
	else
	{
		ReadDate(m_GetOrd,(char*)pData);
		m_GetOrd=(m_GetOrd+1)%m_BlockCount;
		bRlt=true;
	}

	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

	return bRlt;
}


//	删除数据
void CFixedPackage::DeleteCurBlock(void)
{
	if(!m_pData) return;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	if(m_GetOrd!=m_SetOrd)
		m_GetOrd=(m_GetOrd+1)%m_BlockCount;

	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);
}


//清空缓冲区
void CFixedPackage::Empty(void)
{
	if(!m_pData) return;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	m_GetOrd=m_SetOrd;

	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);
}


bool CFixedPackage::IsEmpty(void)
{
	return m_GetOrd==m_SetOrd?true:false;
}


void CFixedPackage::GetPosition(int& GetOrd,int& SetOrd)
{
    GetOrd=m_GetOrd;
    SetOrd=m_SetOrd;
}



//取块的尺寸
int CFixedPackage::GetBlockSize(void)
{
	return m_BlockSize;
}


//	从指定位置读一块数据
//	iOff是相对位置
//	返回读出数据块个数
bool CFixedPackage::ReadDate(int Ord,char*pData)
{
	if(!m_pData||Ord<0||Ord>=m_BlockCount)
		return false;

	memcpy(pData,m_pData+Ord*m_BlockSize,m_BlockSize);

	return true;
}


//	向指定位置写一块数据
//	iOff是相对位置
//	返回写入数据块的个数
bool CFixedPackage::WriteData(int Ord,char*pData)
{
	if(Ord<0||Ord>=m_BlockCount)					
		return false;			

	if(!m_pData) return false;

	memcpy(m_pData+Ord*m_BlockSize,pData,m_BlockSize);

	return true;
}







