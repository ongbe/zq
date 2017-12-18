// common-AlgoTradingNo1Dlg.h : 定义本项目的通用结构体
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include <string>
#include <sstream>
#include <vector>
using namespace std;



class CTools
{
public:

    static bool IsHaveDot(std::string& str)
    {
        return (std::string::npos == str.find('.'))?false:true;
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
	//从CEdit中取出double值
	static double GetDoubleFromCEdit(CEdit& edit)
	{
		char buf[256];
		memset(buf,0,sizeof(buf));
		edit.GetWindowTextA(buf,sizeof(buf)-1);
		return atof(buf);
	}
	//将double值设置到CEdit中
	static void SetDoubleToCEdit(CEdit& edit,double dval)
	{
		edit.SetWindowTextA(CTools::StringFromDouble(dval).c_str());
	}
	//将int值设置到CEdit中
	static void SetIntToCEdit(CEdit& edit,int ival)
	{
		edit.SetWindowTextA(CTools::StringFromInt(ival).c_str());
	}
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
	//从CEdit中取出string
	static std::string GetStringFromCEdit(CEdit& edit)
	{
		char buf[256];
		memset(buf,0,sizeof(buf));
		edit.GetWindowTextA(buf,sizeof(buf)-1);
		return string(buf);
	}
    //从CDateTimeCtrl中取出日期值
    static CTime GetCTimeFromCDateTimeCtrl(CDateTimeCtrl& edit)
    {
        CTime l_time;
        edit.GetTime(l_time);
        return l_time;
    }
    //将日期值设置到CDateTimeCtrl中
    static void SetCTimeToCDateTimeCtrl(CDateTimeCtrl& edit,const CTime* pVal)
    {
        edit.SetTime(pVal);
    }
};