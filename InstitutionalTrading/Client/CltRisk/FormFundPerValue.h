#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace CurrencyBox;

#include "RiskManageStruct.h"

namespace CltRisk {

	/// <summary>
	/// Summary for FormFundPerValue
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class FormFundPerValue : public System::Windows::Forms::Form
	{
	public:
		FormFundPerValue(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
            m_pNetFundParam = new NetFundParam;
            memset(m_pNetFundParam,0,sizeof(NetFundParam));
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormFundPerValue()
		{
			if (components)
			{
				delete components;
			}
            if(m_pNetFundParam)
            {
                delete m_pNetFundParam;
                m_pNetFundParam = NULL;
            }
		}
    private: System::Windows::Forms::Label^  label1;
    protected: 
    private: System::Windows::Forms::Label^  label2;
    private: System::Windows::Forms::Label^  label3;
    private: System::Windows::Forms::Label^  label4;
    private: System::Windows::Forms::Label^  label5;
    private: System::Windows::Forms::TextBox^  textBox_FundShareOut;
    private: System::Windows::Forms::TextBox^  textBox_FundShareIn;
    private: System::Windows::Forms::TextBox^  textBox_FundPerValueOut;

    private: System::Windows::Forms::ComboBox^  comboBox_FundPerValueIn;






    private: System::Windows::Forms::TextBox^  textBox_UnitPerValue;


    private: System::Windows::Forms::Button^  button1;
    private: System::Windows::Forms::Button^  button2;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;
    private: System::Windows::Forms::Splitter^  splitter1;
    private: ::CurrencyBox::CurrencyBox^  currencyBox_ShareOut;
    private: ::CurrencyBox::CurrencyBox^  currencyBox_ValueOut;
    private: ::CurrencyBox::CurrencyBox^  currencyBox_ShareIn;
             //     private: ::CurrencyBox::CurrencyBox^  currencyBox1;

        NetFundParam* m_pNetFundParam;

    public:
        void SetNetFundParam(NetFundParam* pNetFundParam)
        {
            memcpy(m_pNetFundParam,pNetFundParam,sizeof(NetFundParam));
        }
        void GetNetFundParam(NetFundParam& oNetFundParam)
        {
            memcpy(&oNetFundParam,m_pNetFundParam,sizeof(NetFundParam));
        }
#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->label3 = (gcnew System::Windows::Forms::Label());
            this->label4 = (gcnew System::Windows::Forms::Label());
            this->label5 = (gcnew System::Windows::Forms::Label());
            this->textBox_FundShareOut = (gcnew System::Windows::Forms::TextBox());
            this->textBox_FundShareIn = (gcnew System::Windows::Forms::TextBox());
            this->textBox_FundPerValueOut = (gcnew System::Windows::Forms::TextBox());
            this->comboBox_FundPerValueIn = (gcnew System::Windows::Forms::ComboBox());
            this->textBox_UnitPerValue = (gcnew System::Windows::Forms::TextBox());
            this->button1 = (gcnew System::Windows::Forms::Button());
            this->button2 = (gcnew System::Windows::Forms::Button());
            this->splitter1 = (gcnew System::Windows::Forms::Splitter());
            this->currencyBox_ShareOut = (gcnew ::CurrencyBox::CurrencyBox());
            this->currencyBox_ValueOut = (gcnew ::CurrencyBox::CurrencyBox());
            this->currencyBox_ShareIn = (gcnew ::CurrencyBox::CurrencyBox());
            this->SuspendLayout();
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(155, 26);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(29, 12);
            this->label1->TabIndex = 0;
            this->label1->Text = L"场外";
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Location = System::Drawing::Point(296, 26);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(65, 12);
            this->label2->TabIndex = 1;
            this->label2->Text = L"场内(期货)";
            // 
            // label3
            // 
            this->label3->AutoSize = true;
            this->label3->Location = System::Drawing::Point(58, 55);
            this->label3->Name = L"label3";
            this->label3->Size = System::Drawing::Size(29, 12);
            this->label3->TabIndex = 2;
            this->label3->Text = L"份额";
            // 
            // label4
            // 
            this->label4->AutoSize = true;
            this->label4->Location = System::Drawing::Point(34, 98);
            this->label4->Name = L"label4";
            this->label4->Size = System::Drawing::Size(53, 12);
            this->label4->TabIndex = 3;
            this->label4->Text = L"资产净值";
            // 
            // label5
            // 
            this->label5->AutoSize = true;
            this->label5->Location = System::Drawing::Point(34, 141);
            this->label5->Name = L"label5";
            this->label5->Size = System::Drawing::Size(53, 12);
            this->label5->TabIndex = 4;
            this->label5->Text = L"单位净值";
            // 
            // textBox_FundShareOut
            // 
            this->textBox_FundShareOut->Location = System::Drawing::Point(119, 52);
            this->textBox_FundShareOut->MaxLength = 18;
            this->textBox_FundShareOut->Name = L"textBox_FundShareOut";
            this->textBox_FundShareOut->Size = System::Drawing::Size(100, 21);
            this->textBox_FundShareOut->TabIndex = 5;
            this->textBox_FundShareOut->Visible = false;
            this->textBox_FundShareOut->TextChanged += gcnew System::EventHandler(this, &FormFundPerValue::textBox_TextChanged);
            this->textBox_FundShareOut->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &FormFundPerValue::textBox_KeyPress);
            // 
            // textBox_FundShareIn
            // 
            this->textBox_FundShareIn->Location = System::Drawing::Point(280, 52);
            this->textBox_FundShareIn->MaxLength = 18;
            this->textBox_FundShareIn->Name = L"textBox_FundShareIn";
            this->textBox_FundShareIn->Size = System::Drawing::Size(100, 21);
            this->textBox_FundShareIn->TabIndex = 6;
            this->textBox_FundShareIn->Visible = false;
            this->textBox_FundShareIn->TextChanged += gcnew System::EventHandler(this, &FormFundPerValue::textBox_TextChanged);
            this->textBox_FundShareIn->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &FormFundPerValue::textBox_KeyPress);
            // 
            // textBox_FundPerValueOut
            // 
            this->textBox_FundPerValueOut->Location = System::Drawing::Point(119, 95);
            this->textBox_FundPerValueOut->MaxLength = 18;
            this->textBox_FundPerValueOut->Name = L"textBox_FundPerValueOut";
            this->textBox_FundPerValueOut->Size = System::Drawing::Size(100, 21);
            this->textBox_FundPerValueOut->TabIndex = 7;
            this->textBox_FundPerValueOut->Visible = false;
            this->textBox_FundPerValueOut->TextChanged += gcnew System::EventHandler(this, &FormFundPerValue::textBox_TextChanged);
            this->textBox_FundPerValueOut->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &FormFundPerValue::textBox_KeyPress);
            // 
            // comboBox_FundPerValueIn
            // 
            this->comboBox_FundPerValueIn->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
            this->comboBox_FundPerValueIn->FormattingEnabled = true;
            this->comboBox_FundPerValueIn->Items->AddRange(gcnew cli::array< System::Object^  >(1) {L"动态权益"});
            this->comboBox_FundPerValueIn->Location = System::Drawing::Point(280, 95);
            this->comboBox_FundPerValueIn->Name = L"comboBox_FundPerValueIn";
            this->comboBox_FundPerValueIn->Size = System::Drawing::Size(100, 20);
            this->comboBox_FundPerValueIn->TabIndex = 8;
            // 
            // textBox_UnitPerValue
            // 
            this->textBox_UnitPerValue->Enabled = false;
            this->textBox_UnitPerValue->Location = System::Drawing::Point(119, 138);
            this->textBox_UnitPerValue->Name = L"textBox_UnitPerValue";
            this->textBox_UnitPerValue->Text = L"0.00";
            this->textBox_UnitPerValue->Size = System::Drawing::Size(100, 21);
            this->textBox_UnitPerValue->TabIndex = 9;
            // 
            // button1
            // 
            this->button1->Location = System::Drawing::Point(112, 193);
            this->button1->Name = L"button1";
            this->button1->Size = System::Drawing::Size(75, 23);
            this->button1->TabIndex = 10;
            this->button1->Text = L"确定";
            this->button1->UseVisualStyleBackColor = true;
            this->button1->Click += gcnew System::EventHandler(this, &FormFundPerValue::button_OK_Click);
            // 
            // button2
            // 
            this->button2->Location = System::Drawing::Point(225, 193);
            this->button2->Name = L"button2";
            this->button2->Size = System::Drawing::Size(75, 23);
            this->button2->TabIndex = 11;
            this->button2->Text = L"取消";
            this->button2->UseVisualStyleBackColor = true;
            this->button2->Click += gcnew System::EventHandler(this, &FormFundPerValue::button_Cancel_Click);
            // 
            // splitter1
            // 
            this->splitter1->Location = System::Drawing::Point(0, 0);
            this->splitter1->Name = L"splitter1";
            this->splitter1->Size = System::Drawing::Size(3, 233);
            this->splitter1->TabIndex = 12;
            this->splitter1->TabStop = false;
            // 
            // currencyBox_ShareOut
            // 
            this->currencyBox_ShareOut->BackColor = System::Drawing::SystemColors::Window;
            this->currencyBox_ShareOut->ColorNumerosNegativos = System::Drawing::Color::Red;
            this->currencyBox_ShareOut->Cursor = System::Windows::Forms::Cursors::Default;
            this->currencyBox_ShareOut->Location = System::Drawing::Point(119, 53);
            this->currencyBox_ShareOut->Name = L"currencyBox_ShareOut";
            this->currencyBox_ShareOut->Size = System::Drawing::Size(100, 21);
            this->currencyBox_ShareOut->TabIndex = 13;
            this->currencyBox_ShareOut->TextChanged += gcnew System::EventHandler(this, &FormFundPerValue::textBox_TextChanged);
            // 
            // currencyBox_ValueOut
            // 
            this->currencyBox_ValueOut->BackColor = System::Drawing::SystemColors::Window;
            this->currencyBox_ValueOut->ColorNumerosNegativos = System::Drawing::Color::Red;
            this->currencyBox_ValueOut->Cursor = System::Windows::Forms::Cursors::Default;
            this->currencyBox_ValueOut->Location = System::Drawing::Point(119, 96);
            this->currencyBox_ValueOut->Name = L"currencyBox_ValueOut";
            this->currencyBox_ValueOut->Size = System::Drawing::Size(100, 21);
            this->currencyBox_ValueOut->TabIndex = 14;
            this->currencyBox_ValueOut->TextChanged += gcnew System::EventHandler(this, &FormFundPerValue::textBox_TextChanged);
            // 
            // currencyBox_ShareIn
            // 
            this->currencyBox_ShareIn->BackColor = System::Drawing::SystemColors::Window;
            this->currencyBox_ShareIn->ColorNumerosNegativos = System::Drawing::Color::Red;
            this->currencyBox_ShareIn->Cursor = System::Windows::Forms::Cursors::Default;
            this->currencyBox_ShareIn->Location = System::Drawing::Point(280, 53);
            this->currencyBox_ShareIn->Name = L"currencyBox_ShareIn";
            this->currencyBox_ShareIn->Size = System::Drawing::Size(100, 21);
            this->currencyBox_ShareIn->TabIndex = 15;
            this->currencyBox_ShareIn->TextChanged += gcnew System::EventHandler(this, &FormFundPerValue::textBox_TextChanged);
            // 
            // FormFundPerValue
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(413, 233);
            this->Controls->Add(this->currencyBox_ShareIn);
            this->Controls->Add(this->currencyBox_ValueOut);
            this->Controls->Add(this->currencyBox_ShareOut);
            this->Controls->Add(this->splitter1);
            this->Controls->Add(this->button2);
            this->Controls->Add(this->button1);
            this->Controls->Add(this->textBox_UnitPerValue);
            this->Controls->Add(this->comboBox_FundPerValueIn);
            this->Controls->Add(this->textBox_FundPerValueOut);
            this->Controls->Add(this->textBox_FundShareIn);
            this->Controls->Add(this->textBox_FundShareOut);
            this->Controls->Add(this->label5);
            this->Controls->Add(this->label4);
            this->Controls->Add(this->label3);
            this->Controls->Add(this->label2);
            this->Controls->Add(this->label1);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Name = L"FormFundPerValue";
            this->Text = L"基金净值参数设置";
            this->Load += gcnew System::EventHandler(this, &FormFundPerValue::FormFundPerValue_Load);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
private: System::Void FormFundPerValue_Load(System::Object^  sender, System::EventArgs^  e) {
             this->CenterToParent();

             currencyBox_ShareOut->DigitosDespuesDelSimboloDecimal = 0;
             currencyBox_ShareIn->DigitosDespuesDelSimboloDecimal = 0;

             currencyBox_ShareOut->Text = m_pNetFundParam->dOuterVolumn.ToString("N");
             currencyBox_ShareIn->Text = m_pNetFundParam->dInnerVolumn.ToString("N"); 
             currencyBox_ValueOut->Text = m_pNetFundParam->dOuterNetAsset.ToString("N");
             comboBox_FundPerValueIn->SelectedIndex = 0;
         }
private: System::Void button_OK_Click(System::Object^  sender, System::EventArgs^  e) {
             if(String::IsNullOrEmpty(currencyBox_ShareOut->Text))
             {
                 MessageBox::Show("请输入场外份额");
                 return;
             }
             if(String::IsNullOrEmpty(currencyBox_ShareIn->Text))
             {
                 MessageBox::Show("请输入场内份额");
                 return;
             }
             if(String::IsNullOrEmpty(currencyBox_ValueOut->Text))
             {
                 MessageBox::Show("请输入场外资产净值");
                 return;
             }

             m_pNetFundParam->dOuterVolumn = Convert::ToDouble(currencyBox_ShareOut->Text);//Convert::ToDouble(textBox_FundShareOut->Text);
             m_pNetFundParam->dInnerVolumn = Convert::ToDouble(currencyBox_ShareIn->Text);//Convert::ToDouble(textBox_FundShareIn->Text);
             m_pNetFundParam->dOuterNetAsset = Convert::ToDouble(currencyBox_ValueOut->Text);//Convert::ToDouble(textBox_FundPerValueOut->Text);
             comboBox_FundPerValueIn->SelectedIndex = comboBox_FundPerValueIn->SelectedIndex;

             this->DialogResult = ::DialogResult::OK;
         }
private: System::Void button_Cancel_Click(System::Object^  sender, System::EventArgs^  e) {
             this->Close();
         }
private: System::Void textBox_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
             System::Windows::Forms::TextBox^ textBox = (TextBox^)sender;
             int nLen = textBox->Text->Length;
             int nGer = textBox->Text->LastIndexOf(".");

             if(nGer>0 && (nLen-nGer > 2) && e->KeyChar!=8)
                 e->Handled = true;

             if(e->KeyChar=='.' && textBox->Text->IndexOf(".")!=-1)  
             {  
                 e->Handled=true;  
             }  

             if(!((e->KeyChar>=48 && e->KeyChar<=57) || e->KeyChar=='.' || e->KeyChar==8)) 
             {  
                 e->Handled=true;  
             }  
         }
private: System::Void textBox_TextChanged(System::Object^  sender, System::EventArgs^  e) {

             if(String::IsNullOrEmpty(currencyBox_ShareOut->Text) || 0 == (int)Convert::ToDouble(currencyBox_ShareOut->Text))
                 textBox_UnitPerValue->Text = "0.00";
             else
             {
                 double dOutValue = Convert::ToDouble(String::IsNullOrEmpty(currencyBox_ValueOut->Text)?"0.00":currencyBox_ValueOut->Text);
                 double dOutShare = Convert::ToDouble(String::IsNullOrEmpty(currencyBox_ShareOut->Text)?"0.00":currencyBox_ShareOut->Text);

                 double dValue = dOutValue/dOutShare;
                 textBox_UnitPerValue->Text = dValue.ToString("N");
             }
         }
};
}
