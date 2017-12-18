#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include <set>
#include <map>
#include <queue>
#include "defineGridCols.h"
#include "CCommonFunc.h"
#include "TcpLayer.h"
#include "CDataInfo.h"
#include "CFieldDetail.h"
#include "RiskManageCmd.h"
#include "UserApiStruct.h"
#include "CommonPkg.h"
#include "CDlgRealTimeFundInfoData.h"
#include "CFinancialProductIDListDlg.h"
#include "CAssetMgmtOrganIDTreeListDlg.h"
#include "COrganAndAccountIDTreeListDlg.h"

namespace Client {

	/// <summary>
	/// Summary for CDlgRealTimeFundInfo
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class CDlgRealTimeFundInfo : public System::Windows::Forms::Form
	{
	public:
		CDlgRealTimeFundInfo(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			save = NULL;
			m_pdataInfo = CDataInfo::NewInstance();
			m_bHasExit = false;
			m_bHasChanged_QueryParam = true;
			save = CDlgRealTimeFundInfoData::NewInstance();
		}
		void ReleaseForm() {
			m_bHasExit = true;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~CDlgRealTimeFundInfo()
		{
			if (components)
			{
				delete components;
			}
			CDlgRealTimeFundInfoData::ReleaseInstance(save);
			save = NULL;
		}
	private: CDataInfo* m_pdataInfo;
	private: CDlgRealTimeFundInfoData* save;
	private: bool m_bHasExit;
	private: bool m_bHasChanged_QueryParam;
	private: unsigned long dwTcpStartTime;
	private: unsigned long dwTcpEndTime;
	private: unsigned long dwShowStartTime;
	private: unsigned long dwShowEndTime;

	private: CAssetMgmtOrganIDTreeListDlg^ dlgMgmtOrganIDTreeList;
	private: CFinancialProductIDListDlg^ dlgFinancialProductIDList;
	private: COrganAndAccountIDTreeListDlg^ dlgOrganAndAccountIDTreeList;

	protected: 
	private: System::Windows::Forms::DataGridView^  dataGridFund;

	private: System::Windows::Forms::Button^  btnResetZero;
	private: System::Windows::Forms::Button^  btnQuery;

	private: System::Windows::Forms::RadioButton^  radioGroupQuery;
	private: System::Windows::Forms::RadioButton^  radioAccountQuery;
	private: AnywndComboBox::AnywndComboBoxControl^  comboOrganization;
	private: AnywndComboBox::AnywndComboBoxControl^  comboFinaProduct;
	private: AnywndComboBox::AnywndComboBoxControl^  comboAccount;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
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
			this->radioGroupQuery = (gcnew System::Windows::Forms::RadioButton());
			this->dataGridFund = (gcnew System::Windows::Forms::DataGridView());
			this->btnResetZero = (gcnew System::Windows::Forms::Button());
			this->btnQuery = (gcnew System::Windows::Forms::Button());
			this->comboFinaProduct = (gcnew AnywndComboBox::AnywndComboBoxControl());
			this->comboAccount = (gcnew AnywndComboBox::AnywndComboBoxControl());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->comboOrganization = (gcnew AnywndComboBox::AnywndComboBoxControl());
			this->radioAccountQuery = (gcnew System::Windows::Forms::RadioButton());
			this->label1 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridFund))->BeginInit();
			this->SuspendLayout();
			// 
			// radioGroupQuery
			// 
			this->radioGroupQuery->AutoSize = true;
			this->radioGroupQuery->Location = System::Drawing::Point(7, 12);
			this->radioGroupQuery->Name = L"radioGroupQuery";
			this->radioGroupQuery->Size = System::Drawing::Size(71, 16);
			this->radioGroupQuery->TabIndex = 6;
			this->radioGroupQuery->TabStop = true;
			this->radioGroupQuery->Text = L"组合查询";
			this->radioGroupQuery->UseVisualStyleBackColor = true;
			this->radioGroupQuery->Click += gcnew System::EventHandler(this, &CDlgRealTimeFundInfo::radioGroupQuery_Click);
			// 
			// dataGridFund
			// 
			this->dataGridFund->AllowUserToAddRows = false;
			this->dataGridFund->AllowUserToDeleteRows = false;
			this->dataGridFund->AllowUserToResizeRows = false;
			this->dataGridFund->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridFund->EditMode = System::Windows::Forms::DataGridViewEditMode::EditProgrammatically;
			this->dataGridFund->Location = System::Drawing::Point(9, 96);
			this->dataGridFund->Margin = System::Windows::Forms::Padding(1);
			this->dataGridFund->MultiSelect = false;
			this->dataGridFund->Name = L"dataGridFund";
			this->dataGridFund->ReadOnly = true;
			this->dataGridFund->RowHeadersVisible = false;
			this->dataGridFund->RowTemplate->Height = 23;
			this->dataGridFund->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dataGridFund->Size = System::Drawing::Size(613, 307);
			this->dataGridFund->TabIndex = 17;
			// 
			// btnResetZero
			// 
			this->btnResetZero->Location = System::Drawing::Point(129, 62);
			this->btnResetZero->Name = L"btnResetZero";
			this->btnResetZero->Size = System::Drawing::Size(83, 24);
			this->btnResetZero->TabIndex = 15;
			this->btnResetZero->Text = L"清空";
			this->btnResetZero->UseVisualStyleBackColor = true;
			this->btnResetZero->Click += gcnew System::EventHandler(this, &CDlgRealTimeFundInfo::btnResetZero_Click);
			// 
			// btnQuery
			// 
			this->btnQuery->Location = System::Drawing::Point(26, 62);
			this->btnQuery->Name = L"btnQuery";
			this->btnQuery->Size = System::Drawing::Size(83, 24);
			this->btnQuery->TabIndex = 16;
			this->btnQuery->Text = L"查询";
			this->btnQuery->UseVisualStyleBackColor = true;
			this->btnQuery->Click += gcnew System::EventHandler(this, &CDlgRealTimeFundInfo::btnQuery_Click);
			// 
			// comboFinaProduct
			// 
			this->comboFinaProduct->BackColor = System::Drawing::Color::White;
			this->comboFinaProduct->ChildControl = nullptr;
			this->comboFinaProduct->DropDownHeight = 1;
			this->comboFinaProduct->DropDownWidth = 1;
			this->comboFinaProduct->FormattingEnabled = true;
			this->comboFinaProduct->IntegralHeight = false;
			this->comboFinaProduct->Location = System::Drawing::Point(336, 8);
			this->comboFinaProduct->MaxDropDownItems = 1;
			this->comboFinaProduct->Name = L"comboFinaProduct";
			this->comboFinaProduct->Size = System::Drawing::Size(100, 20);
			this->comboFinaProduct->TabIndex = 11;
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
			this->comboAccount->Location = System::Drawing::Point(162, 34);
			this->comboAccount->MaxDropDownItems = 1;
			this->comboAccount->Name = L"comboAccount";
			this->comboAccount->Size = System::Drawing::Size(274, 20);
			this->comboAccount->TabIndex = 13;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(277, 14);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(53, 12);
			this->label2->TabIndex = 8;
			this->label2->Text = L"理财产品";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(103, 40);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(41, 12);
			this->label4->TabIndex = 9;
			this->label4->Text = L"账户号";
			// 
			// comboOrganization
			// 
			this->comboOrganization->BackColor = System::Drawing::Color::White;
			this->comboOrganization->ChildControl = nullptr;
			this->comboOrganization->DropDownHeight = 1;
			this->comboOrganization->DropDownWidth = 1;
			this->comboOrganization->FormattingEnabled = true;
			this->comboOrganization->IntegralHeight = false;
			this->comboOrganization->Location = System::Drawing::Point(162, 8);
			this->comboOrganization->MaxDropDownItems = 1;
			this->comboOrganization->Name = L"comboOrganization";
			this->comboOrganization->Size = System::Drawing::Size(100, 20);
			this->comboOrganization->TabIndex = 12;
			// 
			// radioAccountQuery
			// 
			this->radioAccountQuery->AutoSize = true;
			this->radioAccountQuery->Location = System::Drawing::Point(7, 38);
			this->radioAccountQuery->Name = L"radioAccountQuery";
			this->radioAccountQuery->Size = System::Drawing::Size(71, 16);
			this->radioAccountQuery->TabIndex = 5;
			this->radioAccountQuery->TabStop = true;
			this->radioAccountQuery->Text = L"指定查询";
			this->radioAccountQuery->UseVisualStyleBackColor = true;
			this->radioAccountQuery->Click += gcnew System::EventHandler(this, &CDlgRealTimeFundInfo::radioAccountQuery_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(104, 14);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(53, 12);
			this->label1->TabIndex = 7;
			this->label1->Text = L"组织架构";
			// 
			// CDlgRealTimeFundInfo
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(632, 413);
			this->Controls->Add(this->radioGroupQuery);
			this->Controls->Add(this->dataGridFund);
			this->Controls->Add(this->btnResetZero);
			this->Controls->Add(this->btnQuery);
			this->Controls->Add(this->comboFinaProduct);
			this->Controls->Add(this->comboAccount);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->comboOrganization);
			this->Controls->Add(this->radioAccountQuery);
			this->Controls->Add(this->label1);
			this->Name = L"CDlgRealTimeFundInfo";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"实时资金账户";
			this->Load += gcnew System::EventHandler(this, &CDlgRealTimeFundInfo::CDlgRealTimeFundInfo_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &CDlgRealTimeFundInfo::CDlgRealTimeFundInfo_FormClosed);
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &CDlgRealTimeFundInfo::CDlgRealTimeFundInfo_FormClosing);
			this->Resize += gcnew System::EventHandler(this, &CDlgRealTimeFundInfo::CDlgRealTimeFundInfo_Resize);
			this->ResizeEnd += gcnew System::EventHandler(this, &CDlgRealTimeFundInfo::CDlgRealTimeFundInfo_ResizeEnd);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridFund))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

protected:virtual void WndProc( Message% m ) override {

		if(m.Msg == WM_COMMAND_Win && m.WParam.ToInt32()==WndCmd_YourPkgArrival) {
			char *p = (char*)m.LParam.ToInt32();
			Stru_UniPkgHead head;
			
			memset(&head, 0, sizeof(head));
			memcpy(&head, p, sizeof(head));
			switch(head.cmdid) {
			case Cmd_RM_SubscribeFund_Rsp:
			{
				dwTcpEndTime = GetTickCount();

				if(head.userdata1 == OPERATOR_SUCCESS) {
					RiskAllAccountField* pFundInfo = NULL;
					int nItemCount = head.len/sizeof(RiskAllAccountField);

					save->LockObject();
					for(int i=0; i<nItemCount; i++) {
						pFundInfo = (RiskAllAccountField*)
								(p+sizeof(Stru_UniPkgHead)+i*sizeof(RiskAllAccountField));
						
						TrustTradeAccount traderAccount;
						if(!m_pdataInfo->GetTrustTradeAccount(std::string(pFundInfo->pre.InvestorID), traderAccount)) 
							continue;
						std::set<int>::iterator it = save->setQueryAccountID.find(traderAccount.nTradeAccountID);
						if( it== save->setQueryAccountID.end())//判断是不是这个页面查询的，不是则返回
							continue;

						save->queueAllFundInfo.push(*pFundInfo);
					}
					save->UnlockObject();
					if(head.userdata2 == head.userdata3 - 1) {
						dwShowStartTime = GetTickCount();

						std::queue<RiskAllAccountField> queueRet;
						save->CheckNewData(m_pdataInfo, queueRet);
						ShowQueue(queueRet);
		
						dwShowEndTime = GetTickCount();
						{
							char csText[1024] = {0};
							sprintf(csText, "实时资金查询，本次查询时间消耗 传输==%u, 显示==%u",
									dwTcpEndTime-dwTcpStartTime, dwShowEndTime-dwShowStartTime);
							this->Text = Tools::string2String(csText);
						}
					}
				}
				else {
					p[sizeof(head)+head.len-1]=0;
					//TRACE(p+sizeof(head));
				}
			}
				break;
			case Cmd_RM_UnSubscribeFund_Rsp:
				if(head.userdata1 == OPERATOR_SUCCESS) {
				}
				else {
					p[sizeof(head)+head.len-1]=0;
					//TRACE(p+sizeof(head));
				}
				break;
			case Cmd_RM_Fund_Push:
			{
				if(head.userdata1 == OPERATOR_SUCCESS) {
					RiskSyncAccountField* pFundInfo = NULL;
					int nItemCount = head.len/sizeof(RiskSyncAccountField);

					save->LockObject();
					for(int i=0; i<nItemCount; i++) {
						pFundInfo = (RiskSyncAccountField*)
								(p+sizeof(Stru_UniPkgHead)+i*sizeof(RiskSyncAccountField));

						TrustTradeAccount traderAccount;
						if(!m_pdataInfo->GetTrustTradeAccount(std::string(pFundInfo->InvestorID), traderAccount)) 
							continue;
						std::set<int>::iterator it = save->setQueryAccountID.find(traderAccount.nTradeAccountID);
						if( it== save->setQueryAccountID.end())//判断是不是这个页面查询的，不是则返回
							continue;

						save->queueLastFundInfo.push(*pFundInfo);
					}
					save->UnlockObject();

					if(save->queueLastFundInfo.size() == 0)
						return;
					
					std::queue<RiskSyncAccountField> queueRet;
					save->CheckNewData(m_pdataInfo, queueRet);
					ShowQueue(queueRet);

				}
			}
				break;
			};
		}

		Form::WndProc(m);
	}
	int GetRowIndexByRowID(int nRowID) {
		for(int i=0; i<dataGridFund->Rows->Count; i++) {
			if(int::Parse(dataGridFund->Rows[i]->Tag->ToString()) == nRowID)
				return dataGridFund->Rows[i]->Index;
		}
		return -1;
	}
	int GetColIndexByColID(int nColID) {
		for(int i=0; i<dataGridFund->Columns->Count; i++) {
			if(int::Parse(dataGridFund->Columns[i]->Tag->ToString()) == nColID)
				return dataGridFund->Columns[i]->Index;
		}
		return -1;
	}
	void InitGridHead() {

		dataGridFund->Columns->Clear();

		int nColID = 0;

		System::Windows::Forms::DataGridViewTextBoxColumn^ gridColumn = 
				(gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQA_InvestorID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"投资者代码";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_InvestorID));
		dataGridFund->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQA_CloseProfit;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"平仓盈亏";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_CloseProfit));
		dataGridFund->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQA_PositionProfit;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"持仓盈亏";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_PositionProfit));
		dataGridFund->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQA_Balance;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"总权益";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_Balance));
		dataGridFund->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQA_Available;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"可用资金";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_Available));
		dataGridFund->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQA_Commission;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"手续费";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_Commission));
		dataGridFund->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQA_CurrMargin;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"当前保证金总额";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_CurrMargin));
		dataGridFund->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQA_FrozenMargin;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"保证金冻结";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_FrozenMargin));
		dataGridFund->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQA_FrozenCommission;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"手续费冻结";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_FrozenCommission));
		dataGridFund->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQA_AssertMgmtOrgan;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"账号所属资管";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_AssertMgmtOrgan));
		dataGridFund->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQA_FinanProduct;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"账号所属理财产品";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_FinanProduct));
		dataGridFund->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQA_BrokerID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"经纪公司代码";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_BrokerID));
		dataGridFund->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQA_Withdraw;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"出入金金额";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_Withdraw));
		dataGridFund->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQA_Mortgage;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"质押金额";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_Mortgage));
		dataGridFund->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQA_ExchangeMargin;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"交易所保证金";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_ExchangeMargin));
		dataGridFund->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQA_WithdrawQuota;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"可取资金";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_WithdrawQuota));
		dataGridFund->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQA_Credit;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"信用额度";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_Credit));
		dataGridFund->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQA_AccountID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"投资者帐号";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_AccountID));
		dataGridFund->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQA_PreMortgage;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"上次质押金额";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_PreMortgage));
		dataGridFund->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQA_PreCredit;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"上次信用额度";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_PreCredit));
		dataGridFund->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQA_PreDeposit;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"上次存款额";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_PreDeposit));
		dataGridFund->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQA_PreBalance;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"上次总权益";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_PreBalance));
		dataGridFund->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQA_PreMargin;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"上次占用的保证金";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_PreMargin));
		dataGridFund->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQA_Reserve;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"基本准备金";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_Reserve));
		dataGridFund->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQA_PreExchMargin;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"上次交易所保证金";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_PreExchMargin));
		dataGridFund->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQA_ForceCloseStat;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"历史强平次数";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_ForceCloseStat));
		dataGridFund->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQA_DeliveryMargin;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"投资者交割保证金";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_DeliveryMargin));
		dataGridFund->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQA_ExchangeDeliveryMargin;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"交易所交割保证金";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_ExchangeDeliveryMargin));
		dataGridFund->Columns->Add(gridColumn);

	}
	void UpdateRow(int nIndex, RiskSyncAccountField& lastFund, SeqPreRiskAccountField& preFund, bool bOnlyLastFund) {
#if 1
		if(nIndex<0)
			return;
		int nCol = -1;
		std::string strText;
		DataGridViewRow^ dr = dataGridFund->Rows[nIndex];
		char strTemp[1024];

		if((nCol = GetColIndexByColID(conCol_RTQA_InvestorID))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(lastFund.InvestorID);
		}
		TrustTradeAccount traderAccount;
		memset(&traderAccount, 0, sizeof(traderAccount));
		if(m_pdataInfo->GetTrustTradeAccount(std::string(lastFund.InvestorID), traderAccount)) {
			AssetMgmtOrganization organ;
			memset(&organ, 0, sizeof(organ));
			if(m_pdataInfo->GetAssetMgmtOrg(traderAccount.nAssetMgmtOrgID, organ)) {
				if((nCol = GetColIndexByColID(conCol_RTQA_AssertMgmtOrgan))>=0) {
					dr->Cells[nCol]->Value = Tools::string2String(organ.szName);
				}
			}
			FinancialProduct finanProduct;
			memset(&finanProduct, 0, sizeof(finanProduct));
			if(m_pdataInfo->GetFinancialProduct(traderAccount.nFinancialProductID, finanProduct)) {
				if((nCol = GetColIndexByColID(conCol_RTQA_FinanProduct))>=0) {
					dr->Cells[nCol]->Value = Tools::string2String(finanProduct.szName);
				}
			}
		}
		if((nCol = GetColIndexByColID(conCol_RTQA_BrokerID))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(lastFund.BrokerID);
		}
		if((nCol = GetColIndexByColID(conCol_RTQA_Withdraw))>=0) {
			sprintf(strTemp, "%0.2f", lastFund.Withdraw);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQA_CurrMargin))>=0) {
			sprintf(strTemp, "%0.2f", lastFund.CurrMargin);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQA_Commission))>=0) {
			sprintf(strTemp, "%0.2f", lastFund.Commission);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQA_CloseProfit))>=0) {
			sprintf(strTemp, "%0.2f", lastFund.CloseProfit);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQA_PositionProfit))>=0) {
			sprintf(strTemp, "%0.2f", lastFund.PositionProfit);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQA_Balance))>=0) {
			sprintf(strTemp, "%0.2f", lastFund.Balance);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQA_Available))>=0) {
			sprintf(strTemp, "%0.2f", lastFund.Available);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQA_Mortgage))>=0) {
			sprintf(strTemp, "%0.2f", lastFund.Mortgage);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQA_ExchangeMargin))>=0) {
			sprintf(strTemp, "%0.2f", lastFund.ExchangeMargin);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQA_WithdrawQuota))>=0) {
			sprintf(strTemp, "%0.2f", lastFund.WithdrawQuota);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQA_Credit))>=0) {
			sprintf(strTemp, "%0.2f", lastFund.Credit);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQA_FrozenMargin))>=0) {
			sprintf(strTemp, "%0.2f", lastFund.FrozenMargin);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQA_FrozenCommission))>=0) {
			sprintf(strTemp, "%0.2f", lastFund.FrozenCommission);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}

		if(!bOnlyLastFund) {
			if((nCol = GetColIndexByColID(conCol_RTQA_AccountID))>=0) {
				dr->Cells[nCol]->Value = Tools::string2String(preFund.AccountID);
			}
			if((nCol = GetColIndexByColID(conCol_RTQA_PreMortgage))>=0) {
				sprintf(strTemp, "%0.2f", preFund.PreMortgage);
				dr->Cells[nCol]->Value = Tools::string2String(strTemp);
			}
			if((nCol = GetColIndexByColID(conCol_RTQA_PreCredit))>=0) {
				sprintf(strTemp, "%0.2f", preFund.PreCredit);
				dr->Cells[nCol]->Value = Tools::string2String(strTemp);
			}
			if((nCol = GetColIndexByColID(conCol_RTQA_PreBalance))>=0) {
				sprintf(strTemp, "%0.2f", preFund.PreBalance);
				dr->Cells[nCol]->Value = Tools::string2String(strTemp);
			}
			if((nCol = GetColIndexByColID(conCol_RTQA_PreDeposit))>=0) {
				sprintf(strTemp, "%0.2f", preFund.PreDeposit);
				dr->Cells[nCol]->Value = Tools::string2String(strTemp);
			}
			if((nCol = GetColIndexByColID(conCol_RTQA_PreMargin))>=0) {
				sprintf(strTemp, "%0.2f", preFund.PreMargin);
				dr->Cells[nCol]->Value = Tools::string2String(strTemp);
			}
			if((nCol = GetColIndexByColID(conCol_RTQA_Reserve))>=0) {
				sprintf(strTemp, "%0.2f", preFund.Reserve);
				dr->Cells[nCol]->Value = Tools::string2String(strTemp);
			}
			if((nCol = GetColIndexByColID(conCol_RTQA_PreExchMargin))>=0) {
				sprintf(strTemp, "%0.2f", preFund.PreExchMargin);
				dr->Cells[nCol]->Value = Tools::string2String(strTemp);
			}
			if((nCol = GetColIndexByColID(conCol_RTQA_ForceCloseStat))>=0) {
				sprintf(strTemp, "%0d", preFund.ForceCloseStat);
				dr->Cells[nCol]->Value = Tools::string2String(strTemp);
			}
			if((nCol = GetColIndexByColID(conCol_RTQA_DeliveryMargin))>=0) {
				sprintf(strTemp, "%0.2f", preFund.DeliveryMargin);
				dr->Cells[nCol]->Value = Tools::string2String(strTemp);
			}
			if((nCol = GetColIndexByColID(conCol_RTQA_ExchangeDeliveryMargin))>=0) {
				sprintf(strTemp, "%0.2f", preFund.ExchangeDeliveryMargin);
				dr->Cells[nCol]->Value = Tools::string2String(strTemp);
			}
		}
#endif
	}
	void ShowQueue(std::queue<RiskAllAccountField>& queueRet) {
		std::map<string, int>::iterator it;
		int nIndex = -1;
		char strTemp[1024];
		std::string strKey;
		strKey.reserve(1024);
		save->LockObject();
		while(!queueRet.empty()) {
			RiskAllAccountField field = queueRet.front();
			queueRet.pop();

			memset(strTemp, 0, sizeof(strTemp));
			sprintf(strTemp, "%s", field.cur.InvestorID);
			strKey = strTemp;
			it = save->mapKey2Index.find(strKey);
			if(it != save->mapKey2Index.end()) {
				nIndex = GetRowIndexByRowID(it->second);
			}
			else {
				DataGridViewRow^ newRow = gcnew DataGridViewRow;
				int nRowID = save->GetNextRowID();
				newRow->Tag = nRowID;
				nIndex = dataGridFund->Rows->Add(newRow);
				if(nIndex>=0) {
					save->mapKey2Index.insert(std::make_pair(strKey, nRowID));
					save->mapIndex2Key.insert(std::make_pair(nRowID, strKey));
				}
			}
			if(nIndex>=0) {
				SeqPreRiskAccountField preFund;
				memset(&preFund, 0, sizeof(preFund));
				UpdateRow(nIndex, field.cur, field.pre, false);
			}
		}
		save->UnlockObject();
	}
	void ShowQueue(std::queue<RiskSyncAccountField>& queueRet) {
		std::map<string, int>::iterator it;
		int nIndex = -1;
		char strTemp[1024];
		std::string strKey;
		strKey.reserve(1024);
		save->LockObject();
		while(!queueRet.empty()) {
			RiskSyncAccountField field = queueRet.front();
			queueRet.pop();

			memset(strTemp, 0, sizeof(strTemp));
			sprintf(strTemp, "%s", field.InvestorID);
			strKey = strTemp;
			it = save->mapKey2Index.find(strKey);
			if(it != save->mapKey2Index.end()) {
				nIndex = GetRowIndexByRowID(it->second);
			}
			else {
				DataGridViewRow^ newRow = gcnew DataGridViewRow;
				int nRowID = save->GetNextRowID();
				newRow->Tag = nRowID;
				nIndex = dataGridFund->Rows->Add(newRow);
				if(nIndex>=0) {
					save->mapKey2Index.insert(std::make_pair(strKey, nRowID));
					save->mapIndex2Key.insert(std::make_pair(nRowID, strKey));
				}
			}
			if(nIndex>=0) {
				SeqPreRiskAccountField preFund;
				memset(&preFund, 0, sizeof(preFund));
				UpdateRow(nIndex, field, preFund, true);
			}
		}
		save->UnlockObject();
	}

	private: System::Void CDlgRealTimeFundInfo_Load(System::Object^  sender, System::EventArgs^  e) {
		InitGridHead();

		dlgFinancialProductIDList = gcnew CFinancialProductIDListDlg();
		dlgFinancialProductIDList->ResetRelateCombo(comboFinaProduct);
		comboFinaProduct->ChildControl = dlgFinancialProductIDList;

		dlgMgmtOrganIDTreeList = gcnew CAssetMgmtOrganIDTreeListDlg();
		dlgMgmtOrganIDTreeList->ResetFinaProductDlg(dlgFinancialProductIDList);
		dlgMgmtOrganIDTreeList->ResetRelateCombo(comboOrganization);
		comboOrganization->ChildControl = dlgMgmtOrganIDTreeList;

		dlgOrganAndAccountIDTreeList = gcnew COrganAndAccountIDTreeListDlg();
		dlgOrganAndAccountIDTreeList->ResetRelateCombo(comboAccount);
		comboAccount->ChildControl = dlgOrganAndAccountIDTreeList;

		IntPtr hWnd=this->Handle;
		CTcpLayer::SubscribePkg(Cmd_RM_SubscribeFund_Rsp, (int)hWnd);
		CTcpLayer::SubscribePkg(Cmd_RM_Fund_Push, (int)hWnd);
		CTcpLayer::SubscribePkg(Cmd_RM_UnSubscribeFund_Rsp, (int)hWnd);

		ResetComboBoxTextReadOnly(comboFinaProduct);
		ResetComboBoxTextReadOnly(comboOrganization);
		ResetComboBoxTextReadOnly(comboAccount);

		 comboOrganization->Enabled = false;
		 comboFinaProduct->Enabled = false;
		 comboAccount->Enabled = true;

#ifndef _DEBUG
		 if(m_pdataInfo!=NULL)
			 btnQuery->Enabled = m_pdataInfo->HasPermission(TODAY_ACCOUNT_QUERY);
#endif
	}
private: System::Void CDlgRealTimeFundInfo_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
			if(!m_bHasExit) {
				Hide();
				e->Cancel = true;
			}
			save->ResetQuery();
			dataGridFund->Rows->Clear();
		}
private: System::Void CDlgRealTimeFundInfo_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
			 IntPtr hWnd=this->Handle;
			 CTcpLayer::UnsubscribeAllPkg((int)hWnd);
		}
private: System::Void btnQuery_Click(System::Object^  sender, System::EventArgs^  e) {

			 if(!m_bHasChanged_QueryParam)
				 return;

			 bool bHasData = false;
			 dataGridFund->Rows->Clear();

			 save->ResetQuery();

			 std::set<int> setAccountID;
			 // 然后根据查询条件获得需要订阅的AccountID集合
			 if(radioGroupQuery->Checked) {

				 bool bNoChecked = true;
				 std::set<int> setOrganID;
				 std::set<int> setFinanProdID;
				 dlgMgmtOrganIDTreeList->GetSelectID(setOrganID, bNoChecked);
				 //if(!bNoChecked) {
					 dlgFinancialProductIDList->GetSelectID(setFinanProdID);
				 //}

				 std::set<int> setTemp;
				 std::set<int> setOrganAccntID;

				 std::set<int>::iterator it;
				 std::set<int>::iterator itTmp;
				 for(it = setOrganID.begin(); it != setOrganID.end(); it++) {
					 setTemp.clear();
					 m_pdataInfo->GetTrustTradeAccountByOrgID(*it, bNoChecked, setTemp);
					 for(itTmp = setTemp.begin(); itTmp != setTemp.end(); itTmp++) {
						 setOrganAccntID.insert(*itTmp);
					 }
				 }

				 std::set<int> setProdAccntID;

				 for(it = setFinanProdID.begin(); it != setFinanProdID.end(); it++) {
					 setTemp.clear();
					 m_pdataInfo->GetTrustTradeAccountByFinancialID(*it, setTemp);
					 for(itTmp = setTemp.begin(); itTmp != setTemp.end(); itTmp++) {
						 setProdAccntID.insert(*itTmp);
					 }
				 }

				 int nMax = max(setOrganAccntID.size(), setProdAccntID.size());
				 int* pTempID = new int[nMax];

				 std::set<int> setResult;
				 int* pEnd = std::set_intersection(
						setOrganAccntID.begin(), setOrganAccntID.end(), 
						setProdAccntID.begin(), setProdAccntID.end(), pTempID);
				 int* nPos = pTempID;
				 while(nPos < pEnd) {
					 setResult.insert(*nPos);
					 nPos++;
				 }
				 delete pTempID;

				 save->LockObject();
				 // 将查询集合保存起来
				 save->setQueryAccountID = setResult;
				 bHasData = setResult.size()>0;

				 save->bIsGroupQuery = true;
				 save->UnlockObject();
			 }
			 else {
				 // 将查询集合保存起来
				 save->LockObject();
				 dlgOrganAndAccountIDTreeList->GetSelectID(save->setQueryAccountID);
				 bHasData = save->setQueryAccountID.size()>0;
				 save->bIsGroupQuery = false;
				 save->UnlockObject();
			 }

			 dwTcpStartTime = GetTickCount();
			 dwTcpEndTime = 0;
			 dwShowStartTime = 0;
			 dwShowEndTime = 0;

			 if(bHasData)
				 save->NewQuery();
			 else
                 MessageBox::Show("所设查询条件未筛选出任何帐号。", 
			 				"提示", MessageBoxButtons::OK, MessageBoxIcon::Warning);

			 //m_bHasChanged_QueryParam = false;

		 }
private: System::Void btnResetZero_Click(System::Object^  sender, System::EventArgs^  e) {
			 m_bHasChanged_QueryParam = true;
			 //dlgMgmtOrganIDTreeList->Clear();
			 //dlgFinancialProductIDList->Clear();
			 //dlgOrganAndAccountIDTreeList->Clear();
			 if(!m_bHasChanged_QueryParam)
				 return;

			 dataGridFund->Rows->Clear();
			 save->ResetQuery();
		 }
private: System::Void radioGroupQuery_Click(System::Object^  sender, System::EventArgs^  e) {
			 comboOrganization->Enabled = true;
			 comboFinaProduct->Enabled = true;
			 comboAccount->Enabled = false;
		 }
private: System::Void radioAccountQuery_Click(System::Object^  sender, System::EventArgs^  e) {
			 comboOrganization->Enabled = false;
			 comboFinaProduct->Enabled = false;
			 comboAccount->Enabled = true;
		 }
private: System::Void CDlgRealTimeFundInfo_ResizeEnd(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void CDlgRealTimeFundInfo_Resize(System::Object^  sender, System::EventArgs^  e) {
			 Rectangle rect = this->ClientRectangle;
			 Rectangle rectGrid = dataGridFund->DisplayRectangle;
			 System::Drawing::Size size(rect.Width-dataGridFund->Left-10, rect.Height-dataGridFund->Top-10);
			 dataGridFund->Size = size;
		 }
};
}
