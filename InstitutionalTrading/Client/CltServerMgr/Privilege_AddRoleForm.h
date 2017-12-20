#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "Tools.h"
#include "CDataInfo.h"

namespace CltServerMgr {

	/// <summary>
	/// Summary for Privilege_AddRoleForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Privilege_AddRoleForm : public System::Windows::Forms::Form
	{
	public:
		Privilege_AddRoleForm()
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
            m_pType = new OperType;
            m_pCurRole = new RoleInfo;
            m_pDataInfo = CDataInfo::NewInstance();
            m_nUserType = eManagerType;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Privilege_AddRoleForm()
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

            if ( NULL != m_pCurRole )
            {
                delete m_pCurRole;
                m_pCurRole = NULL;
            }
		}

    private: CDataInfo* m_pDataInfo;
    private: OperType* m_pType;
    private: RoleInfo* m_pCurRole;  
    private: eUserType m_nUserType;


    private: System::Windows::Forms::Label^  label1;
    protected: 
    private: System::Windows::Forms::TextBox^  textBox_Name;
    private: System::Windows::Forms::Button^  button_Ok;
    private: System::Windows::Forms::Button^  button_Cancel;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

    public:
        void SetUserType(eUserType eType)
        {
            m_nUserType = eType;
        }
        void SetOperType(OperType eType)
        {
            *m_pType = eType;
        }
        void SetCurRole(const RoleInfo& roleInfo)
        {
            *m_pCurRole = roleInfo;
        }

        RoleInfo* GetCurRole()
        {
            return m_pCurRole;
        }

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->textBox_Name = (gcnew System::Windows::Forms::TextBox());
            this->button_Ok = (gcnew System::Windows::Forms::Button());
            this->button_Cancel = (gcnew System::Windows::Forms::Button());
            this->SuspendLayout();
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(37, 40);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(41, 12);
            this->label1->TabIndex = 0;
            this->label1->Text = L"名称：";
            // 
            // textBox_Name
            // 
            this->textBox_Name->Location = System::Drawing::Point(106, 36);
            this->textBox_Name->Name = L"textBox_Name";
            this->textBox_Name->Size = System::Drawing::Size(100, 21);
            this->textBox_Name->TabIndex = 1;
            // 
            // button_Ok
            // 
            this->button_Ok->Location = System::Drawing::Point(39, 90);
            this->button_Ok->Name = L"button_Ok";
            this->button_Ok->Size = System::Drawing::Size(75, 23);
            this->button_Ok->TabIndex = 2;
            this->button_Ok->Text = L"确定";
            this->button_Ok->UseVisualStyleBackColor = true;
            this->button_Ok->Click += gcnew System::EventHandler(this, &Privilege_AddRoleForm::button_Ok_Click);
            // 
            // button_Cancel
            // 
            this->button_Cancel->Location = System::Drawing::Point(159, 90);
            this->button_Cancel->Name = L"button_Cancel";
            this->button_Cancel->Size = System::Drawing::Size(75, 23);
            this->button_Cancel->TabIndex = 2;
            this->button_Cancel->Text = L"取消";
            this->button_Cancel->UseVisualStyleBackColor = true;
            this->button_Cancel->Click += gcnew System::EventHandler(this, &Privilege_AddRoleForm::button_Cancel_Click);
            // 
            // Privilege_AddRoleForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(285, 125);
            this->Controls->Add(this->button_Cancel);
            this->Controls->Add(this->button_Ok);
            this->Controls->Add(this->textBox_Name);
            this->Controls->Add(this->label1);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->Name = L"Privilege_AddRoleForm";
            this->Text = L"新增角色";
            this->Load += gcnew System::EventHandler(this, &Privilege_AddRoleForm::Privilege_AddRoleForm_Load);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
    private: System::Void Privilege_AddRoleForm_Load(System::Object^  sender, System::EventArgs^  e) {
                 this->CenterToParent();

                 if ( *m_pType == Oper_Add )
                 {
                     this->Text = "新增角色";
                 }
                 else
                 {
                     this->Text = "修改角色";

                     textBox_Name->Text  = Tools::string2String(m_pCurRole->szName);                 
                 }
             }
    private: System::Void button_Ok_Click(System::Object^  sender, System::EventArgs^  e) {
                 if(String::IsNullOrEmpty(textBox_Name->Text)) 
                 {
                     MessageBox::Show("请输入角色名称!");  
                     return;
                 }

                 RoleInfo roleInfo;
                 roleInfo.nRoleID = (*m_pType == Oper_Modify)?m_pCurRole->nRoleID:-1;
                 strcpy(roleInfo.szName,Tools::String2string(textBox_Name->Text).c_str());  
                 roleInfo.nUserTypeID = (*m_pType == Oper_Modify)?m_pCurRole->nUserTypeID:m_nUserType;

                 if(*m_pType==Oper_Add)
                 {
                     if(!m_pDataInfo->IsValidOfRoleName(Tools::String2string(textBox_Name->Text)))    
                     {
                         MessageBox::Show("角色名称重名,请重新输入!");                 
                         return;
                     }

                     memcpy(m_pCurRole,&roleInfo,sizeof(RoleInfo));

                     this->DialogResult = ::DialogResult::OK;
                     this->Close();
                 }

                 if(*m_pType==Oper_Modify)
                 {
                     if(*m_pCurRole == roleInfo)
                         this->DialogResult = ::DialogResult::Cancel;
                     else
                     {
                         if(0==strcmp(m_pCurRole->szName,roleInfo.szName))
                         {
                             this->DialogResult = ::DialogResult::OK;
                             memcpy(m_pCurRole,&roleInfo,sizeof(RoleInfo));
                             this->Close();
                         }
                         else
                         {
                             if(!m_pDataInfo->IsValidOfRoleName(roleInfo.szName))
                             {                             
                                 MessageBox::Show("角色名称重名,请重新输入!"); 
                             }
                             else
                             {
                                 this->DialogResult = ::DialogResult::OK;
                                 memcpy(m_pCurRole,&roleInfo,sizeof(RoleInfo));
                                 this->Close();
                             }
                         }
                     }                 
                 }
             }
private: System::Void button_Cancel_Click(System::Object^  sender, System::EventArgs^  e) {
             this->Close();
         }
};
}
