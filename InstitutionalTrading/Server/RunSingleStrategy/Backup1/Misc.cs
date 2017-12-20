using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Xml;

namespace com.cifco.zqstp.Misc
{
    public class Log
    {
        protected static bool bSaveLog = false;
        protected static string strPathFile = "";

        public static bool SaveLog
        { 
        get{ return bSaveLog; }
        set{ bSaveLog = value; }
        }

        public static string PathFile
        {
            get { return strPathFile; }
            set { strPathFile = value; }
        }

        public void Printf(string fmt, params object[] args)
        {
            Out(string.Format(fmt, args).Trim());
        }

        public static void Out(string strMsg)
        {
            if (bSaveLog)
            {
                DateTime dt = DateTime.Now;
                String strNewMsg = dt.ToShortDateString() + " " 
                        + dt.ToLongTimeString() + ": " + strMsg;
                try
                {
                    using (StreamWriter sw = new StreamWriter(strPathFile, true))
                    {
                        sw.Write(strNewMsg);
                        sw.Write("\r\n");
                        sw.Close();
                    }
                }
                catch (Exception e)
                {
                }
            }
        }
    }

    public class ParseParams
    {
        // 解析指标参数xml
        public static Object[] ParseIndicatorParams(string strParamsXML)
        {
            Object[] args;

            XmlDocument doc = new XmlDocument();
            doc.LoadXml(strParamsXML);

            XmlNode nodeRoot = doc.SelectSingleNode("indicator");   //获取indicator节点 
            XmlNodeList nodeList = nodeRoot.SelectSingleNode("params").ChildNodes;   //获取Params节点的所有子节点 
            args = new Object[nodeList.Count];
            int nIndex = 0;
            foreach(XmlNode xn in nodeList) {           //遍历所有子节点 
                XmlElement xe=(XmlElement)xn;           //将子节点类型转换为XmlElement类型 
                string valtype = xe.GetAttribute("type");
                string valdefault = xe.GetAttribute("default");
                if (valtype == "int") {
                    args[nIndex] = (object)Int32.Parse(valdefault);
                }
                else if (valtype == "double") {
                    args[nIndex] = (object)Double.Parse(valdefault);
                }
                else if (valtype == "bool") {
                    args[nIndex] = (object)Boolean.Parse(valdefault);
                }
                nIndex++;
            }
            return args;
        }

        // 解析策略参数xml
        public static Object[] ParseStrategyParams(string strParamsXML)
        {
            Object[] args;

            XmlDocument doc = new XmlDocument();
            doc.LoadXml(strParamsXML);

            XmlNode nodeRoot = doc.SelectSingleNode("strategy");   //获取strategy节点 
            XmlNodeList nodeList = nodeRoot.SelectSingleNode("params").ChildNodes;   //获取Params节点的所有子节点 
            args = new Object[nodeList.Count];
            int nIndex = 0;
            foreach (XmlNode xn in nodeList)
            {           //遍历所有子节点 
                XmlElement xe = (XmlElement)xn;           //将子节点类型转换为XmlElement类型 
                string valtype = xe.GetAttribute("type");
                string valdefault = xe.GetAttribute("default");
                if (valtype == "int")
                {
                    args[nIndex] = (object)Int32.Parse(valdefault);
                }
                else if (valtype == "double")
                {
                    args[nIndex] = (object)Double.Parse(valdefault);
                }
                else if (valtype == "bool")
                {
                    args[nIndex] = (object)Boolean.Parse(valdefault);
                }
                else if (valtype == "String")
                {
                    args[nIndex] = valdefault;
                }
                else if (valtype == "DateTime")
                {
                    args[nIndex] = (object)Convert.ToDateTime(valdefault);
                }
                //C# 不支持date和time类型
                //else if (xe.GetAttribute("name") == "date")
                //{
                //    args[nIndex] = (object)Convert.ToDateTime(valdefault);
                //}
                //else if (xe.GetAttribute("name") == "time")
                //{
                //    args[nIndex] = (object)Convert.ToDateTime(valdefault);
                //}
                //由于不知道如何定义Cycle和Strategy也暂时不使用。
                //else if (xe.GetAttribute("name") == "Cycle")
                //{
                //    args[nIndex] = (object)Cycle.Parse(valdefault);
                //}
                //else if (xe.GetAttribute("name") == "Strategy")
                //{
                //    args[nIndex] = (object)Strategy.Parse(valdefault);
                //}
                nIndex++;
            }
            return args;
        }

    }
}
