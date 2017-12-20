#pragma once
#include "stdafx.h"
#include "Tools.h"
#include "SettlementForm.h"
#include "TcpLayer.h"
#include "CommonPkg.h"
#include "CommonDef.h"
#include "CommonErrorCode.h"
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
	/// Summary for HistorySettlementForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class HistorySettlementForm : public System::Windows::Forms::Form
	{
	public:
		HistorySettlementForm(void)
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
		~HistorySettlementForm()
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
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::ProgressBar^  progressBar1;
	private: System::Windows::Forms::Button^  button2;

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
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->progressBar1 = (gcnew System::Windows::Forms::ProgressBar());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(46, 38);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(53, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"结算日期";
			// 
			// dateTimePicker1
			// 
			this->dateTimePicker1->CustomFormat = L"yyyy-MM-dd";
			this->dateTimePicker1->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
			this->dateTimePicker1->Location = System::Drawing::Point(156, 34);
			this->dateTimePicker1->Name = L"dateTimePicker1";
			this->dateTimePicker1->Size = System::Drawing::Size(120, 21);
			this->dateTimePicker1->TabIndex = 1;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(48, 105);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(230, 23);
			this->button1->TabIndex = 2;
			this->button1->Text = L"导入历史交易数据";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &HistorySettlementForm::button1_Click);
			// 
			// progressBar1
			// 
			this->progressBar1->Location = System::Drawing::Point(48, 190);
			this->progressBar1->Name = L"progressBar1";
			this->progressBar1->Size = System::Drawing::Size(228, 23);
			this->progressBar1->TabIndex = 3;
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(100, 281);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(105, 23);
			this->button2->TabIndex = 4;
			this->button2->Text = L"开始结算";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &HistorySettlementForm::button2_Click);
			// 
			// HistorySettlementForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(340, 367);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->progressBar1);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->dateTimePicker1);
			this->Controls->Add(this->label1);
			this->Name = L"HistorySettlementForm";
			this->Text = L"历史结算";
			this->Load += gcnew System::EventHandler(this, &HistorySettlementForm::HistorySettlementForm_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &HistorySettlementForm::HistorySettlementForm_FormClosed);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void HistorySettlementForm_Load(System::Object^  sender, System::EventArgs^  e) 
			 {
				 this->button2->Enabled=false;
				 this->dateTimePicker1->MaxDate = System::DateTime::Now.AddDays(-1);
				 this->dateTimePicker1->Format = DateTimePickerFormat::Custom;
				 this->dateTimePicker1->CustomFormat = "yyyy-MM-dd";
				 this->CenterToParent(); 
				 int nHwnd = this->Handle.ToInt32();
				  m_pDataInfo->GetLogonInfo(*m_pLogin);
				 CTcpLayer::SubscribePkg(Cmd_Settlement_LoadData_Rsp, nHwnd);
				 CTcpLayer::SubscribePkg(Cmd_Settlement_QuerySettlementPrice_Rsp, nHwnd);				 
			 }
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 string  caption = Tools::String2string(dateTimePicker1->Text);
			 sprintf((char*)caption.c_str(),"%s%s",caption.c_str(),"结算");
			 SettlementForm^ dlg = gcnew SettlementForm(Tools::string2String(caption.c_str()));
			 dlg->ShowDialog();
			 this->Close();
             LOG_INFO("结算-历史结算 用户 %d,服务器IP %s,开始结算 按钮点下",m_pLogin->mnUserID,CConfigManager::GetSvrAddr().c_str());

		 }
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) 
		 {

			 string date=Tools::String2string(dateTimePicker1->Text);
			 _SettlementDate lcBuf = {0};
			 strcpy(lcBuf,date.c_str());
			 int  nbHistorySettlement = 1;

			 if ( !CTcpLayer::SendData(Cmd_Settlement_LoadData_Req, lcBuf, sizeof(_SettlementDate), 0, m_pLogin->mnUserID,SettlementLoad_TradingDataWithHistory,nbHistorySettlement))
			 {
				 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
			 }
			 this->progressBar1->Style = System::Windows::Forms::ProgressBarStyle::Marquee;
			 this->button1->Enabled = false;
			 this->button2->Enabled = false;

			 LOG_INFO("结算-历史结算  用户 %d,服务器IP %s,导入历史交易数据 按钮点下，加载日期 %s",m_pLogin->mnUserID,CConfigManager::GetSvrAddr().c_str(),date.c_str());
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
						   case Cmd_Settlement_LoadData_Rsp:
							   {
								   if ( head.userdata1 == CF_ERROR_SUCCESS )
								   {
									   
									    int  nbHistorySettlement = 1;
									   if ( !CTcpLayer::SendData(Cmd_Settlement_QuerySettlementPrice_Req, NULL, 0, 0, m_pLogin->mnUserID,nbHistorySettlement))
									   {
										   MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
									   }									   
								   }
								   else
								   {
									   this->progressBar1->Style = System::Windows::Forms::ProgressBarStyle::Blocks;
									   progressBar1->Value = progressBar1->Maximum;
									   this->button1->Enabled = true;
									   string str = (char*)(p+sizeof(Stru_UniPkgHead));
									   String^ strtemp = Tools::string2String(str.c_str());
										_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
												CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
												str.c_str());
								   }

								   break;
							   }
						   case Cmd_Settlement_QuerySettlementPrice_Rsp:
							   {
								   if ( head.userdata1 == CF_ERROR_SUCCESS )
								   {
									   std::map<std::string,SettlementPriceField>  lMapFiled;
									   int nCount = head.len / sizeof(SettlementPriceField);
									   if(nCount == 0)
										   break;
									   for ( int i = 0; i < nCount; i++ )
									   {
										   SettlementPriceField* pField = (SettlementPriceField*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(SettlementPriceField));
										   lMapFiled[pField->InstrumentID] = *pField;
									   }

									   m_pDataInfo->SetOriginalSettlementPrice(lMapFiled);
									   this->progressBar1->Style = System::Windows::Forms::ProgressBarStyle::Blocks;
									   progressBar1->Value = progressBar1->Maximum;
									   this->button1->Enabled = true;
									   this->button2->Enabled = true;
									  
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
private: System::Void HistorySettlementForm_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) 
		 {
			 int nHwnd = this->Handle.ToInt32();
			 

			 if ( !CTcpLayer::SendData(Cmd_Settlement_SettlementWinColse_Notify, NULL, 0, 0, m_pLogin->mnUserID))
			 {
				 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
			 }
			 IntPtr hWnd=this->Handle;
			 CTcpLayer::UnsubscribeAllPkg((int)hWnd);
			 
		 }
};
}
