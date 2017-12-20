#pragma once
//请在这个头文件中定义客户端请求服务器的command ID

//每个客户端发向服务器的请求包需要在包头的userdata1的地方填写userid，登陆请求除外

#define  MAKE_CMDID(_base,_offset) \
	((int)( (int)(_base) << 16 ) + (int)(_offset))



#define  CMDID_BASE_HIWORD_TCP						0x0000 //Tcp服务高位CMDID
#define  CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT	0x0001 //经纪公司服务高位CMDID
#define  CMDID_BASE_HIWORD_DATA_USER_AND_ORG		0x0002 //用户组织机构服务高位CMDID
#define  CMDID_BASE_HIWORD_DATA_FINANCIAL_PRODUCT	0x0003 //理财产品服务高位CMDID
#define  CMDID_BASE_HIWORD_DATA_TRADER_STRATEGY		0x0004 //交易策略服务高位CMDID
#define  CMDID_BASE_HIWORD_SESSION_LOGIN			0x0005 //登陆服务高位CMDID
#define  CMDID_BASE_HIWORD_MESSAGE_NOTIFY			0x0006 //消息服务高位CMDID
#define  CMDID_BASE_HIWORD_BUSSINESS_OFFER			0x0007 //报盘服务高位CMDID
#define  CMDID_BASE_HIWORD_BUSSINESS_TRADER_DATA	0x0008 //交易数据服务高位CMDID
#define  CMDID_BASE_HIWORD_DATA_INSTRUMENTS			0x0009 //品种数据服务高位CMDID
#define  CMDID_BASE_HIWORD_DATA_RATIO				0x000A //费率数据服务高位CMDID
#define  CMDID_BASE_HIWORD_DATA_SETTLEMENT			0x000B //结算服务高位CMDID
#define  CMDID_BASE_HIWORD_TRADE_EXCUTE				0x000C //交易执行高位CMDID
#define  CMDID_BASE_HIWORD_TRADE_DATA_MANAGER		0x000D //交易数据管理高位CMDID
#define  CMDID_BASE_HIWORD_RISK						0x000E //风控高位CMDID
#define  CMDID_BASE_HIWORD_QUOT						0x000F //行情高位CMDID
#define  CMDID_BASE_HIWORD_STATIC_PRESENTATION		0x0011 //统计展现高位CMDID
#define  CMDID_BASE_HIWORD_DATA_ACCOUNT_AND_ORG		0x0012 //组织机构委托交易账号高位CMDID
#define  CMDID_BASE_STRATEDY						0x0013 //策略
#define  CMDID_BASE_NotifyAndAsk					0x0014 //通知和问询模块消息

//-----------------------------------------------------------------------------------
//	下面定义常量
//-----------------------------------------------------------------------------------
//通知订阅的线程，有消息到达，lParam=EventParamID，EventParam的pEventData是包数据，Stru_UniPkgHead + 数据 + 包尾, userint=hSocket
#ifndef WndCmd_YourPkgArrival2
#define WndCmd_YourPkgArrival2		0x3bbd
#endif

//通知订阅的线程，通知与问询模块发送通知过来，lParam=EventParamID，EventParam的pEventData是Stru_NotifyEvent数据
#ifndef WndCmd_NotifyEventArrival
#define WndCmd_NotifyEventArrival		0x3bbe
#endif

#define MAX_SQL_LENGTH			4000	//SQL语句最大长度
#define MAX_USABLE_SQL_LENGTH	3999

//-----------------------------------------------------------------------------------
//	下面定义命令字
//-----------------------------------------------------------------------------------
#define CMDID_SvrTcpDisconn				MAKE_CMDID(CMDID_BASE_HIWORD_TCP,0x2100)							//TcpServer: 有tcp连接断开 pkghead的userdata1为断开的hSocket，userdata2为客户端IP，userdata3为客户端Port
#define CMDID_SvrTcpAccept			    MAKE_CMDID(CMDID_BASE_HIWORD_TCP,0x2111)							//TcpServer: 有tcp连接 pkghead的userdata1为连接的hSocket，userdata2为客户端IP，userdata3为客户端Port
#define CMDID_TcpClientConnect		    MAKE_CMDID(CMDID_BASE_HIWORD_TCP,0x2112)							//TcpClient: TcpClient连接成功 pkghead的userdata1为hSocket，userdata2为服务器IP，userdata3为服务器Port
#define CMDID_TcpClientDisconn		    MAKE_CMDID(CMDID_BASE_HIWORD_TCP,0x2113)							//TcpClient: TcpClient连接中断 pkghead的userdata1为hSocket，userdata2为服务器IP，userdata3为服务器Port


//登陆服务
#define CMDID_Login_Req					MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x2000)				//登陆请求
#define CMDID_Login_Rsp					MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x2001)				//登陆请求返回
#define CMDID_Trader_Login_Req			MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x3002)				//登陆请求
#define CMDID_Trader_Login_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x3003)				//登陆请求返回
#define CMDID_Logoff_Req				MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x3000)				//注销请求
#define CMDID_Logoff_Rsp				MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x3004)				//注销请求返回
#define CMDID_ForceCloseClient_Push		MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x4000)				//用户被停用，强制关闭该客户端

#define CMDID_GetALLError_Push		    MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x300b)				//获取系统的错误推送

//Slaver Req
#define CMDID_QueryMasterAllUserLoginStatus_Req    MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x4001)				//获取master所有用户的状态
#define CMDID_QueryMasterAllUserLoginStatus_Rsp    MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x4002)				//获取master所有用户的状态
#define CMDID_AddUserLoginStatusToMaster_Req       MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x4003)				//从服务新增用户通知主服务
#define CMDID_AddUserLoginStatusToMaster_Rsp       MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x4004)				//从服务新增用户通知主服务
#define CMDID_RemoveUserLoginStatusToMaster_Req    MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x4005)				//从服务删除用户通知主服务
#define CMDID_RemoveUserLoginStatusToMaster_Rsp    MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x4006)				//从服务新增用户通知主服务
#define CMDID_SubscriberAllUserLoginStatus_Req     MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x4007)				//订阅获取所有用户状态
#define CMDID_SubscriberAllUserLoginStatus_Rsp     MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x4008)				//订阅获取所有用户状态
#define CMDID_UnsubscriberAllUserLoginStatus_Req   MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x4009)				//退订获取所有用户状态
#define CMDID_UnsubscriberAllUserLoginStatus_Rsp   MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x400a)				//退订获取所有用户状态
#define CMDID_AddUserLoginStatusToSlave_Req        MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x400b)				//主服务增加用户通知从服务
#define CMDID_AddUserLoginStatusToSlave_Rsp        MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x400c)				//主服务增加用户通知从服务
#define CMDID_RemoveUserLoginStatusToSlave_Req     MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x400d)				//主服务删除用户通知从服务
#define CMDID_RemoveUserLoginStatusToSlave_Rsp     MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x400e)				//主服务删除用户通知从服务
#define CMDID_AllUserLoginStatus_Push              MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x400f)				//退订获取所有用户状态
#define CMDID_NotifySendToSlave_Req                MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x4010)				//主服务删除用户通知从服务
#define CMDID_NotifySendToSlave_Rsp                MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x4011)				//退订获取所有用户状态
#define CMDID_NotifyStartSettlementToSlave_Push    MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x4012)				//主服务开始结算通知从服务


//

//品种和合约服务
#define CMDID_QueryAllInstruments_Req   MAKE_CMDID(CMDID_BASE_HIWORD_DATA_INSTRUMENTS,0x0001)            //请求获取所有合约
#define CMDID_QueryAllInstruments_Rsp   MAKE_CMDID(CMDID_BASE_HIWORD_DATA_INSTRUMENTS,0x0002)            //响应获取所有合约
#define CMDID_PushAllInstruments_Req    MAKE_CMDID(CMDID_BASE_HIWORD_DATA_INSTRUMENTS,0x0003)            //客户端推送所有的合约的请求
#define CMDID_PushAllInstruments_Rsp    MAKE_CMDID(CMDID_BASE_HIWORD_DATA_INSTRUMENTS,0x0004)            //客户端推送所有的合约的响应

//品种和合约服务
#define CMDID_PushCommissionRate_Req                    MAKE_CMDID(CMDID_BASE_HIWORD_DATA_RATIO,0x0001)					//客户端推送一个手续费率请求
#define CMDID_PushCommissionRate_Rsp                    MAKE_CMDID(CMDID_BASE_HIWORD_DATA_RATIO,0x0002)					//客户端推送一个手续费率响应
#define CMDID_PushMarginRate_Req	                    MAKE_CMDID(CMDID_BASE_HIWORD_DATA_INSTRUMENTS,0x0003)            //客户端推送一个报证金费率请求
#define CMDID_PushMarginRate_Rsp                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_INSTRUMENTS,0x0004)            //客户端推送一个报证金费率响应

#define BASE_YEAR 2013

//经纪公司模块所有数据打包请求命令字
#define Cmd_QueryBrokerModule_Req			            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x1001)	//经纪公司模块所有数据请求
#define Cmd_QueryBrokerModule_Rsp			            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x1002)	//经纪公司模块所有数据返回


//委托交易账号
#define Cmd_AddTradeAccount_Req			                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0001)	//添加委托交易账号请求
#define Cmd_AddTradeAccount_Rsp			                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0002)	//添加委托交易账号返回
#define Cmd_ModifyTradeAccount_Req		                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0003)	//修改委托交易账号请求
#define Cmd_ModifyTradeAccount_Rsp		                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0004)	//修改委托交易账号返回
#define Cmd_DelTradeAccount_Req			                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0005)	//删除委托交易账号请求
#define Cmd_DelTradeAccount_Rsp			                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0006)	//删除委托交易账号返回
#define Cmd_QueryTradeAccount_Req		                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0007)	//查询委托交易账号请求
#define Cmd_QueryTradeAccount_Rsp		                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0008)	//查询委托交易账号返回

//经纪公司
#define Cmd_AddBrokerInfo_Req			                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0101)	//添加经纪公司请求
#define Cmd_AddBrokerInfo_Rsp			                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0102)	//添加经纪公司返回
#define Cmd_ModifyBrokerInfo_Req			            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0103)	//修改经纪公司请求
#define Cmd_ModifyBrokerInfo_Rsp			            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0104)	//修改经纪公司返回
#define Cmd_DelBrokerInfo_Req			                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0105)	//删除经纪公司请求
#define Cmd_DelBrokerInfo_Rsp			                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0106)	//删除经纪公司返回
#define Cmd_QueryBrokerInfo_Req			                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0107)	//查询经纪公司请求
#define Cmd_QueryBrokerInfo_Rsp			                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0108)	//查询经纪公司返回

//服务器组
#define Cmd_AddServerGroup_Req			                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0201)	//添加服务器组请求
#define Cmd_AddServerGroup_Rsp			                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0202)	//添加服务器组返回
#define Cmd_ModifyServerGroup_Req		                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0203)	//修改服务器组请求
#define Cmd_ModifyServerGroup_Rsp		                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0204)	//修改服务器组返回
#define Cmd_DelServerGroup_Req			                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0205)	//删除服务器组请求
#define Cmd_DelServerGroup_Rsp			                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0206)	//删除服务器组返回
#define Cmd_QueryServerGroup_Req			            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0207)	//查询服务器组请求
#define Cmd_QueryServerGroup_Rsp			            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0208)	//查询服务器组返回

//服务器地址
#define Cmd_AddServerAddress_Req			            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0301)	//添加服务器地址请求
#define Cmd_AddServerAddress_Rsp			            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0302)	//添加服务器地址返回
#define Cmd_ModifyServerAddress_Req		                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0303)	//修改服务器地址请求
#define Cmd_ModifyServerAddress_Rsp		                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0304)	//修改服务器地址返回
#define Cmd_DelServerAddress_Req			            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0305)	//删除服务器地址请求
#define Cmd_DelServerAddress_Rsp			            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0306)	//删除服务器地址返回
#define Cmd_QueryServerAddress_Req		                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0307)	//查询服务器地址请求
#define Cmd_QueryServerAddress_Rsp		                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0308)	//查询服务器地址返回

//行情服务器地址
#define Cmd_ModifyBrokerInfoQuot_Req		            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0401)	//修改行情经纪公司请求
#define Cmd_ModifyBrokerInfoQuot_Rsp		            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0402)	//修改行情经纪公司返回
#define Cmd_AddServerAddressQuot_Req		            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0403)	//添加行情服务器地址请求
#define Cmd_AddServerAddressQuot_Rsp		            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0404)	//添加行情服务器地址返回
#define Cmd_ModifyServerAddressQuot_Req	                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0405)	//修改行情服务器地址请求
#define Cmd_ModifyServerAddressQuot_Rsp	                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0406)	//修改行情服务器地址返回
#define Cmd_DelServerAddressQuot_Req		            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0407)	//删除行情服务器地址请求
#define Cmd_DelServerAddressQuot_Rsp		            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0408)	//删除行情服务器地址返回
#define Cmd_QueryServerAddressQuot_Req	                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0409)	//查询行情服务器地址请求
#define Cmd_QueryServerAddressQuot_Rsp	                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x040A)	//查询行情服务器地址返回

//组织机构模块所有数据打包请求命令字
#define Cmd_QueryOrganizationModule_Req			        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x1001)	//组织机构模块所有数据请求
#define Cmd_QueryOrganizationModule_Rsp			        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x1002)	//组织机构模块所有数据返回

//组织架构
#define Cmd_AddOrg_Req			                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0001)	//添加组织架构请求
#define Cmd_AddOrg_Rsp			                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0002)	//添加组织架构返回
#define Cmd_ModifyOrg_Req		                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0003)	//修改组织架构请求
#define Cmd_ModifyOrg_Rsp		                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0004)	//修改组织架构返回
#define Cmd_DelOrg_Req			                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0005)	//删除组织架构请求
#define Cmd_DelOrg_Rsp			                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0006)	//删除组织架构返回
#define Cmd_QueryOrg_Req			                    MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0007)	//查询组织架构请求
#define Cmd_QueryOrg_Rsp			                    MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0008)	//查询组织架构返回

//用户
#define Cmd_AddUser_Req			                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0101)	//添加用户账号请求
#define Cmd_AddUser_Rsp			                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0102)	//添加用户账号返回
#define Cmd_DelUser_Req			                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0103)	//删除用户账号请求
#define Cmd_DelUser_Rsp			                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0104)	//删除用户账号返回
#define Cmd_ModifyUser_Req		                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0105)	//修改用户账户基本信息请求
#define Cmd_ModifyUser_Rsp		                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0106)	//修改用户账户基本信息返回
#define Cmd_QryUser_Req			                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0107)	//查询所有用户账号请求
#define Cmd_QryUser_Rsp			                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0108)	//查询所有用户账号返回
#define Cmd_ModifyUserPassword_Req	                    MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0109)	//修改用户(交易员)密码请求
#define Cmd_ModifyUserPassword_Rsp	                    MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x010A)	//修改用户(交易员)密码返回
#define Cmd_UserStatus_Req		                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x010B)	//是否启用请求
#define Cmd_UserStatus_Rsp		                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x010C)   //是否启用返回

//用户与角色关系
#define Cmd_SaveUserRoleRelation_Req		            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0201)	//保存用户与角色关系请求
#define Cmd_SaveUserRoleRelation_Rsp		            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0202)	//保存用户与角色关系返回
#define Cmd_QryUserRoleRelation_Req		                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0203)	//查询用户与角色关系请求
#define Cmd_QryUserRoleRelation_Rsp		                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0204)	//查询用户与角色关系返回

//用户与组织机构关系
#define Cmd_SaveUserOrgRelation_FromUser_Req		    MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0301)	//保存来自用户模块的用户与组织机构关系请求 
#define Cmd_SaveUserOrgRelation_FromUser_Rsp		    MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0302)	//保存来自用户模块的用户与组织机构关系返回
#define Cmd_SaveUserOrgRelation_FromOrg_Req		        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0303)	//保存来自机构模块的用户与组织机构关系请求
#define Cmd_SaveUserOrgRelation_FromOrg_Rsp		        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0304)	//保存来自机构模块的用户与组织机构关系返回
#define Cmd_QryUserOrgRelation_Req		                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0305)	//查询用户与组织机构关系请求
#define Cmd_QryUserOrgRelation_Rsp		                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0306)	//查询用户与组织机构关系返回


//用户与委托交易账号关系
#define Cmd_SaveUserTradeAcctRelation_Req	            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0401)	//保存用户与委托交易账号关系请求
#define Cmd_SaveUserTradeAcctRelation_Rsp	            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0402)	//保存用户与委托交易账号关系返回
#define Cmd_QryUserTradeAcctRelation_Req		        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0403)	//查询用户与委托交易账号关系请求
#define Cmd_QryUserTradeAcctRelation_Rsp		        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0404)	//查询用户与委托交易账号关系返回

//组织机构和委托交易账号关系
#define Cmd_QryTradeAccountAndOrgRelation_Req		    MAKE_CMDID(CMDID_BASE_HIWORD_DATA_ACCOUNT_AND_ORG,0x0001)	//查询委托交易账号和组织机构关系请求
#define Cmd_QryTradeAccountAndOrgRelation_Rsp		    MAKE_CMDID(CMDID_BASE_HIWORD_DATA_ACCOUNT_AND_ORG,0x0002)	//查询委托交易账号和组织机构关系返回
#define Cmd_SaveTradeAccountAndOrgRelation_Req          MAKE_CMDID(CMDID_BASE_HIWORD_DATA_ACCOUNT_AND_ORG,0x0003)	//保存委托交易账号和组织机构关系请求
#define Cmd_SaveTradeAccountAndOrgRelation_Rsp          MAKE_CMDID(CMDID_BASE_HIWORD_DATA_ACCOUNT_AND_ORG,0x0004)	//保存委托交易账号和组织机构关系返回

//角色
#define Cmd_AddRole_Req		                            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0501)	//添加角色请求
#define Cmd_AddRole_Rsp		                            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0502)	//添加角色返回
#define Cmd_DelRole_Req		                            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0503)	//删除角色请求
#define Cmd_DelRole_Rsp		                            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0504)	//删除角色返回
#define Cmd_ModifyRole_Req	                            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0505)	//修改角色请求
#define Cmd_ModifyRole_Rsp	                            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0506)	//修改角色返回
#define Cmd_QryRole_Req		                            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0507)	//查询角色请求
#define Cmd_QryRole_Rsp		                            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0508)	//查询角色返回

//权限
#define Cmd_QryPrivilege_Req		                    MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0601)	//查询所有权限内容请求
#define Cmd_QryPrivilege_Rsp		                    MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0602)	//查询所有权限内容返回

//角色和权限的关系
#define Cmd_QueryRolePrivilegeRelation_Req		        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0701)	//查询角色和权限关系请求
#define Cmd_QueryRolePrivilegeRelation_Rsp		        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0702)	//查询角色和权限关系返回
#define Cmd_SaveRolePrivilegeRelation_Req		        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0703)	//保存角色和权限关系请求
#define Cmd_SaveRolePrivilegeRelation_Rsp		        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0704)	//保存角色和权限关系返回

//理财产品模块所有数据打包请求命令字
#define Cmd_QueryProductModule_Req			            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_FINANCIAL_PRODUCT,0x1001)	//理财产品模块所有数据请求
#define Cmd_QueryProductModule_Rsp			            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_FINANCIAL_PRODUCT,0x1002)	//理财产品模块所有数据返回

//理财产品
#define Cmd_AddFinicialProduct_Req                      MAKE_CMDID(CMDID_BASE_HIWORD_DATA_FINANCIAL_PRODUCT, 0x0001)//增加理财产品请求
#define Cmd_AddFinicialProduct_Rsp                      MAKE_CMDID(CMDID_BASE_HIWORD_DATA_FINANCIAL_PRODUCT, 0x0002)//增加理财产品返回
#define Cmd_DeleteFinicialProduct_Req                   MAKE_CMDID(CMDID_BASE_HIWORD_DATA_FINANCIAL_PRODUCT, 0x0003)//删除理财产品请求
#define Cmd_DeleteFinicialProduct_Rsp                   MAKE_CMDID(CMDID_BASE_HIWORD_DATA_FINANCIAL_PRODUCT, 0x0003)//删除理财产品返回
#define Cmd_ModifyFinicialProduct_Req                   MAKE_CMDID(CMDID_BASE_HIWORD_DATA_FINANCIAL_PRODUCT, 0x0005)//修改理财产品请求
#define Cmd_ModifyFinicialProduct_Rsp                   MAKE_CMDID(CMDID_BASE_HIWORD_DATA_FINANCIAL_PRODUCT, 0x0006)//修改理财产品返回
#define Cmd_QueryFinicialProduct_Req                    MAKE_CMDID(CMDID_BASE_HIWORD_DATA_FINANCIAL_PRODUCT, 0x0007)//查询所有理财产品请求
#define Cmd_QueryFinicialProduct_Rsp                    MAKE_CMDID(CMDID_BASE_HIWORD_DATA_FINANCIAL_PRODUCT, 0x0008)//查询所有理财产品返回

//理财产品与交易员关系
#define Cmd_QueryProductTraderRelation_Req		        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_FINANCIAL_PRODUCT, 0x0101)	//查询理财产品与交易员关系请求
#define Cmd_QueryProductTraderRelation_Rsp		        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_FINANCIAL_PRODUCT, 0x0102)	//查询理财产品与交易员关系返回
#define Cmd_SaveProductTraderRelation_FromUser_Req      MAKE_CMDID(CMDID_BASE_HIWORD_DATA_FINANCIAL_PRODUCT, 0x0103)//保存来自用户模块的理财产品与交易员关系请求
#define Cmd_SaveProductTraderRelation_FromUser_Rsp      MAKE_CMDID(CMDID_BASE_HIWORD_DATA_FINANCIAL_PRODUCT, 0x0104)//保存来自用户模块的理财产品与交易员关系返回
#define Cmd_SaveProductTraderRelation_FromProduct_Req   MAKE_CMDID(CMDID_BASE_HIWORD_DATA_FINANCIAL_PRODUCT, 0x0105)//保存来自理财产品模块的理财产品与交易员关系请求
#define Cmd_SaveProductTraderRelation_FromProduct_Rsp   MAKE_CMDID(CMDID_BASE_HIWORD_DATA_FINANCIAL_PRODUCT, 0x0106)//保存来自理财产品模块的理财产品与交易员关系返回


//交易策略
#define Cmd_AddTradeStrategy_Req                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_TRADER_STRATEGY, 0x0001)          //增加交易策略请求
#define Cmd_AddTradeStrategy_Rsp                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_TRADER_STRATEGY, 0x0002)          //增加交易策略返回
#define Cmd_DeleteTradeStrategy_Req                     MAKE_CMDID(CMDID_BASE_HIWORD_DATA_TRADER_STRATEGY, 0x0003)          //删除交易策略请求
#define Cmd_DeleteTradeStrategy_Rsp                     MAKE_CMDID(CMDID_BASE_HIWORD_DATA_TRADER_STRATEGY, 0x0004)          //删除交易策略返回   
#define Cmd_ModifyTradeStrategy_Req                     MAKE_CMDID(CMDID_BASE_HIWORD_DATA_TRADER_STRATEGY, 0x0005)          //修改交易策略请求
#define Cmd_ModifyTradeStrategy_Rsp                     MAKE_CMDID(CMDID_BASE_HIWORD_DATA_TRADER_STRATEGY, 0x0006)          //修改交易策略返回
#define Cmd_QueryTradeStrategy_Req                      MAKE_CMDID(CMDID_BASE_HIWORD_DATA_TRADER_STRATEGY, 0x0007)          //查询所有交易策略请求
#define Cmd_QueryTradeStrategy_Rsp                      MAKE_CMDID(CMDID_BASE_HIWORD_DATA_TRADER_STRATEGY, 0x0008)          //查询所有交易策略返回

//交易策略与组织机构关系
#define Cmd_SaveStrategyOrgRelation_Req	                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_TRADER_STRATEGY, 0x0101)	 //增加交易策略与组织机构关系请求
#define Cmd_SaveStrategyOrgRelation_Rsp	                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_TRADER_STRATEGY, 0x0102)	 //增加交易策略与组织机构关系返回
#define Cmd_QueryStrategyOrgRelation_Req	            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_TRADER_STRATEGY, 0x0103)	 //查询交易策略与组织机构关系请求
#define Cmd_QueryStrategyOrgRelation_Rsp	            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_TRADER_STRATEGY, 0x0104)	 //查询交易策略与组织机构关系返回

//报盘模块
#define Cmd_Offer_Trade_Push						 MAKE_CMDID(CMDID_BASE_HIWORD_BUSSINESS_OFFER,0x0001)	//成交推送			
#define Cmd_Offer_Order_Push						 MAKE_CMDID(CMDID_BASE_HIWORD_BUSSINESS_OFFER,0x0002)	//报单推送			
#define Cmd_Offer_InstrumentCommissionRate_Push      MAKE_CMDID(CMDID_BASE_HIWORD_BUSSINESS_OFFER,0x0003)	//合约手续费率		
#define Cmd_Offer_InstrumentMarginRate_Push          MAKE_CMDID(CMDID_BASE_HIWORD_BUSSINESS_OFFER,0x0004)	//合约保证金率


//结算模块			
#define Cmd_Settlement_SettlementFinish_Notify             MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0001)  //服务器通知客户端结算完成信息		
#define Cmd_Settlement_SettlementFinish_Confirm            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0002)  //客户端结算完成确认信息返回
#define Cmd_Settlement_QueryFund_Req                       MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0003)  //客户端查询资金信息		
#define Cmd_Settlement_QueryFund_Rsp                       MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0004)  //服务器返回资金信息
#define Cmd_Settlement_QuerySettlementPrice_Req            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0005)  //客户端查询结算价信息		
#define Cmd_Settlement_QuerySettlementPrice_Rsp            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0006)  //服务器返回结算价信息
#define Cmd_Settlement_ModifySettlementPrice_Req           MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0007)  //客户端修改结算价信息		
#define Cmd_Settlement_ModifySettlementPrice_Rsp           MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0008)  //服务器修改结算价信息
#define Cmd_Settlement_QueryTrade_Req          	           MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0009)	//客户端查询交易员子账号成交结算信息
#define Cmd_Settlement_QueryTrade_Rsp                      MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x000A)	//服务器返回交易员子账号成交信息
#define Cmd_Settlement_AddTrade_Req          	           MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x000B)	//客户端增加交易员子账号成交信息
#define Cmd_Settlement_AddTrade_Rsp                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x000C)	//服务器返回增加交易员子账号成交信息
#define Cmd_Settlement_DeleteTrade_Req          	       MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x000D)	//客户端删除交易员子账号成交信息
#define Cmd_Settlement_DeleteTrade_Rsp                     MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x000E)	//服务器返回删除交易员子账号成交信息
#define Cmd_Settlement_ModifyTrade_Req          	       MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x000F)	//客户端修改交易员子账号成交信息
#define Cmd_Settlement_ModifyTrade_Rsp                     MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0010)	//服务器返回修改交易员子账号成交信息
#define Cmd_Settlement_QueryPositionDetail_Req             MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0011)  //客户端查询交易员子账号持仓明细信息		
#define Cmd_Settlement_QueryPositionDetail_Rsp             MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0012)  //服务器返回交易员子账号持仓明细信息
#define Cmd_Settlement_AddPositionDetail_Req               MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0013)  //客户端增加交易员子账号持仓明细信息		
#define Cmd_Settlement_AddPositionDetail_Rsp               MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0014)  //服务器返回增加交易员子账号持仓明细信息
#define Cmd_Settlement_DeletePositionDetail_Req            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0015)  //客户端删除交易员子账号持仓明细信息		
#define Cmd_Settlement_DeletePositionDetail_Rsp            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0016)  //服务器返回删除交易员子账号持仓明细信息
#define Cmd_Settlement_ModifyPositionDetail_Req	           MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0017)	//客户端修改交易员子账号持仓明细信息			
#define Cmd_Settlement_ModifyPositionDetail_Rsp	           MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0018)	//服务器返回修改交易员子账号持仓明细信息
#define Cmd_Settlement_QueryPosition_Req                   MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0019)	//客户端请求交易委托账号持仓汇总结算信息
#define Cmd_Settlement_QueryPosition_Rsp                   MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x001A)	//服务器返回交易委托账号持仓汇总信息
#define Cmd_Settlement_AddPosition_Req                     MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x001B)	//客户端请求增加交易委托账号持仓汇总信息
#define Cmd_Settlement_AddPosition_Rsp                     MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x001C)	//服务器返回增加交易委托账号持仓汇总信息
#define Cmd_Settlement_DeletePosition_Req                  MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x001D)	//客户端请求删除交易委托账号持仓汇总信息
#define Cmd_Settlement_DeletePosition_Rsp                  MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x001E)	//服务器返回删除交易委托账号持仓汇总信息
#define Cmd_Settlement_ModifyPosition_Req                  MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x001F)	//客户端请求修改交易委托账号持仓汇总信息
#define Cmd_Settlement_ModifyPosition_Rsp                  MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0020)	//服务器返回修改交易委托账号持仓汇总信息
#define Cmd_Settlement_ModifyFund_Req	                   MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0021)	//客户端修改资金结算信息			
#define Cmd_Settlement_ModifyFund_Rsp	                   MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0022)	//服务器确认修改资金结算信息返回
#define Cmd_Settlement_SetSettltmentTime_Req	           MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0023)	//客户端设置结算时间请求			
#define Cmd_Settlement_SetSettltmentTime_Rsp	           MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0024)	//服务器设置结算时间返回
#define Cmd_Settlement_ManualQueryData_Req	               MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0025)	//客户端手动查询成交结算数据请求（报单和成交）			
#define Cmd_Settlement_ManualQueryData_Rsp	               MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0026)	//服务器手动查询成交结算数据返回
#define Cmd_Settlement_ConfirmSettlementPrice_Req          MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0027)	//结算价确认
#define Cmd_Settlement_ConfirmSettlementPrice_Rsp          MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0028)	//结算价确认返回
#define Cmd_Settlement_ConfirmSettlementFund_Req           MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0029)	//结算结果确认
#define Cmd_Settlement_ConfirmSettlementFund_Rsp           MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x002A)	//结算结果确认返回
#define Cmd_Settlement_QueryCommissionMarginRate_Req       MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x002B)  //客户端查询手续费信息	
#define Cmd_Settlement_QueryCommissionMarginRate_Rsp       MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x002C)  //服务器查询手续费信息返回
#define Cmd_Settlement_ModifyCommissionMarginRate_Req      MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x002D)  //客户端修改手续费信息
#define Cmd_Settlement_ModifyCommissionMarginRate_Rsp      MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x002E)  //客服务器修改手续费信息返回
#define Cmd_Settlement_ConfirmCommissionMarginRate_Req     MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x002F)  //客户端费率信息确认
#define Cmd_Settlement_ConfirmCommissionMarginRate_Rsp     MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0030)  //服务器费率信息确认返回
#define Cmd_Settlement_LoadData_Req                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0031)  //加载结算数据请求
#define Cmd_Settlement_LoadData_Rsp                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0032)  //加载结算数据返回
#define Cmd_Settlement_ConfirmTrade_Req                    MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0033)	//成交确认
#define Cmd_Settlement_ConfirmTrade_Rsp                    MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0034)	//成交确认返回
#define Cmd_Settlement_StartSettlement_Req                 MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0035)	//开始结算请求
#define Cmd_Settlement_StartSettlement_Rsp                 MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0036)	//开始结算返回
#define Cmd_Settlement_QueryTradingday_Req                 MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0037)	//客户端请求查询交易日
#define Cmd_Settlement_QueryTradingday_Rsp                 MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0038)	//服务器查询交易日返回
#define Cmd_Settlement_TradeCaculate_Req                   MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0039)	//客户端请求成交试计算
#define Cmd_Settlement_TradeCaculate_Rsp                   MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x003A)	//服务器成交试计算返回
#define Cmd_Settlement_SettleBill_Req                      MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x003B)	//客户端请求结算单
#define Cmd_Settlement_SettleBill_Rsp                      MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x003C)	//服务器结算单返回
#define Cmd_Settlement_QuerySettltmentTime_Req             MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x003D)	//客户端请求自动结算时间
#define Cmd_Settlement_QuerySettltmentTime_Rsp             MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x003E)	//服务器自动结算时间返回
#define Cmd_Settlement_SettlementWinColse_Notify           MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x003F)  //客户端关闭结算窗口
#define Cmd_Settlement_QueryCTPLoadStatus_Req              MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0040)	//每个交易员是否加载成功请求
#define Cmd_Settlement_QueryCTPLoadStatus_Rsp              MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0041)	//每个交易员是否加载成功返回
#define Cmd_Settlement_LoadHistoryCommission_Req           MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0042)	//加载历史费率请求
#define Cmd_Settlement_LoadHistoryCommission_Rsp           MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0043)	//加载历史费率返回
#define Cmd_Settlement_ImmediatelyAutoSettlement_Req       MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0044)	//立即自动结算请求
#define Cmd_Settlement_ImmediatelyAutoSettlement_Rsp       MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0045)	//立即自动结算返回

//交易执行
#define Cmd_TradeExcute_InputOrder_Req						MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_EXCUTE,0x0001)	//下单请求
#define Cmd_TradeExcute_InputOrder_Rsp						MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_EXCUTE,0x0002)	//下单请求
#define Cmd_TradeExcute_Trader_Rsp							MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_EXCUTE,0x0003)	//CTP传回报单响应
#define Cmd_TradeExcute_OrderStatus_Rsp						MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_EXCUTE,0x0004)	//发送报单状态给客户端
#define Cmd_TradeExcute_VerifyOrder_Req						MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_EXCUTE,0x0005)	//请求审核客户端审核下单请求
#define Cmd_TradeExcute_VerifyOrder_Rsp						MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_EXCUTE,0x0006)	//下单请求审核结果返回
#define Cmd_TradeExcute_VerifyOrder_RspRsp					MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_EXCUTE,0x0007)	//审核结果响应的返回
#define Cmd_TradeExcute_CancelOrder_Req						MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_EXCUTE,0x0101)	//撤单请求
#define Cmd_TradeExcute_CancelOrder_Fail_RSP				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_EXCUTE,0x0103)	//撤单失败
#define Cmd_TradeExcute_CancelOrder_Error_RSP				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_EXCUTE,0x0104)	//撤单错误
#define Cmd_TradeExcute_QryApprovingOrder_Req				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_EXCUTE,0x0105)	//查询审核中的报单请求
#define Cmd_TradeExcute_QryApprovingOrder_Rsp				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_EXCUTE,0x0106)	//查询审核中的报单返回
#define Cmd_TradeExcute_StategyInputOrder_Req				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_EXCUTE,0x0107)	//策略进程下单请求
#define Cmd_TradeExcute_StategyInputOrder_Rsp				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_EXCUTE,0x0108)	//策略进程下单请求
#define Cmd_TradeExcute_StategyActionOrder_Req				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_EXCUTE,0x0109)	//策略进程撤单请求
#define Cmd_TradeExcute_StategyActionOrder_Rsp				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_EXCUTE,0x010a)	//策略进程撤单请求
// 用户消息管理
#define Cmd_UserMsg_SubsMsg_Req								MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x0001)	//用户订阅消息请求
#define Cmd_UserMsg_SubsMsg_Rsp								MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x0002)	//用户订阅消息服务器应答
#define Cmd_UserMsg_UnSubsMsg_Req							MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x0003)	//用户取消订阅消息请求
#define Cmd_UserMsg_UnSubsMsg_Rsp							MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x0004)	//用户取消订阅消息服务器应答
#define Cmd_UserMsg_PushMsg									MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x0005)	//用户消息推送
#define Cmd_UserMsg_Send_Req								MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x0006)	//用户发送消息请求
#define Cmd_UserMsg_Send_Rsp								MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x0007)	//用户发送消息服务器应答
#define Cmd_UserMsg_Query_Send_Req							MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x0008)	//用户查询发送消息请求
#define Cmd_UserMsg_Query_Send_Rsp							MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x0009)	//用户查询发送消息服务器应答
#define Cmd_UserMsg_Query_Recv_Req							MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x000a)	//用户查询接收消息请求
#define Cmd_UserMsg_Query_Recv_Rsp							MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x000b)	//用户查询接收消息服务器应答
#define Cmd_UserMsg_QueryTargetMsgFromSlave_Req				MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x000c)	//slave向master查询所有的未发送的目标消息
#define Cmd_UserMsg_QueryTargetMsgFromSlave_Rsp				MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x000d)	//slave向master查询所有的未发送的目标消息的应答
#define Cmd_UserMsg_SynchrPushMsg							MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x000e)	//同步slave时的目标消息推送
#define Cmd_UserMsg_TurnSendReqToMaster_Req					MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x000f)	//用户发送消息请求转发到master
#define Cmd_UserMsg_TurnSendReqToMaster_Rsp					MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x0010)	//用户发送消息请求转发到master的应答
//#define Cmd_UserMsg_SynchroTargetMsgToSlave_Req				MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x0011)	//master把目标消息同步到slave
//#define Cmd_UserMsg_SynchroTargetMsgToSlave_Rsp				MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x0012)	//master把目标消息同步到slave的应答
#define Cmd_UserMsg_SynchroSentStatusToSlave_Req			MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x0013)	//master把目标消息发送状态同步到slave
#define Cmd_UserMsg_SynchroSentStatusToSlave_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x0014)	//master把目标消息发送状态同步到slave的应答
#define Cmd_UserMsg_SynchroSentStatusToMaster_Req			MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x0015)	//slave把目标消息发送状态同步到master
#define Cmd_UserMsg_SynchroSentStatusToMaster_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x0016)	//slave把目标消息发送状态同步到master的应答


//交易数据管理模块
#define Cmd_TradeDataManager_QryInstrument_Req				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0001)	//查询合约
#define Cmd_TradeDataManager_QryInstrument_Rsp				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0002)	//查询合约响应
#define Cmd_TradeDataManager_QryTraderAccount_Req		    MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0003)	//查询资金账户
#define Cmd_TradeDataManager_QryTraderAccount_Rsp		    MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0004)	//查询资金账户响应
#define Cmd_TradeDataManager_QryTrade_Req					MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0005)	//查询成交
#define Cmd_TradeDataManager_QryTrade_Rsp					MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0006)	//查询成交响应
#define Cmd_TradeDataManager_QryOrder_Req				    MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0007)	//查询报单
#define Cmd_TradeDataManager_QryOrder_Rsp					MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0008)	//查询报单响应
#define Cmd_TradeDataManager_QryPosition_Req				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0009)	//查询持仓
#define Cmd_TradeDataManager_QryPosition_Rsp				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x000a)	//查询持仓响应
#define Cmd_TradeDataManager_QryPositionDetail_Req			MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x000b)	//查询持仓明细
#define Cmd_TradeDataManager_QryPositionDetail_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x000c)	//查询持仓明细响应
#define Cmd_TradeDataManager_QryComPositionDetail_Req		MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x000d)	//查询组合持仓明细
#define Cmd_TradeDataManager_QryComPositionDetail_Rsp		MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x000e)	//查询组合持仓明细响应
#define Cmd_TradeDataManager_QryCommissionRat_Req			MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x000f)	//查询手续费率
#define Cmd_TradeDataManager_QryCommissionRat_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0010)	//查询手续费率响应
#define Cmd_TradeDataManager_QryMarginRate_Req				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0011)	//查询保证金率
#define Cmd_TradeDataManager_QryMarginRate_Rsp				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0012)	//查询保证金响应
#define Cmd_TradeDataManager_FundInOut_Req				    MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0013)	//出入金请求
#define Cmd_TradeDataManager_FundInOut_Rsp				    MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0014)	//出入金响应
#define Cmd_TradeDataManager_QryFundInOut_Req				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0015)	//查询出入金请求
#define Cmd_TradeDataManager_QryFundInOut_Rsp				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0016)	//查询出入金响应
#define Cmd_TradeDataManager_QrySettlementConfirm_Req       MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0017)	//查询结算确认
#define Cmd_TradeDataManager_QrySettlementConfirm_Rsp	    MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0018)	//查询结算确认响应
#define Cmd_TradeDataManager_QrySettlementInfo_Req          MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0019)	//查询结算单信息
#define Cmd_TradeDataManager_QrySettlementInfo_Rsp	        MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x001a)	//查询结算单信息响应
#define Cmd_TradeDataManager_QryAccountStatus_Req           MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x001b)	//查询委托交易账号状态
#define Cmd_TradeDataManager_QryAccountStatus_Rsp	        MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x001c)	//查询委托交易账号状态响应
#define Cmd_TradeDataManager_LoginAccount_Req			    MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x001d)	//登陆到委托交易账号的请求
#define Cmd_TradeDataManager_LoginAccount_Rsp	            MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x001e)	//登陆到委托交易账号的响应
#define Cmd_TradeDataManager_SettlementConfirm_Req 		    MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0020)	//确认结算单请求
#define Cmd_TradeDataManager_SettlementConfirm_Rsp	        MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0021)	//确认结算单响应
#define Cmd_TradeDataManager_QryUserInitStatus_Req 		    MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0022)	//查询用户初始化状态
#define Cmd_TradeDataManager_QryUserInitStatus_Rsp	        MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0023)	//查询用户初始化状态响应
#define Cmd_TradeDataManager_InitUser_Req			        MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0024)	//初始化用户请求
#define Cmd_TradeDataManager_InitUser_Rsp	                MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0025)	//初始化用户请求
#define Cmd_TradeDataManager_ReInit_Req					    MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0026)	//重新初始化所有模块
#define Cmd_TradeDataManager_ReInit_Rsp					    MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0027)	//重新初始化所有模块
#define Cmd_TradeDataManager_QryReinitTime_Req              MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0028)	//客户端请求自动重新初始化时间
#define Cmd_TradeDataManager_QryReinitTime_Rsp              MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0029)	//服务器自动重新初始化时间
#define Cmd_TradeDataManager_SetReinitTime_Req              MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x002a)	//客户端请求设置自动重新初始化时间
#define Cmd_TradeDataManager_SetReinitTime_Rsp              MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x002b)	//服务器自动重新初始化时间
#define Cmd_TradeDataManager_QryStatus_Req                  MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x002c)	//客户端请求设置自动重新初始化时间
#define Cmd_TradeDataManager_QryStatus_Rsp                  MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x002d)	//服务器自动重新初始化时间
#define Cmd_TradeDataManager_QryTradingTime_Req             MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x002e)	//查询交易时间
#define Cmd_TradeDataManager_QryTradingTime_Rsp             MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x002f)	//查询交易时间
#define Cmd_TradeDataManager_SetTradingTime_Req             MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0030)	//设置交易时间
#define Cmd_TradeDataManager_SetTradingTime_Rsp             MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0031)	//设置交易时间
#define Cmd_TradeDataManager_QryNotice_Req                  MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0032)	//请求查询客户通知
#define Cmd_TradeDataManager_QryNotice_Rsp                  MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0033)	//请求查询客户通知响应
#define Cmd_TradeDataManager_QryContractBank_Req            MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0034)	//期货发起查询银行余额通知
#define Cmd_TradeDataManager_QryContractBank_Rsp            MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0035)	//期货发起查询银行余额通知
#define Cmd_TradeDataManager_QryParkedOrder_Req             MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0036)	//请求查询预埋单
#define Cmd_TradeDataManager_QryParkerOrder_Rsp             MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0037)	//请求查询预埋单
#define Cmd_TradeDataManager_QryParkedOrderAction_Req       MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0038)	//请求查询预埋撤单
#define Cmd_TradeDataManager_QryParkerOrderAction_Rsp       MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0039)	//请求查询预埋撤单
#define Cmd_TradeDataManager_QryDepthMarket_Req             MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0040)	//请求查询深度行情
#define Cmd_TradeDataManager_QryDepthMarket_Rsp             MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0041)	//请求查询深度行情
#define Cmd_TradeDataManager_ReForceInit_Req				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0042)	//强制重新初始化所有模块
#define Cmd_TradeDataManager_ReForceInit_Rsp				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0043)	//强制重新初始化所有模块

#define Cmd_TradeDataManager_SlaverQryTradingDay_Req	    MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0050)	//从查询主的交易日
#define Cmd_TradeDataManager_SlaverQryTradingDay_Rsp		MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0051)	//从查询主的交易日
#define Cmd_TradeDataManager_SlaverQryOrderAndTraderBiggerFTID_Req	MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0059)	//从查询主的报单
#define Cmd_TradeDataManager_SlaverQryOrderAndTraderBiggerFTID_Rsp	MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x005a)	//从查询主的报单
#define Cmd_TradeDataManager_MasterOrder_Push				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x005b)	//主报单的推送
#define Cmd_TradeDataManager_MasterTrader_Push				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x005e)	//主成交的推送
#define Cmd_TradeDataManager_MasterQuot_Req 			    MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0062)	//从行情请求
#define Cmd_TradeDataManager_MasterQuot_Push				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x005f)	//主行情的推送
#define Cmd_TradeDataManager_MasterCTPOrder_Push			MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0060)	//主报单的推送
#define Cmd_TradeDataManager_MasterCTPTrader_Push			MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0061)	//主成交的推送

#define Cmd_TradeDataManager_MasterQryCommission_Push		MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0064)	//主行情的推送
#define Cmd_TradeDataManager_MasterQryMargin_Push		    MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0065)	//主行情的推送
#define Cmd_TradeDataManager_MasterQryFund_Push			    MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0066)	//主行情的推送
#define Cmd_TradeDataManager_MasterQryPosition_Push		    MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0067)	//主行情的推送
#define Cmd_TradeDataManager_MasterQryPositionDetail_Push	MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0068)	//主行情的推送
#define Cmd_TradeDataManager_MasterUserInit_Push			MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0069)	//主行情的推送
#define Cmd_TradeDataManager_MasterQryCTPOrder_Push			MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0070)	//主行情的推送
#define Cmd_TradeDataManager_MasterQryCTPTrader_Push		MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0071)	//主行情的推送
#define Cmd_TradeDataManager_MasterErrOrderAction_Push      MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0072)	//主行情的推送
#define Cmd_TradeDataManager_MasterErrOrderInsert_Push      MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0073)	//主行情的推送


//行情
#define Cmd_SubscribeQuotEvent_Req						    MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0001)	//订阅行情事件请求
#define Cmd_SubscribeQuotEvent_Rsp						    MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0002)	//订阅行情事件返回
#define Cmd_UnSubscribeQuotEvent_Req						MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0003)	//退订行情事件请求
#define Cmd_UnSubscribeQuotEvent_Rsp						MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0004)	//退订行情事件返回
#define Cmd_QuotEvent_Push								    MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0005)	//行情事件推送
#define Cmd_ReInitQuotServerEvent_Req						MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0006)	//重启行情事件请求
#define Cmd_ReInitQuotServerEvent_Rsp						MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0007)	//重启行情事件返回
#define Cmd_QuotServerTimeResetEvent_Req					MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0008)	//重启行情定时重启设置事件请求
#define Cmd_QuotServerTimeResetEvent_Rsp					MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0009)	//重启行情定时重启设置事件返回
#define Cmd_QryQuotServerTimeEvent_Req					    MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x000A)	//查询重启行情定时重启设置事件请求
#define Cmd_QryQuotServerTimeEvent_Rsp					    MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x000B)	//查询重启行情定时重启设置事件返回
#define Cmd_QuotSvr_Login_Req                               MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x000C)	//登录行情服务器请求
#define Cmd_QuotSvr_Login_Rsp                               MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x000D)	//登录行情服务器返回
#define Cmd_SubscribeQuotEvent2_Req						    MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x000E)	//验证用户订阅行情事件请求
#define Cmd_SubscribeQuotEvent2_Rsp						    MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x000F)	//验证用户订阅行情事件返回
#define Cmd_QueryQuotKLineData_Req						    MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0010)	//查询行情K线事件请求
#define Cmd_QueryQuotKLineData_Rsp						    MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0011)	//查询行情K线事件返回
#define Cmd_QueryQuotTimeLineData_Req						MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0012)	//查询行情分时线事件请求
#define Cmd_QueryQuotTimeLineData_Rsp						MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0013)	//查询行情分时线事件返回
#define Cmd_UnSubscribeQuotEvent2_Req					    MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0014)	//验证用户退订行情事件请求
#define Cmd_UnSubscribeQuotEvent2_Rsp						MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0015)	//验证用户退订行情事件返回
#define Cmd_QueryVolatilityData_Req                         MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0016)	//查询波动率请求
#define Cmd_QueryVolatilityData_Rsp                         MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0017)	//查询波动率返回
#define Cmd_QuerySettlementPrice_Req                        MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0018)	//查询结算价请求
#define Cmd_QuerySettlementPrice_Rsp                        MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0019)	//查询结算价返回


//模拟行情服务器配置
#define Cmd_QrySimulateDay_Req					            MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0011)	//查询模拟交易日请求
#define Cmd_QrySimulateDay_Rsp					            MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0012)	//查询模拟交易日返回
#define Cmd_QrySimulatePlayMode_Req				            MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0013)	//查询模拟回放方式请求
#define Cmd_QrySimulatePlayMode_Rsp				            MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0014)	//查询模拟回放方式返回

#define Cmd_SimulatePlay_Req					            MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0015)	//开始模拟请求
#define Cmd_SimulatePlay_Rsp					            MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0016)	//开始模拟返回
#define Cmd_SimulatePause_Req					            MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0017)	//暂停模拟请求
#define Cmd_SimulatePause_Rsp					            MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0018)	//暂停模拟返回
#define Cmd_SimulateStop_Req					            MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0019)	//停止模拟请求
#define Cmd_SimulateStop_Rsp					            MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x001A)	//停止模拟返回
#define Cmd_SimulateApply_Req					            MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x001B)	//开始模拟配置应用请求
#define Cmd_SimulateApply_Rsp					            MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x001D)	//开始模拟配置应用返回

#define Cmd_QueryQuotDataEvent_Req						    MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0020)	//查询行情历史数据事件请求
#define Cmd_QueryQuotDataEvent_Rsp						    MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0021)	//查询行情历史数据事件返回

//以下风控相关
#define Cmd_RM_Config_Min									MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0099)
#define Cmd_RM_Config_Max									MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0999)
#define Cmd_RM_QryOrganization_Req							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0100)	//查询所有部门信息请求
#define Cmd_RM_QryOrganization_Rsp							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0102)	//返回所有部门信息

#define Cmd_RM_QryUser_Req								MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0103)	//查询User信息请求
#define Cmd_RM_QryUser_Rsp								MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0104)	//查询User信息返回

#define Cmd_RM_QryUserAndOrgRelation_Req					MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0105)	//查询交易员和部门的关系
#define Cmd_RM_QryUserAndOrgRelation_Rsp					MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0106)	//查询交易员和部门的关系信息返回

#define Cmd_RM_QryTraderAndTradeAccountRelation_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0107)	//交易员和委托交易账号的关系
#define Cmd_RM_QryTraderAndTradeAccountRelation_Rsp		    MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0108)	//交易员和委托交易账号的关系信息返回

#define Cmd_RM_QryProducts_Req								MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0109)	//查询所有品种
#define Cmd_RM_QryProducts_Rsp								MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x010a)	//查询所有品种返回
#define Cmd_RM_QryResponse_Req								MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x010b)	//查询动作请求
#define Cmd_RM_QryResponse_Rsp								MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x010c)	//查询动作返回

#define Cmd_RM_SetLimitTrade_Req							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x010d)	//设置限制交易请求
#define Cmd_RM_SetLimitTrade_Rsp							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x010e)	//设置限制交易返回


#define Cmd_RM_SetManualVerify_Req							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x010f)	//设置手动审核请求
#define Cmd_RM_SetManualVerify_Rsp							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0111)	//设置手动审核返回



//后台终端修改后经过风控服务器发送过来
#define Cmd_RM_SetLimitAndVerify_Rsp						MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0112)	//设置限制交易和手动审核返回

#define Cmd_RM_AddRiskPlan_Req								MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0135)	//增加风控方案
#define Cmd_RM_AddRiskPlan_Rsp								MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0136)    //增加风控方案返回
#define Cmd_RM_UseRiskPlanAdd_Req							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0137)	//是否启用风控方案
#define Cmd_RM_UseRiskPlanAdd_Rsp							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0138)    //是否启用风控方案返回
#define Cmd_RM_DeleteRiskPlan_Req							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0139)	//删除风控方案
#define Cmd_RM_DeleteRiskPlan_Rsp							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x013a)    //删除风控方案返回//风控客户端相关命令
#define Cmd_RM_QueryRiskPlan_Req							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x013b)	//查询风控方案
#define Cmd_RM_QueryRiskPlan_Rsp							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x013c)    //查询风控方案返回

#define Cmd_RM_CONTRACTSET_Req								MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x013d)	 //合同级风控设置请求
#define Cmd_RM_CONTRACTSET_Rsp								MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x013e)    //合同级风控设置请求返回

#define Cmd_RM_CONTRACTSET_QUERY_Req						MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x013f)	 //合同级风控设置请求查询
#define Cmd_RM_CONTRACTSET_QUERY_Rsp						MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0141)    //合同级风控设置请求查询返回


#define Cmd_RM_ModifyPassword_Req							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0125)	//修改密码请求
#define Cmd_RM_ModifyPassword_Rsp							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0126)	//修改密码响应
#define Cmd_RM_UnLockWindow_Req							    MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0127)	//解锁窗口请求
#define Cmd_RM_UnLockWindow_Rsp							    MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0128)	//解锁窗口响应


//风险指标相关接口
#define Cmd_RM_RiskIndicator_Min							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1001)
#define Cmd_RM_RiskIndicator_Max							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1020)

#define Cmd_RM_AddRiskIndicator_Req							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1002)	//添加风险指标请求
#define Cmd_RM_AddRiskIndicator_Rsp							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1003)	//添加风险指标返回
#define Cmd_RM_DelRiskIndicator_Req							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1004)	//删除风险指标请求
#define Cmd_RM_DelRiskIndicator_Rsp							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1005)	//删除风险指标返回
#define Cmd_RM_ModifyRiskIndicator_Req						MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1006)	//修改风险指标请求
#define Cmd_RM_ModifyRiskIndicator_Rsp						MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1007)	//修改风险指标返回
#define Cmd_RM_QryRiskIndicator_Req							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1008)	//查询风险指标请求
#define Cmd_RM_QryRiskIndicator_Rsp							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1009)	//查询风险指标返回


//风控事件类型
#define Cmd_RM_RiskEvent_Min								MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1021)
#define Cmd_RM_RiskEvent_Max								MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1040)

#define Cmd_RM_SubscribeRiskEvent_Req						MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1022)	//订阅风控事件请求
#define Cmd_RM_SubscribeRiskEvent_Rsp						MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1023)	//订阅风控事件返回	
#define Cmd_RM_RiskEvent_Push								MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1024)	//风控事件推送	
#define Cmd_RM_UnSubscribeRiskEvent_Req						MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1025)	//退订风控事件请求
#define Cmd_RM_UnSubscribeRiskEvent_Rsp						MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1026)	//退订风控事件返回
#define Cmd_RM_QryRiskEvent_Req								MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1027)	//查询风险事件请求
#define Cmd_RM_QryRiskEvent_Rsp								MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1028)	//查询风险事件返回


/*
//消息相关接口
#define Cmd_RM_Message_Min									MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1041)
#define Cmd_RM_Message_Max									MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1060)

#define Cmd_RM_AddMessage_Req								MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1042)	//添加消息请求
#define Cmd_RM_AddMessage_Rsp								MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1043)	//添加消息响应
#define Cmd_RM_QrySendMessage_Req							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1044)	//查询消息请求，查询已发送的消息
#define Cmd_RM_QrySendMessage_Rsp							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1045)	//查询消息返回
#define Cmd_RM_QryMsgSendStatus_Req							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1046)	//查询消息发送状态请求
#define Cmd_RM_QryMsgSendStatus_Rsp							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1047)	//查询消息发送状态返回
#define Cmd_RM_QryUnReadMessage_Req							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1048)	//查询未阅读的消息
#define Cmd_RM_SendUnReadMessage_Rsp						MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1049)	//发送未阅读的消息
#define Cmd_RM_SendUnReadMessage_Push						MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1050)	//推送未阅读的消息
#define Cmd_RM_QryRecvMessage_Req							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1051)	//查询已收到消息请求
#define Cmd_RM_QryRecvMessage_Rsp							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1052)	//返回已收到消息
*/

//行情							
#define	Cmd_RM_SubscribeQuot_Min			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1061)		
#define	Cmd_RM_SubscribeQuot_Max			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1080)			
#define	Cmd_RM_SubscribeQuot_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1062)	//订阅行情请求		
#define	Cmd_RM_SubscribeQuot_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1063)	//订阅行情返回
#define Cmd_RM_Quot_Push					MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1064)	//行情推送		
#define	Cmd_RM_UnSubscribeQuot_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1065)	//退订行情请求		
#define	Cmd_RM_UnSubscribeQuot_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1066)	//退订行情返回									
//出入金 机构版没有出入金的概念
/*
#define	Cmd_RM_SubscribeDeposit_Min			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1081)			
#define	Cmd_RM_SubscribeDeposit_Max			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10a0)			
#define	Cmd_RM_SubscribeDeposit_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1082)	//订阅出入金请求		
#define	Cmd_RM_SubscribeDeposit_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1083)	//订阅出入金返回		
#define	Cmd_RM_Deposit_Push					MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1084)	//出入金推送		
#define	Cmd_RM_UnSubscribeDeposit_Req		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1085)	//退订出入金请求		
#define	Cmd_RM_UnSubscribeDeposit_Rsp		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1086)	//退订出入金返回		
#define	Cmd_RM_QryTraderDeposit_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1087)//查询出入金请求		
#define	Cmd_RM_QryTraderDeposit_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1088)//查询出入金返回								
*/
//成交							
#define	Cmd_RM_SubscribeTrade_Min			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10a1)				
#define	Cmd_RM_SubscribeTrade_Max			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10c0)				
#define	Cmd_RM_SubscribeTrade_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10a2)		//订阅成交请求		
#define	Cmd_RM_SubscribeTrade_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10a3)		//订阅成交返回		
#define	Cmd_RM_Trade_Push					MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10a4)		//成交推送		
#define	Cmd_RM_UnSubscribeTrade_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10a5)		//退订成交请求		
#define	Cmd_RM_UnSubscribeTrade_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10a6)		//退订成交返回	
//#define Cmd_RM_QryHistroyTrade_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10a7)		//查询历史成交请求
//#define Cmd_RM_QryHistroyTrade_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10a8)		//查询历史成交返回

//资金							
#define	Cmd_RM_SubscribeFund_Min			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10c1)				
#define	Cmd_RM_SubscribeFund_Max			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10e0)				
#define	Cmd_RM_SubscribeFund_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10c2)		//订阅资金请求		
#define	Cmd_RM_SubscribeFund_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10c3)		//订阅资金返回		
#define	Cmd_RM_Fund_Push					MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10c4)		//资金推送		
#define	Cmd_RM_UnSubscribeFund_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10c5)		//退订资金请求		
#define	Cmd_RM_UnSubscribeFund_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10c6)		//退订资金返回
//#define Cmd_RM_QryHistroyRiskFund_Req		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10c7)		//查询历史资金请求
//#define Cmd_RM_QryHistroyRiskFund_Rsp		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10c8)		//查询历史资金返回

//持仓							
#define	Cmd_RM_SubscribePosition_Min		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10e1)			
#define	Cmd_RM_SubscribePosition_Max		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1100)			
#define	Cmd_RM_SubscribePosition_Req		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10e2)	//订阅持仓请求		
#define	Cmd_RM_SubscribePosition_Rsp		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10e3)	//订阅持仓返回		
#define	Cmd_RM_Position_Push				MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10e4)	//持仓推送		
#define	Cmd_RM_UnSubscribePosition_Req		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10e5)	//退订持仓请求		
#define	Cmd_RM_UnSubscribePosition_Rsp		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10e6)	//退订持仓返回	
//#define Cmd_RM_QryHistroyPosition_Req		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10e7)	//查询历史持仓请求
//#define Cmd_RM_QryHistroyPosition_Rsp		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10e8)	//查询历史持仓返回

//报单							
#define	Cmd_RM_SubscribeOrder_Min			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1101)			
#define	Cmd_RM_SubscribeOrder_Max			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1120)		
#define	Cmd_RM_SubscribeOrder_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1102)	//订阅报单请求		
#define	Cmd_RM_SubscribeOrder_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1103)	//订阅报单返回		
#define	Cmd_RM_Order_Push					MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1104)	//报单推送		
#define	Cmd_RM_UnSubscribeOrder_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1105)	//退订报单请求		
#define	Cmd_RM_UnSubscribeOrder_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1106)	//退订报单返回
#define Cmd_RM_QryHistroyOrder_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1107)	//查询历史报单请求
#define Cmd_RM_QryHistroyOrder_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1108)	//查询历史报单返回
#define Cmd_RM_VerifyOrder_Req				MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1109)	//未审核单审核结果请求
#define Cmd_RM_VerifyOrder_Rsp				MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x110a)	//未审核单审核结果返回
#define Cmd_RM_QryVerifyOrder_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x110b)	//报单审核结果查询请求
#define Cmd_RM_QryVerifyOrder_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x110c)	//报单审核结果查询返回
//基金净值
#define	Cmd_RM_FundNet_Min					MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1121)			
#define	Cmd_RM_FundNet_Max					MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1140)
#define	Cmd_RM_AddFundNetParam_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1122)	//请求设置基金净值参数
#define	Cmd_RM_AddFundNetParam_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1123)	//设置基金净值参数返回
#define	Cmd_RM_QueryFundNetParam_Req		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1124)	//请求查询设置基金净值参数，参数为委托交易账号ID
#define	Cmd_RM_QueryFundNetParam_Rsp		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1125)	//查询基金净值参数返回
#define	Cmd_RM_QueryFundNetCalcResult_Req	MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1126)	//请求查询基金净值计算结果
#define	Cmd_RM_QueryFundNetCalcResult_Rsp	MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1127)	//查询基金净值计算结果返回
#define	Cmd_RM_FundNetValue_push			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1128)	//基金净值推送

#define	Cmd_RM_ForceCloseDo_Min				MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1141)
#define	Cmd_RM_ForceCloseDo_Max				MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1160)
#define	Cmd_RM_ForceCloseOrderInsert_Req	MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1142)	//请求强平报单录入
#define	Cmd_RM_ForceCloseOrderInsert_Rsp	MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1143)	//强平报单录入返回
#define	Cmd_RM_QryForceCloseOrder_Req		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1144)	//请求查询强平报单录入
#define	Cmd_RM_QryForceCloseOrder_Rsp		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1145)	//强平查询报单录入返回
#define	Cmd_RM_OrderAction_Req				MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1146)	//请求撤单录入
#define	Cmd_RM_OrderAction_Rsp				MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1147)	//请求撤单录入返回


#define	Cmd_RM_QryAdmin_Min				MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1161)    //管理终端查询命令起始
#define	Cmd_RM_QryAdmin_Max				MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1180)	//管理终端查询命令结束
#define	Cmd_RM_QryHistoryFundInfo_Req		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1162)	//请求查询历史资金
#define	Cmd_RM_QryHistoryFundInfo_Rsp		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1163)	//强平查询历史资金返回
#define	Cmd_RM_QryHistoryPosition_Req		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1164)	//请求查询历史持仓
#define	Cmd_RM_QryHistoryPosition_Rsp		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1165)	//强平查询历史持仓返回
#define	Cmd_RM_QryHistoryPositionDetail_Req		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1166)	//请求查询历史持仓明细持仓
#define	Cmd_RM_QryHistoryPositionDetail_Rsp		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1167)	//强平查询历史持仓明细返回
#define	Cmd_RM_QryHistoryTrade_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1168)	//请求查询历史成交
#define	Cmd_RM_QryHistoryTrade_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1169)	//强平查询历史成交返回
#define	Cmd_RM_QryHistoryOrders_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x116a)	//请求查询历史报单
#define	Cmd_RM_QryHistoryOrders_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x116b)	//强平查询历史报单返回
#define	Cmd_RM_QryHistoryFundInOut_Req		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x116c)	//请求查询历史出入金
#define	Cmd_RM_QryHistoryFundInOut_Rsp		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x116d)	//强平查询历史出入金返回

#define	Cmd_RM_ToExecute_Min						MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1300)    //交易执行命令起始
#define	Cmd_RM_ToExecute_Max						MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1380)	//交易执行命令结束
#define	Cmd_RM_ToExecute_OrderAction_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1301)	//给交易执行 请求撤单录入
#define	Cmd_RM_ToExecute_OrderAction_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1302)	//给交易执行 请求撤单录入返回
#define Cmd_RM_ToExecute_VerifyOrder_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1303)	//给交易执行 未审核单审核结果请求
#define Cmd_RM_ToExecute_VerifyOrder_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1304)	//给交易执行 未审核单审核结果返回
#define	Cmd_RM_ToExecute_ForceCloseOrderInsert_Req	MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1305)	//给交易执行 请求强平报单录入
#define	Cmd_RM_ToExecute_ForceCloseOrderInsert_Rsp	MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1306)	//给交易执行 强平报单录入返回
#define	Cmd_RM_ToExecute_RiskForce_Req				MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1307)	//给交易执行 下单限制
#define	Cmd_RM_ToExecute_RiskForce_Rsp				MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1308)	//给交易执行 下单限制返回

#define Cmd_RM_Master_SetLimitTrade_Req				MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1309)	//转发主服务 设置限制交易请求
#define Cmd_RM_Master_SetLimitTrade_Rsp				MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x130a)	//转发主服务 设置限制交易返回
#define Cmd_RM_Master_SetManualVerify_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x130b)	//转发主服务 设置手动审核请求
#define Cmd_RM_Master_SetManualVerify_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x130c)	//转发主服务 设置手动审核返回
#define Cmd_RM_Master_ModifyUserPassword_Req		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x130d)	//转发主服务 修改用户密码请求
#define Cmd_RM_Master_ModifyUserPassword_Rsp		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x130e)	//转发主服务 修改用户密码返回

//统计展现使用CMD
#define	Cmd_RM_SubscribeStatistics_Req		MAKE_CMDID(CMDID_BASE_HIWORD_STATIC_PRESENTATION,0x0001)	 //订阅统计请求
#define	Cmd_RM_SubscribeStatistics_Rsp		MAKE_CMDID(CMDID_BASE_HIWORD_STATIC_PRESENTATION,0x0002)	 //订阅统计返回
#define	Cmd_RM_Statistics_Push				MAKE_CMDID(CMDID_BASE_HIWORD_STATIC_PRESENTATION,0x0003)  //听月统计推送
#define	Cmd_RM_UnSubscribeStatistics_Req	MAKE_CMDID(CMDID_BASE_HIWORD_STATIC_PRESENTATION,0x0004)  //退订请求
#define	Cmd_RM_UnSubscribeStatistics_Rsp	MAKE_CMDID(CMDID_BASE_HIWORD_STATIC_PRESENTATION,0x0005)  //退订返回
#define	Cmd_RM_SubscribeUserFund_Req		MAKE_CMDID(CMDID_BASE_HIWORD_STATIC_PRESENTATION,0x0006)	 //订阅资金请求		
#define	Cmd_RM_SubscribeUserFund_Rsp		MAKE_CMDID(CMDID_BASE_HIWORD_STATIC_PRESENTATION,0x0007)	 //订阅资金返回		
#define	Cmd_RM_UserFund_Push				MAKE_CMDID(CMDID_BASE_HIWORD_STATIC_PRESENTATION,0x0008)	 //资金推送		
#define	Cmd_RM_UnSubscribeUserFund_Req		MAKE_CMDID(CMDID_BASE_HIWORD_STATIC_PRESENTATION,0x0009)  //退订请求
#define	Cmd_RM_UnSubscribeUserFund_Rsp		MAKE_CMDID(CMDID_BASE_HIWORD_STATIC_PRESENTATION,0x000a)  //退订返回
#define	Cmd_RM_QryAvilabeInstrument_Req		MAKE_CMDID(CMDID_BASE_HIWORD_STATIC_PRESENTATION,0x000b)  //查询可有的合约
#define	Cmd_RM_QryAvilabeInstrument_Rsp		MAKE_CMDID(CMDID_BASE_HIWORD_STATIC_PRESENTATION,0x000c)  //查询可用的合约返回

//策略相关的CMD
#define CMD_STRTEGY_UPLOAD_Base_Req					MAKE_CMDID(CMDID_BASE_STRATEDY,0x0001)  //上传请求
#define CMD_STRTEGY_UPLOAD_Base_Rsp					MAKE_CMDID(CMDID_BASE_STRATEDY,0x0002)  //上传返回
//对应结构体SStrategy


#define CMD_STRTEGY_UPLOAD_STRATEGY2INDEX_Base_Req					MAKE_CMDID(CMDID_BASE_STRATEDY,0x0003)  //上传策略和指标关系请求
#define CMD_STRTEGY_UPLOAD_STRATEGY2INDEX_Base_Rsp					MAKE_CMDID(CMDID_BASE_STRATEDY,0x0004)  //上传策略和指标关系返回
//上传一个std::vector<SStrategy2Index>的数组

#define CMD_STRTEGY_UPLOAD_File_Req					MAKE_CMDID(CMDID_BASE_STRATEDY,0x0007)  //上传请求
#define CMD_STRTEGY_UPLOAD_File_Rsp					MAKE_CMDID(CMDID_BASE_STRATEDY,0x0008)  //上传返回
//第二次上传包括 int char szName[25]	char szUploader[25];	dll文件二进制内容; (int 2 表示上传dll)
//第三次上传包括 int char szName[25]	char szUploader[25];	zip文件二进制内容; (int 3 表示上传zip)

#define CMD_STRTEGY_DOWNLOAD_All_Base_Req					MAKE_CMDID(CMDID_BASE_STRATEDY,0x00010)  //下载所有策略请求
#define CMD_STRTEGY_DOWNLOAD_All_Base_Rsp					MAKE_CMDID(CMDID_BASE_STRATEDY,0x00011)  //下载所有策略返回
//传入参数：char szUploader[25]   如果为空指针，则代表所有策略
//返回对应结构体vector<SStrategy>


#define CMD_STRTEGY_DOWNLOAD_Base_Req					MAKE_CMDID(CMDID_BASE_STRATEDY,0x00012)  //上传请求
#define CMD_STRTEGY_DOWNLOAD_Base_Rsp					MAKE_CMDID(CMDID_BASE_STRATEDY,0x00013)  //上传返回
//char szName[25]   返回对应结构体SStrategy

#define CMD_STRTEGY_DOWNLOAD_Req				MAKE_CMDID(CMDID_BASE_STRATEDY,0x0015)  //下载请求
#define CMD_STRTEGY_DOWNLOAD_Rsp				MAKE_CMDID(CMDID_BASE_STRATEDY,0x0016)  //下载返回
//int char szName[25] char szUploader[25] int 2表示 请求策略dll，szName为策略名
//int char szName[25] char szUploader[25] int 3表示 请求策略zip，szName为策略名

//返回  int  char szName[25]	char szUploader[25];	dll文件二进制内容; //(int 2 表示上传dll)(int 3 表示上传zip)

#define CMD_STRTEGY_DEL_Req				MAKE_CMDID(CMDID_BASE_STRATEDY,0x0027)  //删除策略请求
#define CMD_STRTEGY_DEL_Rsp				MAKE_CMDID(CMDID_BASE_STRATEDY,0x0028)  //删除策略返回
//SDelStrategy


//指标相关的CMD
#define CMD_STRTEGY_UPLOADINDEX_Base_Req					MAKE_CMDID(CMDID_BASE_STRATEDY,0x0101)  //上传请求
#define CMD_STRTEGY_UPLOADINDEX_Base_Rsp					MAKE_CMDID(CMDID_BASE_STRATEDY,0x0102)  //上传返回
//SIndex

#define CMD_STRTEGY_UPLOADINDEX_File_Req					MAKE_CMDID(CMDID_BASE_STRATEDY,0x0103)  //上传请求
#define CMD_STRTEGY_UPLOADINDEX_File_Rsp					MAKE_CMDID(CMDID_BASE_STRATEDY,0x0104)  //上传返回
//第二次上传包括 int char szName[25]	char szUploader[25];	dll文件二进制内容; (int 2 表示上传dll)

#define CMD_STRTEGY_DOWNLOADINDEX_Base_Req					MAKE_CMDID(CMDID_BASE_STRATEDY,0x00112)  //下载请求
#define CMD_STRTEGY_DOWNLOADINDEX_Base_Rsp					MAKE_CMDID(CMDID_BASE_STRATEDY,0x00113)  //下载返回
//char szName[25] 返回对应结构体SIndex

#define CMD_STRTEGY_DOWNLOADINDEX_Req				MAKE_CMDID(CMDID_BASE_STRATEDY,0x0115)  //下载请求
#define CMD_STRTEGY_DOWNLOADINDEX_Rsp				MAKE_CMDID(CMDID_BASE_STRATEDY,0x0116)  //下载返回
//int char szName[25] char szUploader[25] int 2表示 请求策略dll，szName为策略名

#define CMD_STRTEGY_DOWNLOAD_ALLINDEX_Base_Req					MAKE_CMDID(CMDID_BASE_STRATEDY,0x00117)  //下载所有指标信息请求
#define CMD_STRTEGY_DOWNLOAD_ALLINDEX_Base_Rsp					MAKE_CMDID(CMDID_BASE_STRATEDY,0x00118)  //下载所有指标信息请求返回
//char szUploader[25]  参数某个交易员；  如果为空指针，则代表所有指标
//返回对应结构体vector<SIndex>，所有的自己的指标和别人共享的指标

#define CMD_STRTEGY_USE_Req						MAKE_CMDID(CMDID_BASE_STRATEDY,0x0119)  //策略启用和停用请求
#define CMD_STRTEGY_USE_Rsp						MAKE_CMDID(CMDID_BASE_STRATEDY,0x0120)  //策略启用和停用返回
//SUseStrategy 


#define CMD_INSTANCE_ADD_Req						MAKE_CMDID(CMDID_BASE_STRATEDY,0x0139)  //策略方案增加请求
#define CMD_INSTANCE_ADD_Rsp						MAKE_CMDID(CMDID_BASE_STRATEDY,0x0130)  //策略方案增加请求返回
//SStrategyInstance

#define CMD_INSTANCE_Modify_Req						MAKE_CMDID(CMDID_BASE_STRATEDY,0x0131)  //策略方案修改请求
#define CMD_INSTANCE_Modify_Rsp						MAKE_CMDID(CMDID_BASE_STRATEDY,0x0132)  //策略方案修改请求返回
//SStrategyInstance

#define CMD_INSTANCE_DEL_Req						MAKE_CMDID(CMDID_BASE_STRATEDY,0x0133)  //策略方案删除请求
#define CMD_INSTANCE_DEL_Rsp						MAKE_CMDID(CMDID_BASE_STRATEDY,0x0134)  //策略方案删除请求返回
//SDelStrategy

#define CMD_INSTANCE_USE_Req						MAKE_CMDID(CMDID_BASE_STRATEDY,0x0138)  //策略方案启用和停用请求
#define CMD_INSTANCE_USE_Rsp						MAKE_CMDID(CMDID_BASE_STRATEDY,0x0139)  //策略方案启用和停用返回
//SUseStrategy 

#define CMD_INSTANCE_AllDownlaod_Req				MAKE_CMDID(CMDID_BASE_STRATEDY,0x0140)  //下载所有策略方案请求
#define CMD_INSTANCE_AllDownlaod_Rsp				MAKE_CMDID(CMDID_BASE_STRATEDY,0x0141)  //下载所有策略方案返回
//请求char Uploader[25];
//返回std::vector<SStrategyInstance>& vecInstance

#define CMD_UPLOAD_START_Req						MAKE_CMDID(CMDID_BASE_STRATEDY,0x0170)  //开始上传请求
#define CMD_UPLOAD_START_Rsp						MAKE_CMDID(CMDID_BASE_STRATEDY,0x0171)  //开始上传请求返回
//请求参数： UploadStart
//返回：无

#define CMD_UPLOAD_END_Req							MAKE_CMDID(CMDID_BASE_STRATEDY,0x0172)  //上传结束请求
#define CMD_UPLOAD_END_Rsp							MAKE_CMDID(CMDID_BASE_STRATEDY,0x0173)  //上传结束请求返回
//请求参数： UploadEnd
//返回：无

/*
1.  上传一个指标（用户名、指标名、指标dll、指标源程序zip、私有/共享）
2.  上传一个策略（用户名、策略名、策略dll、策略源程序zip、私有/共享）
3.  下载(私有/共享)指标列表（指标名数组）
4.  下载(私有/共享)策略列表（策略名数组）
5.  下载一个(私有/共享)指标dll
6.  下载一个(私有/共享)策略dll
7.  下载一个私有指标源程序zip
7.  下载一个私有策略源程序zip

*/

#define CMD_QUERY_COMMISSION_Req					MAKE_CMDID(CMDID_BASE_STRATEDY,0x0182)  //查询合约手续费率
#define CMD_QUERY_COMMISSION_Rsp					MAKE_CMDID(CMDID_BASE_STRATEDY,0x0183)  //查询合约手续费率返回
//请求包：QueryCommission
//返回：PlatformStru_InstrumentCommissionRate

#define CMD_QUERY_MARGINRATE_Req					MAKE_CMDID(CMDID_BASE_STRATEDY,0x0184)  //查询合约保证金率
#define CMD_QUERY_MARGINRATE_Rsp					MAKE_CMDID(CMDID_BASE_STRATEDY,0x0185)  //查询合约保证金率返回
//请求包：QueryMarginRate
//返回：PlatformStru_InstrumentMarginRate


//通知和问询模块使用CMD
#define  CMDID_NotifyAndAsk_BroadcastNotify			MAKE_CMDID(CMDID_BASE_NotifyAndAsk,0x0001)	//通知消息广播给其它服务器
