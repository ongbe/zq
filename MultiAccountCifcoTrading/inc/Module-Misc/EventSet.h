#pragma once

BEGIN_DECLARE_EVENT_TYPES()	
	DECLARE_EVENT_TYPE(wxEVT_WRITE_USER_LOG, 1000)
	DECLARE_EVENT_TYPE(wxEVT_CONFIG_CHANGED, 1001)
	DECLARE_EVENT_TYPE(wxEVT_UPDATE_LIST_TABLE, 1002)
	DECLARE_EVENT_TYPE(wxEVT_SHOW_PANEL, 1003)
	DECLARE_EVENT_TYPE(wxEVT_LIST_CHAR_HOOK, 1004)
	DECLARE_EVENT_TYPE(wxEVT_STYLE_CHANGED, 1005)
	DECLARE_EVENT_TYPE(wxEVT_GET_STYLE, 1006)
	DECLARE_EVENT_TYPE(wxEVT_GROUP_CHANGE, 1007)
    DECLARE_EVENT_TYPE(wxEVT_QUOT_VIEW_CHANGE, 2647)
    DECLARE_EVENT_TYPE(wxEVT_SOAP_MSG, 2647)
    DECLARE_EVENT_TYPE(wxEVT_CFG_CHANGE, 2650)
	DECLARE_EVENT_TYPE(wxEVT_BUYSELL5_SHOW_CHANGE, 2651)
    DECLARE_EVENT_TYPE(wxEVT_PANEL_FOCUS,2652)
    DECLARE_EVENT_TYPE(wxEVT_CHAR_EX,2653)
    DECLARE_EVENT_TYPE(wxEVT_SUBSCRIBE, 2654)
	DECLARE_EVENT_TYPE(wxEVT_COMMAND_READY, 2655)
	DECLARE_EVENT_TYPE(wxEVT_ALL_ORDER_LIST_UPDATE,2656)
	DECLARE_EVENT_TYPE(wxEVT_ALL_TRADE_LIST_UPDATE,2657)
    DECLARE_EVENT_TYPE(wxEVT_PANEL_CHAR_HOOK,2658)
    DECLARE_EVENT_TYPE(wxEVT_FRONT_DISCONNECTED,2659)
    DECLARE_EVENT_TYPE(wxEVT_TRADING_NOTICE,2660)
    DECLARE_EVENT_TYPE(wxEVT_INSTRUMENT_STATUS,2661)
    DECLARE_EVENT_TYPE(wxEVT_STATIC_LEFT_UP, 2662)
	DECLARE_EVENT_TYPE(wxEVT_POSITION_LIST_UPDATE, 2663)
	DECLARE_EVENT_TYPE(wxEVT_POSITION_LIST_TRADE_UPDATE, 2664)
	DECLARE_EVENT_TYPE(wxEVT_POSITION_DETAIL_LIST_UPDATE,2665)
	DECLARE_EVENT_TYPE(wxEVT_POSITION_DETAIL_LIST_TRADE_UPDATE,2666)
	DECLARE_EVENT_TYPE(wxEVT_QUOT_PANEL_NEW_QUOT,2667)
	DECLARE_EVENT_TYPE(wxEVT_FIREREQQRY,2668)
	DECLARE_EVENT_TYPE(wxEVT_RtnTradeRecord,2669)
	DECLARE_EVENT_TYPE(wxEVT_RtnTradeHistory,2670)
	DECLARE_EVENT_TYPE(wxEVT_ORDER_INSERT_PANEL_INSTRUMENT_ID, 2671)
	DECLARE_EVENT_TYPE(wxEVT_ORDER_DELETE, 2672)
	DECLARE_EVENT_TYPE(wxEVT_CODECLICKED, 2673)
	DECLARE_EVENT_TYPE(wxEVT_PRICECLICKED, 2674)
	DECLARE_EVENT_TYPE(wxEVT_VOLUMECLICKED, 2675)
	DECLARE_EVENT_TYPE(wxEVT_ORDER_INSERT_PANEL_NEW_ORDER,2676)
	DECLARE_EVENT_TYPE(wxEVT_ORDER_DELETE_RESULT,2677);
	DECLARE_EVENT_TYPE(wxEVT_JSDPANEL_INSTRUMENTID_CHANGED,2678)
	DECLARE_EVENT_TYPE(wxEVT_JSDPANEL_AUTOOPENCLOSE_CHANGED,2679)
	DECLARE_EVENT_TYPE(wxEVT_JSDPANEL_AUTOTRACKPRICE_CHANGED,2680)
	DECLARE_EVENT_TYPE(wxEVT_KQPANEL_INSTRUMENTID_CHANGED,2681)
	DECLARE_EVENT_TYPE(wxEVT_KQPANEL_AUTOOPENCLOSE_CHANGED,2682)
	DECLARE_EVENT_TYPE(wxEVT_KQPANEL_AUTOTRACKPRICE_CHANGED,2683)
	DECLARE_EVENT_TYPE(wxEVT_STDPANEL_INSTRUMENTID_CHANGED,2684)
	DECLARE_EVENT_TYPE(wxEVT_STDPANEL_AUTOOPENCLOSE_CHANGED,2685)
	DECLARE_EVENT_TYPE(wxEVT_STDPANEL_AUTOTRACKPRICE_CHANGED,2686)
	DECLARE_EVENT_TYPE(wxEVT_OPEN_ORDER_RSP_ORDER,2687)
	DECLARE_EVENT_TYPE(wxEVT_OPEN_ORDER_RSP_TRADE,2688)
	DECLARE_EVENT_TYPE(wxEVT_TRANSFER_RSP,2689)
	DECLARE_EVENT_TYPE(wxEVT_HisCal,2690)
	DECLARE_EVENT_TYPE(wxEVT_TradePwdChange,2691)
	DECLARE_EVENT_TYPE(wxEVT_FundPwdChange,2692)
	DECLARE_EVENT_TYPE(wxEVT_ORDERSERVICE_THREAD_NEW_QUOT,2693)
	DECLARE_EVENT_TYPE(wxEVT_ORDERSERVICE_THREAD_NEW_ORDER,2694)
	DECLARE_EVENT_TYPE(wxEVT_ORDERSERVICE_THREAD_NEW_TRADE,2695)
	DECLARE_EVENT_TYPE(wxEVT_FIVEPRICE_PRICESELECTED,2696)
	DECLARE_EVENT_TYPE(wxEVT_TEXT_FOCUS,2697)
	DECLARE_EVENT_TYPE(wxEVT_MOUSEINPUT_CODE,2698)
	DECLARE_EVENT_TYPE(wxEVT_MOUSEINPUT_DIGIT,2699)
	DECLARE_EVENT_TYPE(wxEVT_MOUSEINPUT_VOLUME,2700)

	// 下单板有关的公共指令事件
	DECLARE_EVENT_TYPE(wxEVT_GRID_INSTRUMENTID_CHANGED, 2701)			// 表格上的合约代码改变事件
	DECLARE_EVENT_TYPE(wxEVT_ORDERINSERT_VIEWMODE_CHANGED, 2702)		// 下单板风格变化的事件
	DECLARE_EVENT_TYPE(wxEVT_ORDERINSERT_KEYORDER, 2703)				// 键盘快速下单的事件
	DECLARE_EVENT_TYPE(wxEVT_ORDERINSERT_MOUSEORDER, 2704)				// 鼠标快速下单的事件
	DECLARE_EVENT_TYPE(wxEVT_ORDERINSERT_MOUSECLOSE, 2705)				// 鼠标快速平仓的事件
	DECLARE_EVENT_TYPE(wxEVT_ORDERINSERT_ORDERCHANGE, 2706)				// 市价反手、快速改单的事件
	DECLARE_EVENT_TYPE(wxEVT_ORDERINSERT_INSTRUMENTID_CHANGED, 2707)	// 下单板合约改变的事件，向下单板外的地方发送
    //合约列表有关的事件 added by l.
	DECLARE_EVENT_TYPE(wxEVT_INSTRUMENT_LIST_COMMISSIONRATE_UPDATE,2801)
	DECLARE_EVENT_TYPE(wxEVT_INSTRUMENT_LIST_MARGINRATE_UPDATE,2802)
	//报价表事件
//	DECLARE_EVENT_TYPE(wxEVT_QUOT_GET_INSTRUMENTID, 2708)//响应外部指令：获得当前选中合约
	DECLARE_EVENT_TYPE(wxEVT_QUOT_SET_GROUP, 2709)//响应外部指令：选择指定合约组

	//主窗体响应
	DECLARE_EVENT_TYPE(wxEVT_UPDATE_CONFIG_PANE, 2710)					//响应子窗口获取配置面板是否显示的请求
	DECLARE_EVENT_TYPE(wxEVT_LOAD_CONFIG_APP,2711)						//响应子窗口启动ConfigApp的请求
//	DECLARE_EVENT_TYPE(wxEVT_GET_PANE_CAPTION,2712)						//响应子窗口获取面板显示名称的请求
//	DECLARE_EVENT_TYPE(wxEVT_GET_ALL_COMMISSIONRATE,2713)				//响应子窗口获取当前全部手续费和的请求
    DECLARE_EVENT_TYPE(wxEVT_SUCESS_TRANSFER,2714)						//银期模块转账成功后发送消息给主窗体，主窗体通知资金模块
//	DECLARE_EVENT_TYPE(wxEVT_GET_ALL_COMMISSIONRATE,2715)				//响应子窗口获取当前全部手续费和的请求
	DECLARE_EVENT_TYPE(wxEVT_REMOVE,2716)								//报单板块撤单事件
	DECLARE_EVENT_TYPE(wxEVT_REMOVEALL,2717)							//报单板块全撤事件
//	DECLARE_EVENT_TYPE(wxEVT_QUOT_GET_GROUP, 2718)						//响应外部指令：选择指定合约组

	// 同步查询合约信息
	DECLARE_EVENT_TYPE(wxEVT_INSTRUMENT_ASYNCGET, 2719)					// 自动申请合约保证金率,合约手续费率
	// 快期下单板与下单模块
	DECLARE_EVENT_TYPE(wxEVT_KQPANEL_BUYSELLSEL_CHANGED, 2720)			// 自动申请合约保证金率,合约手续费率

	DECLARE_EVENT_TYPE(wxEVT_QUERY_PARKED_ORDERINSERT, 2721)			// 
	DECLARE_EVENT_TYPE(wxEVT_QUERY_PARKED_ORDERACTION, 2722)			// 
	DECLARE_EVENT_TYPE(wxEVT_QUERY_PARKED_REMOVEPARKEDORDER, 2723)		// 
	DECLARE_EVENT_TYPE(wxEVT_QUERY_PARKED_REMOVEPARKEDORDERACTION, 2724)// 
	DECLARE_EVENT_TYPE(wxEVT_QUERY_PARKED_QUERYPARKED, 2725)			// 
	DECLARE_EVENT_TYPE(wxEVT_QUERY_PARKED_QUERYPARKEDACTION, 2726)		// 

	DECLARE_EVENT_TYPE(wxEVT_PLATFORMSVR_STATUSCHANGE, 2727)			// 综合交易平台交易状态改变事件

	DECLARE_EVENT_TYPE(wxEVT_QUERY_BATCH_REQUERY, 2728)					// 重新查询所有的数据
	DECLARE_EVENT_TYPE(wxEVT_QUERY_PARKED_REQUERY, 2729)				// 重新查询所有的数据
	DECLARE_EVENT_TYPE(wxEVT_QUERY_CONDITION_REQUERY, 2730)				// 重新查询所有的数据
	DECLARE_EVENT_TYPE(wxEVT_QUERY_STOP_REQUERY, 2731)					// 重新查询所有的数据
    DECLARE_EVENT_TYPE(wxEVT_CONNECT_LOGIN,2732)

    DECLARE_EVENT_TYPE(wxEVT_MOVEPOSITION_TIMER, 2733)					// 移仓的Timer事件
    DECLARE_EVENT_TYPE(wxEVT_MOVEPOSITION_ORDERLOG, 2734)				// 移仓的报单回报Log事件
    DECLARE_EVENT_TYPE(wxEVT_MOVEPOSITION_TRADELOG, 2735)				// 移仓的成交Log事件
    DECLARE_EVENT_TYPE(wxEVT_MOVEPOSITION_ACTION, 2736)					// 移仓的自动执行动作Log事件
    DECLARE_EVENT_TYPE(wxEVT_SWAPPOSITION_TIMER, 2737)					// 仓位互换的Timer事件
    DECLARE_EVENT_TYPE(wxEVT_SWAPPOSITION_ORDERLOG, 2738)				// 仓位互换的报单回报Log事件
    DECLARE_EVENT_TYPE(wxEVT_SWAPPOSITION_TRADELOG, 2739)				// 仓位互换的成交Log事件
    DECLARE_EVENT_TYPE(wxEVT_SWAPPOSITION_ACTION, 2740)					// 仓位互换的自动执行动作Log事件
    DECLARE_EVENT_TYPE(wxEVT_ORDER_LOGON, 2741)
    DECLARE_EVENT_TYPE(wxEVT_POS_MOVE_SWAP_UPDATE, 2742)
    DECLARE_EVENT_TYPE(wxEVT_DIALOG_SHOW, 2743)
    DECLARE_EVENT_TYPE(wxEVT_LANGUAGE_CHANGE,2744)
	DECLARE_EVENT_TYPE(wxEVT_CLOSE_PLUSIN,2745)
	DECLARE_EVENT_TYPE(wxEVT_OPEN_PLUSIN,2746)
	DECLARE_EVENT_TYPE(wxEVT_LOAD_PLUSIN,2747)
	DECLARE_EVENT_TYPE(wxEVT_COMMISSIONRATE,2748)
	DECLARE_EVENT_TYPE(wxEVT_CONNECTIVITY_STATUS, 2749)
	DECLARE_EVENT_TYPE(wxEVT_ORDERINSERT_ORDERREPLACE, 2750)				// 改单
	DECLARE_EVENT_TYPE(wxEVT_PLUGIN_HIDE, 2751)
	DECLARE_EVENT_TYPE(wxEVT_PLUGIN_SHOW, 2752)
	DECLARE_EVENT_TYPE(wxEVT_STOP_PLATFORM, 2753)
	DECLARE_EVENT_TYPE(wxEVT_PATSPANEL_ORDERTYPE_CHANGED, 2754)			// 下单类型改变
	DECLARE_EVENT_TYPE(wxEVT_CALC_FUNDACCOUNT, 2755)
	DECLARE_EVENT_TYPE(wxEVT_EXCHANGERATE, 2756)
	
	DECLARE_EVENT_TYPE(wxEVT_LOCALPARKED_STATUSCHANGED, 2757)
	DECLARE_EVENT_TYPE(wxEVT_LOCALCONDITION_STATUSCHANGED, 2758)
	DECLARE_EVENT_TYPE(wxEVT_LOCALPOSITION_STATUSCHANGED, 2759)
	DECLARE_EVENT_TYPE(wxEVT_DULPLICATE_KEY_PROMPT,2760)
	DECLARE_EVENT_TYPE(wxEVT_RECV_MSG,2761)
	
    DECLARE_EVENT_TYPE(wxEVT_ALL_ORDER_LIST_UPDATE_ByRate, 2801)
    DECLARE_EVENT_TYPE(wxEVT_USER_NOTICE,2810)

    DECLARE_EVENT_TYPE(wxEVT_RspQryTradeLast,3000)
    DECLARE_EVENT_TYPE(wxEVT_RspQryOrderLast,3001)
    DECLARE_EVENT_TYPE(wxEVT_RspQryPositionLast,3002)
    DECLARE_EVENT_TYPE(wxEVT_RspQryPositionDetailLast,3003)
    DECLARE_EVENT_TYPE(wxEVT_RspQryPositionCombLast,3004)

    DECLARE_EVENT_TYPE(wxEVT_RspQryCommissionRate, 3010)
    DECLARE_EVENT_TYPE(wxEVT_RspQryMarginRate, 3011)

    DECLARE_EVENT_TYPE(wxEVT_RtnOrder, 3020)
    DECLARE_EVENT_TYPE(wxEVT_RtnTrade, 3021)
    DECLARE_EVENT_TYPE(wxEVT_RtnDepthMarketData, 3022)

	DECLARE_EVENT_TYPE(wxEVT_ALL_ORDER_RspOrderAction1,3030)
	DECLARE_EVENT_TYPE(wxEVT_ALL_ORDER_RspOrderAction2,3031)

	DECLARE_EVENT_TYPE(wxEVT_MAINFRAME_QUOT_SETFOCUS,3032)
    DECLARE_EVENT_TYPE(wxEVT_MAINFRAME_QUOT_SELGROUP,3033)
    DECLARE_EVENT_TYPE(wxEVT_SEND_IE_CONTENT,3034)
    DECLARE_EVENT_TYPE(wxEVT_RTN_RECONNECTED,3035)
    DECLARE_EVENT_TYPE(wxEVT_REQ_REQRY,3036)

	DECLARE_EVENT_TYPE(wxEVT_PLUSIN_MENU_CHANGE,3050)
	DECLARE_EVENT_TYPE(wxEVT_MA_SOCKET,3051)
	DECLARE_EVENT_TYPE(wxEVT_MA_PLAT_MODIFY,3052)
	DECLARE_EVENT_TYPE(wxEVT_MA_PLAT_ADD,3053)
	DECLARE_EVENT_TYPE(wxEVT_MA_FUND_UPDATE,3054)
	DECLARE_EVENT_TYPE(wxEVT_MA_UI_CALLBACK,3055)
	DECLARE_EVENT_TYPE(wxEVT_MA_LIST_UPDATE,3056)
	DECLARE_EVENT_TYPE(wxEVT_MA_USER_CHANGE,3057)
	DECLARE_EVENT_TYPE(wxEVT_MA_ORDERINSERT_ORDERCHANGE,3058)
	DECLARE_EVENT_TYPE(wxEVT_MA_ORDERINSERT_MOUSECLOSE,3059)
	DECLARE_EVENT_TYPE(wxEVT_QRY_ACCOUNT_DLG,3060)
	DECLARE_EVENT_TYPE(wxEVT_MA_PLAT_DELETE,3053)
END_DECLARE_EVENT_TYPES()

#define EVT_LIST_CHAR_HOOK(fn) \
	DECLARE_EVENT_TABLE_ENTRY( \
	wxEVT_LIST_CHAR_HOOK, wxID_ANY, wxID_ANY, \
	(wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent( wxCommandEventFunction, &fn ), \
	(wxObject *) NULL \
	),
#define EVT_INITQRY_STEP(fn) \
    DECLARE_EVENT_TABLE_ENTRY( \
        wxEVT_INITQRY, wxID_ANY, wxID_ANY, \
        (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent( wxCommandEventFunction, &fn ), \
        (wxObject *) NULL \
    ),
#define EVT_CFG_CHANGE(fn) \
    DECLARE_EVENT_TABLE_ENTRY( \
        wxEVT_CFG_CHANGE, wxID_ANY, wxID_ANY, \
        (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent( wxCommandEventFunction, &fn ), \
        (wxObject *) NULL \
    ),
#define EVT_STYLE_CHANGE(fn) \
	DECLARE_EVENT_TABLE_ENTRY( \
	wxEVT_STYLE_CHANGED, wxID_ANY, wxID_ANY, \
	(wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent( wxCommandEventFunction, &fn ), \
	(wxObject *) NULL \
	),
#define EVT_PANEL_FOCUS(fn) \
    DECLARE_EVENT_TABLE_ENTRY( \
        wxEVT_PANEL_FOCUS, wxID_ANY, wxID_ANY, \
        (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent( wxCommandEventFunction, &fn ), \
        (wxObject *) NULL \
    ),
#define EVT_CHAR_EX(func)  wx__DECLARE_EVT0(wxEVT_CHAR_EX, wxCharEventHandler(func))
#define EVT_SUBSCRIBE(fn) \
    DECLARE_EVENT_TABLE_ENTRY( \
        wxEVT_SUBSCRIBE, wxID_ANY, wxID_ANY, \
        (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent( wxCommandEventFunction, &fn ), \
        (wxObject *) NULL \
    ),
#define EVT_PANEL_CHAR_HOOK(fn) \
    DECLARE_EVENT_TABLE_ENTRY( \
        wxEVT_PANEL_CHAR_HOOK, wxID_ANY, wxID_ANY, \
        (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent( wxCommandEventFunction, &fn ), \
        (wxObject *) NULL \
    ),
#define EVT_STATIC_LEFT_UP(fn) \
    DECLARE_EVENT_TABLE_ENTRY( \
        wxEVT_STATIC_LEFT_UP, wxID_ANY, wxID_ANY, \
        (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent( wxCommandEventFunction, &fn ), \
        (wxObject *) NULL \
    ),