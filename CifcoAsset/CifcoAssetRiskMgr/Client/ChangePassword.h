#pragma once
//#include <windows.h>
#include "RiskManageCmd.h"
#include "RiskManageStruct.h"
#include "CommonPkg.h"
#include "TcpLayer.h"
#include "md5.h"
#include "CDataInfo.h"
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace Client {

	/// <summary>
	/// Summary for ChangePassword
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class ChangePassword : public System::Windows::Forms::Form
	{
	public:
		ChangePassword(void)
		{
			InitializeComponent();
			this->CenterToParent();
			//
			//TODO: Add the constructor code here
			//
			m_pdataInfo = CDataInfo::NewInstance();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ChangePassword()
		{
			CDataInfo::DestroyInstance();
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  PASSWORD_ORG;
	private: System::Windows::Forms::TextBox^  PASSWORD_NEW;
	private: System::Windows::Forms::TextBox^  PASSWORD_NEW2;
	protected: 



	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button2;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

	public: CDataInfo*  m_pdataInfo;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->PASSWORD_ORG = (gcnew System::Windows::Forms::TextBox());
			this->PASSWORD_NEW = (gcnew System::Windows::Forms::TextBox());
			this->PASSWORD_NEW2 = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(35, 42);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(53, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"原密码：";
			this->label1->Click += gcnew System::EventHandler(this, &ChangePassword::label1_Click);
			// 
			// PASSWORD_ORG
			// 
			this->PASSWORD_ORG->Location = System::Drawing::Point(95, 39);
			this->PASSWORD_ORG->Name = L"PASSWORD_ORG";
			this->PASSWORD_ORG->Size = System::Drawing::Size(220, 21);
			this->PASSWORD_ORG->TabIndex = 1;
			this->PASSWORD_ORG->UseSystemPasswordChar = true;
			// 
			// PASSWORD_NEW
			// 
			this->PASSWORD_NEW->Location = System::Drawing::Point(95, 91);
			this->PASSWORD_NEW->Name = L"PASSWORD_NEW";
			this->PASSWORD_NEW->Size = System::Drawing::Size(220, 21);
			this->PASSWORD_NEW->TabIndex = 1;
			this->PASSWORD_NEW->UseSystemPasswordChar = true;
			// 
			// PASSWORD_NEW2
			// 
			this->PASSWORD_NEW2->Location = System::Drawing::Point(95, 141);
			this->PASSWORD_NEW2->Name = L"PASSWORD_NEW2";
			this->PASSWORD_NEW2->Size = System::Drawing::Size(220, 21);
			this->PASSWORD_NEW2->TabIndex = 1;
			this->PASSWORD_NEW2->UseSystemPasswordChar = true;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(36, 91);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(53, 12);
			this->label2->TabIndex = 2;
			this->label2->Text = L"新密码：";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(12, 150);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(77, 12);
			this->label3->TabIndex = 3;
			this->label3->Text = L"确认新密码：";
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(132, 195);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 4;
			this->button1->Text = L"更改";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &ChangePassword::change_Click);
			// 
			// button2
			// 
			this->button2->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->button2->Location = System::Drawing::Point(240, 195);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 5;
			this->button2->Text = L"取消";
			this->button2->UseVisualStyleBackColor = true;
			// 
			// ChangePassword
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(334, 233);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->PASSWORD_NEW2);
			this->Controls->Add(this->PASSWORD_NEW);
			this->Controls->Add(this->PASSWORD_ORG);
			this->Controls->Add(this->label1);
			this->Name = L"ChangePassword";
			this->Text = L"更改密码";
			this->Load += gcnew System::EventHandler(this, &ChangePassword::changepassword_Load);
			this->ResumeLayout(false);
			this->PerformLayout();
			

		}
#pragma endregion
	private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
			 }
	private: System::Void change_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 String ^pwd1		= PASSWORD_ORG->Text;
				 String ^pwd2       = PASSWORD_NEW->Text;
				 String ^pwd3       = PASSWORD_NEW2->Text;
				if(pwd2 != pwd3)
				{			
					MessageBox::Show("两次新密码设置不同！", "提示",MessageBoxButtons::OK);
					return;
				}
				if(pwd2 == "" || pwd3 =="")
				{
					MessageBox::Show("新密码不能为空！", "提示",MessageBoxButtons::OK);
					return;
				}
				const char* pWND1 = (const char*)(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(pwd1)).ToPointer();
				const char* pWND2 = (const char*)(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(pwd2)).ToPointer();

				string strPwd;
				m_pdataInfo->GetPwd(strPwd);
				string str = pWND1;
				if(str != strPwd)
				{
					MessageBox::Show("原密码输入错误！", "提示",MessageBoxButtons::OK);
					return;
				}
				LoginRsp	loginRsp;
				m_pdataInfo->GetLoginRsp(loginRsp);

				ModifyPwd modifyPwd;
				modifyPwd.nRiskMgmtUserID = loginRsp.nAccountID;				
				string strPwd2 = pWND2;
				MD5 md5(strPwd2);
				strcpy(modifyPwd.szPassword, md5.toString().c_str());		
			

				CTcpLayer::SendData(Cmd_RM_ModifyPwd_Req, (void*)&modifyPwd,sizeof(modifyPwd), 0);
/*
				char buf[sizeof(Stru_UniPkgHead)+ sizeof(ModifyPwd) +4];
				Stru_UniPkgHead& PkgHead=*(Stru_UniPkgHead*)buf;
				PkgHead.Set(sizeof(ModifyPwd),Cmd_RM_ModifyPwd_Req,0,0);

				memcpy(buf+sizeof(Stru_UniPkgHead),(void*)&modifyPwd,sizeof(modifyPwd));
				*(unsigned int*)(buf+sizeof(PkgHead) + sizeof(modifyPwd))=TailID_UniPkg;

				CTcpLayer::Senddata(buf,sizeof(buf));*/
			 }
			 //重载WndProc，以处理WM_COMMAND消息
	protected:virtual void WndProc( Message% m ) override
			  {
				  if (  m.Msg == WM_COMMAND_Win && m.WParam.ToInt32()==WndCmd_YourPkgArrival )
				  {
					  char *p = (char*)m.LParam.ToInt32();
					  Stru_UniPkgHead head;					
					  memset(&head, 0, sizeof(head));
					  memcpy(&head, p, sizeof(head));

					  if ( head.cmdid == Cmd_RM_ModifyPwd_Rsp )
					  {
						  if ( head.userdata1 == OPERATOR_SUCCESS )
						  {	
								String ^pwd2       = PASSWORD_NEW->Text;
								const char* pWND2 = (const char*)(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(pwd2)).ToPointer();
							    m_pdataInfo->SetPwd(pWND2);								
								MessageBox::Show("密码更改成功！", "提示",MessageBoxButtons::OK);
								Close();
						  }
						  else
						  {
							  MessageBox::Show("密码更改失败！", "提示",MessageBoxButtons::OK);
						  }
					  }
				  }
				  Form::WndProc ( m );
			  }
	private: System::Void changepassword_Load(System::Object^  sender, System::EventArgs^  e)
			 {
				 //订阅事件
				 IntPtr hWnd=this->Handle;
				 CTcpLayer::SubscribePkg(Cmd_RM_ModifyPwd_Rsp, (int)(hWnd.ToInt32()));
			 }
};
}
