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
	/// Summary for ServerGroupForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class ServerGroupForm : public System::Windows::Forms::Form
	{
	public:
		ServerGroupForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
            m_nBrokerID=-1;
            m_bValidSettle=true;
            m_pType = new OperType;
            m_pCurServerGroup = new ServerGroup;
            m_pDataInfo = CDataInfo::NewInstance();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ServerGroupForm()
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

            if ( NULL != m_pCurServerGroup )
            {
                delete m_pCurServerGroup;
                m_pCurServerGroup = NULL;
            }
		}

    private: bool m_bValidSettle;
    private: CDataInfo* m_pDataInfo;
    private: OperType* m_pType;
    private: int m_nBrokerID;
    private: ServerGroup* m_pCurServerGroup;  

    public:
        void SetOperType(OperType eType)
        {
            *m_pType = eType;
        }
        void SetCurServerGroup(const ServerGroup& oServerGroup)
        {
            *m_pCurServerGroup = oServerGroup;
        }

        void SetBrokerID(int nBrokerID)
        {
            m_nBrokerID = nBrokerID;
        }
        ServerGroup* GetCurServerGroup()
        {
            return m_pCurServerGroup;
        }

    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::TextBox^  textBox_ServerName;
    private: System::Windows::Forms::CheckBox^  checkBox_Settle;
    private: System::Windows::Forms::Button^  button_OK;
    private: System::Windows::Forms::Button^  button_Cancle;
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
            this->textBox_ServerName = (gcnew System::Windows::Forms::TextBox());
            this->checkBox_Settle = (gcnew System::Windows::Forms::CheckBox());
            this->button_OK = (gcnew System::Windows::Forms::Button());
            this->button_Cancle = (gcnew System::Windows::Forms::Button());
            this->SuspendLayout();
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(11, 30);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(113, 12);
            this->label1->TabIndex = 0;
            this->label1->Text = L"交易服务器组名称：";
            // 
            // textBox_ServerName
            // 
            this->textBox_ServerName->Location = System::Drawing::Point(122, 25);
            this->textBox_ServerName->MaxLength = 31;
            this->textBox_ServerName->Name = L"textBox_ServerName";
            this->textBox_ServerName->Size = System::Drawing::Size(163, 21);
            this->textBox_ServerName->TabIndex = 2;
            // 
            // checkBox_Settle
            // 
            this->checkBox_Settle->AutoSize = true;
            this->checkBox_Settle->Location = System::Drawing::Point(12, 62);
            this->checkBox_Settle->Name = L"checkBox_Settle";
            this->checkBox_Settle->Size = System::Drawing::Size(168, 16);
            this->checkBox_Settle->TabIndex = 3;
            this->checkBox_Settle->Text = L"是否同时作为结算地址使用";
            this->checkBox_Settle->UseVisualStyleBackColor = true;
            // 
            // button_OK
            // 
            this->button_OK->Location = System::Drawing::Point(51, 121);
            this->button_OK->Name = L"button_OK";
            this->button_OK->Size = System::Drawing::Size(75, 23);
            this->button_OK->TabIndex = 4;
            this->button_OK->Text = L"确定";
            this->button_OK->UseVisualStyleBackColor = true;
            this->button_OK->Click += gcnew System::EventHandler(this, &ServerGroupForm::button_OK_Click);
            // 
            // button_Cancle
            // 
            this->button_Cancle->Location = System::Drawing::Point(170, 121);
            this->button_Cancle->Name = L"button_Cancle";
            this->button_Cancle->Size = System::Drawing::Size(75, 23);
            this->button_Cancle->TabIndex = 5;
            this->button_Cancle->Text = L"取消";
            this->button_Cancle->UseVisualStyleBackColor = true;
            this->button_Cancle->Click += gcnew System::EventHandler(this, &ServerGroupForm::button_Cancle_Click);
            // 
            // ServerGroupForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(292, 163);
            this->Controls->Add(this->button_Cancle);
            this->Controls->Add(this->button_OK);
            this->Controls->Add(this->checkBox_Settle);
            this->Controls->Add(this->textBox_ServerName);
            this->Controls->Add(this->label1);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Name = L"ServerGroupForm";
            this->Text = L"服务器组";
            this->Load += gcnew System::EventHandler(this, &ServerGroupForm::ServerGroupForm_Load);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
    private: System::Void ServerGroupForm_Load(System::Object^  sender, System::EventArgs^  e) {
                 this->CenterToParent();

                 if ( *m_pType == Oper_Add )
                 {
                     this->Text = "新增服务器组名称";
                 }
                 else
                 {
                     this->Text = "修改服务器组名称";

                     textBox_ServerName->Text  = Tools::string2String(m_pCurServerGroup->szServerName);                 
                     checkBox_Settle->Checked  = (m_pCurServerGroup->nIsSettle)?true:false;
                 }
             }
private: System::Void button_OK_Click(System::Object^  sender, System::EventArgs^  e) {
             if(String::IsNullOrEmpty(textBox_ServerName->Text)) 
             {
                 MessageBox::Show("请输服务器组名称!");  
                 return;
             }

             ServerGroup oSG;
             memset(&oSG,0,sizeof(ServerGroup));

             strcpy(oSG.szServerName,Tools::String2string(textBox_ServerName->Text).c_str());
             oSG.nIsSettle = checkBox_Settle->Checked?1:0; 

             if(*m_pType==Oper_Add)
             {
                 if(!m_pDataInfo->IsValidOfServerName(m_nBrokerID,Tools::String2string(textBox_ServerName->Text).c_str()))    
                 {
                     MessageBox::Show("服务器组名称重名,请重新输入!");                 
                     return;
                 }

                 if(!m_pDataInfo->IsValidOfSettle(m_nBrokerID,checkBox_Settle->Checked))
                 {
                     MessageBox::Show("经纪公司只能选择一个服务器组作为结算地址，请重新设置!");
                     checkBox_Settle->Checked = false;
                     return;
                 }

                 memcpy(m_pCurServerGroup,&oSG,sizeof(ServerGroup));

                 this->DialogResult = ::DialogResult::OK;
                 this->Close();
             }

             if(*m_pType==Oper_Modify)
             {         
                 oSG.nBrokerID = m_pCurServerGroup->nBrokerID;
                 oSG.nServerID = m_pCurServerGroup->nServerID;

                 if(*m_pCurServerGroup == oSG)
                     this->DialogResult = ::DialogResult::Cancel;
                 else
                 {     
                     if( 0==strcmp(m_pCurServerGroup->szServerName,oSG.szServerName))
                     {
                         if(!m_pDataInfo->IsValidOfSettle(m_nBrokerID,checkBox_Settle->Checked))
                         {
                             MessageBox::Show("经纪公司只能选择一个服务器组作为结算地址，请重新设置!");
                             checkBox_Settle->Checked = false;
                             return;
                         }   
                     }else
                     {
                         if(!m_pDataInfo->IsValidOfServerName(m_nBrokerID,Tools::String2string(textBox_ServerName->Text).c_str()))
                         {                             
                             MessageBox::Show("服务器组名称重名,请重新输入!"); 
                             return;
                         }
                     }

                     this->DialogResult = ::DialogResult::OK;
                     memcpy(m_pCurServerGroup,&oSG,sizeof(ServerGroup));
                     this->Close();
                 }                 
             }
         }
private: System::Void button_Cancle_Click(System::Object^  sender, System::EventArgs^  e) {
             this->Close();
         }
};
}
