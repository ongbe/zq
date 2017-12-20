#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
#include "TcpLayer.h"
#include "CommonDef.h"
#include "CommonPkg.h"
#include "CommonErrorCode.h"
#include "CDataInfo.h"
#include "Tools.h"
#include "CommonStruct.h"

namespace CltServerMgr {

	/// <summary>
	/// Summary for ReinitTimeOption
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class ReinitTimeOption : public System::Windows::Forms::Form
	{
	public:
		ReinitTimeOption(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ReinitTimeOption()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  button_cancel;
	protected: 
	private: System::Windows::Forms::Button^  confirm;
	private: System::Windows::Forms::DateTimePicker^  dateTimePicker1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::DateTimePicker^  dateTimePicker2;
	private: System::Windows::Forms::Label^  label3;

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
			this->button_cancel = (gcnew System::Windows::Forms::Button());
			this->confirm = (gcnew System::Windows::Forms::Button());
			this->dateTimePicker1 = (gcnew System::Windows::Forms::DateTimePicker());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->dateTimePicker2 = (gcnew System::Windows::Forms::DateTimePicker());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// button_cancel
			// 
			this->button_cancel->Location = System::Drawing::Point(239, 129);
			this->button_cancel->Name = L"button_cancel";
			this->button_cancel->Size = System::Drawing::Size(75, 23);
			this->button_cancel->TabIndex = 9;
			this->button_cancel->Text = L"取消";
			this->button_cancel->UseVisualStyleBackColor = true;
			this->button_cancel->Click += gcnew System::EventHandler(this, &ReinitTimeOption::button_cancel_Click);
			// 
			// confirm
			// 
			this->confirm->Location = System::Drawing::Point(111, 129);
			this->confirm->Name = L"confirm";
			this->confirm->Size = System::Drawing::Size(75, 23);
			this->confirm->TabIndex = 8;
			this->confirm->Text = L"设置";
			this->confirm->UseVisualStyleBackColor = true;
			this->confirm->Click += gcnew System::EventHandler(this, &ReinitTimeOption::confirm_Click);
			// 
			// dateTimePicker1
			// 
			this->dateTimePicker1->CustomFormat = L"HH:mm:ss";
			this->dateTimePicker1->Enabled = false;
			this->dateTimePicker1->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
			this->dateTimePicker1->Location = System::Drawing::Point(205, 83);
			this->dateTimePicker1->Name = L"dateTimePicker1";
			this->dateTimePicker1->ShowUpDown = true;
			this->dateTimePicker1->Size = System::Drawing::Size(118, 21);
			this->dateTimePicker1->TabIndex = 7;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(34, 87);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(161, 12);
			this->label1->TabIndex = 6;
			this->label1->Text = L"每日夜盘自动重新初始化时间";
			this->label1->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			this->label1->Click += gcnew System::EventHandler(this, &ReinitTimeOption::label1_Click);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(34, 60);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(161, 12);
			this->label2->TabIndex = 6;
			this->label2->Text = L"每日日盘自动重新初始化时间";
			// 
			// dateTimePicker2
			// 
			this->dateTimePicker2->CustomFormat = L"HH:mm:ss";
			this->dateTimePicker2->Enabled = false;
			this->dateTimePicker2->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
			this->dateTimePicker2->Location = System::Drawing::Point(205, 54);
			this->dateTimePicker2->Name = L"dateTimePicker2";
			this->dateTimePicker2->ShowUpDown = true;
			this->dateTimePicker2->Size = System::Drawing::Size(118, 21);
			this->dateTimePicker2->TabIndex = 7;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(34, 19);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(179, 12);
			this->label3->TabIndex = 10;
			this->label3->Text = L"正在查询自动重新初始化时间...";
			// 
			// ReinitTimeOption
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(420, 197);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->button_cancel);
			this->Controls->Add(this->confirm);
			this->Controls->Add(this->dateTimePicker2);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->dateTimePicker1);
			this->Controls->Add(this->label1);
			this->Name = L"ReinitTimeOption";
			this->Text = L"设置服务器定时重新初始化时间";
			this->Load += gcnew System::EventHandler(this, &ReinitTimeOption::FormLoad);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void confirm_Click(System::Object^  sender, System::EventArgs^  e) {

				 this->confirm->Enabled = false;

				 sReinitTime ls;
				 memset(&ls,0,sizeof(ls));
				 string  lTime1 =Tools::String2string(dateTimePicker1->Text);				
				 strncpy(ls.mDayTime,lTime1.c_str(),sizeof(ls.mDayTime)-1);
				 string  lTime2 =Tools::String2string(dateTimePicker2->Text);				
				 strncpy(ls.mNightTime,lTime2.c_str(),sizeof(ls.mNightTime)-1);
                 
				 sLoginRsp lLogin;
				 CDataInfo::NewInstance()->GetLogonInfo(lLogin);

				 if ( !CTcpLayer::SendData(Cmd_TradeDataManager_SetReinitTime_Req, &ls, sizeof(ls), 0,lLogin.mnUserID))					 
				 {
					 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
				 }



			 }
	private: System::Void button_cancel_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->Close();
			 }
	private: System::Void FormLoad(System::Object^  sender, System::EventArgs^  e) {
				 this->confirm->Enabled = false;
				 this->dateTimePicker1->Enabled = false;
				 this->dateTimePicker2->Enabled = false;

				 int nHwnd = this->Handle.ToInt32();
				 CTcpLayer::SubscribePkg(Cmd_TradeDataManager_QryReinitTime_Rsp, nHwnd);
				 CTcpLayer::SubscribePkg(Cmd_TradeDataManager_SetReinitTime_Rsp, nHwnd);
				 CTcpLayer::SendData(Cmd_TradeDataManager_QryReinitTime_Req,NULL,0,0);


			 }
	protected:virtual void WndProc( Message% m ) override
			  {
				  if (  m.Msg == WM_USER_Win && m.WParam.ToInt32()==WndUserCmd_YourPkgArrival )
				  {
					  char *p = (char*)m.LParam.ToInt32();
					  Stru_UniPkgHead head;					
					  memset(&head, 0, sizeof(head));
					  memcpy(&head, p, sizeof(head));

					  switch ( head.cmdid )
					  {

					  case Cmd_TradeDataManager_QryReinitTime_Rsp:
						  {
							  if ( head.userdata3 == CF_ERROR_SUCCESS && head.len == sizeof(sReinitTime) )
							  {
								  this->confirm->Enabled = true;
								  this->dateTimePicker1->Enabled = true;
								  this->dateTimePicker2->Enabled = true;
								  label3->Text = "获取服务器重新初始化时间成功";
								  sReinitTime * lsReinitTime = (sReinitTime*)(p+sizeof(Stru_UniPkgHead));
								  dateTimePicker1->Text=Tools::string2String(lsReinitTime->mDayTime);
								  dateTimePicker2->Text=Tools::string2String(lsReinitTime->mNightTime);

							  }
							  else
							  {
								  string str = (char*)(p+sizeof(Stru_UniPkgHead));
								  String^ strtemp = Tools::string2String(str.c_str());
								 // MessageBox::Show(strtemp, "提示",MessageBoxButtons::OK);
								  label3->Text = strtemp;
							  }

							  break;
						  }
					  case Cmd_TradeDataManager_SetReinitTime_Rsp:
						  {
							   this->confirm->Enabled = true;
							  if ( head.userdata3 == CF_ERROR_SUCCESS )
							  {
									label3->Text = "设置服务器重新初始化时间成功";
							  }
							  else
							  {
								  string str = (char*)(p+sizeof(Stru_UniPkgHead));
								  String^ strtemp = Tools::string2String(str.c_str());								 
								  label3->Text = strtemp;
							  }

							  break;
						  }

					  default:
						  break;
					  }

				  }

				  Form::WndProc ( m );
			  }
	private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e) {
			 }
};
}
