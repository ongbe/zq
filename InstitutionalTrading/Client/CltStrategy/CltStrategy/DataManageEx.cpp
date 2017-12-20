
#include "stdafx.h"
#include "DataManageEx.h"
#include "Tools.h"
#include "ToolsEx.h"
#include "Windows.h"

using namespace System;
using namespace System::Collections;
using namespace EnvDTE;
using namespace EnvDTE80;
using namespace EnvDTE90;
using namespace VSLangProj;
using namespace System::Text;
using namespace System::Reflection;
using namespace System::Diagnostics;
using namespace System::Collections::Generic;


CDataManageEx* CDataManageEx::m_pInstance = NULL;
int CDataManageEx::sm_nCount = 0;

CDataManageEx* CDataManageEx::NewInstance()
{
	sm_nCount++;
	if( m_pInstance == NULL ) 
	{
		m_pInstance = new CDataManageEx();
	}

	return m_pInstance;
}

void CDataManageEx::DestroyInstance()
{
	sm_nCount--;
	if(sm_nCount == 0 && m_pInstance != NULL) 
	{		
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

Project^ GetProjectByName(EnvDTE90::Solution3^ soln, String^ projName) 
{
	Project^ proj = nullptr;
	for(int i = 0; i<soln->Projects->Count; i++) {
		proj = soln->Projects->Item(i+1);
		if(proj->default == projName) {
			return proj;
		}
		else {
			for(int i = 0; i<proj->ProjectItems->Count; i++) {
				ProjectItem^ proj2 = proj->ProjectItems->Item(i+1);
				if(proj2->default == projName) {
					return (Project^)proj2->Object;
				}
			}
		}
	}
	return nullptr;
}

CDataManageEx::CDataManageEx() : 
	CDataManage()
{
	m_pMDApi = NULL;
	m_pTraderApi = NULL;
	m_pDTE = NULL;

	m_bIsConnectedQuot = false;
	m_bIsConnectedTrader = false;

	m_nLoginQuotStatus = conLogout;
	m_nLoginTraderStatus = conLogout;

	m_mutex = ::CreateMutex(NULL, FALSE, "");
}

CDataManageEx::~CDataManageEx()
{
	CloseHandle(m_mutex);
}

// 通过交易所ID获得相关的品种
bool CDataManageEx::GetProductID(const std::string& strExchangID, std::set<std::string>& setRetProductID)
{
	std::map<std::string, PlatformStru_InstrumentInfo>::iterator it;
	for(it = m_mapInstrument.begin(); it != m_mapInstrument.end(); it++) {
		if(strExchangID.compare(it->second.ExchangeID)==0) {
			setRetProductID.insert(it->second.ProductID);
		}
	}
	return true;
}

// 通过品种ID获得相关的合约信息
bool CDataManageEx::GetInstrument(const std::string& strProductID, 
		std::map<std::string, PlatformStru_InstrumentInfo>& mapRetInstrument)
{
	std::map<std::string, PlatformStru_InstrumentInfo>::iterator it;
	for(it = m_mapInstrument.begin(); it != m_mapInstrument.end(); it++) {
		if(strProductID.compare(it->second.ProductID)==0) {
			mapRetInstrument[it->first] = it->second;
		}
	}
	return true;
}

bool CDataManageEx::Init()
{
	DWORD dwRet = 0;
	char csIniFile[1024] = {0};
	char csTemp[1024] = {0};

	strncpy(csIniFile, _GetAddinDllFileName().c_str(), sizeof(csIniFile));
	csIniFile[strlen(csIniFile)-4] = 0;
	strcat(csIniFile, ".ini");

	ZeroMemory(csTemp, sizeof(csTemp));
	dwRet = ::GetPrivateProfileString("Server", "StrategyServer", "", csTemp, sizeof(csTemp)-1, csIniFile);

	m_pTraderApi = CStrategyTraderApi::CreateStrategyTraderApi();
	m_pTraderApi->RegisterFront(csTemp);
	m_pTraderApi->SetBuzSpi(this);
   	m_pTraderApi->Init();

	ZeroMemory(csTemp, sizeof(csTemp));
	dwRet = ::GetPrivateProfileString("Server", "QuotServer", "", csTemp, sizeof(csTemp)-1, csIniFile);
	m_pMDApi = CStrategyMdApi::CreateStrategyMdApi();
	m_pMDApi->RegisterFront(csTemp);
 	m_pMDApi->SetBuzSpi(this);
  	m_pMDApi->Init();

	return true;
}

void CDataManageEx::Release()
{
	m_pTraderApi->Release();
	//delete m_pTraderApi;
	m_pTraderApi = NULL;

	m_pMDApi->Release();
	//delete m_pMDApi;
	m_pMDApi = NULL;
}

bool CDataManageEx::Reload(const std::string& strProjectPath)
{
	std::vector<std::string> vecList;
	int i=0;

	vecList.clear();
    try { 
        StreamReader^ sr = gcnew StreamReader(
				gcnew String((strProjectPath + "indicators.txt").c_str()));
        String^ line = nullptr;
        while(line = sr->ReadLine()) {
			vecList.push_back(Tools::String2string(line));
        }
        sr->Close();
    }
    catch (Exception^ e) {
    }

	i = 0;
	for(int i=0; i<vecList.size(); i++) {
		std::string strPath(_GetProjectsPath());
		strPath += vecList[i];
		strPath += "\\params.xml";

		CIndicatorDataEx indicator;
		
		indicator.ImportFromXmlFile(strPath);
		indicator.SetIndicatorPrivate(1);
		//indicator.SetOwner(rsp.strTraderName);
		AddIndcator(indicator);
	}

	vecList.clear();
    try { 
        StreamReader^ sr = gcnew StreamReader(
				gcnew String((strProjectPath + "strategies.txt").c_str()));
        String^ line = nullptr;
        while(line = sr->ReadLine()) {
			vecList.push_back(Tools::String2string(line));
        }
        sr->Close();
    }
    catch (Exception^ e) {
    }

	i = 0;
	for(int i=0; i<vecList.size(); i++) {
		std::string strPath(_GetProjectsPath());
		strPath += vecList[i];
		strPath += "\\params.xml";

		CStrategyDataEx strategy;
		
		strategy.ImportFromXmlFile(strPath);
		strategy.SetStrategyPrivate(1);
		//strategy.SetOwner(rsp.strTraderName);
		AddStrategy(strategy);
	}

	return true;
}

bool CDataManageEx::SaveList(const std::string& strProjectPath)
{
	std::string strText;
	std::map<std::string, CIndicatorDataEx>::iterator itI;
	std::map<std::string, CStrategyDataEx>::iterator itS;
	int i=0;

	strText = "";
	i = 0;
	for(itI = m_mapIndicatorInfo.begin(); itI != m_mapIndicatorInfo.end(); itI++, i++) {
		strText += itI->first;
		if(i<m_mapIndicatorInfo.size()-1)
			strText += "\r\n";
	}

    try { 
        StreamWriter^ sw = gcnew StreamWriter(
				gcnew String((strProjectPath + "indicators.txt").c_str()));
		sw->Write(gcnew String(strText.c_str()));
        sw->Close();
    }
    catch (Exception^ e) {
    }

	strText = "";
	i = 0;
	for(itS = m_mapStrategyInfo.begin(); itS != m_mapStrategyInfo.end(); itS++, i++) {
		strText += itS->first;
		if(i<m_mapStrategyInfo.size()-1)
			strText += "\r\n";
	}

    try { 
        StreamWriter^ sw = gcnew StreamWriter(
				gcnew String((strProjectPath + "strategies.txt").c_str()));
		sw->Write(gcnew String(strText.c_str()));
        sw->Close();
    }
    catch (Exception^ e) {
    }

    return true;

}

void CDataManageEx::SubscribeEvent(UINT nEventID, HWND hWnd)
{
	std::map<UINT, set<HWND>>::iterator it;
	//LockMe();
	it = m_mapSubs.find(nEventID);
	if(it != m_mapSubs.end()) {
		it->second.insert(hWnd);
	}
	else {
		std::set<HWND> setHWnd;
		setHWnd.insert(hWnd);
		m_mapSubs.insert(std::make_pair(nEventID, setHWnd));
	}
	//UnLockMe();
}

void CDataManageEx::UnsubscribeEvent(UINT nEventID, HWND hWnd)
{
	std::map<UINT, set<HWND>>::iterator it;
	//LockMe();
	it = m_mapSubs.find(nEventID);
	if(it != m_mapSubs.end()) {
		it->second.erase(hWnd);
	}
	//UnLockMe();
}

void CDataManageEx::UnsubscribeAllEvent(HWND hWnd)
{
	std::map<UINT, set<HWND>>::iterator it;
	//LockMe();
	for(it = m_mapSubs.begin(); it != m_mapSubs.end(); it++) {
		it->second.erase(hWnd);
	}
	//UnLockMe();
}

bool CDataManageEx::PostEventToWnd(int nEventID, int nErrorNum)
{
	std::map<UINT, std::set<HWND>>::iterator it;
	std::set<HWND>::iterator itWnd;

	//LockMe();

	if((it = m_mapSubs.find(nEventID)) != m_mapSubs.end()) {
		for(itWnd = it->second.begin(); itWnd != it->second.end(); itWnd++) {
			if(IsWindow(*itWnd)) {
				PostMessage(*itWnd, _GetEventMsgID(), nEventID, (LPARAM)nErrorNum);
			}
		}
	}

	//UnLockMe();
	return true;
}

bool CDataManageEx::LoginQuot(const std::string& strUserID, const std::string& strPasswd)
{
	if(m_pMDApi==NULL)
		return false;

	m_nLoginQuotStatus = conLogon;
	CThostFtdcReqUserLoginField req = {0};
	strncpy(req.UserID, strUserID.c_str(), sizeof(req.UserID)-1);
	strncpy(req.Password, strPasswd.c_str(), sizeof(req.Password)-1);
	return m_pMDApi->ReqUserLogin(&req, 0)==0;
}

bool CDataManageEx::LoginTrader(const std::string& strUserID, const std::string& strPasswd)
{
	if(m_pTraderApi==NULL)
		return false;

	int nRet = m_pTraderApi->ReqUserLogin(strUserID, strPasswd, 0);
	if(nRet==0)
		m_nLoginTraderStatus = conLoginning;
	else
		m_nLoginTraderStatus = conLoginFail;
	return nRet==0;
}

bool CDataManageEx::LogoutQuot()
{
	return true;
}

bool CDataManageEx::LogoutTrader()
{
	if(m_pTraderApi==NULL)
		return false;

	m_pTraderApi->ReqUserLogout(0);
	m_nLoginTraderStatus = conLogout;
	return true;
}

bool CDataManageEx::ExportCodes(const std::set<std::string>& setStrategy, 
		const std::set<std::string>& setIndicator, const std::string& strExportFile)
{
	std::set<std::string>::const_iterator itIndi;
	std::set<std::string>::const_iterator itStra;
	std::vector<std::string> vecFiles;

	std::string strText;
	std::string strRetZipFile;
	std::string strWinRar(_GetSupportFilesPath());
	std::string strWorkPath(_GetProjectsPath());

	strText = "";
	for(itIndi = setIndicator.begin(); itIndi != setIndicator.end(); itIndi++) {
		vecFiles.push_back(*itIndi+".zip");
		strText += *itIndi;
		strText += "\r\n";
		CToolsEx::Zip_Project(strWinRar, strWorkPath, *itIndi, strRetZipFile);
	}
    try { 
        StreamWriter^ sw = gcnew StreamWriter(
				gcnew String((strWorkPath + "indicatorsExport.txt").c_str()));
		sw->Write(gcnew String(strText.c_str()));
        sw->Close();
    }
    catch (Exception^ e) {
    }

	strText = "";
	for(itStra = setStrategy.begin(); itStra != setStrategy.end(); itStra++) {
		vecFiles.push_back(*itStra+".zip");
		strText += *itStra;
		strText += "\r\n";
		CToolsEx::Zip_Project(strWinRar, strWorkPath, *itStra, strRetZipFile);
	}
    try { 
        StreamWriter^ sw = gcnew StreamWriter(
				gcnew String((strWorkPath + "StrategiesExport.txt").c_str()));
		sw->Write(gcnew String(strText.c_str()));
        sw->Close();
    }
    catch (Exception^ e) {
    }

	vecFiles.push_back("indicatorsExport.txt");
	vecFiles.push_back("StrategiesExport.txt");

	CToolsEx::Zip_Files(strWinRar, strWorkPath, vecFiles, strExportFile);
	
	return true;
}

bool CDataManageEx::ImportCodes(const std::string& strImportFile, bool bReplaceAll)
{
	std::string strText;
	std::string strRetZipFile;
	std::string strWinRar(_GetSupportFilesPath());
	std::string strWorkPath(_GetProjectsPath());
	std::vector<std::string> vecIndicator;
	std::vector<std::string> vecStrategy;
	int i=0;
	std::string strRet;

	CToolsEx::UnZip_File(strWinRar, strWorkPath, strImportFile);

	vecIndicator.clear();
    try { 
        StreamReader^ sr = gcnew StreamReader(
				gcnew String((strWorkPath + "indicatorsExport.txt").c_str()));
        String^ line = nullptr;
        while(line = sr->ReadLine()) {
			if(!String::IsNullOrEmpty(line))
				vecIndicator.push_back(Tools::String2string(line));
        }
        sr->Close();
    }
    catch (Exception^ e) {
    }

	vecStrategy.clear();
    try { 
        StreamReader^ sr = gcnew StreamReader(
				gcnew String((strWorkPath + "StrategiesExport.txt").c_str()));
        String^ line = nullptr;
        while(line = sr->ReadLine()) {
			if(!String::IsNullOrEmpty(line))
				vecStrategy.push_back(Tools::String2string(line));
        }
        sr->Close();
    }
    catch (Exception^ e) {
    }

	i = 0;
	for(int i=0; i<vecIndicator.size(); i++) {
		CToolsEx::UnZip_Project(_GetSupportFilesPath(), 
				_GetProjectsPath(), vecIndicator[i], strRet);
		Debug::WriteLine((gcnew String("Download Strategy = [")) 
				+ gcnew String(vecIndicator[i].c_str())
				+ "] ZIP, save then add to solution");

		CIndicatorDataEx indicator;
		std::string strPath(_GetProjectsPath());
		strPath += vecIndicator[i];
		strPath += "\\params.xml";
		
		indicator.ImportFromXmlFile(strPath);
		indicator.SetIndicatorPrivate(1);
		//indicator.SetOwner(rsp.strTraderName);
		AddIndcator(indicator);

		EnvDTE80::DTE2^ dte2 = (EnvDTE80::DTE2^)
				System::Runtime::InteropServices::Marshal::GetObjectForIUnknown(
				IntPtr(m_pDTE));
		String^ projectFile = (gcnew String(_GetProjectsPath().c_str())) 
				+ (gcnew String(vecIndicator[i].c_str()))
				+ "\\"
				+ (gcnew String(vecIndicator[i].c_str())) + ".csproj";
		EnvDTE90::Solution3^ soln = (EnvDTE90::Solution3^)
				dte2->Solution;
		Project^ proj = GetProjectByName(soln, 
				gcnew String(vecIndicator[i].c_str()));
		if(proj != nullptr) {
			SolutionFolder^ SF = (SolutionFolder^)proj->Object;
			SF->AddFromFile(projectFile);
		}
	}

	i = 0;
	for(int i=0; i<vecStrategy.size(); i++) {
		CToolsEx::UnZip_Project(_GetSupportFilesPath(), 
				_GetProjectsPath(), vecStrategy[i], strRet);
		Debug::WriteLine((gcnew String("Download Strategy = [")) 
				+ gcnew String(vecStrategy[i].c_str())
				+ "] ZIP, save then add to solution");
		
		CStrategyDataEx strategy;
		std::string strPath(_GetProjectsPath());
		strPath += vecStrategy[i];
		strPath += "\\params.xml";
		
		strategy.ImportFromXmlFile(strPath);
		strategy.SetStrategyPrivate(1);
		//strategy.SetOwner(rsp.strTraderName);
		AddStrategy(strategy);

		EnvDTE80::DTE2^ dte2 = (EnvDTE80::DTE2^)
				System::Runtime::InteropServices::Marshal::GetObjectForIUnknown(
				IntPtr(m_pDTE));
		String^ projectFile = (gcnew String(_GetProjectsPath().c_str())) 
				+ (gcnew String(vecStrategy[i].c_str()))
				+ "\\"
				+ (gcnew String(vecStrategy[i].c_str())) + ".csproj";
		EnvDTE90::Solution3^ soln = (EnvDTE90::Solution3^)
				dte2->Solution;
		Project^ proj = GetProjectByName(soln, "策略");
		if(proj != nullptr) {
			SolutionFolder^ SF = (SolutionFolder^)proj->Object;
			SF->AddFromFile(projectFile);
		}
	}

	return true;
}

bool CDataManageEx::GetProjectFiles(const std::string& strPath, 
									const std::set<std::string>& setProjects, 
									std::set<std::string>& setFiles)
{
	std::set<std::string>::const_iterator it;
	
	for(it = setProjects.begin(); it != setProjects.end(); it++) {
		std::string strNewPath(strPath);
		strNewPath += *it;
		strNewPath += "\\";
		std::string strStdPath(*it);
		strStdPath += "\\";
		CToolsEx::SearchFiles(strNewPath, strStdPath, setFiles);
	}
	return true;
}

bool CDataManageEx::GetStrategyInstance(const std::string& strStrategy, SStrategyInstance& instance)
{
	std::map<std::string, SStrategyInstance>::iterator it = m_mapStrategyInstances.find(strStrategy);
	if(it != m_mapStrategyInstances.end()) {
		instance = it->second;
		return true;
	}
	else
		return false;
}

bool CDataManageEx::ResetStrategyInstance(const std::string& strStrategy, SStrategyInstance& instance)
{
	m_mapStrategyInstances[strStrategy] = instance;
	return true;
}

bool CDataManageEx::DeleteStrategyInstance(const std::string& strStrategy)
{
	std::map<std::string, SStrategyInstance>::iterator it = m_mapStrategyInstances.find(strStrategy);
	if(it != m_mapStrategyInstances.end()) {
		m_mapStrategyInstances.erase(it);
		return true;
	}
	else
		return false;
}

bool CDataManageEx::GetExchangeID(const std::string& strInstrumentID, std::string& strExchangeID)
{
	std::map<std::string, PlatformStru_InstrumentInfo>::iterator it = m_mapInstrument.find(strInstrumentID);
	if(it != m_mapInstrument.end()) {
		strExchangeID = it->second.ExchangeID;
		return true;
	}
	else
		return false;
}

bool CDataManageEx::GetProductID(const std::string& strInstrumentID, std::string& strProductID)
{
	std::map<std::string, PlatformStru_InstrumentInfo>::iterator it = m_mapInstrument.find(strInstrumentID);
	if(it != m_mapInstrument.end()) {
		strProductID = it->second.ProductID;
		return true;
	}
	else
		return false;
}

void CDataManageEx::ClearDownloadDatas()
{
	m_mapDownloadIndicators.clear();
	m_mapDownloadStrategies.clear();
	m_mapStrategyInstances.clear();
}

void CDataManageEx::OnQuotConnected()
{
	m_bIsConnectedQuot = true;
}

void CDataManageEx::OnQuotDisconnected()
{
	m_bIsConnectedQuot = false;
	m_nLoginQuotStatus = conLogout;
}

void CDataManageEx::OnTraderConnected()
{
	m_bIsConnectedTrader = true;
}

void CDataManageEx::OnTraderDisconnected()
{
	m_bIsConnectedTrader = false;
	m_nLoginTraderStatus = conLogout;
}

void CDataManageEx::OnQuotLogin(const sTraderLoginRsp& rsp, int nErrorNum, const std::string& strErrorMsg)
{
}

void CDataManageEx::OnTraderLogin(const sTraderLoginRsp& rsp, int nErrorNum, const std::string& strErrorMsg)
{
	m_strLoginTraderFailMsg = strErrorMsg;
	if(nErrorNum==0) {
		ClearDownloadDatas();
		m_nLoginTraderStatus = conLogon;
		if(m_pTraderApi!=NULL) {
			if(m_mapInstrument.size()==0) {
				PlatformStru_QryInstrument qry;
				m_pTraderApi->ReqQryInstrument(&qry, 0);
			}
			m_pTraderApi->ReqDownloadIndicatorList(0);
			m_pTraderApi->ReqDownloadStategyList(0);
		}
	}
	else
		m_nLoginTraderStatus = conLoginFail;
	PostEventToWnd(conTraderLogin);
}

void CDataManageEx::OnInstrument(const std::vector<PlatformStru_InstrumentInfo>& vec, int nErrorNum)
{
	char csText[256] = {0};
	char* ppInstrumentID[] = { (char*)&csText };
	for(int i=0; i<vec.size(); i++) {
		m_mapInstrument[vec[i].InstrumentID] = vec[i];
		if(m_pMDApi != NULL) {
			strncpy(csText, vec[i].InstrumentID, sizeof(csText)-1);
			m_pMDApi->SubscribeMarketData(ppInstrumentID, 1);
		}
	}
}

void CDataManageEx::OnSubscribeQuot(const std::string& strInstrumentID, int nErrorNum)
{
}

void CDataManageEx::OnUnSubscribeQuot(const std::string& strInstrumentID, int nErrorNum)
{
}

void CDataManageEx::OnDepthMarketData(const PlatformStru_DepthMarketData& rsp, int nErrorNum)
{
	m_mapMD[rsp.InstrumentID] = rsp;
	PostEventToWnd(conDepthMarketData);
}

void CDataManageEx::OnOrder(const PlatformStru_OrderInfo& rsp, int nErrorNum)
{
	m_mapOrders[OrderKey(rsp)] = rsp;
	PostEventToWnd(conOrder);
}

void CDataManageEx::OnTrade(const PlatformStru_TradeInfo& rsp, int nErrorNum)
{
	m_mapTrades[TradeKey(rsp)] = rsp;
	PostEventToWnd(conTrade);
}

void CDataManageEx::OnOrderInsert(const PlatformStru_InputOrder& rsp, int nErrorNum)
{
	PostEventToWnd(conOrderInsert);
}

void CDataManageEx::OnOrderError(const PlatformStru_InputOrder& rsp, int nErrorNum)
{
	PostEventToWnd(conOrderError);
}

void CDataManageEx::OnOrderActionFail(const PlatformStru_InputOrderAction& rsp, int nErrorNum)
{
	PostEventToWnd(conOrderActionFail);
}

void CDataManageEx::OnOrderActionError(const CThostFtdcOrderActionField& rsp, int nErrorNum)
{
	PostEventToWnd(conOrderActionError);
}

void CDataManageEx::OnOrders(const std::vector<PlatformStru_OrderInfo>& vec, int nErrorNum)
{
	for(int i=0; i<vec.size(); i++) {
		m_mapOrders[OrderKey(vec[i])] = vec[i];
	}
	PostEventToWnd(conQryOrders);
}

void CDataManageEx::OnTrades(const std::vector<PlatformStru_TradeInfo>& vec, int nErrorNum)
{
	for(int i=0; i<vec.size(); i++) {
		m_mapTrades[TradeKey(vec[i])] = vec[i];
	}
	PostEventToWnd(conQryTrades);
}

void CDataManageEx::OnTradingAccount(const PlatformStru_TradingAccountInfo& rsp, int nErrorNum)
{
	m_tradingAccount = rsp;
	PostEventToWnd(conQryTradingAccount);
}

void CDataManageEx::OnPositions(const std::vector<PlatformStru_Position>& vec, int nErrorNum)
{
	for(int i=0; i<vec.size(); i++) {
		m_mapPositions[PositionKey(vec[i])] = vec[i];
	}
	PostEventToWnd(conQryPositions);
}

void CDataManageEx::OnPositionDetails(const std::vector<PlatformStru_PositionDetail>& vec, int nErrorNum)
{
}

void CDataManageEx::OnPositionDetailCombs(const std::vector<PlatformStru_PositionDetailComb>& vec, int nErrorNum)
{
}

void CDataManageEx::OnCommissionRate(const PlatformStru_InstrumentCommissionRate& rsp, int nErrorNum)
{
}

void CDataManageEx::OnMarginRate(const PlatformStru_InstrumentMarginRate& rsp, int nErrorNum)
{
}

void CDataManageEx::OnUploadStartRsp(int nErrorNum, const std::string& strMsg)
{
	if(nErrorNum!=0) 
		_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
				CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, strMsg.c_str());
	PostEventToWnd(conUploadStartRsp, nErrorNum);
}

void CDataManageEx::OnUploadEndRsp(int nErrorNum, const std::string& strMsg)
{
	if(nErrorNum!=0) 
		_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
				CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, strMsg.c_str());
	PostEventToWnd(conUploadEndRsp, nErrorNum);
}

void CDataManageEx::OnUploadIndicatorInfoRsp(int nErrorNum, const std::string& strMsg)
{
	if(nErrorNum!=0) 
		_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
				CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, strMsg.c_str());
	PostEventToWnd(conUploadIndicatorInfoRsp, nErrorNum);
}

void CDataManageEx::OnUploadIndicatorFileRsp(int nErrorNum, int nFileType, const std::string& strMsg)
{
	if(nErrorNum==0) {
		if(nFileType==2)
			_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
					CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, "指标DLL文件上传成功");
		else
			_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
					CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, "指标源程序上传成功");
	}
	else {
		_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
				CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, strMsg.c_str());
	}
	PostEventToWnd(conUploadIndicatorFileRsp);
}

void CDataManageEx::OnUploadStrategyInfoRsp(int nErrorNum, const std::string& strMsg)
{
	if(nErrorNum!=0) 
		_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
				CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, strMsg.c_str());
	else
		PostEventToWnd(conUploadStrategyInfoRsp);
}

void CDataManageEx::OnUploadStrategyFileRsp(int nErrorNum, int nFileType, const std::string& strMsg)
{
	PostEventToWnd(conUploadStrategyFileRsp);
	if(nErrorNum==0) {
		if(nFileType==2)
			_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
					CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, "策略DLL文件上传成功");
		else
			_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
					CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, "策略源程序上传成功");
	}
	else {
		_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
				CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, strMsg.c_str());
	}
}

void CDataManageEx::OnDownloadIndicatorListRsp(const std::vector<SIndex>& vec, int nErrorNum, const std::string& strMsg)
{
	if(nErrorNum==0) {
		LockObject();
		m_mapDownloadIndicators.clear();
		for(int i=0; i<vec.size(); i++) {
			CIndicatorDataEx indicator;
			
			indicator.ImportFromXml(std::string(vec[i].strPara));
			indicator.SetOwner(vec[i].strTraderName);
			indicator.SetIndicatorPrivate(vec[i].nPrivate);
			//m_mapDownloadIndicators[vec[i].IndexName] = indicator;
			AddDownloadIndicator(indicator);
		}
		UnLockObject();
	}
	PostEventToWnd(conDownloadIndicatorListRsp);
}

void CDataManageEx::OnDownloadIndicatorInfoRsp(const SIndex rsp, int nErrorNum, const std::string& strMsg)
{
	if(nErrorNum==0) {
		//if(rsp.nPrivate!=0 && IsIndicatorExist(rsp.IndexName))
		//	return;
		
		LockObject();

		CIndicatorDataEx indicator;
		
		indicator.ImportFromXml(std::string(rsp.strPara));
		indicator.SetOwner(rsp.strTraderName);
		indicator.SetIndicatorPrivate(rsp.nPrivate);
		AddDownloadIndicator(indicator);
		{
		std::string strFile(_GetShareDllsPath());
		strFile += rsp.IndexName;
		strFile += ".xml";
		HANDLE hFile = CreateFile(strFile.c_str(),				// file to open
						   GENERIC_WRITE,						// open for reading
						   0,									// share for reading
						   NULL,								// default security
						   CREATE_ALWAYS,						// existing file only
						   FILE_ATTRIBUTE_NORMAL,				// normal file
						   NULL);								// no attr. template
		if(hFile!=INVALID_HANDLE_VALUE) {
			DWORD dwRealWrite = 0;
			::WriteFile(hFile, rsp.strPara, strlen(rsp.strPara), &dwRealWrite, NULL);
			::CloseHandle(hFile);
		}
		}

		UnLockObject();
	}
	PostEventToWnd(conDownloadIndicatorInfoRsp);
}

void CDataManageEx::OnDownloadIndicatorFileRsp(const std::string& strIndicatorID, int nFileType, LPVOID pFileData, int nFileLen, int nErrorNum, const std::string& strMsg)
{
	if(nErrorNum==0) {
		LockObject();

		std::string strFile;
		bool bIsOwner = IsOwner_Indicator(strIndicatorID, _GetCurrUserID());
		
		if(nFileType==2) {
			strFile = _GetShareDllsPath();
			strFile += strIndicatorID;
			strFile += ".dll";
		}
		else {
			strFile = _GetProjectsPath();
			strFile += strIndicatorID;
			strFile += ".zip";
		}
		HANDLE hFile = CreateFile(strFile.c_str(),				// file to open
						   GENERIC_WRITE,						// open for reading
						   0,									// share for reading
						   NULL,								// default security
						   CREATE_ALWAYS,						// existing file only
						   FILE_ATTRIBUTE_NORMAL,				// normal file
						   NULL);								// no attr. template
		if(hFile!=INVALID_HANDLE_VALUE) {
			DWORD dwRealWrite = 0;
			::WriteFile(hFile, pFileData, nFileLen, &dwRealWrite, NULL);
			::CloseHandle(hFile);
			if(nFileType==2) {
				if(!bIsOwner) {

					m_mapDownloadIndicatorsNext.insert(std::make_pair((std::string&)strIndicatorID, false));

					Debug::WriteLine((gcnew String("Download Indicator = [")) 
							+ gcnew String(strIndicatorID.c_str())
							+ "] DLL, save then add to solution");

				}
			}
			else {
				std::string strRet;
				CToolsEx::UnZip_Project(_GetSupportFilesPath(), _GetProjectsPath(), 
						strIndicatorID, strRet);
				if(bIsOwner) {

					CIndicatorDataEx indicator;
					GetDownloadIndicator(strIndicatorID, indicator);
					indicator.SetOwner(std::string(""));
					AddIndcator(indicator);

					m_mapDownloadIndicatorsNext.insert(std::make_pair((std::string&)strIndicatorID, true));

					Debug::WriteLine((gcnew String("Download Indicator = [")) 
							+ gcnew String(strIndicatorID.c_str())
							+ "] ZIP, save then add to solution");

				}
			}
		}

		UnLockObject();
	}
	PostEventToWnd(conDownloadIndicatorFileRsp);
	if(nErrorNum==0) {
		if(nFileType==2)
			_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
					CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, "指标DLL文件下载成功");
		else
			_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
					CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, "指标源程序下载成功");
	}
	else {
		_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
				CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, strMsg.c_str());
	}
}

void CDataManageEx::OnDownloadStrategyListRsp(const std::vector<SStrategy>& vec, int nErrorNum, const std::string& strMsg)
{
	if(nErrorNum==0) {
		LockObject();
		m_mapDownloadStrategies.clear();
		for(int i=0; i<vec.size(); i++) {
			CStrategyDataEx strategy;
			
			strategy.ImportFromXml(std::string(vec[i].strPara));
			strategy.SetOwner(vec[i].strTraderName);
			strategy.SetStrategyPrivate(vec[i].nPrivate);
			strategy.SetServerUseStatus(vec[i].nUse);
			//m_mapDownloadStrategies[vec[i].strategyName] = strategy;
			AddDownloadStrategy(strategy);
		}
		UnLockObject();
	}
	PostEventToWnd(conDownloadStrategyListRsp);
}

void CDataManageEx::OnDownloadStrategyInfoRsp(const SStrategy& rsp, int nErrorNum, const std::string& strMsg)
{
	if(nErrorNum==0) {
		//if(rsp.nPrivate!=0 && IsInStrategyExist(rsp.strategyName))
		//	return;
			
		LockObject();

		CStrategyDataEx strategy;
		
		strategy.ImportFromXml(std::string(rsp.strPara));
		strategy.SetOwner(rsp.strTraderName);
		strategy.SetStrategyPrivate(rsp.nPrivate);
		strategy.SetServerUseStatus(rsp.nUse);
		AddDownloadStrategy(strategy);
		{
		std::string strFile(_GetShareDllsPath());
		strFile += rsp.strategyName;
		strFile += ".xml";
		HANDLE hFile = CreateFile(strFile.c_str(),				// file to open
						   GENERIC_WRITE,						// open for reading
						   0,									// share for reading
						   NULL,								// default security
						   CREATE_ALWAYS,						// existing file only
						   FILE_ATTRIBUTE_NORMAL,				// normal file
						   NULL);								// no attr. template
		if(hFile!=INVALID_HANDLE_VALUE) {
			DWORD dwRealWrite = 0;
			::WriteFile(hFile, rsp.strPara, strlen(rsp.strPara), &dwRealWrite, NULL);
			::CloseHandle(hFile);
		}
		}

		UnLockObject();
	}
	PostEventToWnd(conDownloadStrategyInfoRsp);
}

void CDataManageEx::OnDownloadStrategyFileRsp(const std::string& strStrategyID, int nFileType, LPVOID pFileData, int nFileLen, int nErrorNum, const std::string& strMsg)
{
	if(nErrorNum==0) {
		LockObject();

		std::string strFile;
		bool bIsOwner = IsOwner_Strategy(strStrategyID, _GetCurrUserID());
		
		if(nFileType==2) {
			strFile = _GetShareDllsPath();
			strFile += strStrategyID;
			strFile += ".dll";
		}
		else {
			strFile = _GetProjectsPath();
			strFile += strStrategyID;
			strFile += ".zip";
		}
		HANDLE hFile = CreateFile(strFile.c_str(),				// file to open
						   GENERIC_WRITE,						// open for reading
						   0,									// share for reading
						   NULL,								// default security
						   CREATE_ALWAYS,						// existing file only
						   FILE_ATTRIBUTE_NORMAL,				// normal file
						   NULL);								// no attr. template
		if(hFile!=INVALID_HANDLE_VALUE) {
			DWORD dwRealWrite = 0;
			::WriteFile(hFile, pFileData, nFileLen, &dwRealWrite, NULL);
			::CloseHandle(hFile);
			if(nFileType==2) {
				if(!bIsOwner) {

					m_mapDownloadStrategiesNext.insert(std::make_pair((std::string&)strStrategyID, false));

					Debug::WriteLine((gcnew String("Download Strategy = [")) 
							+ gcnew String(strStrategyID.c_str())
							+ "] DLL, save then add to solution");

				}
			}
			else {
				std::string strRet;
				CToolsEx::UnZip_Project(_GetSupportFilesPath(), 
						_GetProjectsPath(), strStrategyID, strRet);
				if(bIsOwner) {
					
					CStrategyDataEx strategy;
					GetDownloadStrategy(strStrategyID, strategy);
					strategy.SetOwner(std::string(""));
					AddStrategy(strategy);

					m_mapDownloadStrategiesNext.insert(std::make_pair((std::string&)strStrategyID, true));
					
					Debug::WriteLine((gcnew String("Download Strategy = [")) 
							+ gcnew String(strStrategyID.c_str())
							+ "] ZIP, save then add to solution");

				}
			}
		}
		UnLockObject();
	}

	PostEventToWnd(conDownloadStrategyFileRsp);
	if(nErrorNum==0) {
		if(nFileType==2)
			_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
					CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, "策略DLL文件下载成功");
		else
			_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
					CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, "策略源程序下载成功");
	}
	else {
		_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
				CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, strMsg.c_str());
	}
}

void CDataManageEx::OnDownloadFileNext()
{
	std::map<std::string, bool>::iterator itIndi;
	std::map<std::string, bool>::iterator itStra;

	LockObject();

	for(itIndi = m_mapDownloadIndicatorsNext.begin(); 
			itIndi != m_mapDownloadIndicatorsNext.end(); itIndi++) {

		std::string strIndicatorID(itIndi->first);
		CIndicatorDataEx indicator;
		GetDownloadIndicator(strIndicatorID, indicator);

		EnvDTE80::DTE2^ dte2 = (EnvDTE80::DTE2^)
				System::Runtime::InteropServices::Marshal::GetObjectForIUnknown(
				IntPtr(m_pDTE));

		if(itIndi->second) {

			String^ projectFile = (gcnew String(_GetProjectsPath().c_str())) 
					+ (gcnew String(strIndicatorID.c_str())) 
					+ "\\"
					+ (gcnew String(strIndicatorID.c_str())) + ".csproj";
			EnvDTE90::Solution3^ soln = (EnvDTE90::Solution3^)
					dte2->Solution;
			Project^ proj = GetProjectByName(soln, 
					gcnew String(indicator.GetIndicatorFile().c_str()));
			if(proj != nullptr) {
				SolutionFolder^ SF = (SolutionFolder^)proj->Object;
				try {
				SF->AddFromFile(projectFile);
				} catch(Exception^ e) {
					_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
							CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
							Tools::String2string(e->ToString()).c_str());
				}
			}
		}
		else {

			String^ projectFile = (gcnew String(_GetShareDllsPath().c_str())) 
					+ (gcnew String(strIndicatorID.c_str())) + ".dll";
			String^ projectXmlFile = (gcnew String(_GetShareDllsPath().c_str())) 
					+ (gcnew String(strIndicatorID.c_str())) + ".xml";
			EnvDTE90::Solution3^ soln = (EnvDTE90::Solution3^)dte2->Solution;
			//ProjectItem^ itemFind = soln->FindProjectItem("策略");

			Project^ proj = GetProjectByName(soln, 
					gcnew String(indicator.GetIndicatorFile().c_str()));
			if(proj != nullptr) {
				try {
				proj->ProjectItems->AddFromFile(projectFile);
				proj->ProjectItems->AddFromFile(projectXmlFile);
				} catch(Exception^ e) {
					_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
							CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
							Tools::String2string(e->ToString()).c_str());
				}
			}
		}
	}

	for(itStra = m_mapDownloadStrategiesNext.begin(); itStra != m_mapDownloadStrategiesNext.end(); itStra++) {

		std::string strStrategyID(itStra->first);
		CStrategyDataEx strategy;
		GetDownloadStrategy(itStra->first, strategy);

		EnvDTE80::DTE2^ dte2 = (EnvDTE80::DTE2^)
				System::Runtime::InteropServices::Marshal::GetObjectForIUnknown(
				IntPtr(m_pDTE));

		if(itStra->second) {

			String^ projectFile = (gcnew String(_GetProjectsPath().c_str())) 
					+ (gcnew String(strStrategyID.c_str()))
					+ "\\"
					+ (gcnew String(strStrategyID.c_str())) + ".csproj";
			EnvDTE90::Solution3^ soln = (EnvDTE90::Solution3^)
					dte2->Solution;
			Project^ proj = GetProjectByName(soln, "策略");
			if(proj != nullptr) {
				SolutionFolder^ SF = (SolutionFolder^)proj->Object;
				try {
				SF->AddFromFile(projectFile);
				} catch(Exception^ e) {
					_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
							CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
							Tools::String2string(e->ToString()).c_str());
				}
			}
		}
		else {

			String^ projectFile = (gcnew String(_GetShareDllsPath().c_str())) 
					+ (gcnew String(strStrategyID.c_str())) + ".dll";
			String^ projectXmlFile = (gcnew String(_GetShareDllsPath().c_str())) 
					+ (gcnew String(strStrategyID.c_str())) + ".xml";
			EnvDTE90::Solution3^ soln = (EnvDTE90::Solution3^)
					dte2->Solution;
			Project^ proj = GetProjectByName(soln, "策略");
			if(proj != nullptr) {
				try {
				proj->ProjectItems->AddFromFile(projectFile);
				proj->ProjectItems->AddFromFile(projectXmlFile);
				} catch(Exception^ e) {
					_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
							CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
							Tools::String2string(e->ToString()).c_str());
				}
			}
		}
	}

	m_mapDownloadIndicatorsNext.clear();
	m_mapDownloadStrategiesNext.clear();

	UnLockObject();
}

void CDataManageEx::OnResetStrategyUseStatusRsp(int nErrorNum, const std::string& strMsg)
{
	PostEventToWnd(conResetStrategyUseStatusRsp);
}

void CDataManageEx::OnDeleteStrategyRsp(int nErrorNum, const std::string& strMsg)
{
	PostEventToWnd(conDeleteStrategyRsp);
}

void CDataManageEx::OnDownloadStrategyInstanceListRsp(const std::vector<SStrategyInstance>& vec, int nErrorNum, const std::string& strMsg)
{
	LockObject();
	m_mapStrategyInstances.clear();
	for(int i=0; i<vec.size(); i++) {
		m_mapStrategyInstances[vec[i].strategyName] = vec[i];
	}
	UnLockObject();
	PostEventToWnd(conDownloadStrategyInstanceListRsp);
}

void CDataManageEx::OnStrategyInstanceAddRsp(int nErrorNum, const std::string& strMsg)
{
	PostEventToWnd(conStrategyInstanceAddRsp);
}

void CDataManageEx::OnStrategyInstanceResetRsp(int nErrorNum, const std::string& strMsg)
{
	PostEventToWnd(conStrategyInstanceResetRsp);
}

void CDataManageEx::OnStrategyInstanceDeleteRsp(int nErrorNum, const std::string& strMsg)
{
	PostEventToWnd(conStrategyInstanceDeleteRsp);
}

void CDataManageEx::OnStrategyInstanceRunStopRsp(int nErrorNum, const std::string& strMsg)
{
	PostEventToWnd(conStrategyInstanceRunStopRsp);
}
