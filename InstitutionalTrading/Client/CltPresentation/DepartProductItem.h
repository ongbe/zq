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
	/// Summary for DepartProductItem
	/// </summary>
	public ref class DepartProductItem : public System::Windows::Forms::UserControl
	{
	public:
		DepartProductItem(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}
	public : property String^ ProductName
			 {
				 String ^ get() {return this->label4->Text;}
				 Void set(String ^ nsVal) { this->label4->Text = nsVal;}
			 }
	public : property String^ TotalQuanYi //总权益比例数
			 {
				 String ^ get() {return this->label5->Text;}
				 Void set(String ^ nsVal) { this->label5->Text = nsVal;}
			 }
	public : property String^ TotalFengXian //总风险度比例 
			 {
				 String ^ get() {return this->label6->Text;}
				 Void set(String ^ nsVal) { this->label6->Text = nsVal;}
			 }
	public : property Int32 FirstQuanYi   //初始权益比例 0-100
			 {
				 Int32  get() {return this->simpleGradientProgessBar2->Value;}
				 Void set(Int32  nVal) { this->simpleGradientProgessBar2->Value = nVal;}
			 }
	//public : property Int32 HodeYinKui   //持仓比例 0-100
	//		 {
	//			 Int32  get() {return this->simpleGradientProgessBar3->Value;}
	//			 Void set(Int32  nVal) { this->simpleGradientProgessBar3->Value = nVal;}
	//		 }
	public : property Double YingKui   //盈亏
			 {
				
				 Void set(Double  nVal) 
				 {
					 this->label3->Text = (nVal/10000).ToString("N2") + L"万" ;                                             
					 this->simpleFlagControl1->Value = (Single) TotalStatiscData::SafeDivMulti100(nVal , TotalStatiscData::GetNearestTen(nVal));
				 }
			 }
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~DepartProductItem()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label4;
	protected: 
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Label^  label6;

	private: System::Windows::Forms::Panel^  panel3;
	private: System::Windows::Forms::Label^  label5;

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Panel^  panel2;


	private: SimpleGradientProgressBar::SimpleGradientProgessBar^  simpleGradientProgessBar2;
	private: ::SimpleFlagControl::SimpleFlagControl^  simpleFlagControl1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Panel^  panel4;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label8;

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
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(DepartProductItem::typeid));
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->panel3 = (gcnew System::Windows::Forms::Panel());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->simpleFlagControl1 = (gcnew ::SimpleFlagControl::SimpleFlagControl());
			this->simpleGradientProgessBar2 = (gcnew SimpleGradientProgressBar::SimpleGradientProgessBar());
			this->panel4 = (gcnew System::Windows::Forms::Panel());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->panel3->SuspendLayout();
			this->panel2->SuspendLayout();
			this->panel4->SuspendLayout();
			this->SuspendLayout();
			// 
			// label4
			// 
			this->label4->BackColor = System::Drawing::Color::Transparent;
			this->label4->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->label4->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei", 15, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->label4->ForeColor = System::Drawing::Color::White;
			this->label4->Location = System::Drawing::Point(0, 76);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(180, 28);
			this->label4->TabIndex = 2;
			this->label4->Text = L"理财产品1";
			this->label4->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// panel1
			// 
			this->panel1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"panel1.BackgroundImage")));
			this->panel1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->panel1->Location = System::Drawing::Point(1, 1);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(57, 104);
			this->panel1->TabIndex = 3;
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
			// panel3
			// 
			this->panel3->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"panel3.BackgroundImage")));
			this->panel3->Controls->Add(this->label8);
			this->panel3->Controls->Add(this->label7);
			this->panel3->Controls->Add(this->label6);
			this->panel3->Controls->Add(this->label5);
			this->panel3->Location = System::Drawing::Point(248, 2);
			this->panel3->Name = L"panel3";
			this->panel3->Size = System::Drawing::Size(205, 104);
			this->panel3->TabIndex = 2;
			// 
			// label8
			// 
			this->label8->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->label8->ForeColor = System::Drawing::SystemColors::ControlDark;
			this->label8->Location = System::Drawing::Point(102, 10);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(20, 93);
			this->label8->TabIndex = 1;
			this->label8->Text = L"风险度";
			// 
			// label7
			// 
			this->label7->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->label7->ForeColor = System::Drawing::SystemColors::ControlDark;
			this->label7->Location = System::Drawing::Point(-1, 10);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(20, 93);
			this->label7->TabIndex = 1;
			this->label7->Text = L"总权益";
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
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->ForeColor = System::Drawing::Color::Lime;
			this->label1->Location = System::Drawing::Point(6, 11);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(53, 12);
			this->label1->TabIndex = 1;
			this->label1->Text = L"初始权益";
			// 
			// panel2
			// 
			this->panel2->Controls->Add(this->simpleFlagControl1);
			this->panel2->Controls->Add(this->label4);
			this->panel2->Controls->Add(this->label1);
			this->panel2->Controls->Add(this->simpleGradientProgessBar2);
			this->panel2->Controls->Add(this->panel4);
			this->panel2->Location = System::Drawing::Point(64, 1);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(180, 104);
			this->panel2->TabIndex = 1;
			// 
			// simpleFlagControl1
			// 
			this->simpleFlagControl1->BackColor = System::Drawing::Color::Transparent;
			this->simpleFlagControl1->BackColour = System::Drawing::Color::Red;
			this->simpleFlagControl1->ForeColor = System::Drawing::Color::Lime;
			this->simpleFlagControl1->ForeColour = System::Drawing::Color::Lime;
			this->simpleFlagControl1->Location = System::Drawing::Point(7, 58);
			this->simpleFlagControl1->Maximum = 100;
			this->simpleFlagControl1->Minimum = -100;
			this->simpleFlagControl1->Name = L"simpleFlagControl1";
			this->simpleFlagControl1->Size = System::Drawing::Size(166, 12);
			this->simpleFlagControl1->TabIndex = 5;
			this->simpleFlagControl1->Value = 10;
			// 
			// simpleGradientProgessBar2
			// 
			this->simpleGradientProgessBar2->BackColor = System::Drawing::Color::Transparent;
			this->simpleGradientProgessBar2->GradientStyle = System::Drawing::Drawing2D::LinearGradientMode::Horizontal;
			this->simpleGradientProgessBar2->Location = System::Drawing::Point(63, 9);
			this->simpleGradientProgessBar2->Maximum = 100;
			this->simpleGradientProgessBar2->Minimum = 0;
			this->simpleGradientProgessBar2->Name = L"simpleGradientProgessBar2";
			this->simpleGradientProgessBar2->ProgressBarBackColour = System::Drawing::Color::Lime;
			this->simpleGradientProgessBar2->ProgressBarForeColour = System::Drawing::Color::Red;
			this->simpleGradientProgessBar2->ShowLabel = true;
			this->simpleGradientProgessBar2->Size = System::Drawing::Size(110, 15);
			this->simpleGradientProgessBar2->TabIndex = 0;
			this->simpleGradientProgessBar2->Value = 0;
			// 
			// panel4
			// 
			this->panel4->BackColor = System::Drawing::Color::DimGray;
			this->panel4->Controls->Add(this->label2);
			this->panel4->Controls->Add(this->label3);
			this->panel4->Location = System::Drawing::Point(4, 35);
			this->panel4->Name = L"panel4";
			this->panel4->Size = System::Drawing::Size(173, 38);
			this->panel4->TabIndex = 6;
			// 
			// label2
			// 
			this->label2->ForeColor = System::Drawing::Color::Lime;
			this->label2->Location = System::Drawing::Point(3, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(82, 20);
			this->label2->TabIndex = 4;
			this->label2->Text = L"盈亏";
			this->label2->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// label3
			// 
			this->label3->ForeColor = System::Drawing::Color::Lime;
			this->label3->Location = System::Drawing::Point(87, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(83, 20);
			this->label3->TabIndex = 3;
			this->label3->Text = L"+0%";
			this->label3->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// DepartProductItem
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::Transparent;
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->panel3);
			this->Controls->Add(this->panel2);
			this->Name = L"DepartProductItem";
			this->Size = System::Drawing::Size(452, 108);
			this->Load += gcnew System::EventHandler(this, &DepartProductItem::OnLoad);
			this->panel3->ResumeLayout(false);
			this->panel2->ResumeLayout(false);
			this->panel2->PerformLayout();
			this->panel4->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void OnLoad(System::Object^  sender, System::EventArgs^  e) {
				 this->panel1->BackColor = Color::FromArgb(125,0,0,0);
				 this->panel2->BackColor = Color::FromArgb(125,0,0,0);
				 this->panel3->BackColor = Color::FromArgb(125,0,0,0);
				 this->label1->BackColor = Color::Transparent;
				 this->label2->BackColor = Color::Transparent;
				 this->label3->BackColor = Color::Transparent;
				 this->label5->BackColor = Color::Transparent;
				 this->label6->BackColor = Color::Transparent;
				 this->label7->BackColor = Color::Transparent;
				 this->label8->BackColor = Color::Transparent;

			 }
};
}
