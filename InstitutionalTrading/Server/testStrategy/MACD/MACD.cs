using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using com.cifco.zqstp.Market;
using com.cifco.zqstp.Trader;
using com.cifco.zqstp.strategy.Indicator;
using com.cifco.zqstp.Misc;

namespace com.cifco.zqstp.strategy.Indicator.Customer
{

    /// <summary>
    /// 指标名称:MACD
    /// 指标类型:快照行情指标
    /// 指标描述:
    /// </summary>
    public class MACD : CIndicator
    {

        /// <summary>
        ///  
        /// </summary>
        int p1 = 12;

        /// <summary>
        ///  
        /// </summary>
        int p2 = 24;

        /// <summary>
        ///  
        /// </summary>
        int p3 = 8;


        public MACD(int p1, int p2, int p3) 
        {
			Log.Out("调用MACD(int p1, int p2, int p3)构造函数");
            //base();
			
            this.p1 = p1;
            this.p2 = p2;
            this.p3 = p3;

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
            

            // 是否需要立即进行指标计算。
            if(bCalcImmediately)
                return Calculate();
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
	    public override bool Calculate() 
		{
            // 请在这里填写实现代码。

            
            // 基类的Calculate()函数会清空未算深度行情队列。
    		return base.Calculate();
	    }

    }
}
