//保存和维护CTP的合约、费率、交易所时间、交易所信息等数据

#pragma warning(disable:4996)

#include <vector>
#include <string>
#include <map>
using namespace std;

#include "PlatformDataMgr_Instruments.h"
#include "PlatformDataMgrMacro.h"
#include "Module-Misc2/GlobalFunc.h"
#include "Tools_Ansi.h"
#include "FileOpr.h"



#ifdef WIN32
#pragma unmanaged
#endif 


//构造：
CPlatformDataMgr_Instruments::CPlatformDataMgr_Instruments(bool bUseComb)
:   m_bUseComb(bUseComb)
{
}

CPlatformDataMgr_Instruments::~CPlatformDataMgr_Instruments(void)
{
    LOG_INFO("CPlatformDataMgr_Instruments::~CPlatformDataMgr_Instruments(析构CPlatformDataMgr_Instruments对象)");
}

//设置指定产品信息
void CPlatformDataMgr_Instruments::SetProductInfo(const string& strProductID, const PlatformStru_ProductInfo& inData)
{
	LOG_INFO("CPlatformDataMgr_Instruments::SetProductInfo");

    Lock_CPlatformDataMgr;

    m_allProducts[strProductID] = inData;

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr_Instruments::SetProductInfo-end");
}

//从产品列表中删除没有合约的产品. 查询合约结束后执行一次 (实盘里发现有这种情况，出现有过期的产品)
void CPlatformDataMgr_Instruments::DeleteEmptyProduct()
{
	LOG_INFO("CPlatformDataMgr_Instruments::DeleteEmptyProduct");

    Lock_CPlatformDataMgr;

    map<string, PlatformStru_ProductInfo>::iterator it=m_allProducts.begin();
    while(it!=m_allProducts.end())
    {
        map<string,set<string>>::const_iterator it2=m_ProductIDsByExchangeID.find(it->second.ExchangeID);
        if(it2==m_ProductIDsByExchangeID.end()||it2->second.find(it->first)==it2->second.end())
            it=m_allProducts.erase(it);
        else it++;
    }

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr_Instruments::DeleteEmptyProduct-end");
}

//ctp测试环境下，从合约名称解析出交易所ID
const string CPlatformDataMgr_Instruments::Instrument2ExchangeID(const string& strInstrument)
{
	//只用于ctp测试服务器。实盘环境可以从合约信息得到交易所ID
	size_t len= strInstrument.length();
	if(len==6)
	{
        char ch1=strInstrument.at(0);
        char ch2=strInstrument.at(1);
        if(isupper(ch1)&&isupper(ch2))  //like IFxxxx、TFxxxx, which is instrument of cffex
			return "CFFEX";
		else
			return "SHFE";
	}
	else if(len==5)
	{
		char ch=strInstrument.at(1);
		if(ch>='0'&&ch<='9')
			return "DCE";
		else
			return "CZCE";
	}
	else
		return "";
}

///设置指定合约信息
void CPlatformDataMgr_Instruments::SetInstrumentInfo(const string& InstrumentID, const PlatformStru_InstrumentInfo& inData,bool bCTPSimulate,bool bLast)
{
	LOG_INFO("CPlatformDataMgr_Instruments::SetInstrumentInfo");

    PlatformStru_InstrumentInfo  newData;
	newData=inData;

    if(bCTPSimulate)
	{
		string strNewID=Instrument2ExchangeID(newData.InstrumentID);
		if(!strNewID.empty())
			strcpy(newData.ExchangeID,strNewID.c_str());
	}

    Lock_CPlatformDataMgr;

    ///针对期权合约进行相关处理 
    if(newData.ProductClass==THOST_FTDC_PC_Options||newData.ProductClass==THOST_FTDC_PC_SpotOption)
    {
        ///重新设置中金所期权合约的标的期货合约代码(UnderlyingInstrID)
        ///目前(2014.8.20)，中金所的期权合约里UnderlyingInstrID未显示出标的期货合约，需要单独处理一下，转换为标的期货合约
        if(stricmp(newData.ExchangeID,"CFFEX")==0)
        {
            if(memicmp(newData.InstrumentID,"HO",2)==0)
            {
                memcpy(newData.UnderlyingInstrID,"IH",2);
                memcpy(newData.UnderlyingInstrID+2,newData.InstrumentID+2,4);
                memset(newData.UnderlyingInstrID+6,0,sizeof(newData.UnderlyingInstrID)-6);
            }
            else if(memicmp(newData.InstrumentID,"IO",2)==0)
            {
                memcpy(newData.UnderlyingInstrID,"IF",2);
                memcpy(newData.UnderlyingInstrID+2,newData.InstrumentID+2,4);
                memset(newData.UnderlyingInstrID+6,0,sizeof(newData.UnderlyingInstrID)-6);
            }
        }
    }


    m_allInstruments[InstrumentID] = newData;
    m_allInstrumentList.insert(InstrumentID);
    if(inData.ProductClass!=THOST_FTDC_PC_Combination)
    {
        //单腿合约
        m_allInstrumentListWithoutComb.insert(InstrumentID);
    }
    else
    {
        //组合合约
        string Leg1,Leg2;
        if(GlobalFunc::IsCombInstrument(InstrumentID,Leg1,Leg2))
        {
            if(!CTools_Ansi::IsKeyValueInMultimap<string,string>(m_Leg2Comb,Leg1,InstrumentID))
                m_Leg2Comb.insert(make_pair(Leg1,InstrumentID));

            if(!CTools_Ansi::IsKeyValueInMultimap<string,string>(m_Leg2Comb,Leg2,InstrumentID))
                m_Leg2Comb.insert(make_pair(Leg2,InstrumentID));

            m_Legs2Comb[Leg1+string("&")+Leg2]=InstrumentID;
        }
    }

    m_allInstrumentIDsByProductID[inData.ProductID].insert(InstrumentID);

    string strExchangeID(inData.ExchangeID);
    string strProductID(inData.ProductID);

    //加入m_ProductIDsByExchangeID
    map<string,set<string>>::iterator it_ProductIDsByExchangeID=m_ProductIDsByExchangeID.find(strExchangeID);
    if(it_ProductIDsByExchangeID==m_ProductIDsByExchangeID.end())
    {
        m_ProductIDsByExchangeID.insert(make_pair(strExchangeID,set<string>()));
        it_ProductIDsByExchangeID=m_ProductIDsByExchangeID.find(strExchangeID);
    }
    set<string>::iterator it_ProductID=it_ProductIDsByExchangeID->second.find(strProductID);
    if(it_ProductID==it_ProductIDsByExchangeID->second.end())
        it_ProductIDsByExchangeID->second.insert(strProductID);


    if(bLast)
    {
        ///针对期权合约进行处理
        ///设置交易所ID->期权标的物品种ID数据。合约查询完毕后设置
        for(map<string, PlatformStru_InstrumentInfo>::const_iterator it=m_allInstruments.begin();it!=m_allInstruments.end();it++)
        {
            //取标的物期权合约
            if(it->second.ProductClass!=THOST_FTDC_PC_Options&&it->second.ProductClass!=THOST_FTDC_PC_SpotOption) 
                continue;

            string strUnderlyingInstrumentID(it->second.UnderlyingInstrID);
            map<string, PlatformStru_InstrumentInfo>::const_iterator it_UnderlyingInstrumentInfo=m_allInstruments.find(strUnderlyingInstrumentID);
            if(it_UnderlyingInstrumentInfo==m_allInstruments.end())
                continue;

            //加入m_OptionUnderlyingProductIDsByExchangeID中
            if(!CTools_Ansi::IsKeyValueInMultimap<string,string>(m_OptionUnderlyingProductIDsByExchangeID,it_UnderlyingInstrumentInfo->second.ExchangeID,it_UnderlyingInstrumentInfo->second.ProductID))
                m_OptionUnderlyingProductIDsByExchangeID.insert(make_pair(string(it_UnderlyingInstrumentInfo->second.ExchangeID),string(it_UnderlyingInstrumentInfo->second.ProductID)));
            //加入m_OptionInstrumentIDsByUnderlyingInstrumentID中
            if(!CTools_Ansi::IsKeyValueInMultimap<string,string>(m_OptionInstrumentIDsByUnderlyingInstrumentID,it_UnderlyingInstrumentInfo->second.UnderlyingInstrID,it->second.InstrumentID))
                m_OptionInstrumentIDsByUnderlyingInstrumentID.insert(make_pair(string(it_UnderlyingInstrumentInfo->second.InstrumentID),string(it->second.InstrumentID)));
            if(it->second.OptionsType==THOST_FTDC_CP_CallOptions&&
                !CTools_Ansi::IsKeyValueInMultimap<string,string>(m_CallOptionInstrumentIDsByUnderlyingInstrumentID,it_UnderlyingInstrumentInfo->second.UnderlyingInstrID,it->second.InstrumentID))
                m_CallOptionInstrumentIDsByUnderlyingInstrumentID.insert(make_pair(string(it_UnderlyingInstrumentInfo->second.InstrumentID),string(it->second.InstrumentID)));
            else if(it->second.OptionsType==THOST_FTDC_CP_PutOptions&&
                !CTools_Ansi::IsKeyValueInMultimap<string,string>(m_PutOptionInstrumentIDsByUnderlyingInstrumentID,it_UnderlyingInstrumentInfo->second.UnderlyingInstrID,it->second.InstrumentID))
                m_PutOptionInstrumentIDsByUnderlyingInstrumentID.insert(make_pair(string(it_UnderlyingInstrumentInfo->second.InstrumentID),string(it->second.InstrumentID)));
        }
    }

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr_Instruments::SetInstrumentInfo-end");
}




///获取指定合约信息
bool CPlatformDataMgr_Instruments::GetInstrumentInfo(const string& strInstrumentID, PlatformStru_InstrumentInfo& outData)
{
    LOG_INFO("CPlatformDataMgr_Instruments::GetInstrumentInfo");
    bool brlt=false;

    Lock_CPlatformDataMgr;

    map<string,PlatformStru_InstrumentInfo>::iterator it =  m_allInstruments.find(strInstrumentID);
    if(it==m_allInstruments.end())
    {
        memset(&outData,0,sizeof(outData));
    }
    else 
    {
        outData = it->second;
        brlt=true;
    }

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr_Instruments::GetInstrumentInfo-end");
    return brlt;
}


///指定合约是否合法(存在)
bool CPlatformDataMgr_Instruments::InstrumentIsValid(const string& strInstrumentID)
{
    LOG_INFO("CPlatformDataMgr_Instruments::InstrumentIsValid");
    bool ret;
    map<string,PlatformStru_InstrumentInfo>::iterator it;

    Lock_CPlatformDataMgr;

    it =  m_allInstruments.find(strInstrumentID);
    if(it==m_allInstruments.end()) ret=false;
    else ret=true;

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr_Instruments::InstrumentIsValid-end");
    return ret;
}

///获取包含此单腿合约的组合合约列表
bool CPlatformDataMgr_Instruments::GetCombInstrumentIDs_IncludeLeg(const string& LegInstrument,vector<string>& vecCombInstruments)
{
    LOG_INFO("CPlatformDataMgr_Instruments::GetCombInstrumentIDs_IncludeLeg");
    bool ret=false;
    multimap<string,string>::iterator itlower,itupper;

    Lock_CPlatformDataMgr;

    itlower=m_Leg2Comb.lower_bound(LegInstrument);
    itupper=m_Leg2Comb.upper_bound(LegInstrument);
    vecCombInstruments.clear();
    while(itlower!=itupper)
    {
        vecCombInstruments.push_back(itlower->second);
        itlower++;
    }

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr_Instruments::GetCombInstrumentIDs_IncludeLeg-end");
    return vecCombInstruments.size()==0?false:true;
}


///获取合约的产品类型，失败返回-1
char CPlatformDataMgr_Instruments::GetProductClassType(const string& InstrumentID)
{
    char ProductClassType;
    map<string,  PlatformStru_InstrumentInfo>::iterator it;

    Lock_CPlatformDataMgr;

    it =  m_allInstruments.find(InstrumentID);
    if(it==m_allInstruments.end())
        ProductClassType=-1;
    else 
        ProductClassType = it->second.ProductClass;

    Unlock_CPlatformDataMgr;

    return ProductClassType;
}
///获取合约的ProductID
string CPlatformDataMgr_Instruments::GetProductID(const string& strInstrumentID)
{
    string strProductID;
    map<string,  PlatformStru_InstrumentInfo>::iterator it;

    Lock_CPlatformDataMgr;

    it =  m_allInstruments.find(strInstrumentID);
    if(it!=m_allInstruments.end())
        strProductID = string(it->second.ProductID);

    Unlock_CPlatformDataMgr;

    return strProductID;
}
///获取合约的交易所，失败返回空
bool CPlatformDataMgr_Instruments::GetExchangeID(const string& InstrumentID,string& outExchangeID)
{
    CeasymutexGuard guard(m_mutex);
    map<string,PlatformStru_InstrumentInfo>::const_iterator it=m_allInstruments.find(InstrumentID);
    if(it==m_allInstruments.end())
    {
        outExchangeID.clear();
        return false;
    }
    else 
    {
        outExchangeID=string(it->second.ExchangeID);
        return true;
    }
}
///获取合约的交易所，失败返回空
string CPlatformDataMgr_Instruments::GetExchangeID(const string& InstrumentID)
{
    string ExchangeID;
    if(GetExchangeID(InstrumentID,ExchangeID)) return ExchangeID;
    else return string("");
}

///获取合约列表
void CPlatformDataMgr_Instruments::GetInstrumentList(vector<InstrumentGroupInfo> &outData)
{
	outData.clear();
    LOG_INFO("CPlatformDataMgr_Instruments::GetInstrumentList");

    map<string,PlatformStru_InstrumentInfo>::iterator it_all;

    Lock_CPlatformDataMgr;

	map<string,int> GroupName2ID;
	map<string,int>::iterator GroupName2ID_it;
    for(it_all=m_allInstruments.begin();it_all!=m_allInstruments.end();it_all++)
	{
        if(!m_bUseComb&&
            it_all->second.ProductClass==THOST_FTDC_PC_Combination)
            continue;

        GroupName2ID_it=GroupName2ID.find(it_all->second.ExchangeID);
		if(GroupName2ID_it==GroupName2ID.end())
		{
			InstrumentGroupInfo ginfo;
            ginfo.GroupName=it_all->second.ExchangeID;
			ginfo.InstrVec.push_back(SimpleInstrumentInfo(it_all->second.InstrumentID,it_all->second.InstrumentName));
			outData.push_back(ginfo);
			GroupName2ID[it_all->second.ExchangeID]=outData.size()-1;
		}
		else
		{
			outData[GroupName2ID_it->second].InstrVec.push_back(SimpleInstrumentInfo(it_all->second.InstrumentID,it_all->second.InstrumentName));
		}
	}

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr_Instruments::GetInstrumentList-end");
}
///获取合约列表
void CPlatformDataMgr_Instruments::GetInstrumentList(set<string> &outData,const string ExchangeID)
{
    LOG_INFO("CPlatformDataMgr_Instruments::GetInstrumentList");

    outData.clear();

    Lock_CPlatformDataMgr;

    if(ExchangeID.empty())
    {
        if(!m_bUseComb)
		    outData = m_allInstrumentListWithoutComb;
        else 
            outData = m_allInstrumentList;
    }
	else
	{
        map<string,  PlatformStru_InstrumentInfo>::iterator it2;
        for(it2=m_allInstruments.begin();it2!=m_allInstruments.end();it2++)
        {
            if(!m_bUseComb&&
                it2->second.ProductClass==THOST_FTDC_PC_Combination)
                continue;
            if(string(it2->second.ExchangeID)==ExchangeID)
                outData.insert(it2->first);
        }
	}
    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr_Instruments::GetInstrumentList-end");
}
void CPlatformDataMgr_Instruments::GetInstrumentList(vector<string> &outData,const string ExchangeID)
{
    LOG_INFO("CPlatformDataMgr_Instruments::GetInstrumentList");

    outData.clear();

    Lock_CPlatformDataMgr;

    if(ExchangeID.empty())
    {
        if(!m_bUseComb)
            CTools_Ansi::ConvertSet2Vector<string>(m_allInstrumentListWithoutComb,outData);
        else 
            CTools_Ansi::ConvertSet2Vector<string>(m_allInstrumentList,outData);
    }
	else
	{
        map<string,  PlatformStru_InstrumentInfo>::iterator it2;
        for(it2=m_allInstruments.begin();it2!=m_allInstruments.end();it2++)
        {
            if(!m_bUseComb&&
                it2->second.ProductClass==THOST_FTDC_PC_Combination)
                continue;
            if(string(it2->second.ExchangeID)==ExchangeID)
                outData.push_back(it2->first);
        }
	}
    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr_Instruments::GetInstrumentList-end");
}
void CPlatformDataMgr_Instruments::GetInstrumentInfos(vector<PlatformStru_InstrumentInfo> &outData,const string ExchangeID)
{
    LOG_INFO("CPlatformDataMgr_Instruments::GetInstrumentInfos");

    outData.clear();

    Lock_CPlatformDataMgr;

    bool bExchangeIDEmpty=ExchangeID.empty();
    for(map<string,PlatformStru_InstrumentInfo>::const_iterator it=m_allInstruments.begin();it!=m_allInstruments.end();it++)
    {
        if(!m_bUseComb&&it->second.ProductClass==THOST_FTDC_PC_Combination) continue;
        if(!bExchangeIDEmpty&&string(it->second.ExchangeID)!=ExchangeID) continue;
        outData.push_back(it->second);
    }

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr_Instruments::GetInstrumentInfos-end");
}

///获取合约列表
int CPlatformDataMgr_Instruments::GetMainInstrumentList(set<string> &outData)
{
    LOG_INFO("CPlatformDataMgr_Instruments::GetMainInstrumentList");

    outData.clear();

    Lock_CPlatformDataMgr;

#ifdef _NEED_MAIN_INSTRUMENT
		outData = m_allMainInstrumentList;
#else

    if(!m_bUseComb)
		outData = m_allInstrumentListWithoutComb;
    else
        outData = m_allInstrumentList;

#endif	

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr_Instruments::GetMainInstrumentList-end");
    return outData.size();
}
///添加主力合约列表
void CPlatformDataMgr_Instruments::AddMainInstrumentList(string instrument)
{
    Lock_CPlatformDataMgr;

    m_allMainInstrumentList.insert(instrument);

    Unlock_CPlatformDataMgr;
}
///设置合约容差列表
void CPlatformDataMgr_Instruments::SetInstrumentVarietyMap(const map<string,string>& inData)
{
    LOG_INFO("CPlatformDataMgr_Instruments::SetInstrumentVarietyMap");

    Lock_CPlatformDataMgr;

    m_allInstrumentVarietyMap = inData;

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr_Instruments::SetInstrumentVarietyMap-end");
}

// 获取合约容差列表
void CPlatformDataMgr_Instruments::GetInstrumentVarietyMap(map<string,string>& outData)
{
    LOG_INFO("CPlatformDataMgr_Instruments::SetInstrumentVarietyMap");

    Lock_CPlatformDataMgr;

    outData = m_allInstrumentVarietyMap;

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr_Instruments::SetInstrumentVarietyMap-end");
}

///获取指定品种的合约列表
void CPlatformDataMgr_Instruments::GetInstrumentListByProductID(const string& ProductID,set<string> &outData)
{
    LOG_INFO("CPlatformDataMgr_Instruments::GetInstrumentListByProductID");

    outData.clear();

    Lock_CPlatformDataMgr;

    map<string, set<string> >::iterator it=m_allInstrumentIDsByProductID.find(ProductID);
    if(it!=m_allInstrumentIDsByProductID.end()) 
        outData = it->second;

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr_Instruments::GetInstrumentListByProductID-end");
}
void CPlatformDataMgr_Instruments::GetInstrumentListByProductID(const string& ProductID,vector<string> &outData)
{
    LOG_INFO("CPlatformDataMgr_Instruments::GetInstrumentListByProductID");

    outData.clear();

    Lock_CPlatformDataMgr;

    map<string, set<string> >::iterator it=m_allInstrumentIDsByProductID.find(ProductID);
    if(it!=m_allInstrumentIDsByProductID.end()) 
        CTools_Ansi::ConvertSet2Vector(it->second,outData);

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr_Instruments::GetInstrumentListByProductID-end");
}

///获取指定交易所的所有品种->合约
void CPlatformDataMgr_Instruments::GetProductID_InstrumentIDsByExchangeID(map<string, set<string> >& outData,const string ExchangeID)
{
    LOG_INFO("CPlatformDataMgr_Instruments::GetProductID_InstrumentIDsByExchangeID");

    map<string,  PlatformStru_InstrumentInfo>::iterator it2; 
    Lock_CPlatformDataMgr;

	if(ExchangeID.empty())
        outData=m_allInstrumentIDsByProductID;
    else
	{
        outData.clear();
		map<string, set<string> >::iterator it;
		for(it=m_allInstrumentIDsByProductID.begin();it!=m_allInstrumentIDsByProductID.end();)
		{
			if(it->second.size()>0&&
                (it2=m_allInstruments.find(*(it->second.begin())))!=m_allInstruments.end()&&
                string(it2->second.ExchangeID)==ExchangeID)
			{
                outData.insert(make_pair(it->first,it->second));
			}
		}
	}

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr_Instruments::GetProductID_InstrumentIDsByExchangeID-end");
}
///获取全部交易所的品种合约ID vector<pair<ExchangeID,vector<pair<ProductID,vector<InstrumentID>>>>>
void CPlatformDataMgr_Instruments::GetExchangeID_ProductID_InstrumentIDs(vector<pair<string,vector<pair<string,vector<string>>>>>& outData)
{
    LOG_INFO("CPlatformDataMgr_Instruments::GetExchangeID_ProductID_InstrumentIDs");

    outData.clear();
    Lock_CPlatformDataMgr;

    for(map<string,set<string>>::const_iterator itExchangeID=m_ProductIDsByExchangeID.begin();itExchangeID!=m_ProductIDsByExchangeID.end();itExchangeID++)
    {
        outData.push_back(make_pair(itExchangeID->first,vector<pair<string,vector<string>>>()));
        pair<string,vector<pair<string,vector<string>>>>& vecProductIDs=outData.back();

        for(set<string>::const_iterator it_ProductID=itExchangeID->second.begin();it_ProductID!=itExchangeID->second.end();it_ProductID++)
        {
            vecProductIDs.second.push_back(make_pair(*it_ProductID,vector<string>()));
            pair<string,vector<string>>& vecInstrumentIDs=vecProductIDs.second.back();

            map<string, set<string>>::const_iterator it_InstrumentIDs=m_allInstrumentIDsByProductID.find(*it_ProductID);
            if(it_InstrumentIDs!=m_allInstrumentIDsByProductID.end())
            {
                CTools_Ansi::ConvertSet2Vector<string>(it_InstrumentIDs->second,vecInstrumentIDs.second);
            }
        }
    }

    Unlock_CPlatformDataMgr;
    LOG_INFO("CPlatformDataMgr_Instruments::GetExchangeID_ProductID_InstrumentIDs-end");
}
///获取全部交易所的品种信息 vector<pair<ExchangeID,vector<PlatformStru_ProductInfo>>>
void CPlatformDataMgr_Instruments::GetExchangeID_ProductInfos(vector<pair<string,vector<PlatformStru_ProductInfo>>>& outData)
{
    LOG_INFO("CPlatformDataMgr_Instruments::GetExchangeID_ProductInfos");

    outData.clear();
    Lock_CPlatformDataMgr;

    for(map<string,set<string>>::const_iterator itExchangeID=m_ProductIDsByExchangeID.begin();itExchangeID!=m_ProductIDsByExchangeID.end();itExchangeID++)
    {
        outData.push_back(make_pair(itExchangeID->first,vector<PlatformStru_ProductInfo>()));
        vector<PlatformStru_ProductInfo>& vecProductInfos=outData.back().second;

        for(set<string>::const_iterator it_ProductID=itExchangeID->second.begin();it_ProductID!=itExchangeID->second.end();it_ProductID++)
        {
            map<string, PlatformStru_ProductInfo>::const_iterator itProductInfo=m_allProducts.find(*it_ProductID);
            if(itProductInfo!=m_allProducts.end())
            {
                vecProductInfos.push_back(itProductInfo->second);
            }
        }
    }

    Unlock_CPlatformDataMgr;
    LOG_INFO("CPlatformDataMgr_Instruments::GetExchangeID_ProductInfos-end");
}

///获取合约乘数，成功返回合约乘数，失败返回-1
int CPlatformDataMgr_Instruments::GetInstrumentMultiple(const string& InstrumentID)
{
    LOG_INFO("CPlatformDataMgr_Instruments::GetInstrumentMultiple");
    int Multiple;
    map<string,  PlatformStru_InstrumentInfo>::iterator it;

    Lock_CPlatformDataMgr;

    it =  m_allInstruments.find(InstrumentID);
    if(it==m_allInstruments.end())
        Multiple=-1;
    else 
        Multiple= it->second.VolumeMultiple;

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr_Instruments::GetInstrumentMultiple-end");
    return Multiple;
}

//将两个单腿合约合成组合合约，返回成功与否。合成时会检查合约列表里是否有该组合合约
bool CPlatformDataMgr_Instruments::GetComboInstrumentID(const string strInstrumentID1, 
                                                                 const string strInstrumentID2, 
                                                                 string& strComInstrumentID,
                                                                 string& strLeg1InstrumentID,
                                                                 string& strLeg2InstrumentID)
{
    PlatformStru_InstrumentInfo InstrumentInfo1,InstrumentInfo2,InstrumentInfo3;
    if(!GetInstrumentInfo(strInstrumentID1,InstrumentInfo1)||
        !GetInstrumentInfo(strInstrumentID2,InstrumentInfo2))
        return false;

    bool brlt=false;
    Lock_CPlatformDataMgr;

    map<string,string>::const_iterator it1;

    it1=m_Legs2Comb.find(strInstrumentID1+string("&")+strInstrumentID2);
    if(it1!=m_Legs2Comb.end())
    {
        strComInstrumentID=it1->second;
        strLeg1InstrumentID=strInstrumentID1;
        strLeg2InstrumentID=strInstrumentID2;
        brlt=true;
    }

    if(!brlt)
    {
        it1=m_Legs2Comb.find(strInstrumentID2+string("&")+strInstrumentID1);
        if(it1!=m_Legs2Comb.end())
        {
            strComInstrumentID=it1->second;
            strLeg1InstrumentID=strInstrumentID2;
            strLeg2InstrumentID=strInstrumentID1;
            brlt=true;
        }
    }

    Unlock_CPlatformDataMgr;

    return brlt;
}

bool CPlatformDataMgr_Instruments::GetLegInstrumentID(const string& strComInstrumentID,
                                    string& strLeg1InstrumentID,
                                    string& strLeg2InstrumentID)
{
    return GlobalFunc::IsCombInstrument(strComInstrumentID,strLeg1InstrumentID,strLeg2InstrumentID);
}



//获取交易所的交易日
string CPlatformDataMgr_Instruments::GetTradingDay(void)
{
    string ret;
    Lock_CPlatformDataMgr;
	if(m_exchangeTime.CurTickCount>0)
		ret=string(m_exchangeTime.TradingDay);
	else ret=string("");
    Unlock_CPlatformDataMgr;
    return ret;
}
//判断一个字符串日期是否是当天
bool CPlatformDataMgr_Instruments::IsToday(const char *OpenDate)
{
    int tradingday=0;

	if(OpenDate==NULL) return false;

    string Tradingday=GetTradingDay();

    tradingday=atoi(Tradingday.c_str());
    if(tradingday==0) 
    {
    	char OpenYear[5]={0}, OpenMonth[3]={0},OpenDay[3]={0};
		time_t tCurdate = time(NULL);
		struct tm local;
		localtime_s(&local, &tCurdate);
		tradingday= (local.tm_year+1900)*10000+(local.tm_mon+1)*100+local.tm_mday;
    }
    return (tradingday==atoi(OpenDate));
}

//获取交易所时间信息
unsigned long CPlatformDataMgr_Instruments::GetExchangeTime(const string& ExchangeID,string& time)
{
    Lock_CPlatformDataMgr;
    if(ExchangeID=="SHFE")
		time=m_exchangeTime.SHFETime;
	else if(ExchangeID=="DCE")
		time=m_exchangeTime.DCETime;
    else if(ExchangeID=="CZCE")
		time=m_exchangeTime.CZCETime;
	else if(ExchangeID=="CFFEX")
		time=m_exchangeTime.FFEXTime;
    unsigned long ret=m_exchangeTime.CurTickCount;
    Unlock_CPlatformDataMgr;
    return ret;
}

//获取交易所时间
int CPlatformDataMgr_Instruments::GetExchangeTime(ExchangeTime& outData)
{
    int irlt=0;
    Lock_CPlatformDataMgr;
    if(m_exchangeTime.CurTickCount==0)
    {
        memset(&outData,0,sizeof(outData));
        irlt=-1;
    }
    outData=m_exchangeTime;
    Unlock_CPlatformDataMgr;
    return irlt;
}
//设置交易所时间信息
void CPlatformDataMgr_Instruments::SetExchangeTime(const ExchangeTime& inData)
{
    Lock_CPlatformDataMgr;
    m_exchangeTime=inData;
    Unlock_CPlatformDataMgr;
}

//获取全部交易所
int CPlatformDataMgr_Instruments::GetExchangeIDs(vector<string>& outData)
{
	outData.clear();
	outData.push_back("SHFE");
	outData.push_back("CZCE");
	outData.push_back("DCE");
	outData.push_back("CFFEX");
	return outData.size();
}

void CPlatformDataMgr_Instruments::clear(void)
{
    Lock_CPlatformDataMgr;
    m_allProducts.clear();
    m_OptionUnderlyingProductIDsByExchangeID.clear();
    m_OptionInstrumentIDsByUnderlyingInstrumentID.clear();
    m_CallOptionInstrumentIDsByUnderlyingInstrumentID.clear();
    m_PutOptionInstrumentIDsByUnderlyingInstrumentID.clear();
    m_allInstruments.clear();
    m_allInstrumentList.clear();
    m_allInstrumentListWithoutComb.clear();
    m_allInstrumentIDsByProductID.clear();
    m_ProductIDsByExchangeID.clear();
    m_Leg2Comb.clear();
    m_Legs2Comb.clear();
    m_allMainInstrumentList.clear();
	m_allInstrumentVarietyMap.clear();
    Unlock_CPlatformDataMgr;
}

bool CPlatformDataMgr_Instruments::empty(void)
{
    Lock_CPlatformDataMgr;
    bool brlt=m_allInstruments.empty();
    Unlock_CPlatformDataMgr;
    return brlt;
}

///获取品种列表
int CPlatformDataMgr_Instruments::GetProductList(vector<string> &outData,const string ExchangeID)
{
    LOG_INFO("CPlatformDataMgr_Instruments::GetProductList");

    outData.clear();

    Lock_CPlatformDataMgr;

    if(ExchangeID.empty())
    {
        for(map<string, PlatformStru_ProductInfo>::const_iterator it=m_allProducts.begin();it!=m_allProducts.end();it++)
        {
            outData.push_back(it->first);
        }
    }
    else
    {
        for(map<string, PlatformStru_ProductInfo>::const_iterator it=m_allProducts.begin();it!=m_allProducts.end();it++)
        {
            if(string(it->second.ExchangeID)==ExchangeID)
                outData.push_back(it->first);
        }
    }

    int rltcnt=outData.size();

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr_Instruments::GetProductList-end");

    return rltcnt;
}
void CPlatformDataMgr_Instruments::GetProductInfos(vector<PlatformStru_ProductInfo> &outData,const string ExchangeID)
{
    LOG_INFO("CPlatformDataMgr_Instruments::GetProductInfos");

    outData.clear();

    Lock_CPlatformDataMgr;

    bool bExchangeIDEmpty=ExchangeID.empty();
    for(map<string, PlatformStru_ProductInfo>::const_iterator it=m_allProducts.begin();it!=m_allProducts.end();it++)
    {
        if(!bExchangeIDEmpty&&string(it->second.ExchangeID)!=ExchangeID) continue;
        outData.push_back(it->second);
    }

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr_Instruments::GetProductInfos-end");
}

bool CPlatformDataMgr_Instruments::GetProductInfo(const string& strInstrumentID,PlatformStru_ProductInfo& outData)
{
    Lock_CPlatformDataMgr;
    PlatformStru_InstrumentInfo InstrumentInfo;
    bool brlt=CTools_Ansi::GetValueInMap<string, PlatformStru_InstrumentInfo>(m_allInstruments,strInstrumentID,InstrumentInfo);
    if(brlt) brlt=CTools_Ansi::GetValueInMap<string, PlatformStru_ProductInfo>(m_allProducts,InstrumentInfo.ProductID,outData);
    Unlock_CPlatformDataMgr;
    return brlt;
}
bool CPlatformDataMgr_Instruments::GetProductInfo2(const string& strProductID,PlatformStru_ProductInfo& outData)
{
    Lock_CPlatformDataMgr;
    bool brlt=CTools_Ansi::GetValueInMap<string, PlatformStru_ProductInfo>(m_allProducts,strProductID,outData);
    Unlock_CPlatformDataMgr;
    return brlt;
}

//获取指定交易所的期权标的物期货品种
int CPlatformDataMgr_Instruments::GetOptionProductIDsByExchangeID(const string& ExchangeID,vector<string>& outProductIDs)
{
    Lock_CPlatformDataMgr;
    CTools_Ansi::GetValuesInMultimap<string,string>(m_OptionUnderlyingProductIDsByExchangeID,ExchangeID,outProductIDs);
    Unlock_CPlatformDataMgr;
    return outProductIDs.size();
}

//获取指定标的物期货合约对应的期权合约ID，返回目标期权合约的数量。UnderlyingInstrumentID为空则返回全部期权合约
int CPlatformDataMgr_Instruments::GetOptionInstrumentIDsByUnderlyingInstrumentID(const string& UnderlyingInstrumentID,vector<string>& outOptionInstrumentIDs)
{
    Lock_CPlatformDataMgr;
    if(!UnderlyingInstrumentID.empty())
        CTools_Ansi::GetValuesInMultimap<string,string>(m_OptionInstrumentIDsByUnderlyingInstrumentID,UnderlyingInstrumentID,outOptionInstrumentIDs);
    else 
        CTools_Ansi::GetAllValuesInMultimap<string,string>(m_OptionInstrumentIDsByUnderlyingInstrumentID,outOptionInstrumentIDs);
    Unlock_CPlatformDataMgr;
    return outOptionInstrumentIDs.size();
}
//获取指定标的物期货合约对应的看涨期权合约ID数组和看跌期权合约ID数组。UnderlyingInstrumentID为空则返回全部期权合约
void CPlatformDataMgr_Instruments::GetOptionInstrumentIDsByUnderlyingInstrumentID(const string& UnderlyingInstrumentID,vector<string>& outCallOptionInstrumentIDs,vector<string>& outPutOptionInstrumentIDs)
{
    Lock_CPlatformDataMgr;
    if(!UnderlyingInstrumentID.empty())
    {
        CTools_Ansi::GetValuesInMultimap<string,string>(m_CallOptionInstrumentIDsByUnderlyingInstrumentID,UnderlyingInstrumentID,outCallOptionInstrumentIDs);
        CTools_Ansi::GetValuesInMultimap<string,string>(m_PutOptionInstrumentIDsByUnderlyingInstrumentID,UnderlyingInstrumentID,outPutOptionInstrumentIDs);
    }
    else 
    {
        CTools_Ansi::GetAllValuesInMultimap<string,string>(m_CallOptionInstrumentIDsByUnderlyingInstrumentID,outCallOptionInstrumentIDs);
        CTools_Ansi::GetAllValuesInMultimap<string,string>(m_PutOptionInstrumentIDsByUnderlyingInstrumentID,outPutOptionInstrumentIDs);
    }
    Unlock_CPlatformDataMgr;
}

//获取所有标的物期货合约
int CPlatformDataMgr_Instruments::GetAllOptionUnderlyingInstrumentIDs(vector<string>& outUnderlyingInstrumentIDs)
{
    Lock_CPlatformDataMgr;
    CTools_Ansi::GetAllKeysInMultimap<string,string>(m_OptionInstrumentIDsByUnderlyingInstrumentID,outUnderlyingInstrumentIDs);
    Unlock_CPlatformDataMgr;
    return outUnderlyingInstrumentIDs.size();
}
//获取所有标的物期货合约
int CPlatformDataMgr_Instruments::GetAllOptionUnderlyingInstrumentIDs(set<string>& outUnderlyingInstrumentIDs)
{
    Lock_CPlatformDataMgr;
    CTools_Ansi::GetAllKeysInMultimap<string,string>(m_OptionInstrumentIDsByUnderlyingInstrumentID,outUnderlyingInstrumentIDs);
    Unlock_CPlatformDataMgr;
    return outUnderlyingInstrumentIDs.size();
}

//判断是否是期权标的物期货合约
bool CPlatformDataMgr_Instruments::IsUnderlyingInstrumentID(const string& InstrumentID)
{
    Lock_CPlatformDataMgr;
    bool brlt=m_OptionInstrumentIDsByUnderlyingInstrumentID.find(InstrumentID)==m_OptionInstrumentIDsByUnderlyingInstrumentID.end() ? false : true;
    Unlock_CPlatformDataMgr;
    return brlt;
}
