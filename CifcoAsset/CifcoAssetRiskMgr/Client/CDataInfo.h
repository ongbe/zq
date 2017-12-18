#pragma once
#include "RiskManageCmd.h"
#include "RiskManageStruct.h"
#include "UserApiStruct.h"
#include "CommonPkg.h"
#include "TcpLayer.h"
#include <vector>
#include <map>
#include <set>
using namespace std;

typedef std::map<int, AssetMgmtOrganization>				  MAPAssetMgmtOrg;
typedef std::map<int, std::map<int, AssetMgmtOrganization>>   MAPAeestMgmtOrganization;
typedef std::map<int, FinancialProduct>						  MAPFinancialProduct;
typedef std::map<int, RiskIndicatorModule>					  MAPRiskIndicatorModule;
typedef std::map<int, RiskIndicator>						  MAPRiskIndicator;
typedef std::map<int, TrustTradeAccount>					  MAPTrustTradeAccount;
typedef std::map<string, TrustTradeAccount>					  MAPTrustTradeAccountByName;
typedef std::map<int, TraderInfo>							  MAPTraderInfo;

typedef std::multimap<int, TrustTradeAccount>				  MAPOrgID2TrustTradeAccount;  //根据组织机构对应的交易账号
typedef std::multimap<int, TrustTradeAccount>				  MAPFinancialID2TrustTradeAccount;  //根据理财产品对应的交易账号

typedef std::map<string, InstrumentField>					  MAPInstrumentID2InstrumentField; //合约id映射到合约详情
typedef std::multimap<string, InstrumentField>				  MAPProductID2InstrumentField;    //根据产品代码映射到合约详情

typedef std::map<int, PermissionValue>						  MAPID2PermissionValue;

typedef std::multimap<int, RiskEventHandling>				  MAPAccountID2RiskEventHandling;


typedef std::multimap<int, RiskMgmtUserInfo>				  MAPOrgID2RiskMgmtUserInfo;


typedef multimap<int, RiskEvent>  MAPInt2RiskEvent;
struct RiskEventHistory
{
	RiskEvent				riskEvent;	
	RSTIME					lEventTime;//风险消失时间	
	void set(RiskEvent&	rEvent, RSTIME lTime)
	{
		riskEvent.nRiskEventID		= rEvent.nRiskEventID;
		riskEvent.lEventTime		= rEvent.lEventTime;	
		riskEvent.nTradeAccountID	= rEvent.nTradeAccountID;
		strcpy(riskEvent.InstrumentID, rEvent.InstrumentID);
		riskEvent.nRiskIndModuleID	= rEvent.nRiskIndModuleID;
		riskEvent.nRiskIndicatorID	= rEvent.nRiskIndicatorID;
		riskEvent.nRiskLevelID		= rEvent.nRiskLevelID;
		riskEvent.dblIndicatorValue	= rEvent.dblIndicatorValue;
		riskEvent.nMsgSendStatus	= rEvent.nMsgSendStatus;
		riskEvent.nIsValid			= rEvent.nIsValid;
	}
};

typedef map<int, RiskEventHistory>  MAPInt2RiskEventHistory;

struct RiskWarningKey
{
	int	  nRiskIndModuleID; //风险模型
	int   nRiskIndicatorID; //风险指标	
	int	  nRiskLevelID;		//风险等级
	bool operator<(const RiskWarningKey item) const
	{
		if(nRiskIndModuleID < item.nRiskIndModuleID)
			return true;
		else if(nRiskIndModuleID > item.nRiskIndModuleID)
			return false;

		if(nRiskIndicatorID < item.nRiskIndicatorID)
			return true;
		else if(nRiskIndicatorID > item.nRiskIndicatorID)
			return false;

		if(nRiskLevelID < item.nRiskLevelID)
			return true;
		else  if(nRiskLevelID > item.nRiskLevelID)
			return false;
		
		return false;
	}
};

typedef map<RiskWarningKey, RiskWarning>  MAPRiskWarning;

struct ProductRiskWarningKey
{
	int				  nTradeAccountID;
	string			  szProductID;	//品种名称
	int				  nRiskIndicatorID;
	int				  nRiskLevelID;
	bool operator<(const ProductRiskWarningKey item) const
	{
		if(nTradeAccountID < item.nTradeAccountID)
			return true;
		else if(nTradeAccountID > item.nTradeAccountID)
			return false;

		int i = strcmp(szProductID.c_str(), item.szProductID.c_str());			
		if(i<0)
			return true;
		else if(i>0)
			return false;

		if(nRiskIndicatorID < item.nRiskIndicatorID)
			return true;
		else if(nRiskIndicatorID > item.nRiskIndicatorID)
			return false;

		if(nRiskLevelID < item.nRiskLevelID)
			return true;
		else if(nRiskLevelID > item.nRiskLevelID)
			return false;

		return false;
	}
};

typedef map<ProductRiskWarningKey, ProductRiskWarning>  MAPProductRiskWarningKey;

class CDataInfo
{
private:
	CDataInfo(){
		Clear();
	};
	~CDataInfo(){};
public:
	static CDataInfo* NewInstance();
	static void DestroyInstance();
	static CDataInfo* m_pInstance;


protected:	
	string  m_strUserName;					//登录用户名
	string  m_strPwd;						//登录密码
	LoginRsp	m_loginRsp;					//登录成功后返回的信息

	//资管组织
	MAPAeestMgmtOrganization	m_mapAssetMgmtOrgTree;	
	MAPAssetMgmtOrg				m_mapAssetMgmtOrg;//保存ID和组织机构的关系
	bool m_bAeestMgmtOrganization;	

	//理财产品
	MAPFinancialProduct			m_mapFinancialProduct;
	bool m_bFinancialProduct;	

	//风险指标模型结构
	MAPRiskIndicatorModule		m_mapRiskIndicatorModule;
	bool m_bRiskIndicatorModule;	

	//风险指标
	MAPRiskIndicator			m_mapRiskIndicator;
	bool m_bRiskIndicator;	

	//委托交易账户结构
	MAPTrustTradeAccount		m_mapTrustTradeAccount;
	MAPTrustTradeAccountByName	m_mapTrustTradeAccountByName;

	//根据组织机构对应的交易账号
	MAPOrgID2TrustTradeAccount  m_multimapOrgID2TrustTradeAccount;

	//根据理财产品对应的交易账号
	MAPFinancialID2TrustTradeAccount m_multimapFinancialID2TrustTradeAccount;
	bool m_bTrustTradeAccount;

	//交易员信息
	MAPTraderInfo				m_mapTraderInfo;
	bool m_bTraderInfo;

	//查询资管组织与理财产品关系
	multimap<int, int>			m_mulmapMgmtOrg2Financial;
	bool m_bMgmtOrg2Financial;

	//根据合约ID得到合约详情
	MAPInstrumentID2InstrumentField			m_mapInstrumentID2InstrumentField;

	//根据产品ID得到合约详情
	MAPProductID2InstrumentField			m_mapProductID2InstrumentField;
	bool m_bInstrumentField;

	//管理终端权限
	MAPID2PermissionValue					m_mapID2PermissionValue;
	bool m_bPermissionValue;

	//账户处理记录(非基础数据)
	MAPAccountID2RiskEventHandling			m_mapAccountID2RiskEventHandling;

	//查询某资管组织下属所有账
	MAPOrgID2RiskMgmtUserInfo				m_mapOrgID2RiskMgmtUserInfo;
	bool m_bRiskMgmtUserInfo;

	//风险警示
	MAPRiskWarning					m_mapRiskWarning;
	bool m_bRiskWarning;
	
	MAPProductRiskWarningKey		m_mapProductRiskWarningKey;
	bool m_bProductRiskWarning;
public:	
	MAPInt2RiskEvent						m_mapCurRiskEvent;        //当前风险事件
	MAPInt2RiskEventHistory					m_mapHisEvent;
	map<int, __int64>						m_mapInt2Time;			  //风险事件ID对应消失时间

public://获取基础数据的外部接口
	bool IsInit();//判断是否初始化完成

	void GetLoginRsp(LoginRsp&	loginRsp);  //得到登录信息
	void GetPwd(string& strPwd);			//得到登录密码
	void GetUserName(string& strUserName);	//得到登录用户名

	//得到资管组织
	void GetAeestMgmtOrg(MAPAeestMgmtOrganization&		mapAssetMgmtOrgTree);
	//根据资管ID得到资管结构
	bool GetAssetMgmtOrg(int nAssetMgmtOrgID, AssetMgmtOrganization& sOrg);

	//理财产品
	void GetFinancialProduct(MAPFinancialProduct&	mapFinancialProduct);
	//根据理财产品ID号得到理财产品结构
	bool GetFinancialProduct(int nFinancialID, FinancialProduct& fP);

	//风险指标模型结构
	void GetRiskIndicatorModule(MAPRiskIndicatorModule&	mapRiskIndicatorModule);	
	//根据风险指标模型ID得到风险指标模型结构
	bool GetRiskIndicatorModule(int nRiskIndicatorModuleID, RiskIndicatorModule&	sRiskIndicatorModule);

	//风险指标
	void GetRiskIndicator(MAPRiskIndicator&	mapRiskIndicator);
	//根据风险指标ID得到风险指标结构
	bool GetRiskIndicator(int nRiskIndicator, RiskIndicator&	sRiskIndicator);

	//委托交易账户
	void GetTrustTradeAccount(MAPTrustTradeAccount&	mapTrustTradeAccount);
	//根据交易账号ID号得到交易账号结构
	bool GetTrustTradeAccount(int nTradeAccountID, TrustTradeAccount& fP);
	//根据交易账号名称得到交易账号结构
	bool GetTrustTradeAccount(string strTradeAccountID, TrustTradeAccount& fP);

	//交易员信息
	void GetTraderInfo(MAPTraderInfo&	mapTraderInfo);
	//根据交易员ID得到交易结构
	bool GetTraderInfo(int nID, TraderInfo&	 sTraderInfo);

	/*
	通过组织机构编号查询相关的理财产品。
    参数：组织机构ID
          是否包括子机构的理财产品
          包含理财产品ID的set
    返回值：int，理财产品的数量，-1 表示失败*/
	int GetFinancialProduct(int  nAssetMgmtOrg, bool bIncludeChildren, set<int>& setFPID);

	/*
	通过组织机构编号查询相关的交易帐号。
    参数：组织机构ID
          是否包括子机构的交易账号
          包含交易账号ID的set
    返回值：int，交易账号的数量，-1 表示失败*/
	int GetTrustTradeAccountByOrgID(int  nAssetMgmtOrg, bool bIncludeChildren, set<int>& setFPID);
	int GetTrustTradeAccountByOrgID(int  nAssetMgmtOrg, bool bIncludeChildren, set<TrustTradeAccount>& setFPID);
	
	/*
	通过理财产品编号查询相关的交易帐号。
    参数：理财产品ID
          包含交易账号ID的set
		  返回值：int，交易账号的数量，-1 表示失败*/
	int GetTrustTradeAccountByFinancialID(int  nFinancialID, set<int>& setFPID);
	int GetTrustTradeAccountByFinancialID(int  nFinancialID, set<TrustTradeAccount>& setFPID);
	
	/*
	通过组织机构编号和理财产品编号查询相关的交易帐号。
	参数：组织机构ID
	是否包括子机构的交易账号
	包含交易账号ID的set
	返回值：int，交易账号的数量，-1 表示失败*/
	int GetTrustTradeAccountByOrgIDFinancialID(int  nAssetMgmtOrg, bool bIncludeChildren,int  nFinancialID, set<int>& setFPID);
	int GetTrustTradeAccountByOrgIDFinancialID(int  nAssetMgmtOrg, bool bIncludeChildren,int  nFinancialID, set<TrustTradeAccount>& setFPID);
	
	//通过合约代码获得相关合约信息
	bool GetInstrumentFieldByInstrumentID(string strID, InstrumentField& instrumentField);

	//通过品种代码获得相关合约信息
	bool GetInstrumentFieldByProductID(string strID, vector<InstrumentField>& setInstrumentField);
	
	//获得所有品种代码
	void GetAllProducts(set<string>& setProductID);

	//得到所有合约
	void GetAllProducts(MAPInstrumentID2InstrumentField& mapID2InstrumentField);

	//得到所有管理端权限
	void GetPermission(MAPID2PermissionValue&	mapPermissionValue);
	//判断某一权限是否允许
	bool HasPermission(PermissionActionType PAType);

	//得到某一账号下，某一风险ID的处理记录; 如果nRiskEventID 为-1，则表示获取该账号下所有的风险事件
	bool GetRiskEvent(int nAccountID, int nRiskEventID, vector<RiskEventHandling>& vexRiskEventHandling);

	//通过组织机构编号得到这一级或者下一级的风控员账户。
	int GetRiskMgmtUserInfoByOrgID(int  nAssetMgmtOrg, bool bIncludeChildren, vector<RiskMgmtUserInfo>& vec);
	void GetmapOrgID2RiskMgmtUserInfo(MAPOrgID2RiskMgmtUserInfo& mapOrgID2RiskMgmtUserInfo);

	//得到风险警示
	bool GetRiskWarning(RiskWarningKey key, RiskWarning& rw);

	//得到品种风险警示
	bool GetProductRiskWarning(ProductRiskWarningKey key, ProductRiskWarning& rw);

public://设置基础数据的接口，一般从服务器获取数据时候用的
	void Clear();
	
	void FinishAeestMgmtOrganization(bool bFinish);
	void FinishFinancialProduct(bool bFinish);
	void FinishRiskIndicatorModule(bool bFinish);
	void FinishRiskIndicator(bool bFinish);
	void FinishTrustTradeAccount(bool bFinish);
	void FinishTraderInfo(bool bFinish);
	void FinishMgmtOrg2Financial(bool bFinish);
	void FinishInstrumentField(bool bFinish);
	void FinishPermissionValue(bool bFinish);
	void FinishRiskMgmtUserInfo(bool bFinish);
	void FinishProductRiskWarning(bool bFinish);
	void FinishRiskWarning(bool bFinish);

	double GetinitProgress();

	void SetLoginRsp(LoginRsp	loginRsp);  //设置登录信息
	void SetPwd(string strPwd);				//设置登录密码
	void SetUserName(string strUserName);	//设置登录用户名
	

	void SetAeestMgmtOrg(MAPAssetMgmtOrg&		mapAssetMgmtOrg);
	

	void SetFinancialProduct(FinancialProduct *pFP);
	void SetRiskIndicatorModule(RiskIndicatorModule *pRIM);
	

	void SetRiskIndicator(RiskIndicator *pRI);
	void SetTrustTradeAccount(TrustTradeAccount *pRI);

	void SetOrgID2TrustTradeAccount(TrustTradeAccount *pRI);
	void SetFinancialID2TrustTradeAccount(TrustTradeAccount *pRI);
	void SetTraderInfo(TraderInfo *pRI);

	void SetMulmapMgmtOrg2Financial(AssetOrgProRelation *pAopr);

	void SetInstrumentField(InstrumentField *pField);


	void SetRiskEvent(RiskEvent *pRiskEvent);

	void SetPermissionValue(PermissionValue* pValue);

	void SetRiskEventHandling(RiskEventHandling* pValue);

	void SetRiskMgmtUserInfo(RiskMgmtUserInfo *pValue);

	//设置品种查询风险预警返回
	void SetProductRiskWarning(ProductRiskWarning* pValue);

	//设置查询风险预警
	void SetRiskWarning(RiskWarning* pValue);
};
