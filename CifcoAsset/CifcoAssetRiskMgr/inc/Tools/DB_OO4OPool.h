// DB_OO4OPool.h: interface for the CDB_OO4OPool class.
//
//	
//	FileName	:db_oo4opool.h
//	Description	:1. 实现oo4o数据库连接池操作
//	Include		:oracl.h
//	Lib			:oraclm32.lib
//	Platform	:windows
//
//	Author		:lzq
//				 Copyright (C), 2008-2028
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DB_OO4OPOOL_H__77FB0FA4_D29B_44F7_BC83_A47453B2642F__INCLUDED_)
#define AFX_DB_OO4OPOOL_H__77FB0FA4_D29B_44F7_BC83_A47453B2642F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define				UseDB_OO4OPOOL

#ifdef				UseDB_OO4OPOOL


#include "public.h"

#define OdbPoolStrSize	128														//odb的pool中字符串的尺寸


struct PoolDataStr																//连接池的数据
{
	char			*pOdb;
	int				RefCount;													//对本连接的引用次数
	char			Odb_Source[OdbPoolStrSize];
	char			Odb_User[OdbPoolStrSize];
	char			Odb_PW[OdbPoolStrSize];
	DWORD			Odb_OperateTime;											//最近操作时间
	DWORD			Odb_CloseTime;												//关闭时间
};


class CDB_OO4OPool  
{
public:
	CDB_OO4OPool(int PoolCount=10,int IdleTimeOut=30);
	virtual ~CDB_OO4OPool();

	int				ConnectDB(char *db_sSource,char *db_sUser,char *db_sPasswd);//连接数据库 返回数据库连接的指针
	bool			DisconnectDB(int PoolOrd);									//断开数据库连接 在OdbPool中找到相应的odb，并且设置关闭时间
	bool			GetConnectOK(int PoolOrd);									//查询数据库是否正处于连接状态
	int				GetLastError(int PoolOrd,char* pRltErrText,int ErrTextSize);//取最后的错误，错误文本放在pRltErrText中，返回错误号

	//注意：下面5个函数要在单线程中使用
	char*			OpenRSet(int PoolOrd,char* pSql);							//打开RecordSet，返回记录集的指针
	void			CloseRSet(char* pRSet);										//关闭RecordSet
	void			MoveNext(char* pRSet);										//移到下一个记录
	bool			IsEOF(char* pRSet);											//判断是否到了记录集的最后
	bool			GetFieldData(char* pRSet,int FieldNum,char **pRltBuf,int *RltSize);//取前几个域的字符串值 在odb 上执行sql语句，取前几个结果记录的域值

	bool			GetSqlFieldData(int PoolOrd,char *sql,int FieldNum,char **pRltBuf,int *RltSize);//取前几个域的字符串值 在odb 上执行sql语句，取前几个结果记录的域值
	bool			ExecSql(int PoolOrd,char *sql);								//执行sql语句



private:
	PoolDataStr		*m_pPool;

	int				m_PoolCount;
	int				m_IdleTimeOut;
	char			*m_pSem;

};



#endif

#endif // !defined(AFX_DB_OO4OPOOL_H__77FB0FA4_D29B_44F7_BC83_A47453B2642F__INCLUDED_)
