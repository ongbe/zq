#pragma once

#include "TcpLayer.h"
#include "defineGridCols.h"
#include "CCommonFunc.h"
#include "CDataInfo.h"
#include "FormQueryHistoryFundData.h"
#include "PanelOrganDealerTree.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace CltServerMgr {

	/// <summary>
	/// Summary for FormQueryHistoryFund
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class FormQueryHistoryFund : public System::Windows::Forms::Form
	{
	public:
		FormQueryHistoryFund(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			save = NULL;
			m_pdataInfo = CDataInfo::NewInstance();
			m_bQuerying = false;
			m_bHasExit = false;
			m_bHasChanged_QueryParam = true;
			save = FormQueryHistoryFundData::NewInstance();
			dTableFund = gcnew DataTable();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormQueryHistoryFund()
		{
			if (components)
			{
				delete components;
			}
		}

	private: CDataInfo* m_pdataInfo;
	private: FormQueryHistoryFundData* save;
	private: bool m_bQuerying;
	private: bool m_bHasExit;
	private: bool m_bHasChanged_QueryParam;
	private: unsigned long dwTcpStartTime;
	private: unsigned long dwTcpEndTime;
	private: unsigned long dwShowStartTime;
	private: unsigned long dwShowEndTime;
	private: DataTable ^ dTableFund;

	private: System::Windows::Forms::Label^  label1;
	private: AnywndComboBox::AnywndComboBoxControl^  comboAccount;
	private: PanelOrganDealerTree^  panelOrganDealerTreeList;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::DateTimePicker^  dtpStart;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::DateTimePicker^  dtpEnd;
	private: System::Windows::Forms::Button^  btnResetZero;
	private: System::Windows::Forms::Button^  btnQuery;

	private: SimpleReportControl::SimpleReportControlControl^  viewGridFund;
	private: System::Windows::Forms::Timer^  timerTimeout;
	private: System::Windows::Forms::ProgressBar^  progQueryBar;

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
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->dtpStart = (gcnew System::Windows::Forms::DateTimePicker());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->dtpEnd = (gcnew System::Windows::Forms::DateTimePicker());
			this->btnResetZero = (gcnew System::Windows::Forms::Button());
			this->btnQuery = (gcnew System::Windows::Forms::Button());
			this->timerTimeout = (gcnew System::Windows::Forms::Timer(this->components));
			this->progQueryBar = (gcnew System::Windows::Forms::ProgressBar());
			this->comboAccount = (gcnew AnywndComboBox::AnywndComboBoxControl());
			this->viewGridFund = (gcnew SimpleReportControl::SimpleReportControlControl());
			this->SuspendLayout();
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(9, 12);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(41, 12);
			this->label4->TabIndex = 14;
			this->label4->Text = L"账户号";
			// 
			// dtpStart
			// 
			this->dtpStart->CustomFormat = L"yyyy-MM-dd";
			this->dtpStart->Location = System::Drawing::Point(68, 31);
			this->dtpStart->Name = L"dtpStart";
			this->dtpStart->Size = System::Drawing::Size(110, 21);
			this->dtpStart->TabIndex = 17;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(9, 36);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(53, 12);
			this->label1->TabIndex = 16;
			this->label1->Text = L"起始日期";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(186, 36);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(53, 12);
			this->label2->TabIndex = 16;
			this->label2->Text = L"结束日期";
			// 
			// dtpEnd
			// 
			this->dtpEnd->CustomFormat = L"yyyy-MM-dd";
			this->dtpEnd->Location = System::Drawing::Point(246, 31);
			this->dtpEnd->Name = L"dtpEnd";
			this->dtpEnd->Size = System::Drawing::Size(110, 21);
			this->dtpEnd->TabIndex = 17;
			// 
			// btnResetZero
			// 
			this->btnResetZero->Location = System::Drawing::Point(272, 58);
			this->btnResetZero->Name = L"btnResetZero";
			this->btnResetZero->Size = System::Drawing::Size(83, 24);
			this->btnResetZero->TabIndex = 18;
			this->btnResetZero->Text = L"清空";
			this->btnResetZero->UseVisualStyleBackColor = true;
			this->btnResetZero->Click += gcnew System::EventHandler(this, &FormQueryHistoryFund::btnResetZero_Click);
			// 
			// btnQuery
			// 
			this->btnQuery->Location = System::Drawing::Point(68, 58);
			this->btnQuery->Name = L"btnQuery";
			this->btnQuery->Size = System::Drawing::Size(83, 24);
			this->btnQuery->TabIndex = 19;
			this->btnQuery->Text = L"查询";
			this->btnQuery->UseVisualStyleBackColor = true;
			this->btnQuery->Click += gcnew System::EventHandler(this, &FormQueryHistoryFund::btnQuery_Click);
			// 
			// timerTimeout
			// 
			this->timerTimeout->Interval = 3000;
			this->timerTimeout->Tick += gcnew System::EventHandler(this, &FormQueryHistoryFund::timerTimeout_Tick);
			// 
			// progQueryBar
			// 
			this->progQueryBar->Location = System::Drawing::Point(157, 65);
			this->progQueryBar->Name = L"progQueryBar";
			this->progQueryBar->Size = System::Drawing::Size(109, 10);
			this->progQueryBar->TabIndex = 22;
			// 
			// comboAccount
			// 
			this->comboAccount->AllowDrop = true;
			this->comboAccount->BackColor = System::Drawing::Color::White;
			this->comboAccount->ChildControl = nullptr;
			this->comboAccount->DropDownHeight = 1;
			this->comboAccount->DropDownWidth = 1;
			this->comboAccount->FormattingEnabled = true;
			this->comboAccount->IntegralHeight = false;
			this->comboAccount->Location = System::Drawing::Point(68, 7);
			this->comboAccount->MaxDropDownItems = 1;
			this->comboAccount->Name = L"comboAccount";
			this->comboAccount->Size = System::Drawing::Size(288, 20);
			this->comboAccount->TabIndex = 15;
			// 
			// viewGridFund
			// 
			this->viewGridFund->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->viewGridFund->ConfigGuid = L"{6B51A2FA-3FCE-4691-BE77-A952856AD15C}";
			this->viewGridFund->Location = System::Drawing::Point(8, 86);
			this->viewGridFund->Name = L"viewGridFund";
			this->viewGridFund->RealDataTable = nullptr;
			this->viewGridFund->SimpleMode = true;
			this->viewGridFund->Size = System::Drawing::Size(795, 435);
			this->viewGridFund->StatisticDataTable = nullptr;
			this->viewGridFund->TabIndex = 21;
			// 
			// FormQueryHistoryFund
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(813, 531);
			this->Controls->Add(this->progQueryBar);
			this->Controls->Add(this->viewGridFund);
			this->Controls->Add(this->btnResetZero);
			this->Controls->Add(this->btnQuery);
			this->Controls->Add(this->dtpEnd);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->dtpStart);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->comboAccount);
			this->Controls->Add(this->label4);
			this->Name = L"FormQueryHistoryFund";
			this->Text = L"查询资金历史数据";
			this->Load += gcnew System::EventHandler(this, &FormQueryHistoryFund::FormQueryHistoryFund_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &FormQueryHistoryFund::FormQueryHistoryFund_FormClosed);
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &FormQueryHistoryFund::FormQueryHistoryFund_FormClosing);
			this->Resize += gcnew System::EventHandler(this, &FormQueryHistoryFund::FormQueryHistoryFund_Resize);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

protected:virtual void WndProc( Message% m ) override {

		if(m.Msg == WM_USER_Win && m.WParam.ToInt32()==WndUserCmd_YourPkgArrival) {
			char *p = (char*)m.LParam.ToInt32();
			Stru_UniPkgHead head;
			
			memset(&head, 0, sizeof(head));
			memcpy(&head, p, sizeof(head));
			switch(head.cmdid) {
			case Cmd_RM_QryHistoryFundInfo_Rsp:
			{
				if(head.userdata1 == CF_ERROR_SUCCESS) {
					PlatformStru_TradingAccountInfo* pFundInfo = NULL;
					int nItemCount = head.len/sizeof(PlatformStru_TradingAccountInfo);

					save->LockObject();
					for(int i=0; i<nItemCount; i++) {
						pFundInfo = (PlatformStru_TradingAccountInfo*)
								(p+sizeof(Stru_UniPkgHead)+i*sizeof(PlatformStru_TradingAccountInfo));

						// 这里应该不需要，一般理解为返回即为需要的数据
						if(save->setQueryAccountID.find(pFundInfo->AccountID) 
								== save->setQueryAccountID.end())//判断是不是这个页面查询的，不是则返回
							continue;

						save->queueLastFundInfo.push(*pFundInfo);
					}
					save->UnlockObject();
					
					progQueryBar->Value = progQueryBar->Value+1;

					if(progQueryBar->Value==progQueryBar->Maximum) {
						m_bQuerying = false;
						this->btnQuery->Text = gcnew String("查询");
						save->LockObject();
						if(!save->queueLastFundInfo.empty()) {
							ShowQueue(save->queueLastFundInfo);
						}
						else {
							_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
									CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, "查询历史资金，无数据返回。");
						}
						save->UnlockObject();
					}
					else
						save->NextQuery();

					//if(save->queueLastFundInfo.size() == 0) {
					//	_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
					//			CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
					//			"历史资金查询：本次查询无数据返回。");
					//	return;
					//}
					//
					////std::queue<PlatformStru_TradingAccountInfo> queueRet;
					////save->CheckNewData(m_pdataInfo, queueRet);
					//ShowQueue(save->queueLastFundInfo);

				}
				else {
					_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
							CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, p+sizeof(Stru_UniPkgHead));
				}
			}
				break;
			};
		}

		Form::WndProc(m);
	}

	int GetColIndex(string& strColName) {
#if 1
		for(int i=0; i<dTableFund->Columns->Count; i++) {
			if(dTableFund->Columns[i]->Caption == Tools::string2String(strColName.c_str()))
				return dTableFund->Columns->IndexOf(dTableFund->Columns[i]->ColumnName);
		}
		return -1;
#else
		return dTableFund->Columns->IndexOf(Tools::string2String(strColName.c_str()));
#endif
	}

	void InitGridHead() {

		dTableFund->Columns->Clear();

		int nColID = 0;
		System::Data::DataColumn^ gridColumn;

		gridColumn = dTableFund->Columns->Add(L"交易日", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQA_TradingDay));

		gridColumn = dTableFund->Columns->Add(L"账户号", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQA_InvestorID));

		//gridColumn = dTableFund->Columns->Add(L"账户名称", Type::GetType("System.String"));
		//gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQA_InvestorName));

		gridColumn = dTableFund->Columns->Add(L"期初结存", Type::GetType("System.Double"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQA_StaticProfit));

		gridColumn = dTableFund->Columns->Add(L"期末结存", Type::GetType("System.Double"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQA_DynamicProfit));

		gridColumn = dTableFund->Columns->Add(L"入金金额", Type::GetType("System.Double"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQA_Within));

		gridColumn = dTableFund->Columns->Add(L"出金金额", Type::GetType("System.Double"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQA_Without));

		gridColumn = dTableFund->Columns->Add(L"占用保证金", Type::GetType("System.Double"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQA_CurrMargin));

		gridColumn = dTableFund->Columns->Add(L"可用资金", Type::GetType("System.Double"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQA_Available));

		gridColumn = dTableFund->Columns->Add(L"平仓盈亏", Type::GetType("System.Double"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQA_CloseProfit));

		gridColumn = dTableFund->Columns->Add(L"持仓盈亏", Type::GetType("System.Double"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQA_PositionProfit));

		gridColumn = dTableFund->Columns->Add(L"手续费", Type::GetType("System.Double"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQA_Commission));

		gridColumn = dTableFund->Columns->Add(L"风险度", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQA_RiskDegree));

	}

	void UpdateRow(DataRow^ row, PlatformStru_TradingAccountInfo& lastFund) {

		if(row == nullptr)
			return;
		int nCol = -1;
		char strTemp[1024];

		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQA_TradingDay))))>=0) {
			row[nCol] = Tools::string2String(lastFund.TradingDay);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQA_InvestorID))))>=0) {
			row[nCol] = Tools::string2String(lastFund.AccountID);
		}
		//if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQA_InvestorName))))>=0) {
		//	row[nCol] = Tools::string2String(lastFund.Account);
		//}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQA_StaticProfit))))>=0) {
			sprintf(strTemp, "%0.2f", lastFund.StaticProfit);
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQA_DynamicProfit))))>=0) {
			sprintf(strTemp, "%0.2f", lastFund.DynamicProfit);
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQA_Within))))>=0) {
			sprintf(strTemp, "%0.2f", lastFund.Deposit);
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQA_Without))))>=0) {
			sprintf(strTemp, "%0.2f", lastFund.Withdraw);
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQA_CurrMargin))))>=0) {
			sprintf(strTemp, "%0.2f", lastFund.CurrMargin);
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQA_Available))))>=0) {
			sprintf(strTemp, "%0.2f", lastFund.Available);
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQA_CloseProfit))))>=0) {
			sprintf(strTemp, "%0.2f", lastFund.CloseProfit);
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQA_PositionProfit))))>=0) {
			sprintf(strTemp, "%0.2f", lastFund.PositionProfit);
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQA_Commission))))>=0) {
			sprintf(strTemp, "%0.2f", lastFund.Commission);
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQA_RiskDegree))))>=0) {
			sprintf(strTemp, "%0.1f%%", lastFund.RiskDegree);
			row[nCol] = Tools::string2String(strTemp);
		}

	}

	void ShowQueue(std::queue<PlatformStru_TradingAccountInfo>& queueRet) {

		save->LockObject();
		
		while(!queueRet.empty()) {
			PlatformStru_TradingAccountInfo field = queueRet.front();
			queueRet.pop();

			DataRow^ newRow = dTableFund->NewRow();
			UpdateRow(newRow, field);
			dTableFund->Rows->Add(newRow);
		}
		save->UnlockObject();

		viewGridFund->Refresh2();
	}

	private: System::Void FormQueryHistoryFund_Load(System::Object^  sender, System::EventArgs^  e) {
				 InitGridHead();
				 panelOrganDealerTreeList = gcnew PanelOrganDealerTree();
				 panelOrganDealerTreeList->ResetRelateCombo(comboAccount);
				 comboAccount->ChildControl = panelOrganDealerTreeList;

				 IntPtr hWnd=this->Handle;
				 CTcpLayer::SubscribePkg(Cmd_RM_QryHistoryFundInfo_Rsp, (int)hWnd);

				 ResetComboBoxTextReadOnly(comboAccount);
				 comboAccount->Enabled = true;

				 viewGridFund->RealDataTable = dTableFund;
			 }
	private: System::Void FormQueryHistoryFund_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
				 //if(!m_bHasExit) {
					// Hide();
					// e->Cancel = true;
				 //}
				 save->ResetQuery();
				 dTableFund->Rows->Clear();
			 }
	private: System::Void FormQueryHistoryFund_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
				 IntPtr hWnd=this->Handle;
				 CTcpLayer::UnsubscribeAllPkg((int)hWnd);
			 }
	private: System::Void btnQuery_Click(System::Object^  sender, System::EventArgs^  e) {

				 if(m_bQuerying) {
					if(MessageBox::Show("是否终止本次查询？", 
							"警告", MessageBoxButtons::YesNo, MessageBoxIcon::Warning)
							==::DialogResult::Yes) {
						 save->StopQuery();
						 m_bQuerying = false;
						 progQueryBar->Value = 0;
						 this->btnQuery->Text = gcnew String("查询");
						 if(MessageBox::Show("是否显示未完成的查询结果？", 
								"警告", MessageBoxButtons::YesNo, MessageBoxIcon::Warning)
								==::DialogResult::Yes) {
							save->LockObject();
							if(!save->queueLastFundInfo.empty()) {
								ShowQueue(save->queueLastFundInfo);
							}
							else {
								_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
										CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, "查询历史资金，无数据返回。");
							}
							save->UnlockObject();
						}
					}
					 return;
				 }
				 
				 if(!m_bHasChanged_QueryParam)
					 return;

				 bool bHasData = false;
				 viewGridFund->Clear();
				 dTableFund->Rows->Clear();

				 save->ResetQuery();
				 progQueryBar->Value = 0;

				 std::string strStartDate;
				 std::string strEndDate;
				 std::set<std::string> setAccountID;
				 // 然后根据查询条件获得需要订阅的AccountID集合

				 strStartDate = Tools::String2string(dtpStart->Value.Date.ToString("yyyyMMdd"));
				 strEndDate = Tools::String2string(dtpEnd->Value.Date.ToString("yyyyMMdd"));
				 if(atoi(strEndDate.c_str()) < atoi(strStartDate.c_str())) {
					 MessageBox::Show("起始日期必须小于等于结束日期。");
					 return;
				 }

				 strStartDate = Tools::String2string(dtpStart->Value.Date.ToString("yyyy-MM-dd"));
				 strEndDate = Tools::String2string(dtpEnd->Value.Date.ToString("yyyy-MM-dd"));

				 // 将查询集合保存起来
				 save->LockObject();
				 bHasData = panelOrganDealerTreeList->GetSelectID(save->setQueryAccountID);
				 save->UnlockObject();

				 dwTcpStartTime = GetTickCount();
				 dwTcpEndTime = 0;
				 dwShowStartTime = 0;
				 dwShowEndTime = 0;

				 if(bHasData) {
					 int nCount = 0;
					 sLoginRsp loginRsp;
					 memset(&loginRsp, 0, sizeof(loginRsp));
					 m_pdataInfo->GetLogonInfo(loginRsp);
					 save->NewQuery(strStartDate, strEndDate, loginRsp.mnUserID, nCount);
					 progQueryBar->Minimum = 0;
					 progQueryBar->Maximum = nCount;
					 //this->btnQuery->Enabled = false;
					 //this->timerTimeout->Start();
					 this->btnQuery->Text = gcnew String("停止");
					 m_bQuerying = true;
				 }
				 else
					 MessageBox::Show("所设查询条件未筛选出任何帐号。", 
							"提示", MessageBoxButtons::OK, MessageBoxIcon::Warning);
			 }
	private: System::Void btnResetZero_Click(System::Object^  sender, System::EventArgs^  e) {
				 m_bHasChanged_QueryParam = true;
				 panelOrganDealerTreeList->Clear();
				 if(!m_bHasChanged_QueryParam)
					 return;

				 viewGridFund->Clear();
				 dTableFund->Rows->Clear();
				 save->ResetQuery();
			 }
private: System::Void FormQueryHistoryFund_Resize(System::Object^  sender, System::EventArgs^  e) {
			 System::Drawing::Rectangle rect = this->ClientRectangle;
			 System::Drawing::Rectangle rectGrid = viewGridFund->DisplayRectangle;
			 System::Drawing::Size size(rect.Width-viewGridFund->Left-10, rect.Height-viewGridFund->Top-10);
			 viewGridFund->Size = size;
		 }
private: System::Void timerTimeout_Tick(System::Object^  sender, System::EventArgs^  e) {
			 this->btnQuery->Enabled = true;
			 this->timerTimeout->Stop();
		 }
};
}
