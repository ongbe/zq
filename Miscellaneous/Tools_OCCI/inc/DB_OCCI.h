#pragma once

#include <comutil.h>
#include <string>
#include <vector>
using std::string;
using std::vector;
#include "easymutex.h"
#include "EasyTime.h"


//批量读写时，完成一批后的回调程序，让调用程序有机会设置下一批数据/读取本批数据，再执行下一批处理
//BatchNo: 下一批批次，从零开始。一共调用n+1次。准备读写前以BatchNo==0先调用一次，然后每完成一批调用一次
//ThisTimeRWCount: 本批完成读写的记录数量
//NextTimeRWCount: 下一批要读写的数量
//UserData: 用户定义字，回调时带回。用于调用者区分上下文
//返回是否进行下一批读写。true: 执行下一批读写; false: 停止读写
typedef bool (*BatchDBRWProcessFunc_OCCI)(int BatchNo,int ThisTimeRWCount,int& NextTimeRWCount,unsigned int UserData);

//批量读写时，字段类型。和oracle的Type值对应，可以直接在setDataBuffer中使用
enum FieldType_DB_OCCI
{
	eOCCIINT=3,
	eOCCI_SQLT_STR=5,
	eOCCIFLOAT=4,
};

class CDB_OCCI
{
public:
	CDB_OCCI(const string& username,const string& pw,const string& connstr);
	virtual ~CDB_OCCI(void);

	virtual void Init(void);
	virtual void Release(void);

	virtual void Connect(void);
	virtual void Disconn(void);

	/**
	* @功能描述: 执行查询SQL语句，并返回结果集
	* @参数列表: pSql :查询SQL语句
	* @参数列表: rtnvec :返回结果集
	* @参数列表: ErrCode/ErrMsg :返回错误码/错误信息
	* @返 回 值: true,执行成功 false,执行失败
	**/
	virtual bool QueryData( const char* pSql, std::vector<std::vector<_variant_t>>& rtnvec, int& ErrCode, string& ErrMsg );

	/**
	* @功能描述: 执行更新、删除SQL语句，返回影响的记录条数
	* @参数列表: pSql :SQL语句
	* @参数列表: AffectedCount :受影响的记录条数
	* @参数列表: ErrCode/ErrMsg :错误码/错误信息
	* @返 回 值: true,执行成功 false,执行失败
	**/
	virtual bool ExcuteUpdate( const char* pSql, int& AffectedCount, int& ErrCode, string& ErrMsg );

	/**
	* @功能描述: 执行任意SQL语句
	* @参数列表: pSql :SQL语句
	* @参数列表: ErrCode/ErrMsg :错误码/错误信息
	* @返 回 值: true,执行成功 false,执行失败
	**/
	virtual bool Excute( const char* pSql, int& ErrCode, string& ErrMsg );

	/**
	* @功能描述: 执行查询SQL语句，返回查询结果的首记录的第一个字段的整型值。一般查询select count(*) from...
	* @参数列表: pSql :SQL语句
	* @参数列表: FirstRtnValue: 查询结果的首记录的第一个字段的整型值
	* @参数列表: ErrCode/ErrMsg :错误码/错误信息
	* @返 回 值: true,执行成功 false,执行失败
	**/
	virtual bool ExcuteSelectCount( const char* pSql, int& FirstRtnValue, int& ErrCode, string& ErrMsg );

	/**
	* @功能描述: 插入一条数据，并返回插入记录的ID
	* @参数列表: pSql :插入数据SQL语句
	* @参数列表: pGetIDSql :查询可用sequnce id的SQL语句
	* @参数列表: rtnID : 返回插入记录的ID
	* @参数列表: ErrCode/ErrMsg :错误码/错误信息
	* @返 回 值: true,执行成功 false,执行失败
	**/
	virtual bool InsertAndReturnID( const char* pSql, const char* pGetIDSql, int& rtnID, int& ErrCode, string& ErrMsg );

	/**
	* @功能描述: 执行批量读取数据
	* @参数列表: bQuery : 是否是读取。true表示批量读取，执行rset->getNumArrayRows；false表示批量更新，执行pSmt->executeArrayUpdate
	* @参数列表: pSql : 读的SQL语句
	* @参数列表: fieldnum : 字段/参数的数量
	* @参数列表: buffers : 数据指针数组，应该是数组void* [fieldnum]
	* @参数列表: types : 字段类型数组。应该是数组FieldType_DB_OCCI[fieldnum]
	* @参数列表: sizes : 字段缓冲区尺寸数组，应该是int[fieldnum]
	* @参数列表: lens :  字段数据长度数组，应该是unsigned short*[fieldnum]。int类型应该是sizeof(int); 字符串类型应该是strlen()。此值可以是NULL
	* @参数列表: cbfunc : 读写过程中的回调函数。开始读写前和每次读写后都会执行此回调函数
	* @参数列表: UserData: 用户定义字，回调时带回。用于调用者区分上下文
	* @参数列表: ErrCode/ErrMsg :错误码/错误信息
	* @返 回 值: true,执行成功 false,执行失败
	**/
	virtual bool BatchQuery( const char* pSql, int fieldnum,void**buffers, FieldType_DB_OCCI* types,int* sizes, unsigned short** lens, BatchDBRWProcessFunc_OCCI cbfunc,unsigned int UserData,int& ErrCode, string& ErrMsg);

	/**
	* @功能描述: 执行批量更新数据
	* @参数列表: pSql : 写的SQL语句。应该类似于：insert into dbname values(:v1,:v2,:v3,:v4,:v5,:v6,:v7,:v8,:v9,:v10)
	* @参数列表: fieldnum : 参数的数量
	* @参数列表: buffers : 数据指针数组，应该是数组void* [fieldnum]
	* @参数列表: types : 字段类型数组。应该是数组FieldType_DB_OCCI[fieldnum]
	* @参数列表: sizes : 字段缓冲区尺寸数组，应该是int[fieldnum]
	* @参数列表: lens :  字段数据长度数组，应该是unsigned short*[fieldnum]。int类型应该是sizeof(int); 字符串类型应该是strlen()
	* @参数列表: cbfunc : 读写过程中的回调函数。开始读写前和每次读写后都会执行此回调函数
	* @参数列表: UserData: 用户定义字，回调时带回。用于调用者区分上下文
	* @参数列表: ErrCode/ErrMsg :错误码/错误信息
	* @返 回 值: true,执行成功 false,执行失败
	**/
	virtual bool BatchUpdate( const char* pSql, int fieldnum,void**buffers, FieldType_DB_OCCI* types,int* sizes, unsigned short** lens, BatchDBRWProcessFunc_OCCI cbfunc,unsigned int UserData,int& ErrCode, string& ErrMsg);
	/**
	* @功能描述: 执行批量更新数据
	* @参数列表: UpdateCount :  需要更新的记录数量
	**/
	virtual bool BatchUpdate( const char* pSql, int fieldnum,void**buffers, FieldType_DB_OCCI* types,int* sizes, unsigned short** lens, int UpdateCount,int& ErrCode, string& ErrMsg);

private:
	Ceasymutex m_mutex;

	void 			*m_pEnv;
	void 			*m_pConn;

	string 			m_username;
	string			m_pw;
	string			m_connstr;

	CEasyTime		m_TryInitTime;
	CEasyTime		m_ReleaseTime;
	CEasyTime		m_TryConnTime;
	CEasyTime		m_DisconnTime;



	void Connect_Internal(void);
	void Disconn_Internal(void);
	void CheckConn_Internal(void);
	
	bool CheckParam_Init_Internal(const char* pSql, int& ErrCode, string& ErrMsg );
	bool ReleaseStmt_Rset_Internal(void* pStmt,void* pRes);
	void Rollback_Internal();



	//这个类禁止拷贝
private:
	CDB_OCCI(const CDB_OCCI&);
	CDB_OCCI& operator= (const CDB_OCCI&);

};
