using System;
using System.Collections.Generic;
using System.Diagnostics;
using com.cifco.zqstp.Market;
using com.cifco.zqstp.Trader;
using com.cifco.zqstp.strategy.Indicator;


namespace com.cifco.zqstp.strategy.Strategy
{
    public abstract class CStrategy
    {
        private IOrderService orderServer = null;
        private bool bHasTimer = false;
        private int nTimerInterval = 0;
        private Dictionary<String, List<CIndicator>> mapIndictors;

        public CStrategy()
        {
            mapIndictors = new Dictionary<String, List<CIndicator>>();
        }

        public void DebugOut(string fmt, params object[] args)
        {
            Debug.WriteLine("{0}", string.Format(fmt, args).Trim());
        }

        // 此函数只能在初始化策略的加载函数中调用，并且在Initialize之前调用。
        // 用户不要使用此函数，否则可能会发生异常。
        public void ResetOrderService(IOrderService orderServer)
        {
            DebugOut("Call CStrategy Function ResetOrderService from MainForm Successfully");

            this.orderServer = orderServer;
        }

        // 此函数只能在初始化策略的加载函数中调用，并且在Initialize之前调用。
        // 用户不要使用此函数，否则可能会发生异常。
        public void ResetTimer(bool bHasTimer, int nInterval)
        {
            DebugOut("Call CStrategy Function ResetTimer from MainForm Successfully");

            this.bHasTimer = bHasTimer;
            this.nTimerInterval = nInterval;
        }

        // 保存指标与合约之间的关系
        public bool SaveIndicator(CIndicator indictor, String instrument)
        {
            List<CIndicator> lstIndicators;
            if(mapIndictors.TryGetValue(instrument, out lstIndicators)) {
                lstIndicators.Add(indictor);
            }
            else {
                lstIndicators = new List<CIndicator>();
                lstIndicators.Add(indictor);
                mapIndictors[instrument] = lstIndicators;
            }
            return true;
        }

        // 保存指标与合约之间的关系
        public bool SaveIndicator(CIndicator indictor, String[] instruments)
        {
            foreach (String instrument in instruments) {
                SaveIndicator(indictor, instrument);
            }
            return true;
        }

        // 根据合约查找相关的指标
        public bool GetIndicators(String instrument, out List<CIndicator> lstIndicators)
        {
            return mapIndictors.TryGetValue(instrument, out lstIndicators);
        }

        public abstract bool Initialize(String[] strInstruments);

        virtual public bool ResetBarCycles(String[] strInstruments) 
        { 
            return true; 
        }
        
        virtual public bool InitIndicators(String[] strInstruments) 
        { 
            return true; 
        }

        virtual public bool OnMarketDepth(MarketDetph md) 
        {
            DebugOut("Call CStrategy Function AppendMD from MainForm Successfully");

            return true; 
        }

        virtual public bool OnBar(Bar bar, BarSeries barSeries) 
        {
            DebugOut("Call CStrategy Function AppendBar from MainForm Successfully");

            return true; 
        }
        
        virtual public bool OnTimer() 
        { 
            return true; 
        }

        virtual public bool OnOrder(OrderInfo order) 
        {
            DebugOut("Call CStrategy Function OnOrder from MainForm Successfully");

            return true; 
        }

        virtual public bool OnTrade(TradeInfo trade) 
        {
            DebugOut("Call CStrategy Function OnTrade from MainForm Successfully");

            return true; 
        }

        virtual public bool OnOrderInsert(OrderInput orderInput) 
        {
            DebugOut("Call CStrategy Function OnOrderInsert from MainForm Successfully");

            return true; 
        }

        virtual public bool OnOrderError(OrderInfo order) 
        {
            DebugOut("Call CStrategy Function OnOrderError from MainForm Successfully");

            return true; 
        }

        virtual public bool OnOrderAction(ActionInput actionInput) 
        {
            DebugOut("Call CStrategy Function OnOrderAction from MainForm Successfully");

            return true; 
        }

        // 请求下单
        public int ReqOrderInsert(InputOrder inputOrder) 
        {
            DebugOut("Call CStrategy Function ReqOrderInsert from MainForm Successfully");

            if (this.orderServer == null)
                return 999;
            return this.orderServer.ReqOrderInsert(inputOrder);
        }
        
        // 请求撤单
        public int ReqOrderAction(InputAction inputAction) 
        {
            DebugOut("Call CStrategy Function ReqOrderAction from MainForm Successfully");

            if (this.orderServer == null)
                return 999;
            return this.orderServer.ReqOrderAction(inputAction);
        }
        
        // 请求查询交易员资金帐号
        public bool GetTradingAccount(QryTradingAccount qry, ref TradingAccount refTradingAccount) 
        {
            DebugOut("Call CStrategy Function GetTradingAccount from MainForm Successfully");

            if (this.orderServer == null)
                return false;
            return this.orderServer.GetTradingAccount(qry, ref refTradingAccount); 
        }
        
        // 请求查询交易员持仓
        public bool GetPositions(QryPosition qry, ref List<PositionInfo> retPositions) 
        {
            DebugOut("Call CStrategy Function GetPositions from MainForm Successfully");

            if (this.orderServer == null)
                return false;
            return this.orderServer.GetPositions(qry, ref retPositions); 
        }

        // 请求查询交易员持仓明细
        public bool GetPositionDetails(QryPositionDetail qry, ref List<PositionDetailInfo> retPositionDetails)
        {
            DebugOut("Call CStrategy Function GetPositionDetails from MainForm Successfully");

            if (this.orderServer == null)
                return false;
            return this.orderServer.GetPositionDetails(qry, ref retPositionDetails);
        }

        // 请求查询报单
        public bool GetOrders(QryOrder qry, ref List<OrderInfo> retOrders) 
        {
            DebugOut("Call CStrategy Function GetOrders from MainForm Successfully");

            if (this.orderServer == null)
                return false;
            return this.orderServer.GetOrders(qry, ref retOrders); 
        }
        
        // 请求查询成交
        public bool GetTrades(QryTrade qry, ref List<TradeInfo> refTrades) 
        {
            DebugOut("Call CStrategy Function GetTrades from MainForm Successfully");

            if (this.orderServer == null)
                return false;
            return this.orderServer.GetTrades(qry, ref refTrades); 
        }
        
    }

}
