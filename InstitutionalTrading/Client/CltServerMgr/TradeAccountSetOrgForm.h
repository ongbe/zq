#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "CDataInfo.h"
#include "Tools.h"
#include "CCommonFunc.h"
#include "OrganizeCheckedTreeListForm.h"

namespace CltServerMgr {

	/// <summary>
	/// Summary for TradeAccountSetOrgForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class TradeAccountSetOrgForm : public System::Windows::Forms::Form
	{
	public:
		TradeAccountSetOrgForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
            m_pDataInfo = CDataInfo::NewInstance();
            m_pTradeAccountInfo = new TradeAccount;
            m_relation = new TradeAccountAndOrg;

            memset(m_pTradeAccountInfo,0,sizeof(TradeAccount));
            memset(m_relation,0,sizeof(TradeAccountAndOrg));
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~TradeAccountSetOrgForm()
		{
			if (components)
			{
				delete components;
			}
            if (m_pTradeAccountInfo)
            {
                delete m_pTradeAccountInfo;
                m_pTradeAccountInfo = NULL;
            }
            if (m_relation)
            {
                delete m_relation;
                m_relation = NULL;
            }
		}
    private: System::Windows::Forms::Label^  label1;
    protected: 
    private: System::Windows::Forms::Label^  label2;
    private: System::Windows::Forms::Label^  label3;
    private: System::Windows::Forms::Button^  button1;
    private: System::Windows::Forms::Label^  label_TradeAccount;

    private: System::Windows::Forms::Label^  label_RealName;
    private: AnywndComboBox::AnywndComboBoxControl^  anywndComboBoxControl;


    private: CDataInfo* m_pDataInfo;
    private: TradeAccount* m_pTradeAccountInfo;
    private: TradeAccountAndOrg* m_relation;
    private: OrganizeCheckedTreeListForm^ m_OrganizeCheckedTreeListForm;

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
            this->button1 = (gcnew System::Windows::Forms::Button());
            this->label_TradeAccount = (gcnew System::Windows::Forms::Label());
            this->label_RealName = (gcnew System::Windows::Forms::Label());
            this->anywndComboBoxControl = (gcnew AnywndComboBox::AnywndComboBoxControl());
            this->SuspendLayout();
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(35, 45);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(89, 12);
            this->label1->TabIndex = 0;
            this->label1->Text = L"委托交易账号：";
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Location = System::Drawing::Point(35, 77);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(65, 12);
            this->label2->TabIndex = 0;
            this->label2->Text = L"真实姓名：";
            // 
            // label3
            // 
            this->label3->AutoSize = true;
            this->label3->Location = System::Drawing::Point(35, 109);
            this->label3->Name = L"label3";
            this->label3->Size = System::Drawing::Size(65, 12);
            this->label3->TabIndex = 0;
            this->label3->Text = L"指定部门：";
            // 
            // button1
            // 
            this->button1->Location = System::Drawing::Point(104, 180);
            this->button1->Name = L"button1";
            this->button1->Size = System::Drawing::Size(75, 23);
            this->button1->TabIndex = 1;
            this->button1->Text = L"确定";
            this->button1->UseVisualStyleBackColor = true;
            this->button1->Click += gcnew System::EventHandler(this, &TradeAccountSetOrgForm::button_OK_Click);
            // 
            // label_TradeAccount
            // 
            this->label_TradeAccount->AutoSize = true;
            this->label_TradeAccount->Location = System::Drawing::Point(154, 45);
            this->label_TradeAccount->Name = L"label_TradeAccount";
            this->label_TradeAccount->Size = System::Drawing::Size(113, 12);
            this->label_TradeAccount->TabIndex = 2;
            this->label_TradeAccount->Text = L"label_TradeAccount";
            // 
            // label_RealName
            // 
            this->label_RealName->AutoSize = true;
            this->label_RealName->Location = System::Drawing::Point(154, 77);
            this->label_RealName->Name = L"label_RealName";
            this->label_RealName->Size = System::Drawing::Size(89, 12);
            this->label_RealName->TabIndex = 3;
            this->label_RealName->Text = L"label_RealName";
            // 
            // anywndComboBoxControl
            // 
            this->anywndComboBoxControl->BackColor = System::Drawing::Color::White;
            this->anywndComboBoxControl->ChildControl = nullptr;
            this->anywndComboBoxControl->DropDownHeight = 1;
            this->anywndComboBoxControl->DropDownWidth = 1;
            this->anywndComboBoxControl->FormattingEnabled = true;
            this->anywndComboBoxControl->IntegralHeight = false;
            this->anywndComboBoxControl->Location = System::Drawing::Point(156, 101);
            this->anywndComboBoxControl->MaxDropDownItems = 1;
            this->anywndComboBoxControl->Name = L"anywndComboBoxControl";
            this->anywndComboBoxControl->Size = System::Drawing::Size(111, 20);
            this->anywndComboBoxControl->TabIndex = 8;
            this->anywndComboBoxControl->SelectedIndexChanged += gcnew System::EventHandler(this, &TradeAccountSetOrgForm::anywndComboBoxControl_SelectedIndexChanged);
            // 
            // TradeAccountSetOrgForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(292, 243);
            this->Controls->Add(this->anywndComboBoxControl);
            this->Controls->Add(this->label_RealName);
            this->Controls->Add(this->label_TradeAccount);
            this->Controls->Add(this->button1);
            this->Controls->Add(this->label3);
            this->Controls->Add(this->label2);
            this->Controls->Add(this->label1);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Name = L"TradeAccountSetOrgForm";
            this->Text = L"设置组织机构";
            this->Load += gcnew System::EventHandler(this, &TradeAccountSetOrgForm::TradeAccountSetOrgForm_Load);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
    private: System::Void TradeAccountSetOrgForm_Load(System::Object^  sender, System::EventArgs^  e) {
                 m_OrganizeCheckedTreeListForm = gcnew OrganizeCheckedTreeListForm();             
                 m_OrganizeCheckedTreeListForm->ResetRelateCombo(anywndComboBoxControl);
                 anywndComboBoxControl->ChildControl = m_OrganizeCheckedTreeListForm; 

                 ResetComboBoxTextReadOnly(anywndComboBoxControl);

                 label_TradeAccount->Text = Tools::string2String(m_pTradeAccountInfo->szTradeAccount);
                 label_RealName->Text = Tools::string2String(m_pTradeAccountInfo->szRealName);
                 anywndComboBoxControl->Text = Tools::string2String(m_pDataInfo->GetOrgByTradeAccountID(m_pTradeAccountInfo->nTradeAccountID).c_str());    

                 this->CenterToParent();
             }
private: System::Void button_OK_Click(System::Object^  sender, System::EventArgs^  e) {
             this->DialogResult = ::DialogResult::OK;
             Close();
         }
private: System::Void anywndComboBoxControl_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
             if(String::IsNullOrEmpty(anywndComboBoxControl->Text))
                 return;

             int nOrgID = m_pDataInfo->GetOrganizationIDByName(Tools::String2string(anywndComboBoxControl->Text));
             
             m_relation->nOrgID = nOrgID;
             m_relation->nTradeAccountID = m_pTradeAccountInfo->nTradeAccountID;
         }

public: 
    void SetTradeAccountInfo(const TradeAccount& tradeAccount)
    {
        memcpy(m_pTradeAccountInfo,&tradeAccount,sizeof(TradeAccount));
    }
    void GetTradeAccountAndOrgRelation(TradeAccountAndOrg& relation)
    {
        memcpy(&relation,m_relation,sizeof(TradeAccountAndOrg));
    }
};
}
