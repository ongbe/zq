// DB_OO4O.h: interface for the CDB_OO4O class.
//
//	
//	FileName	:db_oo4o.h
//	Description	:1. 实现oo4o数据库操作
//	Include		:oracl.h
//	Lib			:oraclm32.lib
//	Platform	:windows
//
//	Author		:lzq
//				 Copyright (C), 2008-2028
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DB_OO4O_H__1821CB9E_5237_4309_BADD_31B7F7D35F63__INCLUDED_)
#define AFX_DB_OO4O_H__1821CB9E_5237_4309_BADD_31B7F7D35F63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define				UseDB_OO4O


#ifdef				UseDB_OO4O

#include "Tools_Win32.h"


class CDB_OO4O  
{
public:
	static void		Regist(void);												//在使用这个类之前，需要注册一下，注意，单线程使用
	static void		Unregist(void);												//在使用完这个类之前，需要注销一下，注意，单线程使用

	CDB_OO4O();
	CDB_OO4O(char *db_sSource,char *db_sUser,char *db_sPasswd);
	virtual ~CDB_OO4O();

	bool			ConnectDB(char *db_sSource,char *db_sUser,char *db_sPasswd);//连接数据库，得到的数据库句柄放在odb中 连接时使用指定的数据库名称、用户名和密码
	void			DisconnectDB(void);											//断开数据库连接
	bool			GetConnectOK(void);
	int				GetLastError(char* pRltErrText,int ErrTextSize);			//取最后的错误，错误文本放在pRltErrText中，返回错误号

	//注意：下面6个函数要在单线程中使用
	char*			OpenRSet(char *pSql);										//打开RecordSet，返回记录集的指针
	void			CloseRSet(char *pRSet);										//关闭RecordSet
	void			MoveNext(char *pRSet);										//移到下一个记录
	bool			IsEOF(char *pRSet);											//判断是否到了记录集的最后
	bool			GetFieldData(char *pRSet,int FieldNum,char **pRltBuf,int *RltSize);//取前几个域的字符串值 在odb 上执行sql语句，取前几个结果记录的域值
	bool			GetFieldData(char *pRSet,char *pRltBuf1,int RltSize1,char *pRltBuf2=NULL,int RltSize2=0,char *pRltBuf3=NULL,int RltSize3=0,char *pRltBuf4=NULL,int RltSize4=0);//取前2个域的字符串值 在odb 上执行sql语句，取前几个结果记录的域值


	bool			GetSqlFieldData(char *sql,int FieldNum,char **pRltBuf,int *RltSize);//取前几个域的字符串值 在odb 上执行sql语句，取前几个结果记录的域值
	bool			GetSqlFieldData(char *sql,char *pRltBuf1,int RltSize1,char *pRltBuf2=NULL,int RltSize2=0,char *pRltBuf3=NULL,int RltSize3=0,char *pRltBuf4=NULL,int RltSize4=0);//取前2个域的字符串值 在odb 上执行sql语句，取前几个结果记录的域值
	bool			ExecSql(char *sql);											//执行sql语句



private:

	static char*	m_pDB_OO4OPool;												//这是一个类成员变量，所有对象共有

	int				m_PoolOrd;
	char			*m_pRSet;													//记录集的指针
	char			*m_pSem;

};


#endif


#endif // !defined(AFX_DB_OO4O_H__1821CB9E_5237_4309_BADD_31B7F7D35F63__INCLUDED_)
