using System;
using System.Collections.Generic;
//using System.Linq;
using System.Text;
using System.IO;

namespace com.cifco.zqstp.strategy.codegenerate
{

    public class CStrategyClassMake
    {
        public class CParameter
        {
            public CParameter(string strTitle, string strName, string strType, 
                              string strDirect, string AppearMode, 
                              bool bReadOnly, string strDefault, string strDescription,
                              List<CParamSubItem> lstSubItem)
            {
                this.strTitle = strTitle;
                this.strName = strName;
                this.strType = strType;
                this.strDirect = strDirect;
                this.AppearMode = AppearMode;
                this.bReadOnly = bReadOnly;
                this.strDefault = strDefault;
                this.strDescription = strDescription;
                this.lstSubItem = lstSubItem;
            }

            public string strTitle;
            public string strName;
            public string strType;
            public string strDirect;
            public string AppearMode;
            public bool bReadOnly;
            public string strDefault;
            public string strDescription;
            public List<CParamSubItem> lstSubItem;
        }

        public class CParamSubItem
        {
            public CParamSubItem(string strName, string strValue)
            {
                this.strName = strName;
                this.strValue = strValue;
            }

            public string strName = "";
            public string strValue = "";
        }

        public class CIndicatorItem
        {
            public CIndicatorItem(string strIndicator, string strName,
                              int nInstrumentIndex, int nBarCycle, int nCycleUnit, 
                              bool bOut, string strIndicatorParamAssign)
            {
                this.strIndicator = strIndicator;
                this.strName = strName;
                this.nInstrumentIndex = nInstrumentIndex;
                this.nBarCycle = nBarCycle;
                this.nCycleUnit = nCycleUnit;
                this.bOut = bOut;
                this.strIndicatorParamAssign = strIndicatorParamAssign;
            }

            public string strIndicator;
            public string strName;
            public int nInstrumentIndex;
            public int nBarCycle;
            public int nCycleUnit;
            public bool bOut;
            public string strIndicatorParamAssign;
        }

        public CStrategyClassMake()
        {
            lstParams = new List<CParameter>();
            lstIndicators = new List<CIndicatorItem>();
        }

        public string GetTemplateFile()
        {
            return strTemplateFile;
        }
        public void SetTemplateFile(string strFile)
        {
            strTemplateFile = strFile;
        }
        public string GetNameEng()
        {
            return strNameEng;
        }
        public void SetNameEng(string strNameEng)
        {
            this.strNameEng = strNameEng;
        }
        public string GetNameChs()
        {
            return strNameChs;
        }
        public void SetNameChs(string strNameChs)
        {
            this.strNameChs = strNameChs;
        }
        public string GetVerNum()
        {
            return strVerNum;
        }
        public void SetVerNum(string strVerNum)
        {
            this.strVerNum = strVerNum;
        }
        public void SetDescription(string strDescription)
        {
            this.strDescription = strDescription;
        }
        public string GetDescription()
        {
            return strDescription;
        }
        public void AppendParameter(string strTitle, string strName, string strType,
                              string strDirect, string AppearMode,
                              bool bReadOnly, string strDefault, string strDescription, 
                              List<CParamSubItem> lstSubItem)
        {
            CParameter param = new CParameter(strTitle, strName, strType, strDirect, AppearMode,
                                              bReadOnly, strDefault, strDescription, lstSubItem);
            lstParams.Add(param);
        }
        public void ClearParameters()
        {
            lstParams.Clear();
        }
        // nInstrumentIndex, 0 表示全部合约，>0 表示指定下标的合约
        public void AppendIndicator(string strIndicator, string strName,
                              int nInstrumentIndex, int nBarCycle, int nCycleUnit,
                              bool bOut, string strIndicatorParamAssign)
        {
            CIndicatorItem param = new CIndicatorItem(strIndicator, strName,
                              nInstrumentIndex, nBarCycle, nCycleUnit, 
                              bOut, strIndicatorParamAssign);
            lstIndicators.Add(param);
        }
        public void ClearIndicators()
        {
            lstIndicators.Clear();
        }

        public bool MakeCode(ref string strRetCode)
        {
            string strText = "";
            string strItem = "";
            try
            {
                StreamReader sr = new StreamReader(strTemplateFile);
                strText = sr.ReadToEnd();    //此方法等于一口气读完
                sr.Close();
            }
            catch (Exception e)
            {
                return false;
            }

            // 1.  替换名称和类型
            strText = strText.Replace("<#StrategyNameEng#>", strNameEng);
            strText = strText.Replace("<#StrategyName#>", strNameChs);
            strText = strText.Replace("<#ClassName#>", strNameEng);

            // 2.  生成成员变量
            strItem = "";
            foreach (CParameter param in lstParams)
            {
                strItem += "        \r\n";
                strItem += "        /// <summary>\r\n";
                strItem += "        /// 参数描述: \r\n";
                //strItem += "        /// @title = \"" + param.strTitle + "\"\r\n";
                //strItem += "        /// @default = " + param.strDefault + "\r\n";
                //strItem += "        /// @readonly = " + (param.bReadOnly ? "true" : "false") + "\r\n";
                strItem += "        /// </summary>\r\n";
                strItem += "        public " + param.strType + " " + param.strName + " = " + param.strDefault + ";\r\n";
            }
            foreach (CIndicatorItem param in lstIndicators)
            {
                strItem += "        \r\n";
                strItem += "        /// <summary>\r\n";
                strItem += "        /// 指标: " + param.strIndicator + "\r\n";
                strItem += "        /// </summary>\r\n";
                strItem += "        " + param.strIndicator + " " + param.strName + ";\r\n";
            }
            strText = strText.Replace("<#MembarParams#>", strItem);

            // 3.  生成构造函数
            strItem = strNameEng;
            strText = strText.Replace("<#ClassConstract#>", strItem);

            // 4.  生成参数设置函数
            strItem = "(";
            foreach (CParameter param in lstParams)
            {
                strItem += param.strType + " " + param.strName + ", ";
                // 不能设置缺省值，否则会出现Compiler Error CS0241
                //strItem += param.strType + " " + param.strName + " = " + param.strDefault + ", ";
            }
            strItem += ")";
            strItem = strItem.Replace(", )", ")");
            strText = strText.Replace("<#ResetParamsDeclare#>", strItem);

            // 5.  生成参数设置函数实现代码
            strItem = "";
            foreach (CParameter param in lstParams)
            {
                strItem += "            newobj." + param.strName + " = " + param.strName + ";\r\n";
            }
            strText = strText.Replace("<#ResetParamsBody#>", strItem);

            // 6.  生成指标初始化代码
            strItem = "";
            foreach (CIndicatorItem param in lstIndicators)
            {
                strItem += "            " + param.strName + " = new " + param.strIndicator + 
                           " (" + param.strIndicatorParamAssign + ");\r\n";
                if(param.nInstrumentIndex==0)
                    strItem += "            //" + "SaveIndicator (" + param.strName + ", strInstruments);\r\n";
                else
                    strItem += "            //" + "SaveIndicator (" + param.strName +
                               ", strInstruments[" + (param.nInstrumentIndex-1) + "]);\r\n";
            }
            strText = strText.Replace("<#InitIndicator#>", strItem);

            strRetCode = strText;
            return true;
        }

        public bool MakeParamsList(ref string strRetParams)
        {
            string strItem = "";
            strItem += "<strategy" + " id = '" + strNameEng + "' name = '" + strNameChs
                    + "' version = '" + strVerNum + "'>\r\n";
            strItem += "    <description>\r\n";
            strItem += strDescription;
            strItem += "\r\n    </description>\r\n";
            strItem += "    <params>\r\n";
            foreach (CParameter param in lstParams)
            {
                strItem += "        <param type='" + param.strType + "' name='" + param.strName
                        + "' direction='" + param.strDirect
                        + "' appearmode='" + param.AppearMode
                        + "' readonly='" + param.bReadOnly
                        + "' title='" + param.strTitle
                        + "' default='" + param.strDefault
                        + "'>\r\n";
                foreach (CParamSubItem subItem in param.lstSubItem)
                {
                    strItem += "            <subitem name='" + subItem.strName
                            + "' value='" + subItem.strValue
                            + "'></subitem>\r\n";
                }
                strItem += "        </param>\r\n";
            }
            strItem += "    </params>\r\n";
            strItem += "    <indicators>\r\n";
            foreach (CIndicatorItem item in lstIndicators)
            {
                strItem += "        <indicator type='" + item.strIndicator + "' name='" + item.strName
                        + "' instrumentindex='" + item.nInstrumentIndex
                        + "' barcycle='" + item.nBarCycle
                        + "' cycleunit='" + item.nCycleUnit
                        + "' paramassign='" + item.strIndicatorParamAssign
                        + "' outlog='" + item.bOut
                        + "'>" + "</indicator>\r\n";
            }
            strItem += "    </indicators>\r\n";
            strItem += "</strategy>\r\n";
            // 返回参数列表
            strRetParams = strItem;

            return true;
        }

        public bool MakeCodeToFile(string strPathFile)
        {
            string strCode = "";
            bool bRet = false;
            if (MakeCode(ref strCode))
            {
                try
                {
                    using (StreamWriter sw = new StreamWriter(strPathFile))
                    {
                        sw.Write(strCode);
                        sw.Close();
                    }
                    bRet = true;
                }
                catch (Exception e)
                {
                }
            }
            return bRet;
        }

        public bool MakeParasToFile(string strPathFile)
        {
            string strCode = "";
            bool bRet = false;
            if (MakeParamsList(ref strCode))
            {
                try
                {
                    using (StreamWriter sw = new StreamWriter(strPathFile))
                    {
                        sw.Write(strCode);
                        sw.Close();
                    }
                    bRet = true;
                }
                catch (Exception e)
                {
                }
            }
            return bRet;
        }

        string strTemplateFile = "";
        string strNameEng = "";
        string strNameChs = "";
        string strVerNum = "";
        string strDescription = "";
        List<CParameter> lstParams;
        List<CIndicatorItem> lstIndicators;
    }
}
