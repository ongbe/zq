#pragma once
#include "TcpDataConnect.h"
#include "CommonDef.h"
#include "CommonStruct.h"
#include "CommonPkg.h"
#include "CommonErrorCode.h"
#include "MD5.h"
#include "Tools.h"
#include "RiskData.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace CltRisk {

	/// <summary>
	/// Summary for FormModifyPsw
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class FormModifyPsw : public System::Windows::Forms::Form
	{
	public:
		FormModifyPsw(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
            m_pUserPsw = new UserPassword;
            memset(m_pUserPsw,0,sizeof(UserPassword));
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormModifyPsw()
		{
			if (components)
			{
				delete components;
			}
            if(m_pUserPsw)
            {
                delete m_pUserPsw;
                m_pUserPsw = NULL;
            }
		}
    private: System::Windows::Forms::Label^  label1;
    protected: 
    private: System::Windows::Forms::Label^  label2;
    private: System::Windows::Forms::Label^  label3;
    private: System::Windows::Forms::TextBox^  textBox_OldPsw;
    private: System::Windows::Forms::TextBox^  textBox_NewPsw1;
    private: System::Windows::Forms::TextBox^  textBox_NewPsw2;
    private: System::Windows::Forms::Button^  button_OK;
    private: System::Windows::Forms::Button^  button_Cancel;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

        UserPassword* m_pUserPsw;

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
            this->textBox_OldPsw = (gcnew System::Windows::Forms::TextBox());
            this->textBox_NewPsw1 = (gcnew System::Windows::Forms::TextBox());
            this->textBox_NewPsw2 = (gcnew System::Windows::Forms::TextBox());
            this->button_OK = (gcnew System::Windows::Forms::Button());
            this->button_Cancel = (gcnew System::Windows::Forms::Button());
            this->SuspendLayout();
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(45, 39);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(65, 12);
            this->label1->TabIndex = 0;
            this->label1->Text = L"原始密码：";
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Location = System::Drawing::Point(57, 79);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(53, 12);
            this->label2->TabIndex = 0;
            this->label2->Text = L"新密码：";
            // 
            // label3
            // 
            this->label3->AutoSize = true;
            this->label3->Location = System::Drawing::Point(45, 116);
            this->label3->Name = L"label3";
            this->label3->Size = System::Drawing::Size(65, 12);
            this->label3->TabIndex = 0;
            this->label3->Text = L"确认密码：";
            // 
            // textBox_OldPsw
            // 
            this->textBox_OldPsw->Location = System::Drawing::Point(117, 34);
            this->textBox_OldPsw->Name = L"textBox_OldPsw";
            this->textBox_OldPsw->PasswordChar = '*';
            this->textBox_OldPsw->Size = System::Drawing::Size(130, 21);
            this->textBox_OldPsw->TabIndex = 1;
            this->textBox_OldPsw->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &FormModifyPsw::textBox_KeyPress);
            // 
            // textBox_NewPsw1
            // 
            this->textBox_NewPsw1->Location = System::Drawing::Point(117, 75);
            this->textBox_NewPsw1->Name = L"textBox_NewPsw1";
            this->textBox_NewPsw1->PasswordChar = '*';
            this->textBox_NewPsw1->Size = System::Drawing::Size(130, 21);
            this->textBox_NewPsw1->TabIndex = 2;
            this->textBox_NewPsw1->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &FormModifyPsw::textBox_KeyPress);
            // 
            // textBox_NewPsw2
            // 
            this->textBox_NewPsw2->Location = System::Drawing::Point(117, 112);
            this->textBox_NewPsw2->Name = L"textBox_NewPsw2";
            this->textBox_NewPsw2->PasswordChar = '*';
            this->textBox_NewPsw2->Size = System::Drawing::Size(130, 21);
            this->textBox_NewPsw2->TabIndex = 3;
            this->textBox_NewPsw2->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &FormModifyPsw::textBox_KeyPress);
            // 
            // button_OK
            // 
            this->button_OK->Location = System::Drawing::Point(58, 162);
            this->button_OK->Name = L"button_OK";
            this->button_OK->Size = System::Drawing::Size(75, 23);
            this->button_OK->TabIndex = 4;
            this->button_OK->Text = L"确定";
            this->button_OK->UseVisualStyleBackColor = true;
            this->button_OK->Click += gcnew System::EventHandler(this, &FormModifyPsw::button_OK_Click);
            // 
            // button_Cancel
            // 
            this->button_Cancel->Location = System::Drawing::Point(172, 162);
            this->button_Cancel->Name = L"button_Cancel";
            this->button_Cancel->Size = System::Drawing::Size(75, 23);
            this->button_Cancel->TabIndex = 5;
            this->button_Cancel->Text = L"取消";
            this->button_Cancel->UseVisualStyleBackColor = true;
            this->button_Cancel->Click += gcnew System::EventHandler(this, &FormModifyPsw::button_Cancel_Click);
            // 
            // FormModifyPsw
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(315, 205);
            this->Controls->Add(this->button_Cancel);
            this->Controls->Add(this->button_OK);
            this->Controls->Add(this->textBox_NewPsw2);
            this->Controls->Add(this->textBox_NewPsw1);
            this->Controls->Add(this->textBox_OldPsw);
            this->Controls->Add(this->label3);
            this->Controls->Add(this->label2);
            this->Controls->Add(this->label1);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Name = L"FormModifyPsw";
            this->Text = L"密码修改";
            this->Load += gcnew System::EventHandler(this, &FormModifyPsw::FormModifyPsw_Load);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
public:  void GetModifyPSW(UserPassword& sPsw)
         {
             memcpy(&sPsw,m_pUserPsw,sizeof(UserPassword));
         }
private: System::Void FormModifyPsw_Load(System::Object^  sender, System::EventArgs^  e) {
             this->CenterToParent();
         }
private: System::Void button_OK_Click(System::Object^  sender, System::EventArgs^  e) {
             if(String::IsNullOrEmpty(textBox_OldPsw->Text))
                 MessageBox::Show("原始密码不能为空!");
             else if(String::IsNullOrEmpty(textBox_NewPsw1->Text))
                 MessageBox::Show("新密码不能为空!");
             else if(String::IsNullOrEmpty(textBox_NewPsw2->Text))
                 MessageBox::Show("确认密码不能为空!");
             else if(textBox_NewPsw1->Text->CompareTo(textBox_NewPsw2->Text))
                 MessageBox::Show("新密码和确认密码不同，请重新输入!");
             else
             {
                 sLoginRsp Login;
                 CRiskData::GetInstance()->GetLogonInfo(Login);

                 m_pUserPsw->nUserID = Login.mnUserID;
                 string szOldPassword = Tools::String2string(textBox_OldPsw->Text).c_str();
                 string szNewPassword = Tools::String2string(textBox_NewPsw1->Text).c_str();

                 MD5 md5(szOldPassword);
                 strcpy(m_pUserPsw->szOldPassword, md5.toString().c_str());
                 MD5 md5_(szNewPassword);
                 strcpy(m_pUserPsw->szNewPassword, md5_.toString().c_str());

                 this->DialogResult = ::DialogResult::OK;                 
             }

         }
private: System::Void button_Cancel_Click(System::Object^  sender, System::EventArgs^  e) {
             Close();
         }
private: System::Void textBox_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
             if((e->KeyChar>='0'&&e->KeyChar<='9')||(e->KeyChar>='A'&&e->KeyChar<='Z')||(e->KeyChar>='a'&&e->KeyChar<='z')||e->KeyChar == (char)8)   
                 e->Handled=false;   
             else   
                 e->Handled=true;   
         }
};
}
