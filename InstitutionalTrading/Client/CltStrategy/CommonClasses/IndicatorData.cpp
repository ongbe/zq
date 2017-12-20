
#include "stdafx.h"
#include "IndicatorData.h"

CIndicatorData::CIndicatorData()
{
	m_eIndiType = MDIndicator;
	m_nPrivate = 1;
	m_bHasUpload = false;
	m_nServerUseStatus = 0;
}

CIndicatorData::~CIndicatorData()
{
}
bool CIndicatorData::CheckName(const std::string& name)
{
	std::map<std::string,IndicatorPara>::iterator it = m_mapIndcatorPara.find(name);
	if (it == m_mapIndcatorPara.end())
	{
		return true;//没重名的
	}
	else
		return false;
}

/*增加一条指标参数*/
void CIndicatorData::AddIndcatorPara(IndicatorPara& data)
{
	m_mapIndcatorPara[data.szName] = data;
}
/*删除一条指标参数*/
void CIndicatorData::DeleteIndcatorPara( const std::string& name)
{
	std::map<std::string, IndicatorPara>::iterator it =m_mapIndcatorPara.find(name);
	if(it != m_mapIndcatorPara.end())
		m_mapIndcatorPara.erase(it);
}

/*新建指标时清空指标参数MAP*/
void CIndicatorData::ClearIndiPara( )
{
	m_mapIndcatorPara.clear();

}
/*得到一条指标的所有参数*/
void CIndicatorData::GetIndiPara(std::map<std::string, IndicatorPara> &  lMapFiled)
{

	lMapFiled = m_mapIndcatorPara;

}
const std::string CIndicatorData::GetIndicatorName()
{
	return m_szName;
}
void CIndicatorData::SetIndicatorName(const std::string name)
{
    m_szName =name;
}
std::string CIndicatorData::GetIndicatorDescribe()
{
	return m_szDescribe;
}
void CIndicatorData:: SetIndicatorDescribe(const std::string data)
{
	m_szDescribe = data;
}
void CIndicatorData::SetIndicatorType( IndicatorType data)
{
	m_eIndiType = data;
}
IndicatorType CIndicatorData::GetIndicatorType()
{
	return m_eIndiType;
}
void CIndicatorData::SetIndicatorFile(const std::string data)
{
	m_szIndiFile = data;
}
std::string CIndicatorData::GetIndicatorFile()
{
	return m_szIndiFile;
}
void  CIndicatorData::SetIndicatorPrivate(const int data)
{
    m_nPrivate = data;
}
int  CIndicatorData::GetIndicatorPrivate()
{
    return m_nPrivate;
}

bool CIndicatorData::IsOwner(const std::string& strCurrUserID)
{
	return m_strOwner.size()==0 || strCurrUserID.compare(m_strOwner)==0;
}

void CIndicatorData::SetOwner(const std::string& strName)
{
	m_strOwner = strName;
}

std::string CIndicatorData::GetOwner()
{
	return m_strOwner;
}

bool CIndicatorData::HasUpload()
{
	return m_bHasUpload;
}

void CIndicatorData::ResetUpdoad(bool bHas)
{
	m_bHasUpload = bHas;
}

int CIndicatorData::GetServerUseStatus()
{
	return m_nServerUseStatus;
}

void CIndicatorData::SetServerUseStatus(int nStatus)
{
	m_nServerUseStatus = nStatus;
}
