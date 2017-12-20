#pragma once
#include "TcpLayer.h"
#include "CommonDef.h"
#include "CommonPkg.h"
#include "CommonErrorCode.h"
#include "CDataInfo.h"
#include "Tools.h"
#include "CommonStruct.h"
#include "ConfigMgr.h"
#include "CommonUserOpr.h"
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace CltServerMgr {

	/// <summary>
	/// Summary for SetAutoSettleTime
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class SetAutoSettleTime : public System::Windows::Forms::Form
	{
	public:
		SetAutoSettleTime(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_pLogin = new sLoginRsp();
			m_pDataInfo = CDataInfo::NewInstance();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~SetAutoSettleTime()
		{
			if (components)
			{
				delete components;
			}
			CDataInfo::DestroyInstance();
			if(m_pLogin)
			{
				delete m_pLogin;
				m_pLogin = nullptr;
			}
		}

	private: sLoginRsp*     m_pLogin;
	private: CDataInfo*     m_pDataInfo;
	private: System::Windows::Forms::Label^  label1;
	protected: 
	private: System::Windows::Forms::DateTimePicker^  dateTimePicker1;
	private: System::Windows::Forms::Button^  confirm;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::ComboBox^  comboBox1;
	private: System::Windows::Forms::Button^  button_cancel;



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
			this->dateTimePicker1 = (gcnew System::Windows::Forms::DateTimePicker());
			this->confirm = (gcnew System::Windows::Forms::Button());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->button_cancel = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(81, 108);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(101, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"每日自动结算时间";
			// 
			// dateTimePicker1
			// 
			this->dateTimePicker1->CustomFormat = L"HH:mm:ss";
			this->dateTimePicker1->Enabled = false;
			this->dateTimePicker1->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
			this->dateTimePicker1->Location = System::Drawing::Point(214, 99);
			this->dateTimePicker1->Name = L"dateTimePicker1";
			this->dateTimePicker1->ShowUpDown = true;
			this->dateTimePicker1->Size = System::Drawing::Size(118, 21);
			this->dateTimePicker1->TabIndex = 1;
			// 
			// confirm
			// 
			this->confirm->Location = System::Drawing::Point(85, 162);
			this->confirm->Name = L"confirm";
			this->confirm->Size = System::Drawing::Size(75, 23);
			this->confirm->TabIndex = 2;
			this->confirm->Text = L"设置";
			this->confirm->UseVisualStyleBackColor = true;
			this->confirm->Click += gcnew System::EventHandler(this, &SetAutoSettleTime::confirm_Click);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(83, 45);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(77, 12);
			this->label2->TabIndex = 3;
			this->label2->Text = L"自动结算功能";
			// 
			// comboBox1
			// 
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"关闭", L"开启"});
			this->comboBox1->Location = System::Drawing::Point(214, 42);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(121, 20);
			this->comboBox1->TabIndex = 4;
			this->comboBox1->Text = L"关闭";
			this->comboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &SetAutoSettleTime::comboBox1_SelectedIndexChanged);
			// 
			// button_cancel
			// 
			this->button_cancel->Location = System::Drawing::Point(267, 162);
			this->button_cancel->Name = L"button_cancel";
			this->button_cancel->Size = System::Drawing::Size(75, 23);
			this->button_cancel->TabIndex = 5;
			this->button_cancel->Text = L"取消";
			this->button_cancel->UseVisualStyleBackColor = true;
			this->button_cancel->Click += gcnew System::EventHandler(this, &SetAutoSettleTime::button_cancel_Click);
			// 
			// SetAutoSettleTime
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(469, 255);
			this->Controls->Add(this->button_cancel);
			this->Controls->Add(this->comboBox1);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->confirm);
			this->Controls->Add(this->dateTimePicker1);
			this->Controls->Add(this->label1);
			this->Name = L"SetAutoSettleTime";
			this->Text = L"设置自动结算时间";
			this->Load += gcnew System::EventHandler(this, &SetAutoSettleTime::SetAutoSettleTime_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &SetAutoSettleTime::SetAutoSettleTime_FormClosed);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void SetAutoSettleTime_Load(System::Object^  sender, System::EventArgs^  e) 
			 {
				 this->CenterToParent(); 
				 int nHwnd = this->Handle.ToInt32();
				 CTcpLayer::SubscribePkg(Cmd_Settlement_SetSettltmentTime_Rsp, nHwnd);
				 CTcpLayer::SubscribePkg(Cmd_Settlement_QuerySettltmentTime_Rsp, nHwnd);
				 m_pDataInfo->GetLogonInfo(*m_pLogin);
				 if ( !CTcpLayer::SendData(Cmd_Settlement_QuerySettltmentTime_Req, NULL, 0, 0, m_pLogin->mnUserID))
				 {
					 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
				 }
			 }
	private: System::Void confirm_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 int Flag=comboBox1->SelectedIndex;

				 string  dp =Tools::String2string(dateTimePicker1->Text);
				 _SettlementDate  settleTime;
				 strncpy(settleTime,dp.c_str(),sizeof(_SettlementDate)-1);
				 int nHwnd = this->Handle.ToInt32();

				 if ( !CTcpLayer::SendData(Cmd_Settlement_SetSettltmentTime_Req, settleTime, sizeof(_SettlementDate), 0, m_pLogin->mnUserID,Flag))
				 {
					 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
				 }
				  LOG_INFO("结算-设置自动结算时间  用户 %d,服务器IP %s,设置 按钮点下，设置时间 %s",m_pLogin->mnUserID,CConfigManager::GetSvrAddr().c_str(),settleTime);
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
							   case Cmd_Settlement_SetSettltmentTime_Rsp:
								   {
									   if ( head.userdata1 == CF_ERROR_SUCCESS )
									   {
										   MessageBox::Show("设置成功", "提示",MessageBoxButtons::OK);
										   
									   }
									   else
									   {
										   string str = (char*)(p+sizeof(Stru_UniPkgHead));
										   String^ strtemp = Tools::string2String(str.c_str());
											_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
													CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
													str.c_str());
									   }

									   break;
								   }
							   case Cmd_Settlement_QuerySettltmentTime_Rsp:
								   {
									   if ( head.userdata1 == CF_ERROR_SUCCESS )
									   {
										   if(head.userdata2 == 1)
										   {
											   this->comboBox1->Text = L"开启";
											   this->dateTimePicker1->Enabled = true;
											   string str = (char*)(p+sizeof(Stru_UniPkgHead));
											   dateTimePicker1->Text=Tools::string2String(str.c_str());
										   }
										   if(head.userdata2 == 0)
										   {
											   this->comboBox1->Text = L"关闭";
											   this->dateTimePicker1->Enabled = false;
											   
										   }


									   }
									   else
									   {
										   string str = (char*)(p+sizeof(Stru_UniPkgHead));
										   String^ strtemp = Tools::string2String(str.c_str());
											_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
													CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
													str.c_str());
									   }

									   break;
								   }

							   default:
								   break;
							   }

						   }

						   Form::WndProc ( m );
					   }
private: System::Void comboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 switch (comboBox1->SelectedIndex)
			 {
			 case 0 :
				 this->dateTimePicker1->Enabled = false;
				 break;
			 case 1:
				 this->dateTimePicker1->Enabled = true;
				 break;
			 default:
				 break;
			 }
		 }
private: System::Void button_cancel_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 this->Close();
		 }
private: System::Void SetAutoSettleTime_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) 
		 {
			 IntPtr hWnd=this->Handle;
			 CTcpLayer::UnsubscribeAllPkg((int)hWnd);
			
		 }
};
}
