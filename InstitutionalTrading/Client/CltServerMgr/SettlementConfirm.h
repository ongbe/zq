#pragma once
#include "TcpLayer.h"
#include "CommonPkg.h"
#include "CommonDef.h"
#include "CommonErrorCode.h"
#include "ConfigMgr.h"
#include "CommonUserOpr.h"
#include "CommonStruct.h"
#include "CDataInfo.h"
#include "Tools.h"
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace CltServerMgr {

	/// <summary>
	/// Summary for SettlementConfirm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class SettlementConfirm : public System::Windows::Forms::Form
	{
	public:
		SettlementConfirm(void)
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
		~SettlementConfirm()
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
	private: String^ Tradingday;
	private: sLoginRsp*     m_pLogin;
	private: CDataInfo*     m_pDataInfo;
	private: System::Windows::Forms::Label^  label1;
	protected: 
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::ProgressBar^  progressBar1;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;

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
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->progressBar1 = (gcnew System::Windows::Forms::ProgressBar());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(75, 52);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(65, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"结算日期：";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(146, 52);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(41, 12);
			this->label2->TabIndex = 1;
			this->label2->Text = L"label2";
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(77, 146);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(131, 23);
			this->button1->TabIndex = 2;
			this->button1->Text = L"开始结算";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &SettlementConfirm::button1_Click);
			// 
			// progressBar1
			// 
			this->progressBar1->Location = System::Drawing::Point(43, 207);
			this->progressBar1->Name = L"progressBar1";
			this->progressBar1->Size = System::Drawing::Size(203, 23);
			this->progressBar1->TabIndex = 3;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(30, 296);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(257, 12);
			this->label3->TabIndex = 4;
			this->label3->Text = L"提示：开始自动结算后服务器会切换到结算状态";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(12, 93);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(41, 12);
			this->label4->TabIndex = 5;
			this->label4->Text = L"label4";
			// 
			// SettlementConfirm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(319, 403);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->progressBar1);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Name = L"SettlementConfirm";
			this->Text = L"结算确认";
			this->Load += gcnew System::EventHandler(this, &SettlementConfirm::SettlementConfirm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void SettlementConfirm_Load(System::Object^  sender, System::EventArgs^  e) 
			 {
				 this->CenterToParent(); 
				 int nHwnd = this->Handle.ToInt32();
				 CTcpLayer::SubscribePkg(Cmd_Settlement_ImmediatelyAutoSettlement_Rsp, nHwnd);
				 CTcpLayer::SubscribePkg(Cmd_Settlement_QuerySettlementPrice_Rsp, nHwnd);
				 CTcpLayer::SubscribePkg(Cmd_Settlement_QueryTradingday_Rsp, nHwnd);

				 label4->Text =Tools::string2String(" ");
				 label2->Text =Tools::string2String(" ");

				 m_pDataInfo->GetLogonInfo(*m_pLogin);

				 if ( !CTcpLayer::SendData(Cmd_Settlement_QueryTradingday_Req, NULL, 0, 0, m_pLogin->mnUserID))
				 {
					 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
				 }
				 this->button1->Enabled = false;
			 }
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if ( !CTcpLayer::SendData(Cmd_Settlement_ImmediatelyAutoSettlement_Req, NULL, 0, 0, m_pLogin->mnUserID))
			 {
				 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
			 }
			 this->progressBar1->Style = System::Windows::Forms::ProgressBarStyle::Marquee;
			 label4->Text =Tools::string2String("        正在结算，请稍后");
			 button1->Enabled =false;			

			 LOG_INFO("结算-立即自动结算  用户 %d,服务器IP %s,开始结算 按钮点下",m_pLogin->mnUserID,CConfigManager::GetSvrAddr().c_str());

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
				  case Cmd_Settlement_ImmediatelyAutoSettlement_Rsp:
					  {
						  if ( head.userdata1 == CF_ERROR_SUCCESS )
						  {
							  this->progressBar1->Style = System::Windows::Forms::ProgressBarStyle::Blocks;
							  progressBar1->Value = progressBar1->Maximum;
							  
							  label4->Text =Tools::string2String("           结算完成");
							  MessageBox::Show("结算完成，需初始化交易服务器才能进入交易状态，点击确认并关闭结算窗口！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Information);
							 
						  }
						  else
						  {
							  this->progressBar1->Style = System::Windows::Forms::ProgressBarStyle::Blocks;
							  progressBar1->Value = progressBar1->Maximum;
							  label4->Text =Tools::string2String("           结算失败");
							 
							  string str = (char*)(p+sizeof(Stru_UniPkgHead));
							  String^ strtemp = Tools::string2String(str.c_str());
							  _GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
								  CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
								  str.c_str());
						  }

						  break;
					  }

				  case Cmd_Settlement_QueryTradingday_Rsp:
					  {
						  if ( head.userdata1 == CF_ERROR_SUCCESS )
						  {
							  string strTradingday = (char*)(p+sizeof(Stru_UniPkgHead));
							  Tradingday = Tools::string2String(strTradingday.c_str());

							  label2->Text =Tools::string2String(strTradingday.c_str());

							  if (head.userdata2 == 0)
							  {
								  label4->Text =Tools::string2String("          提示：收盘后方可进行结算");
								  this->button1->Enabled = false;
							  }
							  else
							  {
								  //查询到交易日后再查询结算价
								  int  nbHistorySettlement = 0;
								  if ( !CTcpLayer::SendData(Cmd_Settlement_QuerySettlementPrice_Req, NULL, 0, 0, m_pLogin->mnUserID,nbHistorySettlement))
								  {
									  MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
								  }
							  }

						  }
						  else
						  {
							  this->button1->Enabled = false;
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
							 if(m_pDataInfo->CheckSettlePrice() == false)
							  {

								  label4->Text =Tools::string2String("提示：结算价获取不全，点击加载数据后将无法继续接收");
								  this->button1->Enabled = false;
							  }
							  else
							      button1->Enabled = true;

						  }
						  else
						  {
							  this->button1->Enabled = false;
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
};
}
