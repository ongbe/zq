#pragma once
#include "TcpDataConnect.h"
#include "CommonDef.h"
#include "CommonStruct.h"
#include "CommonPkg.h"
#include "CommonErrorCode.h"
#include "MD5.h"
#include "Tools.h"
#include "RiskData.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace CltRisk {

	/// <summary>
	/// Summary for FormLockWindow
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>

	public ref class FormLockWindow : public System::Windows::Forms::Form
	{
	public:
        FormLockWindow(Form^ parent)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
            m_bOkClicked = false;
            m_pUserPsw = new UserPassword;
            memset(m_pUserPsw,0,sizeof(UserPassword));
            m_MainWnd = parent;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormLockWindow()
		{
			if (components)
			{
				delete components;
			}
            if(m_pUserPsw)
            {
                delete m_pUserPsw;
                m_pUserPsw = NULL;
            }
		}
    private: bool m_bOkClicked;
    private: System::Windows::Forms::Button^  button_UnLock;
    protected: 

    protected: 
    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::TextBox^  textBox_UnLockPsw;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

        UserPassword* m_pUserPsw;
        Form^ m_MainWnd;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(FormLockWindow::typeid));
            this->button_UnLock = (gcnew System::Windows::Forms::Button());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->textBox_UnLockPsw = (gcnew System::Windows::Forms::TextBox());
            this->SuspendLayout();
            // 
            // button_UnLock
            // 
            resources->ApplyResources(this->button_UnLock, L"button_UnLock");
            this->button_UnLock->Name = L"button_UnLock";
            this->button_UnLock->UseVisualStyleBackColor = true;
            this->button_UnLock->Click += gcnew System::EventHandler(this, &FormLockWindow::button_UnLock_Click);
            // 
            // label1
            // 
            resources->ApplyResources(this->label1, L"label1");
            this->label1->Name = L"label1";
            // 
            // textBox_UnLockPsw
            // 
            resources->ApplyResources(this->textBox_UnLockPsw, L"textBox_UnLockPsw");
            this->textBox_UnLockPsw->Name = L"textBox_UnLockPsw";
            // 
            // FormLockWindow
            // 
            this->AcceptButton = this->button_UnLock;
            resources->ApplyResources(this, L"$this");
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ControlBox = false;
            this->Controls->Add(this->textBox_UnLockPsw);
            this->Controls->Add(this->label1);
            this->Controls->Add(this->button_UnLock);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Name = L"FormLockWindow";
            this->Load += gcnew System::EventHandler(this, &FormLockWindow::FormLockWindow_Load);
            this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &FormLockWindow::FormLockWindow_FormClosing);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
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


                       if ( head.cmdid == Cmd_RM_UnLockWindow_Rsp )
                       {                          
                           if ( head.userdata1 == CF_ERROR_SUCCESS )
                           {
                                this->textBox_UnLockPsw->Clear();
                                this->Hide();
                                m_MainWnd->Show();
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

    public:  void SetMainWnd(/*FormMain* nWnd*/)
             {
                 //m_MainWnd = nWnd;
             }
    public:  void GetUnLockPsw(UserPassword& sPsw)
             {
                 memcpy(&sPsw,m_pUserPsw,sizeof(UserPassword));
             }
    private: System::Void FormLockWindow_Load(System::Object^  sender, System::EventArgs^  e) {
                 int nWnd = this->Handle.ToInt32();
                 CTcpLayer::SetMainWndHandle(nWnd);
                 CTcpLayer::SubscribePkg(Cmd_RM_UnLockWindow_Rsp, nWnd);

                 this->CenterToParent();
             }
    private: System::Void button_UnLock_Click(System::Object^  sender, System::EventArgs^  e) {

                 sLoginRsp Login;
                 CRiskData::GetInstance()->GetLogonInfo(Login);

                 m_pUserPsw->nUserID = Login.mnUserID;
                 string szPassword = Tools::String2string(textBox_UnLockPsw->Text).c_str();

                 MD5 md5(szPassword);
                 strcpy(m_pUserPsw->szNewPassword, md5.toString().c_str());

                 m_bOkClicked = true;
                 //this->DialogResult = ::DialogResult::OK;  


                 CTcpLayer::SendData(Cmd_RM_UnLockWindow_Req, (void*)m_pUserPsw,sizeof(UserPassword), 0, Login.mnUserID);

             }
    private: System::Void FormLockWindow_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
//                  if(!m_bOkClicked) 
//                      e->Cancel = true;
                 int nWnd = this->Handle.ToInt32();
                 CTcpLayer::UnsubscribePkg(Cmd_RM_UnLockWindow_Rsp, nWnd);

             }
};
}
