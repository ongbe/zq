#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;



#include <map>
using namespace std;

#include "Tools.h"
#include "CDataInfo.h"



namespace CltServerMgr {

	/// <summary>
	/// Summary for TradeStrategyForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class TradeStrategyForm : public System::Windows::Forms::Form
	{
	public:
		TradeStrategyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
            m_pType = new OperType;
            m_pCurTradeStrategy = new TradeStrategy;
            m_pDataInfo = CDataInfo::NewInstance();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~TradeStrategyForm()
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

            if ( NULL != m_pCurTradeStrategy )
            {
                delete m_pCurTradeStrategy;
                m_pCurTradeStrategy = NULL;
            }
		}

    private: CDataInfo* m_pDataInfo;
    private: OperType* m_pType;
    private: TradeStrategy* m_pCurTradeStrategy;   


    private: System::Windows::Forms::Button^  FolderBrose_button;
    private: System::Windows::Forms::FolderBrowserDialog^  folderBrowserDialog;
    private: System::Windows::Forms::Label^  Name_label;
    private: System::Windows::Forms::Label^  Frequency_label;
    private: System::Windows::Forms::Label^  Addr_label;
    private: System::Windows::Forms::TextBox^  Name_textBox;
    private: System::Windows::Forms::TextBox^  Frequency_textBox;
    private: System::Windows::Forms::TextBox^  Addr_textBox;
    private: System::Windows::Forms::Button^  OK_button;
    private: System::Windows::Forms::Button^  Cancel_button;
    private: System::Windows::Forms::Label^  label1;

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
        void SetCurTradeStrategy(const TradeStrategy& oTradeStrategy)
        {
            *m_pCurTradeStrategy = oTradeStrategy;
        }

        TradeStrategy* GetCurTradeStrategy()
        {
            return m_pCurTradeStrategy;
        }

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            this->Name_label = (gcnew System::Windows::Forms::Label());
            this->Name_textBox = (gcnew System::Windows::Forms::TextBox());
            this->OK_button = (gcnew System::Windows::Forms::Button());
            this->Cancel_button = (gcnew System::Windows::Forms::Button());
            this->Addr_textBox = (gcnew System::Windows::Forms::TextBox());
            this->Addr_label = (gcnew System::Windows::Forms::Label());
            this->Frequency_label = (gcnew System::Windows::Forms::Label());
            this->Frequency_textBox = (gcnew System::Windows::Forms::TextBox());
            this->folderBrowserDialog = (gcnew System::Windows::Forms::FolderBrowserDialog());
            this->FolderBrose_button = (gcnew System::Windows::Forms::Button());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->SuspendLayout();
            // 
            // Name_label
            // 
            this->Name_label->AutoSize = true;
            this->Name_label->Location = System::Drawing::Point(18, 25);
            this->Name_label->Name = L"Name_label";
            this->Name_label->Size = System::Drawing::Size(41, 12);
            this->Name_label->TabIndex = 0;
            this->Name_label->Text = L"名称：";
            // 
            // Name_textBox
            // 
            this->Name_textBox->Location = System::Drawing::Point(86, 20);
            this->Name_textBox->MaxLength = 11;
            this->Name_textBox->Name = L"Name_textBox";
            this->Name_textBox->Size = System::Drawing::Size(101, 21);
            this->Name_textBox->TabIndex = 1;
            // 
            // OK_button
            // 
            this->OK_button->Location = System::Drawing::Point(87, 133);
            this->OK_button->Name = L"OK_button";
            this->OK_button->Size = System::Drawing::Size(75, 23);
            this->OK_button->TabIndex = 5;
            this->OK_button->Text = L"确定";
            this->OK_button->UseVisualStyleBackColor = true;
            this->OK_button->Click += gcnew System::EventHandler(this, &TradeStrategyForm::OK_button_Click);
            // 
            // Cancel_button
            // 
            this->Cancel_button->DialogResult = System::Windows::Forms::DialogResult::Cancel;
            this->Cancel_button->Location = System::Drawing::Point(186, 133);
            this->Cancel_button->Name = L"Cancel_button";
            this->Cancel_button->Size = System::Drawing::Size(75, 23);
            this->Cancel_button->TabIndex = 6;
            this->Cancel_button->Text = L"取消";
            this->Cancel_button->UseVisualStyleBackColor = true;
            this->Cancel_button->Click += gcnew System::EventHandler(this, &TradeStrategyForm::Cancel_button_Click);
            // 
            // Addr_textBox
            // 
            this->Addr_textBox->Location = System::Drawing::Point(87, 90);
            this->Addr_textBox->Name = L"Addr_textBox";
            this->Addr_textBox->Size = System::Drawing::Size(209, 21);
            this->Addr_textBox->TabIndex = 3;
            // 
            // Addr_label
            // 
            this->Addr_label->AutoSize = true;
            this->Addr_label->Location = System::Drawing::Point(18, 95);
            this->Addr_label->Name = L"Addr_label";
            this->Addr_label->Size = System::Drawing::Size(65, 12);
            this->Addr_label->TabIndex = 7;
            this->Addr_label->Text = L"绝对路径：";
            // 
            // Frequency_label
            // 
            this->Frequency_label->AutoSize = true;
            this->Frequency_label->Location = System::Drawing::Point(18, 59);
            this->Frequency_label->Name = L"Frequency_label";
            this->Frequency_label->Size = System::Drawing::Size(47, 12);
            this->Frequency_label->TabIndex = 8;
            this->Frequency_label->Text = L"频率： ";
            // 
            // Frequency_textBox
            // 
            this->Frequency_textBox->Location = System::Drawing::Point(87, 56);
            this->Frequency_textBox->MaxLength = 4;
            this->Frequency_textBox->Name = L"Frequency_textBox";
            this->Frequency_textBox->Size = System::Drawing::Size(100, 21);
            this->Frequency_textBox->TabIndex = 2;
            this->Frequency_textBox->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &TradeStrategyForm::Frequency_textBox_KeyPress);
            // 
            // FolderBrose_button
            // 
            this->FolderBrose_button->Location = System::Drawing::Point(303, 90);
            this->FolderBrose_button->Name = L"FolderBrose_button";
            this->FolderBrose_button->Size = System::Drawing::Size(36, 23);
            this->FolderBrose_button->TabIndex = 4;
            this->FolderBrose_button->Text = L"...";
            this->FolderBrose_button->UseVisualStyleBackColor = true;
            this->FolderBrose_button->Click += gcnew System::EventHandler(this, &TradeStrategyForm::FolderBrose_button_Click);
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(192, 61);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(17, 12);
            this->label1->TabIndex = 9;
            this->label1->Text = L"秒";
            // 
            // TradeStrategyForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(351, 168);
            this->Controls->Add(this->label1);
            this->Controls->Add(this->FolderBrose_button);
            this->Controls->Add(this->Frequency_textBox);
            this->Controls->Add(this->Frequency_label);
            this->Controls->Add(this->Addr_label);
            this->Controls->Add(this->Addr_textBox);
            this->Controls->Add(this->Cancel_button);
            this->Controls->Add(this->OK_button);
            this->Controls->Add(this->Name_textBox);
            this->Controls->Add(this->Name_label);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Name = L"TradeStrategyForm";
            this->Text = L"交易策略";
            this->Load += gcnew System::EventHandler(this, &TradeStrategyForm::TradeStrategyForm_Load);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
    private: System::Void TradeStrategyForm_Load(System::Object^  sender, System::EventArgs^  e) {
                 this->CenterToParent();

                 if ( *m_pType == Oper_Add )
                 {
                     this->Text = "新增交易策略";
                 }
                 else
                 {
                     this->Text = "修改交易策略";

                     char temp[20] = {0};
                     ::itoa(m_pCurTradeStrategy->nFrequency,temp,10);

                     Name_textBox->Text  = Tools::string2String(m_pCurTradeStrategy->szName);                 
                     Frequency_textBox->Text  = Tools::string2String(temp);
                     Addr_textBox->Text = Tools::string2String(m_pCurTradeStrategy->szAddr);
                 }
             }
    private: System::Void OK_button_Click(System::Object^  sender, System::EventArgs^  e) { 
                 if(String::IsNullOrEmpty(Name_textBox->Text)) 
                 {
                     MessageBox::Show("请输入交易策略名称!");  
                     return;
                 }

                 if(String::IsNullOrEmpty(Frequency_textBox->Text)) 
                 {
                     MessageBox::Show("请输入频率!");  
                     return;
                 }

                 TradeStrategy oTS;
                 if(*m_pType == Oper_Modify) oTS.nID = m_pCurTradeStrategy->nID;
                 strcpy(oTS.szName,Tools::String2string(Name_textBox->Text).c_str());
                 strcpy(oTS.szAddr,Tools::String2string(Addr_textBox->Text).c_str());
                 oTS.nFrequency = ::Convert::ToInt32(Frequency_textBox->Text);

                 if(*m_pType==Oper_Add)
                 {
                     if(!m_pDataInfo->IsValidOfStrategyName(Tools::String2string(Name_textBox->Text)))    
                     {
                         MessageBox::Show("交易策略名称重名,请重新输入!");                 
                         return;
                     }

                     memcpy(m_pCurTradeStrategy,&oTS,sizeof(TradeStrategy));

                     this->DialogResult = ::DialogResult::OK;
                     this->Close();
                 }

                 if(*m_pType==Oper_Modify)
                 {
                     if(*m_pCurTradeStrategy == oTS)
                         this->DialogResult = ::DialogResult::Cancel;
                     else
                     {
                         if(0==strcmp(m_pCurTradeStrategy->szName,oTS.szName))
                         {
                             this->DialogResult = ::DialogResult::OK;
                             memcpy(m_pCurTradeStrategy,&oTS,sizeof(TradeStrategy));
                             this->Close();
                         }
                         else
                         {
                             if(!m_pDataInfo->IsValidOfStrategyName(oTS.szName))
                             {                             
                                 MessageBox::Show("交易策略名称重名,请重新输入!"); 
                             }
                             else
                             {
                                 this->DialogResult = ::DialogResult::OK;
                                 memcpy(m_pCurTradeStrategy,&oTS,sizeof(TradeStrategy));
                                 this->Close();
                             }
                         }
                     }                 
                 }
             }
    private: System::Void Cancel_button_Click(System::Object^  sender, System::EventArgs^  e) {
                 this->Close();
             }
    private: System::Void FolderBrose_button_Click(System::Object^  sender, System::EventArgs^  e) {                 
                 if(this->folderBrowserDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
                 {
                     String^ strPath = this->folderBrowserDialog->SelectedPath;
                     Addr_textBox->Text = strPath;
                 }
             }
    private: System::Void Frequency_textBox_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
                 if (e->KeyChar < '0' || e->KeyChar > '9')   
                     if (e->KeyChar != (char)8)    
                         e->Handled = true;
             }
};
}
