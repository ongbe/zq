2014.11.20
	1. 在Module-Misc2下增加RuntimeConfigMgr.h/cpp，放一些运行时的配置信息
	2. 增加第三方插件接口：
		CMDID_UI_ColorScheme_QUERY: 查询UI配色方案，参数为空
		CMDID_UI_ColorScheme_RSP: 返回UI配色方案，成功时lpData指向一个Int, 0:浅底色方案；1:深底色方案; cbData=4

2014.11.12
	1. 底层增加询价订阅、退订、通知
		(1)订阅询价
			///订阅询价通知，OptionInstrumentIDs为空则订阅全部期权合约的询价通知
			int  SubscribeForQuoteRsp(const vector<string>& OptionInstrumentIDs);
		(2)退订询价
			///退订询价通知，OptionInstrumentIDs为空则退订全部合约的询价通知
			int UnSubscribeForQuoteRsp(const vector<string>& OptionInstrumentIDs);
		(3)QuotSpi中增加应答处理
			#ifdef CTP060300
				///订阅询价应答
				void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
				///退订询价应答
				void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
				///询价通知
				void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp);
			#endif
		(4)应答返回的BID和数据结构：
			订阅询价应答	: 	BID_RspSubForQuoteRsp, 	DataRspSubForQuoteRsp
			退订询价应答	:	BID_RspUnSubForQuoteRsp,DataRspUnSubForQuoteRsp
			询价通知		:	BID_RtnForQuoteRsp,		DataRtnForQuoteRsp
	2. 增加配置项
		(1)在platform.xml中增加配置项ReceiveForQuote，表示是否支持接收询价通知的功能，默认不支持
			<platform>
				<Option>
					<ReceiveForQuote>1</ReceiveForQuote>
				</Option>
			</platform>
		(2)用GlobalConfigManager::m_CanReceiveForQuote获取该配置参数
		

2014.11.6
	1. 插件修改：
	   (1)插件管理中，执行“审核插件退出时，取消审核功能”时，审核插件窗口错误地用了m_hSendThreadHandle，更正为m_hOrderVerifyPlugin
	   (2)插件配置xml中，增加两个配置项，允许打开一个插件的多个实例进程，以及允许打开插件时将当前合约ID作为命令行参数带入：
			//是否可以打开多个实例，1可以，0不可以，默认为0
			<MultiInstance>0</MultiInstance>	
			//启动时是否需要将当前合约ID作为参数，1需要，0不需要，默认为0。
			//如需要，则打开方式为： PluginApp.exe TopWndOfFastTrader CurrentInstrumentID=***
			<NeedCurrentInstrumentID>0</NeedCurrentInstrumentID> 	
	2. 底层增加接口：
			//判断是否是期权标的物期货合约
			bool IsUnderlyingInstrumentID(const string& InstrumentID)
	3. FastTrader在RunMode为2/3时，对期权策略插件进行特殊处理。只有在当前合约是期权标的物合约时，才允许在报价表窗口的右键菜单和左键双击打开期权策略窗口		

2014.11.5
	1. CTools_Ansi增加GetFirstAlphasOfChineseStrGBK(),取汉字串里每个汉字的首字母组成的串。汉字必须是GB2312编码
	2. PlatformStru_ProductInfo增加ProductShortName，表示产品简称。缺省等于ProductName，对于ProductName过长的情况，在productshortname.xml里进行定义
	3. 底层查询合约结束后，通过合约反查产品，删除没有合约的产品(实盘里发现有这种情况，出现有过期的产品)
    4. 增加底层接口，获取全部交易所的品种信息 
		void GetExchangeID_ProductInfos(vector<pair<string,vector<PlatformStru_ProductInfo>>>& outData);
	5. 增加第三方插件接口，获取全部品种信息
		CMDID_PRODUCT_ALLINFO_QUERY：查询全部品种详情。参数为空
		CMDID_PRODUCT_ALLINFO_RSP  ：查询全部品种详情响应。成功时lpData指向PlatformStru_ProductInfo数组，cbData=n*sizeof(PlatformStru_ProductInfo)
	6. 增加第三方插件接口
		通知插件更改配色方案：通知插件更改配色方案。lpData指向一个Int, 0:浅底色方案；1:深底色方案

2014.10.24
	底层增加对询价和执行的处理
	1. 底层提供如下接口：
		///请求询价录入
		virtual int ReqForQuoteInsert(PlatformStru_InputForQuoteField& InputForQuote, int nRequestID)=0;
		///执行宣告录入请求
		virtual int ReqExecOrderInsert(PlatformStru_InputExecOrderField& InputExecOrder, int nRequestID)=0;
		///执行宣告操作请求
		virtual int ReqExecOrderAction(PlatformStru_InputExecOrderActionField& InputExecOrderAction, int nRequestID)=0;
		///请求查询执行宣告
		virtual int ReqQryExecOrder(PlatformStru_QryExecOrderField& QryExecOrder, int nRequestID)=0;
	2. 底层处理如下回执和回报：
		///询价录入请求响应
		void OnRspForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		///执行宣告录入请求响应
		void OnRspExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		///执行宣告操作请求响应
		void OnRspExecOrderAction(CThostFtdcInputExecOrderActionField *pInputExecOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		///请求查询执行宣告响应
		void OnRspQryExecOrder(CThostFtdcExecOrderField *pExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		///询价录入错误回报
		void OnErrRtnForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo);
		///执行宣告录入错误回报
		void OnErrRtnExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo);
		///执行宣告操作错误回报
		void OnErrRtnExecOrderAction(CThostFtdcExecOrderActionField *pExecOrderAction, CThostFtdcRspInfoField *pRspInfo);
		///执行宣告通知
		void OnRtnExecOrder(CThostFtdcExecOrderField *pExecOrder);
	3. 底层回执和回报返回的BID和数据结构：
		询价录入请求响应		: 	BID_RspForQuoteInsert, 		DataRspForQuoteInsert
		执行宣告录入请求响应	:	BID_RspExecOrderInsert,		DataRspExecOrderInsert
		执行宣告操作请求响应	:	BID_RspExecOrderAction,		DataRspExecOrderAction
		请求查询执行宣告响应	:	BID_RspQryExecOrder,		DataRspQryExecOrder
		询价录入错误回报		:	BID_ErrRtnForQuoteInsert,	DataErrRtnForQuoteInsert
		执行宣告录入错误回报	:	BID_ErrRtnExecOrderInsert,	DataErrRtnExecOrderInsert
		执行宣告操作错误回报	:	BID_ErrRtnExecOrderAction,	DataErrRtnExecOrderAction
		执行宣告通知			:	BID_RtnExecOrder,			DataRtnExecOrder
	

2014.9.29
	1. 增加期权合约报单冻结保证金的计算
	2. 增加期权合约持仓明细的保证金计算。期权持仓明细暂不考虑组合优惠
	3. 增加期权合约持仓的保证金计算。期权持仓明细暂不考虑组合优惠
	4. 合约列表中，期权手续费率和交易成本查询回来时，更新合约信息的显示

2014.9.23
	1. 查询到手续费率响应后，原来在m_pDataMgr->OnRspQryInstrumentCommissionRate()中判断是品种手续费还是合约手续费，现改为在DataCenter中判断
	2. ExtListCtrl中增加RefreshHeader()方法。合约列表中，从无垂直滚动条切换到有垂直滚动条的窗口，如果水平滚动条在最右边，存在表头未刷新的问题。强制刷新一下表头
	3. 在合约列表中显示期权的行权手续费率和保证金最新部分及最小保证金

2014.9.22
	1. 在xml中以统一方式缓存当日交易参数、品种、合约、手续费率、保证金率等信息
		1) 分别保存为tradingparam.xml、product.xml、instrument.xml、commissionrate.xml、marginrate.xml
		1) 在DataCenter.cpp的BID_RspUserLogin时读取并设置到底层数据集中
		2) 在收到最后一条合约信息和最后一条品种信息时，保存合约信息和品种信息
		3) 在收到经纪公司交易参数时，保存之
		4）在DataCenter析构时，保存费率
	2. 原来UserLoginCore的PreLoginQuery()中加载本地品种和合约信息删除，改为在StartLoginQuery()判断当前底层是否有品种和合约信息，没有则查询
	3. UserLoginCore中不再加载LoadCommissionRateAndMarginRate
	4. UserLoginDlg::QryInstrumentListCallBackFunc()中不再保存当日合约信息

2014.9.19
	1. 将插件接口PlusinAPI的Debug编译目标目录放到debug/Plugin下
	2. 修改缓存/加载当日合约信息时double处理的错误
	3. QryQueue查询费率的顺序调整：
		1) 对手续费和保证金都只查一次。查不到不管了
		2) 先用InstrumentID为空查手续费率、保证金率、期权手续费率、期权交易成本，以取到持仓合约的相关值
		3) 用m_bStartQryRate表示是否已经开始查询费率
		4) 刚开始查询费率时，将合约按顺序放到m_InstrumentIDsNeedQry队列中。顺序为：期权标的物合约(RunMode3)/期权合约(3)/期货合约(123)/期权合约(12)
		5) 如果某个合约查询无费率，将同品种的其它合约移到m_InstrumentIDsNeedQry2队列后面，最后再查

2014.9.17
	1. 将当前服务器信息tagServerParamCfg的current更名为CurSvrGrpID，表示当前的服务器组ID
	2. 登录成功后，检查tradingparam.xml中的tradingday，如果不是当前交易日，则删除配置文件
	3. 查询结算信息确认结果响应后，检查tradingparam.xml中是否缓存了BrokerTradingParams，如果没有，启动查询

2014.9.17
	1. 修改CPlatformDataMgr_LoginInfo，保存ctp的完整登录信息。增加PlatformStru_LoginInfo结构体
	2. 每天首次启动时查询交易参数ReqQryBrokerTradingParams。从中取到OptionRoyaltyPriceType，用于期权权利金计算。
	   查到后保存到tradingparam.xml中，当天以后直接从文件中读取。在查询结算信息确认相应后，将查询指令放到查询队列中去
	   仅在RunMode==2/3下查询
	3. 手续费率数据结构增加执行手续费，期权合约利用ReqQryOptionInstrCommRate得到手续费率, 扩展PlatformStru_InstrumentCommissionRate结果，增加执行手续费
	4. 用ReqQryOptionInstrTradeCost()获取投资者期权合约保证金不变部分和最小保证金，扩展PlatformStru_InstrumentMarginRate，增加期权交易成本信息
	5. 底层查询费率时，在QryQueue中QryID_ReqQryInstrumentCommissionRate时，期货查期货费率，期权查期权费率；在QryID_ReqQryInstrumentMarginRate时，期货查保证金率，期权查交易成本
	6. 修改第三方插件接口
        //CMDID_PRODUCT_LIST_QUERY                      : 查询指定交易所的品种列表。参数为ExchangeID；成功时返回n*32字符串数组。第一个字符串是ExchangeID，后面是结果(n-1)个ProductID
        //CMDID_OPTIONUNDERLYINGPRODUCT_LIST_QUERY      : 查询指定交易所的期权标的物品种列表。参数为ExchangeID；成功时返回n*32字符串数组。第一个字符串是ExchangeID，后面是结果(n-1)个ProductID
        //CMDID_OPTIONINSTRUMENTSBYUNDERLYING_LIST_QUERY: 查询指定标的物品合约对应的期权合约。参数为UnderlyingInstrumentID；成功时返回n*32字符串数组。第一个字符串是UnderlyingInstrumentID，后面是结果(n-1)个期权InstrumentID
        //CMDID_INSTRUMENT_LIST_BYPRODUCTID_QUERY       : 查询指定品种对应的合约。参数为ProductID；成功时返回n*32字符串数组。第一个字符串是ProductID，后面是结果(n-1)个期权InstrumentID

2014.9.15		
	1.  CPlatformDataMgr::m_baseFTID/m_baseUpdateSeq改用static变量
		1) 为了兼容多账号版本，以便于在多账号中使用统一的FTID和UpdateSeq, 避免由于不同账号的FTID相同导致UI更新异常
		2) 每个记录数据的FTID的不变的，因此多账号里使用同一个baseFTID不会引起long不够用的问题。
		3) UpdateSeq在多账号里可能出现long不够用的问题，如7fffffff递增后0。由于UpdateSeq只进行是否相等的判断，不进行大于小于的判断，因此折返为0的情况不影响系统运行
	2.  记录线程创建信息到threadid.log中
	3.  版本号更新到1.7.2.28
	4.  底层增加接口：
		//获取所有标的物期货合约
		virtual int GetAllOptionUnderlyingInstrumentIDs(vector<string>& outUnderlyingInstrumentIDs)=0;
		virtual int GetAllOptionUnderlyingInstrumentIDs(set<string>& outUnderlyingInstrumentIDs)=0;
		virtual bool IsCommissionRateExist(const string& InstrumentID)=0;
		virtual bool IsMarginRateExist(const string& InstrumentID)=0;
		
2014.9.12
	1. CQryQueue的静态变量m_bInQry改为CPlatFormService的类成员，便于在TradeSpi中可以访问。静态变量在多账号版本下，由于多个底层实例同时访问，会出现冲突而失去作用
	2. 优化CQryQueue中的SendQryCmd()，简化逻辑，提交效率。解决在多账号(10个账号，总共700多持仓明细)条件下卡死的问题
	3. CDataCenter()的析构里先调用Stop()，解决多账号下，未登录而直接退出导致异常的问题

2014.9.5
	1. 修改wxEVT_GRID_INSTRUMENTID_CHANGED相关处理，GetInt()为EvtParamID，GetExtraLong()为发送者GID
	2. 使T型报价窗口能处理报价表当前合约变更事件
	3. OptionTPriceWrapper在更改当前期权合约后，马上将合约的最后一条行情推送给TPriceList
	4. 修改FastTraderSetup-simu-option.iss
		1) [Files]删除旧的OptionPriceListT插件
		2）[Files]排除commonfiles路径下的layout子目录以及layout.xml 
		3) source\FastTrader-simu-option路径下加入专有layout.xml
	5. 发布1.7.2.27期权交易终端，用于测试T型报价模块

2014.9.4
	1. 修改保存/加载当天合约信息的模块，提高加载速度

2014.9.3
	1. 报价表改变当前合约后，向T型报价窗口发送消息
	2. 底层增加GetOptionInstrumentIDsByUnderlyingInstrumentID()，根据标的物期货合约，取对应的期权合约
	3. CPlatformDataMgr_Instruments::m_OptionProductIDsByExchangeID更名为m_OptionUnderlyingProductIDsByExchangeID
	4. 第三方插件接口增加CMDID_OPTIONINSTRUMENTSBYUNDERLYING_LIST_QUERY/RSP命令字
		#define CMDID_OPTIONINSTRUMENTSBYUNDERLYING_LIST_QUERY	0x3014	//查询标的物期货合约对应的期权合约列表。参数为Underlying InstrumentID
		#define CMDID_OPTIONINSTRUMENTSBYUNDERLYING_LIST_RSP	0x3015	//查询标的物期货合约对应的期权合约列表响应。成功时lpData指向字符串数组，每个字符串64字节，放一个Option InstrumentID; cbData等于品种数量*64
	5. 对于期权终端(RunMode==2/3)，因此下单板切换菜单，以及交易风格切换菜单，增加【深色背景/浅色背景】菜单
	6. 对于期权终端，在【帮助】-【关于】里显示【中国国际期货期权交易终端】

2014.8.29
	1. 增加期权T型报价模块
		1) 增加GID_OptionTPrice=8138，标志T型报价模块
		2) 将TPriceList项目加入到FastTrader的sln中
		3) 创建TPriceList的父窗口COptionTPriceWrapper，在里面创建TPriceList窗口

2014.8.21
	1. 原底层提供的GetProductID()接口更名为GetProductID_InstrumentIDsByExchangeID()
	2. 修改底层提供的GetProductList()接口，使用vector代替set
	3. 底层增加GetOptionProductIDsByExchangeID()接口，用于获取指定交易所的期权标的物期货品种
	4. 第三方插件接口增加CMDID_OPTIONUNDERLYINGPRODUCT_LIST_QUERY/RSP命令字
		#define CMDID_OPTIONUNDERLYINGPRODUCT_LIST_QUERY	0x3012	//查询特定交易所的期权标的物期货品种列表。参数为ExchangeID
		#define CMDID_OPTIONUNDERLYINGPRODUCT_LIST_RSP	    0x3013	//查询特定交易所的期权标的物期货品种列表响应。成功时lpData指向字符串数组，每个字符串64字节，放一个ProductID; cbData等于品种数量*64
	5. CPlatformDataMgr_Instruments类成员变量m_allProductID更名为m_allInstrumentIDsByProductID

2014.8.20
提交内容：
	1. ctp API：启用6.3.0API，支持期权(用于期权开发，暂不能发布标准版)
	2. platform.xml：增加RunMode节，配置运行模式
	3. 底层：增加品种信息管理
		启动时查询品种信息
		并在PlatformDataMgr_Instrumnets模块中管理品种信息
		提供GetProductList()、GetProductInfo()、GetProductInfo2()接口供UI层获取品种列表和品种信息
	4. 第三方插件接口：增加CMDID_PRODUCT_LIST_QUERY/RSP、CMDID_PRODUCT_INFO_QUERY/RSP命令字用于获取品种列表和品种信息
	5. 日内保存合约信息文件由Instruments.txt改为Instruments.xml
	6. 日内保存品种信息到Products.xml中
	7. 版本号改为1.7.2.26


2014.8.15
	1. platformSvr项目：在项目属性中增加宏定义CTP060300，表示是否支持ctp ver 6.3.0(option)。因为ctp api6.3.0for option里关于期权的结构定义在ctp6.2.5里没有。
	2. ctp api 6.3.0 for option 和6.2.5的区别
		1)ThostFtdcMdApi.h
		`	增加订阅/退订询价的函数：
			订阅询价：					virtual int SubscribeForQuoteRsp(char *ppInstrumentID[], int nCount) = 0;
			退订询价：					virtual int UnSubscribeForQuoteRsp(char *ppInstrumentID[], int nCount) = 0;
			订阅询价应答：				virtual void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
			退订询价应答：				virtual void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
			询价通知：					virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) {};

		2)ThostFtdcTraderApi.h
			执行宣告录入请求：			virtual int ReqExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, int nRequestID) = 0;
			执行宣告操作请求：			virtual int ReqExecOrderAction(CThostFtdcInputExecOrderActionField *pInputExecOrderAction, int nRequestID) = 0;
			询价录入请求：				virtual int ReqForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, int nRequestID) = 0;
			报价录入请求：				virtual int ReqQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, int nRequestID) = 0;
			报价操作请求：				virtual int ReqQuoteAction(CThostFtdcInputQuoteActionField *pInputQuoteAction, int nRequestID) = 0;

			请求查询期权交易成本：		virtual int ReqQryOptionInstrTradeCost(CThostFtdcQryOptionInstrTradeCostField *pQryOptionInstrTradeCost, int nRequestID) = 0;
			请求查询期权合约手续费：		virtual int ReqQryOptionInstrCommRate(CThostFtdcQryOptionInstrCommRateField *pQryOptionInstrCommRate, int nRequestID) = 0;
			请求查询执行宣告：			virtual int ReqQryExecOrder(CThostFtdcQryExecOrderField *pQryExecOrder, int nRequestID) = 0;
			请求查询询价：				virtual int ReqQryForQuote(CThostFtdcQryForQuoteField *pQryForQuote, int nRequestID) = 0;
			请求查询报价：				virtual int ReqQryQuote(CThostFtdcQryQuoteField *pQryQuote, int nRequestID) = 0;
		
			执行宣告录入请求响应：		virtual void OnRspExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
			执行宣告操作请求响应：		virtual void OnRspExecOrderAction(CThostFtdcInputExecOrderActionField *pInputExecOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
			询价录入请求响应：			virtual void OnRspForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
			报价录入请求响应：			virtual void OnRspQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
			报价操作请求响应：			virtual void OnRspQuoteAction(CThostFtdcInputQuoteActionField *pInputQuoteAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
		
			请求查询期权交易成本响应：	virtual void OnRspQryOptionInstrTradeCost(CThostFtdcOptionInstrTradeCostField *pOptionInstrTradeCost, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
			请求查询期权合约手续费响应：	virtual void OnRspQryOptionInstrCommRate(CThostFtdcOptionInstrCommRateField *pOptionInstrCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
			请求查询执行宣告响应：		virtual void OnRspQryExecOrder(CThostFtdcExecOrderField *pExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
			请求查询询价响应：			virtual void OnRspQryForQuote(CThostFtdcForQuoteField *pForQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
			请求查询报价响应：			virtual void OnRspQryQuote(CThostFtdcQuoteField *pQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
		
			执行宣告通知：				virtual void OnRtnExecOrder(CThostFtdcExecOrderField *pExecOrder) {};
			执行宣告录入错误回报：		virtual void OnErrRtnExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo) {};
			执行宣告操作错误回报：		virtual void OnErrRtnExecOrderAction(CThostFtdcExecOrderActionField *pExecOrderAction, CThostFtdcRspInfoField *pRspInfo) {};
			询价录入错误回报：			virtual void OnErrRtnForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo) {};
			报价通知：					virtual void OnRtnQuote(CThostFtdcQuoteField *pQuote) {};
			报价录入错误回报：			virtual void OnErrRtnQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo) {};
			报价操作错误回报：			virtual void OnErrRtnQuoteAction(CThostFtdcQuoteActionField *pQuoteAction, CThostFtdcRspInfoField *pRspInfo) {};
			询价通知：					virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) {};

		3)ThostFtdcUserApiDataType.h
			TFtdcProductClassType(产品类型类型)：						#define THOST_FTDC_PC_SpotOption '6'				//现货期权
			TFtdcUnderlyingMultipleType(基础商品乘数类型)：				typedef double TThostFtdcUnderlyingMultipleType;	
			TFtdcExchangeSettlementParamIDType(交易所结算参数代码类型)：	#define THOST_FTDC_ESPI_OptOutDisCountRate 'a'		///虚值期权保证金优惠比率
																		#define THOST_FTDC_ESPI_OptMiniGuarantee 'b'		///最低保障系数
			TFtdcFileIDType(文件标识类型)：								#define THOST_FTDC_FI_OptionStrike 'S'				///期权执行文件
																		#define THOST_FTDC_FI_SettlementPriceComparison 'M'	///结算价比对文件
																		#define THOST_FTDC_FI_NonTradePosChange 'B'			///上期所非持仓变动明细
			TFtdcCSRCOptionsTypeType(期权类型类型)：						typedef char TThostFtdcCSRCOptionsTypeType[2];
			TFtdcCSRCStrikePriceType(执行价类型)：						typedef double TThostFtdcCSRCStrikePriceType;
			TFtdcCSRCTargetProductIDType(标的品种类型)：					typedef char TThostFtdcCSRCTargetProductIDType[3];
			TFtdcCSRCTargetInstrIDType(标的合约类型)：					typedef char TThostFtdcCSRCTargetInstrIDType[31];
					
			删除TFtdcCombineTypeType(组合类型类型)

			TFtdcCusAccountTypeType(结算账户类型类型)：			删除	THOST_FTDC_CAT_Assetmgr							///资管结算账户
																增加	#define THOST_FTDC_CAT_AssetmgrFuture '2'		///纯期货资管业务下的资管结算账户
																		#define THOST_FTDC_CAT_AssetmgrTrustee '3'		///综合类资管业务下的期货资管托管账户
																		#define THOST_FTDC_CAT_AssetmgrTransfer '4'		///综合类资管业务下的资金中转账户

			TFtdcDirectionEnType(买卖方向类型)：							typedef char TThostFtdcDirectionEnType;
			TFtdcOffsetFlagEnType(开平标志类型)：						typedef char TThostFtdcOffsetFlagEnType;
			TFtdcHedgeFlagEnType(投机套保标志类型)：						typedef char TThostFtdcHedgeFlagEnType;
			TFtdcFundIOTypeEnType(出入金类型类型)：						typedef char TThostFtdcFundIOTypeEnType;
			TFtdcFundTypeEnType(资金类型类型)：							typedef char TThostFtdcFundTypeEnType;
			TFtdcFundDirectionEnType(出入金方向类型)：					typedef char TThostFtdcFundDirectionEnType;
			TFtdcFundMortDirectionEnType(货币质押方向类型)：				typedef char TThostFtdcFundMortDirectionEnType;
			TFtdcSwapBusinessTypeType(换汇业务种类类型)：				typedef char TThostFtdcSwapBusinessTypeType[3];
			TFtdcOptionsTypeType(期权类型类型)：							typedef char TThostFtdcOptionsTypeType;
			TFtdcStrikeModeType(执行方式类型)：							typedef char TThostFtdcStrikeModeType;
			TFtdcStrikeTypeType(执行类型类型)：							typedef char TThostFtdcStrikeTypeType;
			TFtdcApplyTypeType(中金所期权放弃执行申请类型类型)：			typedef char TThostFtdcApplyTypeType;
			TFtdcGiveUpDataSourceType(放弃执行申请数据来源类型)：			typedef char TThostFtdcGiveUpDataSourceType;
			TFtdcExecOrderSysIDType(执行宣告系统编号类型)：				typedef char TThostFtdcExecOrderSysIDType[21];
			TFtdcExecResultType(执行结果类型)：							typedef char TThostFtdcExecResultType;
			TFtdcStrikeSequenceType(执行序号类型)：						typedef int TThostFtdcStrikeSequenceType;
			TFtdcStrikeTimeType(执行时间类型)：							typedef char TThostFtdcStrikeTimeType[13];
			TFtdcCombinationTypeType(组合类型类型)：						typedef char TThostFtdcCombinationTypeType;
			TFtdcOptionRoyaltyPriceTypeType(期权权利金价格类型类型)：		typedef char TThostFtdcOptionRoyaltyPriceTypeType;
			TFtdcBalanceAlgorithmType(权益算法类型)：					typedef char TThostFtdcBalanceAlgorithmType;
			TFtdcActionTypeType(执行类型类型)：							typedef char TThostFtdcActionTypeType;
			TFtdcForQuoteStatusType(询价状态类型)：						typedef char TThostFtdcForQuoteStatusType;
			TFtdcQuotStatusType(报价状态类型)：							typedef char TThostFtdcQuotStatusType;
			TFtdcValueMethodType(取值方式类型)：							typedef char TThostFtdcValueMethodType;
			TFtdcExecOrderPositionFlagType(期权行权后是否保留期货头寸的标记类型)：	typedef char TThostFtdcExecOrderPositionFlagType;
			TFtdcExecOrderCloseFlagType(期权行权后生成的头寸是否自动平仓类型)：	typedef char TThostFtdcExecOrderCloseFlagType;
			TFtdcProductTypeType(产品类型类型)：							typedef char TThostFtdcProductTypeType;
			TFtdcCZCEUploadFileNameType(郑商所结算文件名类型)：			typedef char TThostFtdcCZCEUploadFileNameType;
			TFtdcDCEUploadFileNameType(大商所结算文件名类型)：			typedef char TThostFtdcDCEUploadFileNameType;
			TFtdcSHFEUploadFileNameType(上期所结算文件名类型)：			typedef char TThostFtdcSHFEUploadFileNameType;
			TFtdcCFFEXUploadFileNameType(中金所结算文件名类型)：			typedef char TThostFtdcCFFEXUploadFileNameType;

			
		4)ThostFtdcUserApiStruct.h
			struct CThostFtdcProductField					(产品)：					增加	TThostFtdcInstrumentIDType	ExchangeProductID;			///交易所产品代码
																							TThostFtdcUnderlyingMultipleType	UnderlyingMultiple;	///合约基础商品乘数
			struct CThostFtdcInstrumentField				(合约)：					增加	TThostFtdcInstrumentIDType	UnderlyingInstrID;			///基础商品代码
																							TThostFtdcPriceType	StrikePrice;						///执行价
																							TThostFtdcOptionsTypeType	OptionsType;				///期权类型
																							TThostFtdcUnderlyingMultipleType	UnderlyingMultiple;	///合约基础商品乘数
																							TThostFtdcCombinationTypeType	CombinationType;		///组合类型
			struct CThostFtdcTradingAccountField			(资金账户)：				增加	TThostFtdcMoneyType	OptionCloseProfit;					///期权平仓盈亏
																							TThostFtdcMoneyType	OptionValue;						///期权市值
			struct CThostFtdcInvestorPositionField			(投资者持仓)：			增加	TThostFtdcVolumeType	StrikeFrozen;					///执行冻结
																							TThostFtdcMoneyType	StrikeFrozenAmount;					///执行冻结金额
																							TThostFtdcVolumeType	AbandonFrozen;					///放弃执行冻结
																							TThostFtdcMoneyType	OptionValue;						///期权市值
			struct CThostFtdcSyncingTradingAccountField		(正在同步中的交易账号)：	增加	TThostFtdcMoneyType	OptionCloseProfit;					///期权平仓盈亏
																							TThostFtdcMoneyType	OptionValue;						///期权市值
			struct CThostFtdcSyncingInvestorPositionField	(正在同步中的投资者持仓)：增加	TThostFtdcVolumeType StrikeFrozen;						///执行冻结
																							TThostFtdcMoneyType	StrikeFrozenAmount;					///执行冻结金额
																							TThostFtdcVolumeType AbandonFrozen;						///放弃执行冻结
																							TThostFtdcMoneyType	OptionValue;						///期权市值
			struct CThostFtdcQryProductField				(查询产品)：				增加	TThostFtdcProductClassType ProductClass;				///产品类型
			struct CThostFtdcBrokerWithdrawAlgorithmField	(经纪公司可提资金算法表)：增加	TThostFtdcBalanceAlgorithmType BalanceAlgorithm;		///权益算法
			struct CThostFtdcBrokerTradingParamsField		(经纪公司交易参数)：		增加	TThostFtdcOptionRoyaltyPriceTypeType OptionRoyaltyPriceType;///期权权利金价格类型
			
			struct CThostFtdcOptionInstrMiniMarginField		(当前期权合约最小保证金)：		新增结构
			struct CThostFtdcOptionInstrMarginAdjustField	(当前期权合约保证金调整系数)：	新增结构
			struct CThostFtdcOptionInstrCommRateField		(当前期权合约手续费的详细内容)：	新增结构
			struct CThostFtdcOptionInstrTradeCostField		(期权交易成本)：					新增结构
			struct CThostFtdcQryOptionInstrTradeCostField	(期权交易成本查询)：				新增结构
			struct CThostFtdcQryOptionInstrCommRateField	(期权手续费率查询)：				新增结构
			struct CThostFtdcIndexPriceField				(股指现货指数)：					新增结构
			struct CThostFtdcInputExecOrderField			(输入的执行宣告)：				新增结构
			struct CThostFtdcInputExecOrderActionField		(输入执行宣告操作)：				新增结构
			struct CThostFtdcExecOrderField					(执行宣告)：						新增结构
			struct CThostFtdcExecOrderActionField			(执行宣告操作)：					新增结构
			struct CThostFtdcQryExecOrderField				(执行宣告查询)：					新增结构
			struct CThostFtdcExchangeExecOrderField			(交易所执行宣告信息)：			新增结构
			struct CThostFtdcQryExchangeExecOrderField		(交易所执行宣告查询)：			新增结构
			struct CThostFtdcQryExecOrderActionField		(执行宣告操作查询)：				新增结构
			struct CThostFtdcExchangeExecOrderActionField	(交易所执行宣告操作)：			新增结构
			struct CThostFtdcQryExchangeExecOrderActionField(交易所执行宣告操作查询)：		新增结构
			struct CThostFtdcErrExecOrderField				(错误执行宣告)：					新增结构
			struct CThostFtdcQryErrExecOrderField			(查询错误执行宣告)：				新增结构
			struct CThostFtdcErrExecOrderActionField		(错误执行宣告操作)：				新增结构
			struct CThostFtdcQryErrExecOrderActionField		(查询错误执行宣告操作)：			新增结构
			struct CThostFtdcOptionInstrTradingRightField	(投资者期权合约交易权限)：		新增结构
			struct CThostFtdcQryOptionInstrTradingRightField(查询期权合约交易权限)：			新增结构
			struct CThostFtdcInputForQuoteField				(输入的询价)：					新增结构
			struct CThostFtdcForQuoteField					(询价)：							新增结构
			struct CThostFtdcQryForQuoteField				(询价查询)：						新增结构
			struct CThostFtdcExchangeForQuoteField			(交易所询价信息)：				新增结构
			struct CThostFtdcQryExchangeForQuoteField		(交易所询价查询)：				新增结构
			struct CThostFtdcInputQuoteField				(输入的报价)：					新增结构
			struct CThostFtdcInputQuoteActionField			(输入报价操作)：					新增结构
			struct CThostFtdcQuoteField						(报价)：							新增结构
			struct CThostFtdcQuoteActionField				(报价操作)：						新增结构
			struct CThostFtdcQryQuoteField					(报价查询)：						新增结构
			struct CThostFtdcExchangeQuoteField				(交易所报价信息)：				新增结构
			struct CThostFtdcQryExchangeQuoteField			(交易所报价查询)：				新增结构
			struct CThostFtdcQryQuoteActionField			(报价操作查询)：					新增结构
			struct CThostFtdcExchangeQuoteActionField		(交易所报价操作)：				新增结构
			struct CThostFtdcQryExchangeQuoteActionField	(交易所报价操作查询)：			新增结构
			struct CThostFtdcOptionInstrDeltaField			(期权合约delta值)：				新增结构
			struct CThostFtdcForQuoteRspField				(发给做市商的询价请求)：			新增结构
			struct CThostFtdcStrikeOffsetField				(当前期权合约执行偏移值的详细内容)：新增结构
			struct CThostFtdcQryStrikeOffsetField			(期权执行偏移值查询)：			新增结构














