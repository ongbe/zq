#ifndef _CONFIGERMANAGER_H_
#define _CONFIGERMANAGER_H_

class GlobalConfigManager
{
public:
	static void Load();

    static std::string GetstrValueByTitle(void* root,const char* DefValue,
                                            const char* pTitle1,
                                            const char* pTitle2=NULL,
                                            const char* pTitle3=NULL,
                                            const char* pTitle4=NULL);
    static int GetnValueByTitle(void* proot,const int DefValue,
                                            const char* pTitle1,
                                            const char* pTitle2=NULL,
                                            const char* pTitle3=NULL,
                                            const char* pTitle4=NULL);


	static bool IsCalculate_OnRtnTrade();

    //获取成交回报后是否触发的参数
    static bool IsReq_OnRtnTrade(void);

    //获取成交回报后触发查询的延迟秒数
    static int GetDelayBefReq_OnRtnTrade(void);

    //获取成交触发查询持仓明细的最大记录数量阀值，超过这个值就不自动查询持仓明细
    static int GetMaxRecordReqPositionDetail_OnRtnTrade(void);

    //列表框自动排序时，最大行数。超过这个行数，停止自动排序，以免影响速度
    static int GetMaxItemCountForAutoResort_ListCtrl(void);

    //是否启动CifcoServicesHall功能
	static bool IsCifcoServicesHall_ExtraFunction();

    //多账号是否乱序下单
    static bool IsDisorderWhenInsertOrder_MultiAccount();


private:
    //是否已经加载
    static bool m_bLoad;

    //成交回报时是否计算并更新持仓、持仓明细和资金
	static bool m_bCalculate_OnRtnTrade;

    //成交回报后是否触发持仓、持仓明细和资金的查询
    static bool m_bReq_OnRtnTrade;

    //成交回报后延迟查询持仓、持仓明细和资金的时间。单位为秒
    static int m_nDelayBefReq_OnRtnTrade;

    //成交触发查询持仓明细的最大记录数量阀值，超过这个值就不自动查询持仓明细
    static int m_nMaxRecordReqPositionDetail_OnRtnTrade;


    //列表框自动排序时，最大行数。超过这个行数，停止自动排序，以免影响速度
    static int m_nMaxItemCountForAutoResort_ListCtrl;

    //是否启动CifcoServicesHall功能
    static bool m_bCifcoServicesHall_ExtraFunction;

    //多账号是否乱序下单
    static bool m_bDisorderWhenInsertOrder;

};

#endif


