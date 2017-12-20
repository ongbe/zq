#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Text::RegularExpressions;

#include "CDataInfo.h"
#include "CCommonFunc.h"
#include "OrganizeCheckedTreeListForm.h"

namespace CltServerMgr {

	/// <summary>
	/// Summary for OrganizeForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class OrganizeForm : public System::Windows::Forms::Form
	{
	public:
        OrganizeForm()
        {
            InitializeComponent();
            //
            //TODO: Add the constructor code here
            //
            m_nOrganizeID = -1;
            m_pType = new OperType;
            m_pCurOrganize = new Organization;
            m_pDataInfo = CDataInfo::NewInstance();
        }
		OrganizeForm(int nExpandOrgID)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
            m_nOrganizeID = nExpandOrgID;
            m_pType = new OperType;
            m_pCurOrganize = new Organization;
            m_pDataInfo = CDataInfo::NewInstance();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~OrganizeForm()
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

            if ( NULL != m_pCurOrganize )
            {
                delete m_pCurOrganize;
                m_pCurOrganize = NULL;
            }
		}


    private: int m_nOrganizeID;//修改时隐藏本身以下的组织机构ID
    private: String^ m_strUpperOrganize;
    private: OperType* m_pType;
    private: Organization* m_pCurOrganize;  
    private: CDataInfo* m_pDataInfo;
    private: OrganizeCheckedTreeListForm^ m_OrganizeCheckedTreeListForm;


    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::Label^  label2;
    private: System::Windows::Forms::Label^  label3;
    private: System::Windows::Forms::Label^  label4;
    private: AnywndComboBox::AnywndComboBoxControl^  anywndComboBoxControl1;
    private: System::Windows::Forms::Button^  OK_button;
    private: System::Windows::Forms::Button^  Cancel_button;
    private: System::Windows::Forms::TextBox^  Name_textBox;
    private: System::Windows::Forms::TextBox^  Dimention1_textBox;
    private: System::Windows::Forms::TextBox^  Dimention2_textBox;


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

    public:
        void SetOperType(OperType eType)
        {
            *m_pType = eType;
        }

        void SetCurOrganize(const Organization& org)
        {
            *m_pCurOrganize = org;
        }
        Organization* GetCurOrganize()
        {
            return m_pCurOrganize;
        }

        void SetCurUpperOrganize(String^ org)
        {
            m_strUpperOrganize = org;
        }
        String^ GetCurUpperOrganize()
        {
            return m_strUpperOrganize;
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
            this->anywndComboBoxControl1 = (gcnew AnywndComboBox::AnywndComboBoxControl());
            this->OK_button = (gcnew System::Windows::Forms::Button());
            this->Cancel_button = (gcnew System::Windows::Forms::Button());
            this->Name_textBox = (gcnew System::Windows::Forms::TextBox());
            this->Dimention1_textBox = (gcnew System::Windows::Forms::TextBox());
            this->Dimention2_textBox = (gcnew System::Windows::Forms::TextBox());
            this->SuspendLayout();
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(42, 30);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(53, 12);
            this->label1->TabIndex = 0;
            this->label1->Text = L"上级部门";
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Location = System::Drawing::Point(42, 72);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(53, 12);
            this->label2->TabIndex = 1;
            this->label2->Text = L"部门名称";
            // 
            // label3
            // 
            this->label3->AutoSize = true;
            this->label3->Location = System::Drawing::Point(42, 114);
            this->label3->Name = L"label3";
            this->label3->Size = System::Drawing::Size(29, 12);
            this->label3->TabIndex = 2;
            this->label3->Text = L"经度";
            // 
            // label4
            // 
            this->label4->AutoSize = true;
            this->label4->Location = System::Drawing::Point(42, 156);
            this->label4->Name = L"label4";
            this->label4->Size = System::Drawing::Size(29, 12);
            this->label4->TabIndex = 3;
            this->label4->Text = L"纬度";
            // 
            // anywndComboBoxControl1
            // 
            this->anywndComboBoxControl1->BackColor = System::Drawing::Color::White;
            this->anywndComboBoxControl1->ChildControl = nullptr;
            this->anywndComboBoxControl1->DropDownHeight = 1;
            this->anywndComboBoxControl1->DropDownWidth = 1;
            this->anywndComboBoxControl1->FormattingEnabled = true;
            this->anywndComboBoxControl1->IntegralHeight = false;
            this->anywndComboBoxControl1->Location = System::Drawing::Point(115, 27);
            this->anywndComboBoxControl1->MaxDropDownItems = 1;
            this->anywndComboBoxControl1->Name = L"anywndComboBoxControl1";
            this->anywndComboBoxControl1->Size = System::Drawing::Size(100, 20);
            this->anywndComboBoxControl1->TabIndex = 4;
            // 
            // OK_button
            // 
            this->OK_button->Location = System::Drawing::Point(41, 196);
            this->OK_button->Name = L"OK_button";
            this->OK_button->Size = System::Drawing::Size(75, 23);
            this->OK_button->TabIndex = 5;
            this->OK_button->Text = L"确定";
            this->OK_button->UseVisualStyleBackColor = true;
            this->OK_button->Click += gcnew System::EventHandler(this, &OrganizeForm::OK_button_Click);
            // 
            // Cancel_button
            // 
            this->Cancel_button->DialogResult = System::Windows::Forms::DialogResult::Cancel;
            this->Cancel_button->Location = System::Drawing::Point(140, 196);
            this->Cancel_button->Name = L"Cancel_button";
            this->Cancel_button->Size = System::Drawing::Size(75, 23);
            this->Cancel_button->TabIndex = 6;
            this->Cancel_button->Text = L"取消";
            this->Cancel_button->UseVisualStyleBackColor = true;
            this->Cancel_button->Click += gcnew System::EventHandler(this, &OrganizeForm::Cancel_button_Click);
            // 
            // Name_textBox
            // 
            this->Name_textBox->Location = System::Drawing::Point(115, 66);
            this->Name_textBox->Name = L"Name_textBox";
            this->Name_textBox->Size = System::Drawing::Size(100, 21);
            this->Name_textBox->TabIndex = 7;
            // 
            // Dimention1_textBox
            // 
            this->Dimention1_textBox->Location = System::Drawing::Point(115, 106);
            this->Dimention1_textBox->MaxLength = 20;
            this->Dimention1_textBox->Name = L"Dimention1_textBox";
            this->Dimention1_textBox->Size = System::Drawing::Size(100, 21);
            this->Dimention1_textBox->TabIndex = 8;
            this->Dimention1_textBox->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &OrganizeForm::Dimention1_textBox_KeyPress);
            // 
            // Dimention2_textBox
            // 
            this->Dimention2_textBox->Location = System::Drawing::Point(115, 146);
            this->Dimention2_textBox->MaxLength = 20;
            this->Dimention2_textBox->Name = L"Dimention2_textBox";
            this->Dimention2_textBox->Size = System::Drawing::Size(100, 21);
            this->Dimention2_textBox->TabIndex = 9;
            this->Dimention2_textBox->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &OrganizeForm::Dimention2_textBox_KeyPress);
            // 
            // OrganizeForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(262, 231);
            this->Controls->Add(this->Dimention2_textBox);
            this->Controls->Add(this->Dimention1_textBox);
            this->Controls->Add(this->Name_textBox);
            this->Controls->Add(this->Cancel_button);
            this->Controls->Add(this->OK_button);
            this->Controls->Add(this->anywndComboBoxControl1);
            this->Controls->Add(this->label4);
            this->Controls->Add(this->label3);
            this->Controls->Add(this->label2);
            this->Controls->Add(this->label1);
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Name = L"OrganizeForm";
            this->Load += gcnew System::EventHandler(this, &OrganizeForm::OrganizeForm_Load);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
    private: System::Void OrganizeForm_Load(System::Object^  sender, System::EventArgs^  e) {
                 this->CenterToScreen();

                 m_OrganizeCheckedTreeListForm = gcnew OrganizeCheckedTreeListForm(m_nOrganizeID);
                 m_OrganizeCheckedTreeListForm->ResetRelateCombo(anywndComboBoxControl1);
                 anywndComboBoxControl1->ChildControl = m_OrganizeCheckedTreeListForm;

                 ResetComboBoxTextReadOnly(anywndComboBoxControl1);

                 if ( *m_pType == Oper_Add )
                 {
                     this->Text = "新增部门";
                     anywndComboBoxControl1->Text   = m_strUpperOrganize;
                 }
                 else
                 {
                     this->Text = "修改部门";
                     eTradeDataStatus eStatus = m_pDataInfo->GetTradeDataStatus();
                     if(eStatus != TradeDataStatusSettlemtEnd)
                        anywndComboBoxControl1->Enabled = false;

                     char szDiment1[128],szDiment2[128];
                     sprintf(szDiment1, "%.6f",m_pCurOrganize->dLongitude);
                     sprintf(szDiment2, "%.6f",m_pCurOrganize->dLatitude);

                     Name_textBox->Text             = Tools::string2String(m_pCurOrganize->szName);                 
                     Dimention1_textBox->Text       = Tools::string2String(szDiment1);
                     Dimention2_textBox->Text       = Tools::string2String(szDiment2);
                     anywndComboBoxControl1->Text   = m_strUpperOrganize;
                 }
             }
private: System::Void OK_button_Click(System::Object^  sender, System::EventArgs^  e) {    
             if(String::IsNullOrEmpty(Name_textBox->Text)) 
             {
                 MessageBox::Show("请输入组织机构名称!");  
                 return;
             }

             if(String::IsNullOrEmpty(Dimention1_textBox->Text)) 
             {
                 MessageBox::Show("请输入经度!");  
                 return;
             }

             if(String::IsNullOrEmpty(Dimention2_textBox->Text)) 
             {
                 MessageBox::Show("请输入纬度!");  
                 return;
             }

             m_strUpperOrganize = anywndComboBoxControl1->Text;

             Organization oG;
             if(*m_pType == Oper_Modify) oG.nAssetMgmtOrgID = m_pCurOrganize->nAssetMgmtOrgID;
             strcpy(oG.szName,Tools::String2string(Name_textBox->Text).c_str());
             oG.nScaleLevel = 1 + m_pDataInfo->GetOrganizationScaleLevelByID(m_pDataInfo->GetOrganizationIDByName(Tools::String2string(m_strUpperOrganize)));
             oG.nUpperLevelOrgID = m_pDataInfo->GetOrganizationIDByName(Tools::String2string(m_strUpperOrganize));
             oG.dLongitude = ::Convert::ToDouble(Dimention1_textBox->Text);
             oG.dLatitude = ::Convert::ToDouble(Dimention2_textBox->Text);  


             if(*m_pType==Oper_Add)
             {
                 if(!m_pDataInfo->IsValidOfOrganizationName(Tools::String2string(Name_textBox->Text)))    
                 {
                     MessageBox::Show("组织机构名称重名,请重新输入!");                 
                     return;
                 }

                 if (Regex::IsMatch(Name_textBox->Text, "^[0-9]*$"))//正则表达式 true： 纯数值是交易员 false：组织机构
                 {
                     MessageBox::Show("组织机构名称不能用纯数字,请重新输入!");                 
                     return;
                 }

                 memcpy(m_pCurOrganize,&oG,sizeof(Organization));

                 this->DialogResult = ::DialogResult::OK;
                 this->Close();
             }

             if(*m_pType==Oper_Modify)
             {
                 if(*m_pCurOrganize == oG)
                     this->DialogResult = ::DialogResult::Cancel;
                 else
                 {
                     if(0==strcmp(m_pCurOrganize->szName,oG.szName))
                     {
                         this->DialogResult = ::DialogResult::OK;
                         memcpy(m_pCurOrganize,&oG,sizeof(Organization));
                         this->Close();
                     }
                     else
                     {
                         if(!m_pDataInfo->IsValidOfOrganizationName(oG.szName))
                         {                             
                             MessageBox::Show("组织机构名称重名,请重新输入!"); 
                         }
                         else
                         {
                             this->DialogResult = ::DialogResult::OK;
                             memcpy(m_pCurOrganize,&oG,sizeof(Organization));
                             this->Close();
                         }
                     }
                 }                 
             }
         }
private: System::Void Cancel_button_Click(System::Object^  sender, System::EventArgs^  e) {
             this->Close();
         }
private: System::Void Dimention1_textBox_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
             String ^strText = Dimention1_textBox->Text;
             int nLen = strText->Length;
             int nCount = strText->IndexOf('.');

             if (e->KeyChar < '0' || e->KeyChar > '9')   
                 if (e->KeyChar != (char)8 && e->KeyChar != '.' || (nCount>0 && e->KeyChar=='.') || (nLen==0 && e->KeyChar=='.'))    
                     e->Handled = true;
         }
private: System::Void Dimention2_textBox_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
             String ^strText = Dimention2_textBox->Text;
             int nLen = strText->Length;
             int nCount = strText->IndexOf('.');

             if (e->KeyChar < '0' || e->KeyChar > '9')   
                 if (e->KeyChar != (char)8 && e->KeyChar != '.' || (nCount>0 && e->KeyChar=='.') || (nLen==0 && e->KeyChar=='.'))    
                     e->Handled = true;
         }
};
}
