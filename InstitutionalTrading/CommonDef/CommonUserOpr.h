#pragma once

#include "FileOpr.h"
#include "ConfigMgr.h"

#if 0
#define LOG_INFO(fmt, ...)  ;
#else
#define LOG_INFO(fmt, ...)  CFileOpr::getObj().writelocallogDaily("log","CltServerMgr", "[%s][%d]:"fmt, __FILE__, __LINE__, __VA_ARGS__);
#endif

//此文件记录所有终端用户的操作
#define  MAKE_ERROR_CODE(_base,_offset) ((int)( (int)(_base) << 16 ) + (int)(_offset))

//后台管理终端
#define USER_OPR_SYSTEM				0x0010   //系统操作
#define USER_OPR_ACCOUNT			0x0020   //账户操作
#define USER_OPR_MANAGE				0x0030   //管理操作
#define USER_OPR_STRATEGY			0x0040   //策略操作
#define USER_OPR_PRIVILEGE			0x0050   //权限操作
#define USER_OPR_SETTLEMENT			0x0060   //结算操作
#define USER_OPR_QUERY			    0x0070   //查询操作
#define USER_OPR_MESSAGE			0x0080   //消息操作
#define USER_OPR_CONTROL			0x0090   //控制操作

//系统操作
#define USER_OPR_SYSTEM_EXIT    MAKE_ERROR_CODE(USER_OPR_SYSTEM,0x0001)  //退出窗口

//账户操作
#define USER_OPR_ACCOUNT_SYSTEMUSER_ADD             MAKE_ERROR_CODE(USER_OPR_ACCOUNT,0x0001)  //增加一个系统用户
#define USER_OPR_ACCOUNT_SYSTEMUSER_MODIFY          MAKE_ERROR_CODE(USER_OPR_ACCOUNT,0x0002)  //修改一个系统用户
#define USER_OPR_ACCOUNT_SYSTEMUSER_DEL             MAKE_ERROR_CODE(USER_OPR_ACCOUNT,0x0003)  //删除一个系统用户
#define USER_OPR_ACCOUNT_SYSTEMUSER_USE             MAKE_ERROR_CODE(USER_OPR_ACCOUNT,0x0004)  //启/停用一个系统用户
#define USER_OPR_ACCOUNT_SYSTEMUSER_INIT            MAKE_ERROR_CODE(USER_OPR_ACCOUNT,0x0005)  //初始化一个系统用户
#define USER_OPR_ACCOUNT_SYSTEMUSER_ASSIGNORG       MAKE_ERROR_CODE(USER_OPR_ACCOUNT,0x0006)  //一个系统用户分配组织机构
#define USER_OPR_ACCOUNT_SYSTEMUSER_ASSIGNPRODUCT   MAKE_ERROR_CODE(USER_OPR_ACCOUNT,0x0007)  //一个系统用户分配理财产品
#define USER_OPR_ACCOUNT_SYSTEMUSER_ASSIGNACCOUNT   MAKE_ERROR_CODE(USER_OPR_ACCOUNT,0x0008)  //一个系统用户指定委托交易账号

#define USER_OPR_ACCOUNT_INOUTFUND_QUERY    MAKE_ERROR_CODE(USER_OPR_ACCOUNT,0x0011)  //查询出入金
#define USER_OPR_ACCOUNT_INOUTFUND_IN       MAKE_ERROR_CODE(USER_OPR_ACCOUNT,0x0012)  //给一个账户入金
#define USER_OPR_ACCOUNT_INOUTFUND_OUT      MAKE_ERROR_CODE(USER_OPR_ACCOUNT,0x0013)  //给一个账户出金

#define USER_OPR_ACCOUNT_ACCOUNT_ADD             MAKE_ERROR_CODE(USER_OPR_ACCOUNT,0x0021)  //增加一个委托交易账号
#define USER_OPR_ACCOUNT_ACCOUNT_MODIFY          MAKE_ERROR_CODE(USER_OPR_ACCOUNT,0x0022)  //修改一个委托交易账号
#define USER_OPR_ACCOUNT_ACCOUNT_DEL             MAKE_ERROR_CODE(USER_OPR_ACCOUNT,0x0023)  //删除一个委托交易账号
#define USER_OPR_ACCOUNT_ACCOUNT_LOGIN           MAKE_ERROR_CODE(USER_OPR_ACCOUNT,0x0024)  //登录一个委托交易账号
//管理操作
#define USER_OPR_MANAGE_PRODUCT_ADD         MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0001)  //增加一个理财产品
#define USER_OPR_MANAGE_PRODUCT_MODIFY      MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0002)  //修改一个理财产品
#define USER_OPR_MANAGE_PRODUCT_DEL         MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0003)  //删除一个理财产品
#define USER_OPR_MANAGE_PRODUCT_SAVE        MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0004)  //保存一个理财产品和交易员的关系

#define USER_OPR_MANAGE_BROKER_ADD         MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0011)  //增加一个经纪公司
#define USER_OPR_MANAGE_BROKER_MODIFY      MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0012)  //修改一个经纪公司
#define USER_OPR_MANAGE_BROKER_DEL         MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0013)  //删除一个经纪公司
#define USER_OPR_MANAGE_SERVERGROUP_ADD         MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0021)  //增加一个服务器组
#define USER_OPR_MANAGE_SERVERGROUP_MODIFY      MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0022)  //修改一个服务器组
#define USER_OPR_MANAGE_SERVERGROUP_DEL         MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0023)  //删除一个服务器组
#define USER_OPR_MANAGE_SERVERADDR_ADD         MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0031)  //增加一个服务器组地址
#define USER_OPR_MANAGE_SERVERADDR_MODIFY      MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0032)  //修改一个服务器组地址
#define USER_OPR_MANAGE_SERVERADDR_DEL         MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0033)  //删除一个服务器组地址
#define USER_OPR_MANAGE_QUOTSERVER_CONFIG    MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0041)   //配置行情服务器
#define USER_OPR_MANAGE_QUOTADDR_ADD         MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0051)  //增加一个行情地址
#define USER_OPR_MANAGE_QUOTADDR_MODIFY      MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0052)  //修改一个行情地址
#define USER_OPR_MANAGE_QUOTADDR_DEL         MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0053)  //删除一个行情地址

#define USER_OPR_MANAGE_ORGANIZE_ADD         MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0061)  //增加一个组织机构
#define USER_OPR_MANAGE_ORGANIZE_MODIFY      MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0062)  //修改一个组织机构
#define USER_OPR_MANAGE_ORGANIZE_DEL         MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0063)  //删除一个组织机构
#define USER_OPR_MANAGE_ORGANIZE_SAVE        MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0064)  //保存一个组织机构和交易员的关系
//策略操作
#define USER_OPR_STRATEGY_ADD         MAKE_ERROR_CODE(USER_OPR_STRATEGY,0x0001)  //增加一个交易策略
#define USER_OPR_STRATEGY_MODIFY      MAKE_ERROR_CODE(USER_OPR_STRATEGY,0x0002)  //修改一个交易策略
#define USER_OPR_STRATEGY_DEL         MAKE_ERROR_CODE(USER_OPR_STRATEGY,0x0003)  //删除一个交易策略
#define USER_OPR_STRATEGY_SAVE        MAKE_ERROR_CODE(USER_OPR_STRATEGY,0x0004)  //保存一个交易策略和组织机构的关系
//权限操作
#define USER_OPR_PRIVILEGE_ADD         MAKE_ERROR_CODE(USER_OPR_PRIVILEGE,0x0001)  //增加一个角色
#define USER_OPR_PRIVILEGE_MODIFY      MAKE_ERROR_CODE(USER_OPR_PRIVILEGE,0x0002)  //修改一个角色
#define USER_OPR_PRIVILEGE_DEL         MAKE_ERROR_CODE(USER_OPR_PRIVILEGE,0x0003)  //删除一个角色
#define USER_OPR_PRIVILEGE_SAVE        MAKE_ERROR_CODE(USER_OPR_PRIVILEGE,0x0004)  //保存一个角色和权限的关系
//结算操作

//查询操作

//消息操作

//控制操作
#define USER_OPR_CONTROL_TRADESERVERINIT            MAKE_ERROR_CODE(USER_OPR_CONTROL,0x0001)  //交易服务器初始化
#define USER_OPR_CONTROL_TRADETIMESET               MAKE_ERROR_CODE(USER_OPR_CONTROL,0x0002)  //交易时间段设置
#define USER_OPR_CONTROL_TRADESERVERTIMEAUTOINIT    MAKE_ERROR_CODE(USER_OPR_CONTROL,0x0003)  //交易服务器定时初始化时间设置
#define USER_OPR_CONTROL_QUOTSERVERINIT             MAKE_ERROR_CODE(USER_OPR_CONTROL,0x0004)  //行情服务器初始化
#define USER_OPR_CONTROL_QUOTSERVERTIMEAUTOINIT     MAKE_ERROR_CODE(USER_OPR_CONTROL,0x0005)  //行情服务器初定时重启时间设置
#define USER_OPR_CONTROL_SIMULATEQUOTSERVERSETUP    MAKE_ERROR_CODE(USER_OPR_CONTROL,0x0006)  //模拟行情服务器设置

///////////////////////////////////////////////////////////////////////////////
//格式化错误代码为错误字符串FormatErrorCode
//Input: nErrorCode 为具体的错误码
//Output: 如果有则返回具体的错误字符串
//        如果没有找到对应的错误码，则返回空串
/////////////////////////////////////////////////////////////////////////////////
inline const char * FormatOprCode(const int nErrorCode)
{
    switch(nErrorCode)
    {
        //系统操作
    case USER_OPR_SYSTEM_EXIT:                         return "退出窗口";

        //账户操作
    case USER_OPR_ACCOUNT_SYSTEMUSER_ADD:              return  "增加一个系统用户";
    case USER_OPR_ACCOUNT_SYSTEMUSER_MODIFY:           return  "修改一个系统用户";
    case USER_OPR_ACCOUNT_SYSTEMUSER_DEL:              return  "删除一个系统用户";
    case USER_OPR_ACCOUNT_SYSTEMUSER_USE:              return  "启/停用一个系统用户";
    case USER_OPR_ACCOUNT_SYSTEMUSER_INIT:             return  "初始化一个系统用户";
    case USER_OPR_ACCOUNT_SYSTEMUSER_ASSIGNORG:        return  "一个系统用户分配组织机构";
    case USER_OPR_ACCOUNT_SYSTEMUSER_ASSIGNPRODUCT:    return  "一个系统用户分配理财产品";
    case USER_OPR_ACCOUNT_SYSTEMUSER_ASSIGNACCOUNT:    return  "一个系统用户指定委托交易账号";

    case USER_OPR_ACCOUNT_INOUTFUND_QUERY:             return  "查询出入金";
    case USER_OPR_ACCOUNT_INOUTFUND_IN:                return  "给一个账户入金";
    case USER_OPR_ACCOUNT_INOUTFUND_OUT:               return  "给一个账户出金";

    case USER_OPR_ACCOUNT_ACCOUNT_ADD:                 return  "增加一个委托交易账号";
    case USER_OPR_ACCOUNT_ACCOUNT_MODIFY:              return  "修改一个委托交易账号";
    case USER_OPR_ACCOUNT_ACCOUNT_DEL:                 return  "删除一个委托交易账号";
    case USER_OPR_ACCOUNT_ACCOUNT_LOGIN:               return  "登录一个委托交易账号";
        //管理操作
    case USER_OPR_MANAGE_PRODUCT_ADD:                  return  "增加一个理财产品";
    case USER_OPR_MANAGE_PRODUCT_MODIFY:               return  "修改一个理财产品";
    case USER_OPR_MANAGE_PRODUCT_DEL:                  return  "删除一个理财产品";
    case USER_OPR_MANAGE_PRODUCT_SAVE:                 return  "保存一个理财产品和交易员的关系";

    case USER_OPR_MANAGE_BROKER_ADD:                   return  "增加一个经纪公司";
    case USER_OPR_MANAGE_BROKER_MODIFY:                return  "修改一个经纪公司";
    case USER_OPR_MANAGE_BROKER_DEL:                   return  "删除一个经纪公司";
    case USER_OPR_MANAGE_SERVERGROUP_ADD:              return  "增加一个服务器组";
    case USER_OPR_MANAGE_SERVERGROUP_MODIFY:           return  "修改一个服务器组";
    case USER_OPR_MANAGE_SERVERGROUP_DEL:              return  "删除一个服务器组";
    case USER_OPR_MANAGE_SERVERADDR_ADD:               return  "增加一个服务器组地址";
    case USER_OPR_MANAGE_SERVERADDR_MODIFY:            return  "修改一个服务器组地址";
    case USER_OPR_MANAGE_SERVERADDR_DEL:               return  "删除一个服务器组地址";
    case USER_OPR_MANAGE_QUOTSERVER_CONFIG:            return   "配置行情服务器";
    case USER_OPR_MANAGE_QUOTADDR_ADD:                 return  "增加一个行情地址";
    case USER_OPR_MANAGE_QUOTADDR_MODIFY:              return  "修改一个行情地址";
    case USER_OPR_MANAGE_QUOTADDR_DEL:                 return  "删除一个行情地址";

    case USER_OPR_MANAGE_ORGANIZE_ADD:                 return  "增加一个组织机构";
    case USER_OPR_MANAGE_ORGANIZE_MODIFY:              return  "修改一个组织机构";
    case USER_OPR_MANAGE_ORGANIZE_DEL:                 return  "删除一个组织机构";
    case USER_OPR_MANAGE_ORGANIZE_SAVE:                return  "保存一个组织机构和交易员的关系";
        //策略操作
    case USER_OPR_STRATEGY_ADD:                        return  "增加一个交易策略";
    case USER_OPR_STRATEGY_MODIFY:                     return  "修改一个交易策略";
    case USER_OPR_STRATEGY_DEL:                        return  "删除一个交易策略";
    case USER_OPR_STRATEGY_SAVE:                       return  "保存一个交易策略和组织机构的关系";
        //权限操作
    case USER_OPR_PRIVILEGE_ADD:                        return  "增加一个角色";
    case USER_OPR_PRIVILEGE_MODIFY:                     return  "修改一个角色";
    case USER_OPR_PRIVILEGE_DEL:                        return  "删除一个角色";
    case USER_OPR_PRIVILEGE_SAVE:                       return  "保存一个角色和权限的关系";
        //结算操作

        //查询操作

        //消息操作

        //控制操作
    case USER_OPR_CONTROL_TRADESERVERINIT:            return  "交易服务器初始化";
    case USER_OPR_CONTROL_TRADETIMESET:               return  "交易时间段设置";
    case USER_OPR_CONTROL_TRADESERVERTIMEAUTOINIT:    return  "交易服务器定时初始化时间设置";
    case USER_OPR_CONTROL_QUOTSERVERINIT:             return  "行情服务器初始化";
    case USER_OPR_CONTROL_QUOTSERVERTIMEAUTOINIT:     return  "行情服务器初定时重启时间设置";
    case USER_OPR_CONTROL_SIMULATEQUOTSERVERSETUP:    return  "模拟行情服务器设置";
    default:	return "";
    }
}