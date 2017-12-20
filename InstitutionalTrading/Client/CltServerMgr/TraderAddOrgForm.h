#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "OrganizeCheckedTreeListForm.h"
#include "CCommonFunc.h"

namespace CltServerMgr {

	/// <summary>
	/// Summary for TraderAddOrgForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class TraderAddOrgForm : public System::Windows::Forms::Form
	{
	public:
		TraderAddOrgForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
            m_pCurUserInfo = new UserInfo;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~TraderAddOrgForm()
		{
			if (components)
			{
				delete components;
			}
		}


    private: String^   m_strCurOrg;
    private: UserInfo* m_pCurUserInfo;   
    private: OrganizeCheckedTreeListForm^ m_OrganizeCheckedTreeListForm;

    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::Label^  label2;
    private: System::Windows::Forms::Label^  label3;
    private: System::Windows::Forms::Label^  Account_label;
    private: System::Windows::Forms::Label^  Name_label;
    private: System::Windows::Forms::Button^  OK_button;
    private: AnywndComboBox::AnywndComboBoxControl^  anywndComboBoxControl1;
    private: System::Windows::Forms::Button^  Cancel_button;

    protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

    public:
        void SetCurUserInfo(const UserInfo& userInfo)
        {
            *m_pCurUserInfo = userInfo;
        }

        void SetCurOrg(String^ org)
        {
            m_strCurOrg = org;
        }

        String^ GetCurOrg()
        {
            return (String::IsNullOrEmpty(anywndComboBoxControl1->Text)?"-1":anywndComboBoxControl1->Text);
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
            this->Account_label = (gcnew System::Windows::Forms::Label());
            this->Name_label = (gcnew System::Windows::Forms::Label());
            this->OK_button = (gcnew System::Windows::Forms::Button());
            this->anywndComboBoxControl1 = (gcnew AnywndComboBox::AnywndComboBoxControl());
            this->Cancel_button = (gcnew System::Windows::Forms::Button());
            this->SuspendLayout();
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(23, 22);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(29, 12);
            this->label1->TabIndex = 0;
            this->label1->Text = L"账号";
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Location = System::Drawing::Point(23, 53);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(41, 12);
            this->label2->TabIndex = 1;
            this->label2->Text = L"用户名";
            // 
            // label3
            // 
            this->label3->AutoSize = true;
            this->label3->Location = System::Drawing::Point(23, 86);
            this->label3->Name = L"label3";
            this->label3->Size = System::Drawing::Size(53, 12);
            this->label3->TabIndex = 2;
            this->label3->Text = L"所属部门";
            // 
            // Account_label
            // 
            this->Account_label->AutoSize = true;
            this->Account_label->Location = System::Drawing::Point(88, 22);
            this->Account_label->Name = L"Account_label";
            this->Account_label->Size = System::Drawing::Size(0, 12);
            this->Account_label->TabIndex = 3;
            // 
            // Name_label
            // 
            this->Name_label->AutoSize = true;
            this->Name_label->Location = System::Drawing::Point(88, 53);
            this->Name_label->Name = L"Name_label";
            this->Name_label->Size = System::Drawing::Size(0, 12);
            this->Name_label->TabIndex = 4;
            // 
            // OK_button
            // 
            this->OK_button->Location = System::Drawing::Point(25, 125);
            this->OK_button->Name = L"OK_button";
            this->OK_button->Size = System::Drawing::Size(75, 23);
            this->OK_button->TabIndex = 5;
            this->OK_button->Text = L"确定";
            this->OK_button->UseVisualStyleBackColor = true;
            this->OK_button->Click += gcnew System::EventHandler(this, &TraderAddOrgForm::OK_button_Click);
            // 
            // anywndComboBoxControl1
            // 
            this->anywndComboBoxControl1->BackColor = System::Drawing::Color::White;
            this->anywndComboBoxControl1->ChildControl = nullptr;
            this->anywndComboBoxControl1->DropDownHeight = 1;
            this->anywndComboBoxControl1->DropDownWidth = 1;
            this->anywndComboBoxControl1->FormattingEnabled = true;
            this->anywndComboBoxControl1->IntegralHeight = false;
            this->anywndComboBoxControl1->Location = System::Drawing::Point(88, 83);
            this->anywndComboBoxControl1->MaxDropDownItems = 1;
            this->anywndComboBoxControl1->Name = L"anywndComboBoxControl1";
            this->anywndComboBoxControl1->Size = System::Drawing::Size(104, 20);
            this->anywndComboBoxControl1->TabIndex = 6;
            // 
            // Cancel_button
            // 
            this->Cancel_button->DialogResult = System::Windows::Forms::DialogResult::Cancel;
            this->Cancel_button->Location = System::Drawing::Point(117, 125);
            this->Cancel_button->Name = L"Cancel_button";
            this->Cancel_button->Size = System::Drawing::Size(75, 23);
            this->Cancel_button->TabIndex = 7;
            this->Cancel_button->Text = L"取消";
            this->Cancel_button->UseVisualStyleBackColor = true;
            this->Cancel_button->Click += gcnew System::EventHandler(this, &TraderAddOrgForm::Cancel_button_Click);
            // 
            // TraderAddOrgForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(215, 180);
            this->Controls->Add(this->Cancel_button);
            this->Controls->Add(this->anywndComboBoxControl1);
            this->Controls->Add(this->OK_button);
            this->Controls->Add(this->Name_label);
            this->Controls->Add(this->Account_label);
            this->Controls->Add(this->label3);
            this->Controls->Add(this->label2);
            this->Controls->Add(this->label1);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Name = L"TraderAddOrgForm";
            this->Text = L"分配部门";
            this->Load += gcnew System::EventHandler(this, &TraderAddOrgForm::TraderAddOrgForm_Load);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
private: System::Void TraderAddOrgForm_Load(System::Object^  sender, System::EventArgs^  e) {
             this->CenterToParent();

             m_OrganizeCheckedTreeListForm = gcnew OrganizeCheckedTreeListForm();             
             m_OrganizeCheckedTreeListForm->ResetRelateCombo(anywndComboBoxControl1);
             anywndComboBoxControl1->ChildControl = m_OrganizeCheckedTreeListForm; 

             ResetComboBoxTextReadOnly(anywndComboBoxControl1);

             Account_label->Text = Tools::string2String(m_pCurUserInfo->szAccount);
             Name_label->Text    = Tools::string2String(m_pCurUserInfo->szAlias);
             anywndComboBoxControl1->Text = m_strCurOrg;
         }
private: System::Void OK_button_Click(System::Object^  sender, System::EventArgs^  e) {
             m_strCurOrg = m_OrganizeCheckedTreeListForm->GetCurOrg();
             if(String::IsNullOrEmpty(m_strCurOrg))
             {
                 MessageBox::Show("所属部门不能为空!");
                 return;
             }
             else
             {
                 this->DialogResult = ::DialogResult::OK;
                 this->Close();
             }
         }
private: System::Void Cancel_button_Click(System::Object^  sender, System::EventArgs^  e) {
             this->Close();
         }
};
}
