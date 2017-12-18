// VariablePackage.cpp: implementation of the CVariablePackage class.
//
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
//			6. m_bEmptyID
//				很重要，因为缓存按环形缓冲区处理。需要通过m_bEmptyID判断是否为空
//			7. 线程安全：
//				保存数据和读取数据的操作线程安全，可以多个线程同时读取或同时保存。
//			8. 保存两个指针：GetOff和SetOff，GetOff等于SetOff表示为空，GetOff可以追SetOff，但不能超过它，SetOff不能追上GetOff
//			9. 注意，构造和析构对象时，是线程不安全的。父线程要注意保护 
//
//	history 
//	20100119	lzq		创建该文件
//
//////////////////////////////////////////////////////////////////////


#include "VariablePackage.h"
#include "Tools_Ansi.h"
#include "Tools_Win32.h"




/**************************************************************************************************//**
*   @brief				CVariablePackage        
*   @param[in] CacheSize	总字节数
*   @param[in] Mode			运行模式:
*   - 0 单线程读,单线程写
*   - 3 多线程读,多线程写
*******************************************************************************************************/
CVariablePackage::CVariablePackage(int CacheSize, int Mode)
{
	m_pCache=NULL;
	m_CacheSize=0;
	m_GetOff=0;
	m_SetOff=0;
	m_Seq=0;

	m_Mode = Mode;
	m_pSem = NULL;

	if(m_Mode==3)
		m_pSem=CTools_Win32::AllocWinSem();

	m_CacheSize=(CacheSize/4+(CacheSize%4==0?0:1))*4;
	if(m_CacheSize<0) m_CacheSize=0;

	if(m_CacheSize>0)
		m_pCache=(char*)malloc(m_CacheSize);
}

CVariablePackage::~CVariablePackage()
{
	if(m_pCache) 
	{
		free(m_pCache);
		m_pCache=NULL;
	}

	if(m_pSem)
	{
		CTools_Win32::ReleaseWinSem(m_pSem);
		m_pSem=NULL;
	}
}


//	将一包数据存入本地缓存文件，PackageHead指向的包头后面跟数据和包尾
//	如果数据块大于缓存最大尺寸，返回失败
//	不允许覆盖原来的数据
//	注意：pData占的空间根据DataLen按4字节取整
bool CVariablePackage::SaveDataTo(VariablePackageHeadStru& PackageHead, const void* pData)
{
	int PackageLen,AdditionalLen;
	DWORD dwTail=TAIL_VARIABLEPACKAGE;
	bool bRlt;

	if(PackageHead.DataLen>0&&!pData) return false;
	if(!m_pCache) return false;

    PackageHead.Prefix=PREFIX_VARIABLEPACKAGE;
	PackageHead.dwCRC=0;
	PackageHead.dwSeq=m_Seq++;

	AdditionalLen = PackageHead.DataLen%4==0 ? 0 : 4-PackageHead.DataLen%4;
	PackageLen=sizeof(PackageHead) + PackageHead.DataLen + AdditionalLen + 4;


	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	if( m_SetOff>=m_GetOff&&m_CacheSize-(m_SetOff-m_GetOff)<=PackageLen||
	    m_SetOff<m_GetOff&&m_GetOff-m_SetOff<=PackageLen)
		bRlt=false;
	else bRlt=true;

	if(bRlt)
	{
		WriteData(m_SetOff,(char*)&PackageHead,sizeof(PackageHead));
		if(PackageHead.DataLen>0)
			WriteData((m_SetOff+sizeof(PackageHead))%m_CacheSize,(char*)pData,PackageHead.DataLen);

		WriteData((m_SetOff+sizeof(PackageHead)+PackageHead.DataLen+AdditionalLen)%m_CacheSize,(char*)&dwTail,sizeof(dwTail));
		m_SetOff=(m_SetOff+PackageLen)%m_CacheSize;
	}

	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

	return bRlt;
}

//将一块数据存入缓存
bool CVariablePackage::SaveDataTo(const void* pData,int DataLen,DWORD UserData1,DWORD UserData2,DWORD UserData3,DWORD UserData4)
{
    VariablePackageHeadStru PackageHead;

    memset(&PackageHead,0,sizeof(PackageHead));
    PackageHead.DataLen=DataLen;
    PackageHead.UserData1=UserData1;
    PackageHead.UserData2=UserData2;
    PackageHead.UserData3=UserData3;
    PackageHead.UserData4=UserData4;

    return SaveDataTo(PackageHead,pData);
}


//取当前包里数据长度
int CVariablePackage::GetCurPackageDataLen(void)                                 
{
	VariablePackageHeadStru	PackageHead;
	int DataLen;

	if(!m_pCache) return 0;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	IgnoreInvalidPackage();

	if(m_GetOff==m_SetOff) DataLen=0;
	else
	{
															//这时保证有一个合法包
		PackageHead.DataLen=0;
		ReadData(m_GetOff,(char*)&PackageHead,sizeof(PackageHead));
		DataLen=PackageHead.DataLen;
	}

	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

    return DataLen;
}


//	从字符缓存中取出当前数据块，不删除。如果DataSize不够，返回失败
//	如果pData==NULL则只取包头
bool CVariablePackage::PeekFrom(VariablePackageHeadStru& PackageHead,void* pData,int DataSize)
{
	bool bRlt;

	if(!m_pCache) return false;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	IgnoreInvalidPackage();

	if(m_GetOff==m_SetOff) bRlt=false;
	else bRlt=true;
															//这时保证有一个合法包
	ReadData(m_GetOff,(char*)&PackageHead,sizeof(PackageHead));
	if(PackageHead.DataLen>0&&pData&&PackageHead.DataLen>DataSize)
	{
		bRlt=false;											//缓冲区尺寸不足，直接返回false
	}
	else if(PackageHead.DataLen>0&&pData) 
	{
		ReadData((m_GetOff+sizeof(PackageHead))%m_CacheSize,(char*)pData,PackageHead.DataLen);
	}

	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

	return bRlt;
}



//从字符缓存中取出当前数据块，不删除。如果DataSize不够，返回失败.如果pData==NULL则只取包头
bool CVariablePackage::PeekFrom(void* pData,int DataSize,DWORD* pUserData1,DWORD* pUserData2,DWORD* pUserData3,DWORD* pUserData4)
{
    VariablePackageHeadStru PackageHead;
    bool bRlt;

    bRlt=PeekFrom(PackageHead,pData,DataSize);
    if(!bRlt) return false;

    if(pUserData1) *pUserData1=PackageHead.UserData1;
    if(pUserData2) *pUserData2=PackageHead.UserData2;
    if(pUserData3) *pUserData3=PackageHead.UserData3;
    if(pUserData4) *pUserData4=PackageHead.UserData4;

    return true;
}


//	从缓存中取出当前数据块，然后删除之。如果DataSize不够，返回失败
bool CVariablePackage::GetFrom(VariablePackageHeadStru& PackageHead,void* pData,int DataSize)
{
	bool bRlt;
	int AdditionalLen;
	int PackageLen;

	if(!m_pCache) return false;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	IgnoreInvalidPackage();

	if(m_GetOff==m_SetOff) bRlt=false;
	else bRlt=true;
															//这时保证有一个合法包
	ReadData(m_GetOff,(char*)&PackageHead,sizeof(PackageHead));
	if(PackageHead.DataLen>0&&pData&&PackageHead.DataLen>DataSize)
	{
		bRlt=false;											//缓冲区尺寸不足，直接返回false
	}
	else if(PackageHead.DataLen>0&&pData) 
	{
		ReadData((m_GetOff+sizeof(PackageHead))%m_CacheSize,(char*)pData,PackageHead.DataLen);
	}

	if(bRlt)
	{
		AdditionalLen = PackageHead.DataLen%4==0? 0 : 4-PackageHead.DataLen%4;
		PackageLen=sizeof(PackageHead)+PackageHead.DataLen+AdditionalLen+4;
		m_GetOff=(m_GetOff+PackageLen)%m_CacheSize;
	}

	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

	return bRlt;
}


//	从缓存中取出当前数据块，然后删除之。如果DataSize不够，返回失败
bool CVariablePackage::GetFrom(void* pData,int DataSize,DWORD* pUserData1,DWORD* pUserData2,DWORD* pUserData3,DWORD* pUserData4)
{
    VariablePackageHeadStru PackageHead;
    bool bRlt;

    bRlt=GetFrom(PackageHead,pData,DataSize);
    if(!bRlt) return false;

    if(pUserData1) *pUserData1=PackageHead.UserData1;
    if(pUserData2) *pUserData2=PackageHead.UserData2;
    if(pUserData3) *pUserData3=PackageHead.UserData3;
    if(pUserData4) *pUserData4=PackageHead.UserData4;

    return true;
}


//	删除数据
void CVariablePackage::DeleteCurPackage(void)
{
	VariablePackageHeadStru PackageHead;
	GetFrom(PackageHead,NULL,0);
}



//清空缓冲区
void CVariablePackage::Empty(void)
{
	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	m_GetOff=m_SetOff=0;

	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

}

//获取缓冲区尺寸
int	CVariablePackage::GetCacheSize(void)
{
	return m_CacheSize;
}


bool CVariablePackage::IsEmpty(void)
{
	return m_GetOff==m_SetOff?true:false;
}

void CVariablePackage::GetPosition(int& GetOff,int& SetOff)
{
    GetOff=m_GetOff;
    SetOff=m_SetOff;
}


//	从指定位置读一块数据，考虑数据环形折返，不校验数据长度
//	Off是相对位置
//	返回读出字节个数
int CVariablePackage::ReadData(int Off,char* pData,int ReadLen) const
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
int	CVariablePackage::WriteData(int Off, const char* pData,int WriteLen)
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





//	跳过无效的数据
//	执行完后要么m_GetOff==m_SetOff,要么有一个合法包
void CVariablePackage::IgnoreInvalidPackage(void)
{
	VariablePackageHeadStru	PackageHead;
	int AdditionalLen,PackageLen,ReadLen;
	DWORD dwTail;

	if(!m_pCache) return ;
	if(m_GetOff==m_SetOff) return ;

	while(1)
	{
		ReadLen=ReadData(m_GetOff,(char*)&PackageHead,sizeof(PackageHead));

		if(ReadLen==sizeof(PackageHead)&&PackageHead.Prefix==PREFIX_VARIABLEPACKAGE)
		{
			AdditionalLen = PackageHead.DataLen%4==0 ? 0 : 4-PackageHead.DataLen%4;
			PackageLen=sizeof(PackageHead)+PackageHead.DataLen+AdditionalLen+4;
			dwTail=0;
			ReadData((m_GetOff+PackageLen-4)%m_CacheSize,(char*)&dwTail,4);
		
			if( dwTail==TAIL_VARIABLEPACKAGE&&
				(m_GetOff<m_SetOff && m_GetOff+PackageLen<=m_SetOff ||
				 m_GetOff>m_GetOff && (m_GetOff+PackageLen<m_CacheSize||m_GetOff+PackageLen-m_CacheSize<=m_SetOff))  )
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




