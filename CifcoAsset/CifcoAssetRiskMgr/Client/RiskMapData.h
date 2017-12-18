#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include <set>
#include <map>
#include <queue>
#include <string>
#include "CDataInfo.h"
#include "UserApiStruct.h"
#include "CommonPkg.h"
#include "easymutex.h"

//using namespace std;
namespace Client {

	typedef std::multimap<AssetMgmtOrgIDType, StatisticsEntry> RiskMapStatisticMap;
	ref class TotalStatiscData
	{
	public:
		double mdbCCYK; //
		double mdbCCQY;
		double mdbDQQY;
		double mdbBZJ;
		double mdbCCL;

	public:
		TotalStatiscData()
		{
			mdbCCL = mdbBZJ = mdbCCQY = mdbCCYK = mdbDQQY = 0.0;
		}
		void Add(StatisticsIDType nType,double ndbVal)
		{
			if(nType == ST_PosProfit)
				mdbCCYK += ndbVal;
			else if(nType == ST_InitFund)
				mdbCCQY += ndbVal;
			else if(nType == ST_Balance)
				mdbDQQY += ndbVal;
			else if(nType == ST_UseMargin)
				mdbBZJ += ndbVal; 
			else if(nType == ST_Position)
				mdbCCL += ndbVal; 
		}
		static Double SafeDivMulti100(Double nDbNumerator,Double nDbDenominator)
		{
			if(Math::Abs(nDbDenominator) < 0.000001)
				return 0;
			else
				return (nDbNumerator / nDbDenominator)*100  ;
			
		}
		static Double SafeDiv(Double nDbNumerator,Double nDbDenominator)
		{
			if(Math::Abs(nDbDenominator) < 0.000001)
				return 0.0;
			else
				return nDbNumerator / nDbDenominator;
		}
		static Double GetNearestTen(Double ndbVal)
		{
           double ldbCount = 1;
		   double ldbTemp = Math::Abs(ndbVal);
		   while(ldbTemp > 1)
		   {
			  ldbTemp /= 10;
			  ldbCount *= 10;
		   }
		   return  ldbCount;

		}
	};
	class CRiskMapData {
	public:
		static CRiskMapData* NewInstance() {
			return new CRiskMapData();
		}
		static void ReleaseInstance(CRiskMapData* instance) {
			if(instance!=NULL) 
				delete instance;
		}

	private:
		CRiskMapData(void) {
			
		}
		virtual ~CRiskMapData(void) {
			//清理数据
			mStaticsAccountVector.clear();
			mStaticsFinacalProductVector.clear();
			mStaticsVector.clear();
			mmapStatics.clear();
			mmapFinacialProducctStatics.clear();

		}

	public:
		// LockObject()与UnloadObject()这两个函数必须成对调用
		void LockObject() {
			oMutex.lock();
		}
		void UnlockObject() {
			oMutex.unlock();
		}


		//添加一个统计数据，智能判断是更新还是新增
		bool PushStatics(Int32 nOrgID,  //组织OID
			StatisticsEntry* ns,
			DataTable ^ nOrgHZDataTable);  //统计数据

		//删除一个组织的所有相关项
		bool RemoveOrgStatics(Int32 nOrgID);
		
		//添加一个部门理财产品数据，智能判断是更新还是新增
		bool PushFinacialProductStatics(Int32 nOrgID,  //组织OID
			StatisticsEntry* ns);  //统计数据	
		
		RiskMapStatisticMap mmapStatics;                                //汇总数据
		RiskMapStatisticMap mmapFinacialProducctStatics;                //某部门理财产品汇总数据

		std::vector<AssetOrgProRelation> mStaticsVector;                //订阅的汇总数组
		std::vector<AssetOrgProRelation> mStaticsFinacalProductVector;  //订阅的当前部门的理财产品的数组
	
		std::vector<int> mStaticsAccountVector;                          //


		MAPFinancialProduct mmapFinanProduct;                            //产品ID列表
	private:		
		Ceasymutex oMutex;
		
		
	};
}
