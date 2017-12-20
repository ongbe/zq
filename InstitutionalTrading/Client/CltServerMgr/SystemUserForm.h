#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "Tools.h"
#include "CommonStruct.h"
#include "RoleCheckedListBoxForm.h"
#include "CCommonFunc.h"
#include "MD5.h"

namespace CltServerMgr {

	/// <summary>
	/// Summary for SystemUserForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class SystemUserForm : public System::Windows::Forms::Form
	{
	public:
		SystemUserForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
            m_nUserType = 2;
            m_pType = new OperType;
            m_pCurUserInfo = new UserInfo;
            m_pDataInfo = CDataInfo::NewInstance();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~SystemUserForm()
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

            if ( NULL != m_pCurUserInfo )
            {
                delete m_pCurUserInfo;
                m_pCurUserInfo = NULL;
            }
		}
    
    private: int m_nUserType;
    private: OperType* m_pType;
    private: String^   m_strCurUserRole;
    private: String^   m_strPreUserPsw;
    private: UserInfo* m_pCurUserInfo;    
    private: CDataInfo* m_pDataInfo;
    private: RoleCheckedListBoxForm^ m_RoleCheckedListBoxForm; 

    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::Label^  label2;
    private: System::Windows::Forms::Label^  label3;
    private: System::Windows::Forms::Label^  UserStatus_label;

    private: System::Windows::Forms::Label^  label5;
    private: System::Windows::Forms::Label^  label_Role;
    private: System::Windows::Forms::Label^  label_UserName;
    private: System::Windows::Forms::Label^  label_UserContact;



    private: System::Windows::Forms::TextBox^  UserID_textBox;
    private: System::Windows::Forms::TextBox^  UserPwd_textBox;
    private: System::Windows::Forms::TextBox^  UserPsw2_textBox;

    private: System::Windows::Forms::TextBox^  UserName_textBox;

    private: System::Windows::Forms::ComboBox^  UserStatus_comboBox;
    private: System::Windows::Forms::ComboBox^  UserType_comboBox;

    private: System::Windows::Forms::Button^  Ok_button;
    private: System::Windows::Forms::Button^  Cancel_button;
    private: AnywndComboBox::AnywndComboBoxControl^  anywndComboBoxControl1;
    private: System::Windows::Forms::TextBox^  UserContact_textBox;





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
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->label3 = (gcnew System::Windows::Forms::Label());
            this->UserStatus_label = (gcnew System::Windows::Forms::Label());
            this->label5 = (gcnew System::Windows::Forms::Label());
            this->label_Role = (gcnew System::Windows::Forms::Label());
            this->label_UserName = (gcnew System::Windows::Forms::Label());
            this->label_UserContact = (gcnew System::Windows::Forms::Label());
            this->UserID_textBox = (gcnew System::Windows::Forms::TextBox());
            this->UserPwd_textBox = (gcnew System::Windows::Forms::TextBox());
            this->UserPsw2_textBox = (gcnew System::Windows::Forms::TextBox());
            this->UserName_textBox = (gcnew System::Windows::Forms::TextBox());
            this->UserStatus_comboBox = (gcnew System::Windows::Forms::ComboBox());
            this->UserType_comboBox = (gcnew System::Windows::Forms::ComboBox());
            this->Ok_button = (gcnew System::Windows::Forms::Button());
            this->Cancel_button = (gcnew System::Windows::Forms::Button());
            this->anywndComboBoxControl1 = (gcnew AnywndComboBox::AnywndComboBoxControl());
            this->UserContact_textBox = (gcnew System::Windows::Forms::TextBox());
            this->SuspendLayout();
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(30, 24);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(41, 12);
            this->label1->TabIndex = 0;
            this->label1->Text = L"用户号";
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Location = System::Drawing::Point(30, 62);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(29, 12);
            this->label2->TabIndex = 0;
            this->label2->Text = L"密码";
            // 
            // label3
            // 
            this->label3->AutoSize = true;
            this->label3->Location = System::Drawing::Point(30, 100);
            this->label3->Name = L"label3";
            this->label3->Size = System::Drawing::Size(53, 12);
            this->label3->TabIndex = 0;
            this->label3->Text = L"确认密码";
            // 
            // UserStatus_label
            // 
            this->UserStatus_label->AutoSize = true;
            this->UserStatus_label->Location = System::Drawing::Point(30, 138);
            this->UserStatus_label->Name = L"UserStatus_label";
            this->UserStatus_label->Size = System::Drawing::Size(53, 12);
            this->UserStatus_label->TabIndex = 0;
            this->UserStatus_label->Text = L"账户状态";
            // 
            // label5
            // 
            this->label5->AutoSize = true;
            this->label5->Location = System::Drawing::Point(228, 24);
            this->label5->Name = L"label5";
            this->label5->Size = System::Drawing::Size(53, 12);
            this->label5->TabIndex = 0;
            this->label5->Text = L"用户类型";
            // 
            // label_Role
            // 
            this->label_Role->AutoSize = true;
            this->label_Role->Location = System::Drawing::Point(228, 62);
            this->label_Role->Name = L"label_Role";
            this->label_Role->Size = System::Drawing::Size(29, 12);
            this->label_Role->TabIndex = 0;
            this->label_Role->Text = L"角色";
            // 
            // label_UserName
            // 
            this->label_UserName->AutoSize = true;
            this->label_UserName->Location = System::Drawing::Point(228, 100);
            this->label_UserName->Name = L"label_UserName";
            this->label_UserName->Size = System::Drawing::Size(41, 12);
            this->label_UserName->TabIndex = 0;
            this->label_UserName->Text = L"用户名";
            // 
            // label_UserContact
            // 
            this->label_UserContact->AutoSize = true;
            this->label_UserContact->Location = System::Drawing::Point(228, 138);
            this->label_UserContact->Name = L"label_UserContact";
            this->label_UserContact->Size = System::Drawing::Size(53, 12);
            this->label_UserContact->TabIndex = 0;
            this->label_UserContact->Text = L"联系方式";
            // 
            // UserID_textBox
            // 
            this->UserID_textBox->Location = System::Drawing::Point(89, 21);
            this->UserID_textBox->MaxLength = 13;
            this->UserID_textBox->Name = L"UserID_textBox";
            this->UserID_textBox->Size = System::Drawing::Size(100, 21);
            this->UserID_textBox->TabIndex = 1;
            this->UserID_textBox->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &SystemUserForm::UserID_textBox_KeyPress);
            // 
            // UserPwd_textBox
            // 
            this->UserPwd_textBox->Location = System::Drawing::Point(89, 59);
            this->UserPwd_textBox->MaxLength = 33;
            this->UserPwd_textBox->Name = L"UserPwd_textBox";
            this->UserPwd_textBox->PasswordChar = '*';
            this->UserPwd_textBox->Size = System::Drawing::Size(100, 21);
            this->UserPwd_textBox->TabIndex = 2;
            // 
            // UserPsw2_textBox
            // 
            this->UserPsw2_textBox->Location = System::Drawing::Point(89, 97);
            this->UserPsw2_textBox->MaxLength = 33;
            this->UserPsw2_textBox->Name = L"UserPsw2_textBox";
            this->UserPsw2_textBox->PasswordChar = '*';
            this->UserPsw2_textBox->Size = System::Drawing::Size(100, 21);
            this->UserPsw2_textBox->TabIndex = 3;
            // 
            // UserName_textBox
            // 
            this->UserName_textBox->Location = System::Drawing::Point(287, 95);
            this->UserName_textBox->MaxLength = 12;
            this->UserName_textBox->Name = L"UserName_textBox";
            this->UserName_textBox->Size = System::Drawing::Size(100, 21);
            this->UserName_textBox->TabIndex = 7;
            // 
            // UserStatus_comboBox
            // 
            this->UserStatus_comboBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
            this->UserStatus_comboBox->FormattingEnabled = true;
            this->UserStatus_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"停用", L"启用"});
            this->UserStatus_comboBox->Location = System::Drawing::Point(89, 135);
            this->UserStatus_comboBox->Name = L"UserStatus_comboBox";
            this->UserStatus_comboBox->Size = System::Drawing::Size(100, 20);
            this->UserStatus_comboBox->TabIndex = 4;
            // 
            // UserType_comboBox
            // 
            this->UserType_comboBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
            this->UserType_comboBox->FormattingEnabled = true;
            this->UserType_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(4) {L"管理员", L"风控员", L"交易员", L"展现员"});
            this->UserType_comboBox->Location = System::Drawing::Point(287, 21);
            this->UserType_comboBox->Name = L"UserType_comboBox";
            this->UserType_comboBox->Size = System::Drawing::Size(100, 20);
            this->UserType_comboBox->TabIndex = 5;
            this->UserType_comboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &SystemUserForm::UserType_comboBox_SelectedIndexChanged);
            // 
            // Ok_button
            // 
            this->Ok_button->Location = System::Drawing::Point(102, 189);
            this->Ok_button->Name = L"Ok_button";
            this->Ok_button->Size = System::Drawing::Size(75, 23);
            this->Ok_button->TabIndex = 9;
            this->Ok_button->Text = L"确定";
            this->Ok_button->UseVisualStyleBackColor = true;
            this->Ok_button->Click += gcnew System::EventHandler(this, &SystemUserForm::Ok_button_Click);
            // 
            // Cancel_button
            // 
            this->Cancel_button->DialogResult = System::Windows::Forms::DialogResult::Cancel;
            this->Cancel_button->Location = System::Drawing::Point(259, 189);
            this->Cancel_button->Name = L"Cancel_button";
            this->Cancel_button->Size = System::Drawing::Size(75, 23);
            this->Cancel_button->TabIndex = 10;
            this->Cancel_button->Text = L"取消";
            this->Cancel_button->UseVisualStyleBackColor = true;
            this->Cancel_button->Click += gcnew System::EventHandler(this, &SystemUserForm::Cancel_button_Click);
            // 
            // anywndComboBoxControl1
            // 
            this->anywndComboBoxControl1->BackColor = System::Drawing::Color::White;
            this->anywndComboBoxControl1->ChildControl = nullptr;
            this->anywndComboBoxControl1->DropDownHeight = 1;
            this->anywndComboBoxControl1->DropDownWidth = 1;
            this->anywndComboBoxControl1->FormattingEnabled = true;
            this->anywndComboBoxControl1->IntegralHeight = false;
            this->anywndComboBoxControl1->Location = System::Drawing::Point(287, 59);
            this->anywndComboBoxControl1->MaxDropDownItems = 1;
            this->anywndComboBoxControl1->Name = L"anywndComboBoxControl1";
            this->anywndComboBoxControl1->Size = System::Drawing::Size(100, 20);
            this->anywndComboBoxControl1->TabIndex = 6;
            // 
            // UserContact_textBox
            // 
            this->UserContact_textBox->Location = System::Drawing::Point(287, 133);
            this->UserContact_textBox->MaxLength = 32;
            this->UserContact_textBox->Name = L"UserContact_textBox";
            this->UserContact_textBox->Size = System::Drawing::Size(100, 21);
            this->UserContact_textBox->TabIndex = 8;
            // 
            // SystemUserForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(418, 232);
            this->Controls->Add(this->anywndComboBoxControl1);
            this->Controls->Add(this->Cancel_button);
            this->Controls->Add(this->Ok_button);
            this->Controls->Add(this->UserType_comboBox);
            this->Controls->Add(this->UserStatus_comboBox);
            this->Controls->Add(this->UserContact_textBox);
            this->Controls->Add(this->UserName_textBox);
            this->Controls->Add(this->UserPsw2_textBox);
            this->Controls->Add(this->UserPwd_textBox);
            this->Controls->Add(this->UserID_textBox);
            this->Controls->Add(this->label_UserContact);
            this->Controls->Add(this->UserStatus_label);
            this->Controls->Add(this->label_UserName);
            this->Controls->Add(this->label3);
            this->Controls->Add(this->label_Role);
            this->Controls->Add(this->label2);
            this->Controls->Add(this->label5);
            this->Controls->Add(this->label1);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Name = L"SystemUserForm";
            this->Text = L"系统用户";
            this->Load += gcnew System::EventHandler(this, &SystemUserForm::SystemUserForm_Load);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
    public:
        void SetOperType(OperType eType)
        {
            *m_pType = eType;
        }
        void SetCurUserInfo(const UserInfo& userInfo)
        {
            *m_pCurUserInfo = userInfo;
        }

        UserInfo* GetCurUserInfo()
        {
            return m_pCurUserInfo;
        }

        void SetCurUserRole(String^ userRole)
        {
            m_strCurUserRole = userRole->Trim();
        }

        String^ GetCurUserRole()
        {
            return m_strCurUserRole;
        }

        public: void SetEnable(bool bEnable)
        {
            anywndComboBoxControl1->Enabled = bEnable;            
        }

        public: void SetUserType(int nType)
        {
            m_nUserType = nType;            
        }


private: System::Void SystemUserForm_Load(System::Object^  sender, System::EventArgs^  e) {
             this->CenterToParent();

             m_RoleCheckedListBoxForm = gcnew RoleCheckedListBoxForm();
             m_RoleCheckedListBoxForm->ResetCombo(anywndComboBoxControl1);
             anywndComboBoxControl1->ChildControl = m_RoleCheckedListBoxForm;

             ResetComboBoxTextReadOnly(anywndComboBoxControl1);

             if(m_nUserType== USER_TYPE_TRADER || m_nUserType == USER_TYPE_VIEWER )             
                 anywndComboBoxControl1->Enabled = false;               
             else             
                 anywndComboBoxControl1->Enabled = true;  
             

             UserType_comboBox->Enabled = false;

             if (m_nUserType == USER_TYPE_TRADER || m_nUserType == USER_TYPE_VIEWER)
             {
                 label_Role->Visible = false;
                 anywndComboBoxControl1->Visible = false;

                 label_UserContact->Location=Point(label_UserName->Location.X,label_UserName->Location.Y);
                 label_UserName->Location=Point(label_Role->Location.X,label_Role->Location.Y);  

                 UserContact_textBox->Location=Point(UserName_textBox->Location.X,UserName_textBox->Location.Y);
                 UserName_textBox->Location=Point(anywndComboBoxControl1->Location.X,anywndComboBoxControl1->Location.Y); 
             }

             if ( *m_pType == Oper_Add )
             {
                 this->Text = "新增用户";
                 UserID_textBox->Enabled = true;     

                 UserStatus_label->Visible = true;
                 UserStatus_comboBox->Visible = true;

                 UserType_comboBox->SelectedIndex   = m_nUserType;
                 UserStatus_comboBox->SelectedIndex = 0; 
             }
             else
             {
                 this->Text = "修改用户";
                 UserID_textBox->Enabled = false;  
                 anywndComboBoxControl1->Enabled = true;

                 if(m_nUserType == USER_TYPE_TRADER)
                 {
                     UserStatus_label->Visible = false;
                     UserStatus_comboBox->Visible = false;
                 }

                 UserID_textBox->Text       = Tools::string2String(m_pCurUserInfo->szAccount);                 
                 UserName_textBox->Text     = Tools::string2String(m_pCurUserInfo->szAlias);
                 UserPwd_textBox->Text      = Tools::string2String(m_pCurUserInfo->szPassword);
                 UserPsw2_textBox->Text     = Tools::string2String(m_pCurUserInfo->szPassword);
                 UserContact_textBox->Text  = Tools::string2String(m_pCurUserInfo->szContactInfo);
                 UserStatus_comboBox->SelectedIndex = m_pCurUserInfo->nStatus;
                 UserType_comboBox->SelectedIndex   = m_pCurUserInfo->nUserType;                 
                 anywndComboBoxControl1->Text       = m_strCurUserRole;
                 m_RoleCheckedListBoxForm->SetRoleType(m_pCurUserInfo->nUserType,m_pCurUserInfo->nUserID);

                 m_strPreUserPsw = UserPwd_textBox->Text;
             }
         }
private: System::Void Ok_button_Click(System::Object^  sender, System::EventArgs^  e) {          
             if(*m_pType == Oper_Add && 0==m_pDataInfo->IsValidOfUserAccount(Tools::String2string(UserID_textBox->Text), (UserType)UserType_comboBox->SelectedIndex)) 
                     MessageBox::Show("同用户类型用户名重复,请重新输入!", "提示",MessageBoxButtons::OK);
             else if(*m_pType == Oper_Add && -1==m_pDataInfo->IsValidOfUserAccount(Tools::String2string(UserID_textBox->Text), (UserType)UserType_comboBox->SelectedIndex)) 
                     MessageBox::Show("非同用户类型用户名重复,请重新输入!", "提示",MessageBoxButtons::OK);
             else if ( String::IsNullOrEmpty(UserName_textBox->Text) )             
                 MessageBox::Show("用户名不能为空！", "提示",MessageBoxButtons::OK);
             else if ( String::IsNullOrEmpty(UserContact_textBox->Text) )             
                 MessageBox::Show("联系方式不能为空！", "提示",MessageBoxButtons::OK);
             else if( UserPwd_textBox->Text->CompareTo(UserPsw2_textBox->Text))             
                 MessageBox::Show("密码不一致！", "提示",MessageBoxButtons::OK);
             else if( String::IsNullOrEmpty(UserPwd_textBox->Text) || String::IsNullOrEmpty(UserPsw2_textBox->Text) )             
                 MessageBox::Show("密码不能为空！", "提示",MessageBoxButtons::OK);
             else
             {
                 this->DialogResult = ::DialogResult::OK;

                 if(*m_pType == Oper_Add)
                 {
                     MD5 md5(Tools::String2string(UserPwd_textBox->Text).c_str());
                     strcpy(m_pCurUserInfo->szPassword, md5.toString().c_str());
                 }

                 if(*m_pType == Oper_Modify) 
                 {
                     if(m_strPreUserPsw->CompareTo(UserPwd_textBox->Text))
                     {
                         MD5 md5(Tools::String2string(UserPwd_textBox->Text).c_str());
                         strcpy(m_pCurUserInfo->szPassword, md5.toString().c_str());
                     }else
                         strcpy(m_pCurUserInfo->szPassword,Tools::String2string(UserPwd_textBox->Text).c_str());
                 }

                 strcpy(m_pCurUserInfo->szAccount,Tools::String2string(UserID_textBox->Text).c_str());
                 strcpy(m_pCurUserInfo->szAlias, Tools::String2string(UserName_textBox->Text).c_str());
                 strcpy(m_pCurUserInfo->szContactInfo, Tools::String2string(UserContact_textBox->Text).c_str());
                 m_pCurUserInfo->nStatus = UserStatus_comboBox->SelectedIndex;
                 m_pCurUserInfo->nUserType = (UserType)(UserType_comboBox->SelectedIndex);
                 if(*m_pType == Oper_Add) m_pCurUserInfo->nAssign = 0;
                 m_strCurUserRole = m_RoleCheckedListBoxForm->GetCurUserRole();

                 this->Close();
             }
             
         }
private: System::Void Cancel_button_Click(System::Object^  sender, System::EventArgs^  e) {
             this->Close();
         }
private: System::Void UserType_comboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
             m_RoleCheckedListBoxForm->SetRoleType(UserType_comboBox->SelectedIndex,m_pCurUserInfo->nUserID);

             if(UserType_comboBox->SelectedIndex == USER_TYPE_TRADER || 
                 UserType_comboBox->SelectedIndex == USER_TYPE_VIEWER )
                 m_RoleCheckedListBoxForm->Enabled = false;
             else
                 m_RoleCheckedListBoxForm->Enabled = true;
         }
private: System::Void UserID_textBox_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
             if (e->KeyChar < '0' || e->KeyChar > '9')   
                 if (e->KeyChar != (char)8)    
                     e->Handled = true;
         }
};
}
