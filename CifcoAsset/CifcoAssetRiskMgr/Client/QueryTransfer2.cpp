#include "StdAfx.h"
#include "QueryTransfer2.h"
#include <set>
#include <sstream>
#include <iostream> 


#include <windows.h>
#include "RiskManageCmd.h"
#include "UserApiStruct.h"
#include "CommonPkg.h"
#include "TcpLayer.h"


QueryTransfer2::QueryTransfer2(void)
{
	m_pdataInfo = CDataInfo::NewInstance();

}
QueryTransfer2::~QueryTransfer2(void)
{
}
//订阅事件
void QueryTransfer2::SubscribeEvt(unsigned int hWnd)
{
	CTcpLayer::SubscribePkg(Cmd_RM_QryTraderDeposit_Rsp,(int)hWnd);
}
//退订事件
void QueryTransfer2::UnsubscribeEvt(unsigned int hWnd)
{
	CTcpLayer::UnsubscribeAllPkg((int)hWnd);
}
// 拆分字符串
std::vector<std::string> QueryTransfer2::split(const std::string &str )
{
	std::vector<std::string> elems;
	stringstream ss(str);
	std::string buf;
	while(ss >> buf)
		elems.push_back(buf);
	return elems;
}

//向服务器请求投资者出入金
void QueryTransfer2::ReqQueryTransfer(std::set<int> setAccntID)
{
	
    m_mapTrustTradeAccount.clear();
	/*m_pdataInfo->GetTrustTradeAccount(m_mapTrustTradeAccount);

	std::vector<std::string> vecAccountID = split(strAccountID);
	int accountId[100];
    int i=0;
	std::vector<std::string>::iterator itstr=vecAccountID.begin();
	for(itstr;itstr!=vecAccountID.end();itstr++)
	{

		MAPTrustTradeAccount::iterator itint=m_mapTrustTradeAccount.begin();

		for(itint;itint!=m_mapTrustTradeAccount.end();itint++)
		{
			if(*itstr == itint->second.szAccount)
			{
				accountId[i] = itint->second.nTradeAccountID;
				i++;
				break;
			}
		}
	}*/


	std::set<int>::iterator itstr=setAccntID.begin();
	for(itstr;itstr!=setAccntID.end();itstr++)
	{

	    CTcpLayer::SendData(Cmd_RM_QryTraderDeposit_Req,(void *)&(*itstr), sizeof(int), 0);
	}

	//char buf[sizeof(Stru_UniPkgHead)+4];
	//Stru_UniPkgHead& PkgHead=*(Stru_UniPkgHead*)buf;
	//PkgHead.Set(0,Cmd_RM_QryTarderDeposit_Req,0,0);
	//*(unsigned int*)(buf+sizeof(PkgHead))=TailID_UniPkg;

	//CTcpLayer::Senddata(buf,sizeof(buf));
}

//处理出入金消息
bool QueryTransfer2::ProcessTransferEvtPkg(const char* pPkg,std::map<int, SeqDepositField>& mapSeqDeposit,int fieldcnt,string& Direction)
{
	for ( int i = 0; i < fieldcnt; i++ )
	{
		SeqDepositField* pField = (SeqDepositField*)((char*)(pPkg+sizeof(Stru_UniPkgHead)) + i*sizeof(SeqDepositField));
		
		//根据出入金方向填写map
		if ( Direction == NoChoice)
		{
			AssetMgmtOrganization Organization;
			memset(&Organization,0,sizeof(Organization));
			//m_pdataInfo->GetAssetMgmtOrg(pField->nAssetMgmtOrgID, Organization);

			mapSeqDeposit[pField->UniqSequenceNo]  =	*pField;
		}
		else if (Direction == FD_In)
		{
			if(pField->Direction == T_FD_In)
			{
                mapSeqDeposit[pField->UniqSequenceNo]  =	*pField;
			}

		}
		else if (Direction == FD_Out)
		{
			if(pField->Direction == T_FD_Out)
			{
				mapSeqDeposit[pField->UniqSequenceNo]  =	*pField;
			}

		}
			
	}
	/*SeqDepositField Field;
	memset(&Field,0,sizeof(Field));*/

	/*for(int i=0;i<fieldcnt;i++)
	{
		char buf[256];
		itoa(i,buf,10);
		pstr[i].assign(buf);
	}*/
	return true;
}

string QueryTransfer2::Double2String(double dbData)
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