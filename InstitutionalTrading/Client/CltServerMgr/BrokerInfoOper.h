#pragma once

#include "CommonStruct.h"
#include "Tools.h"
#include "CDataInfo.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace CltServerMgr {

	/// <summary>
	/// Summary for BrokerInfoOper
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class BrokerInfoOper : public System::Windows::Forms::Form
	{
	public:
		BrokerInfoOper(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_pType = new OperType();
			m_pCurBrokerInfo = new BrokerInfo;
            m_pDataInfo = CDataInfo::NewInstance();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~BrokerInfoOper()
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

			if ( NULL != m_pCurBrokerInfo )
			{
				delete m_pCurBrokerInfo;
				m_pCurBrokerInfo = NULL;
			}
        }	
    
    private: OperType* m_pType;
    private: BrokerInfo* m_pCurBrokerInfo;
    private: CDataInfo* m_pDataInfo;

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  textBoxCode;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  textBoxCompany;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button2;








    protected: 

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
            this->textBoxCode = (gcnew System::Windows::Forms::TextBox());
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->textBoxCompany = (gcnew System::Windows::Forms::TextBox());
            this->button1 = (gcnew System::Windows::Forms::Button());
            this->button2 = (gcnew System::Windows::Forms::Button());
            this->SuspendLayout();
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(25, 24);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(77, 12);
            this->label1->TabIndex = 0;
            this->label1->Text = L"经纪公司代码";
            // 
            // textBoxCode
            // 
            this->textBoxCode->Location = System::Drawing::Point(118, 21);
            this->textBoxCode->MaxLength = 11;
            this->textBoxCode->Name = L"textBoxCode";
            this->textBoxCode->Size = System::Drawing::Size(162, 21);
            this->textBoxCode->TabIndex = 1;
            this->textBoxCode->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &BrokerInfoOper::textBoxCode_KeyPress);
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Location = System::Drawing::Point(25, 58);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(77, 12);
            this->label2->TabIndex = 2;
            this->label2->Text = L"经纪公司名称";
            // 
            // textBoxCompany
            // 
            this->textBoxCompany->Location = System::Drawing::Point(118, 55);
            this->textBoxCompany->MaxLength = 10;
            this->textBoxCompany->Multiline = true;
            this->textBoxCompany->Name = L"textBoxCompany";
            this->textBoxCompany->Size = System::Drawing::Size(162, 75);
            this->textBoxCompany->TabIndex = 3;
            // 
            // button1
            // 
            this->button1->Location = System::Drawing::Point(81, 154);
            this->button1->Name = L"button1";
            this->button1->Size = System::Drawing::Size(57, 23);
            this->button1->TabIndex = 4;
            this->button1->Text = L"确定";
            this->button1->UseVisualStyleBackColor = true;
            this->button1->Click += gcnew System::EventHandler(this, &BrokerInfoOper::button1_Click);
            // 
            // button2
            // 
            this->button2->DialogResult = System::Windows::Forms::DialogResult::Cancel;
            this->button2->Location = System::Drawing::Point(161, 154);
            this->button2->Name = L"button2";
            this->button2->Size = System::Drawing::Size(57, 23);
            this->button2->TabIndex = 5;
            this->button2->Text = L"取消";
            this->button2->UseVisualStyleBackColor = true;
            this->button2->Click += gcnew System::EventHandler(this, &BrokerInfoOper::button2_Click);
            // 
            // BrokerInfoOper
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(316, 189);
            this->Controls->Add(this->button2);
            this->Controls->Add(this->button1);
            this->Controls->Add(this->textBoxCompany);
            this->Controls->Add(this->label2);
            this->Controls->Add(this->textBoxCode);
            this->Controls->Add(this->label1);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Name = L"BrokerInfoOper";
            this->Text = L"新增经纪公司";
            this->Load += gcnew System::EventHandler(this, &BrokerInfoOper::BrokerInfoOper_Load);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion

	public:
		void SetOperType(OperType eType)
		{
			*m_pType = eType;
		}

		void SetCurBrokerInfo(const BrokerInfo& brokerInfo)
		{
			*m_pCurBrokerInfo = brokerInfo;
		}

		BrokerInfo* GetCurBrokerInfo()
		{
			return m_pCurBrokerInfo;
		}

	private: System::Void BrokerInfoOper_Load(System::Object^  sender, System::EventArgs^  e) {
				 this->CenterToParent();

				 if ( *m_pType == Oper_Add_Quot )
				 {
					 this->Text = "新增经纪公司";

//                      labelAccount->Visible = true;
//                      labelPSW->Visible = true;
//                      textBoxAccount->Visible = true;
//                      textBoxPSW->Visible = true;
				 }

                 if (*m_pType == Oper_Add_Trade)
                 {
//                      labelAccount->Visible = false;
//                      labelPSW->Visible = false;
//                      textBoxAccount->Visible = false;
//                      textBoxPSW->Visible = false;
                 }
				 
                 if (*m_pType == Oper_Modify_Quot)
				 {
					 this->Text = "修改经纪公司";

//                      labelAccount->Visible = true;
//                      labelPSW->Visible = true;
//                      textBoxAccount->Visible = true;
//                      textBoxPSW->Visible = true;

					 textBoxCode->Text = Tools::string2String(m_pCurBrokerInfo->szBrokerCode);					 
					 textBoxCompany->Text = Tools::string2String(m_pCurBrokerInfo->szCompany);
//                      textBoxAccount->Text = Tools::string2String(m_pCurBrokerInfo->szAccount);
//                      textBoxPSW->Text = Tools::string2String(m_pCurBrokerInfo->szPassword);
				 }

                 if (*m_pType == Oper_Modify_Trade)
                 {
                     this->Text = "修改经纪公司";

//                      labelAccount->Visible = false;
//                      labelPSW->Visible = false;
//                      textBoxAccount->Visible = false;
//                      textBoxPSW->Visible = false;

                     textBoxCode->Text = Tools::string2String(m_pCurBrokerInfo->szBrokerCode);
                     textBoxCompany->Text = Tools::string2String(m_pCurBrokerInfo->szCompany);
                 }

			 }
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
             if(String::IsNullOrEmpty(textBoxCode->Text)) 
             {
                 MessageBox::Show("经纪公司代码不能为空!");  
                 return;
             }

             BrokerInfo oBI;
             memset(&oBI,0,sizeof(BrokerInfo));

             if(*m_pType == Oper_Modify_Quot) 
             {
//                  if(String::IsNullOrEmpty(textBoxAccount->Text)) 
//                  {
//                      MessageBox::Show("账户不能为空!");  
//                      return;
//                  }

//                  if(String::IsNullOrEmpty(textBoxPSW->Text)) 
//                  {
//                      MessageBox::Show("密码不能为空!");  
//                      return;
//                  }

                 oBI.nBrokerID = m_pCurBrokerInfo->nBrokerID;
//                  strcpy(oBI.szAccount,Tools::String2string(textBoxAccount->Text).c_str());
//                  strcpy(oBI.szPassword,Tools::String2string(textBoxPSW->Text).c_str()); 
             }

             if (*m_pType == Oper_Modify_Trade)
             {
                 oBI.nBrokerID = m_pCurBrokerInfo->nBrokerID;
             }
             strcpy(oBI.szBrokerCode,Tools::String2string(textBoxCode->Text).c_str());
             strcpy(oBI.szCompany,Tools::String2string(textBoxCompany->Text).c_str()); 

             if(*m_pType==Oper_Add_Trade)
             {
                 if(!m_pDataInfo->IsValidOfBrokerCode(Tools::String2string(textBoxCode->Text)))    
                 {
                     MessageBox::Show("经纪公司代码重复,请重新输入!");                 
                     return;
                 }

                 oBI.nType = 0;
                 memcpy(m_pCurBrokerInfo,&oBI,sizeof(BrokerInfo));

                 this->DialogResult = ::DialogResult::OK;
                 this->Close();
             }

             if(*m_pType==Oper_Add_Quot)
             {
                 if(!m_pDataInfo->IsValidOfBrokerCode(Tools::String2string(textBoxCode->Text)))    
                 {
                     MessageBox::Show("经纪公司代码重复,请重新输入!");                 
                     return;
                 }

                 oBI.nType = 1;
                 memcpy(m_pCurBrokerInfo,&oBI,sizeof(BrokerInfo));

                 this->DialogResult = ::DialogResult::OK;
                 this->Close();
             }

             if(*m_pType==Oper_Modify_Trade || *m_pType==Oper_Modify_Quot)
             {
                 if(*m_pCurBrokerInfo == oBI)
                     this->DialogResult = ::DialogResult::Cancel;
                 else
                 {
                     if(0==strcmp(m_pCurBrokerInfo->szBrokerCode,oBI.szBrokerCode))
                     {
                         this->DialogResult = ::DialogResult::OK;
                         memcpy(m_pCurBrokerInfo,&oBI,sizeof(BrokerInfo));
                         this->Close();
                     }
                     else
                     {
                         if(!m_pDataInfo->IsValidOfBrokerCode(oBI.szBrokerCode))
                         {                             
                             MessageBox::Show("经纪公司代码重复,请重新输入!"); 
                         }
                         else
                         {
                             this->DialogResult = ::DialogResult::OK;
                             memcpy(m_pCurBrokerInfo,&oBI,sizeof(BrokerInfo));
                             this->Close();
                         }
                     }
                 }                 
             }
		 }
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->Close();

		 }
private: System::Void textBoxCode_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
             if (e->KeyChar < '0' || e->KeyChar > '9')   
                 if (e->KeyChar != (char)8)    
                     e->Handled = true;
         }
};
}
