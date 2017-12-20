#pragma once
#include "AbstractReportDataProvider.h"
using namespace System;
using namespace System::Data;
using namespace System::Windows::Forms::DataVisualization::Charting;


namespace ReportDataProvider {


	
	public ref class CAccountInterestReportDataProvider :public IReportDataProvider
	{	
	private:
		

		array<String^>^ mrCloumeHeaderString;
		array<System::Type^>^ mrColumnType;
		cliext::map<String^,TimeStaticBalanceType^> ^ mStaticBalance; //按账号统计的持仓结果

		array<String^>^ mrStatisticCloumeHeaderString;
		array<System::Type^>^ mrStatisticColumnType;
		SAdminQuery * mHisQuery;
		Int32 mQueryBufferSize;
		Int32 mnUserId;
	public:
		CAccountInterestReportDataProvider()
		{
			mHisQuery = NULL;
			mQueryBufferSize = 0;
			mnUserId = 0;
			mStaticBalance = gcnew cliext::map<String^,TimeStaticBalanceType^>();
			mrCloumeHeaderString = gcnew array<String^>{"日期",
				"经纪公司代码",
				"投资者账号",
				"上次质押金额",
				"上次信用额度",
				"上次存款额",
				"上次总权益",
				"上次占用的保证金",
				"基本准备金",
				"上次交易所保证金",
				"投资者交割保证金",
				"交易所交割保证金",
				"出入金金额",
				"当前保证金总额",
				"手续费",
				"平仓盈亏",
				"持仓盈亏",
				"总权益",
				"可用资金",
				"质押金额",
				"交易所保证金",
				"可取资金",
				"信用额度",
				"保证金冻结",
				"手续费冻结"};

			mrColumnType = gcnew array<Type^> {Type::GetType("System.String"),
				Type::GetType("System.String"),
				Type::GetType("System.String"),
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
				Type::GetType("System.Double"),
				Type::GetType("System.Double"),
				Type::GetType("System.Double"),
				Type::GetType("System.Double"),
				Type::GetType("System.Double"),
				Type::GetType("System.Double"),
				Type::GetType("System.Double"),
				Type::GetType("System.Double")};

			mrStatisticCloumeHeaderString = gcnew array<String^> {"日期",
				"统计名称",
				"总权益"};
			mrStatisticColumnType = gcnew array<Type^> {Type::GetType("System.String"),
				Type::GetType("System.String"),
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
				mrSeriesGraphList->Name = L"每日权益变化量";
				mrSeriesGraphList->XValueType = System::Windows::Forms::DataVisualization::Charting::ChartValueType::DateTime;

				mrSeriesLine->ChartArea = L"ChartArea1";				
				mrSeriesLine->Color = System::Drawing::Color::FromArgb(220,120,150,20);
				mrSeriesLine->Legend = L"LegendLine";
				mrSeriesLine->Name = L"逐日权益曲线";
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
				//nrChart->DataSource = RealDataTable;
			}
			return true;
		}
	public:
		//订阅事件
		virtual void SubscribeEvt(CQueryConditions^ nCondition,IntPtr hWnd ,Int32 nUserID) override
		{
			//CTcpLayer::SubscribePkg(Cmd_RM_QryHistoryFundInfo_Rsp,(int)hWnd);
			//RealDataTable->Rows->Clear();					
			//mStaticBalance->clear();				
			//StatisticDataTable->Rows->Clear();

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
			//	AddQuerySeq(lTempTime,lnSeq++);
			////	CTcpLayer::SendData(Cmd_RM_QryHistoryFundInfo_Req, (void*)lHisQuery, nBufSize,lnSeq++,nUserID);
			//	QueryMode = nCondition->mnQueryMode;

			//	lTempTime = lTempTime->AddDays(1.0);
			//}
			//free(lHisQuery);
			CTcpLayer::SubscribePkg(Cmd_RM_QryHistoryFundInfo_Rsp,(int)hWnd);
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
				CTcpLayer::SendData(Cmd_RM_QryHistoryFundInfo_Req, (void*)mHisQuery, mQueryBufferSize,lnCurrentSeq,mnUserId);
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
			if(head.cmdid == Cmd_RM_QryHistoryFundInfo_Rsp)
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
					CTcpLayer::SendData(Cmd_RM_QryHistoryFundInfo_Req, (void*)mHisQuery, mQueryBufferSize,lnCurrentSeq,mnUserId);
				}
			}
			if(head.cmdid == Cmd_RM_QryHistoryFundInfo_Rsp && head.userdata1 == CF_ERROR_SUCCESS)
			{
				int count= head.len/sizeof(PlatformStru_TradingAccountInfo);
				if(count != 0)
				{			

					PlatformStru_TradingAccountInfo* pStatic = NULL;
					for(Int32 i = 0; i < count;i++)
					{
						pStatic = (PlatformStru_TradingAccountInfo*)
							(npRespond+sizeof(Stru_UniPkgHead)+i*sizeof(PlatformStru_TradingAccountInfo));

						//过滤
						if(FilterAccount(std::string(pStatic->AccountID)))
							continue;

						//统计
						//先找到帐号
						array<String^>^ lStaticKey = GetStaticKey( Tools::string2String(pStatic->AccountID), nullptr);
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
										lTempVector->push_back(pStatic->DynamicProfit);
										lTempVector->push_back(pStatic->PreBalance);
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
										cliext::vector<Double> ^ lTempVector = lIterTime->second;
										lTempVector[0] += pStatic->DynamicProfit;
										lTempVector[1] += pStatic->PreBalance;

										cli::array<String^>^ key = {
											Tools::string2String(pStatic->TradingDay),
											lstrName											
										};
										DataRow ^ nrStatisticDataRow = StatisticDataTable->Rows->Find(key);
										int j = 0;
										nrStatisticDataRow[mrStatisticCloumeHeaderString[j++]] =  Tools::string2String(pStatic->TradingDay);						
										nrStatisticDataRow[mrStatisticCloumeHeaderString[j++]] =  lstrName;
										nrStatisticDataRow[mrStatisticCloumeHeaderString[j++]] =  lTempVector[0];
									}
								}
								else
								{
									//没有找到对应的帐号，插入一条
									TimeStaticBalanceType ^ ltempTimeHode = gcnew TimeStaticBalanceType();
									String ^ lstrTime = Tools::string2String(pStatic->TradingDay);							
									cliext::vector<Double> ^ lTempVector = gcnew cliext::vector<Double>();
									lTempVector->push_back(pStatic->DynamicProfit);
									lTempVector->push_back(pStatic->PreBalance);
									ltempTimeHode->insert(cliext::make_pair(lstrTime,lTempVector));

									mStaticBalance->insert(cliext::make_pair(lstrName,ltempTimeHode));

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
						nrDataRow[mrCloumeHeaderString[j++]] =  Tools::string2String(pStatic->BrokerID);				
						nrDataRow[mrCloumeHeaderString[j++]] =  Tools::string2String(pStatic->AccountID);
						nrDataRow[mrCloumeHeaderString[j++]] =  ConvertDouble(pStatic->PreMortgage);
						nrDataRow[mrCloumeHeaderString[j++]] =  ConvertDouble(pStatic->PreCredit);						
						nrDataRow[mrCloumeHeaderString[j++]] =  ConvertDouble(pStatic->PreDeposit);
						nrDataRow[mrCloumeHeaderString[j++]] =  ConvertDouble(pStatic->PreBalance);
						nrDataRow[mrCloumeHeaderString[j++]] =  ConvertDouble(pStatic->PreMargin);
						nrDataRow[mrCloumeHeaderString[j++]] =  ConvertDouble(pStatic->Reserve);						
						nrDataRow[mrCloumeHeaderString[j++]] = ConvertDouble(pStatic->ExchangeMargin);					
						nrDataRow[mrCloumeHeaderString[j++]] = ConvertDouble(pStatic->DeliveryMargin);
						nrDataRow[mrCloumeHeaderString[j++]] = ConvertDouble(pStatic->ExchangeDeliveryMargin);
						nrDataRow[mrCloumeHeaderString[j++]] = ConvertDouble(pStatic->Withdraw);
						nrDataRow[mrCloumeHeaderString[j++]] = ConvertDouble(pStatic->CurrMargin);
						nrDataRow[mrCloumeHeaderString[j++]] = ConvertDouble(pStatic->Commission);					
						nrDataRow[mrCloumeHeaderString[j++]] = ConvertDouble(pStatic->CloseProfit);
						nrDataRow[mrCloumeHeaderString[j++]] = ConvertDouble(pStatic->PositionProfit);
						nrDataRow[mrCloumeHeaderString[j++]] = ConvertDouble(pStatic->Balance);
						nrDataRow[mrCloumeHeaderString[j++]] = ConvertDouble(pStatic->Available);
						nrDataRow[mrCloumeHeaderString[j++]] = ConvertDouble(pStatic->Mortgage);
						nrDataRow[mrCloumeHeaderString[j++]] = ConvertDouble(pStatic->ExchangeMargin);
						nrDataRow[mrCloumeHeaderString[j++]] = ConvertDouble(pStatic->WithdrawQuota);
						nrDataRow[mrCloumeHeaderString[j++]] = ConvertDouble(pStatic->Credit);
						nrDataRow[mrCloumeHeaderString[j++]] = ConvertDouble(pStatic->FrozenMargin);
						nrDataRow[mrCloumeHeaderString[j++]] = ConvertDouble(pStatic->FrozenCommission);
					

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
			//生成一个chartarea 和两个serial的chart
			//其中一个serial为线性图 另外一个为柱状图
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
					StyleChart(lChart);

					lChart->Titles[0]->Text = lIterStatic->first;

					//添加数据到serial
					TimeStaticBalanceType ^ ltempTimeHode = lIterStatic->second;				
					TimeStaticBalanceType::iterator lIterTime ;
					Double ldbLast = 0.0f;
					for (lIterTime = ltempTimeHode->begin(); lIterTime != ltempTimeHode->end();lIterTime++)
					{
						cliext::vector<Double> ^ lTempVector = lIterTime->second;
						lChart->Series["逐日权益曲线"]->Points->AddXY(lIterTime->first,lTempVector[0]);
						
						lChart->Series["每日权益变化量"]->Points->AddXY(lIterTime->first,lTempVector[0] - ldbLast);
						ldbLast = lTempVector[0];

					}

					//添加tooltip事件
					lChart->GetToolTipText += gcnew EventHandler<ToolTipEventArgs^>(this,&CAccountInterestReportDataProvider::ChartLineGetToolTipText);
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