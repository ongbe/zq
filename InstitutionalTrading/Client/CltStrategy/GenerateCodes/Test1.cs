using System;
using System.Collections.Generic;
//using System.Linq;
using System.Text;

namespace com.cifco.zqstp.strategy.codegenerate
{
    public class Test1
    {
        static void Main(string[] args)
        {
            GenIndicatorCode();
            Console.ReadLine();
        }

        static void GenIndicatorCode()
        {
            string strCode = "";
            CIndicatorClassMake classMake = new CIndicatorClassMake();
            classMake.SetTemplateFile("CIndicatorTemplate.txt");
            classMake.SetType(1);
            classMake.SetFolder("价格类型");
            classMake.SetName("MACD");
            classMake.AppendParameter("fastma", "int", "12", "快速均价");
            classMake.AppendParameter("slowma", "int", "24", "慢速均价");
            classMake.AppendParameter("macd", "int", "1", "平均值");

            classMake.MakeCode(ref strCode);
            Console.Write(strCode);
        }

        static void GenStrategyCode()
        {
        }
    }
}
