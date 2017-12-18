// Global.cpp : 只包括标准包含文件的源文件
// Module-BookOrder.pch 将作为预编译头
// Global.obj 将包含预编译类型信息

#include "stdafx.h"
#include "Global.h"
#include "tinyxml\tinyxml.h"
#include <string>

using namespace std;

HANDLE g_hMutex = CreateMutex(NULL, FALSE, NULL);

BOOL LockGlobalObject()
{
	::WaitForSingleObject(g_hMutex, INFINITE);
	return TRUE;
}

BOOL UnlockGlobalObject()
{
	::ReleaseMutex(g_hMutex);
	return TRUE;
}

UINT g_nColSetChangedMSGID = ::RegisterWindowMessage("OptionPriceListT Cols Set Changed");

UINT GetColSetChangedMSGID()
{
	return g_nColSetChangedMSGID;
}

std::map<std::string, PlatformStru_InstrumentInfo> g_mapInstrumentInfo;

std::map<std::string, PlatformStru_InstrumentInfo>& GetInstrumentInfos()
{
	return g_mapInstrumentInfo;
}

//std::map<std::string, std::map<std::string, std::set<std::string>>> g_mapOptionTargetRelate;
//std::map<std::string, std::string> g_mapOption2Target;
//
//void ParseOptionInstrument(std::map<std::string, PlatformStru_InstrumentInfo>& mapInstrument)
//{
//	std::map<std::string, PlatformStru_InstrumentInfo>::iterator it;
//	std::map<std::string, std::set<std::string>>::iterator itPrice;
//	std::map<std::string, std::map<std::string, std::set<std::string>>>::iterator itTarg;
//	CString strExchangeID, strTargetID, strPrice;
//	BOOL bDirect = FALSE;
//	for(it = mapInstrument.begin(); it != mapInstrument.end(); it++) {
//		if(it->second.ProductClass==THOST_FTDC_PC_Options) {
//			strExchangeID = it->second.ExchangeID;
//			if(ParseOptionFormat(CString(it->second.InstrumentID), strExchangeID, strTargetID, bDirect, strPrice)) {
//				if(stricmp(it->second.ExchangeID, "CFFEX")==0) {
//					strTargetID.Replace("IO", "IF");
//					strTargetID.Replace("HO", "IH");
//				}
//				g_mapOption2Target.insert(std::make_pair(it->second.InstrumentID, std::string(strTargetID)));
//				itTarg = g_mapOptionTargetRelate.find(strTargetID.GetBuffer(0));
//				if(itTarg != g_mapOptionTargetRelate.end()) {
//					itPrice = itTarg->second.find(strPrice.GetBuffer(0));
//					if(itPrice != itTarg->second.end()) {
//						itPrice->second.insert(it->second.InstrumentID);
//					}
//					else {
//						std::set<std::string> setOption;
//						setOption.insert(it->second.InstrumentID);
//						itTarg->second.insert(std::make_pair(strPrice, setOption));
//					}
//				}
//				else {
//					std::set<std::string> setOption;
//					std::map<std::string, std::set<std::string>> mapPrice;
//					setOption.insert(it->second.InstrumentID);
//					mapPrice.insert(std::make_pair(strPrice, setOption));
//					g_mapOptionTargetRelate.insert(std::make_pair(strTargetID, mapPrice));
//				}
//			}
//		}
//	}
//}
//
//std::map<std::string, std::map<std::string, std::set<std::string>>>& GetOptionTargetRelate()
//{
//	return g_mapOptionTargetRelate;
//}
//
//BOOL FindTargetID(const CString& strOptionID, CString& strTargetID) 
//{
//	std::map<std::string, std::string>::iterator it = g_mapOption2Target.find(std::string(strOptionID));
//	if(it != g_mapOption2Target.end()) {
//		strTargetID = it->second.c_str();
//		return TRUE;
//	}
//	else {
//		return FALSE;
//	}
//}

CString GetIniFile()
{
	char strPath[1024];
	ZeroMemory(strPath, sizeof(strPath));
	DWORD dwRet = 0;
	dwRet = ::GetModuleFileName(NULL, strPath, 1023);
	if(dwRet<4)
		return CString("");
	strPath[dwRet-4] = '\000';
	CString strIniFile(strPath);
	strIniFile += ".INI";
	return strIniFile;
}

CString GetXmlFile()
{
	int nPathLen = 0;
	char strPath[1024];
	ZeroMemory(strPath, sizeof(strPath));
	nPathLen = GetModuleFileName(NULL, strPath, 1023);
	string filename=strPath;
	size_t splitpos=filename.find_last_of('\\');
	string strXmlFile(filename.substr(0, splitpos+1).c_str());
	strXmlFile += "grid.xml";
	return strXmlFile.c_str();
}

std::map<int, std::string> g_mapTargetCols;
std::map<int, std::string> g_mapOptionCols;

const std::map<int, std::string>& GetTargetColMap()
{
	return g_mapTargetCols;
}

const std::map<int, std::string>& GetOptionColMap()
{
	return g_mapOptionCols;
}

void InitGridMap()
{

	g_mapTargetCols.clear();
	//g_mapTargetCols[QUOT_InstrumentID] = "合约";
	//g_mapTargetCols[QUOT_ChangeValue] = "涨跌";
	//g_mapTargetCols[QUOT_ChangePercentage] = "涨跌幅%";
	g_mapTargetCols[QUOT_BidPrice1] = "买价";
	//g_mapTargetCols[QUOT_BidVolume1] = "买量";
	g_mapTargetCols[QUOT_AskPrice1] = "卖价";
	//g_mapTargetCols[QUOT_AskVolume1] = "卖量";
	g_mapTargetCols[QUOT_LastPrice] = "最新价";
	//g_mapTargetCols[QUOT_OpenInterest] = "持仓量";
	//g_mapTargetCols[QUOT_OpenPrice] = "开盘价";
	//g_mapTargetCols[QUOT_PreSettlementPrice] = "昨结算价";
	g_mapTargetCols[QUOT_LastVolume] = "现量";
	//g_mapTargetCols[QUOT_HighestPrice] = "最高价";
	//g_mapTargetCols[QUOT_LowestPrice] = "最低价";
	//g_mapTargetCols[QUOT_ClosePrice] = "收盘价";
	g_mapTargetCols[QUOT_Volume] = "成交量";
	//g_mapTargetCols[QUOT_Turnover] = "成交金额";
	//g_mapTargetCols[QUOT_UpperLimitPrice] = "涨停价";
	//g_mapTargetCols[QUOT_LowerLimitPrice] = "跌停价";
	//g_mapTargetCols[QUOT_SettlementPrice] = "结算价";
	//g_mapTargetCols[QUOT_PreClosePrice] = "昨收价";
	//g_mapTargetCols[QUOT_PreOpenInterest] = "昨持仓量";
	//g_mapTargetCols[QUOT_UpdateTime] = "更新时间";
	g_mapTargetCols[QUOT_KLineView] = "K线";

	g_mapOptionCols.clear();
	//g_mapOptionCols[QUOT_InstrumentID] = "合约";
	//g_mapOptionCols[QUOT_ChangeValue] = "涨跌";
	//g_mapOptionCols[QUOT_ChangePercentage] = "涨跌幅%";
	g_mapOptionCols[QUOT_BidPrice1] = "买价";
	//g_mapOptionCols[QUOT_BidVolume1] = "买量";
	g_mapOptionCols[QUOT_AskPrice1] = "卖价";
	//g_mapOptionCols[QUOT_AskVolume1] = "卖量";
	g_mapOptionCols[QUOT_LastPrice] = "最新价";
	//g_mapOptionCols[QUOT_OpenInterest] = "持仓量";
	//g_mapOptionCols[QUOT_OpenPrice] = "开盘价";
	//g_mapOptionCols[QUOT_PreSettlementPrice] = "昨结算价";
	g_mapOptionCols[QUOT_LastVolume] = "现量";
	//g_mapOptionCols[QUOT_HighestPrice] = "最高价";
	//g_mapOptionCols[QUOT_LowestPrice] = "最低价";
	//g_mapOptionCols[QUOT_ClosePrice] = "收盘价";
	g_mapOptionCols[QUOT_Volume] = "成交量";
	//g_mapOptionCols[QUOT_Turnover] = "成交金额";
	//g_mapOptionCols[QUOT_UpperLimitPrice] = "涨停价";
	//g_mapOptionCols[QUOT_LowerLimitPrice] = "跌停价";
	//g_mapOptionCols[QUOT_SettlementPrice] = "结算价";
	//g_mapOptionCols[QUOT_PreClosePrice] = "昨收价";
	//g_mapOptionCols[QUOT_PreOpenInterest] = "昨持仓量";
	//g_mapOptionCols[QUOT_UpdateTime] = "更新时间";
	g_mapOptionCols[QUOT_KLineView] = "K线";

	g_mapOptionCols[QUOT_OptionPrice] = "行权价";

	int nNewHead = QUOTNEWHEAD;

	//g_mapOptionCols[nNewHead + QUOT_InstrumentID] = "合约";
	//g_mapOptionCols[nNewHead + QUOT_ChangeValue] = "涨跌";
	//g_mapOptionCols[nNewHead + QUOT_ChangePercentage] = "涨跌幅%";
	g_mapOptionCols[nNewHead + QUOT_BidPrice1] = "买价";
	//g_mapOptionCols[nNewHead + QUOT_BidVolume1] = "买量";
	g_mapOptionCols[nNewHead + QUOT_AskPrice1] = "卖价";
	//g_mapOptionCols[nNewHead + QUOT_AskVolume1] = "卖量";
	g_mapOptionCols[nNewHead + QUOT_LastPrice] = "最新价";
	//g_mapOptionCols[nNewHead + QUOT_OpenInterest] = "持仓量";
	//g_mapOptionCols[nNewHead + QUOT_OpenPrice] = "开盘价";
	//g_mapOptionCols[nNewHead + QUOT_PreSettlementPrice] = "昨结算价";
	g_mapOptionCols[nNewHead + QUOT_LastVolume] = "现量";
	//g_mapOptionCols[nNewHead + QUOT_HighestPrice] = "最高价";
	//g_mapOptionCols[nNewHead + QUOT_LowestPrice] = "最低价";
	//g_mapOptionCols[nNewHead + QUOT_ClosePrice] = "收盘价";
	g_mapOptionCols[nNewHead + QUOT_Volume] = "成交量";
	//g_mapOptionCols[nNewHead + QUOT_Turnover] = "成交金额";
	//g_mapOptionCols[nNewHead + QUOT_UpperLimitPrice] = "涨停价";
	//g_mapOptionCols[nNewHead + QUOT_LowerLimitPrice] = "跌停价";
	//g_mapOptionCols[nNewHead + QUOT_SettlementPrice] = "结算价";
	//g_mapOptionCols[nNewHead + QUOT_PreClosePrice] = "昨收价";
	//g_mapOptionCols[nNewHead + QUOT_PreOpenInterest] = "昨持仓量";
	//g_mapOptionCols[nNewHead + QUOT_UpdateTime] = "更新时间";
	g_mapOptionCols[nNewHead + QUOT_KLineView] = "K线";
}

// 设置确实的显示列
void ResetDefaultShowCol()
{
	std::vector<int> vecTargetCol;
	//vecTargetCol.push_back(QUOT_InstrumentID);
	vecTargetCol.push_back(QUOT_BidPrice1);
	vecTargetCol.push_back(QUOT_AskPrice1);
	vecTargetCol.push_back(QUOT_LastPrice);
	vecTargetCol.push_back(QUOT_LastVolume);
	vecTargetCol.push_back(QUOT_Volume);
	vecTargetCol.push_back(QUOT_KLineView);

	LockGlobalObject();
	SetTargetShowCol(vecTargetCol);
	UnlockGlobalObject();

	std::vector<int> vecOptionCol;
	int nNewHead = QUOTNEWHEAD;

	vecOptionCol.push_back(QUOT_BidPrice1);
	vecOptionCol.push_back(QUOT_AskPrice1);
	vecOptionCol.push_back(QUOT_LastPrice);
	vecOptionCol.push_back(QUOT_LastVolume);
	vecOptionCol.push_back(QUOT_Volume);
	vecOptionCol.push_back(QUOT_KLineView);

	vecOptionCol.push_back(QUOT_OptionPrice);

	vecOptionCol.push_back(nNewHead + QUOT_KLineView);
	vecOptionCol.push_back(nNewHead + QUOT_Volume);
	vecOptionCol.push_back(nNewHead + QUOT_LastVolume);
	vecOptionCol.push_back(nNewHead + QUOT_LastPrice);
	vecOptionCol.push_back(nNewHead + QUOT_AskPrice1);
	vecOptionCol.push_back(nNewHead + QUOT_BidPrice1);

	LockGlobalObject();
	SetOptionShowCol(vecOptionCol);
	UnlockGlobalObject();
}

std::vector<int> g_vecTargetShowCol;

const std::vector<int>& GetTargetShowCol()
{
	return g_vecTargetShowCol;
}

void SetTargetShowCol(const std::vector<int>& vecCol)
{
	g_vecTargetShowCol = vecCol;
}

std::vector<int> g_vecOptionShowCol;

const std::vector<int>& GetOptionShowCol()
{
	return g_vecOptionShowCol;
}

void SetOptionShowCol(const std::vector<int>& vecCol)
{
	g_vecOptionShowCol = vecCol;
}

void SaveConfig()
{
#if 0
	string strPath(GetXmlFile());
	CString strText;

	// 创建一个XML的文档对象。
	TiXmlDocument *pXmlDocment = new TiXmlDocument(); 
	std::auto_ptr<TiXmlDocument> ptr(pXmlDocment);
	// 创建一个根元素并连接。
	TiXmlElement *pRootElement = new TiXmlElement("root"); 
	pXmlDocment->LinkEndChild(pRootElement); 
	
	//创建一个NormalGrid元素并连接。
	TiXmlElement *pNormalGridElement = new TiXmlElement("NormalGrid"); 
	pRootElement->LinkEndChild(pNormalGridElement); 

	for(int i=0; i<(int)g_vecTargetShowCol.size(); i++) {

		//创建一个NormalGrid元素并连接。
		TiXmlElement *pColumnElement = new TiXmlElement("Column"); 
		pNormalGridElement->LinkEndChild(pColumnElement); 

		strText.Format("%d", g_vecTargetShowCol[i]);

		//设置Column元素的属性。
		pColumnElement->SetAttribute("ID", strText); 
	}
    //保存到文件
	pXmlDocment->SaveFile(strPath.c_str()); 
#endif
}

void LoadConfig()
{
#if 0
	string strPath(GetXmlFile());

	TiXmlDocument* pXmlDocment = new TiXmlDocument(strPath.c_str());
	if(pXmlDocment == NULL) 
		return;

	std::auto_ptr<TiXmlDocument> ptr(pXmlDocment);
	if(!pXmlDocment->LoadFile()) 
		return;

	TiXmlElement *root = pXmlDocment->RootElement();
	if ( root == NULL ) 
		return;

	if( std::string(root->Value()) != "root")
		return;

	TiXmlElement *tList = root->FirstChildElement("NormalGrid");
	if(tList == NULL)
		return;

	std::vector<int> vecCol;

	for(TiXmlElement *ColumnItem = tList->FirstChildElement("Column");
			ColumnItem; 
			ColumnItem = ColumnItem->NextSiblingElement("Column")) {

		const char *pColumnid = ColumnItem->Attribute("ID");
		if(!pColumnid) 
			continue;

		if(atoi(pColumnid) != QUOT_InstrumentID)
			vecCol.push_back(atoi(pColumnid));

		//string strID = pColumnid;
		//const char *pColumnName = ColumnItem->Attribute("name");
		//if(!pColumnName)
		//	continue;
	}
	//delete pXmlDocment;

	if(vecCol.size() > 0) {
		std::vector<int> vecTargetCol;

		vecTargetCol.push_back(QUOT_InstrumentID);
		for(int i=0; i<(int)vecCol.size(); i++) {
			vecTargetCol.push_back(vecCol[i]);
		}

		std::vector<int> vecOptionCol;
		int nNewHead = QUOTNEWHEAD;

		for(int i=(int)vecCol.size()-1; i>=0; i--) {
			vecOptionCol.push_back(vecCol[i]);
		}
		vecOptionCol.push_back(QUOT_OptionPrice);
		for(int i=0; i<(int)vecCol.size(); i++) {
			vecOptionCol.push_back(nNewHead + vecCol[i]);
		}

		LockGlobalObject();
		SetTargetShowCol(vecTargetCol);
		SetOptionShowCol(vecOptionCol);
		UnlockGlobalObject();
	}
#endif
}