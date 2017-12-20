#pragma once
#include "CommonPkg.h"
#include "CommonStruct.h"
#include "RiskManageStruct.h"
#include "TcpLayer.h"
#include "TcpDataConnect.h"
#include "defineGridCols.h"
#include "Tools.h"

#include <vector>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

typedef pair<int, int> Key_RiskPlan;

#define  NULL_QUERY_CONDITION    "查询条件为空，请设置查询条件"

struct sForceClose  
{
    ///投资者帐号
    TThostFtdcAccountIDType	AccountID;
    ///合约代码
    TThostFtdcInstrumentIDType	InstrumentID;
    ///买卖方向
    TThostFtdcDirectionType		Direction;
    ///开平标志
    TThostFtdcOffsetFlagType	OffsetFlag;
    ///数量
    TThostFtdcVolumeType	Volume;   
    ///价格
    TThostFtdcPriceType	Price;
    ///涨停板价
    TThostFtdcPriceType	UpperLimitPrice;
    ///跌停板价
    TThostFtdcPriceType	LowerLimitPrice;
    ///可平量
    TThostFtdcLargeVolumeType	OpenInterest;
    ///可平今量
    TThostFtdcLargeVolumeType	TodayOpenInterest;

    sForceClose()
    {
        memset(AccountID,0,sizeof(AccountID));
        memset(InstrumentID,0,sizeof(InstrumentID));
        Direction = THOST_FTDC_D_Sell;
        OffsetFlag = THOST_FTDC_OF_Close;
        Volume = 0;
        Price = 0.0f;
        UpperLimitPrice = 0.0f;
        LowerLimitPrice = 0.0f;
        OpenInterest = 0;
        TodayOpenInterest = 0;
    }
};

struct sInstrumentInfo
{
    //string  Exchange;           //交易所代码
    int     Volume;             //成交量
    double  fLastPrice;         //行情最新价
    double  fUpperLimitPrice;   //涨停价
    double  fLowerLimitPrice;   //跌停价
};

#define DataGridView_DepthMarket    "DepthMarket"
#define DataGridView_RiskEvent      "RiskEvent"
#define DataGridView_TradingAccount "TradingAccount"
#define DataGridView_AllOrder       "AllOrder"
#define DataGridView_Trade          "Trade"
#define DataGridView_UnTrade        "UnTrade"
#define DataGridView_Position       "Position"
#define DataGridView_ClosePosition  "ClosePosition"
#define DataGridView_NeedVerify     "NeedVerify"

enum eXmlDataType
{
    XML_TYPE_INST,      //自选合约
    XML_TYPE_COLUMN,    //datagridview列
    XML_TYPE_INFO       //保存的合约信息
};

class CRiskData
{
public:
    static CRiskData* GetInstance();
    static void DestroyInstance();
    static CRiskData* m_pInstance;

private:
    CRiskData();
    ~CRiskData();


private:
    void InitData();

public:

    //配置文件
    void SetConfigFile(vector<string>& vecData, eXmlDataType eType);
    map<string,vector<string>> GetConfigFile(eXmlDataType eType);
//     void UpdaeConfigFile(string szKey,vector<string> vecLastData,eXmlDataType eType);

    //登录信息
    void SetLogonInfo(const sLoginRsp& sloginInfo);
    void GetLogonInfo(sLoginRsp& sloginInfo);
    int  GetOrderRef();//下手动强平有关
    void SetRiskerAccount(string& riskAccount);
    string GetRiskAccount();

    //组织机构
    void SetOrganizationLoad(bool bLoad);
    bool GetOrganizationLoad();
    void ClearOrganization();
    void SetOrganization(map<int, Organization>& mapOrganization);
    void GetOrganization(map<int, map<int, Organization>>& mapAssetMgmtOrgTree);
    bool GetOrganizationByID(int nAssetMgmtOrgID, Organization& sOrg);
    bool GetOrganizationIDByName(string& szName, int& nID);
    bool GetOrganizationNameByID(int nID, string& szName);
    bool IsOrganizationID(int nOrgID);
    bool IsMonitorOrg(int nOrgID);
    void GetMonitorOrgByOrgID(int nOrgID,vector<int> vecMonitorOrgIDAll,vector<int>& vecMonitorOrgID);

    //用户
    void SetUserInfoLoad(bool bLoad);
    bool GetUserInfoLoad();
    void ClearUserInfo();
    void SetUserInfo(map<int, UserInfo>& mapUserInfo);
    bool GetAllUserInfo(map<int, UserInfo>& mapUserInfo);
    bool GetUserNameByID(int nUserID, string& szUserName);
    bool GetUserInfoByAccount(const string& szAccount, UserInfo& userInfo);
    bool IsLimitTrade(int nUserID);
    bool IsManualVerify(int nUserID);
    void SetLimitTrade(const SLimitTrade* pData);
    void SetManualVerify(const SManualVerify* pData);
	bool GetUserInfoByID(int nUserID,UserInfo& userinfo);
	const std::string UserType2String(UserType & userType);
    bool IsTraderID(int nTraderID);
    void SetUserLoginStatus(map<string,bool>& mapLogin);
    void GetUserLoginStatus(map<string,bool>& mapLogin);

    //委托交易账号
    void ClearTradeAccount();
    void SetTradeAccount(map<int, TradeAccount>& mapTradeAccount);
    bool GetTradeAccountByID(int nTradeAccountID,string& szTradeAccount);

    //用户与组织机构关系
    void SetUserAndOrgRelationLoad(bool bLoad);
    bool GetUserAndOrgRelationLoad();
    void ClearUserAndOrgRelation();
    void SetUserAndOrgRelation(vector<UserAndOrgRelation>& vecUserAndOrgRelation);
    void GetUserAndOrgRelation(vector<UserAndOrgRelation>& vecUserAndOrgRelation);
    int  GetOrgIDByUserID(int nUserID);
    void GetOrgIDByUserID(int nUserID, eUSERANDORGRELATIONTYPE eType, vector<int>& vecOrgID);
    void GetUserInfosByOrgID(int nOrgID, UserType nUserType, vector<UserInfo>& vecUserInfo, bool bSubOrg);
    void GetUserInfosBySubOrgID(UserType nUserType, map<int, map<int, Organization>>& mapMain, map<int, Organization>& mapSub, vector<UserInfo>& vecUserInfo, bool bSubOrg);


    //用户与委托交易账号
    void SetUserAndTraderAccountRelationLoad(bool bLoad);
    bool GetUserAndTraderAccountRelationLoad();
    void ClearUserAndTraderAccountRelation();
    void SetUserAndTraderAccountRelation(map<int, UserAndTradeAccountRelation>& mapUserAndOrgRelation);
    void GetTradeAccountByTraderCode(const string& szTraderCode, string& szTradeAccount);//目前一个交易员有且只有一个委托交易账号

    //风控指标
    void SetRiskIndicatorLoad(bool bLoad);
    bool GetRiskIndicatorLoad();
    void ClearRiskIndicator();
    void SetRiskIndicator(map<RiskIndicatorType,RiskIndicator>& mapRiskIndicator);
    void GetRiskIndicator(map<RiskIndicatorType,RiskIndicator>& mapRiskIndicator);
    bool GetIndicatorIDByName(string& szName, RiskIndicatorType& nID);
    bool GetIndicatorDescByID(RiskIndicatorType nIndicatorID, string& szRiskDesc);


    //风控方案
    void ClearRiskPlan();
    void SetRiskPlan(map<string,vector<RiskPlan>>& mapRiskPlan);
    void GetRiskPlan(map<string,vector<RiskPlan>>& mapRiskPlan);
    void SetRiskPlanStatus(RiskChange& riskChange);
    void SetKey2RiskPlan(map<Key_RiskPlan,vector<RiskPlan>>& mapKey2RiskPlan);
    bool GetKey2RiskPlan(Key_RiskPlan key,vector<RiskPlan>& vecRiskPlan);
    bool GetRiskPlan(Key_RiskPlan key,int nRiskLevel,RiskPlan& RiskPlan);
    bool GetRiskWaring(int nOrgID,RiskIndicatorType nRiskIndicatorType,int nRiskLevelID,RiskWaring& riskWaring);
    void DelRiskPlan(string& szOrgName,Key_RiskPlan key);
    void UpdateRiskPlan(string& szOrgName, vector<RiskPlan>& vecRiskPlan);
    void UpdateRiskPlan(Key_RiskPlan key, vector<RiskPlan>& vecRiskPlan);
    void SetNetFundParam(Key_RiskPlan key, const NetFundParam* pParam);

    //风控动作
    void SetRiskResponseLoad(bool bLoad);
    bool GetRiskResponseLoad();
    void ClearRiskResponse();
    void SetRiskResponse(vector<RiskResponse>& vecRiskResponse);
    void GetRiskResponse(vector<RiskResponse>& vecRiskResponse);

    //合约品种
    void SetInstrumentStyle(vector<SProduct>& vecProduct);
    void GetInstrumentStyle(vector<SProduct>& vecProduct);

    void SetExchange2Instrument(map<string, vector<string>>& mapExchange2Ist);
    void GetExchange2Instrument(map<string, vector<string>>& mapExchange2Ist);
    string GetExchangeByInstrument(const string& szInst);

    void SetInstrument2PriceTick(map<string, double>& mapInst2Tick);
    double GetPriceTickByInstID(const string& szInstID);

    bool bSubScribeInst(const string& szInst);
    void SubScribeInst(const string& szExchange,const string& szInst);
    void UnSubScribeInst(const string& szExchange,const string& szInst);
    void AnalyseSelectInst(vector<string> vecSrc, vector<string>& vecSub, vector<string>& vecUnSub);

    bool bSubScribeInst2(const string& szInst);
    void SubScribeInst2(const string& szExchange,const string& szInst);
    void UnSubScribeInst2(const string& szExchange,const string& szInst);

    void SetInstrumentInfo(const string& szInst, const sInstrumentInfo& sInstInfo);
    void GetInstrumentInfo(const string& szInst, sInstrumentInfo& sInstInfo);
    map<string, sInstrumentInfo> GetInstrumentInfo();

private:
    bool m_bOrganizationLoad;
    bool m_bUserInfoLoad;
    bool m_bUserAndOrgRelationLoad;
    bool m_bUserAndTraderAccountRelationLoad;
    bool m_bRiskIndicatorLoad;
    bool m_bRiskResponseLoad;

private:

    //配置文件
    map<string,vector<string>>                  m_mapExchange2Inst;//本地文件 配置文件和手工设置
    map<string,vector<string>>                  m_mapExchange2Inst2;//交易端下单过来的合约(没有订阅的)
    map<string,vector<string>>                  m_mapDataGridView2Column;

    //登录信息
    int                                         m_nManualForceCount;
    sLoginRsp                                   m_sLoginInfo;
    string                                      m_szRiskAccount;

    //组织机构
    map<int, map<int, Organization>>            m_mapAssetMgmtOrgTree;//一个组织机构ID下的所有组织机构信息
    map<int, Organization>	                    m_mapOrganization;    

    //用户
    map<int, UserInfo>                          m_mapUserInfo;
    map<string, bool>                           m_mapUserLoginStatus;

    //委托交易账号
    map<int, TradeAccount>                      m_mapTradeAccount;

    //用户与组织机构关系
    vector<UserAndOrgRelation>	                m_vecUserAndOrgRelation;

    //用户与委托交易账户关系
    map<int, UserAndTradeAccountRelation>       m_mapUserAndTradeAccountRelation;

    //风控指标
    map<RiskIndicatorType, RiskIndicator>                     m_mapRiskIndicator;

    //风控方案
    map<string, vector<RiskPlan>>               m_mapRiskPlan;
    map<Key_RiskPlan,vector<RiskPlan>>          m_mapKey2RiskPlan;//Key = <nOrgID,nRiskIndicatorType>

    //风控动作
    vector<RiskResponse>                        m_vecRiskResponse;

public:
    //风险事件	
    multimap<int, RiskEvent>				   m_mapCurRiskEvent;

private:
    //合约品种
    vector<SProduct>                            m_vecProduct;

    //交易所与合约关系
    map<string, vector<string>>                 m_mapExchange2Instrument; //从服务器请求的   

    //合约的信息(深度行情)
    map<string, sInstrumentInfo>                m_mapInstrumentInfo;

    //合约的信息(最小变动价位)
    map<string, double>                         m_mapInstrument2PriceTick;

public:
	std::map <int,int>                          m_mapMessageID2RowID;

};