#pragma once

#include "TcpLayer.h"
#include "CommonDef.h"
#include "CommonStruct.h"
#include "CommonPkg.h"
#include "CommonErrorCode.h"
#include "MD5.h"
#include "RiskData.h"
#include "Tools.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace CltRisk {

	/// <summary>
	/// Summary for FormLogin
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class FormLogin : public System::Windows::Forms::Form
	{
	public:
		FormLogin(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
            m_strUserName = nullptr;
            m_strPWD = nullptr;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormLogin()
		{
			if (components)
			{
				delete components;
			}
		}

    private:
        String ^m_strUserName;
        String ^m_strPWD;

    private: System::Windows::Forms::Label^  label1;
    protected: 
    private: System::Windows::Forms::Label^  label2;
    private: System::Windows::Forms::TextBox^  textBox1;
    private: System::Windows::Forms::TextBox^  textBox2;
    private: System::Windows::Forms::Button^  buttonLogin;

    private: System::Windows::Forms::Button^  buttonQuit;
    private: System::Windows::Forms::Timer^  timer1;
    private: System::ComponentModel::IContainer^  components;


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            this->components = (gcnew System::ComponentModel::Container());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->textBox1 = (gcnew System::Windows::Forms::TextBox());
            this->textBox2 = (gcnew System::Windows::Forms::TextBox());
            this->buttonLogin = (gcnew System::Windows::Forms::Button());
            this->buttonQuit = (gcnew System::Windows::Forms::Button());
            this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
            this->SuspendLayout();
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(52, 42);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(53, 12);
            this->label1->TabIndex = 0;
            this->label1->Text = L"账户号：";
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Location = System::Drawing::Point(54, 84);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(41, 12);
            this->label2->TabIndex = 1;
            this->label2->Text = L"密码：";
            // 
            // textBox1
            // 
            this->textBox1->Location = System::Drawing::Point(128, 37);
            this->textBox1->MaxLength = 31;
            this->textBox1->Name = L"textBox1";
            this->textBox1->Size = System::Drawing::Size(126, 21);
            this->textBox1->TabIndex = 1;
            // 
            // textBox2
            // 
            this->textBox2->Location = System::Drawing::Point(128, 79);
            this->textBox2->MaxLength = 21;
            this->textBox2->Name = L"textBox2";
            this->textBox2->PasswordChar = '*';
            this->textBox2->Size = System::Drawing::Size(126, 21);
            this->textBox2->TabIndex = 2;
            // 
            // buttonLogin
            // 
            this->buttonLogin->Location = System::Drawing::Point(64, 148);
            this->buttonLogin->Name = L"buttonLogin";
            this->buttonLogin->Size = System::Drawing::Size(75, 23);
            this->buttonLogin->TabIndex = 3;
            this->buttonLogin->Text = L"登录";
            this->buttonLogin->UseVisualStyleBackColor = true;
            this->buttonLogin->Click += gcnew System::EventHandler(this, &FormLogin::buttonLogin_Click);
            // 
            // buttonQuit
            // 
            this->buttonQuit->Location = System::Drawing::Point(179, 148);
            this->buttonQuit->Name = L"buttonQuit";
            this->buttonQuit->Size = System::Drawing::Size(75, 23);
            this->buttonQuit->TabIndex = 4;
            this->buttonQuit->Text = L"退出";
            this->buttonQuit->UseVisualStyleBackColor = true;
            this->buttonQuit->Click += gcnew System::EventHandler(this, &FormLogin::buttonQuit_Click);
            // 
            // timer1
            // 
            this->timer1->Interval = 60000;
            // 
            // FormLogin
            // 
            this->AcceptButton = this->buttonLogin;
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(325, 197);
            this->Controls->Add(this->buttonQuit);
            this->Controls->Add(this->buttonLogin);
            this->Controls->Add(this->textBox2);
            this->Controls->Add(this->textBox1);
            this->Controls->Add(this->label2);
            this->Controls->Add(this->label1);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Name = L"FormLogin";
            this->Text = L"登录";
            this->Load += gcnew System::EventHandler(this, &FormLogin::LoginForm_Load);
            this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &FormLogin::LoginForm_FormClosed);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
private: System::Void LoginForm_Load(System::Object^  sender, System::EventArgs^  e) {
			 int nWnd = this->Handle.ToInt32();
             CTcpLayer::SetMainWndHandle(nWnd);
             CTcpLayer::SubscribePkg(CMDID_Login_Rsp, nWnd);

             //第一次起来时调用AppendStr时，窗口还没有创建成功，不会显示，所以提前调用创建窗口
             _GetPopMsg();             
			 this->CenterToParent();
		 }
 protected: virtual void WndProc( Message% m ) override{
                if (  m.Msg == WM_USER_Win && m.WParam.ToInt32()==WndUserCmd_DataArrival )
                {
                    CTcpLayer::ParseRecvPackageInWindowThread();
                }
                else if (  m.Msg == WM_USER_Win && m.WParam.ToInt32()==WndUserCmd_YourPkgArrival )
                {
                    char *p = (char*)m.LParam.ToInt32();
                    Stru_UniPkgHead head;					
                    memset(&head, 0, sizeof(head));
                    memcpy(&head, p, sizeof(head));

                    if ( head.cmdid == CMDID_Login_Rsp )
                    {
                        EnableControls(true);
                        timer1->Stop();
                        if ( head.userdata1 == CF_ERROR_SUCCESS )
                        {	
                            sLoginRsp loginReq;
                            memset(&loginReq, 0, sizeof(sLoginRsp));
                            memcpy(&loginReq, p+sizeof(Stru_UniPkgHead), sizeof(sLoginRsp));
                            CRiskData::GetInstance()->SetLogonInfo(loginReq);
                            CRiskData::GetInstance()->SetRiskerAccount(Tools::String2string(m_strUserName));

                            this->DialogResult = ::DialogResult::OK;
                            this->Close();
                        }
                        else
                        {
                            const char* pError = p+sizeof(Stru_UniPkgHead);   
                            _GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
                                CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
                                pError);
                        }
                    } 
                }
                Form::WndProc ( m );
            }
private: System::Void LoginForm_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
			 int nWnd = this->Handle.ToInt32();
             CTcpLayer::UnsubscribePkg(CMDID_Login_Rsp, nWnd);
		 }
private: System::Void buttonLogin_Click(System::Object^  sender, System::EventArgs^  e) {
			 m_strUserName = textBox1->Text;
			 m_strPWD      = textBox2->Text;

			 if(m_strUserName->IsNullOrEmpty(m_strUserName))
			 {
				 MessageBox::Show("用户名不能为空！", "提示",MessageBoxButtons::OK);
				 textBox1->Focus();
				 return;
			 }				
			 if(m_strPWD->IsNullOrEmpty(m_strPWD))
			 {					
				 MessageBox::Show("密码不能为空！", "提示",MessageBoxButtons::OK);
				 textBox2->Focus();
				 return;
			 }			 

			 const char* pUSER = (const char*)(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(m_strUserName)).ToPointer();
			 const char* pPWND = (const char*)(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(m_strPWD)).ToPointer();

			 sLoginReq sLogin;
			 strcpy(sLogin.msUserName, pUSER);
			 strcpy(sLogin.msPassWord, pPWND);	
			 MD5 md5(sLogin.msPassWord);
			 strcpy(sLogin.msPassWord, md5.toString().c_str());
			 sLogin.meUserType = USER_TYPE_RISK_CONTROL;

    
             if(!CTcpLayer::SendData(CMDID_Login_Req, (void*)&sLogin,sizeof(sLogin), 0))
             {
                 MessageBox::Show("网络连接异常，登录失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
             }
             else
             {
                 EnableControls(false);
                 timer1->Start();
             }
		 }
private: System::Void buttonQuit_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->Close();
		 }

private: System::Void EnableControls(bool bFlag){
			textBox1->Enabled = bFlag;
			textBox2->Enabled = bFlag;
			buttonLogin->Enabled = bFlag;
		 }

private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
			 timer1->Stop();
			 MessageBox::Show("登录超时！", "提示", MessageBoxButtons::OK);
			 EnableControls(true);
		 }
};
}
