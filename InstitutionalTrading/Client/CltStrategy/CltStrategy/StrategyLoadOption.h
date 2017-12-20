#pragma once
#include "Stdafx.h"
#include "DataManageEx.h"
#include "Tools.h"
#include "StrategyPlanCfg.h"
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace CltStrategy {

	/// <summary>
	/// Summary for StrategyLoadOption
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class StrategyLoadOption : public System::Windows::Forms::Form
	{
	public:
		StrategyLoadOption(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_pDataInfo = CDataManageEx::NewInstance();
			m_StrategyInstance = new SStrategyInstance();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~StrategyLoadOption()
		{
			if (components)
			{
				delete components;
			}
			CDataManageEx::DestroyInstance();
		}
	private:SStrategyInstance*	m_StrategyInstance;
	private: CDataManageEx*     m_pDataInfo;
	private: System::Windows::Forms::Label^  label1;
	protected: 
	private: System::Windows::Forms::ComboBox^  comboBox_name;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Button^  button_ok;
	private: System::Windows::Forms::Button^  button_cancel;

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
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->comboBox_name = (gcnew System::Windows::Forms::ComboBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->button_ok = (gcnew System::Windows::Forms::Button());
			this->button_cancel = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(53, 54);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(41, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"策略名";
			// 
			// comboBox_name
			// 
			this->comboBox_name->FormattingEnabled = true;
			this->comboBox_name->Location = System::Drawing::Point(124, 46);
			this->comboBox_name->Name = L"comboBox_name";
			this->comboBox_name->Size = System::Drawing::Size(151, 20);
			this->comboBox_name->TabIndex = 1;
			this->comboBox_name->SelectedIndexChanged += gcnew System::EventHandler(this, &StrategyLoadOption::comboBox_name_SelectedIndexChanged);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(55, 95);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(53, 12);
			this->label2->TabIndex = 2;
			this->label2->Text = L"策略描述";
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(124, 95);
			this->textBox1->Multiline = true;
			this->textBox1->Name = L"textBox1";
			this->textBox1->ReadOnly = true;
			this->textBox1->Size = System::Drawing::Size(151, 121);
			this->textBox1->TabIndex = 3;
			// 
			// button_ok
			// 
			this->button_ok->Location = System::Drawing::Point(57, 283);
			this->button_ok->Name = L"button_ok";
			this->button_ok->Size = System::Drawing::Size(75, 23);
			this->button_ok->TabIndex = 4;
			this->button_ok->Text = L"确定";
			this->button_ok->UseVisualStyleBackColor = true;
			this->button_ok->Click += gcnew System::EventHandler(this, &StrategyLoadOption::button_ok_Click);
			// 
			// button_cancel
			// 
			this->button_cancel->Location = System::Drawing::Point(200, 283);
			this->button_cancel->Name = L"button_cancel";
			this->button_cancel->Size = System::Drawing::Size(75, 23);
			this->button_cancel->TabIndex = 5;
			this->button_cancel->Text = L"取消";
			this->button_cancel->UseVisualStyleBackColor = true;
			this->button_cancel->Click += gcnew System::EventHandler(this, &StrategyLoadOption::button_cancel_Click);
			// 
			// StrategyLoadOption
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(351, 354);
			this->Controls->Add(this->button_cancel);
			this->Controls->Add(this->button_ok);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->comboBox_name);
			this->Controls->Add(this->label1);
			this->Name = L"StrategyLoadOption";
			this->Text = L"策略加载选项";
			this->Load += gcnew System::EventHandler(this, &StrategyLoadOption::StrategyLoadOption_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	public:SStrategyInstance* GetStrategyInstance()
		   {

			   return m_StrategyInstance;
		   }
	private: System::Void StrategyLoadOption_Load(System::Object^  sender, System::EventArgs^  e) 
			 {
				 this->CenterToParent();
				 std::map<std::string, CStrategyDataEx>   MapStrategy;
				 m_pDataInfo->GetAllStrategy_Download(MapStrategy);
				 std::map<std::string, CStrategyDataEx>::iterator its = MapStrategy.begin();
				 for ( its; its!=MapStrategy.end(); its++)
				 {
					  if(its->second.GetServerUseStatus()==1)
					     comboBox_name->Items->Add(Tools::string2String(its->first.c_str()) );
				 }
			 }
private: System::Void comboBox_name_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 string sname =Tools::String2string(comboBox_name->Text);
			 CStrategyDataEx field;
			if(m_pDataInfo->GetDownloadStrategy(sname,field) )
			{
			     this->textBox1->Text =Tools::string2String( field.GetDescribe().c_str());
			}

		 }
private: System::Void button_ok_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 SStrategyInstance instance = {0};
			 if (String::IsNullOrEmpty(comboBox_name->Text))
			 {
				 MessageBox::Show("请选择策略!");  
				 return;
			 }
			 string sname =Tools::String2string(comboBox_name->Text);
			 strncpy(instance.strategyName, sname.c_str(), sizeof(instance.strategyName)-1);

			 StrategyPlanCfg^ dlg = gcnew StrategyPlanCfg(&instance,true);//新增一个策略方案

			 System::Windows::Forms::DialogResult nRet = dlg->ShowDialog(this);
			 *m_StrategyInstance=*dlg->GetStrategyInstance();	
			 this->DialogResult = nRet;
			 this->Close();
		 }
private: System::Void button_cancel_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 this->DialogResult = System::Windows::Forms::DialogResult::No;
			 this->Close();
		 }
};
}
