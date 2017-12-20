
#pragma once
#include "AbstractReportDataProvider.h"
#include "CFieldDetail.h"
using namespace System;
using namespace System::Data;
using namespace System::Windows::Forms::DataVisualization::Charting;
#include "CFieldDetail.h"
using namespace CltPresentation;

namespace ReportDataProvider {


	public ref class CAccountTradesReportDataProvider :public IReportDataProvider
	{	
	private:
		array<String^>^ mrCloumeHeaderString;
		array<System::Type^>^ mrColumnType;
	
		cliext::map<String^,TimeStaticBalanceType^> ^ mStaticBalance; //按账号统计的交易结果

		array<String^>^ mrStatisticCloumeHeaderString;
		array<System::Type^>^ mrStatisticColumnType;

		SAdminQuery * mHisQuery;
		Int32 mQueryBufferSize;
		Int32 mnUserId;
	public:
		CAccountTradesReportDataProvider()
		{
			mHisQuery = NULL;
			mQueryBufferSize = 0;
			mnUserId = 0;
			mStaticBalance = gcnew cliext::map<String^,TimeStaticBalanceType^>();
			mrCloumeHeaderString = gcnew array<String^> {"日期",
				"经纪公司代码",
				"投资者代码",
				"合约代码",
				"报单引用",
				"用户代码",
				"交易所代码",
				"成交编号",
				"买卖方向",
				"报单编号",
				"会员代码",
				"客户代码",
				"交易角色",
				"合约在交易所的代码",
				"开平标志",
				"投机套保标志",
				"价格",
				"数量",
				"成交时期",
				"成交时间",
				"成交类型",
				"成交价来源",
				"交易所交易员代码",
				"本地报单编号",
				"结算会员编号",
				"业务单元",
				"序号",
				"交易日",
				"结算编号",
				"经纪公司报单编号",
				"成交来源",
				"平仓盈亏"};
			mrColumnType = gcnew array<Type^> {Type::GetType("System.String"),
				Type::GetType("System.String"),
				Type::GetType("System.String"),
				Type::GetType("System.String"),
				Type::GetType("System.String"),
				Type::GetType("System.String"),
				Type::GetType("System.String"),
				Type::GetType("System.String"),
				Type::GetType("System.String"),
				Type::GetType("System.String"),
				Type::GetType("System.String"),
				Type::GetType("System.String"),
				Type::GetType("System.String"),
				Type::GetType("System.String"),
				Type::GetType("System.String"),
				Type::GetType("System.String"),
				Type::GetType("System.Double"),
				Type::GetType("System.Int32"),
				Type::GetType("System.String"),
				Type::GetType("System.String"),
				Type::GetType("System.String"),
				Type::GetType("System.String"),
				Type::GetType("System.String"),
				Type::GetType("System.String"),
				Type::GetType("System.String"),
				Type::GetType("System.String"),
				Type::GetType("System.String"),
				Type::GetType("System.String"),
				Type::GetType("System.String"),
				Type::GetType("System.String"),
				Type::GetType("System.String"),
		    	Type::GetType("System.Double")};
			mrStatisticCloumeHeaderString = gcnew array<String^> {"日期",
				"统计名称",
				"多头盈亏",
			    "空头盈亏",
			    "总盈亏"};
			mrStatisticColumnType = gcnew array<Type^> {Type::GetType("System.String"),
				Type::GetType("System.String"),
				Type::GetType("System.Double"),
				Type::GetType("System.Double"),
				Type::GetType("System.Double")};
		

		}

		bool StyleChart(System::Windows::Forms::DataVisualization::Charting::Chart ^ nChart)
		{
			//初始化，第一次调用			
			{
				//图表的背景色
				nChart->BackColor = Color::FromArgb(211, 223, 240);
				//图表背景色的渐变方式
				nChart->BackGradientStyle = System::Windows::Forms::DataVisualization::Charting::GradientStyle::TopBottom;
				//图表的边框颜色、
				nChart->BorderlineColor = Color::FromArgb(26, 59, 105);
				//图表的边框线条样式
				nChart->BorderlineDashStyle = System::Windows::Forms::DataVisualization::Charting::ChartDashStyle::Solid;
				//图表边框线条的宽度
				nChart->BorderlineWidth = 2;
				nChart->BorderSkin->SkinStyle =  System::Windows::Forms::DataVisualization::Charting::BorderSkinStyle::Emboss;


				System::Windows::Forms::DataVisualization::Charting::Series ^ mrSeriesLine;
				System::Windows::Forms::DataVisualization::Charting::Series ^ mrSeriesDuoGraphList;
				System::Windows::Forms::DataVisualization::Charting::Series ^ mrSeriesKongGraphList;
				System::Windows::Forms::DataVisualization::Charting::ChartArea^  mrChartArea ;
				System::Windows::Forms::DataVisualization::Charting::Legend^  mrLegendLine ;
				System::Windows::Forms::DataVisualization::Charting::Legend^  mrLegendDuoGraphList;
				System::Windows::Forms::DataVisualization::Charting::Legend^  mrLegendKongGraphList;
				System::Windows::Forms::DataVisualization::Charting::Title^  mrTitle;

				mrSeriesLine = gcnew System::Windows::Forms::DataVisualization::Charting::Series();				
				mrSeriesDuoGraphList = gcnew System::Windows::Forms::DataVisualization::Charting::Series();
				mrSeriesKongGraphList = gcnew System::Windows::Forms::DataVisualization::Charting::Series();
				mrChartArea = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
				mrLegendLine = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
				mrLegendDuoGraphList = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
				mrLegendKongGraphList = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
				mrSeriesLine->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Column;
				mrSeriesDuoGraphList->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Column;
				mrSeriesKongGraphList->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Column;
				mrTitle = gcnew System::Windows::Forms::DataVisualization::Charting::Title();

				//Legend
				mrLegendLine->Name = L"LegendLine";
				mrLegendDuoGraphList->Name = L"LegendDuoGraphList";
				mrLegendKongGraphList->Name = L"LegendKongGraphList";
				mrLegendLine->Docking = System::Windows::Forms::DataVisualization::Charting::Docking::Bottom;
				mrLegendDuoGraphList->Docking = System::Windows::Forms::DataVisualization::Charting::Docking::Bottom;
				mrLegendKongGraphList->Docking = System::Windows::Forms::DataVisualization::Charting::Docking::Bottom;
				mrLegendDuoGraphList->Alignment = System::Drawing::StringAlignment::Center;
				mrLegendKongGraphList->Alignment = System::Drawing::StringAlignment::Center;
				mrLegendLine->Alignment = System::Drawing::StringAlignment::Center;


				//nrChart->BackColor = System::Drawing::Color::Transparent;
				mrChartArea->Area3DStyle->Enable3D = false;
				mrChartArea->BackColor = System::Drawing::Color::Transparent;
				mrChartArea->BackSecondaryColor = System::Drawing::Color::Transparent;
				mrChartArea->BorderColor = System::Drawing::Color::White;
				mrChartArea->Name = L"ChartArea1";
				mrChartArea->ShadowOffset = 5;			
				mrChartArea->AxisX->Interval = 1;
				
				mrChartArea->Area3DStyle->IsClustered = true;


				//Series			
				mrSeriesDuoGraphList->ChartArea = L"ChartArea1";
				mrSeriesDuoGraphList->CustomProperties = L"DrawingStyle=Cylinder, LabelStyle=Top";
				mrSeriesDuoGraphList->Legend = L"LegendDuoGraphList";
				mrSeriesDuoGraphList->Name = L"多头盈亏";
				mrSeriesDuoGraphList->XValueType = System::Windows::Forms::DataVisualization::Charting::ChartValueType::DateTime;

				mrSeriesKongGraphList->ChartArea = L"ChartArea1";
				mrSeriesKongGraphList->CustomProperties = L"DrawingStyle=Cylinder, LabelStyle=Top";
				mrSeriesKongGraphList->Legend = L"LegendKongGraphList";
				mrSeriesKongGraphList->Name = L"空头盈亏";
				mrSeriesKongGraphList->XValueType = System::Windows::Forms::DataVisualization::Charting::ChartValueType::DateTime;



				mrSeriesLine->ChartArea = L"ChartArea1";				
				mrSeriesLine->Color = System::Drawing::Color::FromArgb(220,120,150,20);
				mrSeriesLine->CustomProperties = L"DrawingStyle=Cylinder, LabelStyle=Top";
				mrSeriesLine->Legend = L"LegendLine";
				mrSeriesLine->Name = L"逐日多头和空头合计盈亏";
				mrSeriesLine->XValueType = System::Windows::Forms::DataVisualization::Charting::ChartValueType::DateTime;

				////线条暗影色彩
				//mrSeriesLine->ShadowColor = Color::Black;
				////暗影宽度
				//mrSeriesLine->ShadowOffset = 2;
				////是否显示数据申明
				//mrSeriesLine->IsVisibleInLegend = true;
				////线条上数据点上是否稀有据显示
				//mrSeriesLine->IsValueShownAsLabel = false;
				////线条上的数据点标记类型
				//mrSeriesLine->MarkerStyle = System::Windows::Forms::DataVisualization::Charting::MarkerStyle::Circle;
				////线条数据点的大小
				//mrSeriesLine->MarkerSize = 10;

				nChart->Legends->Clear();						
				nChart->Legends->Add(mrLegendLine);
				nChart->Legends->Add(mrLegendDuoGraphList);
				nChart->Legends->Add(mrLegendKongGraphList);
				//chart area
				nChart->ChartAreas->Clear();					
				nChart->ChartAreas->Add(mrChartArea);				

				//Series
				nChart->Series->Clear();
				nChart->Series->Add(mrSeriesLine);
				nChart->Series->Add(mrSeriesDuoGraphList);
				nChart->Series->Add(mrSeriesKongGraphList);
				nChart->Titles->Add(mrTitle);	
			

				mrSeriesLine->Points->Clear();
				mrSeriesDuoGraphList->Points->Clear();
				mrSeriesDuoGraphList->Points->Clear();	
	
				//nrChart->DataSource = RealDataTable;
			}
			return true;
		}
	
	public:
		//订阅事件
		virtual void SubscribeEvt(CQueryConditions^ nCondition,IntPtr hWnd,Int32 nUserID) override
		{
			//RealDataTable->Rows->Clear();
			//StatisticDataTable->Rows->Clear();
			//mStaticBalance->clear();
			////CTcpLayer::SubscribePkg(Cmd_RM_QryHistoryFundInfo_Rsp,(int)hWnd);		
			//CTcpLayer::SubscribePkg(Cmd_RM_QryHistoryTrade_Rsp,(int)hWnd);
		
			////CTcpLayer::SendData()
			//int nBufSize = sizeof(SAdminQuery) + (nCondition->marrayAccount->Length-1)*sizeof(CommonAccountType) ;
			//SAdminQuery *lHisQuery = (SAdminQuery*)malloc(nBufSize) ;
			//memset(lHisQuery,0,nBufSize);
			//lHisQuery->nCount = nCondition->marrayAccount->Length;
	
			//for(int i = 0; i < lHisQuery->nCount; i++)
			//{
			//	std::string lstrTempUser = Tools::String2string(nCondition->marrayAccount[i]);
			//	strcpy(lHisQuery->strAccount[i],lstrTempUser.c_str());
			//}
		
			//ClearQuerySeq();
			//System::TimeSpan^ diff1 = nCondition->mendTime.Subtract(nCondition->mstartTime);
			//Int32 nQueryCount = diff1->Days + 1;
			//SetTotalQueryCount(nQueryCount);

			//DateTime ^ lTempTime = nCondition->mstartTime;
			//int lnSeq = 0;
			//while(lTempTime <= nCondition->mendTime)
			//{
			//	std::string lstrStartTime = Tools::String2string(lTempTime->ToString("yyyy-MM-dd"));
			//	std::string lstrEndTime = Tools::String2string(lTempTime->ToString("yyyy-MM-dd"));
			//	strcpy(lHisQuery->szDateBegin,lstrStartTime.c_str());
			//	strcpy(lHisQuery->szDateEnd,lstrEndTime.c_str());
			//	AddQuerySeq(lTempTime,lnSeq);
			//	CTcpLayer::SendData(Cmd_RM_QryHistoryTrade_Req, (void*)lHisQuery, nBufSize,lnSeq++,nUserID);
			//	QueryMode = nCondition->mnQueryMode;

			//	lTempTime = lTempTime->AddDays(1.0);
			//}
			//free(lHisQuery);

			CTcpLayer::SubscribePkg(Cmd_RM_QryHistoryTrade_Rsp,(int)hWnd);
			RealDataTable->Rows->Clear();				
			StatisticDataTable->Rows->Clear();
			mStaticBalance->clear();

			SetUserCancel(false);
			if(mHisQuery != NULL)
			{
				free(mHisQuery);
				mHisQuery = NULL;
			}

			mnUserId = nUserID;
			mQueryBufferSize = sizeof(SAdminQuery) + (nCondition->marrayAccount->Length-1)*sizeof(CommonAccountType) ;
			mHisQuery = (SAdminQuery*)malloc(mQueryBufferSize) ;
			memset(mHisQuery,0,mQueryBufferSize);
			mHisQuery->nCount = nCondition->marrayAccount->Length;		
			for(int i = 0; i < mHisQuery->nCount; i++)
			{
				std::string lstrTempUser = Tools::String2string(nCondition->marrayAccount[i]);
				strcpy(mHisQuery->strAccount[i],lstrTempUser.c_str());
			}
			ClearQuerySeq();
			System::TimeSpan^ diff1 = nCondition->mendTime.Subtract(nCondition->mstartTime);
			Int32 nQueryCount = diff1->Days + 1;
			SetTotalQueryCount(nQueryCount);
			QueryMode = nCondition->mnQueryMode;

			DateTime ^ lTempTime = nCondition->mstartTime;
			int lnSeq = 0;
			while(lTempTime <= nCondition->mendTime)
			{				
				AddQuerySeq(lTempTime,lnSeq++);
				lTempTime = lTempTime->AddDays(1.0);
			}

			int lnCurrentSeq = 0;
			String ^ lCurrentQry = GetCurrentQuery(lnCurrentSeq);
			if(!String::IsNullOrEmpty(lCurrentQry))
			{
				std::string lstrStartTime = Tools::String2string(lCurrentQry);
				std::string lstrEndTime = Tools::String2string(lCurrentQry);
				strcpy(mHisQuery->szDateBegin,lstrStartTime.c_str());
				strcpy(mHisQuery->szDateEnd,lstrEndTime.c_str());
				CTcpLayer::SendData(Cmd_RM_QryHistoryTrade_Req, (void*)mHisQuery, mQueryBufferSize,lnCurrentSeq,mnUserId);
			}
		}
		//退订事件
		virtual void UnsubscribeEvt(unsigned int hWnd) override
		{
			CTcpLayer::UnsubscribeAllPkg((int)hWnd);
		}

		//执行
		virtual bool ProcessData(char*  npRespond) override
		{
			if(npRespond == NULL)
				return false;
			Stru_UniPkgHead head;	
			memset(&head, 0, sizeof(head));
			memcpy(&head, npRespond, sizeof(head));
		
			if(head.cmdid == Cmd_RM_QryHistoryTrade_Rsp)
			{
				FinishedSingleQuey(head.seq);
				int lnCurrentSeq = 0;
				String ^ lCurrentQry = GetCurrentQuery(lnCurrentSeq);
				if(!String::IsNullOrEmpty(lCurrentQry))
				{
					std::string lstrStartTime = Tools::String2string(lCurrentQry);
					std::string lstrEndTime = Tools::String2string(lCurrentQry);
					strcpy(mHisQuery->szDateBegin,lstrStartTime.c_str());
					strcpy(mHisQuery->szDateEnd,lstrEndTime.c_str());
					CTcpLayer::SendData(Cmd_RM_QryHistoryTrade_Req, (void*)mHisQuery, mQueryBufferSize,lnCurrentSeq,mnUserId);
				}
			}
			if(head.cmdid == Cmd_RM_QryHistoryTrade_Rsp && head.userdata1 == CF_ERROR_SUCCESS)
			{
				int count= head.len/sizeof(PlatformStru_TradeInfo);
				if(count != 0)
				{
				/*	RealDataTable->Rows->Clear();
					StatisticDataTable->Rows->Clear();
					mStaticBalance->clear();*/
					PlatformStru_TradeInfo* pStatic = NULL;
					for(Int32 i = 0; i < count;i++)
					{
						pStatic = (PlatformStru_TradeInfo*)
							(npRespond+sizeof(Stru_UniPkgHead)+i*sizeof(PlatformStru_TradeInfo));

						if(FilterAccount(std::string(pStatic->InvestorID)))
							continue;
						if(FilterInstrument(std::string(pStatic->InstrumentID)))
							continue;

						//统计
						//先找到帐号
						//先找到帐号
						array<String^>^ lStaticKey = GetStaticKey( Tools::string2String(pStatic->InvestorID), Tools::string2String(pStatic->InstrumentID));
						if(lStaticKey != nullptr)
						{
							for(int i = 0 ; i < lStaticKey->Length;i++)
							{
								String ^ lstrName = lStaticKey[i];			
								if(FilterOther(Tools::String2string(lstrName)))
									continue;

								cliext::map<String^,TimeStaticBalanceType^>::iterator lIterStatic = mStaticBalance->find(lstrName);
								if(lIterStatic != mStaticBalance->end())
								{
									//找日期
									TimeStaticBalanceType ^ ltempTimeHode = lIterStatic->second;
									String ^ lstrTime = Tools::string2String(pStatic->TradingDay);						
									TimeStaticBalanceType::iterator lIterTime = ltempTimeHode->find(lstrTime);

									if(lIterTime == ltempTimeHode->end())
									{
										//没有找到对应日期的记录，插入一行
										cliext::vector<Double> ^ lTempVector = gcnew cliext::vector<Double>();
										lTempVector->push_back(0.0f);
										lTempVector->push_back(0.0f);
										if(pStatic->Direction == THOST_FTDC_D_Buy)
										{
											if(pStatic->CloseProfitByDate != util::GetDoubleInvalidValue())
												lTempVector[0] += pStatic->CloseProfitByDate;
										}
										else
										{
											if(pStatic->CloseProfitByDate != util::GetDoubleInvalidValue())
												lTempVector[1] += pStatic->CloseProfitByDate;
										}
										//lTempVector->push_back(pStatic->Balance-pStatic->PreBalance);							
										ltempTimeHode->insert(cliext::make_pair(lstrTime,lTempVector));

										DataRow ^ nrStatisticDataRow = StatisticDataTable->NewRow();	
										int j = 0;
										nrStatisticDataRow[mrStatisticCloumeHeaderString[j++]] =  Tools::string2String(pStatic->TradingDay);						
										nrStatisticDataRow[mrStatisticCloumeHeaderString[j++]] =  lstrName;
										nrStatisticDataRow[mrStatisticCloumeHeaderString[j++]] =  lTempVector[0];
										nrStatisticDataRow[mrStatisticCloumeHeaderString[j++]] =  lTempVector[1];
										nrStatisticDataRow[mrStatisticCloumeHeaderString[j++]] =  lTempVector[0]+lTempVector[1];
										StatisticDataTable->Rows->Add(nrStatisticDataRow);
									}
									else
									{
										//找到对应日期的记录，修改
										cliext::vector<Double> ^ lTempVector = lIterTime->second;
										if(pStatic->Direction == THOST_FTDC_D_Buy)
										{
											if(pStatic->CloseProfitByDate != util::GetDoubleInvalidValue())
												lTempVector[0] += pStatic->CloseProfitByDate;
										}
										else
										{
											if(pStatic->CloseProfitByDate != util::GetDoubleInvalidValue())
												lTempVector[1] += pStatic->CloseProfitByDate;
										}
										////lTempVector[0] += (pStatic->Balance - pStatic->PreBalance);

										cli::array<String^>^ key = {
											Tools::string2String(pStatic->TradingDay),
											lstrName											
										};
										DataRow ^ nrStatisticDataRow = StatisticDataTable->Rows->Find(key);
										int j = 0;
										nrStatisticDataRow[mrStatisticCloumeHeaderString[j++]] =  Tools::string2String(pStatic->TradingDay);						
										nrStatisticDataRow[mrStatisticCloumeHeaderString[j++]] =  lstrName;
										nrStatisticDataRow[mrStatisticCloumeHeaderString[j++]] =  lTempVector[0];
										nrStatisticDataRow[mrStatisticCloumeHeaderString[j++]] =  lTempVector[1];
										nrStatisticDataRow[mrStatisticCloumeHeaderString[j++]] =  lTempVector[0]+lTempVector[1];
									}
								}
								else
								{
									//没有找到对应的帐号，插入一条
									TimeStaticBalanceType ^ ltempTimeHode = gcnew TimeStaticBalanceType();
									String ^ lstrTime = Tools::string2String(pStatic->TradingDay);							
									cliext::vector<Double> ^ lTempVector = gcnew cliext::vector<Double>();
									//lTempVector->push_back(pStatic->Balance - pStatic->PreBalance);	
									lTempVector->push_back(0.0f);
									lTempVector->push_back(0.0f);
									if(pStatic->Direction == THOST_FTDC_D_Buy)
									{
										if(pStatic->CloseProfitByDate != util::GetDoubleInvalidValue())
											lTempVector[0] += pStatic->CloseProfitByDate;
									}
									else
									{
										if(pStatic->CloseProfitByDate != util::GetDoubleInvalidValue())
											lTempVector[1] += pStatic->CloseProfitByDate;
									}
									ltempTimeHode->insert(cliext::make_pair(lstrTime,lTempVector));

									mStaticBalance->insert(cliext::make_pair(lstrName,ltempTimeHode));

									DataRow ^ nrStatisticDataRow = StatisticDataTable->NewRow();	
									int j = 0;
									nrStatisticDataRow[mrStatisticCloumeHeaderString[j++]] =  Tools::string2String(pStatic->TradingDay);						
									nrStatisticDataRow[mrStatisticCloumeHeaderString[j++]] =  lstrName;
									nrStatisticDataRow[mrStatisticCloumeHeaderString[j++]] =  lTempVector[0];
									nrStatisticDataRow[mrStatisticCloumeHeaderString[j++]] =  lTempVector[1];
									nrStatisticDataRow[mrStatisticCloumeHeaderString[j++]] =  lTempVector[0]+lTempVector[1];
									StatisticDataTable->Rows->Add(nrStatisticDataRow);

								}					

							}
						}



						DataRow ^ nrDataRow = RealDataTable->NewRow();	
						int j = 0;
						nrDataRow[mrCloumeHeaderString[j++]] =  Tools::string2String(pStatic->TradingDay);							
						nrDataRow[mrCloumeHeaderString[j++]] =  Tools::string2String(pStatic->BrokerID);
						nrDataRow[mrCloumeHeaderString[j++]] =  Tools::string2String(pStatic->InvestorID);
						nrDataRow[mrCloumeHeaderString[j++]] =  Tools::string2String(pStatic->InstrumentID);						
						nrDataRow[mrCloumeHeaderString[j++]] =  Tools::string2String(pStatic->OrderRef);

						nrDataRow[mrCloumeHeaderString[j++]] =  Tools::string2String(pStatic->UserID);
						nrDataRow[mrCloumeHeaderString[j++]] =  Tools::string2String(pStatic->ExchangeID);
						nrDataRow[mrCloumeHeaderString[j++]] =  Tools::string2String(pStatic->TradeID);
						
						nrDataRow[mrCloumeHeaderString[j++]] =  Tools::string2String(CFieldDetail::Direction2String(pStatic->Direction));
						nrDataRow[mrCloumeHeaderString[j++]] = Tools::string2String(pStatic->OrderSysID);
						nrDataRow[mrCloumeHeaderString[j++]] = Tools::string2String(pStatic->ParticipantID);
						nrDataRow[mrCloumeHeaderString[j++]] = Tools::string2String(pStatic->ClientID);
						nrDataRow[mrCloumeHeaderString[j++]] = Tools::string2String(CFieldDetail::TraderRole2String(pStatic->TradingRole));
						nrDataRow[mrCloumeHeaderString[j++]] = Tools::string2String(pStatic->ExchangeInstID);
						nrDataRow[mrCloumeHeaderString[j++]] = Tools::string2String(CFieldDetail::OffsetFlag2String(pStatic->OffsetFlag));				
						nrDataRow[mrCloumeHeaderString[j++]] = Tools::string2String(CFieldDetail::HedgeFlag2String(pStatic->HedgeFlag));
						nrDataRow[mrCloumeHeaderString[j++]] = pStatic->Price;
						nrDataRow[mrCloumeHeaderString[j++]] = pStatic->Volume;
						nrDataRow[mrCloumeHeaderString[j++]] = Tools::string2String(pStatic->TradeDate);
						nrDataRow[mrCloumeHeaderString[j++]] = Tools::string2String(pStatic->TradeTime);
						nrDataRow[mrCloumeHeaderString[j++]] = Tools::string2String(CFieldDetail::TradeType2String(pStatic->TradeType));
						nrDataRow[mrCloumeHeaderString[j++]] = Tools::string2String(CFieldDetail::PriceType2String(pStatic->PriceSource));
						nrDataRow[mrCloumeHeaderString[j++]] = Tools::string2String(pStatic->TraderID);
						nrDataRow[mrCloumeHeaderString[j++]] = Tools::string2String(pStatic->OrderLocalID);
						nrDataRow[mrCloumeHeaderString[j++]] = Tools::string2String(pStatic->ClearingPartID);
						nrDataRow[mrCloumeHeaderString[j++]] = Tools::string2String(pStatic->BusinessUnit);
						nrDataRow[mrCloumeHeaderString[j++]] = pStatic->SequenceNo;
						nrDataRow[mrCloumeHeaderString[j++]] =  Tools::string2String(pStatic->TradingDay);
						nrDataRow[mrCloumeHeaderString[j++]] = pStatic->SettlementID;
						nrDataRow[mrCloumeHeaderString[j++]] = pStatic->BrokerOrderSeq;
						nrDataRow[mrCloumeHeaderString[j++]] = Tools::string2String(CFieldDetail::TradeSourceString(pStatic->TradeSource));
						nrDataRow[mrCloumeHeaderString[j++]] = pStatic->CloseProfitByDate;

						RealDataTable->Rows->Add(nrDataRow);
					}


				}
			}
			return true;
		}

		//数据接收接口
		virtual bool OnReponseDataReciver() override
		{
			return true;
		}

		//生成表头
		virtual bool GenerateGridViewHeader() override
		{
			RealDataTable->Columns->Clear();
			for(Int32 i = 0; i < mrCloumeHeaderString->Length;i++)
			{
				RealDataTable->Columns->Add(mrCloumeHeaderString[i],mrColumnType[i]);
			}

			//StatisticDataTable->Rows->Clear();
			StatisticDataTable->Columns->Clear();
			for(Int32 i = 0; i < mrStatisticCloumeHeaderString->Length;i++)
			{
				StatisticDataTable->Columns->Add(mrStatisticCloumeHeaderString[i],mrStatisticColumnType[i]);
			}
			array<DataColumn^>^ dcPosition = gcnew array<DataColumn^>(2);
			dcPosition[0] = StatisticDataTable->Columns[0];
			dcPosition[1] = StatisticDataTable->Columns[1];				
			StatisticDataTable->PrimaryKey = dcPosition;
			return true;
		}
		//绑定GridView
		virtual bool GenerateDataTable(CQueryConditions^ nCondition) override
		{
			//RealDataTable->Rows->Clear();
			
			return true;
		
		}

		//填充chart
		virtual bool FillChart(System::Windows::Forms::FlowLayoutPanel^ nrChartContainer) override
		{
			if(nrChartContainer)
			{
				nrChartContainer->Controls->Clear();
				nrChartContainer->Update();
				cliext::map<String^,TimeStaticBalanceType^>::iterator lIterStatic = mStaticBalance->begin();
				for(;lIterStatic != mStaticBalance->end(); lIterStatic++)
				{


					System::Windows::Forms::DataVisualization::Charting::Chart ^ lChart = 
						gcnew System::Windows::Forms::DataVisualization::Charting::Chart();

					lChart->Size = System::Drawing::Size(1400,500);
					lChart->AutoSize = true;
					
					StyleChart(lChart);

					lChart->Titles[0]->Text = lIterStatic->first;

					//添加数据到serial
					TimeStaticBalanceType ^ ltempTimeHode = lIterStatic->second;				
					TimeStaticBalanceType::iterator lIterTime ;
					Double ldbTemp = 0.0;
					Double lMax1 = 0.0f;
					Double lMin = 0.0f;
					
					for (lIterTime = ltempTimeHode->begin(); lIterTime != ltempTimeHode->end();lIterTime++)
					{
						cliext::vector<Double> ^ lTempVector = lIterTime->second;
						//ldbTemp += lTempVector[0];	
						//ldbTemp += lTempVector[1];	
						if(lTempVector[0] > lMax1)
							lMax1 = lTempVector[0];
						if(lTempVector[1] > lMax1)
							lMax1 = lTempVector[1];
						if(lTempVector[0] + lTempVector[1] > lMax1)
							lMax1 = lTempVector[0] + lTempVector[1];


						if(lTempVector[0] < lMin )
							lMin = lTempVector[0];
						if(lTempVector[1] < lMin)
							lMin = lTempVector[1];
						if(lTempVector[0]+lTempVector[1] < lMin)
							lMin = lTempVector[0] + lTempVector[1];
						//lChart->ChartAreas["ChartArea1"]->AxisY->Maximum =lMax1+ 1;
						//lChart->ChartAreas["ChartArea1"]->AxisY->Minimum =lMin - 1;
						lChart->Series["逐日多头和空头合计盈亏"]->Points->AddXY(lIterTime->first,lTempVector[0]+lTempVector[1]);
						lChart->Series["多头盈亏"]->Points->AddXY(lIterTime->first,lTempVector[0]);
						lChart->Series["空头盈亏"]->Points->AddXY(lIterTime->first,lTempVector[1]);
						
						//lChart->Series["每日权益变化量"]->Points->AddXY(lIterTime->first,lTempVector[0] - lTempVector[1]);
					}

					//添加tooltip事件
					lChart->GetToolTipText += gcnew EventHandler<ToolTipEventArgs^>(this,&CAccountTradesReportDataProvider::ChartLineGetToolTipText);
					nrChartContainer->Controls->Add(lChart);
				}

			}
			return true;

		
		}  

		private: void ChartLineGetToolTipText(Object^ sender,ToolTipEventArgs^ e)
    	{

			 if(e->HitTestResult->ChartElementType == ChartElementType::DataPoint)
			 {

				 int i = e->HitTestResult->PointIndex;
				 DataPoint^ dp = e->HitTestResult->Series->Points[i];

				 e->Text = String::Format("日期:{0};数值:{1:F1}",dp->AxisLabel,dp->YValues[0]);
			 }

		}
	};
}