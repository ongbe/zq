#include "StdAfx.h"
#include "QueryInvestorInfo2.h"
#include <sstream>
#include <iostream> 
#include <set>
#include <map>
#include <queue>
#include <windows.h>
#include "RiskManageCmd.h"
#include "UserApiStruct.h"
#include "CommonPkg.h"
#include "TcpLayer.h"

QueryInvestorInfo2::QueryInvestorInfo2(void)
{
	m_pdataInfo = CDataInfo::NewInstance();
}

QueryInvestorInfo2::~QueryInvestorInfo2(void)
{
}
// 拆分字符串
std::vector<std::string> QueryInvestorInfo2::split(const std::string &str )
{
	std::vector<std::string> elems;
	stringstream ss(str);
	std::string buf;
	while(ss >> buf)
		elems.push_back(buf);
	return elems;
}

string QueryInvestorInfo2::Double2String(double dbData)
{
	char strPriceText[512]={0};
	ZeroMemory(strPriceText, sizeof(strPriceText));
	strcpy(strPriceText, "-");

	sprintf(strPriceText, "%f", dbData);
	for(int i=strlen(strPriceText)-1; i>=0; i--) 
	{
		if(strPriceText[i]=='0')
			strPriceText[i]=0;
		else
		{
			if(strPriceText[i]=='.')
				strPriceText[i]=0;
			break;
		}
	}
	return strPriceText;
}
 //   void QueryInvestorInfo2::ReqInvestorInfo(std::string strOrganization,std::string strFinancial, std::map<int, TrustTradeAccount>& mapAccount)
	//{

	//}
	void QueryInvestorInfo2::ReqInvestorInfo(std::set<int>&  OrganizationId,std::set<int> & FinancialId, std::map<int, TrustTradeAccount>& mapAccount)
	{
		bool bNoChecked = true;
		//std::set<int> setOrganID;
		//std::set<int> setFinanProdID;
		
		std::set<int> setTemp;
		std::set<int> setOrganAccntID;

		std::set<int>::iterator it;
		std::set<int>::iterator itTmp;
		//得到资管组织下的Account
		for(it = OrganizationId.begin(); it != OrganizationId.end(); it++) 
		{
			setTemp.clear();
			m_pdataInfo->GetTrustTradeAccountByOrgID(*it, bNoChecked, setTemp);
			for(itTmp = setTemp.begin(); itTmp != setTemp.end(); itTmp++) 
			{
				setOrganAccntID.insert(*itTmp);
			}
		}

		std::set<int> setProdAccntID;
        //得到理财产品下的Account
		for(it = FinancialId.begin(); it != FinancialId.end(); it++) 
		{
			setTemp.clear();
			m_pdataInfo->GetTrustTradeAccountByFinancialID(*it, setTemp);
			for(itTmp = setTemp.begin(); itTmp != setTemp.end(); itTmp++) 
			{
				setProdAccntID.insert(*itTmp);
			}
		}
        //得到两者交集
		int nMax = max(setOrganAccntID.size(), setProdAccntID.size());
		int* pTempID = new int[nMax];

		std::set<int> setResult;
		int* pEnd = std::set_intersection(
			setOrganAccntID.begin(), setOrganAccntID.end(), 
			setProdAccntID.begin(), setProdAccntID.end(), pTempID);
		int* nPos = pTempID;
		while(nPos < pEnd) 
		{
			setResult.insert(*nPos);
			nPos++;
		}

		TrustTradeAccount acct;
		memset(&acct,0,sizeof(acct));
		for(it = setResult.begin(); it != setResult.end(); it++)
		{
			m_pdataInfo->GetTrustTradeAccount(*it, acct);
			mapAccount[*it] = acct;
		}

		
	}
	void QueryInvestorInfo2::ReqInvestorInfo(std::set<int> setAccntID,std::map<int, TrustTradeAccount>& mapAccount)
	{
		m_mapTrustTradeAccount.clear();
		m_pdataInfo->GetTrustTradeAccount(m_mapTrustTradeAccount);

		//std::vector<std::string> vecAccountID = split(strAccount);
		//int accountId[100];
		//int i=0;
		std::set<int>::iterator itstr=setAccntID.begin();
		for(itstr;itstr!=setAccntID.end();itstr++)
		{

			MAPTrustTradeAccount::iterator itint=m_mapTrustTradeAccount.begin();

			for(itint;itint!=m_mapTrustTradeAccount.end();itint++)
			{
				if(*itstr == itint->first)
				{
					mapAccount[itint->first]=itint->second;
					break;
				}
			}
		}
	}
