
#pragma once

#include <cliext/map>
using namespace System;
using namespace System::Data;

using namespace CFMap::WindowsForms;
using namespace CFMap::CoreType;
//因为组织ID唯一,可使用map实现
namespace RiskMap {


ref class CSingleOrgnization
{
private:	
	Int32  mnOrgId;                  //组织ID
	PointLatLng mPointLatLng;        //经纬度
	String^ msName;                  //组织名称
	Int32 mnOrgLevel;                //组织级别 1到10级
	RiskLevel mLevel;                //分险级别
	String^ msRiskDes;               //风险描述
	CSingleOrgnization ^ mParentOrg; //父机构
	CMarkerRisk ^ mrMaker;           //用于地图显示的风险标记
	Int32 mnRiskCount;               //风险总数量
    Int32 mnSingalIntrusmentMaxHode; //单合约最大持仓预警数量
    Int32 mnLimitedLost;             //限亏预警数量
	Int32 mnHodeDanger;              //持仓风险敞口预警数量
    Int32 mnScaleLevel;              //缩放级别

public:
	CSingleOrgnization(Int32 nID,PointLatLng np,String^ nsName,Int32 nOrgLevel,RiskLevel nLevel,String^ nRiskDes,Int32 nScaleLevel,CSingleOrgnization ^ nrParent):
	      mnOrgId(nID),
		  mPointLatLng(np),
		  msName(nsName),
		  mnOrgLevel(nOrgLevel),
		  mLevel(nLevel),
		  msRiskDes(nRiskDes),
		  mnScaleLevel(nScaleLevel),
		  mParentOrg(nrParent)
	{
        mrMaker = nullptr;
		mnSingalIntrusmentMaxHode = 10;
		mnLimitedLost = 5;
		mnHodeDanger = 9;
		mnRiskCount = 24;
	}

    CSingleOrgnization()
	{
		msRiskDes = nullptr;
		mParentOrg = nullptr;
		msName = nullptr;
		mnOrgId = -1;
		mLevel = RiskLevel::RiskLevel_none;
		mrMaker = nullptr;
	}

	property CMarkerRisk ^ MakerRef
	{
		CMarkerRisk^ get() { return mrMaker; }
		void set(CMarkerRisk^ nm) { mrMaker = nm; }
	}
	
	property Int32  ID
	{
		Int32 get() { return mnOrgId; }
		void set(Int32 nm) { mnOrgId = nm; }
	}

	property String ^ Name
	{
		String^ get() { return msName; }
		void set(String^ nm) { msName = nm; }
	}

	property PointLatLng  LatLng
	{
		PointLatLng get() { return mPointLatLng; }
		void set(PointLatLng nm) { mPointLatLng = nm; }
	}
	property Int32  OrgLevel
	{
		Int32 get() { return mnOrgLevel; }
		void set(Int32 nm) { mnOrgLevel = nm; }
	}

	property RiskLevel  OrgRiskLevel
	{
		RiskLevel get() { return mLevel; }
		void set(RiskLevel nm) { mLevel = nm; }
	}

	property CSingleOrgnization ^  ParentOrg
	{
		CSingleOrgnization ^ get() { return mParentOrg; }
		void set(CSingleOrgnization ^ nm) { mParentOrg = nm; }
	}

	property String^   RiskDesc
	{
		String^  get() { return msRiskDes; }
		void set(String^  nm) { msRiskDes = nm; }
	}


	property Int32  RiskCount
	{
		Int32 get() { return mnRiskCount; }
		void set(Int32 nm) { mnRiskCount = nm; }
	}
	property Int32  SingalIntrusmentMaxHodeCount
	{
		Int32 get() { return mnSingalIntrusmentMaxHode; }
		void set(Int32 nm) { mnSingalIntrusmentMaxHode = nm; }
	}
	property Int32  LimitedLostCount
	{
		Int32 get() { return mnLimitedLost; }
		void set(Int32 nm) { mnLimitedLost = nm; }
	}
	property Int32  HoldDangerCount
	{
		Int32 get() { return mnHodeDanger; }
		void set(Int32 nm) { mnHodeDanger = nm; }
	}
	property Int32  ScaleLevel
	{
		Int32 get() { return mnScaleLevel; }
		void set(Int32 nm) { mnScaleLevel = nm; }
	}
};


typedef cliext::map<Int32, CSingleOrgnization^> TOrgnizationMap;
//typedef TOrgnizationMap::pair_iter_iter PairiIterator;
ref class COrgnization
{
private:
	TOrgnizationMap^ mmapOrg;
	Int32 mnDeep;
	Int32 mnMaxScaleLevel;
	
public:
	static Int32 INVALID_PARENT_ID=0;
	static Int32 MaxDeep = 10;
	static Int32 MinDeep = 1;
public:
	COrgnization()
	{
		mmapOrg = gcnew TOrgnizationMap();	

	}
	property Int32 Deep
	{
		Int32 get() { return mnDeep; }
		void set(Int32 nDeep) { mnDeep = nDeep; }
	}
	property Int32 MaxScaleLevel
	{
		Int32 get() { return mnMaxScaleLevel; }
		void set(Int32 nVal) { mnMaxScaleLevel = nVal; }
	}

	property TOrgnizationMap^ OrgMap
	{
		TOrgnizationMap^ get() { return mmapOrg; }
		void set(TOrgnizationMap^ nmap) { mmapOrg = nmap; }
	}
	//reset all to a risklevel
	void ResetAllToRiskLevel(RiskLevel nRiskLevel)
	{
		for each (TOrgnizationMap::value_type empl in mmapOrg)
		{
			empl->second->OrgRiskLevel = nRiskLevel;
		}
	}

	//根据一个组织结构的风险状态,来修改其所有的父的状态,如果状态为normal 则不执行动作
	//如果父级状态大于当前状态，则停止
	void UpdateOrgMapStatus(Int32 nID,RiskLevel nRiskLevel,String^ nRiskDes)
	{
		if(nRiskLevel == RiskLevel::RiskLevel_normal || nRiskLevel == RiskLevel::RiskLevel_none)
			return;
		//先根据ID找到组织
		TOrgnizationMap::iterator lIte = mmapOrg->find(nID);
		if(lIte != mmapOrg->end())
		{
			lIte->second->OrgRiskLevel  = nRiskLevel;
			lIte->second->RiskDesc  = nRiskDes;
			CSingleOrgnization^ lParent = lIte->second->ParentOrg;
			while(lParent != nullptr)
			{
				if(lParent->OrgRiskLevel >= nRiskLevel)
				{
					break;
				}
				else
					lParent->OrgRiskLevel = nRiskLevel;

				lParent = lParent->ParentOrg;
			}

		}
	}
	//根据ID来查找
	CSingleOrgnization ^ Find(int nID)
	{
		TOrgnizationMap::iterator lIte = mmapOrg->find(nID);
		if(lIte != mmapOrg->end())
		{
			return lIte->second;
		}
       
		return nullptr;
	}

	//插入一个组织
	bool Insert(Int32 nID,PointLatLng np,String^ nOrgName,Int32 nOrgLevel,RiskLevel nRiskLevel,String^ nRiskDes,Int32 nScaleLevel,Int32 nParentID)
	{
		////察看是不是root node
		if(nParentID == INVALID_PARENT_ID) 
		{
			CSingleOrgnization ^ lrOrg = gcnew CSingleOrgnization(nID,np,nOrgName,nOrgLevel,nRiskLevel,nRiskDes,nScaleLevel,nullptr);
			mmapOrg->insert(TOrgnizationMap::make_value(nID,lrOrg));
			//更新组织深度
			Deep = nOrgLevel;
			mnMaxScaleLevel = nScaleLevel;
			return true;
		}

		//Find Parent
		CSingleOrgnization ^ lParent = Find(nParentID);
		if(lParent != nullptr)
		{
			CSingleOrgnization ^ lrOrg = gcnew CSingleOrgnization(nID,np,nOrgName,nOrgLevel,nRiskLevel,nRiskDes,nScaleLevel,lParent);
			mmapOrg->insert(TOrgnizationMap::make_value(nID,lrOrg));
			//更新组织深度
			if(nOrgLevel > Deep)
				Deep = nOrgLevel;
			if(nScaleLevel > mnMaxScaleLevel)
				MaxScaleLevel = nScaleLevel;
			return true;
		}

		return false;
	}

	//删除一个组织
	bool Remove(Int32 nID)
	{
		TOrgnizationMap::iterator lIte = mmapOrg->find(nID);
		if(lIte != mmapOrg->end())
		{
			mmapOrg->erase(lIte);
			return true;
		}
		return false;
	}

	//是否有子结点,如果有，返回多少个，如没有返回0
	int HasChild(Int32 nID)
	{
		TOrgnizationMap::iterator lIre = mmapOrg->begin();
		Int32 lnCount = 0;
		for(;lIre != mmapOrg->end();lIre++)
		{
			if(lIre->second->ParentOrg != nullptr && lIre->second->ParentOrg->ID == nID)
				++lnCount;
		}
		return lnCount;
	}

	Int32 Size()
	{
		return mmapOrg->size();
	}

	//获取第一个子组织
	CSingleOrgnization ^ GetFirstChildOrg(Int32 nParentID)
	{
		TOrgnizationMap::iterator lIre = mmapOrg->begin();	
		if(nParentID == INVALID_PARENT_ID && lIre != mmapOrg->end())
			return lIre->second;
			
		for(;lIre != mmapOrg->end();lIre++)
		{
			if(lIre->second->ParentOrg != nullptr && lIre->second->ParentOrg->ID == nParentID)
			{
				return lIre->second;
			}
		}
		return nullptr;
	}

	//获取下一个子组织
	CSingleOrgnization ^ GetNextChildOrg(Int32 nParentID,CSingleOrgnization ^ nrPreChildOrg)
	{
		if(nrPreChildOrg == nullptr)
			return nullptr;

		TOrgnizationMap::iterator lIre = mmapOrg->find(nrPreChildOrg->ID);	
		lIre++;
		for(;lIre != mmapOrg->end();lIre++)
		{
			if(lIre->second->ParentOrg != nullptr && lIre->second->ParentOrg->ID == nParentID)
			{
				return lIre->second;
			}
		}
		return nullptr;
	}

	//获取一个层次的第一个节点
	CSingleOrgnization^ GetFirstOrgOfThisLevel(Int32 nOrgLevel)
	{
		TOrgnizationMap::iterator lIre = mmapOrg->begin();		

		for(;lIre != mmapOrg->end();lIre++)
		{
			if( lIre->second->OrgLevel == nOrgLevel)
			{
				return lIre->second;
			}
		}
		return nullptr;
	}

	//获取一个层次的下一个节点
	CSingleOrgnization ^ GetNextOrgOfThisLevel(Int32 nOrgLevel,CSingleOrgnization ^ nrPreOrg)
	{
		if(nrPreOrg == nullptr)
			return nullptr;

		TOrgnizationMap::iterator lIre = mmapOrg->find(nrPreOrg->ID);	
		lIre++;
		for(;lIre != mmapOrg->end();lIre++)
		{
			if( lIre->second->OrgLevel == nOrgLevel)
			{
				return lIre->second;
			}
		}
		return nullptr;
	}

	//得到一个级别的Org并且有子结点 谁离中心点最近
	CSingleOrgnization ^ CheckWhoCloserCenter(Int32 nOrgLevel,PointLatLng npCenter)
	{
		if(nOrgLevel > MaxDeep || nOrgLevel < MinDeep)
			return nullptr;
		TOrgnizationMap::iterator lIre = mmapOrg->begin();		
		TOrgnizationMap::iterator lIterCloser = lIre;
		double lnMin = 10000000000000.0;
		for(;lIre != mmapOrg->end();lIre++)
		{
			if( lIre->second->OrgLevel == nOrgLevel && HasChild(lIre->second->ID))
			{
				double ldbTemp = npCenter.LengthTwoPoint(lIre->second->LatLng);
				if(lnMin > ldbTemp)
				{
					lIterCloser = lIre;
					lnMin = ldbTemp;
				}
			}
		}
		return lIterCloser->second;
	}

};

}