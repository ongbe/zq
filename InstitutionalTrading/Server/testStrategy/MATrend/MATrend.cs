using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using com.cifco.zqstp.Market;
using com.cifco.zqstp.Trader;
using com.cifco.zqstp.strategy.Strategy;
using com.cifco.zqstp.strategy.Indicator.Customer;
using com.cifco.zqstp.Misc;

namespace com.cifco.zqstp.strategy.Strategy.Customer
{
    /// <summary>
    /// 本类是服务器加载策略时使用的，请勿修改，否则服务器创建策略可能会失败。
    /// </summary>
    public class Factory
    {
        public CStrategy CreateObject(int p1, double p2) 
        {
			Log.Out("调用Factory.CreateObject()函数，创建MATrend对象");
			
			MATrend newobj = new MATrend();
            newobj.p1 = p1;
            newobj.p2 = p2;

            return newobj;
        }
    }
    

    /// <summary>
    /// 策略ID:MATrend
    /// 策略名称:均线穿越
    /// 策略描述:
    /// </summary>
    public class MATrend : CStrategy
    {
        
        /// <summary>
        /// 参数描述: 
        /// @title = "策略参数一"
        /// @default = 1
        /// @readonly = false
        /// </summary>
        public int p1 = 1;
        
        /// <summary>
        /// 参数描述: 
        /// @title = "策略参数二"
        /// @default = 1.5
        /// @readonly = false
        /// </summary>
        public double p2 = 1.5;
        
        /// <summary>
        /// 指标: MA
        /// </summary>
        MA ma;
        
        /// <summary>
        /// 指标: MACD
        /// </summary>
        MACD macd;

        bool bBuy = false;

        public MATrend()
        {
			//Log.Out("调用MATrend构造函数");
        }
        
		public override bool Initialize(string[] strInstruments) 
		{
			return true;
		}
		
		public override bool ResetBarCycles(string[] strInstruments) 
		{
			return true;
		}
		
		public override bool InitIndicators(String[] strInstruments) 
		{
            ma = new MA (5, 10, 20, 30, 60);
            //SaveIndicator (ma, strInstruments[0]);
            macd = new MACD (12, 24, 1);
            //SaveIndicator (macd, strInstruments[1]);

		
			return true;
		}

        /// <summary>
        /// 添加新行情。
        /// </summary>
        /// <param name="bCalcImmediately" default=false>是否立即进行指标计算</param>
        /// <returns>
        /// true: 成功
        /// false: 失败，原因指标对象可能未初始化
        /// </returns>
	    public override bool AppendMD(MarketDetph md, bool bCalcImmediately) 
		{
            
            // 预处理深度行情，并缓存到未算深度行情队列中。
            base.AppendMD(md, bCalcImmediately);


            // 请在这里填写实现代码。

            ma.AppendMD(md, bCalcImmediately);
            macd.AppendMD(md, bCalcImmediately);
            
            string[] timesplit = md.updateTime.Split(':');
            if(Int32.Parse(timesplit[2])%5 == 0) {
                InputOrder req = new InputOrder();

                req.InstrumentID = md.instrumentId;
                req.Qty = 1;
                req.Price = md.latestPrice;
                req.Hedge = '1';
                req.IsMarket = false;
                req.Direction = bBuy ? '0' : '1';
                req.OpenClose = '0';

                ReqOrderInsert(req);

                bBuy = !bBuy;
            }

            // 是否需要立即进行指标计算。
            if(bCalcImmediately)
                return CalculateMDIndicator();
            else
		        return true;
	    }
	    
        /// <summary>
        /// 添加新Bar。
        /// </summary>
        /// <param name="bCalcImmediately" default=false>是否立即进行指标计算</param>
        /// <returns>
        /// true: 成功
        /// false: 失败，原因指标对象可能未初始化
        /// </returns>
		public override bool AppendBar(Bar bar, BarSeries barSeries, bool bCalcImmediately) 
		{
            
            // 预处理深度行情，并缓存到未算深度行情队列中。
            base.AppendBar(bar, barSeries, bCalcImmediately);


            // 请在这里填写实现代码。
            

            // 是否需要立即进行指标计算。
            if(bCalcImmediately)
                return CalculateBarIndicator();
            else
		        return true;
		}

        /// <summary>
        /// 进行指标计算。使用缓存的未算深度行情，算完后会清空未算深度行情队列。
        /// </summary>
        /// <returns>
        /// true: 成功
        /// false: 失败，原因指标对象可能未初始化
        /// </returns>
        public override bool CalculateMDIndicator() 
        { 
            // 请在这里填写实现代码。
			
            
            //// 基类的Calculate()函数会清空未算深度行情队列。
    		//return base.Calculate();
			return true; 
		}
        
        /// <summary>
        /// 进行指标计算。使用缓存的未算Bar，算完后会清空未算Bar队列。
        /// </summary>
        /// <returns>
        /// true: 成功
        /// false: 失败，原因指标对象可能未初始化
        /// </returns>
        public override bool CalculateBarIndicator() 
        {
            // 请在这里填写实现代码。
			
            
			return true; 
		}
        
        /// <summary>
        /// 根据最新的数据进行一次策略运算，判断各种条件和状态，进行下单、撤单等动作，或维护自身状态。
        /// </summary>
        /// <returns>
        /// true: 成功
        /// false: 失败，原因指标对象可能未初始化
        /// </returns>
        public override bool LaunchStrategy()
		{
            // 请在这里填写实现代码。
			
            
			return true;
	    }
	    	    
	    // 报单状态通知
	    public override bool OnOrder(OrderInfo order) 
	    {
            if (order.OrderStatus == '1' || order.OrderStatus == '3')
            {
                InputAction req = new InputAction();

                req.BrokerID = order.BrokerID;
                req.DealerID = order.InvestorID;
                req.FrontID = order.FrontID;
                req.Session = order.SessionID;
                req.OrderRef = order.OrderRef;
                req.ExchangeID = order.ExchangeID;
                req.InstrumentID = order.InstrumentID;
                req.OrderSysID = order.OrderSysID;

                ReqOrderAction(req);
                
            }
            return true;
		}
	    
	    // 成交通知
	    public override bool OnTrade(TradeInfo trade) 
	    {

            QryTradingAccount qryTA = new QryTradingAccount();
            TradingAccount taRet = new TradingAccount();

            GetTradingAccount(qryTA, ref taRet);

            QryPosition qryPiso = new QryPosition();
            List<PositionInfo> lstPosi = new List<PositionInfo>();

            GetPositions(qryPiso, ref lstPosi);

            InputOrder req = new InputOrder();

            req.InstrumentID = trade.InstrumentID;
            req.Qty = trade.Volume;
            req.Price = trade.Price + (trade.Direction=='0' ? - 1.0 : 1.0);
            req.Hedge = '1';
            req.IsMarket = false;
            req.Direction = trade.Direction=='0' ? '1' : '0';
            req.OpenClose = '1';

            ReqOrderInsert(req);

            return true;
		}
	    
	    // 报单失败通知
	    public override bool OnOrderInsert(OrderInput orderInput) 
	    {
			return true;
		}
		
	    // 报单错误通知
	    public override bool OnOrderError(OrderInfo order) 
	    {
			return true;
		}
		
		// 撤单失败通知
	    public override bool OnOrderAction(ActionInput actionInput) 
	    {
			return true;
		}

    }
}
