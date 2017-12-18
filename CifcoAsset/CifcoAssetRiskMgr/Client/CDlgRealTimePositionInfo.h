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
#include "CFieldDetail.h"
#include "TcpLayer.h"
#include "CDataInfo.h"
#include "RiskManageCmd.h"
#include "UserApiStruct.h"
#include "CommonPkg.h"
#include "RiskEvt.h"
#include "CDlgRealTimePositionInfoData.h"
#include "CInstrumentIDTreeListDlg.h"
#include "CFinancialProductIDListDlg.h"
#include "CAssetMgmtOrganIDTreeListDlg.h"
#include "COrganAndAccountIDTreeListDlg.h"

#include "ForceShortcutForm.h"

namespace Client {

	/// <summary>
	/// Summary for CDlgRealTimePositionInfo
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class CDlgRealTimePositionInfo : public System::Windows::Forms::Form
	{
	public:
		CDlgRealTimePositionInfo(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			save = NULL;
			m_pdataInfo = CDataInfo::NewInstance();
			m_bHasExit = false;
			m_bHasChanged_QueryParam = true;
			save = CDlgRealTimePositionInfoData::NewInstance();
		}

		void SetRiskEvtModule(RiskEvt^ riskModule) {
			RiskEvtModule = riskModule;
		}

		void ReleaseForm() {
			m_bHasExit = true;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~CDlgRealTimePositionInfo()
		{
			if (components)
			{
				delete components;
			}
			CDlgRealTimePositionInfoData::ReleaseInstance(save);
			save = NULL;
		}

	private: CDataInfo* m_pdataInfo;
	private: CDlgRealTimePositionInfoData* save;
	private: bool m_bHasExit;
	private: bool m_bHasChanged_QueryParam;
	private: RiskEvt^  RiskEvtModule;
	private: unsigned long dwTcpStartTime;
	private: unsigned long dwTcpEndTime;
	private: unsigned long dwShowStartTime;
	private: unsigned long dwShowEndTime;

	private: CAssetMgmtOrganIDTreeListDlg^ dlgMgmtOrganIDTreeList;
	private: CFinancialProductIDListDlg^ dlgFinancialProductIDList;
	private: CInstrumentIDTreeListDlg^ dlgInstrumentIDTreeList;
	private: COrganAndAccountIDTreeListDlg^ dlgOrganAndAccountIDTreeList;

	private: System::Windows::Forms::RadioButton^  radioGroupQuery;
	private: System::Windows::Forms::RadioButton^  radioAccountQuery;

	private: System::Windows::Forms::DataGridView^  dataGridPosition;

	private: System::Windows::Forms::Button^  btnResetZero;
	private: System::Windows::Forms::Button^  btnQuery;
	private: AnywndComboBox::AnywndComboBoxControl^  comboInstrument;
	private: AnywndComboBox::AnywndComboBoxControl^  comboFinaProduct;
	private: AnywndComboBox::AnywndComboBoxControl^  comboAccount;
	private: AnywndComboBox::AnywndComboBoxControl^  comboOrganization;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::CheckBox^  checkShowZero;
	private: System::Windows::Forms::ContextMenuStrip^  contextMenuDataGrid;
	private: System::Windows::Forms::ToolStripMenuItem^  MenuItem_FastForceClose;

	private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  MenuItem_SetColumns;

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
			this->radioGroupQuery = (gcnew System::Windows::Forms::RadioButton());
			this->dataGridPosition = (gcnew System::Windows::Forms::DataGridView());
			this->contextMenuDataGrid = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->MenuItem_FastForceClose = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->MenuItem_SetColumns = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->btnResetZero = (gcnew System::Windows::Forms::Button());
			this->btnQuery = (gcnew System::Windows::Forms::Button());
			this->comboOrganization = (gcnew AnywndComboBox::AnywndComboBoxControl());
			this->comboFinaProduct = (gcnew AnywndComboBox::AnywndComboBoxControl());
			this->comboInstrument = (gcnew AnywndComboBox::AnywndComboBoxControl());
			this->comboAccount = (gcnew AnywndComboBox::AnywndComboBoxControl());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->radioAccountQuery = (gcnew System::Windows::Forms::RadioButton());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->checkShowZero = (gcnew System::Windows::Forms::CheckBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridPosition))->BeginInit();
			this->contextMenuDataGrid->SuspendLayout();
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
			this->radioGroupQuery->Click += gcnew System::EventHandler(this, &CDlgRealTimePositionInfo::radioGroupQuery_Click);
			// 
			// dataGridPosition
			// 
			this->dataGridPosition->AllowUserToAddRows = false;
			this->dataGridPosition->AllowUserToDeleteRows = false;
			this->dataGridPosition->AllowUserToResizeRows = false;
			this->dataGridPosition->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridPosition->ContextMenuStrip = this->contextMenuDataGrid;
			this->dataGridPosition->EditMode = System::Windows::Forms::DataGridViewEditMode::EditProgrammatically;
			this->dataGridPosition->Location = System::Drawing::Point(9, 96);
			this->dataGridPosition->Margin = System::Windows::Forms::Padding(1);
			this->dataGridPosition->MultiSelect = false;
			this->dataGridPosition->Name = L"dataGridPosition";
			this->dataGridPosition->ReadOnly = true;
			this->dataGridPosition->RowHeadersVisible = false;
			this->dataGridPosition->RowTemplate->Height = 23;
			this->dataGridPosition->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dataGridPosition->Size = System::Drawing::Size(613, 307);
			this->dataGridPosition->TabIndex = 17;
			// 
			// contextMenuDataGrid
			// 
			this->contextMenuDataGrid->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->MenuItem_FastForceClose, 
				this->toolStripMenuItem1, this->MenuItem_SetColumns});
			this->contextMenuDataGrid->Name = L"contextMenuDataGrid";
			this->contextMenuDataGrid->Size = System::Drawing::Size(119, 54);
			// 
			// MenuItem_FastForceClose
			// 
			this->MenuItem_FastForceClose->Name = L"MenuItem_FastForceClose";
			this->MenuItem_FastForceClose->Size = System::Drawing::Size(118, 22);
			this->MenuItem_FastForceClose->Text = L"快捷强平";
			this->MenuItem_FastForceClose->Click += gcnew System::EventHandler(this, &CDlgRealTimePositionInfo::MenuItem_FastForceClose_Click);
			// 
			// toolStripMenuItem1
			// 
			this->toolStripMenuItem1->Name = L"toolStripMenuItem1";
			this->toolStripMenuItem1->Size = System::Drawing::Size(115, 6);
			// 
			// MenuItem_SetColumns
			// 
			this->MenuItem_SetColumns->Name = L"MenuItem_SetColumns";
			this->MenuItem_SetColumns->Size = System::Drawing::Size(118, 22);
			this->MenuItem_SetColumns->Text = L"设置列";
			// 
			// btnResetZero
			// 
			this->btnResetZero->Location = System::Drawing::Point(129, 62);
			this->btnResetZero->Name = L"btnResetZero";
			this->btnResetZero->Size = System::Drawing::Size(83, 24);
			this->btnResetZero->TabIndex = 15;
			this->btnResetZero->Text = L"清空";
			this->btnResetZero->UseVisualStyleBackColor = true;
			this->btnResetZero->Click += gcnew System::EventHandler(this, &CDlgRealTimePositionInfo::btnResetZero_Click);
			// 
			// btnQuery
			// 
			this->btnQuery->Location = System::Drawing::Point(26, 62);
			this->btnQuery->Name = L"btnQuery";
			this->btnQuery->Size = System::Drawing::Size(83, 24);
			this->btnQuery->TabIndex = 16;
			this->btnQuery->Text = L"查询";
			this->btnQuery->UseVisualStyleBackColor = true;
			this->btnQuery->Click += gcnew System::EventHandler(this, &CDlgRealTimePositionInfo::btnQuery_Click);
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
			// comboInstrument
			// 
			this->comboInstrument->BackColor = System::Drawing::Color::White;
			this->comboInstrument->ChildControl = nullptr;
			this->comboInstrument->DropDownHeight = 1;
			this->comboInstrument->DropDownWidth = 1;
			this->comboInstrument->FormattingEnabled = true;
			this->comboInstrument->IntegralHeight = false;
			this->comboInstrument->Location = System::Drawing::Point(517, 8);
			this->comboInstrument->MaxDropDownItems = 1;
			this->comboInstrument->Name = L"comboInstrument";
			this->comboInstrument->Size = System::Drawing::Size(100, 20);
			this->comboInstrument->TabIndex = 14;
			// 
			// comboAccount
			// 
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
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(454, 14);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(59, 12);
			this->label3->TabIndex = 10;
			this->label3->Text = L"品种/合约";
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
			this->radioAccountQuery->Click += gcnew System::EventHandler(this, &CDlgRealTimePositionInfo::radioAccountQuery_Click);
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
			// checkShowZero
			// 
			this->checkShowZero->AutoSize = true;
			this->checkShowZero->Location = System::Drawing::Point(234, 67);
			this->checkShowZero->Name = L"checkShowZero";
			this->checkShowZero->Size = System::Drawing::Size(96, 16);
			this->checkShowZero->TabIndex = 18;
			this->checkShowZero->Text = L"0 仓位仍显示";
			this->checkShowZero->UseVisualStyleBackColor = true;
			this->checkShowZero->Visible = false;
			// 
			// CDlgRealTimePositionInfo
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(632, 413);
			this->Controls->Add(this->checkShowZero);
			this->Controls->Add(this->radioGroupQuery);
			this->Controls->Add(this->dataGridPosition);
			this->Controls->Add(this->btnResetZero);
			this->Controls->Add(this->btnQuery);
			this->Controls->Add(this->comboInstrument);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->comboFinaProduct);
			this->Controls->Add(this->comboAccount);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->comboOrganization);
			this->Controls->Add(this->radioAccountQuery);
			this->Controls->Add(this->label1);
			this->Name = L"CDlgRealTimePositionInfo";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"实时持仓";
			this->Load += gcnew System::EventHandler(this, &CDlgRealTimePositionInfo::CDlgRealTimePositionInfo_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &CDlgRealTimePositionInfo::CDlgRealTimePositionInfo_FormClosed);
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &CDlgRealTimePositionInfo::CDlgRealTimePositionInfo_FormClosing);
			this->Resize += gcnew System::EventHandler(this, &CDlgRealTimePositionInfo::CDlgRealTimePositionInfo_Resize);
			this->ResizeEnd += gcnew System::EventHandler(this, &CDlgRealTimePositionInfo::CDlgRealTimePositionInfo_ResizeEnd);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridPosition))->EndInit();
			this->contextMenuDataGrid->ResumeLayout(false);
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
			case Cmd_RM_SubscribePosition_Rsp:
			{
				dwTcpEndTime = GetTickCount();

				if(head.userdata1 == OPERATOR_SUCCESS) {
					SequencialPositionField* pPosiInfo = NULL;
					int nItemCount = head.len/sizeof(SequencialPositionField);

					save->LockObject();
					for(int i=0; i<nItemCount; i++) {
						pPosiInfo = (SequencialPositionField*)
								(p+sizeof(Stru_UniPkgHead)+i*sizeof(SequencialPositionField));
						save->queuePositionInfo.push(*pPosiInfo);
					}
					save->UnlockObject();
					if(head.userdata2 == head.userdata3 - 1) {
						dwShowStartTime = GetTickCount();

						std::queue<SequencialPositionField> queueRet;
						save->CheckNewData(m_pdataInfo, queueRet);
						ShowQueue(queueRet);
		
						dwShowEndTime = GetTickCount();
						{
							char csText[1024] = {0};
							sprintf(csText, "实时持仓查询，本次查询时间消耗 传输==%u, 显示==%u",
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
			case Cmd_RM_UnSubscribePosition_Rsp:
				if(head.userdata1 == OPERATOR_SUCCESS) {
				}
				else {
					p[sizeof(head)+head.len-1]=0;
					//TRACE(p+sizeof(head));
				}
				break;
			case Cmd_RM_Position_Push:
			{
				if(head.userdata1 == OPERATOR_SUCCESS) {
					SequencialPositionField* pPosiInfo = NULL;
					int nItemCount = head.len/sizeof(SequencialPositionField);

					save->LockObject();
					for(int i=0; i<nItemCount; i++) {
						pPosiInfo = (SequencialPositionField*)
								(p+sizeof(Stru_UniPkgHead)+i*sizeof(SequencialPositionField));
						save->queuePositionInfo.push(*pPosiInfo);
					}
					save->UnlockObject();
					std::queue<SequencialPositionField> queueRet;
					save->CheckNewData(m_pdataInfo, queueRet);
					ShowQueue(queueRet);
				}
			}
				break;
			case Cmd_RM_SubscribeQuot_Rsp:
			{
				if(head.userdata1 == OPERATOR_SUCCESS) {
					DepthMarketDataField* pQuot = NULL;
					int nItemCount = head.len/sizeof(DepthMarketDataField);

					save->LockObject();
					for(int i=0; i<nItemCount; i++) {
						pQuot = (DepthMarketDataField*)
								(p+sizeof(Stru_UniPkgHead)+i*sizeof(DepthMarketDataField));
						save->queueQuot.push(*pQuot);
					}
					save->UnlockObject();
					std::queue<SequencialPositionField> queueRet;
					save->CheckNewData_Quot(m_pdataInfo, queueRet);
					ShowQueue(queueRet);
				}
				else {
					p[sizeof(head)+head.len-1]=0;
					//TRACE(p+sizeof(head));
				}
			}
				break;
			case Cmd_RM_UnSubscribeQuot_Req:
				if(head.userdata1 == OPERATOR_SUCCESS) {
				}
				else {
					p[sizeof(head)+head.len-1]=0;
					//TRACE(p+sizeof(head));
				}
				break;
			case Cmd_RM_Quot_Push:
			{
				if(head.userdata1 == OPERATOR_SUCCESS) {
					DepthMarketDataField* pQuot = NULL;
					int nItemCount = head.len/sizeof(DepthMarketDataField);

					save->LockObject();
					for(int i=0; i<nItemCount; i++) {
						pQuot = (DepthMarketDataField*)
								(p+sizeof(Stru_UniPkgHead)+i*sizeof(DepthMarketDataField));
						save->queueQuot.push(*pQuot);
					}
					save->UnlockObject();
					std::queue<SequencialPositionField> queueRet;
					save->CheckNewData_Quot(m_pdataInfo, queueRet);
					ShowQueue(queueRet);
				}
			}
				break;
			};
		}

		Form::WndProc(m);
	}
	int GetRowIndexByRowID(int nRowID) {
		for(int i=0; i<dataGridPosition->Rows->Count; i++) {
			if(int::Parse(dataGridPosition->Rows[i]->Tag->ToString()) == nRowID)
				return dataGridPosition->Rows[i]->Index;
		}
		return -1;
	}
	int GetColIndexByColID(int nColID) {
		for(int i=0; i<dataGridPosition->Columns->Count; i++) {
			if(int::Parse(dataGridPosition->Columns[i]->Tag->ToString()) == nColID)
				return dataGridPosition->Columns[i]->Index;
		}
		return -1;
	}
	void InitGridHead() {

		dataGridPosition->Columns->Clear();

		//System.Windows.Forms.DataGridViewColumn^ gridColumn
		//		gcnew System.Windows.Forms.DataGridViewColumn;
		int nColID = 0;

		System::Windows::Forms::DataGridViewTextBoxColumn^ gridColumn = 
				(gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_InvestorID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"投资者代码";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_InvestorID));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_InstrumentID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"合约代码";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_InstrumentID));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_PosiDirection;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"持仓多空方向";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_PosiDirection));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_Position;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"今日持仓";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_Position));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_PositionProfit;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"持仓盈亏";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_PositionProfit));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_UseMargin;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"占用的保证金";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_UseMargin));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_PositionCost;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"持仓成本";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_PositionCost));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_AssertMgmtOrgan;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"账号所属资管";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_AssertMgmtOrgan));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_FinanProduct;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"账号所属理财产品";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_FinanProduct));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_BrokerID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"经纪公司代码";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_BrokerID));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_HedgeFlag;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"投机套保标志";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_HedgeFlag));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_PositionDate;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"持仓日期";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_PositionDate));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_YdPosition;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"上日持仓";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_YdPosition));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_LongFrozen;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"多头冻结";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_LongFrozen));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_ShortFrozen;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"空头冻结";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_ShortFrozen));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_LongFrozenAmount;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"开仓冻结金额";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_LongFrozenAmount));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_ShortFrozenAmount;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"开仓冻结金额";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_ShortFrozenAmount));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_OpenVolume;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"开仓量";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_OpenVolume));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_CloseVolume;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"平仓量";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_CloseVolume));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_OpenAmount;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"开仓金额";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_OpenAmount));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_CloseAmount;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"平仓金额";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_CloseAmount));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_PreMargin;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"上次占用的保证金";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_PreMargin));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_FrozenMargin;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"冻结的保证金";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_FrozenMargin));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_FrozenCash;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"冻结的资金";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_FrozenCash));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_FrozenCommission;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"冻结的手续费";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_FrozenCommission));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_CashIn;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"资金差额";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_CashIn));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_Commission;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"手续费";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_Commission));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_CloseProfit;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"平仓盈亏";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_CloseProfit));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_PreSettlementPrice;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"上次结算价";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_PreSettlementPrice));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_SettlementPrice;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"本次结算价";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_SettlementPrice));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_TradingDay;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"交易日";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_TradingDay));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_SettlementID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"结算编号";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_SettlementID));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_OpenCost;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"开仓成本";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_OpenCost));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_ExchangeMargin;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"交易所保证金";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_ExchangeMargin));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_CombPosition;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"组合成交形成的持仓";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_CombPosition));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_CombLongFrozen;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"组合多头冻结";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_CombLongFrozen));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_CombShortFrozen;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"组合空头冻结";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_CombShortFrozen));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_CloseProfitByDate;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"逐日盯市平仓盈亏";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_CloseProfitByDate));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_CloseProfitByTrade;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"逐笔对冲平仓盈亏";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_CloseProfitByTrade));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_TodayPosition;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"今日持仓";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_TodayPosition));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_MarginRateByMoney;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"保证金率";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_MarginRateByMoney));
		dataGridPosition->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQP_MarginRateByVolume;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"保证金率(按手数)";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQP_MarginRateByVolume));
		dataGridPosition->Columns->Add(gridColumn);
	}
	void UpdateRow(int nIndex, SequencialPositionField& positionInfo) {
		if(nIndex<0)
			return;
		int nCol = -1;
		std::string strText;
		DataGridViewRow^ dr = dataGridPosition->Rows[nIndex];
		char strTemp[1024];
		if((nCol = GetColIndexByColID(conCol_RTQA_InvestorID))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(positionInfo.InvestorID);
		}
		TrustTradeAccount traderAccount;
		memset(&traderAccount, 0, sizeof(traderAccount));
		if(m_pdataInfo->GetTrustTradeAccount(std::string(positionInfo.InvestorID), traderAccount)) {
			AssetMgmtOrganization organ;
			memset(&organ, 0, sizeof(organ));
			if(m_pdataInfo->GetAssetMgmtOrg(traderAccount.nAssetMgmtOrgID, organ)) {
				if((nCol = GetColIndexByColID(conCol_RTQP_AssertMgmtOrgan))>=0) {
					dr->Cells[nCol]->Value = Tools::string2String(organ.szName);
				}
			}
			FinancialProduct finanProduct;
			memset(&finanProduct, 0, sizeof(finanProduct));
			if(m_pdataInfo->GetFinancialProduct(traderAccount.nFinancialProductID, finanProduct)) {
				if((nCol = GetColIndexByColID(conCol_RTQP_FinanProduct))>=0) {
					dr->Cells[nCol]->Value = Tools::string2String(finanProduct.szName);
				}
			}
		}
		if((nCol = GetColIndexByColID(conCol_RTQP_BrokerID))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(positionInfo.BrokerID);
		}
		if((nCol = GetColIndexByColID(conCol_RTQP_InstrumentID))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(positionInfo.InstrumentID);
		}
		if((nCol = GetColIndexByColID(conCol_RTQP_PosiDirection))>=0) {
			strText = CFieldDetail::PosiDirection2String(positionInfo.PosiDirection);
			dr->Cells[nCol]->Value = Tools::string2String(strText.c_str());
		}
		if((nCol = GetColIndexByColID(conCol_RTQP_HedgeFlag))>=0) {
			strText = CFieldDetail::HedgeFlag2String(positionInfo.HedgeFlag);
			dr->Cells[nCol]->Value = Tools::string2String(strText.c_str());
		}
		if((nCol = GetColIndexByColID(conCol_RTQP_PositionDate))>=0) {
			strText = positionInfo.PositionDate;
			dr->Cells[nCol]->Value = Tools::string2String(strText.c_str());
		}
		if((nCol = GetColIndexByColID(conCol_RTQP_YdPosition))>=0) {
			sprintf(strTemp, "%d", positionInfo.YdPosition);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQP_Position))>=0) {
			sprintf(strTemp, "%d", positionInfo.Position);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQP_LongFrozen))>=0) {
			sprintf(strTemp, "%d", positionInfo.LongFrozen);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQP_ShortFrozen))>=0) {
			sprintf(strTemp, "%d", positionInfo.ShortFrozen);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQP_LongFrozenAmount))>=0) {
			sprintf(strTemp, "%0.2f", positionInfo.LongFrozenAmount);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQP_ShortFrozenAmount))>=0) {
			sprintf(strTemp, "%0.2f", positionInfo.ShortFrozenAmount);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQP_OpenVolume))>=0) {
			sprintf(strTemp, "%d", positionInfo.OpenVolume);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQP_CloseVolume))>=0) {
			sprintf(strTemp, "%d", positionInfo.CloseVolume);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQP_OpenAmount))>=0) {
			sprintf(strTemp, "%0.2f", positionInfo.OpenAmount);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQP_CloseAmount))>=0) {
			sprintf(strTemp, "%0.2f", positionInfo.CloseAmount);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQP_PositionCost))>=0) {
			sprintf(strTemp, "%0.2f", positionInfo.PositionCost);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQP_PreMargin))>=0) {
			sprintf(strTemp, "%0.2f", positionInfo.PreMargin);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQP_UseMargin))>=0) {
			sprintf(strTemp, "%0.2f", positionInfo.UseMargin);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQP_FrozenMargin))>=0) {
			sprintf(strTemp, "%0.2f", positionInfo.FrozenMargin);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQP_FrozenCash))>=0) {
			sprintf(strTemp, "%0.2f", positionInfo.FrozenCash);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQP_FrozenCommission))>=0) {
			sprintf(strTemp, "%0.2f", positionInfo.FrozenCommission);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQP_CashIn))>=0) {
			sprintf(strTemp, "%0.2f", positionInfo.CashIn);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQP_Commission))>=0) {
			sprintf(strTemp, "%0.2f", positionInfo.Commission);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQP_CloseProfit))>=0) {
			sprintf(strTemp, "%0.2f", positionInfo.CloseProfit);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQP_PositionProfit))>=0) {
			sprintf(strTemp, "%0.2f", positionInfo.PositionProfit);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQP_PreSettlementPrice))>=0) {
			sprintf(strTemp, "%0.2f", positionInfo.PreSettlementPrice);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQP_SettlementPrice))>=0) {
			sprintf(strTemp, "%0.2f", positionInfo.SettlementPrice);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQP_TradingDay))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(positionInfo.TradingDay);
		}
		if((nCol = GetColIndexByColID(conCol_RTQP_SettlementID))>=0) {
			sprintf(strTemp, "%d", positionInfo.SettlementID);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQP_OpenCost))>=0) {
			sprintf(strTemp, "%0.2f", positionInfo.OpenCost);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQP_ExchangeMargin))>=0) {
			sprintf(strTemp, "%0.2f", positionInfo.ExchangeMargin);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQP_CombPosition))>=0) {
			sprintf(strTemp, "%d", positionInfo.CombPosition);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQP_CombLongFrozen))>=0) {
			sprintf(strTemp, "%d", positionInfo.CombLongFrozen);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQP_CombShortFrozen))>=0) {
			sprintf(strTemp, "%d", positionInfo.CombShortFrozen);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQP_CloseProfitByDate))>=0) {
			sprintf(strTemp, "%0.2f", positionInfo.CloseProfitByDate);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQP_CloseProfitByTrade))>=0) {
			sprintf(strTemp, "%0.2f", positionInfo.CloseProfitByTrade);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQP_TodayPosition))>=0) {
			sprintf(strTemp, "%d", positionInfo.TodayPosition);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQP_MarginRateByMoney))>=0) {
			sprintf(strTemp, "%0.2f", positionInfo.MarginRateByMoney);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQP_MarginRateByVolume))>=0) {
			sprintf(strTemp, "%0.2f", positionInfo.MarginRateByVolume);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
	}
	bool DeleteRow(std::string& strKey) {
		std::map<string, int>::iterator it;
		int nRowID = -1;
		int nIndex = -1;

		save->LockObject();
		it = save->mapKey2Index.find(strKey);
		if(it == save->mapKey2Index.end()) {
			save->UnlockObject();
			return false;
		}
		
		nRowID = it->second;
		save->UnlockObject();
		nIndex = GetRowIndexByRowID(nRowID);
		if(nIndex<0)
			return false;

		dataGridPosition->Rows->RemoveAt(nIndex);
		save->LockObject();
		save->mapKey2Index.erase(it);
		save->mapIndex2Key.erase(nRowID);
		save->UnlockObject();
		return true;
	}
	void ShowQueue(std::queue<SequencialPositionField>& queueRet) {
		std::map<string, int>::iterator it;
		int nIndex = -1;
		char strTemp[1024];
		std::string strKey;
		strKey.reserve(1024);
		save->LockObject();
		while(!queueRet.empty()) {
			SequencialPositionField field = queueRet.front();
			queueRet.pop();

			memset(strTemp, 0, sizeof(strTemp));
			sprintf(strTemp, "%s, %s, %c, %c", 
					field.InvestorID, field.InstrumentID, field.PosiDirection, field.HedgeFlag);
			strKey = strTemp;
			it = save->mapKey2Index.find(strKey);
			if(it != save->mapKey2Index.end()) {
				nIndex = GetRowIndexByRowID(it->second);
			}
			else {
				DataGridViewRow^ newRow = gcnew DataGridViewRow;
				int nRowID = save->GetNextRowID();
				newRow->Tag = nRowID;
				nIndex = dataGridPosition->Rows->Add(newRow);
				if(nIndex>=0) {
					save->mapKey2Index.insert(std::make_pair(strKey, nRowID));
					save->mapIndex2Key.insert(std::make_pair(nRowID, strKey));
				}
			}
			if(nIndex>=0) {
				if(save->bShowZeroPosition) {
					UpdateRow(nIndex, field);
				}
				else {
					if(field.Position<=0) {
						DeleteRow(strKey);
					}
					else {
						UpdateRow(nIndex, field);
					}
				}
			}
		}
		save->UnlockObject();
	}

	private: System::Void CDlgRealTimePositionInfo_Load(System::Object^  sender, System::EventArgs^  e) {
		 InitGridHead();
			 dlgFinancialProductIDList = gcnew CFinancialProductIDListDlg();
			 dlgFinancialProductIDList->ResetRelateCombo(comboFinaProduct);
			 comboFinaProduct->ChildControl = dlgFinancialProductIDList;

			 dlgMgmtOrganIDTreeList = gcnew CAssetMgmtOrganIDTreeListDlg();
			 dlgMgmtOrganIDTreeList->ResetFinaProductDlg(dlgFinancialProductIDList);
			 dlgMgmtOrganIDTreeList->ResetRelateCombo(comboOrganization);
			 comboOrganization->ChildControl = dlgMgmtOrganIDTreeList;

			 dlgInstrumentIDTreeList = gcnew CInstrumentIDTreeListDlg();
			 dlgInstrumentIDTreeList->ResetRelateCombo(comboInstrument);
			 comboInstrument->ChildControl = dlgInstrumentIDTreeList;
			 
			 dlgOrganAndAccountIDTreeList = gcnew COrganAndAccountIDTreeListDlg();
			 dlgOrganAndAccountIDTreeList->ResetRelateCombo(comboAccount);
			 comboAccount->ChildControl = dlgOrganAndAccountIDTreeList;

			 IntPtr hWnd=this->Handle;
			 CTcpLayer::SubscribePkg(Cmd_RM_SubscribePosition_Rsp, (int)hWnd);
			 CTcpLayer::SubscribePkg(Cmd_RM_Position_Push, (int)hWnd);
			 CTcpLayer::SubscribePkg(Cmd_RM_UnSubscribePosition_Rsp, (int)hWnd);
			 CTcpLayer::SubscribePkg(Cmd_RM_SubscribeQuot_Rsp, (int)hWnd);
			 CTcpLayer::SubscribePkg(Cmd_RM_UnSubscribeQuot_Rsp, (int)hWnd);
			 CTcpLayer::SubscribePkg(Cmd_RM_Quot_Push, (int)hWnd);
			 
			 ResetComboBoxTextReadOnly(comboFinaProduct);
			 ResetComboBoxTextReadOnly(comboOrganization);
			 ResetComboBoxTextReadOnly(comboAccount);
			 ResetComboBoxTextReadOnly(comboInstrument);

			 comboOrganization->Enabled = false;
			 comboFinaProduct->Enabled = false;
			 comboInstrument->Enabled = false;
			 comboAccount->Enabled = true;

			 if(m_pdataInfo!=NULL) {
				 btnQuery->Enabled = m_pdataInfo->HasPermission(TODAY_POSITION_QUERY);
				 MenuItem_FastForceClose->Enabled = m_pdataInfo->HasPermission(FORCE_CLOSE_ORDER);
			 }

		 }
private: System::Void CDlgRealTimePositionInfo_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
			if(!m_bHasExit) {
				Hide();
				e->Cancel = true;
			}
			save->ResetQuery();
			dataGridPosition->Rows->Clear();
		 }
private: System::Void CDlgRealTimePositionInfo_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
			 IntPtr hWnd=this->Handle;
			 CTcpLayer::UnsubscribeAllPkg((int)hWnd);
		 }
private: System::Void btnQuery_Click(System::Object^  sender, System::EventArgs^  e) {

			 if(!m_bHasChanged_QueryParam)
				 return;

			 bool bHasData = false;
			 dataGridPosition->Rows->Clear();

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

				 // 获得最新的过滤合约集合
				 std::set<string> setFilterInstrumentID;
				 dlgInstrumentIDTreeList->GetSelectID(setFilterInstrumentID);
				 save->setFilterInstrumentID = setFilterInstrumentID;

				 save->bIsGroupQuery = true;
				 save->bShowZeroPosition = checkShowZero->Checked;
				 save->UnlockObject();
			 }
			 else {
				 // 将查询集合保存起来
				 save->LockObject();
				 dlgOrganAndAccountIDTreeList->GetSelectID(save->setQueryAccountID);
				 bHasData = save->setQueryAccountID.size()>0;
				 save->bIsGroupQuery = false;
				 save->bShowZeroPosition = checkShowZero->Checked;
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
			 //dlgInstrumentIDTreeList->Clear();
			 //dlgOrganAndAccountIDTreeList->Clear();

			 dataGridPosition->Rows->Clear();

			 save->ResetQuery();
		 }
private: System::Void radioGroupQuery_Click(System::Object^  sender, System::EventArgs^  e) {
			 comboOrganization->Enabled = true;
			 comboFinaProduct->Enabled = true;
			 comboInstrument->Enabled = true;
			 comboAccount->Enabled = false;
		 }
private: System::Void radioAccountQuery_Click(System::Object^  sender, System::EventArgs^  e) {
			 comboOrganization->Enabled = false;
			 comboFinaProduct->Enabled = false;
			 comboInstrument->Enabled = false;
			 comboAccount->Enabled = true;
		 }
private: System::Void CDlgRealTimePositionInfo_ResizeEnd(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void CDlgRealTimePositionInfo_Resize(System::Object^  sender, System::EventArgs^  e) {
			 Rectangle rect = this->ClientRectangle;
			 Rectangle rectGrid = dataGridPosition->DisplayRectangle;
			 System::Drawing::Size size(rect.Width-dataGridPosition->Left-10, rect.Height-dataGridPosition->Top-10);
			 dataGridPosition->Size = size;
		 }
private: System::Void MenuItem_FastForceClose_Click(System::Object^  sender, System::EventArgs^  e) {
			 
			 if(m_pdataInfo==NULL)
				 return;
			 if(!m_pdataInfo->HasPermission(FORCE_CLOSE_ORDER)) 
				 return;

			 if(dataGridPosition->SelectedRows->Count==0)
				 return;
			 int nRowIndex = dataGridPosition->SelectedRows[0]->Index;
			 if(nRowIndex<0)
				 return;
			 
			 int nRowID = int::Parse(dataGridPosition->Rows[nRowIndex]->Tag->ToString());
			 std::map<int, std::string>::iterator it;
			 std::map<std::string, SequencialPositionField>::iterator itpos;
			 SequencialPositionField position;
			 memset(&position, 0, sizeof(position));

			 save->LockObject();
			 it = save->mapIndex2Key.find(nRowID);
			 if(it == save->mapIndex2Key.end()) {
				 save->UnlockObject();
				 return;
			 }
			 itpos = save->mapPositionInfo.find(it->second);
			 if(itpos == save->mapPositionInfo.end()) {
				 save->UnlockObject();
				 return;
			 }
			 position = itpos->second;
			 save->UnlockObject();

			 ForceShortcutForm^ form = gcnew ForceShortcutForm();
			 if(form->init(position, RiskEvtModule)) {
				form->ShowDialog();
			 }
			 else {
                 MessageBox::Show("所属帐号并无风险事件发生，不能进行强平。", 
			 				"提示", MessageBoxButtons::OK, MessageBoxIcon::Error);
			 }
		 }
};
}
