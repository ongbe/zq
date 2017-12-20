#pragma  once
#include "CommonErrorCode.h"
#include "CommonDataChangeEvent.h"
//各业务模块接口类的公共父类。
//这是个纯虚类，用纯虚方法的方式定义各业务模块接口类必须要实现的方法

class __declspec(dllexport) CInterface_SvrCommon
{
public:

	///************************************************************************/
	////函数ResponseToAsk
	////功能说明：该函数可以让你的模块可以拦截某种数据操作，如果想拦截请重写该函数。如果不想拦截，可以忽略该函数
	////注意：    该函数内部实现的时候要求不能调用其它模块的接口函数，只能调用内部函数。
	////参数：    neDataType   为数据的的类型
	////          neChangeType 为数据的操作的类型
	////          nllDataID    为数据的唯一ID
	////返回：    如果本模块不想阻止该数据的操作，返回CF_ERROR_SUCCESS
	////          如果想阻止该数据的操作，返回其他错误码
	///备注：	  一票否决
	///************************************************************************/
	virtual CF_ERROR ResponseToAsk(
		const eAskType& neDataType,
		const eAskOperationType& neOprType,
		const int nllDataID,
		const char * npDataBuf) 
	{
		return CF_ERROR_SUCCESS;
	}
	///************************************************************************/
	///备注：	  一票通过。通过时返回CF_ERROR_SUCCESS，pRlt可以是任意类型的指针，用于应答者返回数据
	///************************************************************************/
	virtual CF_ERROR ResponseToAsk2(
		const eAskType& neDataType,
		const eAskOperationType& neOprType,
		const int nllDataID,
		const char * npDataBuf,
		void* pRlt=NULL) 
	{
		return CF_ERROR_FAILURE;
	}


	////如下定义各业务模块必须要实现的接口方法
	///************************************************************************/
	////函数PreFilterPackage
	////功能说明：该函数使得在真正的请求命令处理服务模块之前有机会先处理这个包
	////注意：    该函数内部实现的时候要求不能调用其它模块的接口函数，只能调用内部函数。
	////参数：    npPackage 为请求包
	////          pConnection 为该次请求用到的数据库连接
	////          pStatament 为该次请求用到的数据库事务
	////返回：    该函数如果想阻止该包继续传递到下一个模块，请返回一个错误码
	////          如果不想阻止该包传递，请返回CF_ERROR_SUCCESS
	///************************************************************************/
	//virtual CF_ERROR PreFilterPackage(void * npPackage,void* pConnection,void* pStatement)
	//{
	//	return CF_ERROR_SUCCESS;
	//}

	///************************************************************************/
	////函数PostFilterPackage
	////功能说明：该函数使得在真正的请求命令处理服务模块之后有机会先处理这个包
	////注意：    该函数内部实现的时候要求不能调用其它模块的接口函数，只能调用内部函数。   
	////参数：    npPackage 为请求包
	////          pConnection 为该次请求用到的数据库连接
	////          pStatament 为该次请求用到的数据库事务
	////返回：    该函数如果想阻止该包继续传递到下一个模块，请返回一个错误码
	////          如果不想阻止该包传递，请返回CF_ERROR_SUCCESS
	//virtual CF_ERROR PostFilterPackage(void * npPackage,void* pConnection,void* pStatement) 
	//{
	//	return CF_ERROR_SUCCESS;
	//}
};