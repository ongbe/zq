using System;
using System.Collections.Generic;
//using System.Linq;
using System.Text;

namespace com.cifco.zqstp.Trader
{
    public abstract class IOrderService
    {
        // 请求下单
        public abstract int ReqOrderInsert(InputOrder inputOrder);

        // 请求撤单
        public abstract int ReqOrderAction(InputAction inputAction);

        // 请求查询交易员资金帐号
        public abstract bool GetTradingAccount(QryTradingAccount qry, ref TradingAccount refTradingAccount);

        // 请求查询交易员持仓
        public abstract bool GetPositions(QryPosition qry, ref List<PositionInfo> retPositions);

        // 请求查询报单
        public abstract bool GetOrders(QryOrder qry, ref List<OrderInfo> retOrders);
<<<<<<< .mine
=======

        // 请求查询成交。保留函数，未实现。
        public abstract bool GetTrades(QryTrade qry, ref List<TradeInfo> refTrades);
>>>>>>> .r3042
    }
}
