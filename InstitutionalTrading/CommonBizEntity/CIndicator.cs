using System;
using System.Collections.Generic;
//using System.Linq;
using System.Text;
using com.cifco.zqstp.Market;
using com.cifco.zqstp.Trader;
using com.cifco.zqstp.Misc;

namespace com.cifco.zqstp.strategy.Indicator
{
    public class CIndicator
    {
        String strKey = "indicator";

        public CIndicator() 
        { 
        }

        // 设置指标的Key，用于输出数据是识别。
        public string IndicatorKey
        {
            get { return strKey; }
            set { strKey = value; }
        }

        // 指定输出为csv格式
        public void StoreData(String strDatas)
        {
            StoreIndicators.Out(this.strKey + "," + strDatas);
        }

        // 指定输出为csv格式
        public void StoreFormatData(string fmt, params object[] args)
        {
            StoreIndicators.Out(this.strKey + "," + string.Format(fmt, args).Trim());
        }

        virtual public bool OnMarketDepth(MarketDetph md) 
        { 
            return true; 
        }

    }
}
