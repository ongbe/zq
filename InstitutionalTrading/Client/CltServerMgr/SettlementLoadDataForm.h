#pragma once
#include "TcpLayer.h"
#include "CommonPkg.h"
#include "CommonDef.h"
#include "CommonErrorCode.h"
#include "SettlementForm.h"
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
	/// Summary for SettlementLoadDataForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class SettlementLoadDataForm : public System::Windows::Forms::Form
	{
	public:
		SettlementLoadDataForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_pLogin = new sLoginRsp();
			m_pDataInfo = CDataInfo::NewInstance();
			m_nDataOption = 0;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~SettlementLoadDataForm()
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
	private: unsigned int   m_nDataOption;
	private: System::Windows::Forms::RadioButton^  radioButton1;
	protected: 
	private: System::Windows::Forms::RadioButton^  radioButton2;

	private: System::Windows::Forms::Button^  button_load;
	private: System::Windows::Forms::ProgressBar^  progressBar1;
	private: System::Windows::Forms::Button^  button_start;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
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
			this->radioButton1 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton2 = (gcnew System::Windows::Forms::RadioButton());
			this->button_load = (gcnew System::Windows::Forms::Button());
			this->progressBar1 = (gcnew System::Windows::Forms::ProgressBar());
			this->button_start = (gcnew System::Windows::Forms::Button());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
			// 
			// radioButton1
			// 
			this->radioButton1->AutoSize = true;
			this->radioButton1->Checked = true;
			this->radioButton1->Location = System::Drawing::Point(6, 20);
			this->radioButton1->Name = L"radioButton1";
			this->radioButton1->Size = System::Drawing::Size(95, 16);
			this->radioButton1->TabIndex = 0;
			this->radioButton1->TabStop = true;
			this->radioButton1->Text = L"盘中原始数据";
			this->radioButton1->UseVisualStyleBackColor = true;
			this->radioButton1->CheckedChanged += gcnew System::EventHandler(this, &SettlementLoadDataForm::radioButton1_CheckedChanged);
			// 
			// radioButton2
			// 
			this->radioButton2->AutoSize = true;
			this->radioButton2->Location = System::Drawing::Point(6, 59);
			this->radioButton2->Name = L"radioButton2";
			this->radioButton2->Size = System::Drawing::Size(137, 16);
			this->radioButton2->TabIndex = 1;
			this->radioButton2->Text = L"重新登录CTP获取数据";
			this->radioButton2->UseVisualStyleBackColor = true;
			this->radioButton2->CheckedChanged += gcnew System::EventHandler(this, &SettlementLoadDataForm::radioButton2_CheckedChanged);
			// 
			// button_load
			// 
			this->button_load->Location = System::Drawing::Point(99, 205);
			this->button_load->Name = L"button_load";
			this->button_load->Size = System::Drawing::Size(127, 27);
			this->button_load->TabIndex = 3;
			this->button_load->Text = L"加载选中数据";
			this->button_load->UseVisualStyleBackColor = true;
			this->button_load->Click += gcnew System::EventHandler(this, &SettlementLoadDataForm::button_load_Click);
			// 
			// progressBar1
			// 
			this->progressBar1->Location = System::Drawing::Point(73, 286);
			this->progressBar1->Name = L"progressBar1";
			this->progressBar1->Size = System::Drawing::Size(210, 27);
			this->progressBar1->TabIndex = 4;
			// 
			// button_start
			// 
			this->button_start->Enabled = false;
			this->button_start->Location = System::Drawing::Point(120, 348);
			this->button_start->Name = L"button_start";
			this->button_start->Size = System::Drawing::Size(75, 27);
			this->button_start->TabIndex = 5;
			this->button_start->Text = L"开始结算";
			this->button_start->UseVisualStyleBackColor = true;
			this->button_start->Click += gcnew System::EventHandler(this, &SettlementLoadDataForm::button_start_Click);
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->radioButton1);
			this->groupBox1->Controls->Add(this->radioButton2);
			this->groupBox1->Location = System::Drawing::Point(83, 70);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(200, 94);
			this->groupBox1->TabIndex = 6;
			this->groupBox1->TabStop = false;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(25, 53);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(41, 12);
			this->label1->TabIndex = 7;
			this->label1->Text = L"label1";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(99, 257);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(41, 12);
			this->label2->TabIndex = 8;
			this->label2->Text = L"label2";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(81, 24);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(41, 12);
			this->label3->TabIndex = 9;
			this->label3->Text = L"label3";
			// 
			// SettlementLoadDataForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(329, 416);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->button_start);
			this->Controls->Add(this->progressBar1);
			this->Controls->Add(this->button_load);
			this->Name = L"SettlementLoadDataForm";
			this->Text = L"加载确认";
			this->Load += gcnew System::EventHandler(this, &SettlementLoadDataForm::SettlementLoadDataForm_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &SettlementLoadDataForm::SettlementLoadDataForm_FormClosed);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void button_load_Click(System::Object^  sender, System::EventArgs^  e) 
			 {				 

				/* time_t lTime;
				 struct tm * lTimeinfo; 
				 time(&lTime);
				 lTimeinfo = localtime(&lTime);*/
				 _SettlementDate lcBuf = {0};
				  strncpy(lcBuf,Tools::String2string(Tradingday).c_str(),Tradingday->Length);
				 //sprintf_s(lcBuf,"%04d-%02d-%02d",1900+lTimeinfo->tm_year,1+lTimeinfo->tm_mon,lTimeinfo->tm_mday);
				 int  nbHistorySettlement = 0;

				 if ( !CTcpLayer::SendData(Cmd_Settlement_LoadData_Req, lcBuf, sizeof(_SettlementDate), 0, m_pLogin->mnUserID,m_nDataOption,nbHistorySettlement))
				 {
					 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
				 }
				 this->progressBar1->Style = System::Windows::Forms::ProgressBarStyle::Marquee;
				 label2->Text =Tools::string2String("正在加载数据，请稍后");
				 button_load->Enabled =false;
				 this->button_start->Enabled = false;

				 LOG_INFO("结算-当日数据结算  用户 %d,服务器IP %s,加载数据 按钮点下，加载选项 %d",m_pLogin->mnUserID,CConfigManager::GetSvrAddr().c_str(),m_nDataOption);
			 }
private: System::Void button_start_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 SettlementForm^ dlg = gcnew SettlementForm(Tradingday);
			 dlg->ShowDialog();
			 this->Close();
			 LOG_INFO("结算-当日数据结算 用户 %d,服务器IP %s,开始结算 按钮点下",m_pLogin->mnUserID,CConfigManager::GetSvrAddr().c_str());
		 }
private: System::Void SettlementLoadDataForm_Load(System::Object^  sender, System::EventArgs^  e) 
		 {
			 this->CenterToParent(); 
			 int nHwnd = this->Handle.ToInt32();
			 CTcpLayer::SubscribePkg(Cmd_Settlement_LoadData_Rsp, nHwnd);
			 CTcpLayer::SubscribePkg(Cmd_Settlement_QueryCTPLoadStatus_Rsp, nHwnd);
			 CTcpLayer::SubscribePkg(Cmd_Settlement_QuerySettlementPrice_Rsp, nHwnd);
			 CTcpLayer::SubscribePkg(Cmd_Settlement_QueryTradingday_Rsp, nHwnd);

			 button_load->Enabled = false;

			  label1->Text =Tools::string2String(" ");
			  label2->Text =Tools::string2String(" ");
			  label3->Text =Tools::string2String(" ");

			 m_pDataInfo->GetLogonInfo(*m_pLogin);

			 if ( !CTcpLayer::SendData(Cmd_Settlement_QueryTradingday_Req, NULL, 0, 0, m_pLogin->mnUserID))
			 {
				 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
			 }
		 }
private: System::Void radioButton1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(!((RadioButton^)sender)->Checked)
				 return;
			m_nDataOption = ((RadioButton^)sender)->TabIndex;
				

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
									  this->progressBar1->Style = System::Windows::Forms::ProgressBarStyle::Blocks;
									   progressBar1->Value = progressBar1->Maximum;
									   this->button_start->Enabled = true;
									   button_load->Enabled =true;
									   label2->Text =Tools::string2String("加载完成");
									   
									   //从CTP加载需要判断每个交易员是否加载成功
									   if(head.userdata2 == 1)
									   {
										   if ( !CTcpLayer::SendData(Cmd_Settlement_QueryCTPLoadStatus_Req, NULL, 0, 0, m_pLogin->mnUserID))
										   {
											   MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
										   }
									   }
								  }
								  else
								  {
									  this->progressBar1->Style = System::Windows::Forms::ProgressBarStyle::Blocks;
									  progressBar1->Value = progressBar1->Maximum;
									  label2->Text =Tools::string2String("       加载失败");
									  this->button_start->Enabled = false;
									  button_load->Enabled =true;
									  string str = (char*)(p+sizeof(Stru_UniPkgHead));
									  String^ strtemp = Tools::string2String(str.c_str());
										_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
												CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
												str.c_str());
								  }

								  break;
							  }
						  case Cmd_Settlement_QueryCTPLoadStatus_Rsp:
							  {
								 
									  string str = (char*)(p+sizeof(Stru_UniPkgHead));
									  String^ strtemp = Tools::string2String(str.c_str());
									  _GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
										  CPopMsgWin::AddCRLF, CPopMsgWin::NoInsertTime, 
										  str.c_str());
								 

								  break;
							  }
						  case Cmd_Settlement_QueryTradingday_Rsp:
							  {
								  if ( head.userdata1 == CF_ERROR_SUCCESS )
								  {
									  string strTradingday = (char*)(p+sizeof(Stru_UniPkgHead));
									  Tradingday = Tools::string2String(strTradingday.c_str());
									  
									  label3->Text =Tools::string2String(("待结算交易日："+strTradingday).c_str());

									  if (head.userdata2 == 0)
									  {
										  label1->Text =Tools::string2String("          提示：收盘后方可进行结算");
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
									  this->button_start->Enabled = false;
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

										  label1->Text =Tools::string2String("提示：结算价获取不全，点击加载数据后将无法继续接收");
									  }

                                      button_load->Enabled = true;
									
								  }
								  else
								  {
									  this->button_start->Enabled = false;
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
private: System::Void radioButton2_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(!((RadioButton^)sender)->Checked)
				 return;
			 m_nDataOption = ((RadioButton^)sender)->TabIndex;
		 }
private: System::Void SettlementLoadDataForm_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) 
		 {
			 int nHwnd = this->Handle.ToInt32();
			 m_pDataInfo->GetLogonInfo(*m_pLogin);

			 if ( !CTcpLayer::SendData(Cmd_Settlement_SettlementWinColse_Notify, NULL, 0, 0, m_pLogin->mnUserID))
			 {
				 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
			 }
			 IntPtr hWnd=this->Handle;
			 CTcpLayer::UnsubscribeAllPkg((int)hWnd);
			 
		 }
};
}
