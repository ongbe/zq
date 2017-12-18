
#pragma once
#include "AbstractReportDataProvider.h"
#include "AccountHodeReportDP.h"
#include "AccountInterestReportDP.h"
#include "AccountTradesReportDP.h"
#include "Singleton.h"
namespace ReportDataProvider
{
	public interface class IReportDataProviderFactory
	{
	public:
		virtual IReportDataProvider^ CreateDataProvider() = 0;
		
	};



	//用户持仓统计工厂
	public ref class CAccountHodeReportDataProviderFactory :public IReportDataProviderFactory,
		Singleton<CAccountHodeReportDataProviderFactory>
	{

	public:
		virtual IReportDataProvider^ CreateDataProvider() 
		{
			return gcnew CAccountHodeReportDataProvider();
		}
	};

	//用户权益统计工厂
	public ref class CAccountInterestsDataProviderFactory :public IReportDataProviderFactory,
		Singleton<CAccountInterestsDataProviderFactory>
	{
	public:
		virtual IReportDataProvider^ CreateDataProvider() 
		{
			return gcnew CAccountInterestReportDataProvider();
		}
	};

	//用户交易统计工厂
	public ref class CAccountTradesReportDataProviderFactory :public IReportDataProviderFactory,
		Singleton<CAccountTradesReportDataProviderFactory>
	{
	public:
		virtual IReportDataProvider^ CreateDataProvider() 
		{
			return gcnew CAccountTradesReportDataProvider();
		}
	};



}