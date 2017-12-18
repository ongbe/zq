#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Runtime::InteropServices;

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
#include "DlgRealTimeOrderInfoData.h"
#include "CInstrumentIDTreeListDlg.h"
#include "CFinancialProductIDListDlg.h"
#include "CAssetMgmtOrganIDTreeListDlg.h"
#include "COrganAndAccountIDTreeListDlg.h"


namespace Client {
	/// <summary>
	/// Summary for DlgRealTimeOrderInfo
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class DlgRealTimeOrderInfo : public System::Windows::Forms::Form
	{
	public:
		DlgRealTimeOrderInfo(void)
		{

			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			save = NULL;
			m_pdataInfo = CDataInfo::NewInstance();
			m_bHasExit = false;
			m_bHasChanged_QueryParam = true;
			save = DlgRealTimeOrderInfoData::NewInstance();
		}

		void ReleaseForm() {
			m_bHasExit = true;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~DlgRealTimeOrderInfo()
		{
			if (components)
			{
				delete components;
			}
			DlgRealTimeOrderInfoData::ReleaseInstance(save);
			save = NULL;
		}

	private: CDataInfo* m_pdataInfo;
	private: DlgRealTimeOrderInfoData* save;
	private: bool m_bHasExit;
	private: bool m_bHasChanged_QueryParam;
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

	private: System::Windows::Forms::Label^  label1;
	private: AnywndComboBox::AnywndComboBoxControl^  comboOrganization;

	private: System::Windows::Forms::Label^  label2;
	private: AnywndComboBox::AnywndComboBoxControl^  comboFinaProduct;


	private: System::Windows::Forms::Label^  label3;
	private: AnywndComboBox::AnywndComboBoxControl^  comboInstrument;

	private: System::Windows::Forms::Label^  label4;
	private: AnywndComboBox::AnywndComboBoxControl^  comboAccount;

	private: System::Windows::Forms::Button^  btnQuery;
	private: System::Windows::Forms::Button^  btnResetZero;
	private: System::Windows::Forms::DataGridView^  dataGridOrder;

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
			this->radioAccountQuery = (gcnew System::Windows::Forms::RadioButton());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->comboOrganization = (gcnew AnywndComboBox::AnywndComboBoxControl());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->comboFinaProduct = (gcnew AnywndComboBox::AnywndComboBoxControl());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->comboInstrument = (gcnew AnywndComboBox::AnywndComboBoxControl());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->comboAccount = (gcnew AnywndComboBox::AnywndComboBoxControl());
			this->btnQuery = (gcnew System::Windows::Forms::Button());
			this->btnResetZero = (gcnew System::Windows::Forms::Button());
			this->dataGridOrder = (gcnew System::Windows::Forms::DataGridView());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridOrder))->BeginInit();
			this->SuspendLayout();
			// 
			// radioGroupQuery
			// 
			this->radioGroupQuery->AutoSize = true;
			this->radioGroupQuery->Location = System::Drawing::Point(9, 11);
			this->radioGroupQuery->Name = L"radioGroupQuery";
			this->radioGroupQuery->Size = System::Drawing::Size(71, 16);
			this->radioGroupQuery->TabIndex = 0;
			this->radioGroupQuery->TabStop = true;
			this->radioGroupQuery->Text = L"组合查询";
			this->radioGroupQuery->UseVisualStyleBackColor = true;
			this->radioGroupQuery->Click += gcnew System::EventHandler(this, &DlgRealTimeOrderInfo::radioGroupQuery_Click);
			// 
			// radioAccountQuery
			// 
			this->radioAccountQuery->AutoSize = true;
			this->radioAccountQuery->Location = System::Drawing::Point(9, 37);
			this->radioAccountQuery->Name = L"radioAccountQuery";
			this->radioAccountQuery->Size = System::Drawing::Size(71, 16);
			this->radioAccountQuery->TabIndex = 0;
			this->radioAccountQuery->TabStop = true;
			this->radioAccountQuery->Text = L"指定查询";
			this->radioAccountQuery->UseVisualStyleBackColor = true;
			this->radioAccountQuery->Click += gcnew System::EventHandler(this, &DlgRealTimeOrderInfo::radioAccountQuery_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(106, 13);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(53, 12);
			this->label1->TabIndex = 1;
			this->label1->Text = L"组织架构";
			// 
			// comboOrganization
			// 
			this->comboOrganization->BackColor = System::Drawing::Color::White;
			this->comboOrganization->ChildControl = nullptr;
			this->comboOrganization->DropDownHeight = 1;
			this->comboOrganization->DropDownWidth = 1;
			this->comboOrganization->FormattingEnabled = true;
			this->comboOrganization->IntegralHeight = false;
			this->comboOrganization->Location = System::Drawing::Point(164, 7);
			this->comboOrganization->MaxDropDownItems = 1;
			this->comboOrganization->Name = L"comboOrganization";
			this->comboOrganization->Size = System::Drawing::Size(100, 20);
			this->comboOrganization->TabIndex = 2;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(279, 13);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(53, 12);
			this->label2->TabIndex = 1;
			this->label2->Text = L"理财产品";
			// 
			// comboFinaProduct
			// 
			this->comboFinaProduct->BackColor = System::Drawing::Color::White;
			this->comboFinaProduct->ChildControl = nullptr;
			this->comboFinaProduct->DropDownHeight = 1;
			this->comboFinaProduct->DropDownWidth = 1;
			this->comboFinaProduct->FormattingEnabled = true;
			this->comboFinaProduct->IntegralHeight = false;
			this->comboFinaProduct->Location = System::Drawing::Point(338, 7);
			this->comboFinaProduct->MaxDropDownItems = 1;
			this->comboFinaProduct->Name = L"comboFinaProduct";
			this->comboFinaProduct->Size = System::Drawing::Size(100, 20);
			this->comboFinaProduct->TabIndex = 2;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(460, 13);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(59, 12);
			this->label3->TabIndex = 1;
			this->label3->Text = L"品种/合约";
			// 
			// comboInstrument
			// 
			this->comboInstrument->BackColor = System::Drawing::Color::White;
			this->comboInstrument->ChildControl = nullptr;
			this->comboInstrument->DropDownHeight = 1;
			this->comboInstrument->DropDownWidth = 1;
			this->comboInstrument->FormattingEnabled = true;
			this->comboInstrument->IntegralHeight = false;
			this->comboInstrument->Location = System::Drawing::Point(519, 7);
			this->comboInstrument->MaxDropDownItems = 1;
			this->comboInstrument->Name = L"comboInstrument";
			this->comboInstrument->Size = System::Drawing::Size(100, 20);
			this->comboInstrument->TabIndex = 2;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(105, 39);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(41, 12);
			this->label4->TabIndex = 1;
			this->label4->Text = L"账户号";
			// 
			// comboAccount
			// 
			this->comboAccount->BackColor = System::Drawing::Color::White;
			this->comboAccount->ChildControl = nullptr;
			this->comboAccount->DropDownHeight = 1;
			this->comboAccount->DropDownWidth = 1;
			this->comboAccount->FormattingEnabled = true;
			this->comboAccount->IntegralHeight = false;
			this->comboAccount->Location = System::Drawing::Point(164, 33);
			this->comboAccount->MaxDropDownItems = 1;
			this->comboAccount->Name = L"comboAccount";
			this->comboAccount->Size = System::Drawing::Size(274, 20);
			this->comboAccount->TabIndex = 2;
			// 
			// btnQuery
			// 
			this->btnQuery->Location = System::Drawing::Point(28, 61);
			this->btnQuery->Name = L"btnQuery";
			this->btnQuery->Size = System::Drawing::Size(83, 24);
			this->btnQuery->TabIndex = 3;
			this->btnQuery->Text = L"查询";
			this->btnQuery->UseVisualStyleBackColor = true;
			this->btnQuery->Click += gcnew System::EventHandler(this, &DlgRealTimeOrderInfo::btnQuery_Click);
			// 
			// btnResetZero
			// 
			this->btnResetZero->Location = System::Drawing::Point(131, 61);
			this->btnResetZero->Name = L"btnResetZero";
			this->btnResetZero->Size = System::Drawing::Size(83, 24);
			this->btnResetZero->TabIndex = 3;
			this->btnResetZero->Text = L"清空";
			this->btnResetZero->UseVisualStyleBackColor = true;
			this->btnResetZero->Click += gcnew System::EventHandler(this, &DlgRealTimeOrderInfo::btnResetZero_Click);
			// 
			// dataGridOrder
			// 
			this->dataGridOrder->AllowUserToAddRows = false;
			this->dataGridOrder->AllowUserToDeleteRows = false;
			this->dataGridOrder->AllowUserToResizeRows = false;
			this->dataGridOrder->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridOrder->EditMode = System::Windows::Forms::DataGridViewEditMode::EditProgrammatically;
			this->dataGridOrder->Location = System::Drawing::Point(11, 95);
			this->dataGridOrder->Margin = System::Windows::Forms::Padding(1);
			this->dataGridOrder->MultiSelect = false;
			this->dataGridOrder->Name = L"dataGridOrder";
			this->dataGridOrder->ReadOnly = true;
			this->dataGridOrder->RowHeadersVisible = false;
			this->dataGridOrder->RowTemplate->Height = 23;
			this->dataGridOrder->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dataGridOrder->Size = System::Drawing::Size(611, 307);
			this->dataGridOrder->TabIndex = 4;
			// 
			// DlgRealTimeOrderInfo
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(632, 413);
			this->Controls->Add(this->dataGridOrder);
			this->Controls->Add(this->btnResetZero);
			this->Controls->Add(this->btnQuery);
			this->Controls->Add(this->comboInstrument);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->comboFinaProduct);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->comboAccount);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->comboOrganization);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->radioAccountQuery);
			this->Controls->Add(this->radioGroupQuery);
			this->Name = L"DlgRealTimeOrderInfo";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"实时报单";
			this->Load += gcnew System::EventHandler(this, &DlgRealTimeOrderInfo::DlgRealTimeOrderInfo_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &DlgRealTimeOrderInfo::DlgRealTimeOrderInfo_FormClosed);
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &DlgRealTimeOrderInfo::DlgRealTimeOrderInfo_FormClosing);
			this->Resize += gcnew System::EventHandler(this, &DlgRealTimeOrderInfo::DlgRealTimeOrderInfo_Resize);
			this->ResizeEnd += gcnew System::EventHandler(this, &DlgRealTimeOrderInfo::DlgRealTimeOrderInfo_ResizeEnd);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridOrder))->EndInit();
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
			case Cmd_RM_SubscribeOrder_Rsp:
			{
				dwTcpEndTime = GetTickCount();

				if(head.userdata1 == OPERATOR_SUCCESS) {
					SequencialOrderField* pOrderInfo = NULL;
					int nItemCount = head.len/sizeof(SequencialOrderField);

					save->LockObject();
					for(int i=0; i<nItemCount; i++) {
						pOrderInfo = (SequencialOrderField*)
								(p+sizeof(Stru_UniPkgHead)+i*sizeof(SequencialOrderField));
						save->queueOrderInfo.push(*pOrderInfo);
					}
					save->UnlockObject();
					if(head.userdata2 == head.userdata3 - 1) {
					
						dwShowStartTime = GetTickCount();

						std::queue<SequencialOrderField> queueRet;
						save->CheckNewData(m_pdataInfo, queueRet);
						ShowQueue(queueRet);

						dwShowEndTime = GetTickCount();
						{
							char csText[1024] = {0};
							sprintf(csText, "实时报单查询，本次查询时间消耗 传输==%u, 显示==%u",
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
			case Cmd_RM_UnSubscribeOrder_Rsp:
				if(head.userdata1 == OPERATOR_SUCCESS) {
				}
				else {
					p[sizeof(head)+head.len-1]=0;
					//TRACE(p+sizeof(head));
				}
				break;
			case Cmd_RM_Order_Push:
			{
				if(head.userdata1 == OPERATOR_SUCCESS) {
					SequencialOrderField* pOrderInfo = NULL;
					int nItemCount = head.len/sizeof(SequencialOrderField);

					save->LockObject();
					for(int i=0; i<nItemCount; i++) {
						pOrderInfo = (SequencialOrderField*)
								(p+sizeof(Stru_UniPkgHead)+i*sizeof(SequencialOrderField));
						save->queueOrderInfo.push(*pOrderInfo);
					}
					save->UnlockObject();
					
					std::queue<SequencialOrderField> queueRet;
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
		for(int i=0; i<dataGridOrder->Rows->Count; i++) {
			if(int::Parse(dataGridOrder->Rows[i]->Tag->ToString()) == nRowID)
				return dataGridOrder->Rows[i]->Index;
		}
		return -1;
	}
	int GetColIndexByColID(int nColID) {
		for(int i=0; i<dataGridOrder->Columns->Count; i++) {
			if(int::Parse(dataGridOrder->Columns[i]->Tag->ToString()) == nColID)
				return dataGridOrder->Columns[i]->Index;
		}
		return -1;
	}
	void InitGridHead() {

		dataGridOrder->Columns->Clear();

		int nColID = 0;

		System::Windows::Forms::DataGridViewTextBoxColumn^ gridColumn = 
				(gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_InvestorID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"投资者代码";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_InvestorID));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_AssertMgmtOrgan;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"账号所属资管";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_AssertMgmtOrgan));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_FinanProduct;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"账号所属理财产品";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_FinanProduct));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_BrokerID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"经纪公司代码";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_BrokerID));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_InstrumentID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"合约代码";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_InstrumentID));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_OrderRef;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"报单引用";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_OrderRef));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_UserID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"用户代码";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_UserID));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_OrderPriceType;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"报单价格条件";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_OrderPriceType));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_Direction;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"买卖方向";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_Direction));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_CombOffsetFlag;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"组合开平标志";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_CombOffsetFlag));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_CombHedgeFlag;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"组合投机套保标志";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_CombHedgeFlag));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_LimitPrice;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"价格";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_LimitPrice));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_VolumeTotalOriginal;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"数量";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_VolumeTotalOriginal));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_TimeCondition;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"有效期类型";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_TimeCondition));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_GTDDate;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"GTD日期";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_GTDDate));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_VolumeCondition;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"成交量类型";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_VolumeCondition));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_MinVolume;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"最小成交量";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_MinVolume));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_ContingentCondition;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"触发条件";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_ContingentCondition));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_StopPrice;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"止损价";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_StopPrice));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_ForceCloseReason;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"强平原因";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_ForceCloseReason));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_IsAutoSuspend;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"自动挂起标志";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_IsAutoSuspend));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_BusinessUnit;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"业务单元";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_BusinessUnit));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_RequestID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"请求编号";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_RequestID));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_OrderLocalID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"本地报单编号";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_OrderLocalID));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_ExchangeID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"交易所代码";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_ExchangeID));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_ParticipantID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"会员代码";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_ParticipantID));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_ClientID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"客户代码";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_ClientID));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_ExchangeInstID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"合约在交易所的代码";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_ExchangeInstID));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_TraderID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"交易所交易员代码";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_TraderID));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_InstallID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"安装编号";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_InstallID));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_OrderSubmitStatus;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"报单提交状态";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_OrderSubmitStatus));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_NotifySequence;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"报单提示序号";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_NotifySequence));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_TradingDay;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"交易日";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_TradingDay));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_SettlementID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"结算编号";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_SettlementID));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_OrderSysID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"报单编号";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_OrderSysID));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_OrderSource;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"报单来源";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_OrderSource));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_OrderStatus;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"报单状态";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_OrderStatus));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_OrderType;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"报单类型";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_OrderType));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_VolumeTraded;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"今成交数量";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_VolumeTraded));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_VolumeTotal;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"剩余数量";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_VolumeTotal));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_InsertDate;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"报单日期";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_InsertDate));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_InsertTime;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"委托时间";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_InsertTime));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_ActiveTime;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"激活时间";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_ActiveTime));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_SuspendTime;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"挂起时间";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_SuspendTime));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_UpdateTime;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"最后修改时间";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_UpdateTime));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_CancelTime;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"撤销时间";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_CancelTime));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_ActiveTraderID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"最后修改交易所交易员代码";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_ActiveTraderID));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_ClearingPartID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"结算会员编号";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_ClearingPartID));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_SequenceNo;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"序号";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_SequenceNo));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_FrontID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"前置编号";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_FrontID));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_SessionID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"会话编号";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_SessionID));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_UserProductInfo;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"用户端产品信息";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_UserProductInfo));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_StatusMsg;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"状态信息";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_StatusMsg));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_UserForceClose;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"用户强评标志";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_UserForceClose));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_ActiveUserID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"操作用户代码";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_ActiveUserID));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_BrokerOrderSeq;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"经纪公司报单编号";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_BrokerOrderSeq));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_RelativeOrderSysID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"相关报单";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_RelativeOrderSysID));
		dataGridOrder->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQO_ZCETotalTradedVolume;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"郑商所成交数量";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQO_ZCETotalTradedVolume));
		dataGridOrder->Columns->Add(gridColumn);
	}
	void UpdateRow(int nIndex, SequencialOrderField& orderInfo) {
		if(nIndex<0)
			return;
		int nCol = -1;
		std::string strText;
		DataGridViewRow^ dr = dataGridOrder->Rows[nIndex];
		char strTemp[1024];
		if((nCol = GetColIndexByColID(conCol_RTQO_InvestorID))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(orderInfo.InvestorID);
		}
		TrustTradeAccount traderAccount;
		memset(&traderAccount, 0, sizeof(traderAccount));
		if(m_pdataInfo->GetTrustTradeAccount(std::string(orderInfo.InvestorID), traderAccount)) {
			AssetMgmtOrganization organ;
			memset(&organ, 0, sizeof(organ));
			if(m_pdataInfo->GetAssetMgmtOrg(traderAccount.nAssetMgmtOrgID, organ)) {
				if((nCol = GetColIndexByColID(conCol_RTQO_AssertMgmtOrgan))>=0) {
					dr->Cells[nCol]->Value = Tools::string2String(organ.szName);
				}
			}
			FinancialProduct finanProduct;
			memset(&finanProduct, 0, sizeof(finanProduct));
			if(m_pdataInfo->GetFinancialProduct(traderAccount.nFinancialProductID, finanProduct)) {
				if((nCol = GetColIndexByColID(conCol_RTQO_FinanProduct))>=0) {
					dr->Cells[nCol]->Value = Tools::string2String(finanProduct.szName);
				}
			}
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_BrokerID))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(orderInfo.BrokerID);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_InstrumentID))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(orderInfo.InstrumentID);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_OrderRef))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(orderInfo.OrderRef);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_UserID))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(orderInfo.UserID);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_OrderPriceType))>=0) {
			strText = CFieldDetail::OrderPriceType2String(orderInfo.OrderPriceType);
			dr->Cells[nCol]->Value = Tools::string2String(strText.c_str());
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_Direction))>=0) {
			strText = CFieldDetail::Direction2String(orderInfo.Direction);
			dr->Cells[nCol]->Value = Tools::string2String(strText.c_str());
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_CombOffsetFlag))>=0) {
			strText = CFieldDetail::OffsetFlag2String(orderInfo.CombOffsetFlag[0]);
			dr->Cells[nCol]->Value = Tools::string2String(strText.c_str());
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_CombHedgeFlag))>=0) {
			strText = CFieldDetail::HedgeFlag2String(orderInfo.CombHedgeFlag[0]);
			dr->Cells[nCol]->Value = Tools::string2String(strText.c_str());
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_LimitPrice))>=0) {
			sprintf(strTemp, "%0.2f", orderInfo.LimitPrice);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_VolumeTotalOriginal))>=0) {
			sprintf(strTemp, "%d", orderInfo.VolumeTotalOriginal);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_TimeCondition))>=0) {
			strText = CFieldDetail::TimeCondition2String(orderInfo.TimeCondition);
			dr->Cells[nCol]->Value = Tools::string2String(strText.c_str());
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_GTDDate))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(orderInfo.GTDDate);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_VolumeCondition))>=0) {
			strText = CFieldDetail::VolumeCondition2String(orderInfo.VolumeCondition);
			dr->Cells[nCol]->Value = Tools::string2String(strText.c_str());
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_MinVolume))>=0) {
			sprintf(strTemp, "%d", orderInfo.MinVolume);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_ContingentCondition))>=0) {
			strText = orderInfo.ContingentCondition;
			dr->Cells[nCol]->Value = Tools::string2String(strText.c_str());
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_StopPrice))>=0) {
			sprintf(strTemp, "%0.2f", orderInfo.StopPrice);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_ForceCloseReason))>=0) {
			strText = CFieldDetail::ForceCloseReason2String(orderInfo.ForceCloseReason);
			dr->Cells[nCol]->Value = Tools::string2String(strText.c_str());
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_IsAutoSuspend))>=0) {
			sprintf(strTemp, "%s", (orderInfo.IsAutoSuspend?"是":"否"));
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_BusinessUnit))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(orderInfo.BusinessUnit);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_RequestID))>=0) {
			sprintf(strTemp, "%d", orderInfo.RequestID);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_OrderLocalID))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(orderInfo.OrderLocalID);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_ExchangeID))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(orderInfo.ExchangeID);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_ParticipantID))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(orderInfo.ParticipantID);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_ClientID))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(orderInfo.ClientID);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_ExchangeInstID))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(orderInfo.ExchangeInstID);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_TraderID))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(orderInfo.TraderID);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_InstallID))>=0) {
			sprintf(strTemp, "%d", orderInfo.InstallID);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_OrderSubmitStatus))>=0) {
			strText = CFieldDetail::OrderSubmitStatus2String(orderInfo.OrderSubmitStatus);
			dr->Cells[nCol]->Value = Tools::string2String(strText.c_str());
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_NotifySequence))>=0) {
			sprintf(strTemp, "%d", orderInfo.NotifySequence);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_TradingDay))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(orderInfo.TradingDay);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_SettlementID))>=0) {
			sprintf(strTemp, "%d", orderInfo.SettlementID);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_OrderSysID))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(orderInfo.OrderSysID);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_OrderSource))>=0) {
			strText = orderInfo.OrderSource;
			dr->Cells[nCol]->Value = Tools::string2String(strText.c_str());
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_OrderStatus))>=0) {
			strText = CFieldDetail::OrderStatus2String(orderInfo.OrderStatus);
			dr->Cells[nCol]->Value = Tools::string2String(strText.c_str());
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_OrderType))>=0) {
			strText = CFieldDetail::OrderType2String(orderInfo.OrderType);
			dr->Cells[nCol]->Value = Tools::string2String(strText.c_str());
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_VolumeTraded))>=0) {
			sprintf(strTemp, "%d", orderInfo.VolumeTraded);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_VolumeTotal))>=0) {
			sprintf(strTemp, "%d", orderInfo.VolumeTotal);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_InsertDate))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(orderInfo.InsertDate);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_InsertTime))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(orderInfo.InsertTime);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_ActiveTime))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(orderInfo.ActiveTime);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_SuspendTime))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(orderInfo.SuspendTime);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_UpdateTime))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(orderInfo.UpdateTime);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_CancelTime))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(orderInfo.CancelTime);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_ActiveTraderID))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(orderInfo.ActiveTraderID);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_ClearingPartID))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(orderInfo.ClearingPartID);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_SequenceNo))>=0) {
			sprintf(strTemp, "%d", orderInfo.SequenceNo);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_FrontID))>=0) {
			sprintf(strTemp, "%u", orderInfo.FrontID);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_SessionID))>=0) {
			sprintf(strTemp, "%x", orderInfo.SessionID);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_UserProductInfo))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(orderInfo.UserProductInfo);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_StatusMsg))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(orderInfo.StatusMsg);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_UserForceClose))>=0) {
			sprintf(strTemp, "%s", (orderInfo.UserForceClose?"是":"否"));
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_ActiveUserID))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(orderInfo.ActiveUserID);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_BrokerOrderSeq))>=0) {
			sprintf(strTemp, "%u", orderInfo.BrokerOrderSeq);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_RelativeOrderSysID))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(orderInfo.RelativeOrderSysID);
		}
		if((nCol = GetColIndexByColID(conCol_RTQO_ZCETotalTradedVolume))>=0) {
			sprintf(strTemp, "%d", orderInfo.ZCETotalTradedVolume);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
	}

	void ShowQueue(std::queue<SequencialOrderField>& queueRet) {
		std::map<string, int>::iterator it;
		int nIndex = -1;
		char strTemp[1024];
		std::string strKey;
		strKey.reserve(1024);
		save->LockObject();
		while(!queueRet.empty()) {
			SequencialOrderField field = queueRet.front();
			queueRet.pop();

			memset(strTemp, 0, sizeof(strTemp));
			sprintf(strTemp, "%s, %s, %x, %x", 
					field.InvestorID, field.OrderRef, field.FrontID, field.SessionID);
			strKey = strTemp;
			it = save->mapKey2Index.find(strKey);
			if(it != save->mapKey2Index.end()) {
				nIndex = GetRowIndexByRowID(it->second);
			}
			else {
				DataGridViewRow^ newRow = gcnew DataGridViewRow;
				int nRowID = save->GetNextRowID();
				newRow->Tag = nRowID;
				nIndex = dataGridOrder->Rows->Add(newRow);
				if(nIndex>=0) {
					save->mapKey2Index.insert(std::make_pair(strKey, nRowID));
					save->mapIndex2Key.insert(std::make_pair(nRowID, strKey));
				}
			}
			if(nIndex>=0) {
				UpdateRow(nIndex, field);
			}
		}
		save->UnlockObject();
	}

private: System::Void DlgRealTimeOrderInfo_Load(System::Object^  sender, System::EventArgs^  e) {
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
			 CTcpLayer::SubscribePkg(Cmd_RM_SubscribeOrder_Rsp, (int)hWnd);
			 CTcpLayer::SubscribePkg(Cmd_RM_Order_Push, (int)hWnd);
			 CTcpLayer::SubscribePkg(Cmd_RM_UnSubscribeOrder_Rsp, (int)hWnd);
			 
			 ResetComboBoxTextReadOnly(comboFinaProduct);
			 ResetComboBoxTextReadOnly(comboOrganization);
			 ResetComboBoxTextReadOnly(comboAccount);
			 ResetComboBoxTextReadOnly(comboInstrument);

			 comboOrganization->Enabled = false;
			 comboFinaProduct->Enabled = false;
			 comboInstrument->Enabled = false;
			 comboAccount->Enabled = true;

			 if(m_pdataInfo!=NULL)
				 btnQuery->Enabled = m_pdataInfo->HasPermission(TODAY_OPEN_ORDER_QUERY);

		 }

private: System::Void DlgRealTimeOrderInfo_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
			if(!m_bHasExit) {
				Hide();
				e->Cancel = true;
			}
			save->ResetQuery();
			dataGridOrder->Rows->Clear();
		 }

private: System::Void DlgRealTimeOrderInfo_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
			 IntPtr hWnd=this->Handle;
			 CTcpLayer::UnsubscribeAllPkg((int)hWnd);
		 }

		 // 查询按钮响应事件
private: System::Void btnQuery_Click(System::Object^  sender, System::EventArgs^  e) {

			 if(!m_bHasChanged_QueryParam)
				 return;

			 bool bHasData = false;
			 dataGridOrder->Rows->Clear();

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
		 
		 // 清空按钮响应事件
private: System::Void btnResetZero_Click(System::Object^  sender, System::EventArgs^  e) {
			
			 m_bHasChanged_QueryParam = true;
			 //dlgMgmtOrganIDTreeList->Clear();
			 //dlgFinancialProductIDList->Clear();
			 //dlgInstrumentIDTreeList->Clear();
			 //dlgOrganAndAccountIDTreeList->Clear();

			 dataGridOrder->Rows->Clear();

			 save->LockObject();
			 save->mapKey2Index.clear();
			 save->mapIndex2Key.clear();
			 save->mapOrderInfo.clear();
			 save->UnlockObject();

			 std::set<int>::iterator it;
			 int nAccountID = 0;
			 
			 save->LockObject();
			 // 先对原来的订阅进行退订
			 for(it = save->setQueryAccountID.begin(); 
					it != save->setQueryAccountID.end(); it++) {
				 nAccountID = *it;
 				 // 这里实际需要的是按帐号进行订阅退订
				 CTcpLayer::SendData(Cmd_RM_UnSubscribeOrder_Req, (void*)&nAccountID, sizeof(nAccountID), 0);
			 }
			 save->setQueryAccountID.clear();
			 save->UnlockObject();
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
private: System::Void DlgRealTimeOrderInfo_ResizeEnd(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void DlgRealTimeOrderInfo_Resize(System::Object^  sender, System::EventArgs^  e) {
			 Rectangle rect = this->ClientRectangle;
			 Rectangle rectGrid = dataGridOrder->DisplayRectangle;
			 System::Drawing::Size size(rect.Width-dataGridOrder->Left-10, rect.Height-dataGridOrder->Top-10);
			 dataGridOrder->Size = size;
		 }
};
}
