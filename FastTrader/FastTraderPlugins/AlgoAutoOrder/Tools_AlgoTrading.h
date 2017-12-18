// Tools_AlgoTradingNo1Dlg.h : 定义本项目的通用结构体
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include <string>
#include <sstream>
#include <vector>
using namespace std;



class CTools_AlgoTrading
{
public:   

	static bool IsHaveDot(CString& str)
    {
		return (-1 == str.Find('.'))?false:true;
    }

	static const string StringFromDouble(double dval)
	{
		char buf[256];
		memset(buf,0,sizeof(buf));
		sprintf_s(buf,sizeof(buf)-1,"%.10f",dval);
		if(strchr(buf,'.')!=NULL)
		{
			int len=strlen(buf);
			while(len>1&&buf[len-1]=='0'&&buf[len-2]!='.')
			{
				buf[len-1]=0;
				len--;
			}
		}
		return string(buf);
	};
	static const string StringFromInt(int ival)
	{
		char buf[256];
		memset(buf,0,sizeof(buf));
		sprintf_s(buf,sizeof(buf)-1,"%d",ival);
		return string(buf);
	}

    //从CEdit中取出int值
    static int GetIntFromCEdit(CEdit& edit)
    {
        CString buf;
        edit.GetWindowTextA(buf);
        return _ttoi(buf);
    }
	//从CEdit中取出double值
	static double GetDoubleFromCEdit(CEdit& edit)
	{
		char buf[256];
		memset(buf,0,sizeof(buf));
		edit.GetWindowTextA(buf,sizeof(buf)-1);
		return atof(buf);
	}
    //从CEdit中取出string
    static string GetStringFromCEdit(CEdit& edit)
    {
        char buf[256];
        memset(buf,0,sizeof(buf));
        edit.GetWindowTextA(buf,sizeof(buf)-1);
        return string(buf);

    }
    //从CEdit中取出CString
    static CString GetCStringFromCEdit(CEdit& edit)
    {
        CString str;
        edit.GetWindowText(str);
        return str;
    }
	//将double值设置到CEdit中
	static void SetDoubleToCEdit(CEdit& edit,double dval)
	{
		edit.SetWindowTextA(CTools_AlgoTrading::StringFromDouble(dval).c_str());
	}
	//将int值设置到CEdit中
	static void SetIntToCEdit(CEdit& edit,int ival)
	{
		edit.SetWindowTextA(CTools_AlgoTrading::StringFromInt(ival).c_str());
	}
    //将string值设置到CEdit中
	static void SetStringToCEdit(CEdit& edit,const char* pval)
	{
		edit.SetWindowTextA(pval);
	}


	//从CComboBox中取出string
	static std::string GetStringFromCComboBox(CComboBox& comboBox)
	{
		char buf[256];
		memset(buf,0,sizeof(buf));
		comboBox.GetWindowTextA(buf,sizeof(buf)-1);
		return string(buf);
	}


	//从CStatic中取出string
	static std::string GetStringFromCStatic(CStatic& sta)
	{
		char buf[256];
		memset(buf,0,sizeof(buf));
		sta.GetWindowTextA(buf,sizeof(buf)-1);
		return string(buf);
	}
    //从CStatic中取出double
    static double GetDoubleFromCStatic(CStatic& sta)
    {
        char buf[256];
        memset(buf,0,sizeof(buf));
        sta.GetWindowTextA(buf,sizeof(buf)-1);
        return atof(buf);
    }
	//将string设置到CStatic中
	static void SetStringToCStatic(CStatic& sta,const string& pval)
	{
		sta.SetWindowTextA((LPCTSTR)pval.c_str());
	}
    
    static void SetdoubleToCStatic(CStatic& sta,const double dval)
    {
        sta.SetWindowTextA(CTools_AlgoTrading::StringFromDouble(dval).c_str());
    }

    static void SetIntToCStatic(CStatic& sta,const int ival)
    {
        sta.SetWindowTextA(CTools_AlgoTrading::StringFromInt(ival).c_str());
    }

    static void SetTextToCEdit(CString& str,CEdit& edit)
    {
        edit.SetWindowText(str);
    }

    static void GetTextFromCEdit(CString& str,CEdit& edit)
    {
        edit.GetWindowTextA(str);
    }

    static void SetTextToCStatic(CStatic& sta,CString& str)
    {
        sta.SetWindowTextA(str);
    }

    //*********************类型转换***************************/
	static char* CStringToChars(CString& str)
	{
		return str.GetBuffer(str.GetLength());
	}
    
    static void CString2String(CString& strSrc, std::string& strDes) 
    { 
#ifndef UNICODE 
        strDes = strSrc; 
#else 
        USES_CONVERSION; 

        strDes = W2A(strSrc.LockBuffer()); 
        strSrc.UnlockBuffer(); 
#endif 	
    }

    static void String2CString(std::string& strSrc,CString& strDes)
    {
        strDes = strSrc.c_str();
    }

    static void CharStar2CString(char* charSrc, CString& strDes)
    {
        strDes = charSrc;
    }

    static char* CString2CharStar(CString& strSrc)
    {
        char* pcharDes;
#ifndef UNICODE 
        pcharDes = new char [strSrc.GetAllocLength()+1];
        sprintf(pcharDes,"%s",strSrc);
        //pcharDes =  (LPTSTR)(LPCTSTR)strSrc; 
        //在非unicode下也可以用这种方式，但是返回值就不用delete[]删除了。
        //不建议用(LPCTSTR)进行强制类型转化，这样strSrc大小发生变化时会出现错误
#else
        int len=WideCharToMultiByte(CP_ACP,0,strSrc,strSrc.GetLength(),NULL,0,NULL,NULL);
        pcharDes=new char[len+1];
        WideCharToMultiByte(CP_ACP,0,strSrc,strSrc.GetLength(),pcharDes,len,NULL,NULL);
        pcharDes[len]='\0';	
#endif
        return pcharDes;
    }

    static void CharStar2String(char* charSrc, std::string& strDes)
    {
        strDes = charSrc;
    }

    static char* String2CharStar(std::string& strSrc)
    {
        return const_cast<char*>(strSrc.c_str());
    }


    static void CString2Char(CString& str, char ch[],int nLength)  

    {  
        int i;
        int ncount;
        char* temp = CString2CharStar(str);
        if (nLength > str.GetLength())
        {
            ncount = str.GetLength();
        }
        else
        {
            ncount = nLength;
        }
        for(i = 0; i < ncount-1; i++) 
        {
            ch[i] = temp[i];
        }
        ch[ncount-1] = '\0';
        delete []temp;
    } 

    static void Char2CString(CString& str, char ch[])
    {
        str = ch;
    }

    /* 将一个指定小数点位数的双精度数, 格式化成带千分号的字符串 dec为小数点位数*/
    static char *mdou(double val,int dec)
    {
        int i,sign;
        int len=24; /* 限定字符串的最大长度 */
        char *s;
        s=(char *)calloc(len+1,sizeof(char));
        /* 先置空格串 */
        /* 然后,从低位到高位将双精度数格式化成字符串 */
        for(i=0;i<len;i++) *(s+i)=' ';
        s+=len;
        *s='\0';
        /* 置符号位 */
        if(val<0){sign=1;val*=-1;} else sign=0;

        /* 误差纠正 */
        val+=0.1e-7; 

        /* 先格式化小数部分 */
        for(i=0;i<dec;i++) val*=10;
        for(i=0;i<dec;i++){*--s=(int)fmod(val,10)+'0';val/=10;}
        if(dec>0) *--s='.'; /* 加入小数点 */
        /* 再格式化整数部分 */
        for(i=1;val>=10;val/=10,i++){
            *--s=(int)fmod(val,10)+'0';
            if(i%3==0) *--s=','; /* 加入千分符 */
        }
        if((int)val>0) *--s=(int)val+'0'; /* 加入最高位 */
        if(sign) *--s='-'; /* 加入符号位 */
        return(s);
    }

    //=============================对时间的处理============================
    static int GetMinutesFromCString(CString& strData)
    {
        CString hour,minute;
        int nHour,nMinute;
        for (int i=0;i<strData.GetLength();i++)
        {
            hour = strData.Left(2);
            minute = strData.Mid(3,2);
            
            nHour = _ttoi(hour);
            nMinute = _ttoi(minute);

            return (nHour*60 + nMinute);
        }
    }

	static int GetMinutesFromString(string strData)
	{
		CString str,hour,minute;
		int nHour,nMinute;

		str = strData.c_str();
		for (int i=0;i<str.GetLength();i++)
		{
			hour = str.Left(2);
			minute = str.Mid(3,2);

			nHour = _ttoi(hour);
			nMinute = _ttoi(minute);

			return (nHour*60 + nMinute);
		}

		return 0;
	}

};