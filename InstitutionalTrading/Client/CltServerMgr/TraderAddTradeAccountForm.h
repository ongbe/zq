#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "CommonStruct.h"
#include "CCommonFunc.h"
#include "CDataInfo.h"
#include "Tools.h"


namespace CltServerMgr {

	/// <summary>
	/// Summary for TraderAddTradeAccountForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class TraderAddTradeAccountForm : public System::Windows::Forms::Form
	{
	public:
		TraderAddTradeAccountForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//

            m_pCurUserInfo = new UserInfo;
            m_pDataInfo = CDataInfo::NewInstance();
            m_pCurUserAndTradeAccountRelation=new UserAndTradeAccountRelation;
            memset(m_pCurUserAndTradeAccountRelation,0,sizeof(UserAndTradeAccountRelation));
            m_nTradeAccountSelectedIndex=0;
            m_bInit=false;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~TraderAddTradeAccountForm()
		{
			if (components)
			{
				delete components;
			}

            if(m_pCurUserAndTradeAccountRelation)
                delete m_pCurUserAndTradeAccountRelation;
		}

    private: bool  m_bInit;
    private: CDataInfo* m_pDataInfo;
    private: UserInfo* m_pCurUserInfo;   
    private: UserAndTradeAccountRelation* m_pCurUserAndTradeAccountRelation;
    private: int m_nTradeAccountSelectedIndex;

    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::Label^  Account_label;
    private: System::Windows::Forms::Label^  label3;
    private: System::Windows::Forms::Label^  Name_label;
    private: System::Windows::Forms::Label^  TradeAccount_label;
    private: System::Windows::Forms::CheckBox^  Order_checkBox;
    private: System::Windows::Forms::Button^  OK_button;
    private: System::Windows::Forms::CheckBox^  Verify_checkBox;
    private: System::Windows::Forms::ComboBox^  TradeAccount_comboBox;
    private: System::Windows::Forms::Button^  Cancel_button;

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

        void SetCurRelation(const UserAndTradeAccountRelation& CurTradeAccountRelation)
        {
            *m_pCurUserAndTradeAccountRelation = CurTradeAccountRelation;
        }

        String^ GetCurTradeAccount()
        {
            return TradeAccount_comboBox->Text;
        }

        bool GetBoolOrder()
        {
            return Order_checkBox->Checked;
        }

        bool GetBoolVerify()
        {
            return Verify_checkBox->Checked;
        }
        
#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->Account_label = (gcnew System::Windows::Forms::Label());
            this->label3 = (gcnew System::Windows::Forms::Label());
            this->Name_label = (gcnew System::Windows::Forms::Label());
            this->TradeAccount_label = (gcnew System::Windows::Forms::Label());
            this->Order_checkBox = (gcnew System::Windows::Forms::CheckBox());
            this->OK_button = (gcnew System::Windows::Forms::Button());
            this->Verify_checkBox = (gcnew System::Windows::Forms::CheckBox());
            this->TradeAccount_comboBox = (gcnew System::Windows::Forms::ComboBox());
            this->Cancel_button = (gcnew System::Windows::Forms::Button());
            this->SuspendLayout();
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(28, 13);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(29, 12);
            this->label1->TabIndex = 0;
            this->label1->Text = L"账号";
            // 
            // Account_label
            // 
            this->Account_label->AutoSize = true;
            this->Account_label->Location = System::Drawing::Point(130, 13);
            this->Account_label->Name = L"Account_label";
            this->Account_label->Size = System::Drawing::Size(0, 12);
            this->Account_label->TabIndex = 1;
            // 
            // label3
            // 
            this->label3->AutoSize = true;
            this->label3->Location = System::Drawing::Point(28, 44);
            this->label3->Name = L"label3";
            this->label3->Size = System::Drawing::Size(41, 12);
            this->label3->TabIndex = 2;
            this->label3->Text = L"用户名";
            // 
            // Name_label
            // 
            this->Name_label->AutoSize = true;
            this->Name_label->Location = System::Drawing::Point(130, 43);
            this->Name_label->Name = L"Name_label";
            this->Name_label->Size = System::Drawing::Size(0, 12);
            this->Name_label->TabIndex = 3;
            // 
            // TradeAccount_label
            // 
            this->TradeAccount_label->AutoSize = true;
            this->TradeAccount_label->Location = System::Drawing::Point(28, 79);
            this->TradeAccount_label->Name = L"TradeAccount_label";
            this->TradeAccount_label->Size = System::Drawing::Size(77, 12);
            this->TradeAccount_label->TabIndex = 4;
            this->TradeAccount_label->Text = L"委托交易账号";
            // 
            // Order_checkBox
            // 
            this->Order_checkBox->AutoSize = true;
            this->Order_checkBox->Location = System::Drawing::Point(30, 116);
            this->Order_checkBox->Name = L"Order_checkBox";
            this->Order_checkBox->Size = System::Drawing::Size(96, 16);
            this->Order_checkBox->TabIndex = 5;
            this->Order_checkBox->Text = L"拥有下单权限";
            this->Order_checkBox->UseVisualStyleBackColor = true;
            // 
            // OK_button
            // 
            this->OK_button->DialogResult = System::Windows::Forms::DialogResult::OK;
            this->OK_button->Location = System::Drawing::Point(30, 196);
            this->OK_button->Name = L"OK_button";
            this->OK_button->Size = System::Drawing::Size(75, 23);
            this->OK_button->TabIndex = 6;
            this->OK_button->Text = L"确定";
            this->OK_button->UseVisualStyleBackColor = true;
            this->OK_button->Click += gcnew System::EventHandler(this, &TraderAddTradeAccountForm::OK_button_Click);
            // 
            // Verify_checkBox
            // 
            this->Verify_checkBox->AutoSize = true;
            this->Verify_checkBox->Location = System::Drawing::Point(30, 151);
            this->Verify_checkBox->Name = L"Verify_checkBox";
            this->Verify_checkBox->Size = System::Drawing::Size(156, 16);
            this->Verify_checkBox->TabIndex = 7;
            this->Verify_checkBox->Text = L"所有下单均需要人工审核";
            this->Verify_checkBox->UseVisualStyleBackColor = true;
            this->Verify_checkBox->CheckedChanged += gcnew System::EventHandler(this, &TraderAddTradeAccountForm::Verify_checkBox_CheckedChanged);
            // 
            // TradeAccount_comboBox
            // 
            this->TradeAccount_comboBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
            this->TradeAccount_comboBox->FormattingEnabled = true;
            this->TradeAccount_comboBox->Location = System::Drawing::Point(110, 74);
            this->TradeAccount_comboBox->Name = L"TradeAccount_comboBox";
            this->TradeAccount_comboBox->Size = System::Drawing::Size(104, 20);
            this->TradeAccount_comboBox->TabIndex = 8;
            this->TradeAccount_comboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &TraderAddTradeAccountForm::TradeAccount_comboBox_SelectedIndexChanged);
            // 
            // Cancel_button
            // 
            this->Cancel_button->DialogResult = System::Windows::Forms::DialogResult::Cancel;
            this->Cancel_button->Location = System::Drawing::Point(122, 196);
            this->Cancel_button->Name = L"Cancel_button";
            this->Cancel_button->Size = System::Drawing::Size(75, 23);
            this->Cancel_button->TabIndex = 9;
            this->Cancel_button->Text = L"取消";
            this->Cancel_button->UseVisualStyleBackColor = true;
            this->Cancel_button->Click += gcnew System::EventHandler(this, &TraderAddTradeAccountForm::Cancel_button_Click);
            // 
            // TraderAddTradeAccountForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(227, 231);
            this->Controls->Add(this->Cancel_button);
            this->Controls->Add(this->TradeAccount_comboBox);
            this->Controls->Add(this->Verify_checkBox);
            this->Controls->Add(this->OK_button);
            this->Controls->Add(this->Order_checkBox);
            this->Controls->Add(this->TradeAccount_label);
            this->Controls->Add(this->Name_label);
            this->Controls->Add(this->label3);
            this->Controls->Add(this->Account_label);
            this->Controls->Add(this->label1);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Name = L"TraderAddTradeAccountForm";
            this->Text = L"指定委托交易账号";
            this->Load += gcnew System::EventHandler(this, &TraderAddTradeAccountForm::TraderAddTradeAccountForm_Load);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
private: System::Void TraderAddTradeAccountForm_Load(System::Object^  sender, System::EventArgs^  e) {
             
             this->CenterToScreen();

             Account_label->Text = Tools::string2String(m_pCurUserInfo->szAccount);
             Name_label->Text    = Tools::string2String(m_pCurUserInfo->szAlias);
             
             m_bInit=true;
             Order_checkBox->Checked = (m_pCurUserAndTradeAccountRelation->nCanInputOrder)?true:false;
             Verify_checkBox->Checked = (m_pCurUserAndTradeAccountRelation->nNeedVerify)?true:false;
             

             LoadTradeAccount();
             m_bInit=false;
         }
private: System::Void OK_button_Click(System::Object^  sender, System::EventArgs^  e) {

             m_pCurUserAndTradeAccountRelation->nCanInputOrder = Order_checkBox->Checked;
             m_pCurUserAndTradeAccountRelation->nNeedVerify = Verify_checkBox->Checked;

             if(::DialogResult::OK == MessageBox::Show("关系一旦创建，只有在每日结算后，且该交易员持仓为空的情况下才允许修改，请再次确认。", "警告", MessageBoxButtons::OKCancel,  MessageBoxIcon::Warning))
             {
                 this->DialogResult = ::DialogResult::OK;
                 this->Close();
             }
         }
private: void LoadTradeAccount()
         {
            TradeAccount_comboBox->Items->Clear();
            TradeAccount_comboBox->Items->Add("");

            string strTradeAcc = m_pDataInfo->GetTradeAccountByID(m_pCurUserAndTradeAccountRelation->nTradeAccountID);

            std::map<int,TradeAccount> mapTradeAccount;
            m_pDataInfo->GetTradeAccount(mapTradeAccount);

            std::map<int,TradeAccount>::iterator it = mapTradeAccount.begin();
            for (int i=0; it!=mapTradeAccount.end(); it++)
            {
                ++i;
                TradeAccount_comboBox->Items->Add(Tools::string2String(it->second.szTradeAccount));          
                if(0==strcmp(it->second.szTradeAccount,strTradeAcc.c_str()))
                    m_nTradeAccountSelectedIndex = i;
            }

            TradeAccount_comboBox->SelectedIndex = m_nTradeAccountSelectedIndex;

         }
private: System::Void Cancel_button_Click(System::Object^  sender, System::EventArgs^  e) {
             this->Close();
         }
private: System::Void TradeAccount_comboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
             Order_checkBox->Checked = false;
             Verify_checkBox->Checked = false;
             
             if(TradeAccount_comboBox->SelectedIndex == 0)
             {
                 Order_checkBox->Enabled = false;
                 Verify_checkBox->Enabled = false;
             }
             else
             {     
                 Order_checkBox->Enabled = true;
                 Verify_checkBox->Enabled = true;

                 if(TradeAccount_comboBox->SelectedIndex == m_nTradeAccountSelectedIndex)
                 {
                     Order_checkBox->Checked = (m_pCurUserAndTradeAccountRelation->nCanInputOrder)?true:false;
                     Verify_checkBox->Checked = (m_pCurUserAndTradeAccountRelation->nNeedVerify)?true:false;
                 }


             }
         }
private: System::Void Verify_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
             if(Verify_checkBox->Checked && !m_bInit)
                 MessageBox::Show("如勾选此按钮，则该交易员的所有下单，均需通过人工审核后才能生成报单。如未分配风控员，或风控员没有立即审核，则可能导致无法正常下单或下单延误！需考虑其风险。", "警告", MessageBoxButtons::OK,  MessageBoxIcon::Warning);
         }
};
}
