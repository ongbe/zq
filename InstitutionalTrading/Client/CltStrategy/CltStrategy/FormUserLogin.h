#pragma once


#include "DataManageEx.h"
#include "Tools.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace CltStrategy {

	/// <summary>
	/// Summary for FormUserLogin
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class FormUserLogin : public System::Windows::Forms::Form
	{
	public:
		FormUserLogin(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_pDataService = CDataManageEx::NewInstance();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormUserLogin()
		{
			m_pDataService->DestroyInstance();
			m_pDataService = NULL;
			if (components)
			{
				delete components;
			}
		}

	protected: CDataManageEx* m_pDataService;


	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  textBoxUserName;
	private: System::Windows::Forms::TextBox^  textBoxPasswd;

	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Button^  buttonOK;
	private: System::Windows::Forms::Button^  buttonClose;

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
			this->textBoxUserName = (gcnew System::Windows::Forms::TextBox());
			this->textBoxPasswd = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->buttonOK = (gcnew System::Windows::Forms::Button());
			this->buttonClose = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(55, 34);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(47, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"用户名:";
			// 
			// textBoxUserName
			// 
			this->textBoxUserName->Location = System::Drawing::Point(123, 31);
			this->textBoxUserName->Name = L"textBoxUserName";
			this->textBoxUserName->Size = System::Drawing::Size(100, 21);
			this->textBoxUserName->TabIndex = 1;
			// 
			// textBoxPasswd
			// 
			this->textBoxPasswd->Location = System::Drawing::Point(123, 58);
			this->textBoxPasswd->Name = L"textBoxPasswd";
			this->textBoxPasswd->PasswordChar = '*';
			this->textBoxPasswd->Size = System::Drawing::Size(100, 21);
			this->textBoxPasswd->TabIndex = 3;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(55, 61);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(47, 12);
			this->label2->TabIndex = 2;
			this->label2->Text = L"密　码:";
			// 
			// buttonOK
			// 
			this->buttonOK->Location = System::Drawing::Point(67, 95);
			this->buttonOK->Name = L"buttonOK";
			this->buttonOK->Size = System::Drawing::Size(75, 23);
			this->buttonOK->TabIndex = 4;
			this->buttonOK->Text = L"确定";
			this->buttonOK->UseVisualStyleBackColor = true;
			this->buttonOK->Click += gcnew System::EventHandler(this, &FormUserLogin::buttonOK_Click);
			// 
			// buttonClose
			// 
			this->buttonClose->Location = System::Drawing::Point(148, 95);
			this->buttonClose->Name = L"buttonClose";
			this->buttonClose->Size = System::Drawing::Size(75, 23);
			this->buttonClose->TabIndex = 5;
			this->buttonClose->Text = L"关闭";
			this->buttonClose->UseVisualStyleBackColor = true;
			this->buttonClose->Click += gcnew System::EventHandler(this, &FormUserLogin::buttonClose_Click);
			// 
			// FormUserLogin
			// 
			this->AcceptButton = this->buttonOK;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(267, 144);
			this->Controls->Add(this->buttonClose);
			this->Controls->Add(this->buttonOK);
			this->Controls->Add(this->textBoxPasswd);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->textBoxUserName);
			this->Controls->Add(this->label1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"FormUserLogin";
			this->Text = L"交易员登录";
			this->Load += gcnew System::EventHandler(this, &FormUserLogin::FormUserLogin_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &FormUserLogin::FormUserLogin_FormClosed);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	protected: virtual void WndProc( Message% m ) override{
				if(m.Msg == _GetEventMsgID()) 
				{
					switch(m.WParam.ToInt32())
					{
					case CBaseBusinessSpi::conTraderLogin:
						{
							String^ strMsg = "";
							if(m_pDataService->GetLoginTraderStatus()==CDataManageEx::conLogon) {
								strMsg = "登录成功";
							}
							else {
								strMsg = "登录失败\r\n";
								strMsg += gcnew String(m_pDataService->GetLoginRspMsg().c_str());
							}
							MessageBox::Show(strMsg);
							this->Close();
						}
						break;
					};
				}
				Form::WndProc ( m );
			}
	private: System::Void FormUserLogin_Load(System::Object^  sender, System::EventArgs^  e) {
				 m_pDataService->SubscribeEvent(CBaseBusinessSpi::conTraderLogin, 
					 (HWND)this->Handle.ToPointer());
			 }
	private: System::Void FormUserLogin_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
				 m_pDataService->UnsubscribeAllEvent((HWND)this->Handle.ToPointer());
			 }
	private: System::Void buttonOK_Click(System::Object^  sender, System::EventArgs^  e) {
				 m_pDataService->LogoutTrader();
				 m_pDataService->LoginTrader(Tools::String2string(textBoxUserName->Text), 
						Tools::String2string(textBoxPasswd->Text));
				 _SetCurrUserID(Tools::String2string(textBoxUserName->Text));
				 _SetPassword(Tools::String2string(textBoxPasswd->Text));
			 }
	private: System::Void buttonClose_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->Close();
			 }
};
}
