#pragma once

#include "Tools.h"
#include "DataManageEx.h"
#include "FormStrategyTester.h"
#include "FormProgress.h"
#include "TestManager.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Runtime::InteropServices;


namespace StrategyTester {

	/// <summary>
	/// Summary for FormMain
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class FormMain : public System::Windows::Forms::Form
	{
	public:
		FormMain(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_pDataInfo = CDataManageEx::NewInstance();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormMain()
		{
			CDataManageEx::DestroyInstance();
			if (components)
			{
				delete components;
			}
		}
	private: CDataManageEx*     m_pDataInfo;
	private: System::Windows::Forms::Button^  button_Start;
	private: System::Windows::Forms::Button^  button_LoadFiles;
	private: System::Windows::Forms::TextBox^  textBox_quot;
	protected: 

	protected: 


	private: System::Windows::Forms::Button^  button_selectQuot;
	private: System::Windows::Forms::Button^  button_selectInstrument;
	private: System::Windows::Forms::TextBox^  textBox_instrument;



	private: System::Windows::Forms::Button^  button_selectRate;
	private: System::Windows::Forms::TextBox^  textBox_rate;


	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;

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
			this->button_Start = (gcnew System::Windows::Forms::Button());
			this->button_LoadFiles = (gcnew System::Windows::Forms::Button());
			this->textBox_quot = (gcnew System::Windows::Forms::TextBox());
			this->button_selectQuot = (gcnew System::Windows::Forms::Button());
			this->button_selectInstrument = (gcnew System::Windows::Forms::Button());
			this->textBox_instrument = (gcnew System::Windows::Forms::TextBox());
			this->button_selectRate = (gcnew System::Windows::Forms::Button());
			this->textBox_rate = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// button_Start
			// 
			this->button_Start->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->button_Start->Location = System::Drawing::Point(538, 156);
			this->button_Start->Name = L"button_Start";
			this->button_Start->Size = System::Drawing::Size(82, 51);
			this->button_Start->TabIndex = 7;
			this->button_Start->Text = L"启动测试";
			this->button_Start->UseVisualStyleBackColor = true;
			this->button_Start->Click += gcnew System::EventHandler(this, &FormMain::button_Start_Click);
			// 
			// button_LoadFiles
			// 
			this->button_LoadFiles->Location = System::Drawing::Point(289, 98);
			this->button_LoadFiles->Name = L"button_LoadFiles";
			this->button_LoadFiles->Size = System::Drawing::Size(72, 21);
			this->button_LoadFiles->TabIndex = 6;
			this->button_LoadFiles->Text = L"加载文件";
			this->button_LoadFiles->UseVisualStyleBackColor = true;
			this->button_LoadFiles->Click += gcnew System::EventHandler(this, &FormMain::button_LoadFiles_Click);
			// 
			// textBox_quot
			// 
			this->textBox_quot->Location = System::Drawing::Point(87, 17);
			this->textBox_quot->MaxLength = 256;
			this->textBox_quot->Name = L"textBox_quot";
			this->textBox_quot->Size = System::Drawing::Size(467, 21);
			this->textBox_quot->TabIndex = 0;
			// 
			// button_selectQuot
			// 
			this->button_selectQuot->Location = System::Drawing::Point(560, 17);
			this->button_selectQuot->Name = L"button_selectQuot";
			this->button_selectQuot->Size = System::Drawing::Size(60, 21);
			this->button_selectQuot->TabIndex = 1;
			this->button_selectQuot->Text = L"选择";
			this->button_selectQuot->UseVisualStyleBackColor = true;
			this->button_selectQuot->Click += gcnew System::EventHandler(this, &FormMain::button_selectQuot_Click);
			// 
			// button_selectInstrument
			// 
			this->button_selectInstrument->Location = System::Drawing::Point(560, 44);
			this->button_selectInstrument->Name = L"button_selectInstrument";
			this->button_selectInstrument->Size = System::Drawing::Size(60, 21);
			this->button_selectInstrument->TabIndex = 3;
			this->button_selectInstrument->Text = L"选择";
			this->button_selectInstrument->UseVisualStyleBackColor = true;
			this->button_selectInstrument->Click += gcnew System::EventHandler(this, &FormMain::button_selectInstrument_Click);
			// 
			// textBox_instrument
			// 
			this->textBox_instrument->Location = System::Drawing::Point(87, 44);
			this->textBox_instrument->MaxLength = 256;
			this->textBox_instrument->Name = L"textBox_instrument";
			this->textBox_instrument->Size = System::Drawing::Size(467, 21);
			this->textBox_instrument->TabIndex = 2;
			// 
			// button_selectRate
			// 
			this->button_selectRate->Location = System::Drawing::Point(560, 71);
			this->button_selectRate->Name = L"button_selectRate";
			this->button_selectRate->Size = System::Drawing::Size(60, 21);
			this->button_selectRate->TabIndex = 5;
			this->button_selectRate->Text = L"选择";
			this->button_selectRate->UseVisualStyleBackColor = true;
			this->button_selectRate->Click += gcnew System::EventHandler(this, &FormMain::button_selectRate_Click);
			// 
			// textBox_rate
			// 
			this->textBox_rate->Location = System::Drawing::Point(87, 71);
			this->textBox_rate->MaxLength = 256;
			this->textBox_rate->Name = L"textBox_rate";
			this->textBox_rate->Size = System::Drawing::Size(467, 21);
			this->textBox_rate->TabIndex = 4;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(17, 21);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(53, 12);
			this->label1->TabIndex = 29;
			this->label1->Text = L"行情文件";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(17, 48);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(53, 12);
			this->label2->TabIndex = 30;
			this->label2->Text = L"合约文件";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(17, 75);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(53, 12);
			this->label3->TabIndex = 30;
			this->label3->Text = L"费率文件";
			// 
			// FormMain
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(638, 225);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->button_selectRate);
			this->Controls->Add(this->textBox_rate);
			this->Controls->Add(this->button_selectInstrument);
			this->Controls->Add(this->textBox_instrument);
			this->Controls->Add(this->button_selectQuot);
			this->Controls->Add(this->textBox_quot);
			this->Controls->Add(this->button_Start);
			this->Controls->Add(this->button_LoadFiles);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"FormMain";
			this->Text = L"策略测试";
			this->Load += gcnew System::EventHandler(this, &FormMain::FormMain_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void button_LoadFiles_Click(System::Object^  sender, System::EventArgs^  e) {
			 //m_pDataInfo->ImportQuot(Tools::String2string(textBox_quot->Text));
			 m_pDataInfo->ImportInstrument(Tools::String2string(textBox_instrument->Text));
			 m_pDataInfo->ImportRate(Tools::String2string(textBox_rate->Text));
			 MessageBox::Show("文件加载完毕。", "信息", MessageBoxButtons::OK);
		 }
private: System::Void button_Start_Click(System::Object^  sender, System::EventArgs^  e) {
			 FormStrategyTester^ dlg = gcnew FormStrategyTester();
			 System::Windows::Forms::DialogResult nRet = dlg->ShowDialog();
			 if(nRet == System::Windows::Forms::DialogResult::OK) {
                 
				 // 显示策略结果显示页面

				 if(CTestManager::sm_pTestEntity != NULL) {
					 FormProgress^ frmProgress = gcnew FormProgress(CTestManager::sm_pTestEntity);
					 //_SetMsChartFormHandle(Marshal::GetIUnknownForObject(oMsChartForm).ToPointer());
					 CTestManager::sm_pTestEntity->Start();
					 frmProgress->ShowDialog();
				 }

				 // 之后关闭主窗体
				 this->DialogResult = System::Windows::Forms::DialogResult::OK;
				 this->Close();
			 }
			 else if(nRet == System::Windows::Forms::DialogResult::Abort) {
				 // 终止程序运行
				 this->DialogResult = System::Windows::Forms::DialogResult::Abort;
				 this->Close();
			 }
		 }
private: System::Void button_selectQuot_Click(System::Object^  sender, System::EventArgs^  e) {
			System::Windows::Forms::OpenFileDialog^ dlg = gcnew OpenFileDialog();
			dlg->Title ="选择行情文件";
			dlg->Filter = "csv文件(*.csv)|*.csv";
			if(dlg->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
				textBox_quot->Text =dlg->FileName;
				_SetQuotFile(Tools::String2string(dlg->FileName));
			}
		 }
private: System::Void button_selectInstrument_Click(System::Object^  sender, System::EventArgs^  e) {
			System::Windows::Forms::OpenFileDialog^ dlg = gcnew OpenFileDialog();
			dlg->Title ="选择合约文件";
			dlg->Filter = "txt文件(*.txt)|*.txt";
			if(dlg->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
				textBox_instrument->Text =dlg->FileName;
			}
		 }
private: System::Void button_selectRate_Click(System::Object^  sender, System::EventArgs^  e) {
			System::Windows::Forms::OpenFileDialog^ dlg = gcnew OpenFileDialog();
			dlg->Title ="选择手续费、保证金文件";
			dlg->Filter = "xml文件(*.xml)|*.xml";
			if(dlg->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
				textBox_rate->Text =dlg->FileName;
			}
		 }
private: System::Void FormMain_Load(System::Object^  sender, System::EventArgs^  e) {
			 this->CenterToScreen();
		 }
};
}
