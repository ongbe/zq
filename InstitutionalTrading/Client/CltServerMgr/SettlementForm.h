#pragma once

#include "SettlementPriceConfirmForm.h"
#include "CommissionMarginConfirmForm.h"
#include "TradeConfirmForm.h"
#include "PositionConfirmForm.h"
#include "FundConfirmForm.h"
#include "Tools.h"
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
	/// Summary for SettlementForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class SettlementForm : public System::Windows::Forms::Form
	{
	public:
		SettlementForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			m_pLogin = new sLoginRsp();
			m_pDataInfo = CDataInfo::NewInstance();
			//
		}
		SettlementForm(String^ caption)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			this->Text = caption;
			m_pLogin = new sLoginRsp();
			m_pDataInfo = CDataInfo::NewInstance();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~SettlementForm()
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
		//private: String^  m_caption;

	private: SettlementPriceConfirmForm^ m_SettlementPriceConfirmForm;
	private: CommissionMarginConfirmForm^ m_CommissionMarginConfirmForm;
	private: TradeConfirmForm^ m_TradeConfirmForm;
	private: PositionConfirmForm^ m_PositionConfirmForm;
	private: FundConfirmForm^ m_FundConfirmForm;
	private: System::Windows::Forms::Button^  button_settlePrice;
	private: sLoginRsp*     m_pLogin;
	private: CDataInfo*     m_pDataInfo;
	protected: 

	private: System::Windows::Forms::Button^  button_rate;
	private: System::Windows::Forms::Button^  button_trade;
	private: System::Windows::Forms::Button^  button_position;
	private: System::Windows::Forms::Button^  button_fund;
	private: System::Windows::Forms::Button^  button1;

	private: System::Windows::Forms::CheckBox^  checkBox1;
	private: System::Windows::Forms::CheckBox^  checkBox2;
	private: System::Windows::Forms::CheckBox^  checkBox3;
	private: System::Windows::Forms::CheckBox^  checkBox4;
	private: System::Windows::Forms::CheckBox^  checkBox5;

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
			this->button_settlePrice = (gcnew System::Windows::Forms::Button());
			this->button_rate = (gcnew System::Windows::Forms::Button());
			this->button_trade = (gcnew System::Windows::Forms::Button());
			this->button_position = (gcnew System::Windows::Forms::Button());
			this->button_fund = (gcnew System::Windows::Forms::Button());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox2 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox3 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox4 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox5 = (gcnew System::Windows::Forms::CheckBox());
			this->SuspendLayout();
			// 
			// button_settlePrice
			// 
			this->button_settlePrice->Location = System::Drawing::Point(82, 49);
			this->button_settlePrice->Name = L"button_settlePrice";
			this->button_settlePrice->Size = System::Drawing::Size(75, 23);
			this->button_settlePrice->TabIndex = 0;
			this->button_settlePrice->Text = L"结算价确认";
			this->button_settlePrice->UseVisualStyleBackColor = true;
			this->button_settlePrice->Click += gcnew System::EventHandler(this, &SettlementForm::button_settlePrice_Click);
			// 
			// button_rate
			// 
			this->button_rate->Enabled = false;
			this->button_rate->Location = System::Drawing::Point(82, 102);
			this->button_rate->Name = L"button_rate";
			this->button_rate->Size = System::Drawing::Size(75, 23);
			this->button_rate->TabIndex = 1;
			this->button_rate->Text = L"费率确认";
			this->button_rate->UseVisualStyleBackColor = true;
			this->button_rate->Click += gcnew System::EventHandler(this, &SettlementForm::button_rate_Click);
			// 
			// button_trade
			// 
			this->button_trade->Enabled = false;
			this->button_trade->Location = System::Drawing::Point(82, 154);
			this->button_trade->Name = L"button_trade";
			this->button_trade->Size = System::Drawing::Size(75, 23);
			this->button_trade->TabIndex = 2;
			this->button_trade->Text = L"成交单确认";
			this->button_trade->UseVisualStyleBackColor = true;
			this->button_trade->Click += gcnew System::EventHandler(this, &SettlementForm::button_trade_Click);
			// 
			// button_position
			// 
			this->button_position->Enabled = false;
			this->button_position->Location = System::Drawing::Point(82, 202);
			this->button_position->Name = L"button_position";
			this->button_position->Size = System::Drawing::Size(75, 23);
			this->button_position->TabIndex = 3;
			this->button_position->Text = L"持仓\\明细确认";
			this->button_position->UseVisualStyleBackColor = true;
			this->button_position->Click += gcnew System::EventHandler(this, &SettlementForm::button_position_Click);
			// 
			// button_fund
			// 
			this->button_fund->Enabled = false;
			this->button_fund->Location = System::Drawing::Point(82, 253);
			this->button_fund->Name = L"button_fund";
			this->button_fund->Size = System::Drawing::Size(75, 23);
			this->button_fund->TabIndex = 4;
			this->button_fund->Text = L"资金确认";
			this->button_fund->UseVisualStyleBackColor = true;
			this->button_fund->Click += gcnew System::EventHandler(this, &SettlementForm::button_fund_Click);
			// 
			// button1
			// 
			this->button1->Enabled = false;
			this->button1->Location = System::Drawing::Point(106, 328);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(90, 28);
			this->button1->TabIndex = 5;
			this->button1->Text = L"结算完成";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &SettlementForm::button1_Click);
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->Enabled = false;
			this->checkBox1->Location = System::Drawing::Point(199, 107);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(15, 14);
			this->checkBox1->TabIndex = 7;
			this->checkBox1->UseVisualStyleBackColor = true;
			// 
			// checkBox2
			// 
			this->checkBox2->AutoSize = true;
			this->checkBox2->Enabled = false;
			this->checkBox2->Location = System::Drawing::Point(199, 53);
			this->checkBox2->Name = L"checkBox2";
			this->checkBox2->Size = System::Drawing::Size(15, 14);
			this->checkBox2->TabIndex = 8;
			this->checkBox2->UseVisualStyleBackColor = true;
			// 
			// checkBox3
			// 
			this->checkBox3->AutoSize = true;
			this->checkBox3->Enabled = false;
			this->checkBox3->Location = System::Drawing::Point(199, 154);
			this->checkBox3->Name = L"checkBox3";
			this->checkBox3->Size = System::Drawing::Size(15, 14);
			this->checkBox3->TabIndex = 9;
			this->checkBox3->UseVisualStyleBackColor = true;
			// 
			// checkBox4
			// 
			this->checkBox4->AutoSize = true;
			this->checkBox4->Enabled = false;
			this->checkBox4->Location = System::Drawing::Point(199, 206);
			this->checkBox4->Name = L"checkBox4";
			this->checkBox4->Size = System::Drawing::Size(15, 14);
			this->checkBox4->TabIndex = 10;
			this->checkBox4->UseVisualStyleBackColor = true;
			// 
			// checkBox5
			// 
			this->checkBox5->AutoSize = true;
			this->checkBox5->Enabled = false;
			this->checkBox5->Location = System::Drawing::Point(199, 257);
			this->checkBox5->Name = L"checkBox5";
			this->checkBox5->Size = System::Drawing::Size(15, 14);
			this->checkBox5->TabIndex = 11;
			this->checkBox5->UseVisualStyleBackColor = true;
			// 
			// SettlementForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(341, 463);
			this->Controls->Add(this->checkBox5);
			this->Controls->Add(this->checkBox4);
			this->Controls->Add(this->checkBox3);
			this->Controls->Add(this->checkBox2);
			this->Controls->Add(this->checkBox1);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->button_fund);
			this->Controls->Add(this->button_position);
			this->Controls->Add(this->button_trade);
			this->Controls->Add(this->button_rate);
			this->Controls->Add(this->button_settlePrice);
			this->Name = L"SettlementForm";
			this->Text = L"当日结算";
			this->Load += gcnew System::EventHandler(this, &SettlementForm::SettlementForm_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &SettlementForm::SettlementForm_FormClosed);
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &SettlementForm::SettlementForm_FormClosing);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void SettlementForm_Load(System::Object^  sender, System::EventArgs^  e) 
			 {
				 this->CenterToParent(); 
				 int nHwnd = this->Handle.ToInt32();
				  m_pDataInfo->GetLogonInfo(*m_pLogin);
				 CTcpLayer::SubscribePkg(Cmd_Settlement_SettlementFinish_Confirm, nHwnd);
			 }
	private: System::Void button_settlePrice_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 LOG_INFO("结算-%s 用户 %d,服务器IP %s,结算价确认 按钮点下",Tools::String2string(this->Text).c_str(),m_pLogin->mnUserID,CConfigManager::GetSvrAddr().c_str());
				 this->button_rate->Enabled =false;
				 this->checkBox1->Checked = false;
				 this->button_trade->Enabled =false;
				 this->checkBox3->Checked = false;
				 this->button_position->Enabled =false;
				 this->checkBox4->Checked = false;
				 this->button_fund->Enabled =false;
				 this->checkBox5->Checked = false;
				 this->button1->Enabled = false;

				 m_SettlementPriceConfirmForm = gcnew SettlementPriceConfirmForm();
				 if (m_SettlementPriceConfirmForm->ShowDialog()  == System::Windows::Forms::DialogResult::Yes )
				 {
					 this->button_settlePrice->Enabled =true;
					 this->checkBox2->Checked = true;
					 this->button_rate->Enabled = true;
				 }

			 }
	private: System::Void button_rate_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 LOG_INFO("结算-%s 用户 %d,服务器IP %s,费率确认 按钮点下",Tools::String2string(this->Text).c_str(),m_pLogin->mnUserID,CConfigManager::GetSvrAddr().c_str());
				 this->button_trade->Enabled =false;
				 this->checkBox3->Checked = false;
				 this->button_position->Enabled =false;
				 this->checkBox4->Checked = false;
				 this->button_fund->Enabled =false;
				 this->checkBox5->Checked = false;
				 this->button1->Enabled = false;

				 m_CommissionMarginConfirmForm = gcnew CommissionMarginConfirmForm();
				 if (m_CommissionMarginConfirmForm->ShowDialog()  == System::Windows::Forms::DialogResult::Yes )
				 {
					 this->button_rate->Enabled =true;
					 this->checkBox1->Checked = true;
					 this->button_trade->Enabled = true;
				 }
			 }
	private: System::Void button_trade_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 LOG_INFO("结算-%s 用户 %d,服务器IP %s,成交确认 按钮点下",Tools::String2string(this->Text).c_str(),m_pLogin->mnUserID,CConfigManager::GetSvrAddr().c_str());
				 this->button_position->Enabled =false;
				 this->checkBox4->Checked = false;
				 this->button_fund->Enabled =false;
				 this->checkBox5->Checked = false;
				 this->button1->Enabled = false;

				 std::string strTradeDate= Tools::String2string(this->Text).substr(0,10);
				 strTradeDate.erase (strTradeDate.begin()+4);
				 strTradeDate.erase (strTradeDate.begin()+6);
				 m_TradeConfirmForm = gcnew TradeConfirmForm(Tools::string2String(strTradeDate.c_str()));
				 if (m_TradeConfirmForm->ShowDialog()  == System::Windows::Forms::DialogResult::Yes )
				 {
					 this->button_trade->Enabled =true;
					 this->checkBox3->Checked = true;
					 this->button_position->Enabled = true;
				 }
			 }
	private: System::Void button_position_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 LOG_INFO("结算-%s 用户 %d,服务器IP %s,持仓确认 按钮点下",Tools::String2string(this->Text).c_str(),m_pLogin->mnUserID,CConfigManager::GetSvrAddr().c_str());
				 this->button_fund->Enabled =false;
				 this->checkBox5->Checked = false;
				 this->button1->Enabled = false;

				 m_PositionConfirmForm = gcnew PositionConfirmForm();
				 if (m_PositionConfirmForm->ShowDialog()  == System::Windows::Forms::DialogResult::Yes )
				 {
					 this->button_position->Enabled =true;
					 this->checkBox4->Checked = true;
					 this->button_fund->Enabled = true;
				 }
			 }
	private: System::Void button_fund_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 this->button1->Enabled = false;
				 m_FundConfirmForm = gcnew FundConfirmForm();
				 if (m_FundConfirmForm->ShowDialog()  == System::Windows::Forms::DialogResult::Yes )
				 {
					 this->button_fund->Enabled =true;
					 this->checkBox5->Checked = true;
					 this->button1->Enabled = true;
				 }
				 LOG_INFO("结算-%s 用户 %d,服务器IP %s,资金确认 按钮点下",Tools::String2string(this->Text).c_str(),m_pLogin->mnUserID,CConfigManager::GetSvrAddr().c_str());
			 }
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 int nHwnd = this->Handle.ToInt32();
				
				 if ( !CTcpLayer::SendData(Cmd_Settlement_SettlementFinish_Notify, NULL, 0, 0, m_pLogin->mnUserID))
				 {
					 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
				 }
				 LOG_INFO("结算-%s 用户 %d,服务器IP %s,结算完成 按钮点下",Tools::String2string(this->Text).c_str(),m_pLogin->mnUserID,CConfigManager::GetSvrAddr().c_str());
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
					  case Cmd_Settlement_SettlementFinish_Confirm:
						  {
							  if ( head.userdata1 == CF_ERROR_SUCCESS )
							  {
								  this->Close();
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

	private: System::Void SettlementForm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) 
			 {

				 /*int nHwnd = this->Handle.ToInt32();
				 m_pDataInfo->GetLogonInfo(*m_pLogin);

				 if ( !CTcpLayer::SendData(Cmd_Settlement_SettlementWinColse_Notify, NULL, 0, 0, m_pLogin->mnUserID))
				 {
					 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
				 }*/
				 
				
			 }
private: System::Void SettlementForm_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) 
		 {
			 IntPtr hWnd=this->Handle;
		     CTcpLayer::UnsubscribeAllPkg((int)hWnd);
		 }
};
}
