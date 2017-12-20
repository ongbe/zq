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
	/// Summary for ServerAddrForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class ServerAddrForm : public System::Windows::Forms::Form
	{
	public:
		ServerAddrForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
            m_pType = new OperType;
            m_pCurServerAddr = new ServerAddress;
            m_pCurServerAddrQuot = new ServerAddressQuot;
            m_pDataInfo = CDataInfo::NewInstance();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ServerAddrForm()
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

            if ( NULL != m_pCurServerAddr )
            {
                delete m_pCurServerAddr;
                m_pCurServerAddr = NULL;
            }

            if ( NULL != m_pCurServerAddrQuot )
            {
                delete m_pCurServerAddrQuot;
                m_pCurServerAddrQuot = NULL;
            }
		}

    private: CDataInfo* m_pDataInfo;
    private: OperType* m_pType;
    private: ServerAddress* m_pCurServerAddr; 
    private: IPControl::IPTextBox^  ipTextBox;

    private: ServerAddressQuot* m_pCurServerAddrQuot; 

    public:
        void SetOperType(OperType eType)
        {
            *m_pType = eType;
        }
        void SetCurServerAddress(const ServerAddress& oServerAddress)
        {
            *m_pCurServerAddr = oServerAddress;
        }

        ServerAddress* GetCurServerAddress()
        {
            return m_pCurServerAddr;
        }

        void SetCurServerAddressQuot(const ServerAddressQuot& oServerAddressQuot)
        {
            *m_pCurServerAddrQuot = oServerAddressQuot;
        }

        ServerAddressQuot* GetCurServerAddressQuot()
        {
            return m_pCurServerAddrQuot;
        }

    private: System::Windows::Forms::Button^  buttonOK;
    private: System::Windows::Forms::Button^  buttonCancel;

    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::Label^  label2;






    private: System::Windows::Forms::TextBox^  textBoxPort;



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
            this->buttonOK = (gcnew System::Windows::Forms::Button());
            this->buttonCancel = (gcnew System::Windows::Forms::Button());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->textBoxPort = (gcnew System::Windows::Forms::TextBox());
            this->ipTextBox = (gcnew IPControl::IPTextBox());
            this->SuspendLayout();
            // 
            // buttonOK
            // 
            this->buttonOK->Location = System::Drawing::Point(48, 140);
            this->buttonOK->Name = L"buttonOK";
            this->buttonOK->Size = System::Drawing::Size(75, 23);
            this->buttonOK->TabIndex = 0;
            this->buttonOK->Text = L"确定";
            this->buttonOK->UseVisualStyleBackColor = true;
            this->buttonOK->Click += gcnew System::EventHandler(this, &ServerAddrForm::buttonOK_Click);
            // 
            // buttonCancel
            // 
            this->buttonCancel->Location = System::Drawing::Point(179, 140);
            this->buttonCancel->Name = L"buttonCancel";
            this->buttonCancel->Size = System::Drawing::Size(75, 23);
            this->buttonCancel->TabIndex = 1;
            this->buttonCancel->Text = L"取消";
            this->buttonCancel->UseVisualStyleBackColor = true;
            this->buttonCancel->Click += gcnew System::EventHandler(this, &ServerAddrForm::buttonCancel_Click);
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(12, 37);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(77, 12);
            this->label1->TabIndex = 3;
            this->label1->Text = L"服务器地址：";
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Location = System::Drawing::Point(22, 82);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(53, 12);
            this->label2->TabIndex = 4;
            this->label2->Text = L"端口号：";
            // 
            // textBoxPort
            // 
            this->textBoxPort->Location = System::Drawing::Point(90, 79);
            this->textBoxPort->Name = L"textBoxPort";
            this->textBoxPort->Size = System::Drawing::Size(85, 21);
            this->textBoxPort->TabIndex = 11;
            this->textBoxPort->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &ServerAddrForm::textBoxPort_KeyPress);
            // 
            // ipTextBox
            // 
            this->ipTextBox->Location = System::Drawing::Point(90, 33);
            this->ipTextBox->Name = L"ipTextBox";
            this->ipTextBox->Size = System::Drawing::Size(128, 18);
            this->ipTextBox->TabIndex = 12;
            // 
            // ServerAddrForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(298, 181);
            this->Controls->Add(this->ipTextBox);
            this->Controls->Add(this->textBoxPort);
            this->Controls->Add(this->label2);
            this->Controls->Add(this->label1);
            this->Controls->Add(this->buttonCancel);
            this->Controls->Add(this->buttonOK);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Name = L"ServerAddrForm";
            this->Text = L"交易服务器地址";
            this->Load += gcnew System::EventHandler(this, &ServerAddrForm::ServerAddrForm_Load);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
private: System::Void ServerAddrForm_Load(System::Object^  sender, System::EventArgs^  e) {
             this->CenterToParent();

             if( *m_pType == Oper_Add_Trade )
                 this->Text = "新增交易服务器组地址";
             else if( *m_pType == Oper_Add_Quot )
                 this->Text = "新增行情服务器组地址";
             else if( *m_pType == Oper_Modify_Trade )
             {
                 this->Text = "修改交易服务器组地址";

                 String^ strIP = Tools::string2String(m_pCurServerAddr->szIP);

                 ipTextBox->Text = strIP;
                 textBoxPort->Text  = m_pCurServerAddr->nPort.ToString();
             }
             else if( *m_pType == Oper_Modify_Quot )
             {
                 this->Text = "修改行情服务器组地址";

                 String^ strIP = Tools::string2String(m_pCurServerAddrQuot->szIP);

                 ipTextBox->Text = strIP;
                 textBoxPort->Text  = m_pCurServerAddrQuot->nPort.ToString();
             }
         }
private: System::Void buttonOK_Click(System::Object^  sender, System::EventArgs^  e) {
             if(String::IsNullOrEmpty(ipTextBox->Text))             
                 MessageBox::Show("服务器组地址不能为空!");      
             else if(String::IsNullOrEmpty(textBoxPort->Text))             
                 MessageBox::Show("服务器组端口号不能为空!");   
             else
             {
                 this->DialogResult = ::DialogResult::OK;

                 String^ strIP = ipTextBox->Text;

                 if(*m_pType == Oper_Add_Quot || *m_pType == Oper_Modify_Quot)
                 {
                     strcpy(m_pCurServerAddrQuot->szIP,Tools::String2string(strIP).c_str());
                     m_pCurServerAddrQuot->nPort = ::Convert::ToInt32(textBoxPort->Text); 

                 }else
                 {
                     strcpy(m_pCurServerAddr->szIP,Tools::String2string(strIP).c_str());
                     m_pCurServerAddr->nPort = ::Convert::ToInt32(textBoxPort->Text); 
                 }

                 this->Close();
             }
         }
private: System::Void buttonCancel_Click(System::Object^  sender, System::EventArgs^  e) {
             this->Close();
         }
private: System::Void textBoxAddr_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
             if (e->KeyChar < '0' || e->KeyChar > '9')   
                 if (e->KeyChar != (char)8)    
                     e->Handled = true;
         }
private: System::Void textBoxPort_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
             if (e->KeyChar < '0' || e->KeyChar > '9')   
                 if (e->KeyChar != (char)8)    
                     e->Handled = true;
         }
};
}
