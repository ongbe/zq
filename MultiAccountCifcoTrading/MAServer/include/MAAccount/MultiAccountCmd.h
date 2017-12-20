#pragma once

#define Cmd_MA_UnLogon_Rsp			0x100000	//未登录错误返回
#define Cmd_MA_Login_Req			0x100001	//登录请求
#define Cmd_MA_Login_Rsp			0x100002	//登录返回
#define Cmd_MA_Connected_Rsp		0x100003	//连接成功返回
//账号相关接口
#define Cmd_MA_Account_Min			0x100003
#define Cmd_MA_Account_Max			0x1FFFFF
#define Cmd_MA_AddAccount_Req		0x100003	//添加账号请求
#define Cmd_MA_AddAccount_Rsp		0x100004	//添加账号返回

#define Cmd_MA_DelAccount_Req		0x100005	//删除账号请求
#define Cmd_MA_DelAccount_Rsp		0x100006	//删除账号返回

#define Cmd_MA_ModifyPwd_Req		0x100007	//修改密码请求
#define Cmd_MA_ModifyPwd_Rsp		0x100008	//修改密码返回
#define Cmd_MA_ModifyAccount_Req	0x100009	//修改账户基本信息请求
#define Cmd_MA_ModifyAccount_Rsp	0x10000A	//修改账户基本信息返回

#define Cmd_MA_QryALLAccount_Req		0x10000B	//查询所有账号请求
#define Cmd_MA_QryAccountByOwner_Req	0x10000C	//账号查询请求，参数为创建者账号
#define Cmd_MA_QryAccount_Rsp		0x10000D	//账号查询返回

#define Cmd_MA_ForceCloseClient_Rsp	0x10000E	//账号被停用时，强制关闭客户端
#define Cmd_MA_DelAccountCaseCade_Rsp		0x10000F	//级联删除账号返回

//IDCard相关接口
#define Cmd_MA_IDCard_Min			0x200000
#define Cmd_MA_IDCard_Max			0x2FFFFF
#define Cmd_MA_AddIDCard_Req		0x200001	//添加IDCard信息请求
#define Cmd_MA_AddIDCard_Rsp		0x200002	//添加IDCard信息返回

#define Cmd_MA_DelIDCard_Req		0x200003	//删除IDCard信息请求
#define Cmd_MA_DelIDCard_Rsp		0x200004	//删除IDCard信息返回

#define Cmd_MA_ModifyIDCard_Req		0x200005	//修改IDCard信息请求
#define Cmd_MA_ModifyIDCard_Rsp		0x200006	//修改IDCard信息返回

#define Cmd_MA_QryIDCard_Req		0x200007	//查询IDCard信息请求
#define Cmd_MA_QryIDCard_Rsp		0x200008	//查询IDCard信息返回

//服务器相关接口
#define Cmd_MA_Server_Min			0x300000
#define Cmd_MA_Server_Max			0x3FFFFF
#define Cmd_MA_AddServerGroup_Req		0x300001	//添加服务器组信息请求
#define Cmd_MA_AddServerGroup_Rsp		0x300002	//添加服务器组信息返回

#define Cmd_MA_DelServerGroup_Req		0x300003	//删除服务器组信息请求
#define Cmd_MA_DelServerGroup_Rsp		0x300004	//删除服务器组信息返回

#define Cmd_MA_ModifyServerGroup_Req	0x300005	//修改服务器组信息请求
#define Cmd_MA_ModifyServerGroup_Rsp	0x300006	//修改服务器组信息返回

#define Cmd_MA_QryServerGroup_Req		0x300007	//查询服务器组信息请求
#define Cmd_MA_QryServerGroup_Rsp		0x300008	//查询服务器组信息返回

#define Cmd_MA_AddServerAddr_Req		0x300009	//添加服务器地址信息请求
#define Cmd_MA_AddServerAddr_Rsp		0x30000A	//添加服务器地址息返回

#define Cmd_MA_DelServerAddr_Req		0x30000B	//删除服务器地址信息请求
#define Cmd_MA_DelServerAddr_Rsp		0x30000C	//删除服务器地址信息返回

#define Cmd_MA_ModifyServerAddr_Req		0x30000D	//修改服务器地址信息请求
#define Cmd_MA_ModifyServerAddr_Rsp		0x30000E	//修改服务器地址信息返回

#define Cmd_MA_QryServerAddr_Req		0x30000F	//查询服务器地址信息请求
#define Cmd_MA_QryServerAddr_Rsp		0x300010	//查询服务器地址信息返回

//经纪公司相关接口
#define Cmd_MA_Broker_Min			0x400000
#define Cmd_MA_Broker_Max			0x4FFFFF
#define Cmd_MA_AddBroker_Req		0x400001	//添加经纪公司请求
#define Cmd_MA_AddBroker_Rsp		0x400002	//添加经纪公司返回

#define Cmd_MA_DelBroker_Req		0x400003	//删除经纪公司请求
#define Cmd_MA_DelBroker_Rsp		0x400004	//删除经纪公司返回

#define Cmd_MA_ModifyBroker_Req		0x400005	//修改经纪公司信息请求
#define Cmd_MA_ModifyBroker_Rsp		0x400006	//修改经纪公司信息返回

#define Cmd_MA_QryBroker_Req		0x400007	//查询经纪公司信息请求，参数为所有者ID
#define Cmd_MA_QryBroker_Rsp		0x400008	//查询经纪公司信息返回

#define Cmd_MA_QryUsableBroker_Req		0x400009	//查询可用的经纪公司信息请求，参数为前台账户ID
#define Cmd_MA_QryUsableBroker_Rsp		0x40000A	//返回可用的经纪公司信息请求

//交易账号相关接口
#define Cmd_MA_TradeAccount_Min			0x500000
#define Cmd_MA_TradeAccount_Max			0x5FFFFF
#define Cmd_MA_AddTradeAccount_Req		0x500001	//添加交易账号请求
#define Cmd_MA_AddTradeAccount_Rsp		0x500002	//添加交易账号返回

#define Cmd_MA_DelTradeAccount_Req		0x500003	//删除交易账号请求
#define Cmd_MA_DelTradeAccount_Rsp		0x500004	//删除交易账号返回

#define Cmd_MA_ModifyTradeAccount_Req	0x500005	//修改交易账号请求
#define Cmd_MA_ModifyTradeAccount_Rsp	0x500006	//修改交易账号返回

#define Cmd_MA_QryTradeAccount_Req		0x500007	//查询交易账号请求
#define Cmd_MA_QryTradeAccount_Rsp		0x500008	//查询交易账号返回

#define Cmd_MA_SaveTradeAccount_Req		0x500009	//保存账号请求
#define Cmd_MA_SaveTradeAccount_Rsp		0x50000A	//保存账号返回

//权限模块相关接口
#define Cmd_MA_PermissionModule_Min			0x600000
#define Cmd_MA_PermissionModule_Max			0x699999
#define Cmd_MA_AddPermissionModule_Req		0x600001	//添加权限模块请求
#define Cmd_MA_AddPermissionModule_Rsp		0x600002	//添加权限模块返回

#define Cmd_MA_DelPermissionModule_Req		0x600003	//删除权限模块请求
#define Cmd_MA_DelPermissionModule_Rsp		0x600004	//删除权限模块返回

#define Cmd_MA_ModifyPermissionModule_Req	0x600005	//修改权限模块请求
#define Cmd_MA_ModifyPermissionModule_Rsp	0x600006	//修改权限模块返回

#define Cmd_MA_QryPermissionModule_Req		0x600007	//查询权限模块请求
#define Cmd_MA_QryPermissionModule_Rsp		0x600008	//查询权限模块返回

//权限相关接口
#define Cmd_MA_Permission_Min			0x700000
#define Cmd_MA_Permission_Max			0x7FFFFF
#define Cmd_MA_AddPermission_Req		0x700001	//添加权限请求
#define Cmd_MA_AddPermission_Rsp		0x700002	//添加权限返回

#define Cmd_MA_DelPermission_Req		0x700003	//删除权限请求
#define Cmd_MA_DelPermission_Rsp		0x700004	//删除权限返回

#define Cmd_MA_ModifyPermission_Req		0x700005	//修改权限请求
#define Cmd_MA_ModifyPermission_Rsp		0x700006	//修改权限返回

#define Cmd_MA_QryPermissionByOwner_Req		0x700007	//查询权限请求，参数为所有者ID
#define Cmd_MA_QryPermissionByOwner_Rsp		0x700008	//查询权限返回，参数为所有者ID

#define Cmd_MA_QryPermissionByID_Req		0x700009	//查询权限请求，参数为权限ID
#define Cmd_MA_QryPermissionByID_Rsp		0x70000A	//查询权限返回，参数为权限ID

//消息相关接口
#define Cmd_MA_Message_Min				0x800000
#define Cmd_MA_Message_Max				0x8FFFFF
#define Cmd_MA_AddMessage_Req			0x800001	//添加消息请求
#define Cmd_MA_AddMessage_Rsp			0x800002	//添加消息响应
#define Cmd_MA_QryMessage_Req			0x800003	//查询消息请求
#define Cmd_MA_QryMessage_Rsp			0x800004	//查询消息请求
#define Cmd_MA_QryTargetAccount_Req		0x800005	//查询消息的目标账户列表
#define Cmd_MA_QryTargetAccount_Rsp		0x800006	//返回消息的目标账户列表
#define Cmd_MA_QryUnReadMessage_Req		0x800007	//查询未阅读的消息
#define Cmd_MA_SendUnReadMessage_Rsp	0x800008	//发送未阅读的消息
#define Cmd_MA_QryWaitForSendAccount_Req	0x800009	//查询未送达账户列表
#define Cmd_MA_QryWaitForSendAccount_Rsp	0x80000A	//返回未送达账户列表

#define OPERATOR_SUCCESS	1		//请求成功
#define OPERATOR_FAILED		0		//请求失败