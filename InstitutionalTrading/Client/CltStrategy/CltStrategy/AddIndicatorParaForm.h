#pragma once
#include "DataManageEx.h"
#include "Tools.h"
#include "..\CommonClasses\ConvEnum.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace CltStrategy {

	/// <summary>
	/// Summary for AddIndicatorParaForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class AddIndicatorParaForm : public System::Windows::Forms::Form
	{
	public:
		AddIndicatorParaForm(CIndicatorDataEx* pIndicator,int nParaCount)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_pDataInfo = CDataManageEx::NewInstance();
			m_IndicatorPara = new IndicatorPara;
			memset(m_IndicatorPara,0,sizeof(IndicatorPara));
			m_bOprationType=true;
			m_IndicatorData = pIndicator;
			m_nParaCount=nParaCount;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~AddIndicatorParaForm()
		{
			if (components)
			{
				delete components;
			}
			if ( NULL != m_IndicatorPara )
			{
				delete m_IndicatorPara;
				m_IndicatorPara = NULL;
			}
			CDataManageEx::DestroyInstance();
			/*if ( NULL != m_IndicatorData )
			{
				delete m_IndicatorData;
				m_IndicatorData = NULL;
			}*/
		}
	private: int m_nParaCount;
	private: CIndicatorData*     m_IndicatorData;
    private: CDataManageEx*     m_pDataInfo;
	private: IndicatorPara*   m_IndicatorPara;
	private: bool             m_bOprationType;//新增参数true,修改参数

	private: System::Windows::Forms::Label^  label1;
	protected: 
	private: System::Windows::Forms::TextBox^  textBox_name;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::ComboBox^  comboBox_type;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::TextBox^  textBox_default;
	private: System::Windows::Forms::Button^  button_ok;
	private: System::Windows::Forms::Button^  button_cancel;


	public: void SetParaInfo(const IndicatorPara& indiPara)
			 {
				*m_IndicatorPara = indiPara;
				m_bOprationType=false;
				this->textBox_name->Enabled = false;
			 }

	public:IndicatorPara* GetParaInfo()
			 {

				 return m_IndicatorPara;
			 }


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
			this->textBox_name = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->comboBox_type = (gcnew System::Windows::Forms::ComboBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->textBox_default = (gcnew System::Windows::Forms::TextBox());
			this->button_ok = (gcnew System::Windows::Forms::Button());
			this->button_cancel = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(28, 34);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(41, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"名称：";
			// 
			// textBox_name
			// 
			this->textBox_name->Location = System::Drawing::Point(92, 25);
			this->textBox_name->Name = L"textBox_name";
			this->textBox_name->Size = System::Drawing::Size(132, 21);
			this->textBox_name->TabIndex = 1;
			this->textBox_name->Text = L"p1";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(28, 77);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(41, 12);
			this->label2->TabIndex = 2;
			this->label2->Text = L"类型：";
			// 
			// comboBox_type
			// 
			this->comboBox_type->FormattingEnabled = true;
			this->comboBox_type->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"int", L"double", L"bool"});
			this->comboBox_type->Location = System::Drawing::Point(92, 69);
			this->comboBox_type->Name = L"comboBox_type";
			this->comboBox_type->Size = System::Drawing::Size(132, 20);
			this->comboBox_type->TabIndex = 3;
			this->comboBox_type->Text = L"int";
			this->comboBox_type->SelectedIndexChanged += gcnew System::EventHandler(this, &AddIndicatorParaForm::comboBox_type_SelectedIndexChanged);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(28, 117);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(53, 12);
			this->label3->TabIndex = 4;
			this->label3->Text = L"默认值：";
			// 
			// textBox_default
			// 
			this->textBox_default->Location = System::Drawing::Point(92, 108);
			this->textBox_default->Name = L"textBox_default";
			this->textBox_default->Size = System::Drawing::Size(132, 21);
			this->textBox_default->TabIndex = 5;
			this->textBox_default->Text = L"1";
			// 
			// button_ok
			// 
			this->button_ok->Location = System::Drawing::Point(30, 191);
			this->button_ok->Name = L"button_ok";
			this->button_ok->Size = System::Drawing::Size(75, 23);
			this->button_ok->TabIndex = 6;
			this->button_ok->Text = L"确定";
			this->button_ok->UseVisualStyleBackColor = true;
			this->button_ok->Click += gcnew System::EventHandler(this, &AddIndicatorParaForm::button_ok_Click);
			// 
			// button_cancel
			// 
			this->button_cancel->Location = System::Drawing::Point(167, 191);
			this->button_cancel->Name = L"button_cancel";
			this->button_cancel->Size = System::Drawing::Size(75, 23);
			this->button_cancel->TabIndex = 7;
			this->button_cancel->Text = L"取消";
			this->button_cancel->UseVisualStyleBackColor = true;
			this->button_cancel->Click += gcnew System::EventHandler(this, &AddIndicatorParaForm::button_cancel_Click);
			// 
			// AddIndicatorParaForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(280, 266);
			this->Controls->Add(this->button_cancel);
			this->Controls->Add(this->button_ok);
			this->Controls->Add(this->textBox_default);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->comboBox_type);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->textBox_name);
			this->Controls->Add(this->label1);
			this->Name = L"AddIndicatorParaForm";
			this->Text = L"参数配置";
			this->Load += gcnew System::EventHandler(this, &AddIndicatorParaForm::AddIndicatorParaForm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void AddIndicatorParaForm_Load(System::Object^  sender, System::EventArgs^  e) 
			 {
				 this->CenterToParent();
				 if ( !m_bOprationType )
				 {
					 textBox_name->Text  = Tools::string2String(m_IndicatorPara->szName); 
					 /*char strField[128];
					 memset(strField,0,sizeof(strField));
					 sprintf(strField, "%d",m_IndicatorPara->eType);*/
					 comboBox_type->Text = Tools::string2String(IndiPara2String(m_IndicatorPara->eType));
					 textBox_default->Text  = Tools::string2String(m_IndicatorPara->szDefault);
				 }
				 else
				 {
					  char strtemp[32];
					  memset(strtemp,0,sizeof(strtemp));
					  sprintf( strtemp, "%s%d","p",m_nParaCount );
					 textBox_name->Text  = Tools::string2String(strtemp); 
				 }
			 }
private: System::Void button_ok_Click(System::Object^  sender, System::EventArgs^  e) 
		 {

			 if(m_bOprationType) {
				 if (!m_IndicatorData->CheckName(Tools::String2string(textBox_name->Text).c_str()))
				 {
					 MessageBox::Show("参数名称重复!");
					 return;
				 }
			 }

			  strncpy(m_IndicatorPara->szName , Tools::String2string(textBox_name->Text).c_str(),sizeof(m_IndicatorPara->szName)-1);
			  m_IndicatorPara->eType = IndiPara2Enum(Tools::String2string(comboBox_type->Text).c_str());
			  strncpy(m_IndicatorPara->szDefault , Tools::String2string(textBox_default->Text).c_str(),sizeof(m_IndicatorPara->szDefault)-1);

			  this->DialogResult = System::Windows::Forms::DialogResult::Yes;
			  this->Close();
			 
		 }
private: System::Void button_cancel_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 this->Close();
		 }
private: System::Void comboBox_type_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 m_IndicatorPara->eType = IndiPara2Enum(Tools::String2string(comboBox_type->Text).c_str());
			 switch(comboBox_type->SelectedIndex)
			 {
			 case 0:
				 textBox_default->Text ="1";
				 textBox_default->ImeMode = System::Windows::Forms::ImeMode::Disable;
				 break;
			 case 1:
				 textBox_default->Text ="1.0";
				 textBox_default->ImeMode = System::Windows::Forms::ImeMode::Disable;
				 break;
			 case 2:
				 textBox_default->Text ="false";
				 break;
			 default:
				 textBox_default->Text ="1";
				 break;
			 }
		 }
};
}
