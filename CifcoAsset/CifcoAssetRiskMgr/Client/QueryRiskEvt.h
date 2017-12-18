#pragma once
#include <set>
#include <map>
#include <queue>
#include "CCommonFunc.h"
#include "TcpLayer.h"
#include "CDataInfo.h"
#include "RiskManageCmd.h"
#include "UserApiStruct.h"
#include "CommonPkg.h"
#include "CFinancialProductIDListDlg.h"
#include "CAssetMgmtOrganIDTreeListDlg.h"
#include "COrganAndAccountIDTreeListDlg.h"
#include "Tools.h"
#include "QueryRiskEvt2.h"
#include "RiskRecord.h"

using namespace std;
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace Client {

	/// <summary>
	/// Summary for QueryRiskEvt
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class QueryRiskEvt : public System::Windows::Forms::Form
	{
	public:
		QueryRiskEvt(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_QueryRiskEvt2 = new QueryRiskEvt2();
			m_pdataInfo = CDataInfo::NewInstance();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~QueryRiskEvt()
		{
			if (components)
			{
				delete components;
			}
		}
  
    private: QueryRiskEvt2*                    m_QueryRiskEvt2;
	private: CDataInfo*                        m_pdataInfo;
	private: CAssetMgmtOrganIDTreeListDlg^     dlgMgmtOrganIDTreeList;
	private: CFinancialProductIDListDlg^       dlgFinancialProductIDList;
	private: COrganAndAccountIDTreeListDlg^    dlgOrganAndAccountIDTreeList;
	private: RiskRecord^                       dlgRiskRecord;
    


	private: System::Windows::Forms::Label^  label1;

	protected: 

	private: System::Windows::Forms::RadioButton^  radioButton1;
	private: System::Windows::Forms::Label^  label2;

	private: System::Windows::Forms::Label^  label3;

	private: System::Windows::Forms::RadioButton^  radioButton2;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::DateTimePicker^  dateTimePicker1;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::ComboBox^  comboBox_status;
	private: System::Windows::Forms::DataGridView^  dataGridView1;
	private: AnywndComboBox::AnywndComboBoxControl^  comboOrganization;
	private: AnywndComboBox::AnywndComboBoxControl^  comboFinaProduct;
	private: AnywndComboBox::AnywndComboBoxControl^  comboAccount;
	private: System::Windows::Forms::Button^  button_query;
	private: System::Windows::Forms::Button^  button_clear;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_riskEvtId;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_acct;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_org;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_Finacial;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_time;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_riskModule;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_RiskIndicatorID;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_InstrumentID;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_RiskLevelID;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_IndicatorValue;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_EvtStatus;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ColumnType;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::DateTimePicker^  dateTimePickerStart;










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
			this->radioButton1 = (gcnew System::Windows::Forms::RadioButton());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->radioButton2 = (gcnew System::Windows::Forms::RadioButton());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->dateTimePicker1 = (gcnew System::Windows::Forms::DateTimePicker());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->comboBox_status = (gcnew System::Windows::Forms::ComboBox());
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->Column_riskEvtId = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_acct = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_org = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_Finacial = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_time = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_riskModule = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_RiskIndicatorID = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_InstrumentID = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_RiskLevelID = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_IndicatorValue = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_EvtStatus = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ColumnType = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->comboFinaProduct = (gcnew AnywndComboBox::AnywndComboBoxControl());
			this->comboOrganization = (gcnew AnywndComboBox::AnywndComboBoxControl());
			this->comboAccount = (gcnew AnywndComboBox::AnywndComboBoxControl());
			this->button_query = (gcnew System::Windows::Forms::Button());
			this->button_clear = (gcnew System::Windows::Forms::Button());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->dateTimePickerStart = (gcnew System::Windows::Forms::DateTimePicker());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->BeginInit();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(125, 67);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(41, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"账户号";
			// 
			// radioButton1
			// 
			this->radioButton1->AutoSize = true;
			this->radioButton1->Location = System::Drawing::Point(33, 29);
			this->radioButton1->Name = L"radioButton1";
			this->radioButton1->Size = System::Drawing::Size(71, 16);
			this->radioButton1->TabIndex = 2;
			this->radioButton1->TabStop = true;
			this->radioButton1->Text = L"组合查询";
			this->radioButton1->UseVisualStyleBackColor = true;
			this->radioButton1->Click += gcnew System::EventHandler(this, &QueryRiskEvt::radioButton1_Click);
			this->radioButton1->CheckedChanged += gcnew System::EventHandler(this, &QueryRiskEvt::radioButton1_CheckedChanged);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(125, 31);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(53, 12);
			this->label2->TabIndex = 3;
			this->label2->Text = L"组织架构";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(397, 31);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(53, 12);
			this->label3->TabIndex = 5;
			this->label3->Text = L"理财产品";
			// 
			// radioButton2
			// 
			this->radioButton2->AutoSize = true;
			this->radioButton2->Location = System::Drawing::Point(33, 65);
			this->radioButton2->Name = L"radioButton2";
			this->radioButton2->Size = System::Drawing::Size(71, 16);
			this->radioButton2->TabIndex = 7;
			this->radioButton2->TabStop = true;
			this->radioButton2->Text = L"指定查询";
			this->radioButton2->UseVisualStyleBackColor = true;
			this->radioButton2->Click += gcnew System::EventHandler(this, &QueryRiskEvt::radioButton2_Click);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(260, 106);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(53, 12);
			this->label4->TabIndex = 8;
			this->label4->Text = L"结束日期";
			// 
			// dateTimePicker1
			// 
			this->dateTimePicker1->Location = System::Drawing::Point(335, 102);
			this->dateTimePicker1->Name = L"dateTimePicker1";
			this->dateTimePicker1->Size = System::Drawing::Size(115, 21);
			this->dateTimePicker1->TabIndex = 9;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(506, 107);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(29, 12);
			this->label5->TabIndex = 10;
			this->label5->Text = L"状态";
			// 
			// comboBox_status
			// 
			this->comboBox_status->FormattingEnabled = true;
			this->comboBox_status->Items->AddRange(gcnew cli::array< System::Object^  >(7) {L"", L"新发生", L"等级变更", L"消失", L"手动操作，委派给交易员", 
				L"手动操作，风险事件解决记录", L"手动操作，执行手动强平记录"});
			this->comboBox_status->Location = System::Drawing::Point(558, 103);
			this->comboBox_status->Name = L"comboBox_status";
			this->comboBox_status->Size = System::Drawing::Size(121, 20);
			this->comboBox_status->TabIndex = 11;
			// 
			// dataGridView1
			// 
			this->dataGridView1->AllowUserToAddRows = false;
			this->dataGridView1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->dataGridView1->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::AllCells;
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(12) {this->Column_riskEvtId, 
				this->Column_acct, this->Column_org, this->Column_Finacial, this->Column_time, this->Column_riskModule, this->Column_RiskIndicatorID, 
				this->Column_InstrumentID, this->Column_RiskLevelID, this->Column_IndicatorValue, this->Column_EvtStatus, this->ColumnType});
			this->dataGridView1->Location = System::Drawing::Point(33, 181);
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->ReadOnly = true;
			this->dataGridView1->RowHeadersVisible = false;
			this->dataGridView1->RowTemplate->Height = 23;
			this->dataGridView1->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dataGridView1->Size = System::Drawing::Size(970, 359);
			this->dataGridView1->TabIndex = 12;
			this->dataGridView1->RowDividerDoubleClick += gcnew System::Windows::Forms::DataGridViewRowDividerDoubleClickEventHandler(this, &QueryRiskEvt::dataGridView1_RowDividerDoubleClick);
			this->dataGridView1->CellDoubleClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &QueryRiskEvt::dataGridView1_CellDoubleClick);
			// 
			// Column_riskEvtId
			// 
			this->Column_riskEvtId->HeaderText = L"风险事件ID";
			this->Column_riskEvtId->Name = L"Column_riskEvtId";
			this->Column_riskEvtId->ReadOnly = true;
			this->Column_riskEvtId->Width = 72;
			// 
			// Column_acct
			// 
			this->Column_acct->HeaderText = L"帐号";
			this->Column_acct->Name = L"Column_acct";
			this->Column_acct->ReadOnly = true;
			this->Column_acct->Width = 51;
			// 
			// Column_org
			// 
			this->Column_org->HeaderText = L"账号所属资管";
			this->Column_org->Name = L"Column_org";
			this->Column_org->ReadOnly = true;
			this->Column_org->Width = 72;
			// 
			// Column_Finacial
			// 
			this->Column_Finacial->HeaderText = L"账号所属理财产品";
			this->Column_Finacial->Name = L"Column_Finacial";
			this->Column_Finacial->ReadOnly = true;
			this->Column_Finacial->Width = 83;
			// 
			// Column_time
			// 
			this->Column_time->HeaderText = L"日期时间";
			this->Column_time->Name = L"Column_time";
			this->Column_time->ReadOnly = true;
			this->Column_time->Width = 61;
			// 
			// Column_riskModule
			// 
			this->Column_riskModule->HeaderText = L"风险指标模型";
			this->Column_riskModule->Name = L"Column_riskModule";
			this->Column_riskModule->ReadOnly = true;
			this->Column_riskModule->Width = 72;
			// 
			// Column_RiskIndicatorID
			// 
			this->Column_RiskIndicatorID->HeaderText = L"风险指标";
			this->Column_RiskIndicatorID->Name = L"Column_RiskIndicatorID";
			this->Column_RiskIndicatorID->ReadOnly = true;
			this->Column_RiskIndicatorID->Width = 61;
			// 
			// Column_InstrumentID
			// 
			this->Column_InstrumentID->HeaderText = L"合约ID   ";
			this->Column_InstrumentID->Name = L"Column_InstrumentID";
			this->Column_InstrumentID->ReadOnly = true;
			this->Column_InstrumentID->Width = 56;
			// 
			// Column_RiskLevelID
			// 
			this->Column_RiskLevelID->HeaderText = L"风险等级";
			this->Column_RiskLevelID->Name = L"Column_RiskLevelID";
			this->Column_RiskLevelID->ReadOnly = true;
			this->Column_RiskLevelID->Width = 61;
			// 
			// Column_IndicatorValue
			// 
			this->Column_IndicatorValue->HeaderText = L"风险指标值";
			this->Column_IndicatorValue->Name = L"Column_IndicatorValue";
			this->Column_IndicatorValue->ReadOnly = true;
			this->Column_IndicatorValue->Width = 72;
			// 
			// Column_EvtStatus
			// 
			this->Column_EvtStatus->HeaderText = L"状态";
			this->Column_EvtStatus->Name = L"Column_EvtStatus";
			this->Column_EvtStatus->ReadOnly = true;
			this->Column_EvtStatus->Width = 51;
			// 
			// ColumnType
			// 
			this->ColumnType->HeaderText = L"事件类型";
			this->ColumnType->Name = L"ColumnType";
			this->ColumnType->ReadOnly = true;
			this->ColumnType->Width = 61;
			// 
			// comboFinaProduct
			// 
			this->comboFinaProduct->BackColor = System::Drawing::Color::White;
			this->comboFinaProduct->ChildControl = nullptr;
			this->comboFinaProduct->DropDownHeight = 1;
			this->comboFinaProduct->DropDownWidth = 1;
			this->comboFinaProduct->FormattingEnabled = true;
			this->comboFinaProduct->IntegralHeight = false;
			this->comboFinaProduct->Location = System::Drawing::Point(482, 24);
			this->comboFinaProduct->MaxDropDownItems = 1;
			this->comboFinaProduct->Name = L"comboFinaProduct";
			this->comboFinaProduct->Size = System::Drawing::Size(121, 20);
			this->comboFinaProduct->TabIndex = 6;
			// 
			// comboOrganization
			// 
			this->comboOrganization->BackColor = System::Drawing::Color::White;
			this->comboOrganization->ChildControl = nullptr;
			this->comboOrganization->DropDownHeight = 1;
			this->comboOrganization->DropDownWidth = 1;
			this->comboOrganization->FormattingEnabled = true;
			this->comboOrganization->IntegralHeight = false;
			this->comboOrganization->Location = System::Drawing::Point(201, 25);
			this->comboOrganization->MaxDropDownItems = 1;
			this->comboOrganization->Name = L"comboOrganization";
			this->comboOrganization->Size = System::Drawing::Size(121, 20);
			this->comboOrganization->TabIndex = 4;
			// 
			// comboAccount
			// 
			this->comboAccount->BackColor = System::Drawing::Color::White;
			this->comboAccount->ChildControl = nullptr;
			this->comboAccount->DropDownHeight = 1;
			this->comboAccount->DropDownWidth = 1;
			this->comboAccount->FormattingEnabled = true;
			this->comboAccount->IntegralHeight = false;
			this->comboAccount->Location = System::Drawing::Point(201, 61);
			this->comboAccount->MaxDropDownItems = 1;
			this->comboAccount->Name = L"comboAccount";
			this->comboAccount->Size = System::Drawing::Size(121, 20);
			this->comboAccount->TabIndex = 1;
			// 
			// button_query
			// 
			this->button_query->Location = System::Drawing::Point(33, 142);
			this->button_query->Name = L"button_query";
			this->button_query->Size = System::Drawing::Size(75, 23);
			this->button_query->TabIndex = 13;
			this->button_query->Text = L"查询";
			this->button_query->UseVisualStyleBackColor = true;
			this->button_query->Click += gcnew System::EventHandler(this, &QueryRiskEvt::button_query_Click);
			// 
			// button_clear
			// 
			this->button_clear->Location = System::Drawing::Point(155, 142);
			this->button_clear->Name = L"button_clear";
			this->button_clear->Size = System::Drawing::Size(75, 23);
			this->button_clear->TabIndex = 14;
			this->button_clear->Text = L"清空";
			this->button_clear->UseVisualStyleBackColor = true;
			this->button_clear->Click += gcnew System::EventHandler(this, &QueryRiskEvt::button_clear_Click);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(33, 106);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(53, 12);
			this->label6->TabIndex = 15;
			this->label6->Text = L"起始日期";
			// 
			// dateTimePickerStart
			// 
			this->dateTimePickerStart->Location = System::Drawing::Point(104, 103);
			this->dateTimePickerStart->Name = L"dateTimePickerStart";
			this->dateTimePickerStart->Size = System::Drawing::Size(111, 21);
			this->dateTimePickerStart->TabIndex = 16;
			// 
			// QueryRiskEvt
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1021, 565);
			this->Controls->Add(this->dateTimePickerStart);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->button_clear);
			this->Controls->Add(this->button_query);
			this->Controls->Add(this->dataGridView1);
			this->Controls->Add(this->comboBox_status);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->dateTimePicker1);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->radioButton2);
			this->Controls->Add(this->comboFinaProduct);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->comboOrganization);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->radioButton1);
			this->Controls->Add(this->comboAccount);
			this->Controls->Add(this->label1);
			this->Name = L"QueryRiskEvt";
			this->Text = L"用户风险事件查询";
			this->Load += gcnew System::EventHandler(this, &QueryRiskEvt::QueryRiskEvt_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &QueryRiskEvt::QueryRiskEvt_FormClosed);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void QueryRiskEvt_Load(System::Object^  sender, System::EventArgs^  e) 
		 {
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
			 CTcpLayer::SubscribePkg(Cmd_RM_QryRiskEvent_Rsp, (int)hWnd);


			 ResetComboBoxTextReadOnly(comboFinaProduct);
			 ResetComboBoxTextReadOnly(comboOrganization);
			 ResetComboBoxTextReadOnly(comboAccount);

			 // comboAccount
			this->comboAccount->ChildControl = nullptr;
			this->comboAccount->DropDownHeight = 1;
			this->comboAccount->DropDownWidth = 1;
			this->comboAccount->FormattingEnabled = true;
			this->comboAccount->IntegralHeight = false;
			this->comboAccount->MaxDropDownItems = 1;
			//this->comboAccount->DropDown += gcnew System::EventHandler(this, &QueryRiskEvt::comboAccount_DropDown);

			// comboFinaProduct
			this->comboFinaProduct->ChildControl = nullptr;
			this->comboFinaProduct->DropDownHeight = 1;
			this->comboFinaProduct->DropDownWidth = 1;
			this->comboFinaProduct->FormattingEnabled = true;
			this->comboFinaProduct->IntegralHeight = false;
			this->comboFinaProduct->MaxDropDownItems = 1;
			//this->comboFinaProduct->DropDown += gcnew System::EventHandler(this, &QueryRiskEvt::comboFinaProduct_DropDown);

			//comboOrganization
			this->comboOrganization->ChildControl = nullptr;
			this->comboOrganization->DropDownHeight = 1;
			this->comboOrganization->DropDownWidth = 1;
			this->comboOrganization->IntegralHeight = false;

			this->comboOrganization->MaxDropDownItems = 1;
			//this->comboOrganization->DropDown += gcnew System::EventHandler(this, &QueryRiskEvt::comboOrganization_DropDown);



			this->dateTimePicker1->MaxDate = System::DateTime::Now.AddDays(-1);
			this->dateTimePicker1->Format = DateTimePickerFormat::Custom;
			this->dateTimePicker1->CustomFormat = "yyyy-MM-dd";

			this->dateTimePickerStart->MaxDate = System::DateTime::Now.AddDays(-1);
			this->dateTimePickerStart->Format = DateTimePickerFormat::Custom;
			this->dateTimePickerStart->CustomFormat = "yyyy-MM-dd";

			radioButton1->Checked = true;
			comboOrganization->Enabled = true;
			comboFinaProduct->Enabled = true;
			comboAccount->Enabled = false;



		 }

private: System::Void radioButton1_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
		 {
		 }
private: System::Void button_query_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 std::set<int> setOrganID;
			 std::set<int> setFinanProdID;
			 //std::map<int, RiskEvent> mapRiskEvent;
			 std::string strDateStart = Tools::String2string(this->dateTimePickerStart->Text->ToString());
			 std::string strDateEnd = Tools::String2string(this->dateTimePicker1->Text->ToString());
			 if(radioButton1->Checked)
			 {
				 String^ istrt=comboOrganization->Text;
				 if(String::IsNullOrEmpty(istrt))
				 {
					 String^ strtemp = NULL_QUERY_CONDITION;
					 MessageBox::Show(strtemp, "提示",MessageBoxButtons::OK);
				 }
				 else
				 {
					 bool bNoChecked = true;
					 bool bHasData = false;
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
					 /* String^ istrt=comboOrganization->Text;
					 const char*pchars = (const char*)(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(istrt)).ToPointer();
					 std::string strOrganization(pchars);
					 Runtime::InteropServices::Marshal::FreeHGlobal(IntPtr((void*)pchars));

					 String^ istrt2=comboFinaProduct->Text;
					 const char*pchars2 = (const char*)(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(istrt2)).ToPointer();
					 std::string strFinancial(pchars2);
					 Runtime::InteropServices::Marshal::FreeHGlobal(IntPtr((void*)pchars2));
					 std::set<int> setAccountID;
					 mapAccount.clear();*/

					 //请求查询风险事件

					 bHasData = setResult.size()>0;
					 if(bHasData)
					 {
						 m_QueryRiskEvt2->ReqQueryRiskEvt(setResult,strDateStart,strDateEnd );
						 dataGridView1->Rows->Clear();
					 }
					 else
						 MessageBox::Show("所设查询条件未筛选出任何帐号。", 
						 "提示", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				 }
			 }
			 if(radioButton2->Checked)
			 {
				 //String^ istrt=textBox_Account->Text;
				 //const char*pchars = (const char*)(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(istrt)).ToPointer();
				 //std::string strAccount(pchars);
				 //Runtime::InteropServices::Marshal::FreeHGlobal(IntPtr((void*)pchars));
				 //mapAccount.clear();
				 String^ istrt=comboAccount->Text;
				 if(String::IsNullOrEmpty(istrt))
				 {
					 String^ strtemp = NULL_QUERY_CONDITION;
					 MessageBox::Show(strtemp, "提示",MessageBoxButtons::OK);
				 }
				 else
				 {

					 std::set<int> setAccntID;
					 dlgOrganAndAccountIDTreeList->GetSelectID(setAccntID);
					 ////请求查询风险事件
					 m_QueryRiskEvt2->ReqQueryRiskEvt( setAccntID, strDateStart,strDateEnd );
					  dataGridView1->Rows->Clear();

					 ////测试代码
					 //std::map<int, RiskEvent> mapRiskEvent;
					 //RiskEvent RiskEvent2;
					 //memset(&RiskEvent2, 0, sizeof(RiskEvent2));

					 //RiskEvent* pRiskEvent =  &RiskEvent2;

					 //pRiskEvent->nRiskEventID			= 11;
					 //pRiskEvent->nTradeAccountID			= 1;	
					 //pRiskEvent->nRiskIndicatorID		= RI_MaxPosition;
					 //pRiskEvent->nRiskIndModuleID       = 0;
					 //pRiskEvent->lEventTime				= 811111111;			
					 //pRiskEvent->nRiskLevelID			= 111;
					 //pRiskEvent->nMsgSendStatus          = MsgStatus_New;
					 //pRiskEvent->nIsValid				= RiskEvent_Realtime;
					 //strcpy(pRiskEvent->InstrumentID, "abc");	
					 //mapRiskEvent[pRiskEvent->nRiskEventID]  =	*pRiskEvent;


					 //pRiskEvent->nRiskEventID			= 22;
					 //pRiskEvent->nTradeAccountID			= 2;		
					 //pRiskEvent->nRiskIndicatorID		= RI_MaxPosition;
					 //pRiskEvent->nRiskIndModuleID       = 1;
					 //pRiskEvent->lEventTime				= 82342554;			
					 //pRiskEvent->nRiskLevelID			= 222;
					 //pRiskEvent->nMsgSendStatus          = MsgStatus_Handling;
					 //pRiskEvent->nIsValid				= RiskEvent_Realtime;
					 //strcpy(pRiskEvent->InstrumentID, "abc");
					 //mapRiskEvent[pRiskEvent->nRiskEventID]  =	*pRiskEvent;

					 //pRiskEvent->nRiskEventID			= 33;
					 //pRiskEvent->nTradeAccountID			= 2;			
					 //pRiskEvent->nRiskIndicatorID		= RI_MaxPosition;
					 //pRiskEvent->nRiskIndModuleID       = 2;
					 //pRiskEvent->lEventTime				= 92342554;			
					 //pRiskEvent->nRiskLevelID			= 333;
					 //pRiskEvent->nMsgSendStatus          = MsgStatus_Delete;
					 //pRiskEvent->nIsValid				= RiskEvent_Realtime;
					 //strcpy(pRiskEvent->InstrumentID, "abc");	
					 //mapRiskEvent[pRiskEvent->nRiskEventID]  =	*pRiskEvent;

					 //pRiskEvent->nRiskEventID			= 44;
					 //pRiskEvent->nTradeAccountID			= 2;			
					 //pRiskEvent->nRiskIndicatorID		= RI_MaxPosition;
					 //pRiskEvent->nRiskIndModuleID       = 2;
					 //pRiskEvent->lEventTime				= 92342554;			
					 //pRiskEvent->nRiskLevelID			= 333;
					 //pRiskEvent->nMsgSendStatus          = MsgStatus_Delete;
					 //pRiskEvent->nIsValid				= RiskEvent_Realtime;
					 //strcpy(pRiskEvent->InstrumentID, "abc");	
					 //mapRiskEvent[pRiskEvent->nRiskEventID]  =	*pRiskEvent;

					 //pRiskEvent->nRiskEventID			= 55;
					 //pRiskEvent->nTradeAccountID			= 1;	
					 //pRiskEvent->nRiskIndicatorID		= RI_MaxPosition;
					 //pRiskEvent->nRiskIndModuleID       = 0;
					 //pRiskEvent->lEventTime				= 811111111;			
					 //pRiskEvent->nRiskLevelID			= 111;
					 //pRiskEvent->nMsgSendStatus          = MsgStatus_New;
					 //pRiskEvent->nIsValid				= RiskEvent_Realtime;
					 //strcpy(pRiskEvent->InstrumentID, "abc");	
					 //mapRiskEvent[pRiskEvent->nRiskEventID]  =	*pRiskEvent;

					 //pRiskEvent->nRiskEventID			= 66;
					 //pRiskEvent->nTradeAccountID			= 1;	
					 //pRiskEvent->nRiskIndicatorID		= RI_MaxPosition;
					 //pRiskEvent->nRiskIndModuleID       = 0;
					 //pRiskEvent->lEventTime				= 811111111;			
					 //pRiskEvent->nRiskLevelID			= 111;
					 //pRiskEvent->nMsgSendStatus          = MsgStatus_New;
					 //pRiskEvent->nIsValid				= RiskEvent_Realtime;
					 //strcpy(pRiskEvent->InstrumentID, "abc");	
					 //mapRiskEvent[pRiskEvent->nRiskEventID]  =	*pRiskEvent;

					 //loadDataGridView1(mapRiskEvent);
				 }
			 }
		 }
private: System::Void button_clear_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			  dataGridView1->Rows->Clear();
		 }

		string GetMsgStatusTypeString(MsgStatusType nMsgStatusType)
		{//得到风险事件状态
			string str ="";
			switch(nMsgStatusType)
			{
			case MsgStatus_New:
				str = STR_MsgStatus_New;
				break;
			case MsgStatus_ChangeLevel:
				str = STR_MsgStatus_ChangeLevel;
				break;
			case MsgStatus_Delete:
				str = STR_MsgStatus_Delete;
				break;
			case MsgStatus_Appoint:
				str = STR_MsgStatus_Appoint;
				break;
			case MsgStatus_Finished:
				str = STR_MsgStatus_Finished;
				break;
			case MsgStatus_ForceClose:
				str = STR_MsgStatus_ForceClose;
				break;
			default:break;
			}
			return str;
		}

		string GetRiskIndicatorIDString(RiskIndicatorType nRiskIndicatorID)
		{//得到风险指标的字符串
			string str ="";
			RiskIndicator	sRiskIndicator;
			if(m_pdataInfo && m_pdataInfo->GetRiskIndicator(nRiskIndicatorID, sRiskIndicator))
			{
				str = sRiskIndicator.szName;
			}
			return str;
		}

		 //重载WndProc，以处理WM_COMMAND消息
protected:virtual void WndProc( Message% m ) override
		  {
			  if (  m.Msg == WM_COMMAND_Win && m.WParam.ToInt32()==WndCmd_YourPkgArrival )
			  {

				  char *p = (char*)m.LParam.ToInt32();
				  Stru_UniPkgHead head;					
				  memset(&head, 0, sizeof(head));
				  memcpy(&head, p, sizeof(head));

				  if ( head.cmdid == Cmd_RM_QryRiskEvent_Rsp)//查询风险事件返回
				  {
					  if ( head.userdata1 == OPERATOR_SUCCESS )
					  {
						  int pkgLen=head.len;
						  int MessageCnt=pkgLen/sizeof(RiskEvent);
						  std::map<int, RiskEvent> mapRiskEvent;

						  bool brlt = m_QueryRiskEvt2->ProcessQueryRiskEvtPkg((const char*)m.LParam.ToInt32(),mapRiskEvent,MessageCnt);
						  if(brlt)
						  {
							  loadDataGridView1(mapRiskEvent);
							  //std::map<int, RiskEvent>::iterator it = mapRiskEvent.begin();

							  //String^ istrt=comboBox_status->Text;
							  //const char*pchars = (const char*)(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(istrt)).ToPointer();
							  //std::string strEvtStatus(pchars);
							  //Runtime::InteropServices::Marshal::FreeHGlobal(IntPtr((void*)pchars));
							  //int status;
							  //if (strEvtStatus == "发生")
								 // status = MsgStatus_New;
							  //else if (strEvtStatus == "处理中")
								 // status = MsgStatus_Handling;
							  //else if (strEvtStatus == "消失")
								 // status = MsgStatus_Delete;
							  //else 
								 // status = MsgStatus_All;

								 // switch(status)
								 // {
								 // case MsgStatus_New:      //发生:
									//  for(it;it!=mapRiskEvent.end();it++)
							  //         {
									//	   if(it->second.nMsgSendStatus != MsgStatus_New)
									//		   mapRiskEvent.erase(it);
									//  }
									//		  									 
									//  break;
								 // case MsgStatus_Handling:	//处理中

									//  for(it;it!=mapRiskEvent.end();it++)
							  //         {
									//	   if(it->second.nMsgSendStatus != MsgStatus_Handling)
									//		   mapRiskEvent.erase(it);
									//  }

									//  break;
								 // case MsgStatus_Delete:	//消失
									//  for(it;it!=mapRiskEvent.end();it++)
							  //         {
									//	   if(it->second.nMsgSendStatus != MsgStatus_Delete)
									//		   mapRiskEvent.erase(it);
									//  }
		 
									//  break;
								 // default:
									//
									//  break;

								 // }
								 // int newRowIndex=dataGridView1->Rows->Add();
								 // DataGridViewRow^ dr = dataGridView1->Rows[newRowIndex];
								 // dr->Cells[0]->Value = it->second.nRiskEventID;//风险事件ID
								 // TrustTradeAccount account;
								 // memset(&account,0,sizeof(account));
								 // m_pdataInfo->GetTrustTradeAccount(it->second.nTradeAccountID,account);

								 // dr->Cells[1]->Value = Tools::string2String(account.szAccount);//帐号

								 // AssetMgmtOrganization organ;
								 // memset(&organ, 0, sizeof(organ));
								 // if(m_pdataInfo->GetAssetMgmtOrg(account.nAssetMgmtOrgID, organ))
									//  dr->Cells[2]->Value = Tools::string2String(organ.szName);//账号所属资管
								 // FinancialProduct finanProduct;
								 // memset(&finanProduct, 0, sizeof(finanProduct));
								 // if(m_pdataInfo->GetFinancialProduct(account.nFinancialProductID, finanProduct))
									//  dr->Cells[3]->Value = Tools::string2String(finanProduct.szName);//账号所属理财产品

								 // tm *pTmp = localtime(&(it->second.lEventTime));
								 // char pszTime[20];
								 // sprintf(pszTime, "%d-%d-%d %d:%d:%d", pTmp->tm_year + 1900, pTmp->tm_mon + 1, pTmp->tm_mday, pTmp->tm_hour, pTmp->tm_min, pTmp->tm_sec);
								 // dr->Cells[4]->Value = Tools::string2String(pszTime);//日期时间

								 // RiskIndicatorModule  riskModule;
								 // memset(&riskModule,0,sizeof(riskModule));
								 // if(m_pdataInfo->GetRiskIndicatorModule(it->second.nRiskIndModuleID, riskModule))
									//  dr->Cells[5]->Value = Tools::string2String(riskModule.szName);//风险指标模型
								 // RiskIndicator  riskInd;
								 // memset(&riskInd,0,sizeof(riskInd));
								 // if(m_pdataInfo->GetRiskIndicator(it->second.nRiskIndicatorID, riskInd))
									//  dr->Cells[6]->Value = Tools::string2String(riskInd.szName);//风险指标

								 // dr->Cells[7]->Value = Tools::string2String(it->second.InstrumentID);//合约ID   


								 // char strlevel[12];
								 // sprintf(strlevel, "%d", it->second.nRiskLevelID);
								 // dr->Cells[8]->Value = Tools::string2String(strlevel);//风险等级

								 // char strlIndicatorValue[128];
								 // sprintf(strlIndicatorValue, "%0.2f", it->second.dblIndicatorValue);
								 // dr->Cells[9]->Value = Tools::string2String(strlIndicatorValue);//风险指标值

								 // dr->Cells[10]->Value = Tools::string2String(strEvtStatus.c_str());//状态

								 // switch(it->second.nIsValid)
								 // {
								 // case RiskEvent_History:      //历史风险事件
									//  dr->Cells[11]->Value = L"历史风险事件";
									//		  									 
									//  break;
								 // case RiskEvent_Realtime:	//实时风险事件

									// dr->Cells[11]->Value = L"实时风险事件";

									//  break;

								 // default:
									//
									//  break;

								 // }

								 




					  }
					  }
					  else
							{
								string str = (char*)(p+sizeof(Stru_UniPkgHead));
								String^ strtemp = Tools::string2String(str.c_str());
								MessageBox::Show(strtemp, "提示",MessageBoxButtons::OK);
							}
				  }
			  }

			  Form::WndProc ( m );
		  }
private: System::Void loadDataGridView1(std::map<int, RiskEvent>& mapRiskEvent)
		 {
			

			 std::map<int, RiskEvent>::iterator it = mapRiskEvent.begin();


			 String^ istrt=comboBox_status->Text;
			 const char*pchars = (const char*)(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(istrt)).ToPointer();
			 std::string strEvtStatus(pchars);
			 Runtime::InteropServices::Marshal::FreeHGlobal(IntPtr((void*)pchars));
			 int status;
			 if (strEvtStatus == STR_MsgStatus_New)
				 status = MsgStatus_New;
			 else if (strEvtStatus == STR_MsgStatus_ChangeLevel)
				 status = MsgStatus_ChangeLevel;
			 else if (strEvtStatus == STR_MsgStatus_Delete)
				 status = MsgStatus_Delete;
			 else if (strEvtStatus == STR_MsgStatus_Appoint)
				 status = MsgStatus_Appoint;
			 else if (strEvtStatus == STR_MsgStatus_Finished)
				 status = MsgStatus_Finished;
			 else if (strEvtStatus == STR_MsgStatus_ChangeLevel)
				 status = MsgStatus_ChangeLevel;
			 else if (strEvtStatus == STR_MsgStatus_ForceClose)
				 status = MsgStatus_ForceClose;
			 else 
				 status = MsgStatus_All;

			 switch(status)
			 {
			 case MsgStatus_New:      //发生:
				 for(it;it!=mapRiskEvent.end();)
				 {
					 if(it->second.nMsgSendStatus != MsgStatus_New)
						 mapRiskEvent.erase(it++);
					 else
						 ++it;
				 }

				 break;
			 case MsgStatus_ChangeLevel:	//等级变更

				 for(it;it!=mapRiskEvent.end();)
				 {
					 if(it->second.nMsgSendStatus != MsgStatus_ChangeLevel)
						 mapRiskEvent.erase(it++);
					 else
						 ++it;
				 }

				 break;
			 case MsgStatus_Delete:	//消失
				 for(it;it!=mapRiskEvent.end();)
				 {
					 if(it->second.nMsgSendStatus != MsgStatus_Delete)
						 mapRiskEvent.erase(it++);
					 else
						 ++it;
				 }

				 break;
			case MsgStatus_Appoint:      //手动操作，委派给交易员:
				 for(it;it!=mapRiskEvent.end();)
				 {
					 if(it->second.nMsgSendStatus != MsgStatus_Appoint)
						 mapRiskEvent.erase(it++);
					 else
						 ++it;
				 }

				 break;
			 case MsgStatus_Finished:	//手动操作，风险事件解决记录

				 for(it;it!=mapRiskEvent.end();)
				 {
					 if(it->second.nMsgSendStatus != MsgStatus_Finished)
						 mapRiskEvent.erase(it++);
					 else
						 ++it;
				 }

				 break;
			 case MsgStatus_ForceClose:	//手动操作，执行手动强平记录
				 for(it;it!=mapRiskEvent.end();)
				 {
					 if(it->second.nMsgSendStatus != MsgStatus_ForceClose)
						 mapRiskEvent.erase(it++);
					 else
						 ++it;
				 }

				 break;
			 default:

				 break;

			 }
			 for(it = mapRiskEvent.begin(); it != mapRiskEvent.end(); it++)
			 {
				 int newRowIndex=dataGridView1->Rows->Add();
				 DataGridViewRow^ dr = dataGridView1->Rows[newRowIndex];
				 dr->Cells[0]->Value = it->second.nRiskEventID;//风险事件ID
				 dr->Cells[0]->Tag= it->second.nRiskEventSubID;//风险事件SubID
				 TrustTradeAccount account;
				 memset(&account,0,sizeof(account));
				 m_pdataInfo->GetTrustTradeAccount(it->second.nTradeAccountID,account);

				 dr->Cells[1]->Value = Tools::string2String(account.szAccount);//帐号

				 AssetMgmtOrganization organ;
				 memset(&organ, 0, sizeof(organ));
				 if(m_pdataInfo->GetAssetMgmtOrg(account.nAssetMgmtOrgID, organ))
					 dr->Cells[2]->Value = Tools::string2String(organ.szName);//账号所属资管
				 FinancialProduct finanProduct;
				 memset(&finanProduct, 0, sizeof(finanProduct));
				 if(m_pdataInfo->GetFinancialProduct(account.nFinancialProductID, finanProduct))
					 dr->Cells[3]->Value = Tools::string2String(finanProduct.szName);//账号所属理财产品

				 tm *pTmp = localtime(&(it->second.lEventTime));
				 char pszTime[20];
				 sprintf(pszTime, "%4d-%02d-%02d %02d:%02d:%02d", pTmp->tm_year + 1900, pTmp->tm_mon + 1, pTmp->tm_mday, pTmp->tm_hour, pTmp->tm_min, pTmp->tm_sec);
				 dr->Cells[4]->Value = Tools::string2String(pszTime);//日期时间
				 dr->Cells[4]->Tag = it->second.lEventTime;

				 RiskIndicatorModule  riskModule;
				 memset(&riskModule,0,sizeof(riskModule));
				 if(m_pdataInfo->GetRiskIndicatorModule(it->second.nRiskIndModuleID, riskModule))
					 dr->Cells[5]->Value = Tools::string2String(riskModule.szName);//风险指标模型
				 //RiskIndicator  riskInd;
				 //memset(&riskInd,0,sizeof(riskInd));
				 //if(m_pdataInfo->GetRiskIndicator(it->second.nRiskIndicatorID, riskInd))
				 string strRiskIndicator = GetRiskIndicatorIDString(it->second.nRiskIndicatorID);
					 dr->Cells[6]->Value = Tools::string2String(strRiskIndicator.c_str());//风险指标

				 dr->Cells[7]->Value = Tools::string2String(it->second.InstrumentID);//合约ID   


				 char strlevel[12];
				 sprintf(strlevel, "%d", it->second.nRiskLevelID);
				 dr->Cells[8]->Value = Tools::string2String(strlevel);//风险等级

				 char strlIndicatorValue[128];
				 sprintf(strlIndicatorValue, "%f", it->second.dblIndicatorValue);
				 dr->Cells[9]->Value = Tools::string2String(strlIndicatorValue);//风险指标值

				 string strMsgSendStatus=GetMsgStatusTypeString(it->second.nMsgSendStatus);
                 
				 dr->Cells[10]->Value = Tools::string2String(strMsgSendStatus.c_str());//状态

				 switch(it->second.nIsValid)
				 {
				 case RiskEvent_History:      //历史风险事件
					 dr->Cells[11]->Value = L"历史风险事件";

					 break;
				 case RiskEvent_Realtime:	//实时风险事件

					 dr->Cells[11]->Value = L"实时风险事件";

					 break;

				 default:

					 break;

				 }
			 }
		 }
private: System::Void dataGridView1_CellDoubleClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) 
		 {
			 if(dlgRiskRecord != nullptr)
			 {
				 delete dlgRiskRecord;
				 dlgRiskRecord = nullptr;
			 }
			 dlgRiskRecord = gcnew RiskRecord();
			  String^ riskEvtId = dataGridView1->Rows[e->RowIndex]->Cells[0]->Value->ToString();
			  int nRiskEvtId = atoi(Tools::String2string(riskEvtId).c_str());
			  TrustTradeAccount account;
			  memset(&account,0,sizeof(account));
			  String^ szAccount = dataGridView1->Rows[e->RowIndex]->Cells[1]->Value->ToString();
			  m_pdataInfo->GetTrustTradeAccount(Tools::String2string(szAccount).c_str(),account);
			  RiskEventHandlingQueryKey riskEvtHandleKey;
			  memset(&riskEvtHandleKey,0,sizeof(riskEvtHandleKey));
			  riskEvtHandleKey.nRiskEventID = nRiskEvtId;
			  //riskEvtHandleKey.nRiskEventSubID = atoi(Tools::String2string(dataGridView1->Rows[e->RowIndex]->Cells[0]->Tag->ToString()).c_str());
			 // strncpy(riskEvtHandleKey.szDate ,Tools::String2string(dataGridView1->Rows[e->RowIndex]->Cells[4]->Value->ToString()).c_str(),sizeof(riskEvtHandleKey.szDate)-1);
			 dlgRiskRecord->SetRiskRecord(riskEvtHandleKey);
			 dlgRiskRecord->Show();

		 }
private: System::Void dataGridView1_RowDividerDoubleClick(System::Object^  sender, System::Windows::Forms::DataGridViewRowDividerDoubleClickEventArgs^  e) {
		 }
private: System::Void radioButton1_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 comboOrganization->Enabled = true;
			 comboFinaProduct->Enabled = true;
			 comboAccount->Enabled = false;
		 }
private: System::Void radioButton2_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 comboOrganization->Enabled = false;
			 comboFinaProduct->Enabled = false;
			 comboAccount->Enabled = true;
		 }
private: System::Void QueryRiskEvt_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) 
		 {
			 IntPtr hWnd=this->Handle;
			 CTcpLayer::SubscribePkg( Cmd_RM_QryRiskEvent_Rsp ,(int)(hWnd.ToInt32()));
		 }
};
}
