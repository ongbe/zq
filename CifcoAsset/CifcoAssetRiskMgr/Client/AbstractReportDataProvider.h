#pragma once
#include <cliext/set>
#include <cliext/map>
#include <cliext/vector>
#include "Tools.h"
using namespace System;
using namespace System::Data;



namespace ReportDataProvider {

	//为一个int32型的数组 ，账号ID列表
	public ref class  CQueryConditions
	{
	public:
		array<Int32>^ marrayAccount;   //交易帐号列表
		DateTime   mstartTime;         //开始时间
		DateTime   mendTime;           //结束时间
		array<Int32>^ marrayInstructID;//合约ID数组
		
	};


	typedef cliext::set<String^> FilterAccontSetType;
    typedef cliext::map<String^,cliext::vector<Double>^> TimeStaticBalanceType;
	typedef cliext::map<String^,cliext::vector<Int32>^> TimeStaticHodeType;
	public ref class IReportDataProvider abstract
	{	

	private:
		System::Data::DataTable ^ mrDataTable;   //统一返回的大表
		FilterAccontSetType^ mFilterAccountSet;  //过滤账号
	    FilterAccontSetType^ mFilterInstrument;  //过滤合约
	public:
		
		IReportDataProvider()
		{
			mrDataTable = gcnew System::Data::DataTable();
			mFilterAccountSet = gcnew cliext::set<String^>();
			mFilterInstrument = gcnew cliext::set<String^>();
		}
		property DataTable ^ RealDataTable
		{
			DataTable ^ get() {return mrDataTable;}
			Void set(DataTable ^ nrTable) { mrDataTable = nrTable;}
		}
		/*property Boolean GroupQuery
		{
			Boolean  get() {return mbGroupQuery;}
			Void set(Boolean  nbVal) { mbGroupQuery = nbVal;}
		}  */
		void ClearFilterInstrument()
		{
			mFilterInstrument->clear();
		}
		void ClearFilterAccount()
		{
			mFilterAccountSet->clear();
		}

		void SetFilterAccountSet(std::set<std::string> & nSet)
		{
			mFilterAccountSet->clear();
			std::set<std::string>::iterator lite = nSet.begin();
			while(lite != nSet.end())
			{
				mFilterAccountSet->insert(Tools::string2String((*lite).c_str()));
				lite++;
			}
		}
		void SetFilterInstrument(std::set<std::string> & nSet)
		{
			mFilterInstrument->clear();
			std::set<std::string>::iterator lite = nSet.begin();
			while(lite != nSet.end())
			{
				mFilterInstrument->insert(Tools::string2String((*lite).c_str()));
				lite++;
			}
		}
		bool FilterAccount(std::string nStr)
		{
			if(mFilterAccountSet->find(Tools::string2String(nStr.c_str())) == mFilterAccountSet->end())
				return true;
			return false;
		}
		bool FilterInstrument(std::string nStr)
		{
			//如果没有设置过滤合约，则全部不过滤
			if(mFilterInstrument->size() == 0)
				return false; 

			if(nStr.empty())
				return true;

			//先找合约，再找品种
			if(mFilterInstrument->find(Tools::string2String(nStr.c_str())) == mFilterInstrument->end())
			{
				//找前两位品种代码		
				nStr.erase(2,nStr.length() - 2);
				if(mFilterInstrument->find(Tools::string2String(nStr.c_str())) == mFilterInstrument->end())
					return true;
			}
				
				
			return false;
		}
	//抽象接口	
	public:	
		//订阅事件
		virtual void SubscribeEvt(CQueryConditions^ nCondition,IntPtr hWnd) abstract;
		//退订事件
		virtual void UnsubscribeEvt(unsigned int hWnd) abstract;

		//执行
		virtual bool ProcessData(char*  npRespond) abstract;

		//数据接收接口
		virtual bool OnReponseDataReciver() abstract;

		//生成表头
		virtual bool GenerateGridViewHeader() abstract;

		//绑定GridView
		virtual bool GenerateDataTable(CQueryConditions^ nCondition) abstract;

		//填充chart
		virtual bool FillChart(System::Windows::Forms::FlowLayoutPanel^ nrChartContainer) abstract;
	

	};

}