using System;
using System.Collections.Generic;
//using System.Linq;
using System.Text;
using com.cifco.zqstp.Market;
using com.cifco.zqstp.Trader;

namespace com.cifco.zqstp.strategy.Indicator
{
    public class CIndicator
    {
        public CIndicator() 
        { 
        }

        virtual public bool AppendMD(MarketDetph md, bool bCalcImmediately) 
        { 
            return true; 
        }

        virtual public bool Calculate() 
        { 
            return true; 
        }

    }
}
