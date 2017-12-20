#pragma once
#include "Interface_SvrInstruments.h"

class CCustomInterfaceSvrInstruments : public CInterface_SvrInstruments
{
	//下面getObj()、构造/析构方法和m_pObj用于定义单实例类
public:
	//用于获取接口对象，通过该对象使用接口方法
	static CCustomInterfaceSvrInstruments& getObj(void);
private:
	CCustomInterfaceSvrInstruments();
	virtual ~ CCustomInterfaceSvrInstruments();
	static CCustomInterfaceSvrInstruments* m_pObj;
private:
	_MapInstrumentType mMapInstruments;
public:
	//---------------------------------------------------------------------------------
	//	重置所有合约
	//---------------------------------------------------------------------------------
	virtual CF_ERROR WriteAllInstruments(const _MapInstrumentType & nMapInstruments);


	//---------------------------------------------------------------------------------
	//	获取所有合约
	//---------------------------------------------------------------------------------
	virtual CF_ERROR GetAllInstruments( _MapInstrumentType & nMapInstruments);


	//---------------------------------------------------------------------------------
	//	获取单个合约的具体信息
	//---------------------------------------------------------------------------------
	virtual CF_ERROR GetInstrumentField( const std::string& nInstrumentID,
		InstrumentField & nVecInstruments) ;

	//---------------------------------------------------------------------------------
	//	追加或更新单条合约
	//  返回： CF_ERROR_SUCCESS 代表成功
	//         CF_ERROR_COMMON_MEMEORY 代表内存错误，没有添加成功
	//---------------------------------------------------------------------------------
	virtual CF_ERROR AppendInstrument( const InstrumentField & nVecInstruments) ;

private:
	virtual CF_ERROR WriteSingleInstrumentToDB(const InstrumentField & nInstruments);
};