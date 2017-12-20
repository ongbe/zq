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
	/// Summary for QuotServerTimeResetForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class QuotServerTimeResetForm : public System::Windows::Forms::Form
	{
	public:
		QuotServerTimeResetForm(void)
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
		~QuotServerTimeResetForm()
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
    private: System::Windows::Forms::DateTimePicker^  dateTimePicker_DayTime;

    private: System::Windows::Forms::Label^  label2;
    private: System::Windows::Forms::DateTimePicker^  dateTimePicker_NightTime;

    private: System::Windows::Forms::Label^  label1;

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
            this->dateTimePicker_DayTime = (gcnew System::Windows::Forms::DateTimePicker());
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->dateTimePicker_NightTime = (gcnew System::Windows::Forms::DateTimePicker());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->SuspendLayout();
            // 
            // label3
            // 
            this->label3->AutoSize = true;
            this->label3->Location = System::Drawing::Point(73, 30);
            this->label3->Name = L"label3";
            this->label3->Size = System::Drawing::Size(143, 12);
            this->label3->TabIndex = 17;
            this->label3->Text = L"正在查询自动重启时间...";
            // 
            // button_cancel
            // 
            this->button_cancel->Location = System::Drawing::Point(238, 140);
            this->button_cancel->Name = L"button_cancel";
            this->button_cancel->Size = System::Drawing::Size(75, 23);
            this->button_cancel->TabIndex = 16;
            this->button_cancel->Text = L"取消";
            this->button_cancel->UseVisualStyleBackColor = true;
            this->button_cancel->Click += gcnew System::EventHandler(this, &QuotServerTimeResetForm::button_cancel_Click);
            // 
            // confirm
            // 
            this->confirm->Location = System::Drawing::Point(110, 140);
            this->confirm->Name = L"confirm";
            this->confirm->Size = System::Drawing::Size(75, 23);
            this->confirm->TabIndex = 15;
            this->confirm->Text = L"设置";
            this->confirm->UseVisualStyleBackColor = true;
            this->confirm->Click += gcnew System::EventHandler(this, &QuotServerTimeResetForm::confirm_Click);
            // 
            // dateTimePicker_DayTime
            // 
            this->dateTimePicker_DayTime->CustomFormat = L"HH:mm:ss";
            this->dateTimePicker_DayTime->Enabled = false;
            this->dateTimePicker_DayTime->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
            this->dateTimePicker_DayTime->Location = System::Drawing::Point(204, 65);
            this->dateTimePicker_DayTime->Name = L"dateTimePicker_DayTime";
            this->dateTimePicker_DayTime->ShowUpDown = true;
            this->dateTimePicker_DayTime->Size = System::Drawing::Size(118, 21);
            this->dateTimePicker_DayTime->TabIndex = 14;
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Location = System::Drawing::Point(71, 71);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(125, 12);
            this->label2->TabIndex = 11;
            this->label2->Text = L"每日日盘自动重启时间";
            // 
            // dateTimePicker_NightTime
            // 
            this->dateTimePicker_NightTime->CustomFormat = L"HH:mm:ss";
            this->dateTimePicker_NightTime->Enabled = false;
            this->dateTimePicker_NightTime->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
            this->dateTimePicker_NightTime->Location = System::Drawing::Point(204, 94);
            this->dateTimePicker_NightTime->Name = L"dateTimePicker_NightTime";
            this->dateTimePicker_NightTime->ShowUpDown = true;
            this->dateTimePicker_NightTime->Size = System::Drawing::Size(118, 21);
            this->dateTimePicker_NightTime->TabIndex = 13;
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(71, 98);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(125, 12);
            this->label1->TabIndex = 12;
            this->label1->Text = L"每日夜盘自动重启时间";
            // 
            // QuotServerTimeResetForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(425, 205);
            this->Controls->Add(this->label3);
            this->Controls->Add(this->button_cancel);
            this->Controls->Add(this->confirm);
            this->Controls->Add(this->dateTimePicker_DayTime);
            this->Controls->Add(this->label2);
            this->Controls->Add(this->dateTimePicker_NightTime);
            this->Controls->Add(this->label1);
            this->Name = L"QuotServerTimeResetForm";
            this->Text = L"行情服务器定时重启时间设置";
            this->Load += gcnew System::EventHandler(this, &QuotServerTimeResetForm::QuotServerTimeResetForm_Load);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
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

                  case Cmd_QryQuotServerTimeEvent_Rsp:
                      {
                          if ( head.userdata3 == CF_ERROR_SUCCESS && head.len == sizeof(sReinitTime) )
                          {
                              this->confirm->Enabled = true;
                              this->dateTimePicker_DayTime->Enabled = true;
                              this->dateTimePicker_NightTime->Enabled = true;
                              label3->Text = "获取启动时间成功";
                              sReinitTime * lsReinitTime = (sReinitTime*)(p+sizeof(Stru_UniPkgHead));
                              dateTimePicker_DayTime->Text=Tools::string2String(lsReinitTime->mDayTime);
                              dateTimePicker_NightTime->Text=Tools::string2String(lsReinitTime->mNightTime);

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
                  case Cmd_QuotServerTimeResetEvent_Rsp:
                      {
                          this->confirm->Enabled = true;
                          if ( head.userdata3 == CF_ERROR_SUCCESS )
                          {
                              label3->Text = "设置成功";
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
private: System::Void QuotServerTimeResetForm_Load(System::Object^  sender, System::EventArgs^  e) {
             this->confirm->Enabled = false;
             this->dateTimePicker_DayTime->Enabled = false;
             this->dateTimePicker_NightTime->Enabled = false;

             int nHwnd = this->Handle.ToInt32();
             CTcpLayer::SubscribePkg(Cmd_QryQuotServerTimeEvent_Rsp, nHwnd);
             CTcpLayer::SubscribePkg(Cmd_QuotServerTimeResetEvent_Rsp, nHwnd);
             CTcpLayer::SendData(Cmd_QryQuotServerTimeEvent_Req,NULL,0,0);
         }
private: System::Void confirm_Click(System::Object^  sender, System::EventArgs^  e) {
             this->confirm->Enabled = false;

             sReinitTime ls;
             memset(&ls,0,sizeof(ls));
             string  lTime1 =Tools::String2string(dateTimePicker_DayTime->Text);				
             strncpy(ls.mDayTime,lTime1.c_str(),sizeof(ls.mDayTime)-1);
             string  lTime2 =Tools::String2string(dateTimePicker_NightTime->Text);				
             strncpy(ls.mNightTime,lTime2.c_str(),sizeof(ls.mNightTime)-1);

             sLoginRsp lLogin;
             CDataInfo::NewInstance()->GetLogonInfo(lLogin);

             if ( !CTcpLayer::SendData(Cmd_QuotServerTimeResetEvent_Req, &ls, sizeof(ls), 0,lLogin.mnUserID))					 
             {
                 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
             }

         }
private: System::Void button_cancel_Click(System::Object^  sender, System::EventArgs^  e) {
             this->Close();
         }
};
}
