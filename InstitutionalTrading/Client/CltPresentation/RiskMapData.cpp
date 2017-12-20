#include "StdAfx.h"
#include "RiskmapData.h"

#include "TcpLayer.h"
#include "CDataInfo.h"
#include "CommonDef.h"
#include "CommonPkg.h"


//添加一个统计数据，智能判断是更新还是新增
bool CltPresentation::CRiskMapData::PushStatics(Int32 nOrgID,  //组织OID
		  StatisticsEntry* npStatics)  //统计数据
		  
{
	if(npStatics == NULL)
		return false;

	bool lbUpdate = false;
	RiskMapStatisticMap::iterator lIteratorStart,lIteratorEnd;
	lIteratorStart = mmapStatics.lower_bound(nOrgID);
	lIteratorEnd = mmapStatics.upper_bound(nOrgID);	
	while(lIteratorStart != lIteratorEnd && lIteratorStart != mmapStatics.end())
	{
		if(memcmp(npStatics,&lIteratorStart->second,sizeof(StatisticsEntry)-sizeof(double))==0)
		{
			lbUpdate = true;
			lIteratorStart->second.mdblValue = npStatics->mdblValue;
			break;
		}
		lIteratorStart++;
	}
	if(lbUpdate == false)
	{
		mmapStatics.insert(std::make_pair(nOrgID,*npStatics));
	}
	

	return true;
}
//删除一个组织的所有相关项
bool CltPresentation::CRiskMapData::RemoveOrgStatics(Int32 nOrgID)
{
	mmapStatics.erase(nOrgID);
	return true;
	//RiskMapStatisticMap::iterator lIteratorStart,lIteratorEnd;
	//lIteratorStart = mmapStatics.lower_bound(nOrgID);
	//lIteratorEnd = mmapStatics.upper_bound(nOrgID);	
	//while(lIteratorStart != lIteratorEnd && lIteratorStart != mmapStatics.end())
	//{
	//	mmapStatics.erase(nOrgID);
	//	lIteratorStart++;
	//}
}


//添加一个部门理财产品数据，智能判断是更新还是新增
bool CltPresentation::CRiskMapData::PushFinacialProductStatics(Int32 nFinancial,  //组织OID
				 StatisticsEntry* npStatics)  //统计数据	
{
	if(npStatics == NULL)
		return false;

	bool lbUpdate = false;
	RiskMapStatisticMap::iterator lIteratorStart,lIteratorEnd;
	lIteratorStart = mmapFinacialProducctStatics.lower_bound(nFinancial);
	lIteratorEnd = mmapFinacialProducctStatics.upper_bound(nFinancial);	
	while(lIteratorStart != lIteratorEnd && lIteratorStart != mmapFinacialProducctStatics.end())
	{
		if(memcmp(npStatics,&lIteratorStart->second,sizeof(StatisticsEntry)-sizeof(double))==0)
		{
			lbUpdate = true;
			lIteratorStart->second.mdblValue = npStatics->mdblValue;
			break;
		}
		lIteratorStart++;
	}
	if(lbUpdate == false)
	{
		mmapFinacialProducctStatics.insert(std::make_pair(nFinancial,*npStatics));
	}
	//if(npStatics == NULL)
	//	return false;

	//bool lbUpdate = false;
	//RiskMapStatisticMap::iterator lIteratorStart,lIteratorEnd;
	//lIteratorStart = mmapFinacialProducctStatics.lower_bound(nOrgID);
	//lIteratorEnd = mmapFinacialProducctStatics.upper_bound(nOrgID);	
	//while(lIteratorStart != lIteratorEnd && lIteratorStart != mmapFinacialProducctStatics.end())
	//{
	//	if(memcmp(npStatics,&lIteratorStart->second,sizeof(StatisticsEntry)-sizeof(double))==0)
	//	{
	//		lbUpdate = true;
	//		lIteratorStart->second.mdblValue = npStatics->mdblValue;
	//		break;
	//	}
	//	lIteratorStart++;
	//}
	//if(lbUpdate == false)
	//{
	//	mmapFinacialProducctStatics.insert(std::make_pair(nOrgID,*npStatics));
	//}


	return true;
}