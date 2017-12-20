#include "StdAfx.h"
#include "MultiAccountCore.h"
#include "MAConnSocket.h"
#include <algorithm>
#include <wx/file.h>
#include "../inc/Module-Misc/GlobalFunc.h"
#include "../inc/Module-Misc/CryptoAPI.h"
#include "../inc/cfgMgr/CfgMgr.h"
#include "../inc/cfgMgr/CfgGlobal.h"
#define ACC_STRING_SAVE(tag)\
	do{\
	TiXmlElement *pElement = new TiXmlElement(#tag);\
	TiXmlText *pContent = new TiXmlText(trader.tag);\
	pElement->LinkEndChild(pContent);\
	TraderElement->LinkEndChild(pElement);\
	}while(0)
#define ACC_INT_SAVE(tag)\
	do{\
	TiXmlElement *pElement = new TiXmlElement(#tag);\
	TiXmlText *pContent = new TiXmlText(wxString::Format("%d",trader.tag).c_str());\
	pElement->LinkEndChild(pContent);\
	TraderElement->LinkEndChild(pElement);\
	}while(0)
#define   ROUND(x)     ((int)((x)<0?((x)- 0.5):((x)+0.5))) 
using namespace std;
extern PlatformSvrMgr* g_pPlatformMgr;
CMultiAccountCore* CMultiAccountCore::m_pInstance = NULL;
CMultiAccountCore* CMultiAccountCore::GetInstance()
{
	if( m_pInstance == NULL ) 
	{
		m_pInstance = new CMultiAccountCore();
	}
	return m_pInstance;
}
void CMultiAccountCore::DestroyInstance()
{
	SAFE_DELETE( m_pInstance );
}

CMultiAccountCore::CMultiAccountCore(void):
m_uicallback(NULL),
m_pMACfg(NULL)
{
	CfgMgr* pMgr = CFG_MGR_DEFAULT();
	if(pMgr) m_pMACfg=pMgr->GetMultiAccountCfg();
}

CMultiAccountCore::~CMultiAccountCore(void)
{

}
StrategyItem CMultiAccountCore::GetStrategyNum()
{
	if(m_pMACfg) 
	{
		return (StrategyItem)(int)m_pMACfg->nStrategyItem;
	}
	return Strategy_None;
}
void CMultiAccountCore::SetStrategyNum(StrategyItem it)
{

	if(m_pMACfg) 
	{
		m_pMACfg->nStrategyItem=(int)it;
	}
	m_uicallback(false);
}
bool CMultiAccountCore::GetOrderInsertStrategy(vector<OrderInsertUIItem>& dest,
											   const OrderInsertUIItem& src,
											   StrategyItem stra,
											   map<string,bool>* accstatus)
{
	vector<pair<string,bool>> vecAccountStatus;
	vector<pair<string,bool>>::iterator it;
	for (size_t i=0;i<m_vecTradeAccount.size();++i)
	{
		IPlatformSingleSvr* pSvr=g_pPlatformMgr->GetPlatformSvr(m_vecTradeAccount[i].t.szTradeAccount);
		if(pSvr&&pSvr->GetQryFinished())
			vecAccountStatus.push_back(make_pair(m_vecTradeAccount[i].t.szTradeAccount,m_vecTradeAccount[i].StrategyValid));
	}
	if(accstatus)
	{
		for (it=vecAccountStatus.begin();it!=vecAccountStatus.end();)
		{
			map<string,bool>::iterator mapit=accstatus->find(it->first);
			if(mapit!=accstatus->end())
			{
				it->second=mapit->second;
				++it;
			}
			else
				it=vecAccountStatus.erase(it);
		}
	}
	StrategyItem strate=stra;
	if(strate==Strategy_None) 
		strate=GetStrategyNum();
	dest.clear();
////////////////////////////////////////
	string strProduct;
	PlatformStru_InstrumentInfo outData;
	IPlatformSingleSvr* pSvr=g_pPlatformMgr->GetCurrentPlatformSvr();
	if(pSvr&&pSvr->GetInstrumentInfo(src.InstrumentID,outData)==0)
	{
		strProduct=outData.ProductID;
	}
////////////////////////////////////////
	switch(strate)
	{
	case Strategy_Assign://下单板指定手数
		for (it=vecAccountStatus.begin();it!=vecAccountStatus.end();++it)
		{
			if(it->second)
			{
				OrderInsertUIItem item;
				memcpy(&item,&src,sizeof(OrderInsertUIItem));
				strcpy_s(item.Account,it->first.c_str());
				item.VolumeFlag=item.Volume;
				dest.push_back(item);
			}
		}
		break;
	case Strategy_Base://基数乘数方式
		for (it=vecAccountStatus.begin();it!=vecAccountStatus.end();++it)
		{
			if(it->second)
			{
				int nStrategyData=GetOrderStrategyData(BaseMultipliedWay,it->first,src.InstrumentID,strProduct);
				OrderInsertUIItem item;
				memcpy(&item,&src,sizeof(OrderInsertUIItem));
				strcpy_s(item.Account,it->first.c_str());
				item.VolumeFlag=nStrategyData;
				item.Volume=src.Volume*nStrategyData;
				dest.push_back(item);
			}
		}
		break;
	case Strategy_Total://总数分配方式
		{
			int nStrategyData=0,sumStrategyData=0,nLastVol=0;
			for (it=vecAccountStatus.begin();it!=vecAccountStatus.end();++it)
			{
				if(it->second)
				{
					sumStrategyData+=GetOrderStrategyData(SumDividedWay,it->first,src.InstrumentID,strProduct);
				}
			}
			for (it=vecAccountStatus.begin();it!=vecAccountStatus.end();++it)
			{
				if(it->second)
				{
					nStrategyData+=GetOrderStrategyData(SumDividedWay,it->first,src.InstrumentID,strProduct);
					OrderInsertUIItem item;
					memcpy(&item,&src,sizeof(OrderInsertUIItem));
					item.Volume=0;
					strcpy_s(item.Account,it->first.c_str());
					item.VolumeFlag=GetOrderStrategyData(SumDividedWay,it->first,src.InstrumentID,strProduct);
					if(sumStrategyData)
						item.Volume=ROUND(src.Volume*nStrategyData*1.0/sumStrategyData)-nLastVol;
					nLastVol+=item.Volume;
					dest.push_back(item);
				}
			}
		}
		break;
	case Strategy_FundDeviation://按资金向首帐户对齐
		{
			CfgMgr* pMgr = CFG_MGR_DEFAULT();
			if(pMgr) 
			{
				double firstAvailable=0,Available;
				int nFirstAccountAlign=pMgr->GetMultiAccountCfg()->nFirstAccountAlign;
				IPlatformSingleSvr* pCurSvr=g_pPlatformMgr->GetCurrentPlatformSvr();
				firstAvailable=GetAccountAlignOrAccountSizeData(pCurSvr,nFirstAccountAlign);
				for (it=vecAccountStatus.begin();it!=vecAccountStatus.end();++it)
				{
					if(it->second)
					{
						OrderInsertUIItem item;
						memcpy(&item,&src,sizeof(OrderInsertUIItem));
						strcpy_s(item.Account,it->first.c_str());
						IPlatformSingleSvr* pSvr=g_pPlatformMgr->GetPlatformSvr(it->first);
						if(firstAvailable==0)
						{
							item.Volume=0;
							item.VolumeFlag=0;
						}
						else
						{
							if(pSvr==pCurSvr)
							{
								item.Volume=src.Volume;
								item.VolumeFlag=1;
							}
							else
							{
								Available=GetAccountAlignOrAccountSizeData(pSvr,nFirstAccountAlign);
								if(!util::isInvalidValue(Available)&&!util::isInvalidValue(firstAvailable))
								{
									item.VolumeFlag=Available/firstAvailable;
									item.Volume=ROUND(src.Volume*item.VolumeFlag);
								}
							}
						}
						dest.push_back(item);
					}
				}			
			}
		}
		break;
	case Strategy_AccountSize://按帐户规模分配手数
		{
			CfgMgr* pMgr = CFG_MGR_DEFAULT();
			if(pMgr) 
			{
				double sumAvailable=0,Available;
				int nAccountScaleDivided=pMgr->GetMultiAccountCfg()->nAccountScaleDivided;
				for (it=vecAccountStatus.begin();it!=vecAccountStatus.end();++it)
				{
					if(it->second)
					{
						IPlatformSingleSvr* pSvr=g_pPlatformMgr->GetPlatformSvr(it->first);
						if(pSvr)
						{
							Available=GetAccountAlignOrAccountSizeData(pSvr,nAccountScaleDivided);
							if(!util::isInvalidValue(Available))
								sumAvailable+=Available;
						}
					}
				}
				Available=0;
				int volsum=0;
				for (it=vecAccountStatus.begin();it!=vecAccountStatus.end();++it)
				{
					if(it->second)
					{
						OrderInsertUIItem item;
						memcpy(&item,&src,sizeof(OrderInsertUIItem));
						item.Volume=0;
						item.VolumeFlag=0.0;
						strcpy_s(item.Account,it->first.c_str());
						IPlatformSingleSvr* pSvr=g_pPlatformMgr->GetPlatformSvr(it->first);
						if(pSvr)
						{
							item.VolumeFlag=GetAccountAlignOrAccountSizeData(pSvr,nAccountScaleDivided);
							Available+=item.VolumeFlag;
							if(!util::isInvalidValue(Available)&&sumAvailable)
							{
								item.Volume=ROUND(src.Volume*Available*1.0/sumAvailable)-volsum;
								volsum+=item.Volume;
							}
						}
						dest.push_back(item);
					}
				}
			}
		}
		break;
	case Strategy_UserDefine://策略板自定义手数方式
		for (it=vecAccountStatus.begin();it!=vecAccountStatus.end();++it)
		{
			if(it->second)
			{
				int nStrategyData=GetOrderStrategyData(UserDefinedVolumnWay,it->first,src.InstrumentID,strProduct);
				OrderInsertUIItem item;
				memcpy(&item,&src,sizeof(OrderInsertUIItem));
				strcpy_s(item.Account,it->first.c_str());
				item.Volume=nStrategyData;
				item.VolumeFlag=item.Volume;
				dest.push_back(item);
			}
		}
		break;
	}
	return true;
}
int	CMultiAccountCore::GetAccountStatus(map<string,bool>* accstatus)
{
	int invalidcount=0;
	if(accstatus)
	{
		accstatus->clear();
		for (size_t i=0;i<m_vecTradeAccount.size();++i)
		{
			IPlatformSingleSvr* pSvr=g_pPlatformMgr->GetPlatformSvr(m_vecTradeAccount[i].t.szTradeAccount);
			if(pSvr&&pSvr->GetQryFinished())
				(*accstatus)[m_vecTradeAccount[i].t.szTradeAccount]=m_vecTradeAccount[i].StrategyValid;
			else if(m_vecTradeAccount[i].StrategyValid)
				invalidcount++;
		}
	}
	return invalidcount;
}
wxArrayString CMultiAccountCore::GetAccountArray()
{
	wxArrayString items;
	for (size_t i=0;i<m_vecTradeAccount.size();++i)
	{
		items.push_back(m_vecTradeAccount[i].t.szTradeAccount);
	}
	return items;
}
int	CMultiAccountCore::SetAccountStatus(map<string,bool>* accstatus)
{
	if(accstatus)
	{
		for (size_t i=0;i<m_vecTradeAccount.size();++i)
		{
			m_vecTradeAccount[i].StrategyValid=(*accstatus)[m_vecTradeAccount[i].t.szTradeAccount];
		}
		m_uicallback(true);
	}
	return 0;
}
int CMultiAccountCore::GetOrderStrategyData(OrderStrategy eStrategy, const std::string strAccount,
						 std::string strInstrument, std::string strProduct)
{
	CfgMgr* pMgr = CFG_MGR_DEFAULT();
	if(pMgr) 
		return pMgr->GetOrderStrategyContractVolume(eStrategy,strAccount,strInstrument,strProduct);
	return 0;
}
void CMultiAccountCore::AddTradeAccount(CTPTradeAccount& trader)
{
	//考虑ID排序
	if(m_vecTradeAccount.size()==0)
		m_vecTradeAccount.push_back(trader);
	else
	{
		int index=0;
		for (size_t i=0;i<m_vecTradeAccount.size();++i)
		{
			if(m_vecTradeAccount[i].t.nID<=trader.t.nID)
				index++;
			else
				break;
		}
		m_vecTradeAccount.insert(m_vecTradeAccount.begin()+index,trader);
	}
}
void CMultiAccountCore::EraseTradeAccount(wxString acc,bool bOnlyContent)
{
	int index=FindTradeAccount(acc);
	if(index>=0)
	{
		if(bOnlyContent)
		{
			m_vecTradeAccount[index].UseQuot=false;
			ZeroMemory(&m_vecTradeAccount[index].StatusText,64);
		}
		else
			m_vecTradeAccount.erase(m_vecTradeAccount.begin()+index);
	}
}
void CMultiAccountCore::ClearAllTradeAccount()
{
	m_vecTradeAccount.clear();
}
bool UDgreater (const CTPTradeAccount& elem1, const CTPTradeAccount& elem2 )  
{  
	return elem1.t.nID < elem2.t.nID;  
}
void CMultiAccountCore::SetTradeAccount(vector<CTPTradeAccount>& vec)
{
	m_vecTradeAccount=vec;
	std::sort(m_vecTradeAccount.begin(),m_vecTradeAccount.end(),UDgreater);
}
void CMultiAccountCore::SetTradeAccount(int index,const CTPTradeAccount& pAcc)
{
	if(index>=0&&index<GetTradeAccountCount())
	{
		if(memcmp(&m_vecTradeAccount[index],&pAcc,sizeof(CTPTradeAccount))!=0)
		{
			m_vecTradeAccount[index]=pAcc;
		}
	}
}
void CMultiAccountCore::SetTradeAccountUseQuot(int index,bool UseQuot)
{
	if(index>=0&&index<GetTradeAccountCount())
	{
		if(m_vecTradeAccount[index].UseQuot!=UseQuot)
		{
			m_vecTradeAccount[index].UseQuot=UseQuot;
		}

	}
}
void CMultiAccountCore::SetTradeAccountStrategyValid(int index,bool b)
{
	if(index>=0&&index<GetTradeAccountCount())
	{
		if(m_vecTradeAccount[index].StrategyValid!=b)
		{
			m_vecTradeAccount[index].StrategyValid=b;
		}
	}
}
void CMultiAccountCore::SetTradeAccountStatusText(int index,wxString text)
{
	if(index>=0&&index<GetTradeAccountCount())
	{
		strncpy_s(m_vecTradeAccount[index].StatusText,63,(LPCTSTR)text,text.Length());
	}
}
int CMultiAccountCore::FindTradeAccount(wxString acc) const
{	
	for (size_t i=0;i<m_vecTradeAccount.size();++i)
	{
		if(acc==m_vecTradeAccount[i].t.szTradeAccount)
		{
			return i;
		}
	}
	return -1;
}
int CMultiAccountCore::GetTradeAccountCount() const
{
	return (int)m_vecTradeAccount.size();
}
const CTPTradeAccount* CMultiAccountCore::GetTradeAccount(int index) const
{
	if(index>=0&&index<GetTradeAccountCount())
		return &m_vecTradeAccount[index];
	return NULL;
}
string CMultiAccountCore::GetTradeAccountName(int index) const
{
	const CTPTradeAccount* p=GetTradeAccount(index);
	if(p) return p->t.szTradeAccount;
	return "";
}
int CMultiAccountCore::GetNextTradeAccountId() const
{
	int maxid=0;
	for (int i=0;i<GetTradeAccountCount();++i)
	{
		maxid=m_vecTradeAccount[i].t.nID>maxid?m_vecTradeAccount[i].t.nID:maxid;
	}
	return ++maxid;
}
void CMultiAccountCore::SaveDataToSvr(bool HasPwd,wxWindow* win)
{
	if(win) CMAConnSocket::GetInstance()->RegisteWindow(win);
	int buflen=sizeof(int)+sizeof(TradeAccount)*GetTradeAccountCount();
	char* pBuffer=new char[buflen];
	*((int*)pBuffer)=MA_CORE()->m_pLoginRsp.nAccountID;
	for(int index=0;index<GetTradeAccountCount();++index)
	{
		const CTPTradeAccount* pTradeAccount=GetTradeAccount(index);
		char* pBegin=pBuffer+sizeof(int)+index*sizeof(TradeAccount);
		memcpy(pBegin,&pTradeAccount->t,sizeof(TradeAccount));
		if(!HasPwd)
			ZeroMemory(((TradeAccount*)pBegin)->szPassword,sizeof(PASSWORDSTRING));
		else
		{
			CCryptoAPI::Encode3DES_ToString(((TradeAccount*)pBegin)->szPassword);
		}
	}
	CMAConnSocket::GetInstance()->SendData(Cmd_MA_SaveTradeAccount_Req,pBuffer,buflen);
	delete[] pBuffer;
}

void CMultiAccountCore::LoadFromSvr(wxWindow* win)
{
	CMAConnSocket::GetInstance()->RegisteWindow(win);
	int Accid=MA_CORE()->m_pLoginRsp.nAccountID;
	CMAConnSocket::GetInstance()->SendData(Cmd_MA_QryTradeAccount_Req, &Accid, sizeof(int));

}
void CMultiAccountCore::LoadFromLocal(map<int,vector<TradeAccount>>* pRtn)
{
	//UCHAR* pwd=NULL;
	//int pwdlen=0;
	//CCryptoAPI::Encode3DES(printer.CStr(),&pwd,pwdlen);
	wxFile file;
	string strPath="malocal.bin";
	GlobalFunc::GetPath(strPath);
	if ( !wxFile::Exists(strPath.c_str()))
		return;
	if(!file.Open(strPath.c_str(),wxFile::read))
		return;
	int len=file.Length();
	UCHAR* tmp=new UCHAR[len];
	if(len==file.Read(tmp,len))
	{
		string srcstr;
		int Accid;
		CCryptoAPI::Decode3DES(tmp,len,srcstr);
		TiXmlDocument mydoc;
		mydoc.Parse(srcstr.c_str());
		TiXmlElement*AccElement,*TraderElement,*ItemElement,*RootElement=mydoc.RootElement();
		if(RootElement&&string("Accounts")==RootElement->Value())
		{
			if(!pRtn) ClearAllTradeAccount();
			AccElement=RootElement->FirstChildElement("Account");
			vector<TradeAccount> vec;
			vector<CTPTradeAccount> ctpvec;	
			while(AccElement)
			{
				const char* pAccstr=AccElement->Attribute("Id");
				if(pAccstr==NULL) continue;
				Accid=atoi(pAccstr);
				TraderElement=AccElement->FirstChildElement("Trader");
				while(TraderElement)
				{
					CTPTradeAccount trader;
					ZeroMemory(&trader,sizeof(CTPTradeAccount));
					ItemElement=TraderElement->FirstChildElement();
					while(ItemElement)
					{
						const char* TagName=ItemElement?ItemElement->Value():"";
						const char* txt=ItemElement?ItemElement->GetText():NULL;
						if(txt)
						{
							if(string(TagName)=="szTradeAccount")
								strcpy_s(trader.t.szTradeAccount,sizeof(TRADEACCOUNTSTRING),txt);
							else if(string(TagName)=="szPassword")
								strcpy_s(trader.t.szPassword,sizeof(PASSWORDSTRING),txt);
							else if(string(TagName)=="nOwnerAccountID")
								trader.t.nOwnerAccountID=atoi(txt);
							else if(string(TagName)=="nBrokerID")
								trader.t.nBrokerID=atoi(txt);
							else if(string(TagName)=="nServerNo")
								trader.t.nServerNo=atoi(txt);
							else if(string(TagName)=="szNickName")
								strcpy_s(trader.t.szNickName,21,txt);
							else if(string(TagName)=="nID")
								trader.t.nID=atoi(txt);
							else if(string(TagName)=="nRowNo")
								trader.t.nRowNo=atoi(txt);
						}
						ItemElement=ItemElement->NextSiblingElement();
					}
					vec.push_back(trader.t);
					ctpvec.push_back(trader);
					TraderElement=TraderElement->NextSiblingElement();
				}
				if(pRtn)
					pRtn->insert(make_pair(Accid,vec));
				else if(Accid==m_pLoginRsp.nAccountID)
				{
					//载入当前账号
					SetTradeAccount(ctpvec);
				}
				AccElement=AccElement->NextSiblingElement();
			}
		}
	}
	file.Close();
}
void CMultiAccountCore::SaveDataToLocal(bool HasPwd,map<int,vector<TradeAccount>>* const pContent)
{
	map<int,vector<TradeAccount>> pNote;
	if(pContent==NULL)
	{
		LoadFromLocal(&pNote);
		int AccID=MA_CORE()->m_pLoginRsp.nAccountID;
		vector<TradeAccount> vec;
		for(int i=0;i<GetTradeAccountCount();++i)
		{
			vec.push_back(GetTradeAccount(i)->t);
		}
		pNote[AccID]=vec;
	}
	else
		pNote=*pContent;
	TiXmlDocument  mydoc;
	TiXmlElement *RootElement = new TiXmlElement("Accounts");
	mydoc.LinkEndChild(RootElement);
	map<int,vector<TradeAccount>>::iterator it;
	for (it=pNote.begin();it!=pNote.end();++it)
	{
		TiXmlElement *AccElement = new TiXmlElement("Account");
		AccElement->SetAttribute("Id",it->first);
		RootElement->LinkEndChild(AccElement);
		for (size_t i=0;i<it->second.size();++i)
		{
			TradeAccount& trader=it->second[i];
			TiXmlElement *TraderElement = new TiXmlElement("Trader");
			AccElement->LinkEndChild(TraderElement);
			ACC_STRING_SAVE(szTradeAccount);
			if(HasPwd)
				ACC_STRING_SAVE(szPassword);
			ACC_INT_SAVE(nOwnerAccountID);
			ACC_INT_SAVE(nBrokerID);
			ACC_INT_SAVE(nServerNo);
			ACC_STRING_SAVE(szNickName);
			ACC_INT_SAVE(nID);
			ACC_INT_SAVE(nRowNo);
		}
	}
	TiXmlPrinter printer;  
	//printer.SetIndent( 0 );  
	mydoc.Accept(&printer); 

	UCHAR* pwd=NULL;
	int pwdlen=0;
	CCryptoAPI::Encode3DES(printer.CStr(),&pwd,pwdlen);
	wxFile file;
	string strPath="malocal.bin";
	GlobalFunc::GetPath(strPath);
	if(file.Open(strPath.c_str(),wxFile::write))
	{
		file.Write(pwd,pwdlen);
		file.Close();
	}
	SAFE_DELETE( pwd );
}
void CMultiAccountCore::DelLocal()
{
	map<int,vector<TradeAccount>> mapvec;
	LoadFromLocal(&mapvec);
	map<int,vector<TradeAccount>>::iterator it=mapvec.find(MA_CORE()->m_pLoginRsp.nAccountID);
	if(it!=mapvec.end())
	{
		it->second.clear();
		mapvec.erase(it);
	}
	SaveDataToLocal(true,&mapvec);
}
void CMultiAccountCore::DelServer()
{
	int AccID=MA_CORE()->m_pLoginRsp.nAccountID;
	CMAConnSocket::GetInstance()->SendData(Cmd_MA_SaveTradeAccount_Req,&AccID,sizeof(int));
}
void CMultiAccountCore::SetLoadTypeAuto(int b)
{
	if(m_pMACfg) 
	{
		m_pMACfg->nLoadType=b;
	}
}
void CMultiAccountCore::SetSaveTypeAuto(int b)
{
	if(m_pMACfg) 
	{
		m_pMACfg->nSaveType=b;
	}
}
void CMultiAccountCore::SetLoadTypeManual(int b)
{
	if(m_pMACfg) 
	{
		m_pMACfg->nLoadTypeManual=b;
	}
}
void CMultiAccountCore::SetSaveTypeManual(int b)
{
	if(m_pMACfg) 
	{
		m_pMACfg->nSaveTypeManual=b;
	}
}
int CMultiAccountCore::GetLoadTypeAuto()
{
	if(m_pMACfg) 
	{
		return m_pMACfg->nLoadType;
	}
	return 0;
}
int CMultiAccountCore::GetSaveTypeAuto()
{
	if(m_pMACfg) 
	{
		return m_pMACfg->nSaveType;
	}
	return 0;
}

int CMultiAccountCore::GetLoadTypeManual()
{
	if(m_pMACfg) 
	{
		return m_pMACfg->nLoadTypeManual;
	}
	return 0;
}
int CMultiAccountCore::GetSaveTypeManual()
{
	if(m_pMACfg) 
	{
		return m_pMACfg->nSaveTypeManual;
	}
	return 0;
}

double CMultiAccountCore::GetAccountAlignOrAccountSizeData( IPlatformSingleSvr* pSvr, int nFirstAccountAlign )
{
	double Available;
	PlatformStru_TradingAccountInfo outData;
	pSvr->GetTradingAccount(pSvr->GetAccount(),outData);
	switch(nFirstAccountAlign)
	{
	case 0:
		Available=outData.DynamicProfit;
		break;
	case 1:
		Available=outData.Available;
		break;
	case 2:
		Available=outData.StaticProfit;
		break;
	}
	return Available;
}
