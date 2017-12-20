#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
#include "ProductAccountItem.h"
#include "RiskMapData.h"
#include "CDataInfo.h"
#include "Tools.h"
#include <cliext/map>

namespace CltPresentation {

	/// <summary>
	/// Summary for ProductShow
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class ProductShow : public System::Windows::Forms::Form
	{
	public:
		ProductShow(void)
		{
			InitializeComponent();
			m_pdataInfo = CDataInfo::NewInstance();
			mRiskMapData = CRiskMapData::NewInstance();	
			mMapAccountItemUI = gcnew cliext::multimap<int,ProductAccountItem^>();
			Random^ rand = gcnew Random(Guid::NewGuid().GetHashCode());
			mnSeqID = rand->Next(1000000);
			//
			//TODO: Add the constructor code here
			//
		}
	private: Int32 mnSeqID;
	private: static Int32 mnMaxAccount = 20;
	private: CDataInfo* m_pdataInfo;
	private: CRiskMapData * mRiskMapData;
	private: Int32 mnCurrentOrgID;
	private: Int32 mnCurrentProduceID;
	private: String ^ msProductName;
	private: System::Windows::Forms::Panel^  panel8;

	private: System::Windows::Forms::Label^  label4;
	private: cliext::multimap<int,ProductAccountItem^> ^ mMapAccountItemUI;//帐户UI map
	
	public: property Int32 CurrentOrgID
			{
				Int32 get() {return mnCurrentOrgID;}
				void set(Int32 nID) {mnCurrentOrgID = nID;}
			}
	public: property Int32 CurrentProductID
			{
				Int32 get() {return mnCurrentProduceID;}
				void set(Int32 nID) {mnCurrentProduceID = nID;}
			}
	public: property String^ CurrentProductName
			{
				String^ get() {return msProductName;}
				void set(String^ ns) {msProductName = ns;}
			}
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ProductShow()
		{
			if (components)
			{
				delete components;
			}
		}

	protected: 
	private: System::Windows::Forms::Panel^  panel2;

	private: ListPanel::ListPanelControl^  listPanelControl1;
	private: ListPanel::ListPanelControl^  listPanelControl2;
	private: System::Windows::Forms::Panel^  panel3;

	private: ::SimpleGuage::SimpleGuage^  simpleGuage1;
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Panel^  panel4;
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^  chart1;
	private: System::Windows::Forms::PictureBox^  pictureBox4;
	private: System::Windows::Forms::Panel^  panel5;
	private: System::Windows::Forms::PictureBox^  pictureBox3;
	private: System::Windows::Forms::Panel^  panel6;
	private: System::Windows::Forms::PictureBox^  pictureBox2;
	private: System::Windows::Forms::Panel^  panel7;
	private: System::Windows::Forms::Label^  label2;


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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(ProductShow::typeid));
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea1 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Series^  series1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->panel3 = (gcnew System::Windows::Forms::Panel());
			this->simpleGuage1 = (gcnew ::SimpleGuage::SimpleGuage());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->panel4 = (gcnew System::Windows::Forms::Panel());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->chart1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->pictureBox4 = (gcnew System::Windows::Forms::PictureBox());
			this->panel5 = (gcnew System::Windows::Forms::Panel());
			this->pictureBox3 = (gcnew System::Windows::Forms::PictureBox());
			this->panel6 = (gcnew System::Windows::Forms::Panel());
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			this->panel7 = (gcnew System::Windows::Forms::Panel());
			this->panel8 = (gcnew System::Windows::Forms::Panel());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->listPanelControl1 = (gcnew ListPanel::ListPanelControl());
			this->listPanelControl2 = (gcnew ListPanel::ListPanelControl());
			this->panel1->SuspendLayout();
			this->panel4->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->chart1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox4))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox3))->BeginInit();
			this->panel6->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox2))->BeginInit();
			this->panel8->SuspendLayout();
			this->SuspendLayout();
			// 
			// panel2
			// 
			this->panel2->BackColor = System::Drawing::Color::Transparent;
			this->panel2->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"panel2.BackgroundImage")));
			this->panel2->Location = System::Drawing::Point(326, 172);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(61, 561);
			this->panel2->TabIndex = 19;
			// 
			// panel3
			// 
			this->panel3->BackColor = System::Drawing::Color::Transparent;
			this->panel3->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"panel3.BackgroundImage")));
			this->panel3->Location = System::Drawing::Point(965, 172);
			this->panel3->Name = L"panel3";
			this->panel3->Size = System::Drawing::Size(61, 540);
			this->panel3->TabIndex = 20;
			// 
			// simpleGuage1
			// 
			this->simpleGuage1->BackColor = System::Drawing::Color::Transparent;
			this->simpleGuage1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"simpleGuage1.BackgroundImage")));
			this->simpleGuage1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->simpleGuage1->BigArcCount = 300;
			this->simpleGuage1->BigArcStart = 120;
			this->simpleGuage1->BigNeedleRadius = 95;
			this->simpleGuage1->Location = System::Drawing::Point(59, 3);
			this->simpleGuage1->Name = L"simpleGuage1";
			this->simpleGuage1->Size = System::Drawing::Size(201, 201);
			this->simpleGuage1->SmallArcCount = 270;
			this->simpleGuage1->SmallArcStart = 120;
			this->simpleGuage1->SmallNeedleRadius = 58;
			this->simpleGuage1->TabIndex = 5;
			this->simpleGuage1->Value = 55.5F;
			// 
			// panel1
			// 
			this->panel1->BackColor = System::Drawing::Color::Transparent;
			this->panel1->Controls->Add(this->panel4);
			this->panel1->Location = System::Drawing::Point(2, 165);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(318, 275);
			this->panel1->TabIndex = 16;
			// 
			// panel4
			// 
			this->panel4->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"panel4.BackgroundImage")));
			this->panel4->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->panel4->Controls->Add(this->label2);
			this->panel4->Location = System::Drawing::Point(3, 3);
			this->panel4->Name = L"panel4";
			this->panel4->Size = System::Drawing::Size(312, 269);
			this->panel4->TabIndex = 6;
			// 
			// label2
			// 
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei", 15, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->label2->ForeColor = System::Drawing::Color::White;
			this->label2->Location = System::Drawing::Point(42, 114);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(215, 83);
			this->label2->TabIndex = 0;
			this->label2->Text = L"理财产品";
			this->label2->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// chart1
			// 
			this->chart1->BackColor = System::Drawing::Color::Transparent;
			chartArea1->AxisX->LabelStyle->ForeColor = System::Drawing::Color::Lime;
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
			this->chart1->Location = System::Drawing::Point(393, 552);
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
			this->chart1->TabIndex = 10;
			this->chart1->Text = L"chart1";
			// 
			// pictureBox4
			// 
			this->pictureBox4->BackColor = System::Drawing::Color::Transparent;
			this->pictureBox4->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox4.BackgroundImage")));
			this->pictureBox4->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->pictureBox4->Location = System::Drawing::Point(965, 3);
			this->pictureBox4->Name = L"pictureBox4";
			this->pictureBox4->Size = System::Drawing::Size(306, 136);
			this->pictureBox4->TabIndex = 25;
			this->pictureBox4->TabStop = false;
			this->pictureBox4->Click += gcnew System::EventHandler(this, &ProductShow::pictureBox4_Click);
			// 
			// panel5
			// 
			this->panel5->BackColor = System::Drawing::Color::Black;
			this->panel5->Location = System::Drawing::Point(2, 141);
			this->panel5->Name = L"panel5";
			this->panel5->Size = System::Drawing::Size(1269, 25);
			this->panel5->TabIndex = 24;
			// 
			// pictureBox3
			// 
			this->pictureBox3->BackColor = System::Drawing::Color::Transparent;
			this->pictureBox3->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox3.BackgroundImage")));
			this->pictureBox3->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->pictureBox3->Location = System::Drawing::Point(170, 74);
			this->pictureBox3->Name = L"pictureBox3";
			this->pictureBox3->Size = System::Drawing::Size(119, 61);
			this->pictureBox3->TabIndex = 23;
			this->pictureBox3->TabStop = false;
			// 
			// panel6
			// 
			this->panel6->BackColor = System::Drawing::Color::Transparent;
			this->panel6->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"panel6.BackgroundImage")));
			this->panel6->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->panel6->Controls->Add(this->pictureBox2);
			this->panel6->Location = System::Drawing::Point(2, 3);
			this->panel6->Name = L"panel6";
			this->panel6->Size = System::Drawing::Size(162, 136);
			this->panel6->TabIndex = 22;
			// 
			// pictureBox2
			// 
			this->pictureBox2->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox2.BackgroundImage")));
			this->pictureBox2->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->pictureBox2->Location = System::Drawing::Point(35, 67);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(124, 65);
			this->pictureBox2->TabIndex = 0;
			this->pictureBox2->TabStop = false;
			// 
			// panel7
			// 
			this->panel7->BackColor = System::Drawing::Color::Transparent;
			this->panel7->Location = System::Drawing::Point(2, 451);
			this->panel7->Name = L"panel7";
			this->panel7->Size = System::Drawing::Size(1034, 29);
			this->panel7->TabIndex = 26;
			// 
			// panel8
			// 
			this->panel8->BackColor = System::Drawing::Color::Transparent;
			this->panel8->Controls->Add(this->label4);
			this->panel8->Controls->Add(this->simpleGuage1);
			this->panel8->Location = System::Drawing::Point(2, 491);
			this->panel8->Name = L"panel8";
			this->panel8->Size = System::Drawing::Size(318, 275);
			this->panel8->TabIndex = 16;
			// 
			// label4
			// 
			this->label4->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label4->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->label4->Font = (gcnew System::Drawing::Font(L"SimSun", 36, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->label4->ForeColor = System::Drawing::Color::DarkGoldenrod;
			this->label4->Location = System::Drawing::Point(3, 211);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(312, 58);
			this->label4->TabIndex = 4;
			this->label4->Text = L"总风险度";
			this->label4->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// listPanelControl1
			// 
			this->listPanelControl1->AutoScroll = true;
			this->listPanelControl1->BackColor = System::Drawing::Color::Transparent;
			this->listPanelControl1->FlowDirection = System::Windows::Forms::FlowDirection::TopDown;
			this->listPanelControl1->Location = System::Drawing::Point(393, 172);
			this->listPanelControl1->Name = L"listPanelControl1";
			this->listPanelControl1->Size = System::Drawing::Size(242, 239);
			this->listPanelControl1->TabIndex = 17;
			this->listPanelControl1->Vertical = true;
			this->listPanelControl1->WrapContents = false;
			// 
			// listPanelControl2
			// 
			this->listPanelControl2->AutoScroll = true;
			this->listPanelControl2->BackColor = System::Drawing::Color::Transparent;
			this->listPanelControl2->FlowDirection = System::Windows::Forms::FlowDirection::TopDown;
			this->listPanelControl2->Location = System::Drawing::Point(641, 172);
			this->listPanelControl2->Name = L"listPanelControl2";
			this->listPanelControl2->Size = System::Drawing::Size(296, 239);
			this->listPanelControl2->TabIndex = 18;
			this->listPanelControl2->Vertical = true;
			this->listPanelControl2->WrapContents = false;
			// 
			// ProductShow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"$this.BackgroundImage")));
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->ClientSize = System::Drawing::Size(1440, 868);
			this->Controls->Add(this->panel7);
			this->Controls->Add(this->pictureBox4);
			this->Controls->Add(this->panel5);
			this->Controls->Add(this->pictureBox3);
			this->Controls->Add(this->panel6);
			this->Controls->Add(this->chart1);
			this->Controls->Add(this->panel2);
			this->Controls->Add(this->listPanelControl1);
			this->Controls->Add(this->listPanelControl2);
			this->Controls->Add(this->panel3);
			this->Controls->Add(this->panel8);
			this->Controls->Add(this->panel1);
			this->Name = L"ProductShow";
			this->Text = L"理财产品汇总展示";
			//this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::SizableToolWindow;

			//this->WindowState = System::Windows::Forms::FormWindowState::SizableToolWindow;
			this->Load += gcnew System::EventHandler(this, &ProductShow::OnLoad);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &ProductShow::OnFormClosed);
			this->panel1->ResumeLayout(false);
			this->panel4->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->chart1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox4))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox3))->EndInit();
			this->panel6->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox2))->EndInit();
			this->panel8->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void OnLoad(System::Object^  sender, System::EventArgs^  e) {
				 LayOut();
				 this->panel1->BackColor = Color::FromArgb(125,64,0,64);
				 this->listPanelControl1->BackColor = Color::FromArgb(125,43,166,30);
				 this->listPanelControl2->BackColor = Color::FromArgb(125,232,104,10);	
				 this->panel4->BackColor = Color::Transparent;
				 this->panel7->BackColor = Color::FromArgb(125,0,255,0);	
				 this->panel8->BackColor = Color::FromArgb(125,0,0,255);	
				 this->label2->Text = msProductName;
				 //创建帐户
				 //根据组织和理财产品ID 获取两者的交叉帐户
				 //std::set<int> setFinicalAccount;				
				 m_pdataInfo->GetTraderSetByProductID(mnCurrentProduceID, mRiskMapData->mStaticsAccountVector);
			
				 //std::set<int> setResult;			
				 //std::set_intersection(setOrgAccount.begin(), setOrgAccount.end(), 
					// setFinicalAccount.begin(), setFinicalAccount.end(), inserter(mRiskMapData->mStaticsAccountVector,mRiskMapData->mStaticsAccountVector.end()));
				 std::vector<int>::iterator lIte = mRiskMapData->mStaticsAccountVector.begin();
				 Int32 i = 1;
				 Int32 lnMax = (Int32)mRiskMapData->mStaticsAccountVector.size() > mnMaxAccount ? mnMaxAccount : (Int32)mRiskMapData->mStaticsAccountVector.size();
				 while(lIte != mRiskMapData->mStaticsAccountVector.end() && i <= lnMax )
				 { 
					 ProductAccountItem ^ lItem = gcnew ProductAccountItem();
					 UserInfo  lTraderInfo;
					 m_pdataInfo->GetUserInfoByUserID(*lIte,lTraderInfo);
					 lItem->AccountName = Tools::string2String(lTraderInfo.szAccount);
					 mMapAccountItemUI->insert(cliext::make_pair(*lIte,lItem));
					 if(i % 2 == 0)
					 {					
						 this->listPanelControl2->Controls->Add(lItem);
					 }
					 else
					 {
						 this->listPanelControl1->Controls->Add(lItem);
					 }
					 i++;
					 lIte++;

				 }

				 //this->listPanelControl2->StartAnimate();
				 /*for(int i = 0 ; i < 4; i++)
				 {
					 this->listPanelControl1->SuspendLayout();
					 ProductAccountItem ^ lItem = gcnew ProductAccountItem();
					 this->listPanelControl1->Controls->Add(lItem);
					 this->listPanelControl1->ResumeLayout(false);
				 }

				 for(int i = 0 ; i < 4; i++)
				 {
					 this->listPanelControl2->SuspendLayout();
					 ProductAccountItem ^ lItem = gcnew ProductAccountItem();
					 this->listPanelControl2->Controls->Add(lItem);
					 this->listPanelControl2->ResumeLayout(false);
				 }*/

				SubscribeStatiacsData();
				SubscribeAccountStatiacsData();
			 }
	 private: System::Void LayOut() {
				  this->SuspendLayout();
				  //排版
				  Int32 lnMainXSplit = this->panel1->Location.X + this->panel1->Size.Width;
				  Int32 lnFirstYSplit = this->panel1->Location.Y ;

				  Int32 lnLeftX = this->ClientRectangle.Width - lnMainXSplit;
				  Int32 lnLeftY = this->ClientRectangle.Height - lnFirstYSplit;

				  Int32 lnHorPaneHeigth = 40;
				  System::Drawing::Size  lnProductPanelSize = System::Drawing::Size(476,350) ;
				  System::Drawing::Size  lnProductTypePanelSize = System::Drawing::Size(476,350) ;
				  System::Drawing::Size  lnCharSize = System::Drawing::Size(lnProductPanelSize.Width+lnProductTypePanelSize.Width + 2,this->panel1->Size.Height) ;
				  Int32 lnOtherPaneWidth = (lnLeftX - lnProductPanelSize.Width - lnProductTypePanelSize.Width -10) / 2;
				 
				  this->panel5->Size = System::Drawing::Size(this->Width,this->panel5->Size.Height);
				  this->pictureBox4->Location = Point(this->ClientRectangle.Width - this->pictureBox4->Size.Width,this->pictureBox4->Location.Y);

				  if(lnOtherPaneWidth < 0)
				  {
					  this->panel2->Hide();
					  this->panel3->Hide(); 
					
					  this->panel7->Location = Point(this->panel1->Location.X,lnFirstYSplit + this->panel1->Size.Height + 2);
                      this->panel7->Size = System::Drawing::Size(this->ClientRectangle.Width - 6,lnHorPaneHeigth) ;

					  this->panel8->Location = Point(this->panel1->Location.X,this->panel7->Location.Y + this->panel7->Size.Height + 2);
					  
					  this->listPanelControl1->Location = Point(lnMainXSplit+2,this->ClientRectangle.Height - 2 - lnProductPanelSize.Height);
					  this->listPanelControl1->Size = System::Drawing::Size(lnProductPanelSize.Width,this->ClientRectangle.Height - this->listPanelControl1->Location.Y - 2) ;;
					 
					  this->listPanelControl2->Location = Point(this->listPanelControl1->Location.X + this->listPanelControl1->Size.Width +2,this->ClientRectangle.Height - 2 - lnProductPanelSize.Height);
					  this->listPanelControl2->Size = this->listPanelControl1->Size;

					  this->chart1->Location = Point(lnMainXSplit+2,lnFirstYSplit);
					  this->chart1->Size = lnCharSize;
					 
				  }
				  else
				  {
					  this->panel2->Show();
					  this->panel3->Show();
					  
					  this->panel2->Location = Point(lnMainXSplit+2,lnFirstYSplit);
					  this->panel2->Size = System::Drawing::Size(lnOtherPaneWidth,lnLeftY );

					  this->panel7->Location = Point(this->panel1->Location.X,lnFirstYSplit + this->panel1->Size.Height + 2);
					  this->panel7->Size = System::Drawing::Size(this->ClientRectangle.Width - 6,lnHorPaneHeigth) ;
					  
					  this->panel8->Location = Point(this->panel1->Location.X,this->panel7->Location.Y + this->panel7->Size.Height + 2);
					  
					  this->listPanelControl1->Location = Point(this->panel2->Location.X + this->panel2->Size.Width +2,this->panel8->Location.Y);
					  this->listPanelControl1->Size = System::Drawing::Size(lnProductPanelSize.Width,this->ClientRectangle.Height - this->listPanelControl1->Location.Y - 2) ;;

					  this->listPanelControl2->Location = Point(this->listPanelControl1->Location.X + this->listPanelControl1->Size.Width +2,this->panel8->Location.Y);
					  this->listPanelControl2->Size = this->listPanelControl1->Size; 
					 
					  this->panel3->Location = Point(this->listPanelControl2->Location.X + this->listPanelControl2->Size.Width +2,lnFirstYSplit);
					  this->panel3->Size = System::Drawing::Size(lnOtherPaneWidth,lnLeftY );

					  this->chart1->Location = Point(this->listPanelControl1->Location.X+2,lnFirstYSplit);
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
			 re.mnAssetMgmtOrgID = mnCurrentOrgID;
			 re.mnFinancialProductID=mnCurrentProduceID;
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

private: System::Void SubscribeAccountStatiacsData()
		 {
			 //订阅统计
			 IntPtr hWnd=this->Handle;
			 CTcpLayer::SubscribePkg(Cmd_RM_SubscribeUserFund_Rsp, (int)hWnd);
			 CTcpLayer::SubscribePkg(Cmd_RM_UserFund_Push, (int)hWnd);
			 CTcpLayer::SubscribePkg(Cmd_RM_UnSubscribeUserFund_Rsp, (int)hWnd);

		
			 //
			 if(mRiskMapData->mStaticsAccountVector.size() > 0)
			 {				
				 if( !CTcpLayer::SendData(Cmd_RM_SubscribeUserFund_Req, &mRiskMapData->mStaticsAccountVector[0],sizeof(int)*mRiskMapData->mStaticsAccountVector.size(), mnSeqID))
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
private: System::Void UnsubscribeAccountStatiacsData()
		 {
			 //获取需要订阅的组织和理财产品的关系数据

			 if(mRiskMapData->mStaticsAccountVector.size() > 0)
			 {				
				 if( !CTcpLayer::SendData(Cmd_RM_SubscribeUserFund_Req, &mRiskMapData->mStaticsAccountVector[0],sizeof(int)*mRiskMapData->mStaticsAccountVector.size(), 0))
				 {					
					 MessageBox::Show("退订汇总数据失败", "提示",MessageBoxButtons::OK);
				 }
			 }
		 }

		 //重载WndProc，以处理WM_USER_Win消息
protected:virtual void WndProc( Message% m ) override
		  {

			  if(m.Msg == WM_USER_Win && m.WParam.ToInt32()==WndUserCmd_YourPkgArrival) 
			  {
				  char *p = (char*)m.LParam.ToInt32();
				  Stru_UniPkgHead head;					

				  memset(&head, 0, sizeof(head));
				  memcpy(&head, p, sizeof(head));
				  switch(head.cmdid) 
				  {
				  case Cmd_RM_SubscribeStatistics_Rsp:
					  if(head.userdata3 == CF_ERROR_SUCCESS && head.seq == mnSeqID) 
					  {
						  int count= head.len/sizeof(StatisticsEntry);
						  if(count == 0)
							  break;
						  mRiskMapData->mmapStatics.clear();	

						  StatisticsEntry* pStatic = NULL;
						  mRiskMapData->LockObject();
						  mRiskMapData->mmapStatics.clear();
						  bool lbStatic = false;							 
						  for(int i = 0; i< count; i++)
						  {
							  pStatic = (StatisticsEntry*)
								  (p+sizeof(Stru_UniPkgHead)+i*sizeof(StatisticsEntry));
							  if( pStatic->mnFinancialProductID == mnCurrentProduceID)								 
								  mRiskMapData->PushStatics(pStatic->mnFinancialProductID,pStatic);

						  }
						  mRiskMapData->UnlockObject();
						  UpdateHuiZong();						 


					  }

					  break;
				  case Cmd_RM_Statistics_Push:
					  {
						  if(head.userdata3 == CF_ERROR_SUCCESS ) 
						  {
							  int count= head.len/sizeof(StatisticsEntry);
							  if(count == 0)
								  break;
							
							  StatisticsEntry* pStatic = (StatisticsEntry*)(p+sizeof(Stru_UniPkgHead));
							  if(pStatic->mnFinancialProductID != mnCurrentProduceID)	
								  break;

							  mRiskMapData->mmapStatics.erase(pStatic->mnFinancialProductID);	

							  mRiskMapData->LockObject();
							  mRiskMapData->mmapStatics.clear();
							  bool lbStatic = false;							 
							  for(int i = 0; i< count; i++)
							  {
								  pStatic = (StatisticsEntry*)
									  (p+sizeof(Stru_UniPkgHead)+i*sizeof(StatisticsEntry));
								  if(pStatic->mnFinancialProductID == mnCurrentProduceID)								 
								     mRiskMapData->PushStatics(pStatic->mnFinancialProductID,pStatic);
								 
							  }
							  mRiskMapData->UnlockObject();
							  UpdateHuiZong();						 


						  }

						  break;
					  }
				  //case Cmd_RM_Statistics_Push:
					 // {
						//  //新推一个变化过来，需要判断是新增的还是原来就有这个值
						//  //根据新推的修改datatabel的变化										 
						//  if(head.userdata1 == OPERATOR_SUCCESS && head.len == sizeof(StatisticsEntry))
						//  {
						//	  StatisticsEntry* pStatic = (StatisticsEntry*)(p+sizeof(Stru_UniPkgHead));
						//	  mRiskMapData->LockObject();
						//	  if(pStatic->nAssetMgmtOrgID == mnCurrentOrgID && pStatic->nFinancialProductID == mnCurrentProduceID)
						//	     mRiskMapData->PushStatics(pStatic->nAssetMgmtOrgID,pStatic,nullptr);
						//	  mRiskMapData->UnlockObject();
						//	  UpdateHuiZong();

						//  }
						//  break;
					 // }
				  case Cmd_RM_SubscribeUserFund_Rsp:	
				  case Cmd_RM_UserFund_Push:
					  {
						  if(head.userdata3 == CF_ERROR_SUCCESS ) 
						  {
							  sFundInfoEx* pFundInfo = NULL;
							  int nItemCount = head.len/sizeof(sFundInfoEx);							
							  for(int i=0; i<nItemCount; i++)
							  {
								  pFundInfo = (sFundInfoEx*)(p+sizeof(Stru_UniPkgHead)+i*sizeof(sFundInfoEx));
								  UserInfo acct;
								  memset(&acct,0,sizeof(acct));								 
								  m_pdataInfo->GetUserInfoByAccount(pFundInfo->mTradingAccountInfo.AccountID, acct); 
								  cliext::multimap<int,ProductAccountItem^>::iterator lIte = mMapAccountItemUI->find(acct.nUserID);
								  if(lIte != mMapAccountItemUI->end())
								  {
									 lIte->second->UpdateData(pFundInfo);									 
								  }								
							  }	
						  }
						
					  }
					  break;				
				/*  case Cmd_RM_Fund_Push:
					  {
						  if(head.userdata3 == CF_ERROR_SUCCESS) 
						  {
							  RiskSyncAccountField* pFundInfo = NULL;
							  int nItemCount = head.len/sizeof(RiskSyncAccountField);							
							  for(int i=0; i<nItemCount; i++)
							  {
								  pFundInfo = (RiskSyncAccountField*)(p+sizeof(Stru_UniPkgHead)+i*sizeof(RiskSyncAccountField));
								  TrustTradeAccount acct;
								  memset(&acct,0,sizeof(acct));								 
								  m_pdataInfo->GetTrustTradeAccount(pFundInfo->InvestorID, acct); 
								  cliext::multimap<int,ProductAccountItem^>::iterator lIte = mMapAccountItemUI->find(acct.nTradeAccountID);
								  if(lIte != mMapAccountItemUI->end())
								  {		
									  lIte->second->CSQY=acct.dProfit;
									  lIte->second->UpdateData(pFundInfo);									  
								  }								
							  }	
						  }

					  }
					  break;				 */
				  default:break;
				  }
			  }

			  Form::WndProc ( m );
		  }

private: System::Void OnFormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
			 UnsubscribeAccountStatiacsData();
			 UnsubscribeStatiacsData();
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
						 lDepartTotalData->Add(lIte->second.mnStatisticsType,lIte->second.mdblValue);
						 lIte++;
					 }
				 }

				 lfPercent = (Single)(TotalStatiscData::SafeDivMulti100(lDepartTotalData->mdbBZJ, lDepartTotalData->mdbDQQY));
				 this->simpleGuage1->Value = lfPercent;
//				 this->simpleGuage2->Value = (Single)(TotalStatiscData::SafeDivMulti100(lDepartTotalData->mdbDQQY, lDepartTotalData->mdbCCQY));
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
					 String ^ lsIID = Tools::string2String(lIteratorStart->second.mInstrumentID);
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
						 lTempData->Add(lIteratorStart->second.mnStatisticsType,lIteratorStart->second.mdblValue);
						 lInstrumentTotalData->insert(cliext::make_pair(lsIID,lTempData));

					 }		
					 else
					 {
						 //更新数据
						 TotalStatiscData ^ lTempData = lDataIterator->second;
						 lTempData->Add(lIteratorStart->second.mnStatisticsType,lIteratorStart->second.mdblValue);
					 }
					 lIteratorStart++;
				 }
				 //更新柱状图
				 UpdateGrid(lInstrumentTotalData);

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
				 this->Refresh();
				 LayOut();
			 }
			 else
			 {

				 this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle; 
				 this->WindowState = System::Windows::Forms::FormWindowState::Maximized; 
				 this->TopMost = true; 
				 LayOut();
			 }
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
