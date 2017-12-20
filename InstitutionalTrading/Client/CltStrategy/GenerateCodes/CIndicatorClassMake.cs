using System;
using System.Collections.Generic;
//using System.Linq;
using System.Text;
using System.IO;

namespace com.cifco.zqstp.strategy.codegenerate
{

    public class CIndicatorClassMake
    {
        public class CParameter
        {
            public CParameter(string strName, string strType, string strDefault, string strDescription)
            {
                this.strName = strName;
                this.strType = strType;
                this.strDefault = strDefault;
                this.strDescription = strDescription;
            }
            public string strName;
            public string strType;
            public string strDefault;
            public string strDescription;
        }

        public CIndicatorClassMake()
        {
            lstParams = new List<CParameter>();
        }

        public string GetTemplateFile()
        {
            return strTemplateFile;
        }
        public void SetTemplateFile(string strFile)
        {
            strTemplateFile = strFile;
        }
        public int GetType()
        {
            return nType;
        }
        public void SetType(int nType)
        {
            this.nType = nType;
        }
        public string GetFolder()
        {
            return strFolder;
        }
        public void SetFolder(string strFolder)
        {
            this.strFolder = strFolder;
        }
        public string GetName()
        {
            return strName;
        }
        public void SetName(string strName)
        {
            this.strName = strName;
        }
        public void SetDescription(string strDescription)
        {
            this.strDescription = strDescription;
        }
        public string GetDescription()
        {
            return strDescription;
        }
        public void AppendParameter(string strName, string strType, string strDefault, string strDescription)
        {
            CParameter param = new CParameter(strName, strType, strDefault, strDescription);
            lstParams.Add(param);
        }
        public void ClearParameters()
        {
            lstParams.Clear();
        }

        public bool MakeCode(ref string strRetCode)
        {
            string strText = "";
            string strItem = "";
            string strTemp = "";
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
            strText = strText.Replace("<#IndicatorType#>", nType.ToString());
            strText = strText.Replace("<#IndicatorName#>", strName);
            strText = strText.Replace("<#ClassName#>", strName);

            // 2.  生成成员变量
            strItem = "";
            foreach(CParameter param in lstParams)
            {
                strItem += "\r\n";
                strItem += "        /// <summary>\r\n";
                strItem += "        /// ";
                strTemp = param.strDescription.Replace("\r\n", "\r\n        /// ");
                strItem += strTemp;
                strItem += "\r\n        /// </summary>\r\n";
                strTemp = "        " + param.strType + " " + param.strName + " = " + param.strDefault + ";\r\n";
                strItem += strTemp;
            }
            strText = strText.Replace("<#MembarParams#>", strItem);

            // 3.  生成带参数的构造函数
            strItem = strName;
            strItem += "(";
            foreach (CParameter param in lstParams)
            {
                strTemp = param.strType + " " + param.strName + ", ";
                strItem += strTemp;
            }
            strItem += ")";
            strItem = strItem.Replace(", )", ")");
            strText = strText.Replace("<#ClassConstract#>", strItem);

            // 4.  生成成员变量赋值代码
            strItem = "";
            foreach(CParameter param in lstParams)
            {
                strTemp = "            this." + param.strName + " = " + param.strName + ";\r\n";
                strItem += strTemp;
            }
            strText = strText.Replace("<#MemberParamsAssign#>", strItem);

            strRetCode = strText;
            return true;
        }

        public bool MakeParamsList(ref string strRetParams)
        {
            string strItem = "";
            strItem += "<indicator" + " type = '" + nType
                    + "' name = '" + strName
                    + "' folder = '" + strFolder + "'>\r\n";
            strItem += "    <description>\r\n";
            strItem += strDescription;
            strItem += "\r\n    </description>\r\n";
            strItem += "    <params>\r\n";
            foreach (CParameter param in lstParams)
            {
                strItem += "        <param type='" + param.strType + "' name='" + param.strName
                       + "' default='" + param.strDefault + "'>" + "</param>\r\n";
            }
            strItem += "    </params>\r\n";
            strItem += "</indicator>\r\n";
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
        int nType = 0;
        string strFolder = "";
        string strName = "";
        string strDescription = "";
        List<CParameter> lstParams;
    }
}
