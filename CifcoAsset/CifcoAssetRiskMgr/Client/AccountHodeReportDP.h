
#pragma once
#include "AbstractReportDataProvider.h"
using namespace System;
using namespace System::Data;
using namespace System::Windows::Forms::DataVisualization::Charting;


namespace ReportDataProvider {


	
	public ref class CAccountHodeReportDataProvider :public IReportDataProvider
	{
	public:
		
	private:
		array<String^>^ mrCloumeHeaderString;
		cliext::map<String^,TimeStaticHodeType^> ^ mStaticHode; //按账号统计的持仓结果
	
	public:
		CAccountHodeReportDataProvider()
		{
			mStaticHode = gcnew cliext::map<String^,TimeStaticHodeType^>();
			mrCloumeHeaderString = gcnew array<String^> {"日期","流中唯一的序列号","合约代码","经纪公司代码","投资者代码","持仓多空方向","投机套保标志",
				"持仓日期","上日持仓","今日持仓","多头冻结","空头冻结",
				"开仓冻结金额","平仓冻结金额","开仓量","平仓量","开仓金额",
				"平仓金额","持仓成本","上次占用的保证金","占用的保证金","冻结的保证金","冻结的资金","冻结的手续费",
				"资金差额","手续费","平仓盈亏","持仓盈亏","上次结算价","本次结算价","交易日","结算编号","开仓成本",
				"交易所保证金","组合成交形成的持仓","组合多头冻结","组合空头冻结","逐日盯市平仓盈亏","逐笔对冲平仓盈亏","今仓",
				"保证金率","保证金率(按手数)"};
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
		virtual void SubscribeEvt(CQueryConditions^ nCondition,IntPtr hWnd) override
		{
			CTcpLayer::SubscribePkg(Cmd_RM_QryHistroyPosition_Rsp,(int)hWnd);
			//CTcpLayer::SendData()
			HisTradeDataQueryKey lHisQuery ;
			memset(&lHisQuery,0,sizeof(lHisQuery));
			std::string lstrStartTime = Tools::String2string(nCondition->mstartTime.ToString("yyyy-MM-dd"));
			std::string lstrEndTime = Tools::String2string(nCondition->mendTime.ToString("yyyy-MM-dd"));
			strcpy(lHisQuery.TradingDayFrom,lstrStartTime.c_str());
			strcpy(lHisQuery.TradingDayTo,lstrEndTime.c_str());
			CTcpLayer::SendData(Cmd_RM_QryHistroyPosition_Req, (void*)&lHisQuery, sizeof(HisTradeDataQueryKey), 0);
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
			if(head.cmdid == Cmd_RM_QryHistroyPosition_Rsp && head.userdata1 == OPERATOR_SUCCESS)
			{
				int count= head.len/sizeof(SequencialPositionField);
				if(count != 0)
				{
					RealDataTable->Rows->Clear();
					RealDataTable->Columns->Clear();
					for(Int32 i = 0; i < mrCloumeHeaderString->Length;i++)
					{
						RealDataTable->Columns->Add(mrCloumeHeaderString[i]);
					}
					mStaticHode->clear();
					SequencialPositionField* pStatic = NULL;
					for(Int32 i = 0; i < count;i++)
					{
						pStatic = (SequencialPositionField*)
							(npRespond+sizeof(Stru_UniPkgHead)+i*sizeof(SequencialPositionField));

						//过滤
						if(FilterAccount(std::string(pStatic->InvestorID)))
							continue;
						if(FilterInstrument(std::string(pStatic->InstrumentID)))
							continue;
						//统计
						//先找到帐号
						String ^ lstrName = Tools::string2String(pStatic->InvestorID);					
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
							}
							else
							{
								//找到对应日期的记录，修改
								cliext::vector<Int32> ^ lTempVector = lIterTime->second;
								lTempVector[0] += pStatic->Position;
								lTempVector[1] += pStatic->YdPosition;
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
							
						}

						DataRow ^ nrDataRow = RealDataTable->NewRow();						
						nrDataRow[mrCloumeHeaderString[0]] =  Tools::string2String(pStatic->TradingDay);				
						nrDataRow[mrCloumeHeaderString[1]] =  pStatic->UniqSequenceNo;
						nrDataRow[mrCloumeHeaderString[2]] =  Tools::string2String(pStatic->InstrumentID);
						nrDataRow[mrCloumeHeaderString[3]] =  Tools::string2String(pStatic->BrokerID);
						nrDataRow[mrCloumeHeaderString[4]] =  Tools::string2String(pStatic->InvestorID);
						
						nrDataRow[mrCloumeHeaderString[5]] =  Tools::string2String(Client::CFieldDetail::PosiDirection2String(pStatic->PosiDirection));;

						nrDataRow[mrCloumeHeaderString[6]] =  Tools::string2String(Client::CFieldDetail::HedgeFlag2String(pStatic->HedgeFlag));;;
						nrDataRow[mrCloumeHeaderString[7]] =  "历史持仓";
						nrDataRow[mrCloumeHeaderString[8]] =  pStatic->YdPosition;
						nrDataRow[mrCloumeHeaderString[9]] =  pStatic->Position;
						nrDataRow[mrCloumeHeaderString[10]] = pStatic->LongFrozen;
						nrDataRow[mrCloumeHeaderString[11]] = pStatic->ShortFrozen;
						nrDataRow[mrCloumeHeaderString[12]] =  Convert::ToDouble(pStatic->LongFrozenAmount).ToString("N2");
						nrDataRow[mrCloumeHeaderString[13]] =  Convert::ToDouble(pStatic->ShortFrozenAmount).ToString("N2");
						nrDataRow[mrCloumeHeaderString[14]] = pStatic->OpenVolume;
						nrDataRow[mrCloumeHeaderString[15]] = pStatic->CloseVolume;
						nrDataRow[mrCloumeHeaderString[16]] =  Convert::ToDouble(pStatic->OpenAmount).ToString("N2");
						nrDataRow[mrCloumeHeaderString[17]] =  Convert::ToDouble(pStatic->CloseAmount).ToString("N2");
						nrDataRow[mrCloumeHeaderString[18]] =  Convert::ToDouble(pStatic->PositionCost).ToString("N2");
						nrDataRow[mrCloumeHeaderString[19]] =  Convert::ToDouble(pStatic->PreMargin).ToString("N2");
						nrDataRow[mrCloumeHeaderString[20]] =  Convert::ToDouble(pStatic->UseMargin).ToString("N2");
						nrDataRow[mrCloumeHeaderString[21]] =  Convert::ToDouble(pStatic->FrozenMargin).ToString("N2");
						nrDataRow[mrCloumeHeaderString[22]] =  Convert::ToDouble(pStatic->FrozenCash).ToString("N2");
						nrDataRow[mrCloumeHeaderString[23]] =  Convert::ToDouble(pStatic->FrozenCommission).ToString("N2");
						nrDataRow[mrCloumeHeaderString[24]] =  Convert::ToDouble(pStatic->CashIn).ToString("N2");
						nrDataRow[mrCloumeHeaderString[25]] =  Convert::ToDouble(pStatic->Commission).ToString("N2");
						nrDataRow[mrCloumeHeaderString[26]] =  Convert::ToDouble(pStatic->CloseProfit).ToString("N2");
						nrDataRow[mrCloumeHeaderString[27]] =  Convert::ToDouble(pStatic->PositionProfit).ToString("N2");
						nrDataRow[mrCloumeHeaderString[28]] = pStatic->PreSettlementPrice;
						nrDataRow[mrCloumeHeaderString[29]] = pStatic->SettlementPrice;
						nrDataRow[mrCloumeHeaderString[30]] = Tools::string2String(pStatic->TradingDay);
						nrDataRow[mrCloumeHeaderString[31]] = pStatic->SettlementID;
						nrDataRow[mrCloumeHeaderString[32]] =  Convert::ToDouble(pStatic->OpenCost).ToString("N2");
						nrDataRow[mrCloumeHeaderString[33]] =  Convert::ToDouble(pStatic->ExchangeMargin).ToString("N2");
						nrDataRow[mrCloumeHeaderString[34]] = pStatic->CombPosition;
						nrDataRow[mrCloumeHeaderString[35]] = pStatic->CombLongFrozen;
						nrDataRow[mrCloumeHeaderString[36]] = pStatic->CombShortFrozen;
						nrDataRow[mrCloumeHeaderString[37]] =  Convert::ToDouble(pStatic->CloseProfitByDate).ToString("N2");
						nrDataRow[mrCloumeHeaderString[38]] =  Convert::ToDouble(pStatic->CloseProfitByTrade).ToString("N2");
						nrDataRow[mrCloumeHeaderString[39]] = pStatic->TodayPosition;
						nrDataRow[mrCloumeHeaderString[40]] = pStatic->MarginRateByMoney;
						nrDataRow[mrCloumeHeaderString[41]] = pStatic->MarginRateByVolume;
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

					lChart->Size = System::Drawing::Size(800,400);
					StyleChart(lChart);
					
					lChart->Titles[0]->Text = lIterStatic->first;
				
					//添加数据到serial
					TimeStaticHodeType ^ ltempTimeHode = lIterStatic->second;				
					TimeStaticHodeType::iterator lIterTime ;
					for (lIterTime = ltempTimeHode->begin(); lIterTime != ltempTimeHode->end();lIterTime++)
					{
						cliext::vector<Int32> ^ lTempVector = lIterTime->second;
						lChart->Series["逐日持仓"]->Points->AddXY(lIterTime->first,lTempVector[0]);
						lChart->Series["日内增减量"]->Points->AddXY(lIterTime->first,lTempVector[0] - lTempVector[1]);
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