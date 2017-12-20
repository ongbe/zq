#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
#include "RiskMapData.h"

namespace CltPresentation {

	/// <summary>
	/// Summary for DepartProductTypeItem
	/// </summary>
	public ref class DepartProductTypeItem : public System::Windows::Forms::UserControl
	{
	public:
		DepartProductTypeItem(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}
	public : property String^ TypeName
			 {
				 String ^ get() {return this->label6->Text;}
				 Void set(String ^ nsVal) { this->label6->Text = nsVal;}
			 }
	public : property Double  TotalHodeCount //总持仓数
			 {			
				 Void set(Double  nsVal) 
				 { 
					 this->label2->Text = nsVal.ToString("N0"); 
					 this->simpleGradientProgessBar2->Value = this->simpleGradientProgessBar1->Value;
					 //this->simpleGradientProgessBar2->Value = TotalStatiscData::SafeDivMulti100(nsVal , TotalStatiscData::GetNearestTen(nsVal));
				 }
			 }
	public : property Double TotalHode //总持仓比例 
			 {				
				 Void set(Double nsVal) 
				 { 
					 this->label1->Text = nsVal.ToString("f2") + "%"; 
					 this->simpleGradientProgessBar1->Value = (Int32)nsVal;
				 }
			 }

	public : property Double  TotalBZJ //总保证金
			 {			
				 Void set(Double  nsVal) 
				 { 
					 this->label4->Text = (nsVal/10000).ToString("N2") + L"万"; 
					 this->simpleGradientProgessBar4->Value = this->simpleGradientProgessBar3->Value;
					 // this->simpleGradientProgessBar4->Value = TotalStatiscData::SafeDivMulti100(nsVal , TotalStatiscData::GetNearestTen(nsVal));
				 }
			 }
	public : property Double TotalBJZPercent //总保证金比率
			 {				
				 Void set(Double nsVal) 
				 { 
					 this->label3->Text = nsVal.ToString("f2") + "%"; 
					 this->simpleGradientProgessBar3->Value = (Int32)nsVal;
				 }
			 }
	
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~DepartProductTypeItem()
		{
			if (components)
			{
				delete components;
			}
		}

	protected: 

	private: SimpleGradientProgressBar::SimpleGradientProgessBar^  simpleGradientProgessBar3;
	private: SimpleGradientProgressBar::SimpleGradientProgessBar^  simpleGradientProgessBar2;
	private: SimpleGradientProgressBar::SimpleGradientProgessBar^  simpleGradientProgessBar1;

	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Panel^  panel3;
	private: System::Windows::Forms::Panel^  panel2;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  label7;
	private: SimpleGradientProgressBar::SimpleGradientProgessBar^  simpleGradientProgessBar4;

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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(DepartProductTypeItem::typeid));
			this->simpleGradientProgessBar3 = (gcnew SimpleGradientProgressBar::SimpleGradientProgessBar());
			this->simpleGradientProgessBar2 = (gcnew SimpleGradientProgressBar::SimpleGradientProgessBar());
			this->simpleGradientProgessBar1 = (gcnew SimpleGradientProgressBar::SimpleGradientProgessBar());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->panel3 = (gcnew System::Windows::Forms::Panel());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->simpleGradientProgessBar4 = (gcnew SimpleGradientProgressBar::SimpleGradientProgessBar());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->panel1->SuspendLayout();
			this->panel3->SuspendLayout();
			this->panel2->SuspendLayout();
			this->SuspendLayout();
			// 
			// simpleGradientProgessBar3
			// 
			this->simpleGradientProgessBar3->BackColor = System::Drawing::Color::Transparent;
			this->simpleGradientProgessBar3->GradientStyle = System::Drawing::Drawing2D::LinearGradientMode::Horizontal;
			this->simpleGradientProgessBar3->Location = System::Drawing::Point(2, 96);
			this->simpleGradientProgessBar3->Maximum = 100;
			this->simpleGradientProgessBar3->Minimum = 0;
			this->simpleGradientProgessBar3->Name = L"simpleGradientProgessBar3";
			this->simpleGradientProgessBar3->ProgressBarBackColour = System::Drawing::Color::Lime;
			this->simpleGradientProgessBar3->ProgressBarForeColour = System::Drawing::Color::Red;
			this->simpleGradientProgessBar3->ShowLabel = false;
			this->simpleGradientProgessBar3->Size = System::Drawing::Size(110, 10);
			this->simpleGradientProgessBar3->TabIndex = 1;
			this->simpleGradientProgessBar3->Value = 50;
			// 
			// simpleGradientProgessBar2
			// 
			this->simpleGradientProgessBar2->BackColor = System::Drawing::Color::Transparent;
			this->simpleGradientProgessBar2->GradientStyle = System::Drawing::Drawing2D::LinearGradientMode::Horizontal;
			this->simpleGradientProgessBar2->Location = System::Drawing::Point(2, 60);
			this->simpleGradientProgessBar2->Maximum = 100;
			this->simpleGradientProgessBar2->Minimum = 0;
			this->simpleGradientProgessBar2->Name = L"simpleGradientProgessBar2";
			this->simpleGradientProgessBar2->ProgressBarBackColour = System::Drawing::Color::Lime;
			this->simpleGradientProgessBar2->ProgressBarForeColour = System::Drawing::Color::Red;
			this->simpleGradientProgessBar2->ShowLabel = false;
			this->simpleGradientProgessBar2->Size = System::Drawing::Size(110, 10);
			this->simpleGradientProgessBar2->TabIndex = 1;
			this->simpleGradientProgessBar2->Value = 50;
			// 
			// simpleGradientProgessBar1
			// 
			this->simpleGradientProgessBar1->BackColor = System::Drawing::Color::Transparent;
			this->simpleGradientProgessBar1->GradientStyle = System::Drawing::Drawing2D::LinearGradientMode::Horizontal;
			this->simpleGradientProgessBar1->Location = System::Drawing::Point(2, 23);
			this->simpleGradientProgessBar1->Maximum = 100;
			this->simpleGradientProgessBar1->Minimum = 0;
			this->simpleGradientProgessBar1->Name = L"simpleGradientProgessBar1";
			this->simpleGradientProgessBar1->ProgressBarBackColour = System::Drawing::Color::Lime;
			this->simpleGradientProgessBar1->ProgressBarForeColour = System::Drawing::Color::Red;
			this->simpleGradientProgessBar1->ShowLabel = false;
			this->simpleGradientProgessBar1->Size = System::Drawing::Size(110, 10);
			this->simpleGradientProgessBar1->TabIndex = 1;
			this->simpleGradientProgessBar1->Value = 50;
			// 
			// label4
			// 
			this->label4->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei", 7.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->label4->ForeColor = System::Drawing::Color::White;
			this->label4->Location = System::Drawing::Point(51, 110);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(66, 18);
			this->label4->TabIndex = 0;
			this->label4->Text = L"0";
			this->label4->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// label3
			// 
			this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->label3->ForeColor = System::Drawing::Color::White;
			this->label3->Location = System::Drawing::Point(57, 76);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(60, 18);
			this->label3->TabIndex = 0;
			this->label3->Text = L"0%";
			this->label3->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// panel1
			// 
			this->panel1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"panel1.BackgroundImage")));
			this->panel1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->panel1->Controls->Add(this->label6);
			this->panel1->Location = System::Drawing::Point(0, 0);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(124, 106);
			this->panel1->TabIndex = 2;
			// 
			// label6
			// 
			this->label6->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei", 36, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->label6->ForeColor = System::Drawing::Color::White;
			this->label6->Location = System::Drawing::Point(3, 39);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(118, 67);
			this->label6->TabIndex = 0;
			this->label6->Text = L"IF";
			this->label6->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label2
			// 
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei", 7.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->label2->ForeColor = System::Drawing::Color::White;
			this->label2->Location = System::Drawing::Point(36, 39);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(81, 18);
			this->label2->TabIndex = 0;
			this->label2->Text = L"0.00";
			this->label2->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// label1
			// 
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->label1->ForeColor = System::Drawing::Color::White;
			this->label1->Location = System::Drawing::Point(54, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(63, 23);
			this->label1->TabIndex = 0;
			this->label1->Text = L"0%";
			this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// panel3
			// 
			this->panel3->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"panel3.BackgroundImage")));
			this->panel3->Controls->Add(this->label9);
			this->panel3->Controls->Add(this->label8);
			this->panel3->Controls->Add(this->label7);
			this->panel3->Controls->Add(this->label5);
			this->panel3->Controls->Add(this->simpleGradientProgessBar4);
			this->panel3->Controls->Add(this->simpleGradientProgessBar3);
			this->panel3->Controls->Add(this->simpleGradientProgessBar2);
			this->panel3->Controls->Add(this->simpleGradientProgessBar1);
			this->panel3->Controls->Add(this->label4);
			this->panel3->Controls->Add(this->label3);
			this->panel3->Controls->Add(this->label2);
			this->panel3->Controls->Add(this->label1);
			this->panel3->Location = System::Drawing::Point(4, 4);
			this->panel3->Name = L"panel3";
			this->panel3->Size = System::Drawing::Size(117, 148);
			this->panel3->TabIndex = 0;
			// 
			// label9
			// 
			this->label9->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei", 10.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->label9->ForeColor = System::Drawing::Color::White;
			this->label9->Location = System::Drawing::Point(-2, 110);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(61, 19);
			this->label9->TabIndex = 3;
			this->label9->Text = L"保证金";
			this->label9->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// label8
			// 
			this->label8->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei", 10.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->label8->ForeColor = System::Drawing::Color::White;
			this->label8->Location = System::Drawing::Point(-2, 75);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(69, 19);
			this->label8->TabIndex = 3;
			this->label8->Text = L"保证金比";
			this->label8->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// label7
			// 
			this->label7->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei", 10.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->label7->ForeColor = System::Drawing::Color::White;
			this->label7->Location = System::Drawing::Point(-2, 39);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(55, 19);
			this->label7->TabIndex = 3;
			this->label7->Text = L"总持仓";
			this->label7->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// label5
			// 
			this->label5->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei", 10.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->label5->ForeColor = System::Drawing::Color::White;
			this->label5->Location = System::Drawing::Point(-2, 2);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(69, 19);
			this->label5->TabIndex = 3;
			this->label5->Text = L"总持仓比";
			this->label5->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// simpleGradientProgessBar4
			// 
			this->simpleGradientProgessBar4->BackColor = System::Drawing::Color::Transparent;
			this->simpleGradientProgessBar4->GradientStyle = System::Drawing::Drawing2D::LinearGradientMode::Horizontal;
			this->simpleGradientProgessBar4->Location = System::Drawing::Point(2, 132);
			this->simpleGradientProgessBar4->Maximum = 100;
			this->simpleGradientProgessBar4->Minimum = 0;
			this->simpleGradientProgessBar4->Name = L"simpleGradientProgessBar4";
			this->simpleGradientProgessBar4->ProgressBarBackColour = System::Drawing::Color::Lime;
			this->simpleGradientProgessBar4->ProgressBarForeColour = System::Drawing::Color::Red;
			this->simpleGradientProgessBar4->ShowLabel = false;
			this->simpleGradientProgessBar4->Size = System::Drawing::Size(110, 10);
			this->simpleGradientProgessBar4->TabIndex = 1;
			this->simpleGradientProgessBar4->Value = 50;
			// 
			// panel2
			// 
			this->panel2->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->panel2->Controls->Add(this->panel3);
			this->panel2->Location = System::Drawing::Point(0, 109);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(124, 202);
			this->panel2->TabIndex = 1;
			// 
			// DepartProductTypeItem
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::Transparent;
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->panel2);
			this->Name = L"DepartProductTypeItem";
			this->Size = System::Drawing::Size(124, 314);
			this->Load += gcnew System::EventHandler(this, &DepartProductTypeItem::OnLoad);
			this->panel1->ResumeLayout(false);
			this->panel3->ResumeLayout(false);
			this->panel2->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void OnLoad(System::Object^  sender, System::EventArgs^  e) {
				 this->panel1->BackColor = Color::FromArgb(165,0,0,0);
				 this->panel2->BackColor = Color::FromArgb(165,0,0,0);
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
};
}
