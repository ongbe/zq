#pragma once

#include "TcpLayer.h"
#include "CommonDef.h"
#include "CommonStruct.h"
#include "CommonPkg.h"
#include "CommonErrorCode.h"
#include "MD5.h"
#include "CDataInfo.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace CltPresentation {

	/// <summary>
	/// Summary for LoginForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>

	public ref class LoginForm : public System::Windows::Forms::Form
	{
	public:
		LoginForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			CDataInfo::NewInstance();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~LoginForm()
		{
			CDataInfo::DestroyInstance();
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	protected: 
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::TextBox^  textBox2;

	private: System::Windows::Forms::Button^  buttonLogin;
	private: System::Windows::Forms::Button^  buttonQuit;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::ProgressBar^  progLoginQuery;

	private: System::Windows::Forms::Label^  lblLoginProc;

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
			this->progLoginQuery = (gcnew System::Windows::Forms::ProgressBar());
			this->lblLoginProc = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(26, 22);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(41, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"账户号";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(26, 52);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(41, 12);
			this->label2->TabIndex = 1;
			this->label2->Text = L"密  码";
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(90, 19);
			this->textBox1->MaxLength = 20;
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(140, 21);
			this->textBox1->TabIndex = 2;
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(90, 49);
			this->textBox2->MaxLength = 20;
			this->textBox2->Name = L"textBox2";
			this->textBox2->PasswordChar = '*';
			this->textBox2->Size = System::Drawing::Size(140, 21);
			this->textBox2->TabIndex = 3;
			// 
			// buttonLogin
			// 
			this->buttonLogin->Location = System::Drawing::Point(67, 145);
			this->buttonLogin->Name = L"buttonLogin";
			this->buttonLogin->Size = System::Drawing::Size(75, 23);
			this->buttonLogin->TabIndex = 5;
			this->buttonLogin->Text = L"登录";
			this->buttonLogin->UseVisualStyleBackColor = true;
			this->buttonLogin->Click += gcnew System::EventHandler(this, &LoginForm::buttonLogin_Click);
			// 
			// buttonQuit
			// 
			this->buttonQuit->Location = System::Drawing::Point(155, 145);
			this->buttonQuit->Name = L"buttonQuit";
			this->buttonQuit->Size = System::Drawing::Size(75, 23);
			this->buttonQuit->TabIndex = 6;
			this->buttonQuit->Text = L"退出";
			this->buttonQuit->UseVisualStyleBackColor = true;
			this->buttonQuit->Click += gcnew System::EventHandler(this, &LoginForm::buttonQuit_Click);
			// 
			// timer1
			// 
			this->timer1->Interval = 60000;
			this->timer1->Tick += gcnew System::EventHandler(this, &LoginForm::timer1_Tick);
			// 
			// progLoginQuery
			// 
			this->progLoginQuery->Location = System::Drawing::Point(28, 123);
			this->progLoginQuery->Name = L"progLoginQuery";
			this->progLoginQuery->Size = System::Drawing::Size(202, 12);
			this->progLoginQuery->TabIndex = 7;
			// 
			// lblLoginProc
			// 
			this->lblLoginProc->AutoSize = true;
			this->lblLoginProc->Location = System::Drawing::Point(28, 107);
			this->lblLoginProc->Name = L"lblLoginProc";
			this->lblLoginProc->Size = System::Drawing::Size(53, 12);
			this->lblLoginProc->TabIndex = 8;
			this->lblLoginProc->Text = L"等待登录";
			// 
			// LoginForm
			// 
			this->AcceptButton = this->buttonLogin;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(256, 184);
			this->Controls->Add(this->lblLoginProc);
			this->Controls->Add(this->progLoginQuery);
			this->Controls->Add(this->buttonQuit);
			this->Controls->Add(this->buttonLogin);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"LoginForm";
			this->Text = L"登录";
			this->Load += gcnew System::EventHandler(this, &LoginForm::LoginForm_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &LoginForm::LoginForm_FormClosed);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void LoginForm_Load(System::Object^  sender, System::EventArgs^  e) {
				 int nWnd = this->Handle.ToInt32();
				 CTcpLayer::SetMainWndHandle(nWnd);
				 CTcpLayer::SubscribePkg(CMDID_Login_Rsp, nWnd);
				 CTcpLayer::SubscribePkg(Cmd_QueryOrg_Rsp, nWnd);
				 CTcpLayer::SubscribePkg(Cmd_QryUser_Rsp, nWnd);//用户
				 CTcpLayer::SubscribePkg(Cmd_QryUserOrgRelation_Rsp, nWnd);//用户与组织机构关系
				 CTcpLayer::SubscribePkg(Cmd_QueryFinicialProduct_Rsp, nWnd);//用户与组织机构关系
				 CTcpLayer::SubscribePkg(Cmd_QueryProductTraderRelation_Rsp, nWnd);//用户与组织机构关系
				 CTcpLayer::SubscribePkg(Cmd_RM_QryAvilabeInstrument_Rsp, nWnd);//用户与组织机构关系	
				 progLoginQuery->Minimum = 0;
				 progLoginQuery->Maximum = CDataInfo::conInitItemCount+1;
				 this->CenterToParent();
			 }
private: System::Void LoginForm_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
			 int nWnd = this->Handle.ToInt32();
			 CTcpLayer::UnsubscribeAllPkg(nWnd);	
		 }
private: System::Void buttonLogin_Click(System::Object^  sender, System::EventArgs^  e) {
			 String ^strUserName = textBox1->Text;
			 String ^strPWD      = textBox2->Text;

			 if(strUserName->IsNullOrEmpty(strUserName))
			 {
				 MessageBox::Show("用户名不能为空！", "提示",MessageBoxButtons::OK);
				 textBox1->Focus();
				 return;
			 }				
			 if(strPWD->IsNullOrEmpty(strPWD))
			 {					
				 MessageBox::Show("密码不能为空！", "提示",MessageBoxButtons::OK);
				 textBox2->Focus();
				 return;
			 }			 

			 const char* pUSER = (const char*)(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(strUserName)).ToPointer();
			 const char* pPWND = (const char*)(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(strPWD)).ToPointer();

			 sLoginReq sLogin;
			 strcpy(sLogin.msUserName, pUSER);
			 strcpy(sLogin.msPassWord, pPWND);	
			 MD5 md5(sLogin.msPassWord);
			 strcpy(sLogin.msPassWord, md5.toString().c_str());
			 sLogin.meUserType = USER_TYPE_VIEWER;

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
			//checkBox1->Enabled = bFlag;
			buttonLogin->Enabled = bFlag;
		 }

private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
			 timer1->Stop();
			 MessageBox::Show("登录超时！", "提示", MessageBoxButtons::OK);
			 EnableControls(true);
		 }

protected:
	virtual void WndProc( Message% m ) override{
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

		   switch(head.cmdid) {
			case CMDID_Login_Rsp:
				{
					EnableControls(true);
					timer1->Stop();
					if ( head.userdata1 == CF_ERROR_SUCCESS )
					{	
						sLoginRsp loginReq;
						memset(&loginReq, 0, sizeof(sLoginRsp));
						memcpy(&loginReq, p+sizeof(Stru_UniPkgHead), sizeof(sLoginRsp));
						CDataInfo::NewInstance()->SetLogonInfo(loginReq);

						//this->DialogResult = ::DialogResult::OK;
						//this->Close();
						progLoginQuery->Value = progLoginQuery->Value+1;
						lblLoginProc->Text = L"登录成功，查询组织机构...";

						CTcpLayer::SendData(Cmd_QueryOrg_Req, NULL,0, 0, 0, loginReq.mnUserID);//组织机构               
					}
					else
					{
						lblLoginProc->Text = L"登录失败！";
						const char* pError = p+sizeof(Stru_UniPkgHead);
						String^ strError = gcnew String(pError);
						MessageBox::Show(strError, "提示",MessageBoxButtons::OK);
					}
				}
				break;


			case Cmd_QueryOrg_Rsp:
				{
					if ( head.userdata1 == CF_ERROR_SUCCESS )
					{
						CDataInfo::NewInstance()->ClearOrganization();

						std::map<int, Organization> mapAssetMgmtOrg;
						int nCount = head.len / sizeof(Organization);
						for ( int i = 0; i < nCount; i++ )
						{
							Organization* pAssetMgmtOrg = (Organization*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(Organization));
							mapAssetMgmtOrg[pAssetMgmtOrg->nAssetMgmtOrgID] = *pAssetMgmtOrg;
						}

						CDataInfo::NewInstance()->SetOrganization(mapAssetMgmtOrg);

						progLoginQuery->Value = progLoginQuery->Value+1;
						lblLoginProc->Text = L"查询用户...";

						sLoginRsp loginRsp = {0};
						CDataInfo::NewInstance()->GetLogonInfo(loginRsp);
						CTcpLayer::SendData(Cmd_QryUser_Req, NULL,0, 0, 0, loginRsp.mnUserID);
					}
					else
					{
						lblLoginProc->Text = L"查询组织机构失败！";
					}

					break;
				}
			case Cmd_QryUser_Rsp:
				{                              
					if ( head.userdata1 == CF_ERROR_SUCCESS )
					{
						CDataInfo::NewInstance()->ClearUserInfo();

						std::map<int, UserInfo> mapUserInfo;
						int nCount = head.len / sizeof(UserInfo);
						for ( int i = 0; i < nCount; i++ )
						{
							UserInfo* pUserInfo = (UserInfo*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(UserInfo));
							mapUserInfo[pUserInfo->nUserID] = *pUserInfo;
						}

						CDataInfo::NewInstance()->SetUserInfo(mapUserInfo);

						progLoginQuery->Value = progLoginQuery->Value+1;
						lblLoginProc->Text = L"查询用户组织机构关系...";

						sLoginRsp loginRsp = {0};
						CDataInfo::NewInstance()->GetLogonInfo(loginRsp);
						CTcpLayer::SendData(Cmd_QryUserOrgRelation_Req, NULL,0, 0, 0, loginRsp.mnUserID);
					}
					else
					{
						lblLoginProc->Text = L"查询用户失败！";
					}
				}
				break;
			case Cmd_QryUserOrgRelation_Rsp:
				{
					if ( head.userdata1 == CF_ERROR_SUCCESS )
					{
						CDataInfo::NewInstance()->ClearUserAndOrgRelation();

						std::vector<UserAndOrgRelation> vecUserAndOrgRelation;
						int nCount = head.len / sizeof(UserAndOrgRelation);
						for ( int i = 0; i < nCount; i++ )
						{
							UserAndOrgRelation* pUserAndOrgRelation = (UserAndOrgRelation*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(UserAndOrgRelation));
							vecUserAndOrgRelation.push_back(*pUserAndOrgRelation);                             
						}

						CDataInfo::NewInstance()->SetUserAndOrgRelation(vecUserAndOrgRelation);	

						progLoginQuery->Value = progLoginQuery->Value+1;
						lblLoginProc->Text = L"查询理财产品...";

						sLoginRsp loginRsp = {0};
						CDataInfo::NewInstance()->GetLogonInfo(loginRsp);
						CTcpLayer::SendData(Cmd_QueryFinicialProduct_Req, NULL,0, 0, 0, loginRsp.mnUserID);

						
					}
					else
					{
						lblLoginProc->Text = L"查询用户组织机构关系失败！";
					}

				}
				break;
			case Cmd_QueryFinicialProduct_Rsp:
				{
					if ( head.userdata1 == CF_ERROR_SUCCESS )
					{
						CDataInfo::NewInstance()->ClearFinanceProduct();

						int nCount = head.len / sizeof(FinancialProduct);
						for ( int i = 0; i < nCount; i++ )
						{
							FinancialProduct* pFinancialProduct = (FinancialProduct*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(FinancialProduct));
							CDataInfo::NewInstance()->SetFinancialProduct(pFinancialProduct);									
						}

						progLoginQuery->Value = progLoginQuery->Value+1;
						lblLoginProc->Text = L"查询理财产品和用户的关系...";

						sLoginRsp loginRsp = {0};
						CDataInfo::NewInstance()->GetLogonInfo(loginRsp);
						CTcpLayer::SendData(Cmd_QueryProductTraderRelation_Req, NULL,0, 0, 0, loginRsp.mnUserID);

					}
					else
					{
						lblLoginProc->Text = L"查询理财产品失败！";
					}

					break;
				}
			case Cmd_QueryProductTraderRelation_Rsp:
				{
					if ( head.userdata1 == CF_ERROR_SUCCESS )
					{
						CDataInfo::NewInstance()->ClearProductTraderRelation();

						int nCount = head.len / sizeof(TraderProRelation);
						for ( int i = 0; i < nCount; i++ )
						{
							TraderProRelation* pTraderProRelation = (TraderProRelation*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(TraderProRelation));
							CDataInfo::NewInstance()->SetProductTraderRelation(pTraderProRelation);
						}					

						progLoginQuery->Value = progLoginQuery->Value+1;
						lblLoginProc->Text = L"查询合约和品种...";

						sLoginRsp loginRsp = {0};
						CDataInfo::NewInstance()->GetLogonInfo(loginRsp);
						CTcpLayer::SendData(Cmd_RM_QryAvilabeInstrument_Req, NULL,0, 0, 0, loginRsp.mnUserID);
						/*progLoginQuery->Value = progLoginQuery->Value+1;
						lblLoginProc->Text = L"查询完毕。";

						this->DialogResult = ::DialogResult::OK;
						this->Close();*/
					}
					else
					{
						lblLoginProc->Text = L"查询理财产品和用户关系失败失败！";
					}
					break;
				}
			case Cmd_RM_QryAvilabeInstrument_Rsp:
				{
					if ( head.userdata1 == CF_ERROR_SUCCESS )
					{				
						int nCount = head.len / sizeof(PlatformStru_InstrumentInfo);
						for ( int i = 0; i < nCount; i++ )
						{
							PlatformStru_InstrumentInfo* pInstrument = (PlatformStru_InstrumentInfo*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(PlatformStru_InstrumentInfo));
							CDataInfo::NewInstance()->SetInstrumentField(pInstrument);
						}					

						progLoginQuery->Value = progLoginQuery->Value+1;
						lblLoginProc->Text = L"查询完毕。";

						this->DialogResult = ::DialogResult::OK;
						this->Close();
					}
					else
					{
						lblLoginProc->Text = L"查询合约和品种失败！";
					}
					break;
				}
		   };
	   }
	   Form::WndProc ( m );
	}
};
}
