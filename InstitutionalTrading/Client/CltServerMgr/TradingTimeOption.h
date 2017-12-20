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
	/// Summary for TradingTimeOption
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class TradingTimeOption : public System::Windows::Forms::Form
	{
	public:
		TradingTimeOption(void)
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
		~TradingTimeOption()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label3;
	protected: 
	private: System::Windows::Forms::Button^  button_cancel;
	private: System::Windows::Forms::Button^  confirm;
	private: System::Windows::Forms::DateTimePicker^  dateTimePicker2;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::DateTimePicker^  dateTimePicker1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::DateTimePicker^  dateTimePicker3;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::DateTimePicker^  dateTimePicker4;


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
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->button_cancel = (gcnew System::Windows::Forms::Button());
			this->confirm = (gcnew System::Windows::Forms::Button());
			this->dateTimePicker2 = (gcnew System::Windows::Forms::DateTimePicker());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->dateTimePicker1 = (gcnew System::Windows::Forms::DateTimePicker());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->dateTimePicker3 = (gcnew System::Windows::Forms::DateTimePicker());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->dateTimePicker4 = (gcnew System::Windows::Forms::DateTimePicker());
			this->SuspendLayout();
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(24, 27);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(131, 12);
			this->label3->TabIndex = 17;
			this->label3->Text = L"正在查询交易时间段...";
			// 
			// button_cancel
			// 
			this->button_cancel->Location = System::Drawing::Point(195, 207);
			this->button_cancel->Name = L"button_cancel";
			this->button_cancel->Size = System::Drawing::Size(75, 23);
			this->button_cancel->TabIndex = 16;
			this->button_cancel->Text = L"取消";
			this->button_cancel->UseVisualStyleBackColor = true;
			this->button_cancel->Click += gcnew System::EventHandler(this, &TradingTimeOption::button_cancel_Click);
			// 
			// confirm
			// 
			this->confirm->Location = System::Drawing::Point(67, 207);
			this->confirm->Name = L"confirm";
			this->confirm->Size = System::Drawing::Size(75, 23);
			this->confirm->TabIndex = 15;
			this->confirm->Text = L"设置";
			this->confirm->UseVisualStyleBackColor = true;
			this->confirm->Click += gcnew System::EventHandler(this, &TradingTimeOption::confirm_Click);
			// 
			// dateTimePicker2
			// 
			this->dateTimePicker2->CustomFormat = L"HH:mm:ss";
			this->dateTimePicker2->Enabled = false;
			this->dateTimePicker2->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
			this->dateTimePicker2->Location = System::Drawing::Point(159, 91);
			this->dateTimePicker2->Name = L"dateTimePicker2";
			this->dateTimePicker2->ShowUpDown = true;
			this->dateTimePicker2->Size = System::Drawing::Size(118, 21);
			this->dateTimePicker2->TabIndex = 14;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(24, 69);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(125, 12);
			this->label2->TabIndex = 11;
			this->label2->Text = L"每日日盘开始交易时间";
			// 
			// dateTimePicker1
			// 
			this->dateTimePicker1->CustomFormat = L"HH:mm:ss";
			this->dateTimePicker1->Enabled = false;
			this->dateTimePicker1->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
			this->dateTimePicker1->Location = System::Drawing::Point(159, 63);
			this->dateTimePicker1->Name = L"dateTimePicker1";
			this->dateTimePicker1->ShowUpDown = true;
			this->dateTimePicker1->Size = System::Drawing::Size(118, 21);
			this->dateTimePicker1->TabIndex = 13;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(24, 98);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(125, 12);
			this->label1->TabIndex = 12;
			this->label1->Text = L"每日日盘结束交易时间";
			this->label1->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(26, 152);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(125, 12);
			this->label4->TabIndex = 12;
			this->label4->Text = L"每日夜盘结束交易时间";
			this->label4->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			// 
			// dateTimePicker3
			// 
			this->dateTimePicker3->CustomFormat = L"HH:mm:ss";
			this->dateTimePicker3->Enabled = false;
			this->dateTimePicker3->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
			this->dateTimePicker3->Location = System::Drawing::Point(159, 119);
			this->dateTimePicker3->Name = L"dateTimePicker3";
			this->dateTimePicker3->ShowUpDown = true;
			this->dateTimePicker3->Size = System::Drawing::Size(118, 21);
			this->dateTimePicker3->TabIndex = 13;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(24, 125);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(125, 12);
			this->label5->TabIndex = 11;
			this->label5->Text = L"每日夜盘开始交易时间";
			// 
			// dateTimePicker4
			// 
			this->dateTimePicker4->CustomFormat = L"HH:mm:ss";
			this->dateTimePicker4->Enabled = false;
			this->dateTimePicker4->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
			this->dateTimePicker4->Location = System::Drawing::Point(159, 146);
			this->dateTimePicker4->Name = L"dateTimePicker4";
			this->dateTimePicker4->ShowUpDown = true;
			this->dateTimePicker4->Size = System::Drawing::Size(118, 21);
			this->dateTimePicker4->TabIndex = 14;
			// 
			// TradingTimeOption
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(334, 266);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->button_cancel);
			this->Controls->Add(this->confirm);
			this->Controls->Add(this->dateTimePicker4);
			this->Controls->Add(this->dateTimePicker2);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->dateTimePicker3);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->dateTimePicker1);
			this->Controls->Add(this->label1);
			this->Name = L"TradingTimeOption";
			this->Text = L"设置服务器交易时间段";
			this->Load += gcnew System::EventHandler(this, &TradingTimeOption::FormLoad);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void button_cancel_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->Close();
			 }
	private: System::Void confirm_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->confirm->Enabled = false;

				 sTradingTime ls;
				 memset(&ls,0,sizeof(ls));
				 string  lTime1 =Tools::String2string(dateTimePicker1->Text);				
				 strncpy(ls.mTradingDayStart,lTime1.c_str(),sizeof(ls.mTradingDayStart)-1);
				 string  lTime2 =Tools::String2string(dateTimePicker2->Text);				
				 strncpy(ls.mTradingDayEnd,lTime2.c_str(),sizeof(ls.mTradingDayEnd)-1);
				 string  lTime3 =Tools::String2string(dateTimePicker3->Text);				
				 strncpy(ls.mTradingNightStart,lTime3.c_str(),sizeof(ls.mTradingNightStart)-1);
				 string  lTime4 =Tools::String2string(dateTimePicker4->Text);				
				 strncpy(ls.mTradingNightEnd,lTime4.c_str(),sizeof(ls.mTradingNightEnd)-1);


				 sLoginRsp lLogin;
				 CDataInfo::NewInstance()->GetLogonInfo(lLogin);

				 if ( !CTcpLayer::SendData(Cmd_TradeDataManager_SetTradingTime_Req, &ls, sizeof(ls), 0,lLogin.mnUserID))					 
				 {
					 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
				 }
			 }
	private: System::Void FormLoad(System::Object^  sender, System::EventArgs^  e) {
				 this->confirm->Enabled = false;
				 this->dateTimePicker1->Enabled = false;
				 this->dateTimePicker2->Enabled = false;

				 int nHwnd = this->Handle.ToInt32();
				 CTcpLayer::SubscribePkg(Cmd_TradeDataManager_QryTradingTime_Rsp, nHwnd);
				 CTcpLayer::SubscribePkg(Cmd_TradeDataManager_SetTradingTime_Rsp, nHwnd);
				 CTcpLayer::SendData(Cmd_TradeDataManager_QryTradingTime_Req,NULL,0,0);


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

					  case Cmd_TradeDataManager_QryTradingTime_Rsp:
						  {
							  if ( head.userdata3 == CF_ERROR_SUCCESS && head.len == sizeof(sTradingTime) )
							  {
								  this->confirm->Enabled = true;
								  this->dateTimePicker1->Enabled = true;
								  this->dateTimePicker2->Enabled = true;
								  this->dateTimePicker3->Enabled = true;
								  this->dateTimePicker4->Enabled = true;
								  label3->Text = "获取交易时间段成功";
								  sTradingTime * lsTradingTime = (sTradingTime*)(p+sizeof(Stru_UniPkgHead));
								  dateTimePicker1->Text=Tools::string2String(lsTradingTime->mTradingDayStart);
								  dateTimePicker2->Text=Tools::string2String(lsTradingTime->mTradingDayEnd);
								  dateTimePicker3->Text=Tools::string2String(lsTradingTime->mTradingNightStart);
								  dateTimePicker4->Text=Tools::string2String(lsTradingTime->mTradingNightEnd);

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
					  case Cmd_TradeDataManager_SetTradingTime_Rsp:
						  {
							  this->confirm->Enabled = true;
							  if ( head.userdata3 == CF_ERROR_SUCCESS )
							  {
								  label3->Text = "设置服务器交易时间段成功";
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


	};
}
