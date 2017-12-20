// ForTestor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace System;
using namespace System::IO;
using namespace System::Text;
using namespace System::Reflection;
using namespace System::Diagnostics;
using namespace System::Collections::Generic;
using namespace com::cifco::zqstp::strategy::codegenerate;
using namespace com::cifco::zqstp::Misc;


void TestGenerateIndicator()
{
    String^ strCode = "";
    CIndicatorClassMake^ classMake = gcnew CIndicatorClassMake();
    classMake->SetTemplateFile("CIndicatorTemplate.txt");
    classMake->SetType(1);
    classMake->SetFolder("价格类型");
    classMake->SetName("MACD");
	classMake->SetDescription("这是关于MACD指标的描述");
    classMake->AppendParameter("fastma", "int", "12", "快速均价");
    classMake->AppendParameter("slowma", "int", "24", "慢速均价");
    classMake->AppendParameter("macd", "int", "1", "平均值");

	String^ strRetParams = "";
	classMake->MakeParamsList(strRetParams);
	classMake->MakeCodeToFile("MACD.cs");
	
}

void TestGenerateIndicator2()
{
    String^ strCode = "";
    CIndicatorClassMake^ classMake = gcnew CIndicatorClassMake();
    classMake->SetTemplateFile("CIndicatorTemplate.txt");
    classMake->SetType(1);
    classMake->SetFolder("价格类型");
    classMake->SetName("MA");
	classMake->SetDescription("这是关于MA指标的描述");
    classMake->AppendParameter("ma5", "int", "5", "5 Bar均线");
    classMake->AppendParameter("ma10", "int", "10", "10 Bar均线");
    classMake->AppendParameter("ma20", "int", "20", "20 Bar均线");
    classMake->AppendParameter("ma30", "int", "30", "30 Bar均线");
    classMake->AppendParameter("ma60", "int", "60", "60 Bar均线");

	String^ strRetParams = "";
	classMake->MakeParamsList(strRetParams);
	classMake->MakeCodeToFile("MA.cs");

	ParseParams::ParseIndicatorParams(strRetParams);
	
}

//void TestParse(String^ strStrategyParams)
//{
//	//String^ strParams = "<params><param type=\"int\" name=\"p1\">1</param><param type=\"double\" name=\"p2\">1.0</param></params>";
//}

void TestGenerateStrategy()
{
    String^ strCode = "";
    CStrategyClassMake^ classMake = gcnew CStrategyClassMake();
    classMake->SetTemplateFile("CStrategyTemplate.txt");
    classMake->SetNameEng("MATrend");
    classMake->SetNameChs("均线穿越");
	classMake->SetVerNum("1.0");
	classMake->SetDescription("这是关于MATrend策略的描述");

	List<CStrategyClassMake::CParamSubItem^>^ lstSubParams1 = gcnew List<CStrategyClassMake::CParamSubItem^>();
	lstSubParams1->Add(gcnew CStrategyClassMake::CParamSubItem("选择1", "北京"));
	lstSubParams1->Add(gcnew CStrategyClassMake::CParamSubItem("选择2", "上海"));
    
	classMake->AppendParameter("策略参数一", "p1", "int", "INOUT", "文本框",
                               false, "1", "", lstSubParams1);
	
	List<CStrategyClassMake::CParamSubItem^>^ lstSubParams2 = gcnew List<CStrategyClassMake::CParamSubItem^>();
	lstSubParams2->Add(gcnew CStrategyClassMake::CParamSubItem("选择1", "东京"));
	lstSubParams2->Add(gcnew CStrategyClassMake::CParamSubItem("选择2", "巴黎"));
	lstSubParams2->Add(gcnew CStrategyClassMake::CParamSubItem("选择3", "纽约"));

	classMake->AppendParameter("策略参数二", "p2", "double", "INOUT", "文本框",
                               false, "1.5", "", lstSubParams2);

	classMake->AppendIndicator("MA", "ma", 1, 1, 1, true, "5, 10, 20, 30, 60");
	classMake->AppendIndicator("MACD", "macd", 2, 1, 1, true, "12, 24, 1");

	String^ strRetParams = "";
	classMake->MakeParamsList(strRetParams);
	classMake->MakeCodeToFile("MATrend.cs");
	
	ParseParams::ParseStrategyParams(strRetParams);
}

int _tmain(int argc, _TCHAR* argv[])
{
	TestGenerateIndicator();
	TestGenerateIndicator2();
	TestGenerateStrategy();
	getchar();
	return 0;
}

