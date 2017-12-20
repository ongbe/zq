#pragma once
#include "stdafx.h"
#include "Tools.h"
#include "TcpLayer.h"
#include "CommonPkg.h"
#include "CommonDef.h"
#include "CommonErrorCode.h"
#include "CDataInfo.h"
#include "CommonStruct.h"
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace CltServerMgr {

	/// <summary>
	/// Summary for LoadHistoryCommission
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class LoadHistoryCommission : public System::Windows::Forms::Form
	{
	public:
		LoadHistoryCommission(void)
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
		~LoadHistoryCommission()
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
	private: System::Windows::Forms::Button^  button_loadVacancy;
	private: System::Windows::Forms::Button^  button_loadAll;
	private: System::Windows::Forms::Button^  button_cancel;
	private: System::Windows::Forms::Label^  label2;

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
			this->button_loadVacancy = (gcnew System::Windows::Forms::Button());
			this->button_loadAll = (gcnew System::Windows::Forms::Button());
			this->button_cancel = (gcnew System::Windows::Forms::Button());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(23, 25);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(29, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"日期";
			// 
			// dateTimePicker1
			// 
			this->dateTimePicker1->CustomFormat = L"yyyy-MM-dd";
			this->dateTimePicker1->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
			this->dateTimePicker1->Location = System::Drawing::Point(84, 15);
			this->dateTimePicker1->Name = L"dateTimePicker1";
			this->dateTimePicker1->Size = System::Drawing::Size(141, 21);
			this->dateTimePicker1->TabIndex = 1;
			// 
			// button_loadVacancy
			// 
			this->button_loadVacancy->Location = System::Drawing::Point(84, 89);
			this->button_loadVacancy->Name = L"button_loadVacancy";
			this->button_loadVacancy->Size = System::Drawing::Size(141, 23);
			this->button_loadVacancy->TabIndex = 2;
			this->button_loadVacancy->Text = L"导入空缺费率";
			this->button_loadVacancy->UseVisualStyleBackColor = true;
			this->button_loadVacancy->Click += gcnew System::EventHandler(this, &LoadHistoryCommission::button_loadVacancy_Click);
			// 
			// button_loadAll
			// 
			this->button_loadAll->Location = System::Drawing::Point(84, 153);
			this->button_loadAll->Name = L"button_loadAll";
			this->button_loadAll->Size = System::Drawing::Size(141, 23);
			this->button_loadAll->TabIndex = 3;
			this->button_loadAll->Text = L"导入全部费率";
			this->button_loadAll->UseVisualStyleBackColor = true;
			this->button_loadAll->Click += gcnew System::EventHandler(this, &LoadHistoryCommission::button_loadAll_Click);
			// 
			// button_cancel
			// 
			this->button_cancel->Location = System::Drawing::Point(84, 220);
			this->button_cancel->Name = L"button_cancel";
			this->button_cancel->Size = System::Drawing::Size(141, 23);
			this->button_cancel->TabIndex = 4;
			this->button_cancel->Text = L"取消";
			this->button_cancel->UseVisualStyleBackColor = true;
			this->button_cancel->Click += gcnew System::EventHandler(this, &LoadHistoryCommission::button_cancel_Click);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(82, 278);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(41, 12);
			this->label2->TabIndex = 5;
			this->label2->Text = L"label2";
			// 
			// LoadHistoryCommission
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(319, 329);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->button_cancel);
			this->Controls->Add(this->button_loadAll);
			this->Controls->Add(this->button_loadVacancy);
			this->Controls->Add(this->dateTimePicker1);
			this->Controls->Add(this->label1);
			this->Name = L"LoadHistoryCommission";
			this->Text = L"导入历史费率";
			this->Load += gcnew System::EventHandler(this, &LoadHistoryCommission::LoadHistoryCommission_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &LoadHistoryCommission::LoadHistoryCommission_FormClosed);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void button_loadVacancy_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 string date=Tools::String2string(dateTimePicker1->Text);
				 std::vector<QryHisRateCondition> vecCon;
				 std::vector<RateKey> vecRate;
				 m_pDataInfo->GetQryRate(vecRate,false);
				 std::vector<RateKey>::iterator it= vecRate.begin();
				 while (it!=vecRate.end())
				 {
					 QryHisRateCondition condition;
					 memset(&condition,0,sizeof(condition));
					 condition.conditionKey=*it;
					 strncpy(condition.loadDate,date.c_str(),sizeof(condition.loadDate)-1);
					 vecCon.push_back(condition);
					 it++;
				 }


				 if (vecCon.size()==0)
				 {
					 _GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
						 CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
						 "今日未查询到合约，无法加载历史费率");
				 }
				 else
				 {
					 if ( !CTcpLayer::SendData(Cmd_Settlement_LoadHistoryCommission_Req, &vecCon[0], sizeof(QryHisRateCondition)*vecCon.size(), 0, m_pLogin->mnUserID))
					 {
						 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
					 }
					 else
						 this->label2->Text = "正在导入历史费率，请稍后 ";
				 }
			 }
private: System::Void button_loadAll_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 string date=Tools::String2string(dateTimePicker1->Text);
			 std::vector<QryHisRateCondition> vecCon;
			 std::vector<RateKey> vecRate;
			 m_pDataInfo->GetQryRate(vecRate,true);
			 std::vector<RateKey>::iterator it= vecRate.begin();
			 while (it!=vecRate.end())
			 {
				 QryHisRateCondition condition;
				 memset(&condition,0,sizeof(condition));
				 condition.conditionKey=*it;
				 strncpy(condition.loadDate,date.c_str(),sizeof(condition.loadDate)-1);
				 vecCon.push_back(condition);
				 it++;
			 }


			 if (vecCon.size()==0)
			 {
				 _GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
					 CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
					 "今日未查询到合约，无法加载历史费率");
			 }
			 else
			 {
				 if ( !CTcpLayer::SendData(Cmd_Settlement_LoadHistoryCommission_Req, &vecCon[0], sizeof(QryHisRateCondition)*vecCon.size(), 0, m_pLogin->mnUserID))
				 {
					 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
				 }
				 else
					 this->label2->Text = "正在导入历史费率，请稍后 ";
			 }
			 
		 }
private: System::Void button_cancel_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 this->Close();
		 }
private: System::Void LoadHistoryCommission_Load(System::Object^  sender, System::EventArgs^  e) 
		 {
			 this->dateTimePicker1->MaxDate = System::DateTime::Now;
			 m_pDataInfo->GetLogonInfo(*m_pLogin);
			 this->CenterToParent(); 
			 int nHwnd = this->Handle.ToInt32();			
			 CTcpLayer::SubscribePkg(Cmd_Settlement_LoadHistoryCommission_Rsp, nHwnd);
			 label2->Text =Tools::string2String(" ");
		 }
private: System::Void LoadHistoryCommission_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) 
		 {			
			 IntPtr hWnd=this->Handle;
			 CTcpLayer::UnsubscribeAllPkg((int)hWnd);
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
				  case Cmd_Settlement_LoadHistoryCommission_Rsp:
					  {
						  if ( head.userdata1 == CF_ERROR_SUCCESS )
						  {


							 // std::map<RateKey,CommissionMarginRate>  lMapFiled;
							  int nCount = head.len / sizeof(CommissionMarginRate);
							  //if(nCount == 0)
								 // break;
							  for ( int i = 0; i < nCount; i++ )
							  {
								  CommissionMarginRate* pField = (CommissionMarginRate*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(CommissionMarginRate));
								  RateKey key(pField->AccountID,pField->InstrumentID,pField->BrokerID);
								  m_pDataInfo->SetCommissionMarginRate(key,*pField);
							  }
							 // m_pDataInfo->ClearCommissionMarginRate();
							  //m_pDataInfo->SetCommissionMarginRate(lMapFiled);
							  _GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
								  CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
								  "加载历史费率完成");

						  }
						  else
						  {
							  string str = (char*)(p+sizeof(Stru_UniPkgHead));
							  String^ strtemp = Tools::string2String(str.c_str());
							  _GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
								  CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
								  str.c_str());
						  }
						  this->DialogResult = System::Windows::Forms::DialogResult::Yes;
						  this->Close();

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
