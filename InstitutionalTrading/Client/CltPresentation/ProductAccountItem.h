#pragma once
#include "RiskMapData.h"
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace CltPresentation {

	/// <summary>
	/// Summary for ProductAccountItem
	/// </summary>
	public ref class ProductAccountItem : public System::Windows::Forms::UserControl
	{
	public:
		ProductAccountItem(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}
	private: System::Windows::Forms::Label^  label8;
	public: 
	private: System::Windows::Forms::Label^  label9;
	private: Double mdbCSQY;
	public : property Double CSQY
			 {
				 Double get() {return this->mdbCSQY;}
				 Void set(Double nsVal) { this->mdbCSQY= nsVal;}
			 }
	public : property String^ AccountName
			 {
				// Int32 get() {return this->label4->Text;}
				 Void set(String^ nsVal) { this->label4->Text = nsVal;}
			 }
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ProductAccountItem()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label4;
	protected: 
	private: System::Windows::Forms::Panel^  panel2;

	private: System::Windows::Forms::Label^  label2;




	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Panel^  panel3;
	private: System::Windows::Forms::Panel^  panel4;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Panel^  panel5;
	private: ::SimpleFlagControl::SimpleFlagControl^  simpleFlagControl2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label7;
	private: ::SimpleFlagControl::SimpleFlagControl^  simpleFlagControl1;

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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(ProductAccountItem::typeid));
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->panel5 = (gcnew System::Windows::Forms::Panel());
			this->simpleFlagControl2 = (gcnew ::SimpleFlagControl::SimpleFlagControl());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->panel4 = (gcnew System::Windows::Forms::Panel());
			this->simpleFlagControl1 = (gcnew ::SimpleFlagControl::SimpleFlagControl());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->panel3 = (gcnew System::Windows::Forms::Panel());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->panel2->SuspendLayout();
			this->panel5->SuspendLayout();
			this->panel4->SuspendLayout();
			this->panel3->SuspendLayout();
			this->SuspendLayout();
			// 
			// label4
			// 
			this->label4->BackColor = System::Drawing::Color::DimGray;
			this->label4->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->label4->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei", 15, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->label4->ForeColor = System::Drawing::Color::White;
			this->label4->Location = System::Drawing::Point(0, 81);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(180, 28);
			this->label4->TabIndex = 2;
			this->label4->Text = L"理财帐户1";
			this->label4->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// panel2
			// 
			this->panel2->Controls->Add(this->panel5);
			this->panel2->Controls->Add(this->panel4);
			this->panel2->Controls->Add(this->label4);
			this->panel2->Location = System::Drawing::Point(63, -1);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(180, 109);
			this->panel2->TabIndex = 4;
			// 
			// panel5
			// 
			this->panel5->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"panel5.BackgroundImage")));
			this->panel5->Controls->Add(this->simpleFlagControl2);
			this->panel5->Controls->Add(this->label3);
			this->panel5->Controls->Add(this->label7);
			this->panel5->Location = System::Drawing::Point(1, 41);
			this->panel5->Name = L"panel5";
			this->panel5->Size = System::Drawing::Size(175, 37);
			this->panel5->TabIndex = 4;
			// 
			// simpleFlagControl2
			// 
			this->simpleFlagControl2->BackColor = System::Drawing::Color::Transparent;
			this->simpleFlagControl2->BackColour = System::Drawing::Color::Red;
			this->simpleFlagControl2->ForeColor = System::Drawing::Color::Lime;
			this->simpleFlagControl2->ForeColour = System::Drawing::Color::Lime;
			this->simpleFlagControl2->Location = System::Drawing::Point(-1, 22);
			this->simpleFlagControl2->Maximum = 100;
			this->simpleFlagControl2->Minimum = -100;
			this->simpleFlagControl2->Name = L"simpleFlagControl2";
			this->simpleFlagControl2->Size = System::Drawing::Size(174, 11);
			this->simpleFlagControl2->TabIndex = 2;
			this->simpleFlagControl2->Value = 10;
			// 
			// label3
			// 
			this->label3->ForeColor = System::Drawing::Color::Lime;
			this->label3->Location = System::Drawing::Point(90, -1);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(83, 20);
			this->label3->TabIndex = 1;
			this->label3->Text = L"0万";
			this->label3->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// label7
			// 
			this->label7->ForeColor = System::Drawing::Color::White;
			this->label7->Location = System::Drawing::Point(2, -1);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(82, 20);
			this->label7->TabIndex = 1;
			this->label7->Text = L"浮盈亏";
			this->label7->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// panel4
			// 
			this->panel4->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"panel4.BackgroundImage")));
			this->panel4->Controls->Add(this->simpleFlagControl1);
			this->panel4->Controls->Add(this->label1);
			this->panel4->Controls->Add(this->label2);
			this->panel4->Location = System::Drawing::Point(1, 4);
			this->panel4->Name = L"panel4";
			this->panel4->Size = System::Drawing::Size(175, 37);
			this->panel4->TabIndex = 3;
			// 
			// simpleFlagControl1
			// 
			this->simpleFlagControl1->BackColor = System::Drawing::Color::Transparent;
			this->simpleFlagControl1->BackColour = System::Drawing::Color::Red;
			this->simpleFlagControl1->ForeColor = System::Drawing::Color::Lime;
			this->simpleFlagControl1->ForeColour = System::Drawing::Color::Lime;
			this->simpleFlagControl1->Location = System::Drawing::Point(0, 23);
			this->simpleFlagControl1->Maximum = 100;
			this->simpleFlagControl1->Minimum = -100;
			this->simpleFlagControl1->Name = L"simpleFlagControl1";
			this->simpleFlagControl1->Size = System::Drawing::Size(174, 11);
			this->simpleFlagControl1->TabIndex = 2;
			this->simpleFlagControl1->Value = 10;
			// 
			// label1
			// 
			this->label1->ForeColor = System::Drawing::Color::White;
			this->label1->Location = System::Drawing::Point(3, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(82, 20);
			this->label1->TabIndex = 1;
			this->label1->Text = L"盈亏比";
			this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// label2
			// 
			this->label2->ForeColor = System::Drawing::Color::Lime;
			this->label2->Location = System::Drawing::Point(91, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(83, 20);
			this->label2->TabIndex = 1;
			this->label2->Text = L"+0%";
			this->label2->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// label6
			// 
			this->label6->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->label6->ForeColor = System::Drawing::Color::White;
			this->label6->Location = System::Drawing::Point(128, 26);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(74, 52);
			this->label6->TabIndex = 0;
			this->label6->Text = L"0%";
			this->label6->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label5
			// 
			this->label5->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->label5->ForeColor = System::Drawing::Color::White;
			this->label5->Location = System::Drawing::Point(25, 26);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(72, 52);
			this->label5->TabIndex = 0;
			this->label5->Text = L"0%";
			this->label5->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// panel1
			// 
			this->panel1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"panel1.BackgroundImage")));
			this->panel1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->panel1->Location = System::Drawing::Point(1, -1);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(57, 109);
			this->panel1->TabIndex = 6;
			// 
			// panel3
			// 
			this->panel3->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"panel3.BackgroundImage")));
			this->panel3->Controls->Add(this->label8);
			this->panel3->Controls->Add(this->label9);
			this->panel3->Controls->Add(this->label6);
			this->panel3->Controls->Add(this->label5);
			this->panel3->Location = System::Drawing::Point(247, -1);
			this->panel3->Name = L"panel3";
			this->panel3->Size = System::Drawing::Size(205, 109);
			this->panel3->TabIndex = 5;
			// 
			// label8
			// 
			this->label8->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->label8->ForeColor = System::Drawing::SystemColors::ControlDark;
			this->label8->Location = System::Drawing::Point(102, 13);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(20, 93);
			this->label8->TabIndex = 3;
			this->label8->Text = L"风险度";
			// 
			// label9
			// 
			this->label9->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->label9->ForeColor = System::Drawing::SystemColors::ControlDark;
			this->label9->Location = System::Drawing::Point(-1, 13);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(20, 93);
			this->label9->TabIndex = 2;
			this->label9->Text = L"总权益";
			// 
			// ProductAccountItem
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::Transparent;
			this->Controls->Add(this->panel2);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->panel3);
			this->Name = L"ProductAccountItem";
			this->Size = System::Drawing::Size(452, 108);
			this->Load += gcnew System::EventHandler(this, &ProductAccountItem::OnLoad);
			this->panel2->ResumeLayout(false);
			this->panel5->ResumeLayout(false);
			this->panel4->ResumeLayout(false);
			this->panel3->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void OnLoad(System::Object^  sender, System::EventArgs^  e) {
				/* this->DoubleBuffered = true;
				 this->panel1->DoubleBuffered = true;
				 this->panel2->DoubleBuffered = true;
				 this->panel4->DoubleBuffered = true;
				 this->panel5->DoubleBuffered = true;
				 this->label1->DoubleBuffered = true;
				 this->label2->DoubleBuffered = true;*/
				 this->panel1->BackColor = Color::FromArgb(165,0,0,0);
				 this->panel2->BackColor = Color::FromArgb(165,0,0,0);
				 this->panel3->BackColor = Color::FromArgb(165,0,0,0);
				 this->label1->BackColor = Color::Transparent;
				 this->label2->BackColor = Color::Transparent;
				 this->label3->BackColor = Color::Transparent;
				 this->label4->BackColor = Color::Transparent;
				 this->label5->BackColor = Color::Transparent;
				 this->label6->BackColor = Color::Transparent;
				 this->label7->BackColor = Color::Transparent;
				 this->label8->BackColor = Color::Transparent;
				 this->label9->BackColor = Color::Transparent;

			 }
	public: System::Void UpdateData(sFundInfoEx * nTta){
				if(nTta == NULL)
					return;
				//有初始权益
				CSQY = nTta->mdbInitFund;
				//盈亏比
				Double lnFuYinKui = TotalStatiscData::SafeDivMulti100((nTta->mTradingAccountInfo.DynamicProfit - CSQY),CSQY);
				this->label2->Text = lnFuYinKui.ToString("f2") + L"%";          
				this->simpleFlagControl1->Value = (Single)lnFuYinKui;
				//浮动盈亏
				Double ldbYinKui =(nTta->mTradingAccountInfo.DynamicProfit - CSQY)/10000.00;
				this->label3->Text = ldbYinKui.ToString("N2") + L"万";                                             
				this->simpleFlagControl2->Value = (Single) TotalStatiscData::SafeDivMulti100(ldbYinKui , TotalStatiscData::GetNearestTen(ldbYinKui));
	
				//总权益
	            this->label5->Text = (Int32(TotalStatiscData::SafeDivMulti100(nTta->mTradingAccountInfo.DynamicProfit,CSQY))).ToString() + L"%";     
				 //风险度
				this->label6->Text = (Int32(TotalStatiscData::SafeDivMulti100(nTta->mTradingAccountInfo.CurrMargin,nTta->mTradingAccountInfo.DynamicProfit))).ToString() + L"%";  
				return;

			 }
   // public: System::Void UpdateData(RiskSyncAccountField * nRsa){
			//	
			//	//盈亏比
			//	Double lnFuYinKui = TotalStatiscData::SafeDivMulti100((nRsa->Balance - CSQY),CSQY);
			//	this->label2->Text = lnFuYinKui.ToString("f2") + L"%";          
			//	this->simpleFlagControl2->Value = (Single)lnFuYinKui;
			//	//浮动盈亏
			//	Double ldbYinKui = (nRsa->Balance - CSQY)/10000.00;
			//	this->label3->Text = ldbYinKui.ToString(L"N2") + L"万";                                             
			//	this->simpleFlagControl2->Value = (Single) TotalStatiscData::SafeDivMulti100(ldbYinKui ,  TotalStatiscData::GetNearestTen(ldbYinKui));

			//	//总权益
			//	this->label5->Text = (Int32(TotalStatiscData::SafeDivMulti100(nRsa->Balance,CSQY))).ToString() + L"%";     
			//	//风险度
			//	this->label6->Text = (Int32(TotalStatiscData::SafeDivMulti100(nRsa->CurrMargin,nRsa->Balance))).ToString() + L"%";  
			//	return;
			//	
			//}
};
}
