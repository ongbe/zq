#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "CDataInfo.h"
#include "Tools.h"
#include "CommonStruct.h"


namespace CltServerMgr {

	/// <summary>
	/// Summary for FundOprForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class FundOprForm : public System::Windows::Forms::Form
	{
	public:
		FundOprForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
            m_pType = new eInOut;
            m_pCurFundInOut = new sFundInOut;
            m_pDataInfo = CDataInfo::NewInstance();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FundOprForm()
		{
			if (components)
			{
				delete components;
			}

            if ( NULL != m_pType )
            {
                delete m_pType;
                m_pType = NULL;
            }

            if ( NULL != m_pCurFundInOut )
            {
                delete m_pCurFundInOut;
                m_pCurFundInOut = NULL;
            }
		}


    private: CDataInfo* m_pDataInfo;
    private: eInOut* m_pType;
    private: sFundInOut* m_pCurFundInOut;   

    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::Label^  label2;
    private: System::Windows::Forms::Label^  label3;
    private: System::Windows::Forms::Label^  label4;
    private: System::Windows::Forms::Label^  label_OprType;
    private: System::Windows::Forms::ComboBox^  comboBox_ID;
    private: System::Windows::Forms::TextBox^  textBox_Fund;
    private: System::Windows::Forms::TextBox^  textBox_Desc;
    private: System::Windows::Forms::Button^  button_OK;
    private: System::Windows::Forms::Button^  button_Cancel;
	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

    public:
        void SetOperType(eInOut eType)
        {
            *m_pType = eType;
        }

        sFundInOut* GetCurFundInOut()
        {
            return m_pCurFundInOut;
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
            this->label_OprType = (gcnew System::Windows::Forms::Label());
            this->comboBox_ID = (gcnew System::Windows::Forms::ComboBox());
            this->textBox_Fund = (gcnew System::Windows::Forms::TextBox());
            this->textBox_Desc = (gcnew System::Windows::Forms::TextBox());
            this->button_OK = (gcnew System::Windows::Forms::Button());
            this->button_Cancel = (gcnew System::Windows::Forms::Button());
            this->SuspendLayout();
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(46, 24);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(65, 12);
            this->label1->TabIndex = 0;
            this->label1->Text = L"操作类型：";
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Location = System::Drawing::Point(32, 57);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(77, 12);
            this->label2->TabIndex = 1;
            this->label2->Text = L"交易员账号：";
            // 
            // label3
            // 
            this->label3->AutoSize = true;
            this->label3->Location = System::Drawing::Point(68, 90);
            this->label3->Name = L"label3";
            this->label3->Size = System::Drawing::Size(41, 12);
            this->label3->TabIndex = 2;
            this->label3->Text = L"金额：";
            // 
            // label4
            // 
            this->label4->AutoSize = true;
            this->label4->Location = System::Drawing::Point(68, 123);
            this->label4->Name = L"label4";
            this->label4->Size = System::Drawing::Size(41, 12);
            this->label4->TabIndex = 3;
            this->label4->Text = L"备注：";
            // 
            // label_OprType
            // 
            this->label_OprType->AutoSize = true;
            this->label_OprType->Location = System::Drawing::Point(110, 24);
            this->label_OprType->Name = L"label_OprType";
            this->label_OprType->Size = System::Drawing::Size(65, 12);
            this->label_OprType->TabIndex = 4;
            this->label_OprType->Text = L"交易员入金";
            // 
            // comboBox_ID
            // 
            this->comboBox_ID->FormattingEnabled = true;
            this->comboBox_ID->Location = System::Drawing::Point(110, 54);
            this->comboBox_ID->Name = L"comboBox_ID";
            this->comboBox_ID->Size = System::Drawing::Size(121, 20);
            this->comboBox_ID->TabIndex = 5;
            // 
            // textBox_Fund
            // 
            this->textBox_Fund->Location = System::Drawing::Point(110, 87);
            this->textBox_Fund->MaxLength = 13;
            this->textBox_Fund->Name = L"textBox_Fund";
            this->textBox_Fund->Size = System::Drawing::Size(121, 21);
            this->textBox_Fund->TabIndex = 6;
            this->textBox_Fund->TextChanged += gcnew System::EventHandler(this, &FundOprForm::textBox_Fund_TextChanged);
            this->textBox_Fund->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &FundOprForm::textBox_Fund_KeyPress);
            // 
            // textBox_Desc
            // 
            this->textBox_Desc->Location = System::Drawing::Point(110, 120);
            this->textBox_Desc->MaxLength = 32;
            this->textBox_Desc->Multiline = true;
            this->textBox_Desc->Name = L"textBox_Desc";
            this->textBox_Desc->Size = System::Drawing::Size(121, 105);
            this->textBox_Desc->TabIndex = 7;
            // 
            // button_OK
            // 
            this->button_OK->Location = System::Drawing::Point(48, 254);
            this->button_OK->Name = L"button_OK";
            this->button_OK->Size = System::Drawing::Size(75, 23);
            this->button_OK->TabIndex = 8;
            this->button_OK->Text = L"确定";
            this->button_OK->UseVisualStyleBackColor = true;
            this->button_OK->Click += gcnew System::EventHandler(this, &FundOprForm::button_OK_Click);
            // 
            // button_Cancel
            // 
            this->button_Cancel->Location = System::Drawing::Point(159, 254);
            this->button_Cancel->Name = L"button_Cancel";
            this->button_Cancel->Size = System::Drawing::Size(75, 23);
            this->button_Cancel->TabIndex = 9;
            this->button_Cancel->Text = L"取消";
            this->button_Cancel->UseVisualStyleBackColor = true;
            this->button_Cancel->Click += gcnew System::EventHandler(this, &FundOprForm::button_Cancel_Click);
            // 
            // FundOprForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(292, 292);
            this->Controls->Add(this->button_Cancel);
            this->Controls->Add(this->button_OK);
            this->Controls->Add(this->textBox_Desc);
            this->Controls->Add(this->textBox_Fund);
            this->Controls->Add(this->comboBox_ID);
            this->Controls->Add(this->label_OprType);
            this->Controls->Add(this->label4);
            this->Controls->Add(this->label3);
            this->Controls->Add(this->label2);
            this->Controls->Add(this->label1);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Name = L"FundOprForm";
            this->Text = L"入金";
            this->Load += gcnew System::EventHandler(this, &FundOprForm::FundOprForm_Load);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
    private: void LoadUser()
             {
                 comboBox_ID->Items->Clear();

                 std::map<int, UserInfo> mapUserInfo;
                 m_pDataInfo->GetUserInfo(mapUserInfo);

                 std::map<int, UserInfo>::iterator itUserInfo = mapUserInfo.begin();
                 for (;itUserInfo!=mapUserInfo.end();itUserInfo++)
                 {
                     if (itUserInfo->second.nUserType == USER_TYPE_TRADER)
                        comboBox_ID->Items->Add(Tools::string2String(itUserInfo->second.szAccount));   
                 }
             }
    private: System::Void FundOprForm_Load(System::Object^  sender, System::EventArgs^  e) {
                 this->CenterToParent();

                 if ( *m_pType == FundIN )
                 {
                     this->Text = "入金";
                     this->label_OprType->Text = "交易员入金";            
                 }
                 else
                 {
                     this->Text = "出金";
                     this->label_OprType->Text = "交易员出金";  
                 }

                 LoadUser();
             }
private: System::Void button_OK_Click(System::Object^  sender, System::EventArgs^  e) {
             if (String::IsNullOrEmpty(comboBox_ID->Text))
             {
                 MessageBox::Show("账户号不能为空！");
                 return;
             }

             if(String::IsNullOrEmpty(textBox_Fund->Text))
             {
                 MessageBox::Show("金额不能为空！");
                 return;
             }

             if(::DialogResult::Cancel ==  MessageBox::Show("请确保主帐号资金充足，如果资金溢出会导致下单失败。确认吗？", "提示", MessageBoxButtons::OKCancel, MessageBoxIcon::Warning))
                 return;

             this->DialogResult = ::DialogResult::OK;

             DateTime dt = DateTime::Now;
             String^ strData = dt.ToString("yyyy-MM-dd");
             String^ strTime = dt.ToLongTimeString()->ToString();

             strcpy(m_pCurFundInOut->mUserID, Tools::String2string(comboBox_ID->Text).c_str());
             strcpy(m_pCurFundInOut->msDesc, Tools::String2string(textBox_Desc->Text).c_str());
             strcpy(m_pCurFundInOut->msDay, Tools::String2string(strData).c_str());
             strcpy(m_pCurFundInOut->msTime, Tools::String2string(strTime).c_str()); 
             m_pCurFundInOut->mdbVolume = ::Convert::ToDouble(textBox_Fund->Text);
             m_pCurFundInOut->meInOut = (eInOut)(*m_pType);


             this->Close();
         }
private: System::Void button_Cancel_Click(System::Object^  sender, System::EventArgs^  e) {
             this->Close();
         }
private: System::Void textBox_Fund_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
             if (e->KeyChar < '0' || e->KeyChar > '9')   
                 if (e->KeyChar != (char)8)    
                     e->Handled = true;
         }
private: System::Void textBox_Fund_TextChanged(System::Object^  sender, System::EventArgs^  e) {
             //double dText = ::Convert::ToDouble(this->textBox_Fund->Text);
             //this->textBox_Fund->Text = dText.ToString("N");
             //this->textBox_Fund->SelectionStart = textBox->Text->Length - 3;
         }
};
}
