#pragma once

#include <string>
#include "..\inc\ISvr\PlatformStruct.h"

using namespace std;


enum {	conMaxRltCodeNum = 32 };
enum {	conOCUnknow, conOCOpen, conOCClodeToday, conOCClose };

typedef struct {
	int	 nCmbCodeCount;
	char cType;
	char strCodeName[24];	// 合约简称
	char strCodeNameJP[24];	// 合约简称的拼音首字母
	char strCodeHeads[conMaxRltCodeNum][24];	// 合约代码头
} CODEHEADDATA, *PCODEHEADDATA;


size_t SplitString(const string strInput,vector<string>& outData,const char separator=',',bool bTailAppend=true);
int GetNumDigits(double dbTick);
string Number2String(int nData);
// 价格显示转换函数，返回一个价格字符串
// dbData, 价格
// dbTick，最小变动价位
string Price2String(double dbData, double dbTick);
string Double2String(double dbData);

BOOL CheckSumPrice(double& fPrice, double fPriceTick, int nStep=0);
double AnalysePrice(double fPrice, double fUpperLimitPrice, double fLowerLimitPrice);

BOOL ConvertComboInstrumentID(const string& strComboID, string& strInstrumentID1, string& strInstrumentID2);

BOOL IsMarket(const string& strPrice);

BOOL MakeReq_Order(PlatformStru_InputOrder& req, string strInstrumentID, string strAccount, 
				   int nOrderType, BOOL bDirection, int nOCMode, int nVolume, 
				   double fPrice, double fTriggerPrice, BOOL bIsHedge, BOOL bIsMarketPrice, 
				   BOOL bIsReplaceOrder);

BOOL MakeReq_Parked(PlatformStru_ParkedOrder& req, string strInstrumentID, string strAccount, 
					int nOrderType, BOOL bDirection, int nOCMode, int nVolume, 
					double fPrice, double fTriggerPrice, BOOL bIsHedge, BOOL bIsMarketPrice, 
					BOOL bIsReplaceOrder);

//BOOL MakeReq_Order(PlatformStru_InputOrder& req, string strUserID, string strAccount, 
//				   string strInstrumentID, int nOrderType, BOOL bDirection, int nOCMode, int nVolume, 
//				   double fPrice, double fTriggerPrice, BOOL bIsHedge, BOOL bIsMarketPrice, 
//				   BOOL bIsReplaceOrder);
//
//BOOL MakeReq_Parked(PlatformStru_ParkedOrder& req, string strUserID, string strAccount, 
//					string strInstrumentID, int nOrderType, BOOL bDirection, int nOCMode, int nVolume, 
//					double fPrice, double fTriggerPrice, BOOL bIsHedge, BOOL bIsMarketPrice, 
//					BOOL bIsReplaceOrder);

string Printf_StdOrderReq(const PlatformStru_InputOrder& req, string strAccountTitle, 
						  string strOrderTypeTitle, string strInstrumentIDTitle, string strDirectionTitle, 
						  string strOpenCloseTitle, string strQtyTitle, string strPriceTitle, 
						  string strTriggerPriceTitle, string strHedgeTitle, double fPriceTick=-1.0, 
						  BOOL bShowAccountItem=FALSE, BOOL bShowOpenCloseItem=TRUE, BOOL bShowHedgeItem=TRUE);

char* GetProductSNameItem(char* csProductSName, CODEHEADDATA& codeHead, BOOL& bRetHasData);
BOOL GetCodesItem(char* csCodes, CODEHEADDATA& codeHead);


//strName as std::string
//bIsBuy as BOOL
//strPriceText as std::string
//bIsMarketPrice as BOOL
//fPrice as double
#define AUTOMARKETPRICE(strName, bIsBuy, strPriceText, bIsMarketPrice, fPrice) \
	{\
		if(stricmp(strPriceText.c_str(), LOADSTRING(OLS_MARKET))==0) {\
			PlatformStru_InstrumentInfo contInfo;\
			memset(&contInfo,0,sizeof(contInfo));\
			IPlatformSingleSvr* pSvr=g_pPlatformMgr->GetCurrentPlatformSvr();\
			if(pSvr!=NULL) {\
				pSvr->GetInstrumentInfo(strName.c_str(), contInfo);\
				if(NEEDSIMULATEMARKET(contInfo.ExchangeID)) {\
					bIsMarketPrice = FALSE;\
					PlatformStru_DepthMarketData depthMarket;\
					memset(&depthMarket,0,sizeof(depthMarket));\
					if(DEFAULT_SVR()) DEFAULT_SVR()->GetQuotInfo(strName.c_str(), depthMarket);\
					if(bIsBuy) {\
						fPrice = depthMarket.UpperLimitPrice;\
					}\
					else {\
						fPrice = depthMarket.LowerLimitPrice;\
					}\
				}\
			}\
			else {\
				bIsMarketPrice = TRUE;\
			}\
		}\
		else {\
			bIsMarketPrice=FALSE;\
			fPrice=atof(strPriceText.c_str());\
		}\
	}

//strName as std::string
//bIsBuy as BOOL
//bIsMarketPrice as BOOL
//fPrice as double
#define AUTOMARKETPRICE2(strName, bIsBuy, bIsMarketPrice, fPrice) \
	{\
		if(bIsMarketPrice) {\
			PlatformStru_InstrumentInfo contInfo;\
			memset(&contInfo,0,sizeof(contInfo));\
			IPlatformSingleSvr* pSvr=g_pPlatformMgr->GetCurrentPlatformSvr();\
			if(pSvr!=NULL) {\
				pSvr->GetInstrumentInfo(strName.c_str(), contInfo);\
				if(NEEDSIMULATEMARKET(contInfo.ExchangeID)) {\
					bIsMarketPrice = FALSE;\
					PlatformStru_DepthMarketData depthMarket;\
					memset(&depthMarket,0,sizeof(depthMarket));\
					if(DEFAULT_SVR()) DEFAULT_SVR()->GetQuotInfo(strName.c_str(), depthMarket);\
					if(bIsBuy) {\
						fPrice = depthMarket.UpperLimitPrice;\
					}\
					else {\
						fPrice = depthMarket.LowerLimitPrice;\
					}\
				}\
			}\
		}\
	}
