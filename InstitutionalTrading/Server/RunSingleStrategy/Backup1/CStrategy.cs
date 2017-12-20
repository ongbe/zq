using System;
using System.Collections.Generic;
using System.Diagnostics;
using com.cifco.zqstp.Market;
using com.cifco.zqstp.Trader;


namespace com.cifco.zqstp.strategy.Strategy
{
    public abstract class CStrategy
    {
        public void DebugOut(string fmt, params object[] args)
        {
            Debug.WriteLine("{0}", string.Format(fmt, args).Trim());
        }

        IOrderService orderServer = null;

        public CStrategy()
        {
        }

        // 此函数只能在初始化策略的加载函数中调用，并且在Initialize之前调用。
        // 用户不要使用此函数，否则可能会发生异常。
        public void ResetOrderService(IOrderService orderServer)
        {
            DebugOut("Call CStrategy Function ResetOrderService from MainForm Successfully");

            this.orderServer = orderServer;
        }

        public abstract bool Initialize(string[] strInstruments);
        
        virtual public bool ResetBarCycles(string[] strInstruments) 
        { 
            return true; 
        }
        
        virtual public bool InitIndicators(String[] strInstruments) 
        { 
            return true; 
        }
        
        virtual public bool AppendMD(MarketDetph md, bool bCalcImmediately) 
        {
            DebugOut("Call CStrategy Function AppendMD from MainForm Successfully");

            return true; 
        }

        virtual public bool AppendBar(Bar bar, BarSeries barSeries, bool bCalcImmediately) 
        {
            DebugOut("Call CStrategy Function AppendBar from MainForm Successfully");

            return true; 
        }
        
        virtual public bool CalculateMDIndicator() 
        { 
            return true; 
        }

        virtual public bool CalculateBarIndicator() 
        { 
            return true; 
        }
        
        virtual public bool LaunchStrategy() 
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
