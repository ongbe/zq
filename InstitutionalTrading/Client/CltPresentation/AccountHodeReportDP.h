
#pragma once
#include "AbstractReportDataProvider.h"
using namespace System;
using namespace System::Data;
using namespace System::Windows::Forms::DataVisualization::Charting;
#include "RiskManageStruct.h"
#include "CFieldDetail.h"
using namespace CltPresentation;

namespace ReportDataProvider {


	
	public ref class CAccountHodeReportDataProvider :public IReportDataProvider
	{
	public:
		
	private:
		array<String^>^ mrCloumeHeaderString;
		array<System::Type^>^ mrColumnType;
		cliext::map<String^,TimeStaticHodeType^> ^ mStaticHode; //按账号统计的持仓结果

		array<String^>^ mrStatisticCloumeHeaderString;
		array<System::Type^>^ mrStatisticColumnType;
		SAdminQuery * mHisQuery;
		Int32 mQueryBufferSize;
		Int32 mnUserId;
	
	
	public:
		CAccountHodeReportDataProvider()
		{
	
			mHisQuery = NULL;
			mQueryBufferSize = 0;
			mnUserId = 0;
			mStaticHode = gcnew cliext::map<String^,TimeStaticHodeType^>();
			mrCloumeHeaderString = gcnew array<String^> {"日期",
				"合约代码",
				"经纪公司代码",
				"投资者代码",
				"持仓多空方向",
				"投机套保标志",
				"持仓日期",
				"上日持仓",
				"今日持仓",
				"多头冻结",
				"空头冻结",
				"开仓冻结金额",
				"平仓冻结金额",
				"开仓量",
				"平仓量",
				"开仓金额",
				"平仓金额",
				"持仓成本",
				"上次占用的保证金",
				"占用的保证金",
				"冻结的保证金",
				"冻结的资金",
				"冻结的手续费",
				"资金差额",
				"手续费",
				"平仓盈亏",
				"持仓盈亏",
				"上次结算价",
				"本次结算价",
				"交易日",
				"结算编号",
				"开仓成本",
				"交易所保证金",
				"组合成交形成的持仓",
				"组合多头冻结",
				"组合空头冻结",
				"逐日盯市平仓盈亏",
				"逐笔对冲平仓盈亏",
				"今仓",
				"保证金率",
				"保证金率(按手数)"};
			mrColumnType = gcnew array<Type^> {Type::GetType("System.String"),
				Type::GetType("System.String"),
				Type::GetType("System.String"),
				Type::GetType("System.String"),
				Type::GetType("System.String"),
				Type::GetType("System.String"),
				Type::GetType("System.String"),
				Type::GetType("System.Int32"),
				Type::GetType("System.Int32"),
				Type::GetType("System.Int32"),
				Type::GetType("System.Int32"),
				Type::GetType("System.Double"),
				Type::GetType("System.Double"),
				Type::GetType("System.Int32"),
				Type::GetType("System.Int32"),
				Type::GetType("System.Double"),
				Type::GetType("System.Double"),
				Type::GetType("System.Double"),
				Type::GetType("System.Double"),
				Type::GetType("System.Double"),
				Type::GetType("System.Double"),
				Type::GetType("System.Double"),
				Type::GetType("System.Double"),
				Type::GetType("System.Double"),
				Type::GetType("System.Double"),
				Type::GetType("System.Double"),
				Type::GetType("System.Double"),
				Type::GetType("System.Double"),
				Type::GetType("System.Double"),
				Type::GetType("System.String"),
				Type::GetType("System.Double"),
				Type::GetType("System.Double"),
				Type::GetType("System.Double"),
				Type::GetType("System.Int32"),
				Type::GetType("System.Int32"),
				Type::GetType("System.Int32"),
				Type::GetType("System.Double"),
				Type::GetType("System.Double"),
				Type::GetType("System.Int32"),
				Type::GetType("System.Double"),
				Type::GetType("System.Double")};

			mrStatisticCloumeHeaderString = gcnew array<String^> {"日期",
				"统计名称",
				"总仓位"};
			mrStatisticColumnType = gcnew array<Type^> {Type::GetType("System.String"),
				Type::GetType("System.String"),
				Type::GetType("System.Int32")};

		
		}
		bool StyleChart(System::Windows::Forms::DataVisualization::Charting::Chart ^ nChart)
		{
			//初始化，第一次调用
			//if(mrSeriesLine == nullptr && mrSeriesGraphList == nullptr)
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
				System::Windows::Forms::DataVisualization::Charting::Series ^ mrSeriesGraphList;
				System::Windows::Forms::DataVisualization::Charting::ChartArea^  mrChartArea ;
				System::Windows::Forms::DataVisualization::Charting::Legend^  mrLegendLine ;
				System::Windows::Forms::DataVisualization::Charting::Legend^  mrLegendGraphList;
				System::Windows::Forms::DataVisualization::Charting::Title^  mrTitle;
				mrSeriesLine = gcnew System::Windows::Forms::DataVisualization::Charting::Series();				
				mrSeriesGraphList = gcnew System::Windows::Forms::DataVisualization::Charting::Series();
				mrChartArea = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
				mrLegendLine = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
				mrLegendGraphList = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
				mrSeriesLine->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
				mrSeriesGraphList->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Column;

				mrTitle = gcnew System::Windows::Forms::DataVisualization::Charting::Title();
				//Legend
				mrLegendLine->Name = L"LegendLine";
				mrLegendGraphList->Name = L"LegendGraphList";
				mrLegendLine->Docking = System::Windows::Forms::DataVisualization::Charting::Docking::Bottom;
				mrLegendGraphList->Docking = System::Windows::Forms::DataVisualization::Charting::Docking::Bottom;
				mrLegendGraphList->Alignment = System::Drawing::StringAlignment::Center;
				mrLegendLine->Alignment = System::Drawing::StringAlignment::Center;


				//chart area
			
				//nrChart->BackColor = System::Drawing::Color::Transparent;
				mrChartArea->Area3DStyle->Enable3D = false;
				mrChartArea->BackColor = System::Drawing::Color::Transparent;
				mrChartArea->BackSecondaryColor = System::Drawing::Color::Transparent;
				mrChartArea->BorderColor = System::Drawing::Color::White;
				mrChartArea->Name = L"ChartArea1";
				mrChartArea->ShadowOffset = 5;			
				mrChartArea->AxisX->Interval = 1;//DateTimeIntervalType::Days;

				//Series			
				mrSeriesGraphList->ChartArea = L"ChartArea1";
				mrSeriesGraphList->CustomProperties = L"DrawingStyle=Cylinder, LabelStyle=Top";
				mrSeriesGraphList->Legend = L"LegendGraphList";
				mrSeriesGraphList->Name = L"日内增减量";
				mrSeriesGraphList->XValueType = System::Windows::Forms::DataVisualization::Charting::ChartValueType::DateTime;

				mrSeriesLine->ChartArea = L"ChartArea1";				
				mrSeriesLine->Color = System::Drawing::Color::FromArgb(220,120,150,20);
				mrSeriesLine->Legend = L"LegendLine";
				mrSeriesLine->Name = L"逐日持仓";
				mrSeriesLine->XValueType = System::Windows::Forms::DataVisualization::Charting::ChartValueType::DateTime;

				//线条暗影色彩
				mrSeriesLine->ShadowColor = Color::Black;
				//暗影宽度
				mrSeriesLine->ShadowOffset = 2;
				//是否显示数据申明
				mrSeriesLine->IsVisibleInLegend = true;
				//线条上数据点上是否稀有据显示
				mrSeriesLine->IsValueShownAsLabel = false;
				//线条上的数据点标记类型
				mrSeriesLine->MarkerStyle = System::Windows::Forms::DataVisualization::Charting::MarkerStyle::Circle;
				//线条数据点的大小
				mrSeriesLine->MarkerSize = 10;

				nChart->Legends->Clear();						
				nChart->Legends->Add(mrLegendLine);
				nChart->Legends->Add(mrLegendGraphList);

				//chart area
				nChart->ChartAreas->Clear();			
				nChart->ChartAreas->Add(mrChartArea);			

				//Series
				nChart->Series->Clear();	
				nChart->Series->Add(mrSeriesLine);
				nChart->Series->Add(mrSeriesGraphList);	
				nChart->Titles->Add(mrTitle);

				mrSeriesLine->Points->Clear();
				mrSeriesGraphList->Points->Clear();


			}
			return true;

		}
	public:
		//订阅事件
		virtual void SubscribeEvt(CQueryConditions^ nCondition,IntPtr hWnd,Int32 nUserID) override
		{
			CTcpLayer::SubscribePkg(Cmd_RM_QryHistoryPosition_Rsp,(int)hWnd);
			RealDataTable->Rows->Clear();				
			StatisticDataTable->Rows->Clear();
			mStaticHode->clear();
		
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
				CTcpLayer::SendData(Cmd_RM_QryHistoryPosition_Req, (void*)mHisQuery, mQueryBufferSize,lnCurrentSeq,mnUserId);
			}
			//free(lHisQuery);
		}
		//退订事件
		virtual void UnsubscribeEvt(unsigned int hWnd) override
		{
			//CTcpLayer::UnsubscribeAllPkg((int)hWnd);
		}

		//执行
		virtual bool ProcessData(char*  npRespond) override
		{
			if(npRespond == NULL)
				return false;
			Stru_UniPkgHead head;	
			memset(&head, 0, sizeof(head));
			memcpy(&head, npRespond, sizeof(head));
			if(head.cmdid == Cmd_RM_QryHistoryPosition_Rsp && IsUserCancel() == false)
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
					CTcpLayer::SendData(Cmd_RM_QryHistoryPosition_Req, (void*)mHisQuery, mQueryBufferSize,lnCurrentSeq,mnUserId);
				}
			}
			if(head.cmdid == Cmd_RM_QryHistoryPosition_Rsp && head.userdata1 == CF_ERROR_SUCCESS && IsUserCancel() == false)
			{
				int count= head.len/sizeof(PlatformStru_Position);
				if(count != 0)
				{			

					PlatformStru_Position* pStatic = NULL;
					for(Int32 i = 0; i < count;i++)
					{
						pStatic = (PlatformStru_Position*)
							(npRespond+sizeof(Stru_UniPkgHead)+i*sizeof(PlatformStru_Position));

						//过滤
						if(FilterAccount(std::string(pStatic->InvestorID)))
							continue;
						if(FilterInstrument(std::string(pStatic->InstrumentID)))
							continue;
						//统计
						//先找到帐号
						array<String^>^ lStaticKey = GetStaticKey( Tools::string2String(pStatic->InvestorID), Tools::string2String(pStatic->InstrumentID));
						if(lStaticKey != nullptr)
						{
							for(int i = 0 ; i < lStaticKey->Length;i++)
							{
								String ^ lstrName = lStaticKey[i];	

								if(FilterOther(Tools::String2string(lstrName)))
									continue;

								cliext::map<String^,TimeStaticHodeType^>::iterator lIterStatic = mStaticHode->find(lstrName);
								if(lIterStatic != mStaticHode->end())
								{
									//找日期
									TimeStaticHodeType ^ ltempTimeHode = lIterStatic->second;
									String ^ lstrTime = Tools::string2String(pStatic->TradingDay);						
									TimeStaticHodeType::iterator lIterTime = ltempTimeHode->find(lstrTime);

									if(lIterTime == ltempTimeHode->end())
									{
										//没有找到对应日期的记录，插入一行
										cliext::vector<Int32> ^ lTempVector = gcnew cliext::vector<Int32>();
										lTempVector->push_back(pStatic->Position);
										lTempVector->push_back(pStatic->YdPosition);
										ltempTimeHode->insert(cliext::make_pair(lstrTime,lTempVector));

										DataRow ^ nrStatisticDataRow = StatisticDataTable->NewRow();	
										int j = 0;
										nrStatisticDataRow[mrStatisticCloumeHeaderString[j++]] =  Tools::string2String(pStatic->TradingDay);						
										nrStatisticDataRow[mrStatisticCloumeHeaderString[j++]] =  lstrName;
										nrStatisticDataRow[mrStatisticCloumeHeaderString[j++]] =  lTempVector[0];
										StatisticDataTable->Rows->Add(nrStatisticDataRow);
									}
									else
									{
										//找到对应日期的记录，修改
										cliext::vector<Int32> ^ lTempVector = lIterTime->second;
										lTempVector[0] += pStatic->Position;
										lTempVector[1] += pStatic->YdPosition;

										cli::array<String^>^ key = {
											Tools::string2String(pStatic->TradingDay),
											lstrName											
										};

									/*	DataRow^  dr = nullptr;
										dr = StatisticDataTable->Rows->Find(key);*/
										DataRow ^ nrStatisticDataRow = StatisticDataTable->Rows->Find(key);
										int j = 0;
										nrStatisticDataRow[mrStatisticCloumeHeaderString[j++]] =  Tools::string2String(pStatic->TradingDay);						
										nrStatisticDataRow[mrStatisticCloumeHeaderString[j++]] =  lstrName;
										nrStatisticDataRow[mrStatisticCloumeHeaderString[j++]] =  lTempVector[0];
										//StatisticDataTable->Rows->Add(nrStatisticDataRow);
									}
								}
								else
								{
									//没有找到对应的帐号，插入一条
									TimeStaticHodeType ^ ltempTimeHode = gcnew TimeStaticHodeType();
									String ^ lstrTime = Tools::string2String(pStatic->TradingDay);							
									cliext::vector<Int32> ^ lTempVector = gcnew cliext::vector<Int32>();
									lTempVector->push_back(pStatic->Position);
									lTempVector->push_back(pStatic->YdPosition);
									ltempTimeHode->insert(cliext::make_pair(lstrTime,lTempVector));

									mStaticHode->insert(cliext::make_pair(lstrName,ltempTimeHode));

									DataRow ^ nrStatisticDataRow = StatisticDataTable->NewRow();	
									int j = 0;
									nrStatisticDataRow[mrStatisticCloumeHeaderString[j++]] =  Tools::string2String(pStatic->TradingDay);						
									nrStatisticDataRow[mrStatisticCloumeHeaderString[j++]] =  lstrName;
									nrStatisticDataRow[mrStatisticCloumeHeaderString[j++]] =  lTempVector[0];
									StatisticDataTable->Rows->Add(nrStatisticDataRow);

								}
							}
						}
						

						DataRow ^ nrDataRow = RealDataTable->NewRow();	
						int j = 0;
						nrDataRow[mrCloumeHeaderString[j++]] =  Tools::string2String(pStatic->TradingDay);						
						nrDataRow[mrCloumeHeaderString[j++]] =  Tools::string2String(pStatic->InstrumentID);
						nrDataRow[mrCloumeHeaderString[j++]] =  Tools::string2String(pStatic->BrokerID);


						nrDataRow[mrCloumeHeaderString[j++]] =  Tools::string2String(pStatic->InvestorID);
						
						nrDataRow[mrCloumeHeaderString[j++]] =  Tools::string2String(CFieldDetail::PosiDirection2String(pStatic->PosiDirection));;

						nrDataRow[mrCloumeHeaderString[j++]] =  Tools::string2String(CFieldDetail::HedgeFlag2String(pStatic->HedgeFlag));;;
						nrDataRow[mrCloumeHeaderString[j++]] =  "历史持仓";
						nrDataRow[mrCloumeHeaderString[j++]] =  pStatic->YdPosition;
						nrDataRow[mrCloumeHeaderString[j++]] =  pStatic->Position;
						nrDataRow[mrCloumeHeaderString[j++]] = pStatic->LongFrozen;
						nrDataRow[mrCloumeHeaderString[j++]] = pStatic->ShortFrozen;
						nrDataRow[mrCloumeHeaderString[j++]] =  ConvertDouble(pStatic->LongFrozenAmount);
						nrDataRow[mrCloumeHeaderString[j++]] =  ConvertDouble(pStatic->ShortFrozenAmount);
						nrDataRow[mrCloumeHeaderString[j++]] = pStatic->OpenVolume;
						nrDataRow[mrCloumeHeaderString[j++]] = pStatic->CloseVolume;
						nrDataRow[mrCloumeHeaderString[j++]] =  ConvertDouble(pStatic->OpenAmount);
						nrDataRow[mrCloumeHeaderString[j++]] =  ConvertDouble(pStatic->CloseAmount);
						nrDataRow[mrCloumeHeaderString[j++]] =  ConvertDouble(pStatic->PositionCost);
						nrDataRow[mrCloumeHeaderString[j++]] =  ConvertDouble(pStatic->PreMargin);
						nrDataRow[mrCloumeHeaderString[j++]] =  ConvertDouble(pStatic->UseMargin);
						nrDataRow[mrCloumeHeaderString[j++]] =  ConvertDouble(pStatic->FrozenMargin);
						nrDataRow[mrCloumeHeaderString[j++]] =  ConvertDouble(pStatic->FrozenCash);
						nrDataRow[mrCloumeHeaderString[j++]] =  ConvertDouble(pStatic->FrozenCommission);
						nrDataRow[mrCloumeHeaderString[j++]] =  ConvertDouble(pStatic->CashIn);
						nrDataRow[mrCloumeHeaderString[j++]] =  ConvertDouble(pStatic->Commission);
						nrDataRow[mrCloumeHeaderString[j++]] =  ConvertDouble(pStatic->CloseProfit);
						nrDataRow[mrCloumeHeaderString[j++]] =  ConvertDouble(pStatic->PositionProfit);
						nrDataRow[mrCloumeHeaderString[j++]] = pStatic->PreSettlementPrice;
						nrDataRow[mrCloumeHeaderString[j++]] = pStatic->SettlementPrice;
						nrDataRow[mrCloumeHeaderString[j++]] = Tools::string2String(pStatic->TradingDay);
						nrDataRow[mrCloumeHeaderString[j++]] = pStatic->SettlementID;
						nrDataRow[mrCloumeHeaderString[j++]] =  ConvertDouble(pStatic->OpenCost);
						nrDataRow[mrCloumeHeaderString[j++]] =  ConvertDouble(pStatic->ExchangeMargin);
						nrDataRow[mrCloumeHeaderString[j++]] = pStatic->CombPosition;
						nrDataRow[mrCloumeHeaderString[j++]] = pStatic->CombLongFrozen;
						nrDataRow[mrCloumeHeaderString[j++]] = pStatic->CombShortFrozen;
						nrDataRow[mrCloumeHeaderString[j++]] =  ConvertDouble(pStatic->CloseProfitByDate);
						nrDataRow[mrCloumeHeaderString[j++]] =  ConvertDouble(pStatic->CloseProfitByTrade);
						nrDataRow[mrCloumeHeaderString[j++]] = pStatic->TodayPosition;
						nrDataRow[mrCloumeHeaderString[j++]] = pStatic->MarginRateByMoney;
						nrDataRow[mrCloumeHeaderString[j++]] = pStatic->MarginRateByVolume;
					    RealDataTable->Rows->Add(nrDataRow);
					}


				}
				

			}
			return false;
		}

		//数据接收接口
		virtual bool OnReponseDataReciver() override
		{
			return true;
		}

		//生成表头
		virtual bool GenerateGridViewHeader() override
		{			
			/*for(Int32 i = 0; i < mrCloumeHeaderString->Length;i++)
			{
				RealDataTable->Columns->Add(mrCloumeHeaderString[i]);
			}*/
			
			RealDataTable->Columns->Clear();
			for(Int32 i = 0; i < mrCloumeHeaderString->Length;i++)
			{
				RealDataTable->Columns->Add(mrCloumeHeaderString[i],mrColumnType[i]);
			}
			
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
			//if(nCondition)
			//{
			//	//nrView->Rows->Clear();
			//	RealDataTable->Rows->Clear();
			//	RealDataTable->Columns->Clear();
			//	for(Int32 i = 0; i < mrCloumeHeaderString->Length;i++)
			//	{
			//		RealDataTable->Columns->Add(mrCloumeHeaderString[i]);
			//	}
			//	//sample data
			//	DateTime ^ nrDateTime = nCondition->mstartTime;
			//	for(Int32 i = 0; i < 100;i++)
			//	{
			//		DataRow ^ nrDataRow = RealDataTable->NewRow();
			//		nrDateTime = nrDateTime->AddDays(2.0);
			//		nrDataRow[mrCloumeHeaderString[0]] =  nrDateTime->ToString("yyyy-MM-dd");				
			//		nrDataRow[mrCloumeHeaderString[1]] = "001";
			//		nrDataRow[mrCloumeHeaderString[2]] = "188001";
			//		nrDataRow[mrCloumeHeaderString[3]] = "483024.00";
			//		nrDataRow[mrCloumeHeaderString[4]] = "60";
			//		nrDataRow[mrCloumeHeaderString[5]] = "434344.00";
			//		nrDataRow[mrCloumeHeaderString[6]] = "100000.00";
			//		nrDataRow[mrCloumeHeaderString[7]] = "84032.00";
			//		nrDataRow[mrCloumeHeaderString[8]] = "84032.00";
			//		nrDataRow[mrCloumeHeaderString[9]] = "4348.00";
			//		nrDataRow[mrCloumeHeaderString[10]] = "432424.00";
			//		nrDataRow[mrCloumeHeaderString[11]] = "6";
			//		nrDataRow[mrCloumeHeaderString[12]] = "44443.00";
			//		nrDataRow[mrCloumeHeaderString[13]] = "44443.00";
			//		nrDataRow[mrCloumeHeaderString[14]] = "90000.00";
			//		nrDataRow[mrCloumeHeaderString[15]] = "54344.00";
			//		nrDataRow[mrCloumeHeaderString[16]] = "2444";
			//		nrDataRow[mrCloumeHeaderString[17]] = "6000";
			//		nrDataRow[mrCloumeHeaderString[18]] = "100000";
			//		nrDataRow[mrCloumeHeaderString[19]] = System::Random::Random(Guid::NewGuid().GetHashCode()).Next(10000,1000000);
			//		nrDataRow[mrCloumeHeaderString[20]] = i % 2 ?System::Random::Random(Guid::NewGuid().GetHashCode()).Next(-600000,600000) :-System::Random::Random(Guid::NewGuid().GetHashCode()).Next(-600000,600000);
			//		nrDataRow[mrCloumeHeaderString[21]] = "43455";
			//		nrDataRow[mrCloumeHeaderString[22]] = "424434";
			//		nrDataRow[mrCloumeHeaderString[23]] = "233";
			//		nrDataRow[mrCloumeHeaderString[24]] = "4444";
			//		nrDataRow[mrCloumeHeaderString[25]] = "43455";
			//		nrDataRow[mrCloumeHeaderString[26]] = "424434";
			//		nrDataRow[mrCloumeHeaderString[27]] = "233";
			//		nrDataRow[mrCloumeHeaderString[28]] = "4444";
			//		nrDataRow[mrCloumeHeaderString[29]] = "424434";
			//		nrDataRow[mrCloumeHeaderString[30]] = "233";
			//		nrDataRow[mrCloumeHeaderString[31]] = "4444";
			//		nrDataRow[mrCloumeHeaderString[32]] = "43455";
			//		nrDataRow[mrCloumeHeaderString[33]] = "424434";
			//		nrDataRow[mrCloumeHeaderString[34]] = "233";
			//		nrDataRow[mrCloumeHeaderString[35]] = "4444";
			//		nrDataRow[mrCloumeHeaderString[36]] = "43455";
			//		nrDataRow[mrCloumeHeaderString[37]] = "424434";
			//		nrDataRow[mrCloumeHeaderString[38]] = "233";
			//		nrDataRow[mrCloumeHeaderString[39]] = "4444";
			//		nrDataRow[mrCloumeHeaderString[40]] = "424434";
			//		//nrDataRow[mrCloumeHeaderString[41]] = "233";
			//		
			//		RealDataTable->Rows->Add(nrDataRow);
			//	}
			//	//nrView->DataSource = RealDataTable;				
			//}
			return true;
		}

		//填充chart
		virtual bool FillChart(System::Windows::Forms::FlowLayoutPanel^ nrChartContainer) override
		{
			if(nrChartContainer)
			{
				nrChartContainer->Controls->Clear();
				nrChartContainer->Update();
				cliext::map<String^,TimeStaticHodeType^>::iterator lIterStatic = mStaticHode->begin();
				for(;lIterStatic != mStaticHode->end(); lIterStatic++)
				{

					
					System::Windows::Forms::DataVisualization::Charting::Chart ^ lChart = 
						gcnew System::Windows::Forms::DataVisualization::Charting::Chart();

					lChart->Size = System::Drawing::Size(1400,500);
					StyleChart(lChart);
					
					lChart->Titles[0]->Text = lIterStatic->first;
				
					//添加数据到serial
					TimeStaticHodeType ^ ltempTimeHode = lIterStatic->second;				
					TimeStaticHodeType::iterator lIterTime ;
					int lnLast = 0;
					for (lIterTime = ltempTimeHode->begin(); lIterTime != ltempTimeHode->end();lIterTime++)
					{
						cliext::vector<Int32> ^ lTempVector = lIterTime->second;
						lChart->Series["逐日持仓"]->Points->AddXY(lIterTime->first,lTempVector[0]);
						lChart->Series["日内增减量"]->Points->AddXY(lIterTime->first,lTempVector[0] - lnLast);
						lnLast = lTempVector[0];
					}

					//添加tooltip事件
					lChart->GetToolTipText += gcnew EventHandler<ToolTipEventArgs^>(this,&CAccountHodeReportDataProvider::ChartLineGetToolTipText);
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