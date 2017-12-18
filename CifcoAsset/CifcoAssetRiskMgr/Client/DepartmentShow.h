#pragma once
#include "DepartProductItem.h"
#include "DepartProductTypeItem.h"
#include "RiskMapData.h"
#include "CDataInfo.h"
#include "Tools.h"
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
#include <cliext/map>

namespace Client {

	/// <summary>
	/// Summary for DepartmentShow
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class DepartmentShow : public System::Windows::Forms::Form
	{
	public:
		DepartmentShow(void)
		{
			InitializeComponent();
			m_pdataInfo = CDataInfo::NewInstance();
			mRiskMapData = CRiskMapData::NewInstance();
			mMapProductItemUI = gcnew cliext::multimap<int,DepartProductItem^>();
			mMapProductTypeItemUI = gcnew cliext::multimap<String^,DepartProductTypeItem^>();
			Random^ rand = gcnew Random(Guid::NewGuid().GetHashCode());
			mnSeqID = rand->Next(1000000);
			mbCombinationScreen = false;
			//
			//TODO: Add the constructor code here
			//
		}
	private: bool mbCombinationScreen;
	private: System::Drawing::Point mbCombinationScreenPoint;
 	private: System::Drawing::Size  mbCombinationScreenSize;
	private: Int32 mnSeqID;
    private: CDataInfo* m_pdataInfo;
	private: CRiskMapData * mRiskMapData;
	private: Int32 mnCurrentOrgID;
	private: String^ msCurrentOrgName;
	private: cliext::multimap<int,DepartProductItem^> ^ mMapProductItemUI;//理财产品UI MAP
	private: System::Windows::Forms::Panel^  panel6;
	private: System::Windows::Forms::Label^  label2;
	private: cliext::multimap<String^,DepartProductTypeItem^> ^ mMapProductTypeItemUI;//品种UI MAP
	public: property Int32 CurrentOrgID
    {
	   Int32 get() {return mnCurrentOrgID;}
	   void set(Int32 mnID) {mnCurrentOrgID = mnID;}
    }
	public: property String^ CurrentOrgName
	{
		String^ get() {return msCurrentOrgName;}
		void set(String^ nsVal) {msCurrentOrgName = nsVal;}
				
	}
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~DepartmentShow()
		{
			if(mRiskMapData)
				CRiskMapData::ReleaseInstance(mRiskMapData);
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::PictureBox^  pictureBox2;
    private: System::Windows::Forms::PictureBox^  pictureBox3;

	protected: 
	private: System::Windows::Forms::DataVisualization::Charting::Chart^  chart1;
	private: System::Windows::Forms::Panel^  panel2;
	private: ::SimpleGuage::SimpleGuage^  simpleGuage1;
	private: ListPanel::ListPanelControl^  listPanelControl1;
	private: ListPanel::ListPanelControl^  listPanelControl2;

	private: System::Windows::Forms::Panel^  panel3;
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Panel^  panel4;
	private: System::Windows::Forms::Panel^  panel5;
	private: System::Windows::Forms::PictureBox^  pictureBox4;

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
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(DepartmentShow::typeid));
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea1 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Series^  series1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->chart1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->listPanelControl1 = (gcnew ListPanel::ListPanelControl());
			this->listPanelControl2 = (gcnew ListPanel::ListPanelControl());
			this->panel3 = (gcnew System::Windows::Forms::Panel());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->simpleGuage1 = (gcnew ::SimpleGuage::SimpleGuage());
			this->panel4 = (gcnew System::Windows::Forms::Panel());
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox3 = (gcnew System::Windows::Forms::PictureBox());
			this->panel5 = (gcnew System::Windows::Forms::Panel());
			this->pictureBox4 = (gcnew System::Windows::Forms::PictureBox());
			this->panel6 = (gcnew System::Windows::Forms::Panel());
			this->label2 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->chart1))->BeginInit();
			this->panel1->SuspendLayout();
			this->panel4->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox4))->BeginInit();
			this->panel6->SuspendLayout();
			this->SuspendLayout();
			// 
			// pictureBox1
			// 
			this->pictureBox1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox1.BackgroundImage")));
			this->pictureBox1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->pictureBox1->Location = System::Drawing::Point(0, 3);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(79, 74);
			this->pictureBox1->TabIndex = 6;
			this->pictureBox1->TabStop = false;
			// 
			// chart1
			// 
			this->chart1->BackColor = System::Drawing::Color::Transparent;
			chartArea1->AxisX->LabelStyle->ForeColor = System::Drawing::Color::White;
			chartArea1->AxisX->LineColor = System::Drawing::Color::White;
			chartArea1->AxisX->MajorGrid->LineWidth = 0;
			chartArea1->AxisY->LabelStyle->ForeColor = System::Drawing::Color::White;
			chartArea1->AxisY->LineColor = System::Drawing::Color::White;
			chartArea1->AxisY->MajorGrid->LineWidth = 0;
			chartArea1->BackColor = System::Drawing::Color::Transparent;
			chartArea1->BackSecondaryColor = System::Drawing::Color::White;
			chartArea1->BorderColor = System::Drawing::Color::Red;
			chartArea1->BorderWidth = 0;
			chartArea1->Name = L"ChartArea1";
			this->chart1->ChartAreas->Add(chartArea1);
			this->chart1->Location = System::Drawing::Point(636, 541);
			this->chart1->Name = L"chart1";
			series1->BackGradientStyle = System::Windows::Forms::DataVisualization::Charting::GradientStyle::TopBottom;
			series1->BorderColor = System::Drawing::Color::Transparent;
			series1->ChartArea = L"ChartArea1";
			series1->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(255)), 
				static_cast<System::Int32>(static_cast<System::Byte>(128)));
			series1->IsValueShownAsLabel = true;
			series1->LabelBackColor = System::Drawing::Color::White;
			series1->LabelForeColor = System::Drawing::Color::Red;
			series1->Name = L"Series1";
			series1->SmartLabelStyle->CalloutLineColor = System::Drawing::Color::White;
			this->chart1->Series->Add(series1);
			this->chart1->Size = System::Drawing::Size(455, 214);
			this->chart1->TabIndex = 9;
			this->chart1->Text = L"chart1";
			// 
			// panel2
			// 
			this->panel2->BackColor = System::Drawing::Color::Transparent;
			this->panel2->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"panel2.BackgroundImage")));
			this->panel2->Location = System::Drawing::Point(330, 175);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(68, 491);
			this->panel2->TabIndex = 13;
			// 
			// listPanelControl1
			// 
			this->listPanelControl1->AutoScroll = true;
			this->listPanelControl1->BackColor = System::Drawing::Color::Transparent;
			this->listPanelControl1->FlowDirection = System::Windows::Forms::FlowDirection::TopDown;
			this->listPanelControl1->Location = System::Drawing::Point(636, 227);
			this->listPanelControl1->Name = L"listPanelControl1";
			this->listPanelControl1->Size = System::Drawing::Size(242, 239);
			this->listPanelControl1->TabIndex = 11;
			this->listPanelControl1->Vertical = true;
			this->listPanelControl1->WrapContents = false;
			// 
			// listPanelControl2
			// 
			this->listPanelControl2->AutoScroll = true;
			this->listPanelControl2->BackColor = System::Drawing::Color::Transparent;
			this->listPanelControl2->FlowDirection = System::Windows::Forms::FlowDirection::TopDown;
			this->listPanelControl2->Location = System::Drawing::Point(884, 227);
			this->listPanelControl2->Name = L"listPanelControl2";
			this->listPanelControl2->Size = System::Drawing::Size(296, 239);
			this->listPanelControl2->TabIndex = 12;
			this->listPanelControl2->Vertical = true;
			this->listPanelControl2->WrapContents = false;
			// 
			// panel3
			// 
			this->panel3->BackColor = System::Drawing::Color::Transparent;
			this->panel3->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"panel3.BackgroundImage")));
			this->panel3->Location = System::Drawing::Point(1201, 227);
			this->panel3->Name = L"panel3";
			this->panel3->Size = System::Drawing::Size(61, 366);
			this->panel3->TabIndex = 14;
			// 
			// panel1
			// 
			this->panel1->BackColor = System::Drawing::Color::Transparent;
			this->panel1->Controls->Add(this->label1);
			this->panel1->Controls->Add(this->pictureBox1);
			this->panel1->Controls->Add(this->simpleGuage1);
			this->panel1->Location = System::Drawing::Point(3, 172);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(321, 349);
			this->panel1->TabIndex = 10;
			// 
			// label1
			// 
			this->label1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label1->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->label1->Font = (gcnew System::Drawing::Font(L"SimSun", 36, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->label1->ForeColor = System::Drawing::Color::DarkGoldenrod;
			this->label1->Location = System::Drawing::Point(3, 284);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(312, 58);
			this->label1->TabIndex = 7;
			this->label1->Text = L"总风险度";
			this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// simpleGuage1
			// 
			this->simpleGuage1->BackColor = System::Drawing::Color::Transparent;
			this->simpleGuage1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"simpleGuage1.BackgroundImage")));
			this->simpleGuage1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->simpleGuage1->BigArcCount = 300;
			this->simpleGuage1->BigArcStart = 120;
			this->simpleGuage1->BigNeedleRadius = 95;
			this->simpleGuage1->Location = System::Drawing::Point(66, 63);
			this->simpleGuage1->Name = L"simpleGuage1";
			this->simpleGuage1->Size = System::Drawing::Size(202, 202);
			this->simpleGuage1->SmallArcCount = 270;
			this->simpleGuage1->SmallArcStart = 120;
			this->simpleGuage1->SmallNeedleRadius = 58;
			this->simpleGuage1->TabIndex = 5;
			this->simpleGuage1->Value = 0;
			// 
			// panel4
			// 
			this->panel4->BackColor = System::Drawing::Color::Transparent;
			this->panel4->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"panel4.BackgroundImage")));
			this->panel4->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->panel4->Controls->Add(this->pictureBox2);
			this->panel4->Location = System::Drawing::Point(3, 3);
			this->panel4->Name = L"panel4";
			this->panel4->Size = System::Drawing::Size(162, 136);
			this->panel4->TabIndex = 15;
			// 
			// pictureBox2
			// 
			this->pictureBox2->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox2.BackgroundImage")));
			this->pictureBox2->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->pictureBox2->Location = System::Drawing::Point(35, 68);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(124, 65);
			this->pictureBox2->TabIndex = 0;
			this->pictureBox2->TabStop = false;
			// 
			// pictureBox3
			// 
			this->pictureBox3->BackColor = System::Drawing::Color::Transparent;
			this->pictureBox3->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox3.BackgroundImage")));
			this->pictureBox3->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->pictureBox3->Location = System::Drawing::Point(171, 74);
			this->pictureBox3->Name = L"pictureBox3";
			this->pictureBox3->Size = System::Drawing::Size(119, 61);
			this->pictureBox3->TabIndex = 16;
			this->pictureBox3->TabStop = false;
			// 
			// panel5
			// 
			this->panel5->BackColor = System::Drawing::Color::Black;
			this->panel5->Location = System::Drawing::Point(3, 141);
			this->panel5->Name = L"panel5";
			this->panel5->Size = System::Drawing::Size(1269, 25);
			this->panel5->TabIndex = 18;
			// 
			// pictureBox4
			// 
			this->pictureBox4->BackColor = System::Drawing::Color::Transparent;
			this->pictureBox4->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox4.BackgroundImage")));
			this->pictureBox4->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->pictureBox4->Location = System::Drawing::Point(966, 3);
			this->pictureBox4->Name = L"pictureBox4";
			this->pictureBox4->Size = System::Drawing::Size(306, 136);
			this->pictureBox4->TabIndex = 19;
			this->pictureBox4->TabStop = false;
			this->pictureBox4->Click += gcnew System::EventHandler(this, &DepartmentShow::pictureBox4_Click);
			// 
			// panel6
			// 
			this->panel6->BackColor = System::Drawing::Color::Transparent;
			this->panel6->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"panel6.BackgroundImage")));
			this->panel6->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->panel6->Controls->Add(this->label2);
			this->panel6->Location = System::Drawing::Point(3, 540);
			this->panel6->Name = L"panel6";
			this->panel6->Size = System::Drawing::Size(321, 260);
			this->panel6->TabIndex = 20;
			// 
			// label2
			// 
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei", 21.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->label2->ForeColor = System::Drawing::Color::DarkOrange;
			this->label2->Location = System::Drawing::Point(9, 72);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(295, 54);
			this->label2->TabIndex = 0;
			this->label2->Text = L"北京";
			this->label2->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// DepartmentShow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"$this.BackgroundImage")));
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->ClientSize = System::Drawing::Size(1440, 868);
			this->Controls->Add(this->pictureBox4);
			this->Controls->Add(this->panel5);
			this->Controls->Add(this->pictureBox3);
			this->Controls->Add(this->panel4);
			this->Controls->Add(this->chart1);
			this->Controls->Add(this->panel2);
			this->Controls->Add(this->listPanelControl1);
			this->Controls->Add(this->listPanelControl2);
			this->Controls->Add(this->panel3);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->panel6);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Name = L"DepartmentShow";
			this->Text = L"资管部门汇总展示";
			this->Load += gcnew System::EventHandler(this, &DepartmentShow::OnLoad);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &DepartmentShow::OnFormClosed);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->chart1))->EndInit();
			this->panel1->ResumeLayout(false);
			this->panel4->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox3))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox4))->EndInit();
			this->panel6->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void OnLoad(System::Object^  sender, System::EventArgs^  e) {
				 LayOut();
				 this->panel1->BackColor = Color::FromArgb(125,168,54,167);
				 this->listPanelControl1->BackColor = Color::FromArgb(125,43,166,30);
				 this->listPanelControl2->BackColor = Color::FromArgb(125,196,145,47);
				 this->listPanelControl2->Vertical = false;
				 this->pictureBox1->BackColor = Color::Transparent;
				 if(msCurrentOrgName != nullptr)
					 this->label2->Text = msCurrentOrgName;
				 //this->SuspendLayout();
				 //this->listPanelControl1->SuspendLayout();
				 //创建理财产品界面
				 m_pdataInfo->GetFinancialProduct(mRiskMapData->mmapFinanProduct);
				 std::set<int> mapProductID;
				 m_pdataInfo->GetFinancialProduct( mnCurrentOrgID, true, mapProductID);
				 if(mapProductID.size()>0)
				 {
					 std::set<int>::iterator itIns;
					 MAPFinancialProduct::iterator itInfo;
					 int i = 0;
					 for(itIns = mapProductID.begin(); itIns != mapProductID.end(); itIns++) 
					 {		
						 //get product name 
						 itInfo = mRiskMapData->mmapFinanProduct.find(*itIns);
						 if(itInfo != mRiskMapData->mmapFinanProduct.end())
						 {
							 DepartProductItem ^ lItem = gcnew DepartProductItem();
							 lItem->ProductName = Tools::string2String(itInfo->second.szName);
							 mMapProductItemUI->insert(cliext::make_pair(*itIns,lItem));
							 this->listPanelControl1->Controls->Add(lItem);
						 }
					
					 }
				 }
				/* for(int i = 0 ; i < 4; i++)
				 {
					
					 DepartProductItem ^ lItem = gcnew DepartProductItem();
					 this->listPanelControl1->Controls->Add(lItem);
					 
				 }*/
				
				/* this->listPanelControl2->SuspendLayout();
				 for(int i = 0 ; i < 4; i++)
				 {					 
					 DepartProductTypeItem ^ lItem = gcnew DepartProductTypeItem();
					 this->listPanelControl2->Controls->Add(lItem);					 
				 }
                 this->listPanelControl2->ResumeLayout(false);*/
				 //this->ResumeLayout();

				 //订阅数据
				 SubscribeStatiacsData();
				 SubscribeFinicalProductStatiacsData(mnCurrentOrgID);
			 }

	private: System::Void LayOut() {
				  //排版
				  this->SuspendLayout();
				 
				  Int32 lnMainXSplit = this->panel1->Location.X + this->panel1->Size.Width;
				  Int32 lnFirstYSplit = this->panel1->Location.Y ;

				  Int32 lnLeftX = this->Width - lnMainXSplit;
				  Int32 lnLeftY = this->Height - lnFirstYSplit;

				  System::Drawing::Size  lnProductPanelSize = System::Drawing::Size(476,350) ;
				  System::Drawing::Size  lnProductTypePanelSize = System::Drawing::Size(517,350) ;
				  System::Drawing::Size  lnCharSize = System::Drawing::Size(lnProductPanelSize.Width+lnProductTypePanelSize.Width + 2,this->ClientRectangle.Height - lnFirstYSplit - lnProductPanelSize.Height - 10) ;
				  Int32 lnOtherPaneWidth = (lnLeftX - lnProductPanelSize.Width - lnProductTypePanelSize.Width -10) / 2;
				 
				  this->panel5->Size = System::Drawing::Size(this->Width,this->panel5->Size.Height);
				  this->pictureBox4->Location = Point(this->ClientRectangle.Width - this->pictureBox4->Size.Width,this->pictureBox4->Location.Y);
				  if(lnOtherPaneWidth < 0)
				  {
					  this->panel2->Hide();
					  this->panel3->Hide();
					  this->listPanelControl1->Location = Point(lnMainXSplit+2,lnFirstYSplit);
					  this->listPanelControl1->Size = lnProductPanelSize;
					  this->listPanelControl2->Location = Point(this->listPanelControl1->Location.X + this->listPanelControl1->Size.Width +2,lnFirstYSplit);
					  this->listPanelControl2->Size = lnProductTypePanelSize;
					  this->chart1->Location = Point(lnMainXSplit+2,this->listPanelControl1->Location.Y + this->listPanelControl1->Size.Height +2);
					  this->chart1->Size = lnCharSize;
				  }
				  else
				  {
					  this->panel2->Show();
					  this->panel3->Show();
					  //如果两个修饰的Pane的宽度大于20 则等于20
					  if(lnOtherPaneWidth > 50)
						  lnOtherPaneWidth = 50;

					  this->panel2->Location = Point(lnMainXSplit+2,lnFirstYSplit);
					  this->panel2->Size = System::Drawing::Size(lnOtherPaneWidth,lnLeftY );

					  this->listPanelControl1->Location = Point(this->panel2->Location.X + this->panel2->Size.Width +2,lnFirstYSplit);
					  this->listPanelControl1->Size = lnProductPanelSize;

					  this->listPanelControl2->Location = Point(this->listPanelControl1->Location.X + this->listPanelControl1->Size.Width +2,lnFirstYSplit);
					 // this->listPanelControl2->Size = lnProductTypePanelSize; 
					  this->listPanelControl2->Size =  System::Drawing::Size(this->ClientRectangle.Width - this->listPanelControl2->Location.X - lnOtherPaneWidth - 2,lnProductTypePanelSize.Height) ;//X自动放大 

					  this->panel3->Location = Point(this->listPanelControl2->Location.X + this->listPanelControl2->Size.Width +2,lnFirstYSplit);
					  this->panel3->Size = System::Drawing::Size(lnOtherPaneWidth,lnLeftY );
					 
				

				      this->chart1->Location = Point(this->listPanelControl1->Location.X,this->listPanelControl1->Location.Y + this->listPanelControl1->Size.Height +2);
					  lnCharSize = System::Drawing::Size(this->listPanelControl1->Size.Width+this->listPanelControl2->Size.Width + 2,this->ClientRectangle.Height - this->chart1->Location.Y -2) ;//Y自动放大
					  this->chart1->Size = lnCharSize;
				  }
				  this->ResumeLayout(false);

			  }
private: System::Void SubscribeStatiacsData()
		  {
			  //订阅统计
			  IntPtr hWnd=this->Handle;
			  CTcpLayer::SubscribePkg(Cmd_RM_SubscribeStatistics_Rsp,(int)hWnd);
			  CTcpLayer::SubscribePkg(Cmd_RM_Statistics_Push,(int)hWnd);

			  //获取需要订阅的组织和理财产品的关系数据

			  AssetOrgProRelation re;
			  re.nAssetMgmtOrgID = mnCurrentOrgID;
			  re.nFinancialProductID=-1;
			  mRiskMapData->mStaticsVector.push_back(re);
				
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
			 m_pdataInfo->GetFinancialProduct( lnOrgID, true, mapProductID);
			 if(mapProductID.size()>0)
			 {
				 std::set<int>::iterator itIns;
				 MAPFinancialProduct::iterator itInfo;
				 int i = 0;
				 for(itIns = mapProductID.begin(); itIns != mapProductID.end(); itIns++) 
				 {				
					 itInfo = mRiskMapData->mmapFinanProduct.find(*itIns);
					 if(itInfo != mRiskMapData->mmapFinanProduct.end())
					 {
						 AssetOrgProRelation re;
						 re.nAssetMgmtOrgID = lnOrgID;
						 re.nFinancialProductID = itInfo->second.nFinancialProductID;
						 mRiskMapData->mStaticsFinacalProductVector.push_back(re);
					 }
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
				 if( !CTcpLayer::SendData(Cmd_RM_UnSubscribeStatistics_Req, &mRiskMapData->mStaticsVector[0],sizeof(AssetOrgProRelation)*mRiskMapData->mStaticsVector.size(), mnSeqID))
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
				 if( !CTcpLayer::SendData(Cmd_RM_UnSubscribeStatistics_Req, &mRiskMapData->mStaticsFinacalProductVector[0],sizeof(AssetOrgProRelation)*mRiskMapData->mStaticsFinacalProductVector.size(), mnSeqID))
				 {					
					 MessageBox::Show("退订汇总数据失败", "提示",MessageBoxButtons::OK);
				 }
			 }
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

							   StatisticsEntry* pStatic = (StatisticsEntry*)(p+sizeof(Stru_UniPkgHead));;
							   if(pStatic->nAssetMgmtOrgID != mnCurrentOrgID)//过滤组织机构
								   break;
							   mRiskMapData->LockObject();
							 
							   if(pStatic->nFinancialProductID == -1) //汇总数据
							   {
								   //由于汇总数据请求只有一项，所以可以简单调用clear来清除结果
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
								   //filter data not the current org id
								   if(pStatic->nAssetMgmtOrgID != mnCurrentOrgID)
									   continue;
								   //mRiskMapData->mmapStatics.insert(std::make_pair(pStatic->nAssetMgmtOrgID,*pStatic));	
								   if(pStatic->nFinancialProductID == -1) //汇总数据
								   {
									   lbStatic = true;
									   mRiskMapData->PushStatics(pStatic->nAssetMgmtOrgID,pStatic,nullptr);
								   }
								   else
								   {									
									   mRiskMapData->PushFinacialProductStatics(pStatic->nFinancialProductID,pStatic);
								   }
							   }
							   mRiskMapData->UnlockObject();
							   if(lbStatic)
								   UpdateHuiZong();
							   else
								   UpdateOtherPane();
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

							  StatisticsEntry* pStatic = (StatisticsEntry*)(p+sizeof(Stru_UniPkgHead));;
							  if(pStatic->nAssetMgmtOrgID != mnCurrentOrgID)//过滤组织机构
								  break;
							  mRiskMapData->LockObject();
							 
							  if(pStatic->nFinancialProductID == -1) //汇总数据
							  {								
								  mRiskMapData->mmapStatics.erase(pStatic->nAssetMgmtOrgID);								 
							  }
							  else
							  {								 
								  mRiskMapData->mmapFinacialProducctStatics.erase(pStatic->nFinancialProductID);								 						
							  }
							  bool lbStatic = false;							 
							  for(int i = 0; i< count; i++)
							  {
								  pStatic = (StatisticsEntry*)
									  (p+sizeof(Stru_UniPkgHead)+i*sizeof(StatisticsEntry));
								  //filter data not the current org id
								  if(pStatic->nAssetMgmtOrgID != mnCurrentOrgID)
									  continue;
								  //mRiskMapData->mmapStatics.insert(std::make_pair(pStatic->nAssetMgmtOrgID,*pStatic));	
								  if(pStatic->nFinancialProductID == -1) //汇总数据
								  {
									  lbStatic = true;
									  mRiskMapData->PushStatics(pStatic->nAssetMgmtOrgID,pStatic,nullptr);
								  }
								  else
								  {									
									  mRiskMapData->PushFinacialProductStatics(pStatic->nFinancialProductID,pStatic);
								  }
							  }
							  mRiskMapData->UnlockObject();
							  if(lbStatic)
								  UpdateHuiZong();
							  else
								  UpdateOtherPane();


						  }

						  break;
					  }
				  //case Cmd_RM_Statistics_Push:
					 // {
						//  if(head.userdata1 == OPERATOR_SUCCESS ) 
						//  {
						//	  int count= head.len/sizeof(StatisticsEntry);
						//	  if(count == 0)
						//		  break;
						//	 
						//	  StatisticsEntry* pStatic = (StatisticsEntry*)(p+sizeof(Stru_UniPkgHead));;
						//	 
						//	  mRiskMapData->LockObject();
						//	  if(pStatic->nAssetMgmtOrgID != mnCurrentOrgID) //过滤组织机构
						//	  {
						//		  mRiskMapData->UnlockObject();
						//		  break;
						//	  }
						//	
						//	  bool lbStatic = false;							 
						//	  for(int i = 0; i< count; i++)
						//	  {
						//		  pStatic = (StatisticsEntry*)
						//			  (p+sizeof(Stru_UniPkgHead)+i*sizeof(StatisticsEntry));
						//		  //filter data not the current org id
						//		  if(pStatic->nAssetMgmtOrgID != mnCurrentOrgID)
						//			  continue;
						//		  //mRiskMapData->mmapStatics.insert(std::make_pair(pStatic->nAssetMgmtOrgID,*pStatic));	
						//		  if(pStatic->nFinancialProductID == -1) //汇总数据
						//		  {
						//			  lbStatic = true;
						//			  mRiskMapData->mmapStatics.erase(pStatic->nAssetMgmtOrgID);
						//			  mRiskMapData->PushStatics(pStatic->nAssetMgmtOrgID,pStatic,nullptr);
						//		  }
						//		  else
						//		  {		
						//			  mRiskMapData->mmapFinacialProducctStatics.erase(pStatic->nFinancialProductID);//删除原有的
						//			  mRiskMapData->PushFinacialProductStatics(pStatic->nFinancialProductID,pStatic);
						//		  }
						//	  }
						//	  mRiskMapData->UnlockObject();
						//	  if(lbStatic)
						//		  UpdateHuiZong();
						//	  else
						//		  UpdateOtherPane();

						//	
						//  }

						//  break;
					 // }
				  //case Cmd_RM_Statistics_Push:
					 // {
						//  //新推一个变化过来，需要判断是新增的还是原来就有这个值
						//  //根据新推的修改datatabel的变化
						//  bool lbStatic = false;						 
						//  if(head.userdata1 == OPERATOR_SUCCESS && head.len == sizeof(StatisticsEntry))
						//  {
						//	  StatisticsEntry* pStatic = (StatisticsEntry*)(p+sizeof(Stru_UniPkgHead));
						//	  if(pStatic->nAssetMgmtOrgID != mnCurrentOrgID)
						//		  break;
						//	  mRiskMapData->LockObject();
						//	  if(pStatic->nFinancialProductID == -1) //汇总数据
						//	  {
						//		  lbStatic = true;
						//		  mRiskMapData->PushStatics(pStatic->nAssetMgmtOrgID,pStatic,nullptr);
						//	  }
						//	  else
						//	  {								 
						//		  mRiskMapData->PushFinacialProductStatics(pStatic->nFinancialProductID,pStatic);
						//	  }

						//	  mRiskMapData->UnlockObject();
						//	  if(lbStatic)
						//		  UpdateHuiZong();
						//	  else
						//		  UpdateOtherPane();
						//	 
						//  }


						//  break;
					 // }
				  default:break;
				  }
		    }

			Form::WndProc ( m );
		  }

private: System::Void OnFormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
			 UnsubscribeFinicalProductStatiacsData();
			 UnsubscribeStatiacsData();
		 }
private: System::Void UpdateHuiZong() {
			 //更新百分比 部门的保证金/部门的总权益
			 Single lfPercent = 0.0f;			
			 if(mRiskMapData->mmapStatics.size() > 0)
			 {
				 TotalStatiscData ^ lDepartTotalData = gcnew TotalStatiscData();
				 if(mRiskMapData->mmapStatics.size() > 0)
				 {
					 RiskMapStatisticMap::iterator lIte = mRiskMapData->mmapStatics.begin();
					 while(lIte != mRiskMapData->mmapStatics.end())
					 {
						 lDepartTotalData->Add(lIte->second.nStatisticsType,lIte->second.dblValue);
						 lIte++;
					 }
				 }
				
				 lfPercent = (Single)(TotalStatiscData::SafeDivMulti100(lDepartTotalData->mdbBZJ ,lDepartTotalData->mdbDQQY));
				 this->simpleGuage1->Value = lfPercent;
			 }		 

		 }
private: System::Void UpdateOtherPane() {
			 //更新理财产品页面，品种页面，图表页面
             
			 if(mRiskMapData->mmapStatics.size() == 0 || mRiskMapData->mmapFinacialProducctStatics.size() == 0)
				 return;
			 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
             //先获取部门下的初始值
			 TotalStatiscData ^ lDepartTotalData = gcnew TotalStatiscData();
			 if(mRiskMapData->mmapStatics.size() > 0)
			 {
				 RiskMapStatisticMap::iterator lIte = mRiskMapData->mmapStatics.begin();
				 while(lIte != mRiskMapData->mmapStatics.end())
				 {
					 lDepartTotalData->Add(lIte->second.nStatisticsType,lIte->second.dblValue);
					 lIte++;
				 }
			 }

			 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////			
			 //更新理财产品相关页面
			 cliext::multimap<int,DepartProductItem^> ::iterator lUIIter = mMapProductItemUI->begin();
			 while(lUIIter != mMapProductItemUI->end())
			 {
				//统计单项理财产品的数据
				 mRiskMapData->LockObject();
				 RiskMapStatisticMap::iterator lIteratorStart,lIteratorEnd;
				 int lnFID = lUIIter->first;
				 lIteratorStart = mRiskMapData->mmapFinacialProducctStatics.lower_bound(lnFID);
				 lIteratorEnd = mRiskMapData->mmapFinacialProducctStatics.upper_bound(lnFID);
				 TotalStatiscData ^ lProductTotalData = gcnew TotalStatiscData();
				 while(lIteratorStart != lIteratorEnd && lIteratorStart != mRiskMapData->mmapFinacialProducctStatics.end())
				 {					
					 lProductTotalData->Add(lIteratorStart->second.nStatisticsType,lIteratorStart->second.dblValue);
					 lIteratorStart++;
				 }
				  mRiskMapData->UnlockObject();
				 //update UI
				 DepartProductItem ^ lTemp = lUIIter->second;
				 if(lTemp)
				 {
					 lTemp->TotalQuanYi = ((Int32)(TotalStatiscData::SafeDivMulti100(lProductTotalData->mdbDQQY , lProductTotalData->mdbCCQY))).ToString() + L"%";
					 lTemp->TotalFengXian = ((Int32)(TotalStatiscData::SafeDivMulti100(lProductTotalData->mdbBZJ , lProductTotalData->mdbDQQY))).ToString() + L"%";
					 lTemp->FirstQuanYi = (Int32)(TotalStatiscData::SafeDivMulti100(lProductTotalData->mdbCCQY ,lDepartTotalData->mdbCCQY));
					// lTemp->HodeYinKui = (Int32)(TotalStatiscData::SafeDivMulti100(lProductTotalData->mdbCCYK , lDepartTotalData->mdbCCYK));
					 lTemp->YingKui = lProductTotalData->mdbDQQY - lProductTotalData->mdbCCQY;
				 }

				 lUIIter++;
			 }
             ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			 //根据合约 获取品种 填充品种页面
			 //遍历合约,确定品种 如果是新品种 创建界面
			 {
				
				 //统计数据  
				 RiskMapStatisticMap::iterator lIteratorStart,lIteratorEnd;	
				 lIteratorStart = mRiskMapData->mmapStatics.begin();			
				 cliext::multimap<String^,TotalStatiscData^> ^  lTypeTotalData = gcnew cliext::multimap<String^,TotalStatiscData^>();
				 while(lIteratorStart != mRiskMapData->mmapStatics.end())
				 {		
					 String ^ lsIID = Tools::string2String(lIteratorStart->second.InstrumentID);	
					 if(lsIID == "")
					 {
						 lIteratorStart++;
						 continue;
					 }
					 InstrumentField instrument;
					 memset(&instrument, 0, sizeof(instrument));
					 if(m_pdataInfo->GetInstrumentFieldByInstrumentID(std::string(lIteratorStart->second.InstrumentID), instrument))
					 {
						 std::string strProductID(instrument.ProductID);
						 String ^ lsPID = Tools::string2String(instrument.ProductID);					

						 //UI 操作 汇总
						 cliext::multimap<String^,TotalStatiscData^> ::iterator lDataIterator = lTypeTotalData->find(lsPID);
						 if(lDataIterator == lTypeTotalData->end())
						 {
							 //追加数据
							 TotalStatiscData ^ lTempData = gcnew TotalStatiscData();
							 lTempData->Add(lIteratorStart->second.nStatisticsType,lIteratorStart->second.dblValue);
							 lTypeTotalData->insert(cliext::make_pair(lsPID,lTempData));

						 }		
						 else
						 {
							 //更新数据
							  TotalStatiscData ^ lTempData = lDataIterator->second;
							  lTempData->Add(lIteratorStart->second.nStatisticsType,lIteratorStart->second.dblValue);							 
						 }
					 }

					 lIteratorStart++;
				 }

				 //更新item数据
				 cliext::multimap<String^,TotalStatiscData^> ::iterator lDataPos = lTypeTotalData->begin();				
				 while(lDataPos !=  lTypeTotalData->end())
				 {			
					
					 String ^ lsPID = lDataPos->first;	
					 //UI 操作 汇总
					 cliext::multimap<String^,DepartProductTypeItem^>::iterator lUIIter = mMapProductTypeItemUI->find(lsPID);
					 if(lUIIter == mMapProductTypeItemUI->end())
					 {
						 if(lDataPos->second->mdbCCL > 0.00006)
						 {						
							 //如果持仓量不为0则追加一个UI
							 DepartProductTypeItem ^ lItem = gcnew DepartProductTypeItem();
							 mMapProductTypeItemUI->insert(cliext::make_pair(lsPID,lItem));
							 lItem->TypeName = lsPID;					
							 lItem->TotalHode = (TotalStatiscData::SafeDivMulti100(lDataPos->second->mdbCCL, lDepartTotalData->mdbCCL));
							 lItem->TotalBJZPercent = (TotalStatiscData::SafeDivMulti100(lDataPos->second->mdbBZJ , lDepartTotalData->mdbBZJ));
							 lItem->TotalHodeCount = lDataPos->second->mdbCCL;
							 lItem->TotalBZJ = lDataPos->second->mdbBZJ;						
							 this->listPanelControl2->Controls->Add(lItem);	
						 }
					 }	
					 else
					 { 
						 DepartProductTypeItem ^ lItem = lUIIter->second;
						 if(lDataPos->second->mdbCCL < 0.00006)
						 {
							 //如果持仓量为0隐藏UI							
							 mMapProductTypeItemUI->erase(lsPID);
							 this->listPanelControl2->Controls->Remove(lItem);	

						 }
						 else
						 {						
							 //更新UI												
							 lItem->TypeName = lsPID;					
							 lItem->TotalHode = (TotalStatiscData::SafeDivMulti100(lDataPos->second->mdbCCL , lDepartTotalData->mdbCCL));
							 lItem->TotalBJZPercent = (TotalStatiscData::SafeDivMulti100(lDataPos->second->mdbBZJ , lDepartTotalData->mdbBZJ));
							 lItem->TotalHodeCount = lDataPos->second->mdbCCL;
							 lItem->TotalBZJ = lDataPos->second->mdbBZJ; 
						 }
					 }				

					 lDataPos++;
				 }

			 }

			 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			 //更新柱状图,按合约统计
			 { 
				 //按合约统计
				 RiskMapStatisticMap::iterator lIteratorStart,lIteratorEnd;	
				 lIteratorStart = mRiskMapData->mmapStatics.begin();			
				 cliext::multimap<String^,TotalStatiscData^> ^  lInstrumentTotalData = gcnew cliext::multimap<String^,TotalStatiscData^>();
				 while(lIteratorStart != mRiskMapData->mmapStatics.end())
				 {						
					 //UI 操作 汇总
					  String ^ lsIID = Tools::string2String(lIteratorStart->second.InstrumentID);	
					  if(lsIID == "")
					  {
						  lIteratorStart++;
						  continue;
					  }

					  cliext::multimap<String^,TotalStatiscData^> ::iterator lDataIterator = 
						 lInstrumentTotalData->find(lsIID);

					 if(lDataIterator == lInstrumentTotalData->end())
					 {
						 //追加数据
						 TotalStatiscData ^ lTempData = gcnew TotalStatiscData();
						 lTempData->Add(lIteratorStart->second.nStatisticsType,lIteratorStart->second.dblValue);
						 lInstrumentTotalData->insert(cliext::make_pair(lsIID,lTempData));

					 }		
					 else
					 {
						 //更新数据
						 TotalStatiscData ^ lTempData = lDataIterator->second;
						 lTempData->Add(lIteratorStart->second.nStatisticsType,lIteratorStart->second.dblValue);
					 }
				

					 lIteratorStart++;
				 }
				 //更新柱状图
				 UpdateGrid(lInstrumentTotalData);

			 }
		 }







private: System::Void UpdateGrid(cliext::multimap<String^,TotalStatiscData^> ^ nInstrumentDataMap) {
			 if(nInstrumentDataMap->size() > 0)
			 {
				 chart1->Series["Series1"]->Points->Clear();				
				 cliext::multimap<String^,TotalStatiscData^>::iterator lIter = nInstrumentDataMap->begin();
				 while(lIter != nInstrumentDataMap->end())
				 {
					 if(lIter->second->mdbCCL > 0.00006 || lIter->second->mdbCCL < -0.00006)
						chart1->Series["Series1"]->Points->AddXY(lIter->first,lIter->second->mdbCCL);					 
					  lIter++;
				 }

			 }

		 }

private: System::Void pictureBox4_Click(System::Object^  sender, System::EventArgs^  e) {
			 //去工具条
			 if(this->FormBorderStyle != System::Windows::Forms::FormBorderStyle::None)
			 {
				 this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
				// this->Location =  System::Drawing::Point(0, 0);
				// this->Size = System::Windows::Forms::Screen::PrimaryScreen->Bounds.Size;
				 this->WindowState = System::Windows::Forms::FormWindowState::Maximized; 
				 this->TopMost = true; 				
				 LayOut();
				 this->Refresh();
			 }
			 else
			 {
				
				 this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle; 
				 this->WindowState = System::Windows::Forms::FormWindowState::Maximized; 
				 this->TopMost = false; 
				
				 LayOut(); 
				 this->Refresh();
			 }
		 }
public: System::Void CombinationScreenSetting(System::Drawing::Point  nLocation,System::Drawing::Size  nSize)
		 {
			  this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
			  this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
			  this->WindowState = System::Windows::Forms::FormWindowState::Normal; 
			  this->Location = nLocation;
			  this->Size = nSize;			
			  mbCombinationScreen = true;
			/*  LayOut();
			  this->Refresh();*/

		 }
};
}
