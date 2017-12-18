#pragma once
#include "MapOrgnization.h"
#include "MapOrgLevelMapZoomLevel.h"
#include "RiskEvt2.h"
#include "DepartmentShow.h"
#include "ProductShow.h"
#include "CDataInfo.h"
#include "Tools.h"
#include "RiskMapData.h"
namespace Client {

	using namespace RiskMap;
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	using namespace CFMap::CoreType;
	using namespace CFMap::Interface;
	using namespace CFMap::MapProviders;
	using namespace CFMap::CacheProviders;
	using namespace CFMap::WindowsForms;

	using namespace System::Windows::Forms;
	using namespace System::Windows::Forms::DataVisualization;
	using namespace System::Windows::Forms::DataVisualization::Charting;

  
	/// <summary>
	/// Summary for RiskMapForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class RiskMapForm : public System::Windows::Forms::Form
	{
	public:
		
		RiskMapForm(void)
		{
			
			mrOrgnization = gcnew COrgnization();
			m_pdataInfo = CDataInfo::NewInstance();
			InitializeComponent();
			mbChickToZoom = false;
			mRiskMapData = CRiskMapData::NewInstance();
			mnCurrentOrgParentID = COrgnization::INVALID_PARENT_ID;
			mOrgHZDataTable = gcnew DataTable();
			Random^ rand = gcnew Random(Guid::NewGuid().GetHashCode());
			mnSeqID = rand->Next(1000000);
			mnCurrentFinOrgID = -1;
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~RiskMapForm()
		{
			if(mRiskMapData)
				CRiskMapData::ReleaseInstance(mRiskMapData);
			if (components)
			{
				delete components;
			}
			
		}
	private: Int32 mnSeqID;
	private: CDataInfo* m_pdataInfo;
	private: CFMap::WindowsForms::CFMapControl^  cfMapControl1;
	private: CFMapOverlay^ mRiskMakerOverlay ;
	private: CRiskMapData * mRiskMapData;
	private: Int32 mnCurrentOrgParentID; //当前处理的父组织机构的ID
	private: Int32 mnCurrentFinOrgID; //当前处理的订阅机构和理财产品的ID
	private: DataTable ^ mOrgHZDataTable;
	private: array<String^>^ mHeaders;
	
	
	//风险事件处理模块，包括当前风险事件、历史风险事件、用户风险列表
	
	private: COrgnization ^ mrOrgnization;
	private: System::Windows::Forms::DataGridView^  dataGridView1;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^  chart1;


	/*private: System::Windows::Forms::DataVisualization::Charting::Series^  series2 ;
	private: System::Windows::Forms::DataVisualization::Charting::Series^  series3 ;
	private: System::Windows::Forms::DataVisualization::Charting::Series^  series4;*/
	private: Int32 mnOlderZoom;
	private: bool mbChickToZoom ;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button2;
	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		/*
		// Latitude=34.524661, Longitude=109.511719

		this->cfMapControl1->MapProvider = CFMap::Interface::CCFMapProviders::OpenStreetMap;
		this->cfMapControl1->Position = PointLatLng(34.524661, 109.511719);
		this->cfMapControl1->Manager->Mode = CFMap::CoreType::AccessMode::ServerOnly;
		*/
		void InitializeComponent(void)
		{
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle1 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea1 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^  legend1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^  series1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Title^  title1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Title());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(RiskMapForm::typeid));
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->cfMapControl1 = (gcnew CFMap::WindowsForms::CFMapControl());
			this->chart1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->chart1))->BeginInit();
			this->SuspendLayout();
			// 
			// dataGridView1
			// 
			this->dataGridView1->AllowUserToAddRows = false;
			this->dataGridView1->AllowUserToDeleteRows = false;
			this->dataGridView1->AllowUserToResizeColumns = false;
			this->dataGridView1->AllowUserToResizeRows = false;
			this->dataGridView1->ReadOnly = true;
			dataGridViewCellStyle1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), 
				static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
			this->dataGridView1->AlternatingRowsDefaultCellStyle = dataGridViewCellStyle1;
			this->dataGridView1->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::AllCells;
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView1->Location = System::Drawing::Point(505, 12);
			this->dataGridView1->MultiSelect = false;
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->RowHeadersVisible = false;
			this->dataGridView1->RowHeadersWidthSizeMode = System::Windows::Forms::DataGridViewRowHeadersWidthSizeMode::AutoSizeToDisplayedHeaders;
			this->dataGridView1->RowTemplate->Height = 23;
			this->dataGridView1->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dataGridView1->Size = System::Drawing::Size(418, 256);
			this->dataGridView1->TabIndex = 3;
			this->dataGridView1->CellClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &RiskMapForm::OnCellClick);
			// 
			// cfMapControl1
			// 
			this->cfMapControl1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->cfMapControl1->Bearing = 0;
			this->cfMapControl1->CanDragMap = true;
			this->cfMapControl1->EmptyTileColor = System::Drawing::Color::Navy;
			this->cfMapControl1->GrayScaleMode = false;
			this->cfMapControl1->HelperLineOption = CFMap::WindowsForms::HelperLineOptions::DontShow;
			this->cfMapControl1->LevelsKeepInMemmory = 5;
			this->cfMapControl1->Location = System::Drawing::Point(3, 12);
			this->cfMapControl1->MarkersEnabled = true;
			this->cfMapControl1->MaxZoom = 13;
			this->cfMapControl1->MinZoom = 4;
			this->cfMapControl1->MouseWheelZoomType = CFMap::CoreType::MouseWheelZoomType::MousePositionAndCenter;
			this->cfMapControl1->Name = L"cfMapControl1";
			this->cfMapControl1->NegativeMode = false;
			this->cfMapControl1->PolygonsEnabled = false;
			this->cfMapControl1->RetryLoadTile = 0;
			this->cfMapControl1->RoutesEnabled = false;
			this->cfMapControl1->SelectedAreaFillColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(33)), 
				static_cast<System::Int32>(static_cast<System::Byte>(65)), static_cast<System::Int32>(static_cast<System::Byte>(105)), static_cast<System::Int32>(static_cast<System::Byte>(225)));
			this->cfMapControl1->ShowTileGridLines = false;
			this->cfMapControl1->Size = System::Drawing::Size(834, 593);
			this->cfMapControl1->TabIndex = 0;
			// 
			// chart1
			// 
			this->chart1->BackColor = System::Drawing::Color::Transparent;
			chartArea1->BackColor = System::Drawing::Color::Transparent;
			chartArea1->Name = L"ChartArea1";
			this->chart1->ChartAreas->Add(chartArea1);
			legend1->BackColor = System::Drawing::Color::Transparent;
			legend1->Name = L"Legend1";
			this->chart1->Legends->Add(legend1);
			this->chart1->Location = System::Drawing::Point(505, 274);
			this->chart1->Name = L"chart1";
			series1->ChartArea = L"ChartArea1";
			series1->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Pie;
			series1->Legend = L"Legend1";
			series1->Name = L"Default";
			this->chart1->Series->Add(series1);
			this->chart1->Size = System::Drawing::Size(411, 331);
			this->chart1->TabIndex = 4;
			this->chart1->Text = L"Pie";
			title1->Name = L"风险总数";
			this->chart1->Titles->Add(title1);
			this->chart1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &RiskMapForm::OnPieMouseMove);
			this->chart1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &RiskMapForm::OnPieMouseDown);
			// 
			// button1
			// 
			this->button1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"button1.BackgroundImage")));
			this->button1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->button1->FlatAppearance->BorderSize = 0;
			this->button1->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Silver;
			this->button1->Location = System::Drawing::Point(8, 16);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(74, 78);
			this->button1->TabIndex = 6;
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &RiskMapForm::OnEarthButtonClick);
			// 
			// button2
			// 
			this->button2->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"button2.BackgroundImage")));
			this->button2->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->button2->FlatAppearance->BorderSize = 0;
			this->button2->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Silver;
			this->button2->Location = System::Drawing::Point(88, 16);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(74, 78);
			this->button2->TabIndex = 6;
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &RiskMapForm::OnMapButtonClick);
			// 
			// RiskMapForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::AppWorkspace;
			this->ClientSize = System::Drawing::Size(928, 617);
			this->Controls->Add(this->dataGridView1);
			this->Controls->Add(this->chart1);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->cfMapControl1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::SizableToolWindow;
			this->Name = L"RiskMapForm";
			this->Text = L"汇总地图展示";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->Load += gcnew System::EventHandler(this, &RiskMapForm::RiskMapForm_Load);
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &RiskMapForm::RiskMapForm_FormClosing);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->chart1))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion


		void InitMap()
		{
			this->cfMapControl1->MapProvider = CFMap::Interface::CCFMapProviders::BingSatelliteMap;
			this->cfMapControl1->Position = PointLatLng(34.6961334816182, 109.2985095977783);
			this->cfMapControl1->Manager->Mode = CFMap::CoreType::AccessMode::CacheAndServer;
			this->cfMapControl1->Zoom = 4;
			this->cfMapControl1->CacheLocation = System::Environment::CurrentDirectory +  System::IO::Path::DirectorySeparatorChar;
			mnOlderZoom = this->cfMapControl1->Zoom;

			this->cfMapControl1->OnMarkerClick += gcnew MarkerClick(this,&RiskMapForm::OnRiskMarkerClick);
			this->cfMapControl1->OnMapZoomChanged += gcnew MapZoomChanged(this,&RiskMapForm::OnMapZoomChanged);
			     
		}
		void LoasSubAssertMgmtOrgan(Int32 nOrgLevel,
			         MAPAeestMgmtOrganization& mapMain, 
					 std::map<int, AssetMgmtOrganization>& mapSub,
					 Int32 nLevel) 
		{

			 MAPAeestMgmtOrganization::iterator it;
			 std::map<int, AssetMgmtOrganization>::iterator itSub;

			

			 itSub = mapSub.begin();
			 for(; itSub != mapSub.end(); itSub++) 
			 {
				/* int^ nID = itSub->first;
				 String^ strText = nID->ToString();*/
				 String^ strValue = Tools::string2String(itSub->second.szName);
				 //subNode = nodeMain->Nodes->Add(strText, strValue);
				 mrOrgnization->Insert(itSub->second.nAssetMgmtOrgID,PointLatLng(itSub->second.dLatitude, itSub->second.dLongitude),
					 strValue,nOrgLevel + 1,RiskLevel::RiskLevel_normal,"NULL",nLevel+1,itSub->second.nUpperLevelOrgID);

				 it = mapMain.find(itSub->first);
				 if(it != mapMain.end())
					 LoasSubAssertMgmtOrgan(nOrgLevel + 1, mapMain, it->second,nLevel+1);
			 }


		}

		void LoadAssertMgmtOrgan() 
		{			

			 if(!m_pdataInfo->IsInit())
				 return;

			 LoginRsp loginRsp;
			 memset(&loginRsp, 0, sizeof(loginRsp));
			 m_pdataInfo->GetLoginRsp(loginRsp);

			
			 m_pdataInfo->GetFinancialProduct(mRiskMapData->mmapFinanProduct);

			 AssetMgmtOrganization assertOrg;
			 memset(&assertOrg, 0, sizeof(assertOrg));
			 if(!m_pdataInfo->GetAssetMgmtOrg(
				 loginRsp.nAssetMgmtOrgID, assertOrg))
				 return;

			 MAPAeestMgmtOrganization mapAssetMgmtOrgTree;
			 m_pdataInfo->GetAeestMgmtOrg(mapAssetMgmtOrgTree);
			 MAPAeestMgmtOrganization::iterator it = 
				 mapAssetMgmtOrgTree.find(loginRsp.nAssetMgmtOrgID);
			 if(it == mapAssetMgmtOrgTree.end())
				 return;

			
			
			 //计算该组织在总树上的级别
             int lnLevel = 0;
			 int lnOrgID = loginRsp.nAssetMgmtOrgID;
			 while(lnOrgID != 0)
			 {
				 MAPAeestMgmtOrganization::iterator lIt = 
					 mapAssetMgmtOrgTree.find(lnOrgID);
				 if(lIt == mapAssetMgmtOrgTree.end())
					 break;

				 AssetMgmtOrganization lAssertOrg;
				 memset(&lAssertOrg, 0, sizeof(lAssertOrg));
				 if(!m_pdataInfo->GetAssetMgmtOrg(
					 lnOrgID, lAssertOrg))
					 break;

				 lnOrgID = lAssertOrg.nUpperLevelOrgID;
				 lnLevel++;
				 
			 }
			 

			 //插入父节点
			 String^ strValue = Tools::string2String(assertOrg.szName);
			 mrOrgnization->Insert(assertOrg.nAssetMgmtOrgID,PointLatLng(assertOrg.dLatitude, assertOrg.dLongitude),
				                    strValue,1,RiskLevel::RiskLevel_normal,"NULL",lnLevel,0);


			 //查看是否有子机构
		     std::map<int, AssetMgmtOrganization>& mapSub = it->second;
			 if(mapSub.begin()==mapSub.end())
				 return;

			 LoasSubAssertMgmtOrgan(1,mapAssetMgmtOrgTree, mapSub,lnLevel);


		}
		void InitMapOverlay()
		{
			
			mRiskMakerOverlay = gcnew CFMapOverlay();
			//mrOrgnization = gcnew COrgnization();
			/*mrOrgnization->Insert(1,PointLatLng(34.6961334816182, 109.2985095977783),"china",1,RiskLevel::RiskLevel_normal,"NULL",0);
			mrOrgnization->Insert(11,PointLatLng(39.909736, 116.455078),"北京",2,RiskLevel::RiskLevel_low,"NULL",1);
			mrOrgnization->Insert(12,PointLatLng(39.113014, 117.180176),"天津",2,RiskLevel::RiskLevel_normal,"NULL",1);
			mrOrgnization->Insert(13,PointLatLng(36.708064, 117.037354),"济南",2,RiskLevel::RiskLevel_normal,"NULL",1);
			mrOrgnization->Insert(31,PointLatLng(40.099084, 116.207886),"北京1",3,RiskLevel::RiskLevel_low,"NULL",11);
			mrOrgnization->Insert(32,PointLatLng(39.930801, 116.246338),"北京2",3,RiskLevel::RiskLevel_low,"NULL",11);
			mrOrgnization->Insert(33,PointLatLng(39.972911, 116.482544),"北京3",3,RiskLevel::RiskLevel_low,"NULL",11);
			mrOrgnization->Insert(34,PointLatLng(39.863371, 116.466064),"北京4",3,RiskLevel::RiskLevel_low,"NULL",11);
			mrOrgnization->Insert(35,PointLatLng(39.926588, 116.383667),"北京5",3,RiskLevel::RiskLevel_normal,"NULL",11);

			mrOrgnization->Insert(36,PointLatLng(39.377834, 117.143097),"天津1",3,RiskLevel::RiskLevel_low,"NULL",12);
			mrOrgnization->Insert(37,PointLatLng(39.147103, 117.296906),"天津2",3,RiskLevel::RiskLevel_low,"NULL",12);
			mrOrgnization->Insert(38,PointLatLng(39.026652, 116.990662),"天津3",3,RiskLevel::RiskLevel_low,"NULL",12);
			
			mrOrgnization->Insert(39,PointLatLng(36.675028, 117.045593),"济南1",3,RiskLevel::RiskLevel_low,"NULL",13);
			mrOrgnization->Insert(310,PointLatLng(36.639774, 116.990662),"济南2",3,RiskLevel::RiskLevel_low,"NULL",13);
			mrOrgnization->Insert(311,PointLatLng(36.701458, 117.037354),"济南3",3,RiskLevel::RiskLevel_low,"NULL",13);
		*/
			//获取组织架构参数
			//完善组织架构树
			//并将组织架构的marker 添加到map中
			LoadAssertMgmtOrgan();		

			this->cfMapControl1->Overlays->Add(mRiskMakerOverlay);			
			//添加组织架构的数据到地图图层中去

			TOrgnizationMap ^ lOrgMap = mrOrgnization->OrgMap;
			if(lOrgMap != nullptr)
			{
				TOrgnizationMap::iterator lIte = lOrgMap->begin();
				for (;lIte != lOrgMap->end();lIte++)
				{
					CSingleOrgnization ^ lTempOrg = lIte->second;
					Int32 lnMin, lnMax ;
					if(COrgLevelToMapZoomLevel::TranslateOrgLevelToZoomLevel(mrOrgnization->MaxScaleLevel,lTempOrg->ScaleLevel,&lnMin,&lnMax))
					{
						//如果是根节点，则最小缩放级别定位到地图的最小级别
						if(lTempOrg->ParentOrg == nullptr)
							lnMin = COrgLevelToMapZoomLevel::MinZoomLevel;
						CMarkerRisk ^ lTemp = gcnew CMarkerRisk(lTempOrg->ID,lTempOrg->LatLng,lnMin,lnMax,lTempOrg->OrgRiskLevel,lTempOrg->Name,lTempOrg->OrgLevel);
						lTempOrg->MakerRef = lTemp;
						mRiskMakerOverlay->Markers->Add( lTemp);
					}						
				}
			}
	
		}

		private: System::Void RiskMapForm_Load(System::Object^  sender, System::EventArgs^  e) 
		{
			//Form加载后，将主窗口句柄设置到TcpLayer中去
			LayOut();
			InitMap();
			InitMapOverlay();

			//定义grid列
			mHeaders = gcnew array<String^>{"ID","机构名","持仓盈亏",L"初始权益",L"当前权益",L"保证金",L"持仓量"};
			for(Int32 i = 0; i < mHeaders->Length;i++)
			{
				mOrgHZDataTable->Columns->Add(mHeaders[i]);
			}
			dataGridView1->DataSource = mOrgHZDataTable;
			SubscribeStatiacsData();
        }

	
		//重载WndProc，以处理WM_COMMAND消息
	    protected:virtual void WndProc( Message% m ) override
		{
			 
			if(m.Msg == WM_COMMAND_Win && m.WParam.ToInt32()==WndCmd_YourPkgArrival) 
			{
				char *p = (char*)m.LParam.ToInt32();
				Stru_UniPkgHead head;					

				memset(&head, 0, sizeof(head));
				memcpy(&head, p, sizeof(head));
				switch(head.cmdid) 
				{
					case Cmd_RM_SubscribeStatistics_Rsp:
						{
							if(head.userdata1 == OPERATOR_SUCCESS && head.seq == mnSeqID) 
							{
								int count= head.len/sizeof(StatisticsEntry);
								if(count == 0)
									break;
								StatisticsEntry* pStatic = (StatisticsEntry*)(p+sizeof(Stru_UniPkgHead));
								mRiskMapData->LockObject();
								if(pStatic->nFinancialProductID == -1) //汇总数据
								{								
									mRiskMapData->mmapStatics.clear();									
								}
								else
								{									
									mRiskMapData->mmapFinacialProducctStatics.clear();															
								}
								bool lbStatic = false;							
								for(int i = 0; i< count; i++)
								{
									pStatic = (StatisticsEntry*)
										(p+sizeof(Stru_UniPkgHead)+i*sizeof(StatisticsEntry));
									//mRiskMapData->mmapStatics.insert(std::make_pair(pStatic->nAssetMgmtOrgID,*pStatic));	
									if(pStatic->nFinancialProductID == -1) //汇总数据
									{
										lbStatic = true;										
										mRiskMapData->PushStatics(pStatic->nAssetMgmtOrgID,pStatic,mOrgHZDataTable);
									}
									else
									{									
										if(pStatic->nAssetMgmtOrgID == mnCurrentFinOrgID)
										{											
											mRiskMapData->PushFinacialProductStatics(pStatic->nFinancialProductID,pStatic);
										}
									}
								}
								mRiskMapData->UnlockObject();
								if(lbStatic)
									UpdateGridView(mnCurrentOrgParentID,false);
								else
									UpdatePie(mnCurrentFinOrgID);
							}

							break;

						}						
					case Cmd_RM_Statistics_Push:
						{
							if(head.userdata1 == OPERATOR_SUCCESS ) 
							{
								int count= head.len/sizeof(StatisticsEntry);
								if(count == 0)
									break;
								StatisticsEntry* pStatic = (StatisticsEntry*)(p+sizeof(Stru_UniPkgHead));
								mRiskMapData->LockObject();
								if(pStatic->nFinancialProductID == -1) //汇总数据
								{								
									mRiskMapData->mmapStatics.erase(pStatic->nAssetMgmtOrgID);									
								}
								else
								{	
									if(pStatic->nAssetMgmtOrgID == mnCurrentFinOrgID)
										mRiskMapData->mmapFinacialProducctStatics.erase(pStatic->nFinancialProductID);															
								}
								bool lbStatic = false;
							
								for(int i = 0; i< count; i++)
								{
									pStatic = (StatisticsEntry*)
										(p+sizeof(Stru_UniPkgHead)+i*sizeof(StatisticsEntry));
									//mRiskMapData->mmapStatics.insert(std::make_pair(pStatic->nAssetMgmtOrgID,*pStatic));	
									if(pStatic->nFinancialProductID == -1) //汇总数据
									{
										lbStatic = true;										
										mRiskMapData->PushStatics(pStatic->nAssetMgmtOrgID,pStatic,mOrgHZDataTable);
									}
									else
									{									
										if(pStatic->nAssetMgmtOrgID == mnCurrentFinOrgID)
										{											
										    mRiskMapData->PushFinacialProductStatics(pStatic->nFinancialProductID,pStatic);
										}
									}
								}
								mRiskMapData->UnlockObject();
								if(lbStatic)
									UpdateGridView(mnCurrentOrgParentID,false);
								else
									UpdatePie(mnCurrentFinOrgID);
							}

							break;						
						}					
					default:break;
				}
			}

			Form::WndProc ( m );
		 }

			 
	     private: System::Void RiskMapForm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) 
		 {
			// CTcpLayer::Stop();
			 UnsubscribeStatiacsData();
		 }

		 //marker 鼠标点击事件
		  void OnRiskMarkerClick(IMapMarker^ item, MouseEventArgs^ e)
		  {
			  if(e->Button == System::Windows::Forms::MouseButtons::Left)
			  {
				  //如果有下级，则跳到下级maker的zoom级别
				  CSingleOrgnization ^ ltempOrg = mrOrgnization->GetFirstChildOrg(item->MakerID);
				  if(ltempOrg != nullptr)
				  {			
					  mbChickToZoom = true;
					  mnCurrentOrgParentID = item->MakerID;
					  UpdateGridView(mnCurrentOrgParentID,true);					 
					  this->cfMapControl1->Position = item->Position;;	
					  this->cfMapControl1->Zoom = ltempOrg->MakerRef->MinShowZoomLevel; 
					 
				  }				 
			  }
			  if(e->Button == System::Windows::Forms::MouseButtons::Right)
			  {
                 //弹出该组织架构的风险show
				  DepartmentShow ^ ltempShowForm = gcnew DepartmentShow();
				  ltempShowForm->CurrentOrgID = item->MakerID;
				  CSingleOrgnization ^ ltempOrg = mrOrgnization->Find(item->MakerID);
				  if(ltempOrg)
					 ltempShowForm->CurrentOrgName = ltempOrg->Name;
				  ltempShowForm->Show(this);

				
			  }
		  }
		  //地图缩放事件
		  void OnMapZoomChanged()
		  {
			  //如果是点击，则不再响应点击放大地图
			  if(mbChickToZoom != false)
			  {
				  mnOlderZoom = this->cfMapControl1->Zoom;
				  mbChickToZoom = false;
				  return;
			  }
			
			 
			  //查看该次变动是不是引起了地图标记的变化				
			  //Int32 lnBeShowOrgLevel = COrgLevelToMapZoomLevel::IsChangedShowOrg(mrOrgnization->MaxScaleLevel,mnOlderZoom,this->cfMapControl1->Zoom);
			  //if( lnBeShowOrgLevel >= COrgnization::MinDeep && lnBeShowOrgLevel <= COrgnization::MaxDeep)
			  //{
     //             //再往上追溯一级 看看那个级别那个组织机构离中心点近
				 // Int32 lnBeShowOrgId = COrgnization::INVALID_PARENT_ID;
				 // CSingleOrgnization ^ lTempOrg = mrOrgnization->CheckWhoCloserCenter(lnBeShowOrgLevel-1,this->cfMapControl1->Position);
				 // if(lTempOrg != nullptr)
					//  lnBeShowOrgId = lTempOrg->ID;
				 // 
				 // mnCurrentOrgParentID = lnBeShowOrgId;
				 // UpdateGridView(mnCurrentOrgParentID,true);				 
				 //   
			  //}   
			  //mRiskMakerOverlay->
			  if(mRiskMakerOverlay->IsChangedShowMarker(mnOlderZoom,this->cfMapControl1->Zoom))
			  {
				  IMapMarker ^ lnCloserMarker = mRiskMakerOverlay->CheckWhickMarkCloseToCenter(this->cfMapControl1->Zoom,this->cfMapControl1->Position);
				  if(lnCloserMarker != nullptr)
				  {
					 
					  CSingleOrgnization ^ lTempOrg = mrOrgnization->Find( lnCloserMarker->MakerID);
					  if(lTempOrg != nullptr)
					  {
					    if(lTempOrg->ParentOrg)				   
							mnCurrentOrgParentID = lTempOrg->ParentOrg->ID;
						else
							mnCurrentOrgParentID = COrgnization::INVALID_PARENT_ID;
						UpdateGridView(mnCurrentOrgParentID,true);
					  }
				  }

			  }
			  //无论显示是否变化，需更新老的zoom 级别
			  mnOlderZoom = this->cfMapControl1->Zoom;

		  }

		  //更新该组织机构下级的数据
		  void UpdateGridView(Int32 nParentID,bool nbClear)
		  {
			
			  //先全部清除
			  if(nbClear)
			  {
				 // dataGridView1->Rows->Clear();
				  mOrgHZDataTable->Rows->Clear();	
				 // return;
				 // dataGridView1->Columns[mHeaders->Length-1]->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			  }
			  CSingleOrgnization ^ ltempOrg = mrOrgnization->GetFirstChildOrg(nParentID);
			  while(ltempOrg != nullptr)
			  {
				  //汇总数据不用计算，服务器接受到的就是汇总数据
				  mRiskMapData->LockObject();
				  RiskMapStatisticMap::iterator lIteratorStart,lIteratorEnd;
				  lIteratorStart = mRiskMapData->mmapStatics.lower_bound(ltempOrg->ID);
				  lIteratorEnd = mRiskMapData->mmapStatics.upper_bound(ltempOrg->ID);
				 
				  TotalStatiscData ^ lDepartTotalData = gcnew TotalStatiscData();
				  while(lIteratorStart != lIteratorEnd)
				  {
					  lDepartTotalData->Add(lIteratorStart->second.nStatisticsType,lIteratorStart->second.dblValue);				
					  lIteratorStart++;
				  } 
				  mRiskMapData->UnlockObject();

				  //找到对应的列更新
				  array<DataRow^>^  nrDataRow = mOrgHZDataTable->Select("ID='"+ltempOrg->ID+"'");
				  if(nrDataRow && nrDataRow->Length != 0)
				  {						
					  nrDataRow[0][mHeaders[2]] = lDepartTotalData->mdbCCYK.ToString(L"N2");
					  nrDataRow[0][mHeaders[3]] = lDepartTotalData->mdbCCQY.ToString(L"N2");
					  nrDataRow[0][mHeaders[4]] = lDepartTotalData->mdbDQQY.ToString(L"N2");
					  nrDataRow[0][mHeaders[5]]= lDepartTotalData->mdbBZJ.ToString(L"N2");
					  nrDataRow[0][mHeaders[6]] = lDepartTotalData->mdbCCL.ToString(L"N2");
				  }
				  else
				  {
					  //没有找到，则添加
					  DataRow ^ nrDataRow = mOrgHZDataTable->NewRow();					
					  nrDataRow[mHeaders[0]] =  ltempOrg->ID;				
					  nrDataRow[mHeaders[1]] =  ltempOrg->Name;
					  nrDataRow[mHeaders[2]] = lDepartTotalData->mdbCCYK.ToString(L"N2");
					  nrDataRow[mHeaders[3]] = lDepartTotalData->mdbCCQY.ToString(L"N2");
					  nrDataRow[mHeaders[4]] = lDepartTotalData->mdbDQQY.ToString(L"N2");
					  nrDataRow[mHeaders[5]] = lDepartTotalData->mdbBZJ.ToString(L"N2");
					  nrDataRow[mHeaders[6]] = lDepartTotalData->mdbCCL.ToString(L"N2");


					  mOrgHZDataTable->Rows->Add(nrDataRow);
				  }

				
				  ltempOrg = mrOrgnization->GetNextChildOrg(nParentID,ltempOrg);
			  }			
			
			  if(nbClear && mOrgHZDataTable->Rows->Count > 0)
			  {
				  DataGridViewCellEventArgs ^ e = gcnew DataGridViewCellEventArgs(0,0);
				 // e->RowIndex = 0;
				  OnCellClick(nullptr,e);

			  }
		  }
		  //更新该组织机构下级的数据，饼图显示(保证金比例)
		  void UpdatePie(Int32 nOrgID)
		  {
			 
			  if(nOrgID == -1)
				  return;

			  chart1->Series["Default"]->Points->Clear();	
			  //获取该机构相关的理财产品
		
			  chart1->ChartAreas[0]->Area3DStyle->Enable3D = true;
			  chart1->ChartAreas[0]->BackColor = System::Drawing::Color::Transparent;
			  chart1->ChartAreas[0]->BackSecondaryColor = System::Drawing::Color::Transparent;
			  chart1->ChartAreas[0]->BorderColor = System::Drawing::Color::White;
			  if(mrOrgnization->Find(nOrgID) != nullptr)
				chart1->Titles[0]->Text =mrOrgnization->Find(nOrgID)->Name +L"理财产品总权益分布图，单击图例查看理财产品详情";
			  chart1->Tag = nOrgID;
			  std::set<int> mapProductID;
			  m_pdataInfo->GetFinancialProduct(
				  nOrgID, true, mapProductID);
			  if(mapProductID.size()>0)
			  {
				
				  std::set<int>::iterator itIns;
				  MAPFinancialProduct::iterator itInfo;
				  int i = 0;
				  for(itIns = mapProductID.begin(); 
					  itIns != mapProductID.end(); itIns++) 
				  {
						  //System::Diagnostics::Debug::Print("查询到的产品编号 %d\n", *itset);
						  itInfo = mRiskMapData->mmapFinanProduct.find(*itIns);
						  if(itInfo != mRiskMapData->mmapFinanProduct.end())
						  {
							  //计算该产品的盈亏
							   RiskMapStatisticMap::iterator lIteratorStart = mRiskMapData->mmapFinacialProducctStatics.begin();
							   double ldbValue = 1.0;
							   while(lIteratorStart != mRiskMapData->mmapFinacialProducctStatics.end())
							   {
								   if(lIteratorStart->second.nFinancialProductID == itInfo->second.nFinancialProductID && 
									   lIteratorStart->second.nAssetMgmtOrgID == nOrgID && 
									   lIteratorStart->second.nStatisticsType == ST_Balance) //总权益
								   {
									   ldbValue = lIteratorStart->second.dblValue;
									   break;
								   }
								   lIteratorStart ++;
							   }

							  System::Diagnostics::Debug ::WriteLine(Tools::string2String(itInfo->second.szName) + "" + ldbValue);
							  chart1->Series["Default"]->Points->AddXY(Tools::string2String(itInfo->second.szName),ldbValue);		
							  chart1->Series["Default"]->Points[i]->Label = "#PERCENT{P1}";
							  chart1->Series["Default"]->Points[i]->LegendText = Tools::string2String(itInfo->second.szName);
							  chart1->Series["Default"]->Points[i++]->Tag = *itIns;
						  }
				  }
			  }
		  }
		

		  //重新根据客户区大小调整控件位置和大小
		  void LayOut()
		  {
			  
			  //获取屏幕客户区大小，来分布各控件
			  Int32 lnClientWidth = System::Windows::Forms::Screen::PrimaryScreen->WorkingArea.Width;
			  Int32 lnClientHeight = System::Windows::Forms::Screen::PrimaryScreen->WorkingArea.Height;
			  //if(lnClientWidth < 963 || lnClientHeight < 600)
			  //  return
			  //Pie 和 Grid 的宽度 不变  graph的高度不变 map的位置不变 pie 高度不变
		      //
		      //计算分割线位置
			  Int32 lnVerticalCutLine = lnClientWidth - dataGridView1->Size.Width - 5;
			  Int32 lnHorizonalCutLine = lnClientHeight - 25 ;
			  Int32 lnLittleHorizonalCutLine = lnHorizonalCutLine - chart1->Height;
			  //调整map
			  this->cfMapControl1->Size = System::Drawing::Size(lnVerticalCutLine- 5- cfMapControl1->Location.Y,lnHorizonalCutLine -5 - cfMapControl1->Location.Y);
              this->button1->Location = System::Drawing::Point(this->cfMapControl1->Location.X + 85, button1->Location.Y);
			  this->button2->Location = System::Drawing::Point(this->cfMapControl1->Location.X + 5, button2->Location.Y);
			  //调整datagrid
			  dataGridView1->Location = System::Drawing::Point(lnVerticalCutLine, dataGridView1->Location.Y);
			  dataGridView1->Size = System::Drawing::Size(dataGridView1->Size.Width,lnLittleHorizonalCutLine - dataGridView1->Location.Y);

			  //调整pie
			  chart1->Location = System::Drawing::Point(lnVerticalCutLine,lnLittleHorizonalCutLine);

			
		  }


	     
private: System::Void OnPieMouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 // Call Hit Test Method
			 HitTestResult^ result = chart1->HitTest( e->X, e->Y );

			 if(result->PointIndex < 0 || chart1->Series->Count <= 0)
				 return;
			 if(chart1->Series[0]->Points->Count <= 0)
				 return;
			 // Check if data point is already exploded
			 bool exploded = ( chart1->Series[0]->Points[result->PointIndex]->CustomProperties == "Exploded=true" )? true : false ;

			 // Remove all exploded attributes
			 for each( DataPoint^ point in chart1->Series[0]->Points )
			 {
				 point->CustomProperties = "";
			 }

			 // If data point is already exploded get out.
			 if( exploded )
				 return;

			 // If data point is selected
			 if( result->ChartElementType == ChartElementType::DataPoint )
			 {
				 // Set Attribute
				 DataPoint^ point = chart1->Series[0]->Points[result->PointIndex];
				 point->CustomProperties = "Exploded = true";
			 }

			 // If legend item is selected
			 if( result->ChartElementType == ChartElementType::LegendItem )
			 {
				 // Set Attribute
				// DataPoint^ point = chart1->Series[0]->Points[result->PointIndex];
				// point->CustomProperties = "Exploded = true";
				/* LegendItem ^ lItem = (LegendItem^)result->Object;
				 if(lItem == nullptr)
					 return;*/
				 ProductShow ^ lp = gcnew ProductShow();
				 lp->CurrentOrgID = Convert::ToInt32(chart1->Tag);
				 lp->CurrentProductID = Convert::ToInt32(chart1->Series["Default"]->Points[result->PointIndex]->Tag);
				 lp->CurrentProductName = chart1->Series["Default"]->Points[result->PointIndex]->LegendText;
				 lp->Show();
			 }

		 }
private: System::Void OnPieMouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 // Call Hit Test Method
			 HitTestResult^ result = chart1->HitTest( e->X, e->Y );

			 if(result->PointIndex < 0 || chart1->Series->Count <= 0)
				 return;
			 if(chart1->Series[0]->Points->Count <= 0)
				 return;
			 // Reset Data Point Attributes
			 for each( DataPoint^ point in chart1->Series[0]->Points )
			 {
				 point->BackSecondaryColor = Color::Black;
				 point->BackHatchStyle = ChartHatchStyle::None;
				 point->BorderWidth = 1;
			 }

			 // If a Data Point or a Legend item is selected.
			 if (result->ChartElementType == ChartElementType::DataPoint ||
				 result->ChartElementType == ChartElementType::LegendItem )
			 {                
				 // Set cursor type 
				 this->Cursor = Cursors::Hand;

				 // Find selected data point
				 DataPoint^ point = chart1->Series[0]->Points[result->PointIndex];

				 // Set End Gradient Color to White
				 point->BackSecondaryColor = Color::White;

				 // Set selected hatch style
				 point->BackHatchStyle = ChartHatchStyle::Percent25;

				 // Increase border width
				 point->BorderWidth = 2;
			 }
			 else
			 {
				 // Set default cursor
				 this->Cursor = Cursors::Default;
			 }

		 }
private: System::Void OnEarthButtonClick(System::Object^  sender, System::EventArgs^  e) {
			 if(this->cfMapControl1->MapProvider != CFMap::Interface::CCFMapProviders::BingSatelliteMap)
				this->cfMapControl1->MapProvider = CFMap::Interface::CCFMapProviders::BingSatelliteMap;
		 }
private: System::Void OnMapButtonClick(System::Object^  sender, System::EventArgs^  e) {
			 if(this->cfMapControl1->MapProvider != CFMap::Interface::CCFMapProviders::OpenStreetMap)
			 {
				 this->cfMapControl1->MapProvider = CFMap::Interface::CCFMapProviders::OpenStreetMap;
				
			 }
		 }
private: System::Void SubscribeStatiacsData()
		 {
			 //订阅统计
			 IntPtr hWnd=this->Handle;
			 CTcpLayer::SubscribePkg(Cmd_RM_SubscribeStatistics_Rsp,(int)hWnd);
			 CTcpLayer::SubscribePkg(Cmd_RM_Statistics_Push,(int)hWnd);

			 //获取需要订阅的组织和理财产品的关系数据
			
			 TOrgnizationMap ^ lOrgMap = mrOrgnization->OrgMap;
			 if(lOrgMap != nullptr)
			 {
				 TOrgnizationMap::iterator lIte = lOrgMap->begin();
				 for (;lIte != lOrgMap->end();lIte++)
				 {
					 CSingleOrgnization ^ lTempOrg = lIte->second;					
					 AssetOrgProRelation re;
					 re.nAssetMgmtOrgID=lTempOrg->ID;
					 re.nFinancialProductID=-1;
					 mRiskMapData->mStaticsVector.push_back(re);
				 }
			 }
			 if(mRiskMapData->mStaticsVector.size() > 0)
			 {				
				 if( !CTcpLayer::SendData(Cmd_RM_SubscribeStatistics_Req, &mRiskMapData->mStaticsVector[0],sizeof(AssetOrgProRelation)*mRiskMapData->mStaticsVector.size(), mnSeqID))
				 {				
					// Windows::Forms::MessageBox::Show("订阅汇总数据失败", "提示", Windows::Forms::MessageBoxIcon::Error);
					 MessageBox::Show("订阅汇总数据失败", "提示",MessageBoxButtons::OK);
				 }
			 }

		 }

private: System::Void SubscribeFinicalProductStatiacsData(Int32 lnOrgID)
		 {
			 //订阅统计
			 IntPtr hWnd=this->Handle;
			 CTcpLayer::SubscribePkg(Cmd_RM_SubscribeStatistics_Rsp,(int)hWnd);
			 CTcpLayer::SubscribePkg(Cmd_RM_Statistics_Push,(int)hWnd);

			 //获取需要订阅的组织和理财产品的关系数据
			 mRiskMapData->mStaticsFinacalProductVector.clear();

			 std::set<int> mapProductID;
			 m_pdataInfo->GetFinancialProduct(
				 lnOrgID, true, mapProductID);
			 if(mapProductID.size()>0)
			 {
				 std::set<int>::iterator itIns;
				 MAPFinancialProduct::iterator itInfo;
				 int i = 0;
				 for(itIns = mapProductID.begin(); itIns != mapProductID.end(); itIns++) 
				 {				
						 AssetOrgProRelation re;
						 re.nAssetMgmtOrgID = lnOrgID;
						 re.nFinancialProductID = *itIns;
						 mRiskMapData->mStaticsFinacalProductVector.push_back(re);
					
				 }
			 }
			 if(mRiskMapData->mStaticsFinacalProductVector.size() > 0)
			 {				
				 if( !CTcpLayer::SendData(Cmd_RM_SubscribeStatistics_Req, &mRiskMapData->mStaticsFinacalProductVector[0],sizeof(AssetOrgProRelation)*mRiskMapData->mStaticsFinacalProductVector.size(), mnSeqID))
				 {				
					 // Windows::Forms::MessageBox::Show("订阅汇总数据失败", "提示", Windows::Forms::MessageBoxIcon::Error);
					 MessageBox::Show("订阅汇总数据失败", "提示",MessageBoxButtons::OK);
				 }
			 }

		 }
private: System::Void UnsubscribeStatiacsData()
		 {
			 //获取需要订阅的组织和理财产品的关系数据
			 
			 if(mRiskMapData->mStaticsVector.size() > 0)
			 {				
				 if( !CTcpLayer::SendData(Cmd_RM_UnSubscribeStatistics_Req, &mRiskMapData->mStaticsVector[0],sizeof(AssetOrgProRelation)*mRiskMapData->mStaticsVector.size(), 0))
				 {					
					 MessageBox::Show("退订汇总数据失败", "提示",MessageBoxButtons::OK);
				 }
			 }
		 }
private: System::Void UnsubscribeFinicalProductStatiacsData()
		 {
			 //获取需要订阅的组织和理财产品的关系数据

			 if(mRiskMapData->mStaticsFinacalProductVector.size() > 0)
			 {				
				 if( !CTcpLayer::SendData(Cmd_RM_UnSubscribeStatistics_Req, &mRiskMapData->mStaticsFinacalProductVector[0],sizeof(AssetOrgProRelation)*mRiskMapData->mStaticsFinacalProductVector.size(), 0))
				 {					
					 MessageBox::Show("退订汇总数据失败", "提示",MessageBoxButtons::OK);
				 }
			 }
		 }
private: System::Void OnCellClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) {
             //判断选择的是不是和当前的一样
			 //退订原来的
			 //订阅新的组织架构和理财产品
			 int rowindex = e->RowIndex; 
			 if(rowindex == -1)
				 return;
			 Int32 lnOrgId =  Convert::ToInt32(mOrgHZDataTable->Rows[rowindex][mHeaders[0]]);   
			 if(mRiskMapData->mStaticsFinacalProductVector.size() > 0 &&
				 mRiskMapData->mStaticsFinacalProductVector[0].nAssetMgmtOrgID == lnOrgId)
				 return;		

			 UnsubscribeFinicalProductStatiacsData();

			 //重新订阅
			 mnCurrentFinOrgID = lnOrgId;
			 SubscribeFinicalProductStatiacsData(lnOrgId);

		 }
public: System::Void CombinationScreenSetting(System::Drawing::Point  nLocation,System::Drawing::Size  nSize)
		{
			this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
			this->WindowState = System::Windows::Forms::FormWindowState::Normal; 
			this->Location = nLocation;
			this->Size = nSize;			


		}
};
}

