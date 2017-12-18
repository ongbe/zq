#pragma once

//命令字为DWORD类型，其中，高位表示命令字分类，低位表示命令字编号

#define CMDID_HEARTBEAT				0x000001	//心跳命令
#define CMDID_HEARTBEATRSP			0x000002	//心跳答复

//登录相关命令字
#define Cmd_RM_Login_Min			0x10000
#define Cmd_RM_Login_Max			0x1FFFF
#define Cmd_RM_Login_Req			0x10001	//登录请求
#define Cmd_RM_Login_Rsp			0x10002	//登录返回
#define Cmd_RM_ForceCloseClient_Push	0x10004	//账号被停用时，强制关闭客户端

//管理员/风控员账号相关接口
#define Cmd_RM_Account_Min			0x20000
#define Cmd_RM_Account_Max			0x2FFFF
#define Cmd_RM_AddAccount_Req		0x20001	//添加账号请求
#define Cmd_RM_AddAccount_Rsp		0x20002	//添加账号返回

#define Cmd_RM_DelAccount_Req		0x20003	//删除账号请求
#define Cmd_RM_DelAccount_Rsp		0x20004	//删除账号返回

#define Cmd_RM_ModifyPwd_Req		0x20005	//修改密码请求
#define Cmd_RM_ModifyPwd_Rsp		0x20006	//修改密码返回
#define Cmd_RM_ModifyAccount_Req	0x20007	//修改账户基本信息请求
#define Cmd_RM_ModifyAccount_Rsp	0x20008	//修改账户基本信息返回

#define Cmd_RM_QryAccountByOrgID_Req	0x20009	//查询某资管组织下属所有账号请求，参数为资管组织ID
#define Cmd_RM_QryAccountByOrgID_Rsp	0x2000A	//账号查询返回

//委托交易账号相关接口
#define Cmd_RM_TradeAccount_Min			0x30000
#define Cmd_RM_TradeAccount_Max			0x3FFFF
#define Cmd_RM_AddTradeAccount_Req		0x30001	//添加委托交易账号请求
#define Cmd_RM_AddTradeAccount_Rsp		0x30002	//添加委托交易账号返回

#define Cmd_RM_DelTradeAccount_Req		0x30003	//删除委托交易账号请求
#define Cmd_RM_DelTradeAccount_Rsp		0x30004	//删除委托交易账号返回

#define Cmd_RM_ModifyTradeAccount_Req	0x30005	//修改委托交易账户基本信息请求
#define Cmd_RM_ModifyTradeAccount_Rsp	0x30006	//修改委托交易账户基本信息返回

#define Cmd_RM_QryTradeAccountByOrgID_Req		0x30007	//查询委托交易账户基本信息请求，参数为资管组织ID
#define Cmd_RM_QryTradeAccountByOrgID_Rsp		0x30008	//查询委托交易账户基本信息返回

//交易员相关接口
#define Cmd_RM_Trader_Min			0x40000
#define Cmd_RM_Trader_Max			0x4FFFF
#define Cmd_RM_AddTrader_Req		0x40001	//添加Trader信息请求
#define Cmd_RM_AddTrader_Rsp		0x40002	//添加Trader信息返回

#define Cmd_RM_DelTrader_Req		0x40003	//删除Trader信息请求
#define Cmd_RM_DelTrader_Rsp		0x40004	//删除Trader信息返回

#define Cmd_RM_ModifyTrader_Req		0x40005	//修改Trader信息请求
#define Cmd_RM_ModifyTrader_Rsp		0x40006	//修改Trader信息返回

#define Cmd_RM_QryTraderByOrgID_Req		0x40007	//查询Trader信息请求，参数为资管组织ID
#define Cmd_RM_QryTraderByOrgID_Rsp		0x40008	//查询Trader信息返回

//资管组织相关接口
#define Cmd_RM_AssetMgmtOrg_Min			0x50000
#define Cmd_RM_AssetMgmtOrg_Max			0x5FFFF
#define Cmd_RM_AddAssetMgmtOrg_Req		0x50001	//添加AssetMgmtOrg信息请求
#define Cmd_RM_AddAssetMgmtOrg_Rsp		0x50002	//添加AssetMgmtOrg信息返回

#define Cmd_RM_DelAssetMgmtOrg_Req		0x50003	//删除AssetMgmtOrg信息请求
#define Cmd_RM_DelAssetMgmtOrg_Rsp		0x50004	//删除AssetMgmtOrg信息返回

#define Cmd_RM_ModifyAssetMgmtOrg_Req	0x50005	//修改AssetMgmtOrg信息请求
#define Cmd_RM_ModifyAssetMgmtOrg_Rsp	0x50006	//修改AssetMgmtOrg信息返回

#define Cmd_RM_QryAssetMgmtOrgByOrgID_Req		0x50007	//查询某资管组织下属所有AssetMgmtOrg信息请求
#define Cmd_RM_QryAssetMgmtOrgByOrgID_Rsp		0x50008	//返回某资管组织下属所有AssetMgmtOrg信息

#define Cmd_RM_QryAssetMgmtOrg_Req		0x50009	//查询所有AssetMgmtOrg信息请求
#define Cmd_RM_QryAssetMgmtOrg_Rsp		0x5000A	//返回AssetMgmtOrg信息

//理财产品相关接口
#define Cmd_RM_FinancialProduct_Min			0x60000
#define Cmd_RM_FinancialProduct_Max			0x6FFFF
#define Cmd_RM_AddFinancialProduct_Req		0x60001	//添加理财产品请求
#define Cmd_RM_AddFinancialProduct_Rsp		0x60002	//添加理财产品返回

#define Cmd_RM_DelFinancialProduct_Req		0x60003	//删除理财产品请求
#define Cmd_RM_DelFinancialProduct_Rsp		0x60004	//删除理财产品返回

#define Cmd_RM_ModifyFinancialProduct_Req	0x60005	//修改理财产品信息请求
#define Cmd_RM_ModifyFinancialProduct_Rsp	0x60006	//修改理财产品信息返回

#define Cmd_RM_QryFinancialProduct_Req		0x60007	//查询理财产品信息请求
#define Cmd_RM_QryFinancialProduct_Rsp		0x60008	//查询理财产品信息返回

#define Cmd_RM_QryAssetOrgProRelation_Req	0x60009	//查询资管组织与理财产品关系
#define Cmd_RM_QryAssetOrgProRelation_Rsp	0x6000A	//返回资管组织与理财产品关系

#define Cmd_RM_SaveAssetOrgProRelation_Req	0x6000B	//保存资管组织与理财产品关系请求
#define Cmd_RM_SaveAssetOrgProRelation_Rsp	0x6000C	//保存资管组织与理财产品关系返回

//风险指标模型相关接口
#define Cmd_RM_RiskIndModule_Min			0x70000
#define Cmd_RM_RiskIndModule_Max			0x7FFFF
#define Cmd_RM_AddRiskIndModule_Req			0x70001	//添加风险指标模型请求
#define Cmd_RM_AddRiskIndModule_Rsp			0x70002	//添加风险指标模型返回

#define Cmd_RM_DelRiskIndModule_Req			0x70003	//删除风险指标模型请求
#define Cmd_RM_DelRiskIndModule_Rsp			0x70004	//删除风险指标模型返回

#define Cmd_RM_ModifyRiskIndModule_Req		0x70005	//修改风险指标模型请求
#define Cmd_RM_ModifyRiskIndModule_Rsp		0x70006	//修改风险指标模型返回

#define Cmd_RM_QryRiskIndModule_Req			0x70007	//查询风险指标模型请求
#define Cmd_RM_QryRiskIndModule_Rsp			0x70008	//查询风险指标模型返回

#define Cmd_RM_QryValidRiskIndModule_Req	0x70009	//查询有效的风险指标模型请求
#define Cmd_RM_QryValidRiskIndModule_Rsp	0x7000A	//查询有效的风险指标模型返回

//风险预警相关接口
#define Cmd_RM_RiskWarning_Min			0x80000
#define Cmd_RM_RiskWarning_Max			0x8FFFF
#define Cmd_RM_QryRiskWarning_Req			0x80001	//查询风险预警请求
#define Cmd_RM_QryRiskWarning_Rsp			0x80002	//查询风险预警返回

#define Cmd_RM_SaveRiskWarning_Req			0x80003	//保存风险预警请求
#define Cmd_RM_SaveRiskWarning_Rsp			0x80004	//保存风险预警返回

#define Cmd_RM_QryMaxRiskLevel_Req			0x80005	//查询每个风险指标的最大风险级别请求
#define Cmd_RM_QryMaxRiskLevel_Rsp			0x80006	//返回每个风险指标的最大风险级别

#define Cmd_RM_QryProductRiskWarning_Req	0x80007	//按品种查询风险预警请求
#define Cmd_RM_QryProductRiskWarning_Rsp	0x80008	//按品种查询风险预警返回

#define Cmd_RM_SaveProductRiskWarning_Req	0x80009	//按品种保存风险预警请求
#define Cmd_RM_SaveProductRiskWarning_Rsp	0x8000A	//按品种保存风险预警返回

#define Cmd_RM_DelProductRiskWarning_Req	0x8000B	//按品种删除风险预警请求
#define Cmd_RM_DelProductRiskWarning_Rsp	0x8000C	//按品种删除风险预警返回

//风险指标相关接口
#define Cmd_RM_RiskIndicator_Min			0x90000
#define Cmd_RM_RiskIndicator_Max			0x9FFFF
#define Cmd_RM_AddRiskIndicator_Req			0x90001	//添加风险指标请求
#define Cmd_RM_AddRiskIndicator_Rsp			0x90002	//添加风险指标返回

#define Cmd_RM_DelRiskIndicator_Req			0x90003	//删除风险指标请求
#define Cmd_RM_DelRiskIndicator_Rsp			0x90004	//删除风险指标返回

#define Cmd_RM_ModifyRiskIndicator_Req		0x90005	//修改风险指标请求
#define Cmd_RM_ModifyRiskIndicator_Rsp		0x90006	//修改风险指标返回

#define Cmd_RM_QryRiskIndicator_Req			0x90007	//查询风险指标请求
#define Cmd_RM_QryRiskIndicator_Rsp			0x90008	//查询风险指标返回

//事件消息模版相关接口
#define Cmd_RM_EvtMsgTemplate_Min			0xA0000
#define Cmd_RM_EvtMsgTemplate_Max			0xAFFFF
#define Cmd_RM_QryEvtMsgTemplate_Req		0xA0001	//查询事件消息请求
#define Cmd_RM_QryEvtMsgTemplate_Rsp		0xA0002	//查询事件消息返回

#define Cmd_RM_SaveEvtMsgTemplate_Req		0xA0003	//保存事件消息请求
#define Cmd_RM_SaveEvtMsgTemplate_Rsp		0xA0004	//保存事件消息返回

//风险事件相关接口
#define Cmd_RM_RiskEvent_Min			0xB0000
#define Cmd_RM_RiskEvent_Max			0xBFFFF
#define	Cmd_RM_SubscribeRiskEvent_Req			0xB0001	//订阅风控事件请求		
#define	Cmd_RM_SubscribeRiskEvent_Rsp			0xB0002	//订阅风控事件返回		
#define	Cmd_RM_RiskEvent_Push					0xB0003	//风控事件推送		
#define	Cmd_RM_UnSubscribeRiskEvent_Req			0xB0004	//退订风控事件请求		
#define	Cmd_RM_UnSubscribeRiskEvent_Rsp			0xB0005	//退订风控事件返回	

#define Cmd_RM_QryRiskEvent_Req			0xB0006	//查询风险事件请求
#define Cmd_RM_QryRiskEvent_Rsp			0xB0007	//查询风险事件返回


//风险事件处理相关接口
#define Cmd_RM_RiskEvtHandling_Min			0xC0000
#define Cmd_RM_RiskEvtHandling_Max			0xCFFFF
#define Cmd_RM_AddRiskEvtHandling_Req		0xC0001	//添加风险事件处理请求
#define Cmd_RM_AddRiskEvtHandling_Rsp		0xC0002	//添加风险事件处理返回

#define Cmd_RM_QryRiskEvtHandling_Req		0xC0007	//查询风险事件处理请求
#define Cmd_RM_QryRiskEvtHandling_Rsp		0xC0008	//查询风险事件处理返回

#define	Cmd_RM_SubscribeRiskEvtHandling_Req			0xC0009	//订阅风控事件处理请求		
#define	Cmd_RM_SubscribeRiskEvtHandling_Rsp			0xC000A	//订阅风控事件处理返回		
#define	Cmd_RM_RiskEvtHandling_Push					0xC000B	//风控事件处理推送		
#define	Cmd_RM_UnSubscribeRiskEvtHandling_Req		0xC000C	//退订风控事件处理请求		
#define	Cmd_RM_UnSubscribeRiskEvtHandling_Rsp		0xC000D	//退订风控事件处理返回	

//权限相关接口
#define Cmd_RM_Permission_Min				0xD0000
#define Cmd_RM_Permission_Max				0xD9999
#define Cmd_RM_QryPermissionNode_Req		0xD0001	//查询权限节点请求
#define Cmd_RM_QryPermissionNode_Rsp		0xD0002	//查询权限节点返回

#define Cmd_RM_QryPermissionAction_Req		0xD0003	//查询权限动作请求
#define Cmd_RM_QryPermissionAction_Rsp		0xD0004	//查询权限动作返回

#define Cmd_RM_AddPermissionName_Req		0xD0005	//添加权限名请求
#define Cmd_RM_AddPermissionName_Rsp		0xD0006	//添加权限名返回

#define Cmd_RM_DelPermissionName_Req		0xD0007	//删除权限名请求
#define Cmd_RM_DelPermissionName_Rsp		0xD0008	//删除权限名返回

#define Cmd_RM_ModifyPermissionName_Req		0xD0009	//修改权限名请求
#define Cmd_RM_ModifyPermissionName_Rsp		0xD000A	//修改权限名返回

#define Cmd_RM_QryPermissionName_Req		0xD000B	//查询权限名请求
#define Cmd_RM_QryPermissionName_Rsp		0xD000C	//查询权限名返回

#define Cmd_RM_QryPermissionValue_Req		0xD000D	//查询所有权限内容请求
#define Cmd_RM_QryPermissionValue_Rsp		0xD000E	//查询所有权限内容返回

#define Cmd_RM_QryPermissionValueByID_Req	0xD000F	//查询权限内容请求，参数为权限ID
#define Cmd_RM_QryPermissionValueByID_Rsp	0xD0010	//查询权限内容返回，参数为权限ID

#define Cmd_RM_SavePermissionValue_Req		0xD0011	//保存权限内容请求
#define Cmd_RM_SavePermissionValue_Rsp		0xD0012	//保存权限内容返回

#define Cmd_RM_QryPermissionNameEx_Req		0xD0013	//查询权限名扩展结构请求
#define Cmd_RM_QryPermissionNameEx_Rsp		0xD0014	//查询权限名扩展结构返回

//消息相关接口
#define Cmd_RM_Message_Min					0xE0000
#define Cmd_RM_Message_Max					0xEFFFF
#define Cmd_RM_AddMessage_Req				0xE0001	//添加消息请求
#define Cmd_RM_AddMessage_Rsp				0xE0002	//添加消息响应
#define Cmd_RM_QrySendMessage_Req			0xE0003	//查询消息请求，查询已发送的消息
#define Cmd_RM_QrySendMessage_Rsp			0xE0004	//查询消息返回
#define Cmd_RM_QryMsgSendStatus_Req			0xE0005	//查询消息发送状态请求
#define Cmd_RM_QryMsgSendStatus_Rsp			0xE0006	//查询消息发送状态返回
#define Cmd_RM_QryUnReadMessage_Req			0xE0007	//查询未阅读的消息
#define Cmd_RM_SendUnReadMessage_Rsp		0xE0008	//发送未阅读的消息
#define Cmd_RM_SendUnReadMessage_Push		0xE0009	//推送未阅读的消息
#define Cmd_RM_QryRecvMessage_Req			0xE000A	//查询已收到消息请求
#define Cmd_RM_QryRecvMessage_Rsp			0xE000B	//返回已收到消息

//合约信息							
#define	Cmd_RM_Instrument_Min			0xF0000			
#define	Cmd_RM_Instrument_Max			0xFFFFF	
#define	Cmd_RM_InstrumentIDList_Req		0xF0001
#define	Cmd_RM_InstrumentIDList_Rsp		0xF0002
#define	Cmd_RM_InstrumentInfo_Req		0xF0003
#define	Cmd_RM_InstrumentInfo_Rsp		0xF0004
#define	Cmd_RM_InstrumentInfoList_Req	0xF0005
#define	Cmd_RM_InstrumentInfoList_Rsp	0xF0006
#define	Cmd_RM_ProductIDList_Req		0xF0007
#define Cmd_RM_ProductIDList_Rsp		0xF0008 
//行情							
#define	Cmd_RM_SubscribeQuot_Min			0x100000			
#define	Cmd_RM_SubscribeQuot_Max			0x10FFFF			
#define	Cmd_RM_SubscribeQuot_Req			0x100001	//订阅行情请求		
#define	Cmd_RM_SubscribeQuot_Rsp			0x100002	//订阅行情返回		
#define	Cmd_RM_Quot_Push					0x100003	//行情推送		
#define	Cmd_RM_UnSubscribeQuot_Req			0x100004	//退订行情请求		
#define	Cmd_RM_UnSubscribeQuot_Rsp			0x100005	//退订行情返回									
//出入金							
#define	Cmd_RM_SubscribeDeposit_Min			0x110000			
#define	Cmd_RM_SubscribeDeposit_Max			0x11FFFF			
#define	Cmd_RM_SubscribeDeposit_Req			0x110001	//订阅出入金请求		
#define	Cmd_RM_SubscribeDeposit_Rsp			0x110002	//订阅出入金返回		
#define	Cmd_RM_Deposit_Push					0x110003	//出入金推送		
#define	Cmd_RM_UnSubscribeDeposit_Req		0x110004	//退订出入金请求		
#define	Cmd_RM_UnSubscribeDeposit_Rsp		0x110005	//退订出入金返回		
#define	Cmd_RM_QryTraderDeposit_Req		0x110006	//查询出入金请求		
#define	Cmd_RM_QryTraderDeposit_Rsp		0x110007	//查询出入金返回								
//成交							
#define	Cmd_RM_SubscribeTrade_Min			0x120000			
#define	Cmd_RM_SubscribeTrade_Max			0x12FFFF			
#define	Cmd_RM_SubscribeTrade_Req			0x120001	//订阅成交请求		
#define	Cmd_RM_SubscribeTrade_Rsp			0x120002	//订阅成交返回		
#define	Cmd_RM_Trade_Push					0x120003	//成交推送		
#define	Cmd_RM_UnSubscribeTrade_Req			0x120004	//退订成交请求		
#define	Cmd_RM_UnSubscribeTrade_Rsp			0x120005	//退订成交返回	
#define Cmd_RM_QryHistroyTrade_Req			0x120006	//查询历史成交请求
#define Cmd_RM_QryHistroyTrade_Rsp			0x120007	//查询历史成交返回
							
//资金							
#define	Cmd_RM_SubscribeFund_Min			0x130000			
#define	Cmd_RM_SubscribeFund_Max			0x13FFFF			
#define	Cmd_RM_SubscribeFund_Req			0x130001	//订阅资金请求		
#define	Cmd_RM_SubscribeFund_Rsp			0x130002	//订阅资金返回		
#define	Cmd_RM_Fund_Push					0x130003	//资金推送		
#define	Cmd_RM_UnSubscribeFund_Req			0x130004	//退订资金请求		
#define	Cmd_RM_UnSubscribeFund_Rsp			0x130005	//退订资金返回
#define Cmd_RM_QryHistroyRiskFund_Req		0x130006	//查询历史资金请求
#define Cmd_RM_QryHistroyRiskFund_Rsp		0x130007	//查询历史资金返回
							
//持仓							
#define	Cmd_RM_SubscribePosition_Min			0x140000			
#define	Cmd_RM_SubscribePosition_Max			0x14FFFF			
#define	Cmd_RM_SubscribePosition_Req			0x140001	//订阅持仓请求		
#define	Cmd_RM_SubscribePosition_Rsp			0x140002	//订阅持仓返回		
#define	Cmd_RM_Position_Push					0x140003	//持仓推送		
#define	Cmd_RM_UnSubscribePosition_Req			0x140004	//退订持仓请求		
#define	Cmd_RM_UnSubscribePosition_Rsp			0x140005	//退订持仓返回	
#define Cmd_RM_QryHistroyPosition_Req			0x140006	//查询历史持仓请求
#define Cmd_RM_QryHistroyPosition_Rsp			0x140007	//查询历史持仓返回
							
//报单							
#define	Cmd_RM_SubscribeOrder_Min			0x150000			
#define	Cmd_RM_SubscribeOrder_Max			0x15FFFF			
#define	Cmd_RM_SubscribeOrder_Req			0x150001	//订阅报单请求		
#define	Cmd_RM_SubscribeOrder_Rsp			0x150002	//订阅报单返回		
#define	Cmd_RM_Order_Push					0x150003	//报单推送		
#define	Cmd_RM_UnSubscribeOrder_Req			0x150004	//退订报单请求		
#define	Cmd_RM_UnSubscribeOrder_Rsp			0x150005	//退订报单返回
#define Cmd_RM_QryHistroyOrder_Req			0x150006	//查询历史报单请求
#define Cmd_RM_QryHistroyOrder_Rsp			0x150007	//查询历史报单返回

//强平方案
#define	Cmd_RM_ForceCloseSolution_Min			0x160000
#define	Cmd_RM_ForceCloseSolution_Max			0x16FFFF	
#define	Cmd_RM_ForceCloseSolution_Req			0x160001	//请求强平方案详情
#define	Cmd_RM_ForceCloseSolution_Rsp			0x160002	//强平方案详情返回
#define	Cmd_RM_DelForceCloseSolution_Req		0x160003	//请求删除强平方案
#define	Cmd_RM_DelForceCloseSolution_Rsp		0x160004	//删除强平方案返回
#define	Cmd_RM_EditForceCloseSolution_Req		0x160005	//请求修改方案名称
#define	Cmd_RM_EditForceCloseSolution_Rsp		0x160006	//修改方案名称返回
#define	Cmd_RM_DefaultForceCloseSolution_Req	0x160007	//请求修改默认强平方案
#define	Cmd_RM_DefaultForceCloseSolution_Rsp	0x160008	//修改默认强平方案返回
#define	Cmd_RM_AddForceCloseSolution_Req		0x160009	//请求添加强平方案
#define	Cmd_RM_AddForceCloseSolution_Rsp		0x160010	//添加强平方案返回

//强平规则
#define	Cmd_RM_ForceCloseRule_Min				0x170000
#define	Cmd_RM_ForceCloseRule_Max				0x17FFFF
#define	Cmd_RM_ForceCloseRule_Req				0x170001	//请求强平规则详情
#define	Cmd_RM_ForceCloseRule_Rsp				0x170002	//强平规则详情返回
#define	Cmd_RM_DelForceCloseRule_Req			0x170003	//请求删除强平规则
#define	Cmd_RM_DelForceCloseRule_Rsp			0x170004	//删除强平规则返回
#define	Cmd_RM_EditForceCloseRule_Req			0x170005	//请求修改强平规则
#define	Cmd_RM_EditForceCloseRule_Rsp			0x170006	//修改强平规则返回
#define	Cmd_RM_AddForceCloseRule_Req			0x170007	//请求添加强平规则
#define	Cmd_RM_AddForceCloseRule_Rsp			0x170008	//添加强平规则返回

//强平计算和下单
#define	Cmd_RM_ForceCloseDo_Min				0x180000
#define	Cmd_RM_ForceCloseDo_Max				0x18FFFF
#define	Cmd_RM_ForceCloseCalc_Req			0x180001	//请求强平计算
#define	Cmd_RM_ForceCloseCalc_Rsp			0x180002	//强平计算返回
#define	Cmd_RM_ForceCloseOrderInsert_Req	0x180003	//请求强平报单录入
#define	Cmd_RM_ForceCloseOrderInsert_Rsp	0x180004	//强平报单录入返回

#define	Cmd_RM_AddFundNetParam_Req				0x180009	//请求设置基金净值参数
#define	Cmd_RM_AddFundNetParam_Rsp				0x18000A	//设置基金净值参数返回
#define	Cmd_RM_QueryFundNetParam_Req			0x18000B	//请求查询设置基金净值参数，参数为委托交易账号ID
#define	Cmd_RM_QueryFundNetParam_Rsp			0x18000C	//查询基金净值参数返回

#define	Cmd_RM_QueryFundNetCalcResult_Req		0x18000F	//请求查询基金净值计算结果
#define	Cmd_RM_QueryFundNetCalcResult_Rsp		0x180010	//查询基金净值计算结果返回

#define	Cmd_RM_AddForceCloseParam_Req			0x180011	//请求自动强平参数
#define	Cmd_RM_AddForceCloseParam_Rsp			0x180012	//设置自动强平返回
#define	Cmd_RM_QueryForceCloseParam_Req			0x180013	//请求查询自动强平参数
#define	Cmd_RM_QueryForceCloseParam_Rsp			0x180014	//查询自动强平参数返回

#define	Cmd_RM_FundNetValue_push				0x180015	//基金净值推送

//统计计算
#define	Cmd_RM_Statistics_Min				0x190000
#define	Cmd_RM_Statistics_Max				0x19FFFF
#define	Cmd_RM_SubscribeStatistics_Req		0x190001	//订阅统计请求		
#define	Cmd_RM_SubscribeStatistics_Rsp		0x190002	//订阅统计返回		
#define	Cmd_RM_Statistics_Push				0x190003	//统计推送		
#define	Cmd_RM_UnSubscribeStatistics_Req	0x190004	//退订统计请求		
#define	Cmd_RM_UnSubscribeStatistics_Rsp	0x190005	//退订统计返回

//汇总计算
#define	Cmd_RM_Gather_Min								0x200000
#define	Cmd_RM_Gather_Max								0x20FFFF
#define	Cmd_RM_Gather_ByOrgID_Req						0x200001	//订阅（某部门）下所有帐户汇总信息请求
#define	Cmd_RM_Gather_ByOrgID_Rsp						0x200002	//订阅（某部门）下所有帐户汇总信息返回	

#define	Cmd_RM_Gather_ByFinancialProductID_Req			0x200003	//订阅（某理财产品）下所有帐户汇总信息请求	
#define	Cmd_RM_Gather_ByFinancialProductID_Rsp			0x200004	//订阅（某理财产品）下所有帐户汇总信息返回	

#define	Cmd_RM_Gather_ByAccountID_Req					0x200005	//订阅（某交易账号）下所有帐户汇总信息请求		
#define	Cmd_RM_Gather_ByAccountID_Rsp					0x200006	//订阅（某交易账号）下所有帐户汇总信息返回	

#define Cmd_RM_Rank_ByAccount_Req						0x200007	//订阅昨日帐户收益Top10排行榜请求	
#define Cmd_RM_Rank_ByAccount_Rsp						0x200008	//订阅昨日帐户收益Top10排行榜返回	

#define Cmd_RM_Rank_ByFinancialProductID_Req			0x200009	//订阅昨日年化理财产品收益Top10排行榜请求	
#define Cmd_RM_Rank_ByFinancialProductID_Rsp			0x20000a	//订阅昨日年化理财产品收益Top10排行榜返回	

#define Cmd_RM_AnnualRate_ByAccount_Req					0x20000d	//订阅年化帐户Top10排行榜请求	
#define Cmd_RM_AnnualRate_ByAccount_Rsp					0x20000e	//订阅年化帐户Top10排行榜返回	

#define Cmd_RM_AnnualRate_ByFinancialProductID_Req		0x20000f	//订阅年化理财产品Top10排行榜请求	
#define Cmd_RM_AnnualRate_ByFinancialProductID_Rsp		0x200011	//订阅年化理财产品Top10排行榜返回	

#define Cmd_RM_AnnualRate_ByFinancialProductID_Worst_Req		0x200012	//订阅年化理财产品最差Top10排行榜请求	
#define Cmd_RM_AnnualRate_ByFinancialProductID_Worst_Rsp		0x200013	//订阅年化理财产品最差Top10排行榜返回

#define Cmd_RM_GetAccountList_ByFinancialProductID_Req		0x200014	//根据理财产品ID获取所有的委托交易账号请求	
#define Cmd_RM_GetAccountList_ByFinancialProductID_Rsp		0x200015	//根据理财产品ID获取所有的委托交易账号返回

#define Cmd_Rm_GetProfitCurve_Req		0x200016	//收益曲线图报表接口请求
#define Cmd_Rm_GetProfitCurve_Rsp		0x200017    //收益曲线图报表接口返回

#define OPERATOR_SUCCESS	1		//请求成功
#define OPERATOR_FAILED		0		//请求失败