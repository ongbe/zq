// DB_OO4O.cpp: implementation of the CDB_OO4O class.
//
//	
//	FileName	:db_oo4o.h
//	Description	:1. 实现oo4o数据库操作
//	Include		:oracl.h
//	Lib			:oraclm32.lib
//	Platform	:windows
//	Author		:lzq
//				 Copyright (C), 2008-2028
//////////////////////////////////////////////////////////////////////

#include "DB_OO4O.h"
#include "DB_OO4OPool.h"
#include "Tools_Win32.h"

#ifdef				UseDB_OO4O
#ifdef				UseDB_OO4OPOOL


char* CDB_OO4O::m_pDB_OO4OPool=NULL;


//在使用这个类之前，需要注册一下，注意，单线程使用
void CDB_OO4O::Regist(void)
{
	m_pDB_OO4OPool=(char*)new CDB_OO4OPool;
}


//在使用完这个类之前，需要注销一下，注意，单线程使用
void CDB_OO4O::Unregist(void)
{
	if(m_pDB_OO4OPool)
	{
		delete (CDB_OO4OPool*)m_pDB_OO4OPool;
		m_pDB_OO4OPool=NULL;
	}
}


CDB_OO4O::CDB_OO4O()
{
	if(!m_pDB_OO4OPool)
		Regist();
	if(!m_pDB_OO4OPool) return;

	m_pSem=CTools_Win32::AllocWinSem();
	m_PoolOrd=-1;
}

CDB_OO4O::CDB_OO4O(char *db_sSource,char *db_sUser,char *db_sPasswd)
{
	CDB_OO4O();
	ConnectDB(db_sSource,db_sUser,db_sPasswd);
}

CDB_OO4O::~CDB_OO4O()
{
	if(!m_pDB_OO4OPool) return;

	if(m_PoolOrd>=0)
	{
		((CDB_OO4OPool*)m_pDB_OO4OPool)->DisconnectDB(m_PoolOrd);
		m_PoolOrd=-1;
	}
	if(m_pSem)
	{
		CTools_Win32::ReleaseWinSem(m_pSem);
		m_pSem=NULL;
	}

}

//	连接数据库，得到的数据库句柄放在m_PoolOrd中
//	连接时使用指定的数据库名称、用户名和密码
bool CDB_OO4O::ConnectDB(char *db_sSource,char *db_sUser,char *db_sPasswd)
{
	bool bRlt;
	if(!db_sSource||!db_sSource[0]||!db_sUser||!db_sUser[0]||!db_sPasswd||!db_sPasswd[0])
		return false;

	if(m_pSem)
		CTools_Win32::TakeWinSem(m_pSem);

	bRlt=false;
	if(m_pDB_OO4OPool)
	{
		if(m_PoolOrd>=0)
		{
			DisconnectDB();
		}

		m_PoolOrd=((CDB_OO4OPool*)m_pDB_OO4OPool)->ConnectDB(db_sSource,db_sUser,db_sPasswd);
		bRlt = m_PoolOrd>=0?true:false;
	}

	if(m_pSem)
		CTools_Win32::GiveWinSem(m_pSem);

	return bRlt;
}


//	断开数据库连接
void CDB_OO4O::DisconnectDB(void)
{
	if(m_pSem)
		CTools_Win32::TakeWinSem(m_pSem);

	if(m_pDB_OO4OPool&&m_PoolOrd>=0)
	{
		((CDB_OO4OPool*)m_pDB_OO4OPool)->DisconnectDB(m_PoolOrd);
		m_PoolOrd=-1;
	}

	if(m_pSem)
		CTools_Win32::GiveWinSem(m_pSem);
}

//	查询数据库是否正处于连接状态
bool CDB_OO4O::GetConnectOK(void)
{
	bool bRlt;

	if(m_pSem)
		CTools_Win32::TakeWinSem(m_pSem);

	bRlt=m_PoolOrd>=0&&m_pDB_OO4OPool&&((CDB_OO4OPool*)m_pDB_OO4OPool)->GetConnectOK(m_PoolOrd)?true:false;

	if(m_pSem)
		CTools_Win32::GiveWinSem(m_pSem);

	return bRlt;
}

//取最后的错误，错误文本放在pRltErrText中，返回错误号
int CDB_OO4O::GetLastError(char* pRltErrText,int ErrTextSize)
{
	int iRlt;

	if(m_pSem)
		CTools_Win32::TakeWinSem(m_pSem);
	iRlt=0;
	if(m_pDB_OO4OPool&&m_PoolOrd>=0)
		iRlt=((CDB_OO4OPool*)m_pDB_OO4OPool)->GetLastError(m_PoolOrd,pRltErrText,ErrTextSize);

	if(m_pSem)
		CTools_Win32::GiveWinSem(m_pSem);

	return iRlt;
}



//	打开RecordSet，返回记录集的指针
char* CDB_OO4O::OpenRSet(char *pSql)
{
	char *pRlt=NULL;

	if(m_pSem)
		CTools_Win32::TakeWinSem(m_pSem);

	if(m_PoolOrd>=0&&m_pDB_OO4OPool) 
		pRlt=((CDB_OO4OPool*)m_pDB_OO4OPool)->OpenRSet(m_PoolOrd,pSql);

	if(m_pSem)
		CTools_Win32::GiveWinSem(m_pSem);

	return pRlt;
}

//	关闭RecordSet
void CDB_OO4O::CloseRSet(char *pRSet)
{
	if(m_pSem)
		CTools_Win32::TakeWinSem(m_pSem);

	if(m_PoolOrd>=0&&m_pDB_OO4OPool&&pRSet) 
		((CDB_OO4OPool*)m_pDB_OO4OPool)->CloseRSet(pRSet);

	if(m_pSem)
		CTools_Win32::GiveWinSem(m_pSem);
}


//	移到下一个记录
void CDB_OO4O::MoveNext(char *pRSet)
{
	if(m_pSem)
		CTools_Win32::TakeWinSem(m_pSem);

	if(m_PoolOrd>=0&&m_pDB_OO4OPool&&pRSet) 
		((CDB_OO4OPool*)m_pDB_OO4OPool)->MoveNext(pRSet);

	if(m_pSem)
		CTools_Win32::GiveWinSem(m_pSem);
}


//	判断是否到了记录集的最后
bool CDB_OO4O::IsEOF(char *pRSet)
{
	bool bRlt=true;

	if(m_pSem)
		CTools_Win32::TakeWinSem(m_pSem);

	if(m_PoolOrd>=0&&m_pDB_OO4OPool&&pRSet) 
		bRlt=((CDB_OO4OPool*)m_pDB_OO4OPool)->IsEOF(pRSet);

	if(m_pSem)
		CTools_Win32::GiveWinSem(m_pSem);

	return bRlt;
}

//	取前几个域的字符串值
//	在odb 上执行sql语句，取前几个结果记录的域值
bool CDB_OO4O::GetFieldData(char *pRSet,int FieldNum,char **pRltBuf,int *RltSize)
{
	bool bRlt=false;

	if(m_pSem)
		CTools_Win32::TakeWinSem(m_pSem);
	
	if(m_PoolOrd>=0&&m_pDB_OO4OPool&&pRSet)
		bRlt = ((CDB_OO4OPool*)m_pDB_OO4OPool)->GetFieldData(pRSet,FieldNum,pRltBuf,RltSize);

	if(m_pSem)
		CTools_Win32::GiveWinSem(m_pSem);

	return bRlt;
}


//	取前2个域的字符串值
//	在odb 上执行sql语句，取前几个结果记录的域值
bool CDB_OO4O::GetFieldData(char *pRSet,char *pRltBuf1,int RltSize1,char *pRltBuf2,int RltSize2,char *pRltBuf3,int RltSize3,char *pRltBuf4,int RltSize4)
{
	char			*pFieldData[4];
	int				FieldDataSize[4];
	int				FieldNum;
	bool			bRlt=false;

	pFieldData[0]=pRltBuf1;
	FieldDataSize[0]=RltSize1;
	pFieldData[1]=pRltBuf2;
	FieldDataSize[1]=RltSize2;
	pFieldData[2]=pRltBuf3;
	FieldDataSize[2]=RltSize3;
	pFieldData[3]=pRltBuf4;
	FieldDataSize[3]=RltSize4;

	if(pRltBuf4&&RltSize4>0) FieldNum=4;
	else if(pRltBuf3&&RltSize3>0) FieldNum=3;
	else if(pRltBuf2&&RltSize2>0) FieldNum=2;
	else FieldNum=1;

	if(m_pSem)
		CTools_Win32::TakeWinSem(m_pSem);
	
	if(m_PoolOrd>=0&&m_pDB_OO4OPool&&pRSet)
		bRlt = ((CDB_OO4OPool*)m_pDB_OO4OPool)->GetFieldData(pRSet,FieldNum,pFieldData,FieldDataSize);

	if(m_pSem)
		CTools_Win32::GiveWinSem(m_pSem);

	return bRlt;
}






//	取前几个域的字符串值
//	在odb 上执行sql语句，取前几个结果记录的域值
bool CDB_OO4O::GetSqlFieldData(char *sql,int FieldNum,char **pRltBuf,int *RltSize)
{
	bool bRlt=false;

	if(m_pSem)
		CTools_Win32::TakeWinSem(m_pSem);

	if(m_PoolOrd>=0&&m_pDB_OO4OPool&&sql) 
		bRlt = ((CDB_OO4OPool*)m_pDB_OO4OPool)->GetSqlFieldData(m_PoolOrd,sql,FieldNum,pRltBuf,RltSize);

	if(m_pSem)
		CTools_Win32::GiveWinSem(m_pSem);

	return bRlt;
}


//	取前2个域的字符串值
//	在odb 上执行sql语句，取前几个结果记录的域值
bool CDB_OO4O::GetSqlFieldData(char *sql,char *pRltBuf1,int RltSize1,char *pRltBuf2,int RltSize2,char *pRltBuf3,int RltSize3,char *pRltBuf4,int RltSize4)
{
	char			*pFieldData[4];
	int				FieldDataSize[4];
	int				FieldNum;
	bool			bRlt=false;

	pFieldData[0]=pRltBuf1;
	FieldDataSize[0]=RltSize1;
	pFieldData[1]=pRltBuf2;
	FieldDataSize[1]=RltSize2;
	pFieldData[2]=pRltBuf3;
	FieldDataSize[2]=RltSize3;
	pFieldData[3]=pRltBuf4;
	FieldDataSize[3]=RltSize4;
	if(pRltBuf4&&RltSize4>0) FieldNum=4;
	else if(pRltBuf3&&RltSize3>0) FieldNum=3;
	else if(pRltBuf2&&RltSize2>0) FieldNum=2;
	else FieldNum=1;

	if(m_pSem)
		CTools_Win32::TakeWinSem(m_pSem);

	if(m_PoolOrd>=0&&m_pDB_OO4OPool&&sql) 
		bRlt = ((CDB_OO4OPool*)m_pDB_OO4OPool)->GetSqlFieldData(m_PoolOrd,sql,FieldNum,pFieldData,FieldDataSize);

	if(m_pSem)
		CTools_Win32::GiveWinSem(m_pSem);

	return bRlt;

}


//	执行sql语句
bool CDB_OO4O::ExecSql(char *sql)
{
	bool			bRlt=false;

	if(m_pSem)
		CTools_Win32::TakeWinSem(m_pSem);

	if(m_PoolOrd>=0&&m_pDB_OO4OPool&&sql) 
		bRlt = ((CDB_OO4OPool*)m_pDB_OO4OPool)->ExecSql(m_PoolOrd,sql);

	if(m_pSem)
		CTools_Win32::GiveWinSem(m_pSem);

	return bRlt;
}

#endif
#endif