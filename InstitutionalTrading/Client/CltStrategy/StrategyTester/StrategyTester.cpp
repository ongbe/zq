// StrategyTester.cpp : main project file.

#include "stdafx.h"
#include "FormMain.h"
#include "DataManageEx.h"
#include "TestManager.h"
#include "TestRecordForm.h"

using namespace StrategyTester;
using namespace System::Xml;


bool ParseXml(String^ strXml);
void LoadIniFile(const std::string& strIniFile);


[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// 参数格式
	// <tester strategyid='MATrend' strategypath='E:\\strategy\\Projects\\MATrend\\' 
	//		runpath='E:\\strategy\\Temp\\MATrend\\' 
	//		traderid='888003' password='888888' inifile='CltStrategy.ini' />

#if _DEBUG
 	//::MessageBox(NULL, "StrategyTester.exe", "提示", 0);
#endif

	if(args->Length!=2) {
		::MessageBox(NULL, "args->Length!=2", "错误", 0);
		return -1;
	}

	int nMode = Int32::Parse(args[0]);
	CDataManageEx* pDataService = NULL;

	if(!ParseXml(args[1])) {
		::MessageBox(NULL, "ParseXml(args[1])==FALSE", "错误", 0);
		return -1;
	}

	switch(nMode) {
	case 1:		// 进行策略测试
		{
		pDataService = CDataManageEx::NewInstance();
		LoadIniFile(_GetIniFile());
		pDataService->Init();
		pDataService->LoginTrader(_GetCurrUserID(), _GetPassword());
		pDataService->LoginQuot(_GetCurrUserID(), _GetPassword());

		// Create the main window and run it
		Application::Run(gcnew FormMain());
        //Application::Run(gcnew TestRecordForm(true));

		CDataManageEx::DestroyInstance();
		if(CTestManager::sm_pTestEntity != NULL) {
			CTestEntity* pTestEntity = CTestManager::sm_pTestEntity;
			CTestManager::sm_pTestEntity = NULL;

			CDataEntity* pDataEntity = pTestEntity->GetDataEntity();
			pTestEntity->Release();
			//pTestEntity->SetDataEntity(NULL);
			delete pTestEntity;
			delete pDataEntity;
		}
		}
		break;
	case 2:		// 展示历史测试结果
		{
		DWORD dwRet = 0;
		char csTemp[1024] = {0};

		ZeroMemory(csTemp, sizeof(csTemp));
		dwRet = ::GetPrivateProfileString("RunPath", "StrategyPath", "", 
				csTemp, sizeof(csTemp)-1, _GetIniFile().c_str());
		if(dwRet>0 && csTemp[dwRet-1]!='\\')
			strcat(csTemp, "\\");
		_SetHistoryDataPath(std::string(csTemp)+"HistoryDatas\\");
		// Create the main window and run it
		// 这里用展示历史测试结果的窗体类代替
		Application::Run(gcnew TestRecordForm(true));
		}
		break;
	default:
		break;
	};

	return 0;
}

bool ParseXml(String^ strXml)
{
	String^ valtype = nullptr;
	String^ valtext = nullptr;
	String^ valdefault = nullptr;
    XmlDocument^ doc = gcnew XmlDocument();
    doc->LoadXml(strXml);

    XmlNode^ nodeRoot = doc->SelectSingleNode("tester");

    valtext = ((XmlElement^)nodeRoot)->GetAttribute("strategyid");
	if(valtext!=nullptr)
		_SetStrategyID(Tools::String2string(valtext));
    valtext = ((XmlElement^)nodeRoot)->GetAttribute("traderid");
	if(valtext!=nullptr)
		_SetCurrUserID(Tools::String2string(valtext));
    valtext = ((XmlElement^)nodeRoot)->GetAttribute("password");
	if(valtext!=nullptr)
		_SetPassword(Tools::String2string(valtext));
    valtext = ((XmlElement^)nodeRoot)->GetAttribute("strategypath");
	if(valtext!=nullptr)
		_SetStategyPath(Tools::String2string(valtext));
    valtext = ((XmlElement^)nodeRoot)->GetAttribute("runpath");
	if(valtext!=nullptr)
		_SetRunPath(Tools::String2string(valtext));
    valtext = ((XmlElement^)nodeRoot)->GetAttribute("historypath");
	if(valtext!=nullptr)
		_SetHistoryDataPath(Tools::String2string(valtext));
    valtext = ((XmlElement^)nodeRoot)->GetAttribute("inifile");
	if(valtext!=nullptr)
		_SetIniFile(Tools::String2string(valtext));

	return true;
}

void LoadIniFile(const std::string& strIniFile)
{
	DWORD dwRet = 0;
	int i=0;
	char csTemp[1024] = {0};

	ZeroMemory(csTemp, sizeof(csTemp));
	dwRet = ::GetPrivateProfileString("Server", "StrategyServer", "", csTemp, sizeof(csTemp)-1, strIniFile.c_str());
	_SetStrategyServerAddr(std::string(csTemp));
	dwRet = ::GetPrivateProfileString("Server", "QuotServer", "", csTemp, sizeof(csTemp)-1, strIniFile.c_str());
	_SetQuotServerAddr(std::string(csTemp));
	dwRet = ::GetPrivateProfileString("Server", "RedisServer", "", csTemp, sizeof(csTemp)-1, strIniFile.c_str());
	_SetRedisServerAddr(std::string(csTemp));
	dwRet = ::GetPrivateProfileInt("StrategyTest", "QuoteDelay", 30, strIniFile.c_str());
	_SetStrategyTestDelay(dwRet);
}
