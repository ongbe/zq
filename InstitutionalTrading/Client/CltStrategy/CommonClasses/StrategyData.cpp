
#include "stdafx.h"
#include "StrategyData.h"

CStrategyData::CStrategyData()
{
	m_nPrivate = 1;
	m_bHasUpload = false;
	m_nServerUseStatus = 0;
	m_nInstrumentCount = 0;
}

CStrategyData::~CStrategyData()
{
}
/*增加一条策略参数*/
void CStrategyData::AddStrategyPara(StrategyPara& data)
{
	m_mapStrategyPara[data.szTitle] = data;
}
/*删除一条策略参数*/
void CStrategyData::DeleteStrategyPara( const string& name)
{
	std::map<std::string, StrategyPara>::iterator it =m_mapStrategyPara.find(name);
	if (it != m_mapStrategyPara.end())
	{
		m_mapStrategyPara.erase(it);
	}
	map<string,list<StrategyPrarSubItem>> ::iterator it2 =m_mapPrarSubItem.find(name);
		if (it2 != m_mapPrarSubItem.end())
		{
			m_mapPrarSubItem.erase(it2);
		}
	int nCnt = m_multimapstrategyParaType2Name.count(name);
	multimap<string,string>::iterator it3 = m_multimapstrategyParaType2Name.find(name);
	for ( int i = 0; i< nCnt; i++)
	{					 

		string title(name);
		if ( it3->second  == title)
		{
			it3 = m_multimapstrategyParaType2Name.erase(it3);
			break;
		}
		else
		{
			++it3;
		}

	}

}
/*增加一条策略指标*/
void CStrategyData::AddStrategyIndicator(const string& Strategyname,StrategyIndicator& data)
{
	m_mapStrategyIndicator[data.szTitle] = data;
	m_mapIndicator.insert(make_pair(Strategyname,data.szName));
}
bool CStrategyData::GetStrategyIndicator(const string& szName,StrategyIndicator& data)
{
	std::map<std::string,StrategyIndicator>::iterator it =   m_mapStrategyIndicator.find(szName);
	if (it!= m_mapStrategyIndicator.end())
	{
		data = it->second;
		return true;
	}
	else
		return false;

}
/*删除一条策略指标*/
void CStrategyData::DeleteStrategyIndicator(const string& Strategyname,const string& Indicatorname)
{
	std::map<std::string, StrategyIndicator>::iterator it =m_mapStrategyIndicator.find(Indicatorname);
	if (it != m_mapStrategyIndicator.end())
	{
		m_mapStrategyIndicator.erase(it);
	}

	map<string,string> ::iterator it2 = m_mapIndicator.find(Indicatorname);
	if(it2!=m_mapIndicator.end())
	{					 
		m_mapIndicator.erase(Indicatorname);
	}
}
void CStrategyData::GetIndicator(list<string>& data)
{

	map<string,string>::iterator it= m_mapIndicator.begin();
	for( it;it!=m_mapIndicator.end();it++)
	{
		data.push_back(it->second);
	}

}
/*设置策略参数单选框下拉框时的选项*/
void CStrategyData::SetParaSubItem(const string&szTitle,list<StrategyPrarSubItem>& ListPrarSubItem )
{
    m_mapPrarSubItem[szTitle] = ListPrarSubItem;
}
/*得到策略参数单选框下拉框时的选项*/
void CStrategyData::GetParaSubItem(map<string,list<StrategyPrarSubItem>>&  PrarSubItem)
{
	PrarSubItem = m_mapPrarSubItem;
}
void CStrategyData::GetSubItem(const string name, list<StrategyPrarSubItem>& listSubItem)
{

	map<string,list<StrategyPrarSubItem>>::iterator it =m_mapPrarSubItem.find(name);
	if (it != m_mapPrarSubItem.end())
	{
		listSubItem =it->second;
	}
	

}
bool CStrategyData::CheckStrategyParaName(const std::string& name)
{
	std::map<std::string,StrategyPara>::iterator it = m_mapStrategyPara.find(name);
	if (it == m_mapStrategyPara.end())
	{
		return true;//没重名的
	}
	else
		return false;
}
void CStrategyData::SetParaType2NameMap(multimap<string,string>& data)
{
	m_multimapstrategyParaType2Name.clear();
	m_multimapstrategyParaType2Name = data;
}
void CStrategyData::GetParaType2NameMap(multimap<string,string>& data)
{
	data = m_multimapstrategyParaType2Name;
}
/*得到所有策略参数map*/
void CStrategyData::GetStrategyPara(std::map<std::string,StrategyPara>& data)
{
	data =m_mapStrategyPara;
}
std::map<std::string,StrategyPara>& CStrategyData::GetStrategyParams()
{
	return m_mapStrategyPara;
}

/*得到新建策略的所有指标配置*/
void CStrategyData::GetAllStrategyIndicator(std::map<std::string,StrategyIndicator>& lMapFiled)
{
	lMapFiled = m_mapStrategyIndicator;
}
const string CStrategyData::GetStrategyName()
{
	return m_strategyName;
}
void CStrategyData::SetStrategyName(const string name)
{
	m_strategyName = name;

}
const string CStrategyData::GetStrategyNickName()
{
	return m_strategyNickName;

}
void CStrategyData::SetStrategyNickName(const string name)
{
	m_strategyNickName = name;
}
const string CStrategyData::GetStrategyVersion()
{
	return m_Version;
}
void CStrategyData::SetStrategyVersion(const string Version)
{
	m_Version = Version;
}
void  CStrategyData::SetStrategyPrivate(const int data)
{
	m_nPrivate = data;
}
int  CStrategyData::GetStrategyPrivate()
{
	return m_nPrivate;
}

bool CStrategyData::IsOwner(const string& strCurrUserID)
{
	return m_strOwner.size()==0 || strCurrUserID.compare(m_strOwner)==0;
}

void CStrategyData::SetOwner(const string& strName)
{
	m_strOwner = strName;
}

string CStrategyData::GetOwner()
{
	return m_strOwner;
}

bool CStrategyData::HasUpload()
{
	return m_bHasUpload;
}

void CStrategyData::ResetUpdoad(bool bHas)
{
	m_bHasUpload = bHas;
}

int CStrategyData::GetServerUseStatus()
{
	return m_nServerUseStatus;
}

void CStrategyData::SetServerUseStatus(int nStatus)
{
	m_nServerUseStatus = nStatus;
}

void CStrategyData::SetDescribe(const string& strDescribe)
{
	m_strDescribe = strDescribe;
}
string CStrategyData::GetDescribe()
{
	return m_strDescribe;
}
int CStrategyData::GetInstrumentCount()
{
	return m_nInstrumentCount;
}
void CStrategyData::SetInstrumentCount(int nInstrumentCount)
{
	 m_nInstrumentCount = nInstrumentCount;
}